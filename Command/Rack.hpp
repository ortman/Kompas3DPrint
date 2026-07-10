#ifndef _Kompas3DPrint_Rack_hpp_
#define _Kompas3DPrint_Rack_hpp_
#include "Kompas/Kompas3D.h"

class Rack {
private:
	struct RackParameters {
	    double m;
	    double length;
	    double think;
	    double depth;
	};
	
	Panel* panel;
	Panel::Property* propModule;
	Panel::Property* propLength;
	Panel::Property* propThink;
	Panel::Property* propDepth;
	NodeMacro edit = NodeMacro(nullptr);
	
public:
	Rack() {
		try {
			if (!Kompas3D::Connect()) return;
			static Panel rackPanel = Kompas3D::CreatePanel("Параметры рейки");
			panel = &rackPanel;
			Panel::Tab& rackTab = rackPanel.CreateTab("Параметры");
			static Panel::Property& moduleProp = rackTab.CreateProperty("Модуль", 0.8);
			propModule = &moduleProp;
			static Panel::Property& lengthProp = rackTab.CreateProperty("Длина", 100);
			propLength = &lengthProp;
			static Panel::Property& thinkProp  = rackTab.CreateProperty("Толщина", 5);
			propThink = &thinkProp;
			static Panel::Property& depthProp  = rackTab.CreateProperty("Глубина", 8);
			propDepth = &depthProp;
			
			rackPanel.WhenButtonClick = [=](int buttonId) {
				try {
					if (buttonId == 1) {
						if (edit) {
							ReplaceRack(moduleProp, lengthProp, thinkProp, depthProp);
						} else {
							CreateRack(moduleProp, lengthProp, thinkProp, depthProp);
						}
					}
					edit = NodeMacro(nullptr);
					rackPanel.Hide();
				} catch (const Kompas3DException& e) {
					Kompas3D::Error(e.what());
				}
				return false;
			};
		} catch (const Kompas3DException&) {
		}
	}
	
	void Start() {
		if (!panel) throw new Kompas3DException("Панель свойств рейки отсутствует");
		auto doc = Kompas3D::GetActiveDocument3D();
		if (!doc) {
			Kompas3D::Error("Не найден активный 3D документ");
			return;
		}
		edit = doc->GetEditMacroObject();
		if (edit) {
			RackParameters rackParam;
			if (edit.GetUserParam(&rackParam, sizeof(rackParam))) {
				*propModule = rackParam.m;
				*propLength = rackParam.length;
				*propThink = rackParam.think;
				*propDepth = rackParam.depth;
			}
		}
		panel->Show();
	}
	
private:
	void CreateRack(double m, double length, double think, double depth) {
		auto doc = Kompas3D::GetActiveDocument3D();
		if (!doc) return;
		Part topPart = doc->GetTopPart();
		std::ostringstream name;
		name << "Рейка " << m << " x " << length;
		NodeMacro macro = topPart.Create<NodeMacro>(false, name.str());
		RackParameters rackParam = {m, length, think, depth};
		macro.SetUserParam(&rackParam, sizeof(rackParam), MENU_RACK);
		
		Sketch sketchBlank = topPart.Create<Sketch>(topPart.GetPlaneXOY(), "BlankSketch");
		sketchBlank.Rect(0, 0, length, depth);
		BaseExtrusion blank = topPart.Create<BaseExtrusion>(sketchBlank, think, 0, "Blank");
		
		Sketch sketchToth = topPart.Create<Sketch>(topPart.GetPlaneXOY(), "TothSketch");
		DrawTothSketch(sketchToth, m);
		CutExtrusion toth = topPart.Create<CutExtrusion>(sketchToth, 0, think, "TothCut");
		
		double step = M_PI * m;
		MeshCopy tothArray = topPart.Create<MeshCopy>((int)(length / step) + 1, step, 1, 0, std::move(toth), "TothCutArray");
		
		macro.Add(sketchBlank)
			.Add(blank)
			.Add(sketchToth)
			.Add(toth)
			.Add(tothArray)
			.Update();
	}
	
	void ReplaceRack(double m, double length, double think, double depth) {
		if (!edit) return;
		Sketch sketchBlank(nullptr);
		BaseExtrusion blank(nullptr);
		Sketch sketchToth(nullptr);
		CutExtrusion toth(nullptr);
		MeshCopy tothArray(nullptr);
		for (auto& node : edit.GetNodes()) {
			if (node.IsType(Sketch::TYPE) && node.GetName() == "BlankSketch") {
				sketchBlank = Sketch(node);
			} else if ((node.IsType(BaseExtrusion::TYPE)
				|| node.IsType(25))
				&& node.GetName() == "Blank") {
				blank = BaseExtrusion(node);
			} else if (node.IsType(Sketch::TYPE) && node.GetName() == "TothSketch") {
				sketchToth = Sketch(node);
			} else if (node.IsType(CutExtrusion::TYPE) && node.GetName() == "TothCut") {
				toth = CutExtrusion(node);
			} else if (node.IsType(MeshCopy::TYPE) && node.GetName() == "TothCutArray") {
				tothArray = MeshCopy(node);
			}
		}
		if (!sketchBlank || !blank || !sketchToth || !toth || !tothArray) return;
		RackParameters rackParam = {m, length, think, depth};
		edit.SetUserParam(&rackParam, sizeof(rackParam), MENU_RACK);
		std::ostringstream name;
		name << "Рейка " << m << " x " << length;
		edit.SetName(name.str());
		sketchBlank.Clear().Rect(0, 0, length, depth).EndEdit();
		blank.SetDepth1(think);
		DrawTothSketch(sketchToth.Clear(), m);
		sketchToth.EndEdit();
		toth.SetDepth2(think);
		double step = M_PI * m;
		tothArray.SetParam1((int)(length / step) + 1, step);
		edit.Hide().Update();
	}
	
