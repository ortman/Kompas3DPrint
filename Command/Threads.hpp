#ifndef _Kompas3DPrint_RealThreads_hpp_
#define _Kompas3DPrint_RealThreads_hpp_
#include "../Kompas/Kompas3D.h"

class Threads {
public:
	double clearance = 0.06;
	double minThreadDiameter = 0.0;
	
	void Start() {
		Doc3D doc = Kompas3D::GetActiveDocument3D();
		if (!doc) return;
		Part topPart = doc.GetTopPart();
		
		// Удаляем макрос
		for (Node& node : topPart.GetNodes()) {
			if (node.IsType(NodeMacro::TYPE) && node.GetName() == "Реальные резьбы") {
				topPart.Remove(node);
			}
		}
		
		NodeMacro macro = topPart.Create<NodeMacro>(false, "Реальные резьбы");
		for (Node& node : topPart.GetNodes()) {
			if (node.IsType(ThreadDesignation::TYPE)) {
				CreateThread(topPart, macro, ThreadDesignation(node));
			}
		}
		macro.Update();
	}

private:
	bool CreateThread(Part& part, NodeMacro& macro, const ThreadDesignation& thread) {
		Edge beginEdge = thread.GetBeginEdge();
		
		Face planarFace = beginEdge.RightFace();
		Face cylinderFace = beginEdge.LeftFace();
		if (!planarFace.IsPlanar()) {
			planarFace = beginEdge.LeftFace();
			cylinderFace = beginEdge.RightFace();
		}
		if (!planarFace.IsPlanar() || !cylinderFace.IsCylinder()) return false;
		Vertex::Point3D c = beginEdge.GetOrigin();
		
		// Базовая плоскость спирали
		ParallelPlane plt = part.Create<ParallelPlane>(planarFace, beginEdge.GetBeginVertex());
		Plane::Point2D loc = plt.Projection(c);
		Plane::Point2D c2d = plt.Projection({0., 0., 0.});
		
		// Создаём спираль
		CylindricSpiral helix = part.Create<CylindricSpiral>();
		//helix.name = "Спираль_{0}".format(opNum)
		helix.SetDiam(thread.GetDiameter())
		     .SetStep(thread.GetPitch())
		     .SetHeight(thread.GetLength())
		     .TurnDir(thread.IsForwardDir())
		     .BuildDir(false)
		     .BuildMode(CylindricSpiral::ModeStepHeight)
		     .SetPlane(plt)
		     .SetLocation(loc.x - c2d.x, loc.y - c2d.y)
		     .Create();
		
		// Ось резьбы
		ConeAxis axis = part.Create<ConeAxis>(cylinderFace);
		
		// Плоскость эскиза
		Vertex startVertex = helix.GetBeginVertex();
		EdgePointPlane sketchPlane = part.Create<EdgePointPlane>(axis, startVertex);
		
		// Профиль резьбы
		Sketch sketch = part.Create<Sketch>(sketchPlane);
		double step = thread.GetPitch() * 0.99;
		Vertex::Point3D s = startVertex;
		Vertex::Point3D s1 = vecSumm(plt, s, {0, 0, step});
		Plane::Point2D a2d = plt.Projection(axis.GetFirstPoint());
		Vertex::Point3D a3d = plt.Projection(a2d);
		Plane::Point2D p0 = sketch.Projection(s);
		Plane::Point2D p1 = sketch.Projection(s1);
		Plane::Point2D p2 = sketch.Projection(a3d);
		
		bool xyInvert = abs(p1.x - p0.x) > 0.1;
		bool xPositive = xyInvert ? p2.y > p0.y : p2.x > p0.x;
		bool yPositive = xyInvert ? p1.x > p0.x : p1.y > p0.y;
		
		drawThreadSketch(sketch, p0.x, p0.y, step, xyInvert, xPositive, yPositive, thread.IsOutside(), 0);
		
		CutEvolution cut = part.Create<CutEvolution>(sketch, helix);
		
		// Фаска резьбы
		Sketch chamferSketch = part.Create<Sketch>(sketchPlane);
		
		Plane::Point2D a1 = sketch.Projection(axis.GetFirstPoint());
		Plane::Point2D a2 = sketch.Projection(axis.GetLastPoint());
		chamferSketch.Line(a1.x, a1.y, a2.x, a2.y, LineStyle::Axial);
		drawChamferSketch(chamferSketch, p0.x, p0.y, step, xyInvert, xPositive, yPositive, thread.IsOutside(), 0);
		drawThreadSketch(chamferSketch, p0.x, p0.y, step, xyInvert, xPositive, yPositive, thread.IsOutside(), -thread.GetLength());
		
		CutRotated chamferCut = part.Create<CutRotated>(chamferSketch, axis);
		
		macro.Add(plt)
		     .Add(helix)
		     .Add(axis)
		     .Add(sketchPlane)
		     .Add(sketch)
		     .Add(cut)
		     .Add(chamferSketch)
		     .Add(chamferCut);
		return true;
	}

