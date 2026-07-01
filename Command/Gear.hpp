#ifndef _Kompas3DPrint_Gear_hpp_
#define _Kompas3DPrint_Gear_hpp_
#include "Kompas/Kompas3D.h"
#include <sstream>

class Gear {
private:
	struct GearParameters {
	    double m;
	    int count;
	    double think;
	};

	Panel* panel;
	Panel::Property* propModule;
	Panel::Property* propCount;
	Panel::Property* propThink;
	std::unique_ptr<NodeMacro> edit;

public:
	Gear() {
		try {
			if (!Kompas3D::Connect()) return;
			static Panel gearPanel = Kompas3D::CreatePanel("Параметры шестерни");
			panel = &gearPanel;
			static Panel::Tab& gearTab = gearPanel.CreateTab("Параметры");
			static Panel::Property& moduleProp = gearTab.CreateProperty("Модуль", 0.8);
			propModule = &moduleProp;
			static Panel::Property& countProp  = gearTab.CreateProperty("Количество", 20);
			propCount = &countProp;
			static Panel::Property& thinkProp  = gearTab.CreateProperty("Толщина", 5);
			propThink = &thinkProp;
		
			gearPanel.WhenButtonClick = [=](int buttonId) {
				try {
					if (buttonId == 1) {
						if (edit) {
							ReplaceGear(moduleProp, (int)countProp, thinkProp);
						} else {
							CreateGear(moduleProp, (int)countProp, thinkProp);
						}
					}
					edit = nullptr;
					gearPanel.Hide();
				} catch (const Kompas3DException&) {
				}
				return false;
			};
		} catch (const Kompas3DException&) {
		}
	}
	
	void Start() {
		if (!panel) throw new Kompas3DException("Панель свойств шестерни отсутствует");
		auto doc = Kompas3D::GetActiveDocument3D();
		if (!doc) {
			Kompas3D::Error("Не найден активный 3D документ");
			return;
		}
		auto m = doc->GetEditMacroObject();
		if (m) {
			edit = std::move(m);
			GearParameters gearParam;
			if (edit->GetUserParam(&gearParam, sizeof(gearParam))) {
				*propModule = gearParam.m;
				*propCount = gearParam.count;
				*propThink = gearParam.think;
			}
		}
		panel->Show();
	}

private:
	void CreateGear(double m, int count, double think) {
		auto doc = Kompas3D::GetActiveDocument3D();
		if (!doc) return;
		auto topPart = doc->GetTopPart();
		std::ostringstream name;
		name << "Шестерня " << m << " x " << count;
		auto macro = topPart->Create<NodeMacro>(false, name.str());
		GearParameters gearParam = {m, count, think};
		macro->SetUserParam(&gearParam, sizeof(gearParam), MENU_GEAR);
		
		auto sketchBlank = topPart->Create<Sketch>(topPart->GetPlaneXOY(), "BlankSketch");
		sketchBlank->Circle(0, 0, (m * (count + 2.0)) / 2.0);
		auto blank = topPart->Create<BaseExtrusion>(sketchBlank, think, 0, "Blank");
		
		auto sketchToth = topPart->Create<Sketch>(topPart->GetPlaneXOY(), "TothSketch");
		DrawTothSketch(*sketchToth, m, count);
		auto toth = topPart->Create<CutExtrusion>(sketchToth, 0, think, "TothCut");
		
		macro->Add(sketchBlank)
			.Add(blank)
			.Add(sketchToth)
			.Add(toth);
		
		auto tothArray = topPart->Create<CircularCopy>(count, 360, true, 1, 0, false, topPart->GetAxisOZ(), std::move(toth), "TothCutArray");
		
		macro->Add(tothArray)
			.Update();
	}
	
