#include "../ComKompas.h"
#include "Face.h"

bool Face::IsPlanar() {
	K5::ksFaceDefinitionPtr face = pEntity;
	return face ? face->IsPlanar() : false;
}

int Face::TYPE = KConst3D::o3d_face;
