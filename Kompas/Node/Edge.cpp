#include "../ComKompas.h"
#include "Edge.h"

Face Edge::LeftFace() const {
	K5::ksEntityPtr entity = pEntity;
	K5::ksEdgeDefinitionPtr def = entity->GetDefinition();
	K5::ksFaceDefinitionPtr faceDef = def->GetAdjacentFace(true);
	K5::ksEntityPtr face = faceDef->GetEntity();
	face.AddRef();
	return Face(face.GetInterfacePtr());
}

Face Edge::RightFace() const {
	K5::ksEntityPtr entity = pEntity;
	K5::ksEdgeDefinitionPtr def = entity->GetDefinition();
	K5::ksFaceDefinitionPtr faceDef = def->GetAdjacentFace(false);
	K5::ksEntityPtr face = faceDef->GetEntity();
	face.AddRef();
	return Face(face.GetInterfacePtr());
}

int Edge::TYPE = KConst3D::o3d_edge;