	void ReplaceGear(double m, int count, double think) {
		if (!edit) return;
		std::unique_ptr<Sketch> sketchBlank = nullptr;
		std::unique_ptr<BaseExtrusion> blank = nullptr;
		std::unique_ptr<Sketch> sketchToth = nullptr;
		std::unique_ptr<CutExtrusion> toth = nullptr;
		std::unique_ptr<CircularCopy> tothArray = nullptr;
		for (auto& node : edit->GetNodes()) {
			if (node->IsType(Sketch::TYPE) && node->GetName() == "BlankSketch") {
				sketchBlank = node->As<Sketch>();
			} else if ((node->IsType(BaseExtrusion::TYPE)
				|| node->IsType(25))
				&& node->GetName() == "Blank") {
				blank = node->As<BaseExtrusion>();
			} else if (node->IsType(Sketch::TYPE) && node->GetName() == "TothSketch") {
				sketchToth = node->As<Sketch>();
			} else if (node->IsType(CutExtrusion::TYPE) && node->GetName() == "TothCut") {
				toth = node->As<CutExtrusion>();
			} else if (node->IsType(CircularCopy::TYPE) && node->GetName() == "TothCutArray") {
				tothArray = node->As<CircularCopy>();
			}
		}
		if (!sketchBlank || !blank || !sketchToth || !toth || !tothArray) return;
		GearParameters gearParam = {m, count, think};
		edit->SetUserParam(&gearParam, sizeof(gearParam), MENU_GEAR);
		std::ostringstream name;
		name << "Шестерня " << m << " x " << count;
		edit->SetName(name.str());
		sketchBlank->Clear().Circle(0, 0, (m * (count + 2.0)) / 2.0).EndEdit();
		blank->SetDepth1(think);
		DrawTothSketch(sketchToth->Clear(), m, count);
		sketchToth->EndEdit();
		toth->SetDepth2(think);
		tothArray->SetCircularParam(count, 360, true);
		edit->Show().Update();
	}

