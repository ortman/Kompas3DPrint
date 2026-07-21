#include "../ComKompas.h"
#include "Edge.h"

Face Edge::LeftFace() const {
	K5::ksEdgeDefinitionPtr def = pDefinition;
	K5::ksFaceDefinitionPtr faceDef = def->GetAdjacentFace(true);
	K5::ksEntityPtr face = faceDef->GetEntity();
	//face.AddRef();
	return Face(face.GetInterfacePtr());
}

Face Edge::RightFace() const {
	K5::ksEdgeDefinitionPtr def = pDefinition;
	K5::ksFaceDefinitionPtr faceDef = def->GetAdjacentFace(false);
	K5::ksEntityPtr face = faceDef->GetEntity();
	//face.AddRef();
	return Face(face.GetInterfacePtr());
}

Vertex Edge::GetBeginVertex() const {
	K5::ksEdgeDefinitionPtr def = pDefinition;
	K5::ksVertexDefinitionPtr p = def->GetVertex(true);
	return Vertex(NULL, p.GetInterfacePtr());
}

Vertex::Point3D Edge::GetOrigin() const {
	K5::ksEdgeDefinitionPtr def = pDefinition;
	K5::ksCurve3DPtr curve = def->GetCurve3D();
	Vertex::Point3D res;
	if (K5::ksArc3dParamPtr param = curve->GetCurveParam()) {
		K5::ksPlacementPtr pl = param->GetPlacement();
		if (pl && pl->GetOrigin(&res.x, &res.y, &res.z)) return res;
	}
	if (K5::ksCircle3dParamPtr param = curve->GetCurveParam()) {
		K5::ksPlacementPtr pl = param->GetPlacement();
		if (pl && pl->GetOrigin(&res.x, &res.y, &res.z)) return res;
	}
	if (K5::ksEllipse3dParamPtr param = curve->GetCurveParam()) {
		K5::ksPlacementPtr pl = param->GetPlacement();
		if (pl && pl->GetOrigin(&res.x, &res.y, &res.z)) return res;
	}
	return {0., 0., 0.};
}

int Edge::TYPE = KConst3D::o3d_edge;