	// Эскиз профиля резьбы
	void drawThreadSketch(Sketch& sketch, double x0, double y0, double step, bool xyInvert, bool xPositive, bool yPositive, bool outside, double shiftY) {
		if (!yPositive) {
			step = -step;
			shiftY = -shiftY;
		}
		double h = abs((sqrt(3.) / 2.) * step);
		if (xPositive) h = -h;
		if (outside) {
			h = -h;
			x0 = x0 + (xPositive ? clearance : -clearance) - h / 8.;
		} else {
			if (xyInvert) {
				y0 -= h * 7. / 8. + clearance;
			} else {
				x0 -= h * 7. / 8. + clearance;
			}
		}
		if (xyInvert) {
			sketch.Line(x0 + shiftY, y0, x0 + shiftY + step, y0);
			sketch.Line(x0 + shiftY + step, y0, x0 + shiftY + step / 2., y0 + h);
			sketch.Line(x0 + shiftY + step / 2., y0 + h, x0 + shiftY, y0);
		} else {
			sketch.Line(x0, y0 + shiftY, x0, y0 + shiftY + step);
			drawRoundedCorner(sketch, x0, y0 + shiftY, x0 + h, y0 + shiftY + step / 2., x0, y0 + shiftY + step, abs(step / (outside ? 6. : 12.)));
		}
	}

	// Эскиз файски резьбы
	void drawChamferSketch(Sketch& sketch, double x0, double y0, double step, bool xyInvert, bool xPositive, bool yPositive, bool outside, double shiftY) {
		if (!yPositive) {
			step = -step;
			shiftY = -shiftY;
		}
		double h = abs(step);
		if (xPositive) h = -h;
		if (outside) {
			h = -h;
			x0 = x0 + (xPositive ? clearance : -clearance) - h / 8.;
		} else {
			if (xyInvert) {
				y0 = y0 - h * 7. / 8. + clearance;
			} else {
				x0 = x0 - h * 7. / 8. + clearance;
			}
		}
		if (xyInvert) {
			sketch.Line(x0 + shiftY, y0, x0 + shiftY - step, y0);
			sketch.Line(x0 + shiftY - step, y0, x0 + shiftY, y0 + h);
			sketch.Line(x0 + shiftY, y0 + h, x0 + shiftY, y0);
		} else {
			sketch.Line(x0, y0 + shiftY, x0, y0 + shiftY - step);
			sketch.Line(x0, y0 + shiftY - step, x0 + h, y0 + shiftY);
			sketch.Line(x0 + h, y0 + shiftY, x0, y0 + shiftY);
		}
	}

