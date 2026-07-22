#include "../ComKompas.h"
#include "Axis.h"

K5::ksCurve3DPtr GetCurve3D(IUnknown* pDefinition) {
	if (K5::ksDefaultObjectPtr def = pDefinition) return def->GetCurve3D();
	if (K5::ksAxis2PlanesDefinitionPtr def = pDefinition) return def->GetCurve3D();
	if (K5::ksAxis2PointsDefinitionPtr def = pDefinition) return def->GetCurve3D();
	if (K5::ksAxisConefaceDefinitionPtr def = pDefinition) return def->GetCurve3D();
	if (K5::ksAxisEdgeDefinitionPtr def = pDefinition) return def->GetCurve3D();
	if (K5::ksAxisOperationsDefinitionPtr def = pDefinition) return def->GetCurve3D();
	return NULL;
}

Vertex::Point3D Axis::GetFirstPoint() {
	K5::ksCurve3DPtr curve = GetCurve3D(pDefinition);
	K5::ksLineSeg3dParamPtr param = curve->GetCurveParam();
	Vertex::Point3D res;
	if (param->GetPointFirst(&res.x, &res.y, &res.z)) return res;
	return {0., 0., 0.};
}

Vertex::Point3D Axis::GetLastPoint() {
	K5::ksCurve3DPtr curve = GetCurve3D(pDefinition);
	K5::ksLineSeg3dParamPtr param = curve->GetCurveParam();
	Vertex::Point3D res;
	if (param->GetPointLast(&res.x, &res.y, &res.z)) return res;
	return {0., 0., 0.};
}

int Axis::TYPE = KConst3D::o3d_axisOX; // OX ?

ConeAxis::ConeAxis(IUnknown* pE, IDispatch* pD, const Face& coneFace, bool show) : Axis(pE, pD) {
	K5::ksEntityPtr entity = pEntity;
	K5::ksAxisConefaceDefinitionPtr def = pDefinition;
	def->SetFace((K5::ksEntityPtr)coneFace.pEntity);
	entity->hidden = !show;
	entity->Create();
}

int ConeAxis::TYPE = KConst3D::o3d_axisConeFace;