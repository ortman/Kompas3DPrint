#ifndef _ComTest_Sketch_h_
#define _ComTest_Sketch_h_

#include <unknwn.h>
#include <memory>
#include "Node/Plane.h"

enum class LineStyle : int {
    Main = 1,                           // Основная
    Thin = 2,                           // Тонкая
    Axial = 3,                          // Осевая
    Dashed = 4,                         // Штриховая
    Break = 5,                          // Для линии обрыва
    Auxiliary = 6,                      // Вспомогательная
    Thickened = 7,                      // Утолщенная
    Dotted2 = 8,                        // Пунктир 2
    DashedMain = 9,                     // Штриховая осн
    AxialMain = 10,                     // Осевая осн
    ThinInHatching = 11,                // Тонкая линия, включаемая в штриховку
    // Стили ISO (12-25)
    Iso02_Dashed = 12,                  // ISO 02 штриховая линия
    Iso03_DashedLongSpace = 13,         // ISO 03 штриховая линия (дл. пробел)
    Iso04_ChainLongDash = 14,           // ISO 04 штрихпунктирная линия (дл. штрих)
    Iso05_ChainLongDash2Dot = 15,       // ISO 05 штрихпунктирная линия (дл. штрих 2 пунктира)
    Iso06_ChainLongDash3Dot = 16,       // ISO 06 штрихпунктирная линия (дл. штрих 3 пунктира)
    Iso07_Dotted = 17,                  // ISO 07 пунктирная линия
    Iso08_ChainLongShortDash = 18,      // ISO 08 штрихпунктирная линия (дл. и кор. штрихи)
    Iso09_ChainLong2ShortDash = 19,     // ISO 09 штрихпунктирная линия (дл. и 2 кор. штриха)
    Iso10_Chain = 20,                   // ISO 10 штрихпунктирная линия
    Iso11_Chain2Dash = 21,              // ISO 11 штрихпунктирная линия (2 штриха)
    Iso12_Chain2Dot = 22,               // ISO 12 штрихпунктирная линия (2 пунктира)
    Iso13_Chain3Dot = 23,               // ISO 13 штрихпунктирная линия (3 пунктира)
    Iso14_Chain2Dash2Dot = 24,          // ISO 14 штрихпунктирная линия (2 штриха 2 пунктира)
    Iso15_Chain2Dash3Dot = 25           // ISO 15 штрихпунктирная линия (2 штриха 3 пунктира)
};

class Sketch : public Node {
private:
	IUnknown* eDef = nullptr;
	double lastX = 0.0;
	double lastY = 0.0;
public:
	static int TYPE;
	Sketch(const Node& node) : Node(node.GetEntity()) {}
	Sketch(IUnknown* pSketch, Plane plane, double angle = 0.0, double locX = 0.0, double locY = 0.0, const std::optional<std::string>& name = std::nullopt);
	Sketch(IUnknown* pSketch, Plane plane, const std::optional<std::string>& name = std::nullopt) : Sketch(pSketch, std::move(plane), 0.0, 0.0, 0.0, name) {}
	Sketch& BeginEdit();
	void EndEdit();
	bool IsEdit() { return eDef; }
	Sketch& Clear();
	Sketch& Line(double x1, double y1, double x2, double y2, LineStyle style = LineStyle::Main);
	Sketch& LineTo(double x, double y, LineStyle style = LineStyle::Main);
	Sketch& Circle(double cx, double cy, double r, LineStyle style = LineStyle::Main);
	Sketch& Rect(double x, double y, double h, double w, double angle = 0.0, LineStyle style = LineStyle::Main);
	Sketch& RegularPolygon(double cx, double cy, double r, int count, bool describe = true, double angle = 0.0, LineStyle style = LineStyle::Main);
	Sketch& Point(double x, double y, LineStyle style = LineStyle::Main);
	Sketch& ArcByAngle(double cx, double cy, double r, double f1, double f2, bool cw, LineStyle style = LineStyle::Main);
	Sketch& ArcByPoint(double cx, double cy, double r, double x1, double y1, double x2, double y2, bool cw, LineStyle style = LineStyle::Main);
	Sketch& ArcBy3Points(double x1, double y1, double x2, double y2, double x3, double y3, LineStyle style = LineStyle::Main);
	Sketch& Ellipse(double cx, double cy, double a, double b, double angle = 0.0, LineStyle style = LineStyle::Main);
	Sketch& EllipseArc(double cx, double cy, double a, double b, double a1, double a2, bool cw, double angle = 0.0, LineStyle style = LineStyle::Main);
	//ksLine
	//ksColouring
	//ksConicArc
	//ksEquidistant
	//ksHatch
	//ksInsertRaster
	//ksParEllipseArc
	//ksPointArraw
};

#endif
