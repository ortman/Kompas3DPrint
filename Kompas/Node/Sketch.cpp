#include "../ComKompas.h"
#include "Sketch.h"
#include "../Kompas3D.h"

Sketch::Sketch(IUnknown* pE, IDispatch* pD, Plane plane, double angle, double locX, double locY, const std::optional<std::string>& name) : Node(pE, pD) {
	K5::ksEntityPtr entity = pEntity;
	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
	K5::ksSketchDefinitionPtr def = pDefinition;
	if (!def) throw Kompas3DException("Не могу получить SketchDefinition, entityType=" + std::to_string(entity->type));
	K5::ksEntityPtr planeEntity = plane.pEntity;
	if (!planeEntity) throw Kompas3DException("Не могу получить Plane для Эскиза");
	def->angle = angle;
	def->SetPlane(planeEntity);
	def->SetLocation(locX, locY);
	entity->Create();
}

Sketch& Sketch::BeginEdit() {
	if (eDef) return *this;
	K5::ksSketchDefinitionPtr def = pDefinition;
	if (!def) throw Kompas3DException("Не могу получить SketchDefinition");
	K5::ksDocument2DPtr doc2D = def->BeginEdit();
	if (!doc2D) {
		eDef = nullptr;
		throw Kompas3DException("Не могу получить ksDocument2D!");
	}
	eDef = doc2D;
	eDef->AddRef();
	return *this;
}

void Sketch::EndEdit() {
	if (eDef) {
		K5::ksSketchDefinitionPtr def = pDefinition;
		if (!def) throw Kompas3DException("Не могу получить SketchDefinition");
		def->EndEdit();
		eDef->Release();
		eDef = nullptr;
	}
}

Sketch& Sketch::Clear() {
	BeginEdit();
	K5::ksDocument2DPtr doc2D = eDef;
	doc2D->ksSelectGroup(0, 2, -1, -1, 1, 1);
	doc2D->ksDeleteObj(0);
	doc2D->ksSelectGroup(0, 3, -2, -2, 2, 2);
	doc2D->ksDeleteObj(0);
	return *this;
}

Sketch& Sketch::Line(double x1, double y1, double x2, double y2, LineStyle style) {
	BeginEdit();
	K5::ksDocument2DPtr doc2D = eDef;
	doc2D->ksLineSeg(x1, y1, x2, y2, (int)style);
	lastX = x2;
	lastY = y2;
	return *this;
}

Sketch& Sketch::LineTo(double x, double y, LineStyle style) {
	return Line(lastX, lastY, x, y, style);
}

Sketch& Sketch::Circle(double cx, double cy, double r, LineStyle style) {
	BeginEdit();
	K5::ksDocument2DPtr doc2D = eDef;
	doc2D->ksCircle(cx, cy, r, (int)style);
	lastX = cx;
	lastY = cy;
	return *this;
}

Sketch& Sketch::Rect(double x, double y, double w, double h, double angle, LineStyle style) {
	K5::ksRectangleParamPtr param = Kompas3D::GetParamStruct<K5::ksRectangleParamPtr>(KConst::ko_RectangleParam);
	if (!param) return *this;
	BeginEdit();
	K5::ksDocument2DPtr doc2D = eDef;
	param->Init();
	param->x = x;
	param->y = y;
	param->width = w;
	param->height = h;
	param->style = (int)style;
	param->ang = angle;
	doc2D->ksRectangle(param, 0);
	lastX = x + w;
	lastY = y + h;
	return *this;
}

Sketch& Sketch::RegularPolygon(double cx, double cy, double r, int count, bool describe, double angle, LineStyle style) {
	K5::ksRegularPolygonParamPtr param = Kompas3D::GetParamStruct<K5::ksRegularPolygonParamPtr>(KConst::ko_RegularPolygonParam);
	if (!param) return *this;
	BeginEdit();
	K5::ksDocument2DPtr doc2D = eDef;
	param->Init();
	param->xc = cx;
	param->yc = cy;
	param->radius = r;
	param->count = count;
	param->describe = describe;
	param->style = (int)style;
	doc2D->ksRegularPolygon(param, 0);
	lastX = cx;
	lastY = cy;
	return *this;
}

Sketch& Sketch::Point(double x, double y, LineStyle style) {
	BeginEdit();
	K5::ksDocument2DPtr doc2D = eDef;
	doc2D->ksPoint(x, y, (int)style);
	lastX = x;
	lastY = y;
	return *this;
}

Sketch& Sketch::ArcByAngle(double cx, double cy, double r, double f1, double f2, bool cw, LineStyle style) {
	BeginEdit();
	K5::ksDocument2DPtr doc2D = eDef;
	doc2D->ksArcByAngle(cx, cy, r, f1, f2, cw ? -1 : 1, (int)style);
	lastX = cx;
	lastY = cy;
	return *this;
}

Sketch& Sketch::ArcByPoint(double cx, double cy, double r, double x1, double y1, double x2, double y2, bool cw, LineStyle style) {
	BeginEdit();
	K5::ksDocument2DPtr doc2D = eDef;
	doc2D->ksArcByPoint(cx, cy, r, x1, y1, x2, y2, cw ? -1 : 1, (int)style);
	lastX = cx;
	lastY = cy;
	return *this;
}

Sketch& Sketch::ArcBy3Points(double x1, double y1, double x2, double y2, double x3, double y3, LineStyle style) {
	BeginEdit();
	K5::ksDocument2DPtr doc2D = eDef;
	doc2D->ksArcBy3Points(x1, y1, x2, y2, x3, y3, (int)style);
	lastX = x3;
	lastY = y3;
	return *this;
}

Sketch& Sketch::Ellipse(double cx, double cy, double a, double b, double angle, LineStyle style) {
	K5::ksEllipseParamPtr param = Kompas3D::GetParamStruct<K5::ksEllipseParamPtr>(KConst::ko_EllipseParam);
	if (!param) return *this;
	BeginEdit();
	K5::ksDocument2DPtr doc2D = eDef;
	param->Init();
	param->xc = cx;
	param->yc = cy;
	param->A = a;
	param->B = b;
	param->angle = angle;
	param->style = (int)style;
	doc2D->ksEllipse(param);
	lastX = cx;
	lastY = cy;
	return *this;
}

Sketch& Sketch::EllipseArc(double cx, double cy, double a, double b, double a1, double a2, bool cw, double angle, LineStyle style) {
	K5::ksEllipseArcParamPtr param = Kompas3D::GetParamStruct<K5::ksEllipseArcParamPtr>(KConst::ko_EllipsArcParam);
	if (!param) return *this;
	BeginEdit();
	K5::ksDocument2DPtr doc2D = eDef;
	param->Init();
	param->xc = cx;
	param->yc = cy;
	param->A = a;
	param->B = b;
	param->angle = angle;
	param->angleFirst = a1;
	param->angleSecond = a2;
	param->direction = cw ? -1 : 1;
	param->style = (int)style;
	doc2D->ksEllipseArc(param);
	lastX = cx;
	lastY = cy;
	return *this;
}

int Sketch::TYPE = KConst3D::o3d_sketch;