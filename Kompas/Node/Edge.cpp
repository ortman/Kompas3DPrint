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

int Edge::TYPE = KConst3D::o3d_edge;