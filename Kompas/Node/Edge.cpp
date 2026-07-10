#include "../ComKompas.h"
#include "Edge.h"

Face Edge::LeftFace() const {
	K5::ksEdgeDefinitionPtr def = pEntity;
	return Face(def->GetAdjacentFace(true));
}

Face Edge::RightFace() const {
	K5::ksEdgeDefinitionPtr def = pEntity;
	return Face(def->GetAdjacentFace(false));
}

int Edge::TYPE = KConst3D::o3d_edge;