	void drawRoundedCorner(Sketch& sketch, double x1, double y1, double x2, double y2, double x3, double y3, double r) {
		// 1. Вычисление векторов отсекающих лучей из вершины (x2, y2)
		double v1_x = x1 - x2;
		double v1_y = y1 - y2;
		double v2_x = x3 - x2;
		double v2_y = y3 - y2;
		
		// Длины векторов
		double len1 = hypot(v1_x, v1_y);
		double len2 = hypot(v2_x, v2_y);
		
		if (len1 < 0.0001 || len2 < 0.0001 || r < 0.0001) {
			// Если длины нулевые или радиус равен 0, рисуем обычные отрезки
			sketch.Line(x1, y1, x2, y2);
			sketch.Line(x2, y2, x3, y3);
			return;
		}
		
		// Нормализация векторов
		double u1_x = v1_x / len1;
		double u1_y = v1_y / len1;
		double u2_x = v2_x / len2;
		double u2_y = v2_y / len2;
		
		// 2. Расчет угла между отрезками
		double dot_product = u1_x * u2_x + u1_y * u2_y;
		dot_product = max(-1.0, min(1.0, dot_product)); // Защита от погрешности float
		double alpha = acos(dot_product);
		
		if (alpha < 0.0001 || alpha >= M_PI) {
			// Если отрезки лежат на одной прямой, скругление невозможно
			sketch.Line(x1, y1, x3, y3);
			return;
		}
		
		// 3. Расчет расстояния от вершины до точек касания (Тангенс угла)
		// Угол в прямоугольном треугольнике равен половине угла между векторами
		double t_len = r / tan(alpha / 2.0);
		
		if (t_len > len1 || t_len > len2) {
			throw Kompas3DException("Радиус скругления слишком велик для заданных отрезков.");
		}
		
		// 4. Нахождение точек сопряжения (начала и конца дуги)
		double p1_x = x2 + u1_x * t_len;
		double p1_y = y2 + u1_y * t_len;
		
		double p2_x = x2 + u2_x * t_len;
		double p2_y = y2 + u2_y * t_len;
		
		// 5. Нахождение средней точки дуги для функции ksArcBy3Points
		// Направляющий вектор биссектрисы угла
		double bis_x = u1_x + u2_x;
		double bis_y = u1_y + u2_y;
		double bis_len = hypot(bis_x, bis_y);
		
		if (bis_len < 0.0001) return;
		
		double bis_nx = bis_x / bis_len;
		double bis_ny = bis_y / bis_len;
		
		// Расстояние от вершины угла до центра окружности
		double dist_to_center = r / sin(alpha / 2.0);
		
		// Расстояние от вершины до средней точки дуги
		double dist_to_mid_arc = dist_to_center - r;
		
		// Координаты средней точки дуги
		double mid_x = x2 + bis_nx * dist_to_mid_arc;
		double mid_y = y2 + bis_ny * dist_to_mid_arc;
		
		// 6. Отрисовка усеченных отрезков и дуги сопряжения
		sketch.Line(x1, y1, p1_x, p1_y);          // Первый усеченный отрезок
		sketch.ArcBy3Points(p1_x, p1_y, mid_x, mid_y, p2_x, p2_y); // Дуга скругления
		sketch.Line(p2_x, p2_y, x3, y3);          // Второй усеченный отрезок
	}

	Vertex::Point3D vecSumm(Plane& pl, Vertex::Point3D p1, Vertex::Point3D p2) {
		Vertex::Point3D vx = pl.GetVector(Plane::OX);
		Vertex::Point3D vy = pl.GetVector(Plane::OY);
		Vertex::Point3D vz = pl.GetVector(Plane::OZ);
		double gx = p1.x + p2.x * vx.x + p2.y * vy.x + p2.z * vz.x;
		double gy = p1.y + p2.x * vx.y + p2.y * vy.y + p2.z * vz.y;
		double gz = p1.z + p2.x * vx.z + p2.y * vy.z + p2.z * vz.z;
		return {gx, gy, gz};
	}
};

#endif