	void DrawTothSketch(Sketch& sketch, double m, int count) {
	    // 1. Основные параметры геометрии
	    const double alpha = 20.0 * M_PI / 180.0; // Угол зацепления в радианах
	
	    double r = (m * count) / 2.0;             // Делительный радиус
	    double da = m * (count + 2.0);            // Диаметр вершин
	    double df = m * (count - 2.5);            // Диаметр впадин
	    double ra = da / 2.0;                     // Радиус вершин
	    double rf = df / 2.0;                     // Радиус впадин
	    double rb = r * std::cos(alpha);          // Радиус основной окружности
	
	    // Шаг и угловая ширина впадины на делительной окружности
	    double psi_vpadina = M_PI / count;
	
	    // Инволюта угла зацепления
	    double inv_alpha = std::tan(alpha) - alpha;
	
	    // Угловое расстояние от оси Y до начала эвольвенты на основной окружности
	    double theta_b = (psi_vpadina / 2.0) - inv_alpha;
	
	    // Переменная для хранения финального угла ang3, нужная для внешней окружности
	    double ang3 = 0.0;
	
	    // ================= ПОСТРОЕНИЕ КОНТУРА ВПАДИНЫ =================
	
	    if (count >= 42) {
	        // --- ВАРИАНТ ДЛЯ БОЛЬШИХ КОЛЕС (Z=120) ---
	        double alpha_f = std::acos(rb / rf);
	        double inv_alpha_f = std::tan(alpha_f) - alpha_f;
	        double ang1 = theta_b + inv_alpha_f;
	
	        if (rb < ra) {
	            double alpha_a = std::acos(rb / ra);
	            double inv_alpha_a = std::tan(alpha_a) - alpha_a;
	            ang3 = theta_b + inv_alpha_a;
	        } else {
	            ang3 = psi_vpadina / 2.0;
	        }
	
	        double x2_ev = r * std::sin(psi_vpadina / 2.0);
	        double y2_ev = r * std::cos(psi_vpadina / 2.0);
	        double x3_ev = ra * std::sin(ang3);
	        double y3_ev = ra * std::cos(ang3);
	
	        double x2_ev_l = -x2_ev;
	        double y2_ev_l = y2_ev;
	        double x3_ev_l = -x3_ev;
	        double y3_ev_l = y3_ev;
	
	        double rc = 0.38 * m;
	        double gamma = ang1;
	
	        double delta_angle = (rf + rc) > 0 ? std::asin(rc / (rf + rc)) : 0;
	        if (delta_angle >= ang1) {
	            rc = (rf * std::sin(ang1)) / (1.0 - std::sin(ang1));
	            gamma = ang1 * 0.99;
	        }
	
	        double xc_f = 0.0;
	        double yc_f = (rf + rc);
	
	        double x_touch_r = rc * std::cos(gamma);
	        double y_touch_r = yc_f - rc * std::sin(gamma);
	        double x_touch_l = -x_touch_r;
	        double y_touch_l = y_touch_r;
	
	        // Центральная дуга дна для больших Z
	        sketch.ArcByPoint(xc_f, yc_f, rc, x_touch_l, y_touch_l, x_touch_r, y_touch_r, false);
	
	        // Боковые эвольвенты
	        sketch.ArcBy3Points(x_touch_r, y_touch_r, x2_ev, y2_ev, x3_ev, y3_ev);
	        sketch.ArcBy3Points(x3_ev_l, y3_ev_l, x2_ev_l, y2_ev_l, x_touch_l, y_touch_l);
	
	    } else {
	        // --- ВАРИАНТ ДЛЯ МАЛЫХ КОЛЕС (Z=20) ---
	        double ang1 = theta_b;
	        double x1_ev = rb * std::sin(ang1);
	        double y1_ev = rb * std::cos(ang1);
	
	        double ang2 = (psi_vpadina / 2.0);
	        double x2_ev = r * std::sin(ang2);
	        double y2_ev = r * std::cos(ang2);
	
	        double alpha_a = std::acos(rb / ra);
	        double inv_alpha_a = std::tan(alpha_a) - alpha_a;
	        ang3 = theta_b + inv_alpha_a;
	        double x3_ev = ra * std::sin(ang3);
	        double y3_ev = ra * std::cos(ang3);
	
	        double x1_ev_l = -x1_ev;
	        double y1_ev_l = y1_ev;
	        double x2_ev_l = -x2_ev;
	        double y2_ev_l = y2_ev;
	        double x3_ev_l = -x3_ev;
	        double y3_ev_l = y3_ev;
	
	        double rc = 0.38 * m;
	        double delta_angle = std::asin(rc / (rf + rc));
	        double ang_center_r = ang1 - delta_angle;
	
	        // Координаты центров окружностей галтелей
	        double xc_r = (rf + rc) * std::sin(ang_center_r);
	        double yc_r = (rf + rc) * std::cos(ang_center_r);
	        double xc_l = -xc_r;
	        double yc_l = yc_r;
	
	        // Точки сопряжения галтелей на плоском дне (радиус rf)
	        double x_bot_r = rf * std::sin(ang_center_r);
	        double y_bot_r = rf * std::cos(ang_center_r);
	        double x_bot_l = -x_bot_r;
	        double y_bot_l = y_bot_r;
	
	        // --- ЖЕСТКИЙ ГЕОМЕТРИЧЕСКИЙ РАСЧЕТ СРЕДНИХ ТОЧЕК ГАЛТЕЛЕЙ ---
	        // 1. Находим среднее геометрическое направление вектора от центра окружности скругления
	        double mx_r = ((x_bot_r + x1_ev) / 2.0) - xc_r;
	        double my_r = ((y_bot_r + y1_ev) / 2.0) - yc_r;
	        double len_m_r = std::hypot(mx_r, my_r);
	
	        // Проецируем этот вектор строго на радиус rc, получая идеальную точку на дуге
	        double x_mid_r = len_m_r > 0 ? xc_r + (mx_r / len_m_r) * rc : mx_r;
	        double y_mid_r = len_m_r > 0 ? yc_r + (my_r / len_m_r) * rc : my_r;
	
	        // 2. Аналогичный абсолютно симметричный расчет для левой галтели
	        double mx_l = ((x_bot_l + x1_ev_l) / 2.0) - xc_l;
	        double my_l = ((y_bot_l + y1_ev_l) / 2.0) - yc_l;
	        double len_m_l = std::hypot(mx_l, my_l);
	
	        double x_mid_l = len_m_l > 0 ? xc_l + (mx_l / len_m_l) * rc : mx_l;
	        double y_mid_l = len_m_l > 0 ? yc_l + (my_l / len_m_l) * rc : my_l;
	
	        // ================= ЧЕРЧЕНИЕ ГЕОМЕТРИИ =================
	
	        // 1. Плоское дно впадины
	        double f1_f = (M_PI / 2.0 - ang_center_r) * 180.0 / M_PI;
	        double f2_f = (M_PI / 2.0 + ang_center_r) * 180.0 / M_PI;
	        sketch.ArcByAngle(0, 0, rf, f1_f, f2_f, false);
	
	        // 2. Правое скругление
	        sketch.ArcBy3Points(x1_ev, y1_ev, x_mid_r, y_mid_r, x_bot_r, y_bot_r);
	
	        // 3. Левое скругление
	        sketch.ArcBy3Points(x_bot_l, y_bot_l, x_mid_l, y_mid_l, x1_ev_l, y1_ev_l);
	
	        // 4. Боковые эвольвентные стенки впадины
	        sketch.ArcBy3Points(x1_ev, y1_ev, x2_ev, y2_ev, x3_ev, y3_ev);
	        sketch.ArcBy3Points(x3_ev_l, y3_ev_l, x2_ev_l, y2_ev_l, x1_ev_l, y1_ev_l);
	    }
	
	    // 5. Внешняя окружность заготовки (общая для обоих вариантов)
	    double f1_a_r = (M_PI / 2.0 + ang3) * 180.0 / M_PI;
	    double f2_a_l = (M_PI / 2.0 - ang3) * 180.0 / M_PI;
	    sketch.ArcByAngle(0, 0, ra, f2_a_l, f1_a_r, false); // Передан 1 вместо True, так как тип метода ожидает int направление
	}
};

#endif