	void DrawTothSketch(Sketch& sketch, double m) {
	    // 1. Основные параметры геометрии рейки (ГОСТ 13755)
	    // Переводим 20 градусов в радианы
	    double alpha = 20.0 * M_PI / 180.0;
	
	    double ha = 1.0 * m;         // Высота головки
	    double hf = 1.25 * m;        // Высота ножки
	
	    // Полный шаг рейки P = pi * m. Смещение центра впадины относительно пика зуба
	    double pitch = M_PI * m;
	    double x_offset = pitch / 2.0;    // Смещаем центр впадины вправо на полшага
	
	    // Координаты уровней по оси Y (0 - вершина зуба, контур идет вниз)
	    double y_top = 0.0;
	    double y_bot = -(ha + hf);        // Дно впадины (полная высота зуба рейки)
	
	    // Ширина впадины по делительной линии (на глубине -ha от пика)
	    double half_pitch_width = pitch / 4.0;
	    double y_del = -ha;
	
	    // Радиус скругления (галтели) на дне впадины
	    double rc = 0.38 * m;
	
	    // ================= РАСЧЕТ КЛЮЧЕВЫХ ТОЧЕК ПРОФИЛЯ =================
	
	    // Координаты правого и левого краев впадины на делительной линии с учетом смещения
	    double x_zero_r = x_offset + half_pitch_width;
	    double x_zero_l = x_offset - half_pitch_width;
	
	    // Точка 3: На вершине рейки (y = y_top)
	    double x3_v = x_zero_r + (y_top - y_del) * std::tan(alpha);
	    double y3_v = y_top;
	
	    double x3_v_l = x_zero_l - (y_top - y_del) * std::tan(alpha);
	    double y3_v_l = y_top;
	
	    // Точка 1: Теоретическая точка пересечения боковой стенки и дна (y = y_bot)
	    double x1_v = x_zero_r + (y_bot - y_del) * std::tan(alpha);
	    double y1_v = y_bot;
	
	    // ================= МАТЕМАТИЧЕСКИЙ РАСЧЕТ ГАЛТЕЛЕЙ =================
	
	    // Центр окружности скругления для правого бока
	    double yc_c_r = y_bot + rc;
	    double xc_c_r = x1_v - rc * std::tan((45.0 * M_PI / 180.0) - alpha / 2.0);
	
	    // Точки сопряжения галтели для правой стороны
	    double x_touch_bot_r = xc_c_r;
	    double y_touch_bot_r = y_bot;
	
	    double x_touch_wall_r = xc_c_r + rc * std::sin(alpha);
	    double y_touch_wall_r = yc_c_r - rc * std::cos(alpha);
	
	    // Центр окружности скругления для левого бока
	    // Так как ось симметрии впадины теперь x = x_offset, зеркалируем относительно нее
	    double xc_c_l = x_offset - (xc_c_r - x_offset);
	    double yc_c_l = yc_c_r;
	
	    // Точки сопряжения галтели для левого стороны
	    double x_touch_bot_l = x_offset - (x_touch_bot_r - x_offset);
	    double y_touch_bot_l = y_bot;
	
	    double x_touch_wall_l = x_offset - (x_touch_wall_r - x_offset);
	    double y_touch_wall_l = y_touch_wall_r;
	
	    // --- Геометрический расчет средних точек дуг для ksArcBy3Points ---
	    // Правая галтель: проецируем среднее направление на радиус rc от центра
	    double mx_r = ((x_touch_bot_r + x_touch_wall_r) / 2.0) - xc_c_r;
	    double my_r = ((y_touch_bot_r + y_touch_wall_r) / 2.0) - yc_c_r;
	    double len_m_r = std::hypot(mx_r, my_r);
	
	    double x_mid_r = (len_m_r > 0.0) ? (xc_c_r + (mx_r / len_m_r) * rc) : mx_r;
	    double y_mid_r = (len_m_r > 0.0) ? (yc_c_r + (my_r / len_m_r) * rc) : my_r;
	
	    // Левая галтель: зеркалируем правую среднюю точку относительно оси впадины
	    double x_mid_l = x_offset - (x_mid_r - x_offset);
	    double y_mid_l = y_mid_r;
	    
	    // 1. Плоское дно впадины (между галтелями)
	    sketch.Line(x_touch_bot_l, -y_touch_bot_l, x_touch_bot_r, -y_touch_bot_r);
	
	    // 2. Правая галтель (скругление от дна к наклонной стенке)
	    sketch.ArcBy3Points(x_touch_bot_r, -y_touch_bot_r, x_mid_r, -y_mid_r, x_touch_wall_r, -y_touch_wall_r);
	
	    // 3. Левая галтель (скругление от наклонной стенки к дну)
	    sketch.ArcBy3Points(x_touch_wall_l, -y_touch_wall_l, x_mid_l, -y_mid_l, x_touch_bot_l, -y_touch_bot_l);
	
	    // 4. Правая наклонная стенка впадины
	    sketch.Line(x_touch_wall_r, -y_touch_wall_r, x3_v, -y3_v);
	
	    // 5. Левая наклонная стенка впадины
	    sketch.Line(x3_v_l, -y3_v_l, x_touch_wall_l, -y_touch_wall_l);
	
	    // 6. Замыкающая верхняя линия (соединяет левую и правую вершины выреза)
	    sketch.Line(x3_v, -y3_v, x3_v_l, -y3_v_l);
	}
};

#endif
