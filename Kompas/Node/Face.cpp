#include "../ComKompas.h"
#include "Face.h"

bool Face::IsPlanar() {
	K5::ksEntityPtr entity = pEntity;
	K5::ksFaceDefinitionPtr face = entity->GetDefinition();
	return face ? face->IsPlanar() : false;
}

bool Face::IsCylinder() {
	K5::ksEntityPtr entity = pEntity;
	K5::ksFaceDefinitionPtr face = entity->GetDefinition();
	return face ? face->IsCylinder() : false;
}

int Face::TYPE = KConst3D::o3d_face;
