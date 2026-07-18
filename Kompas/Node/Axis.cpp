#include "../ComKompas.h"
#include "Axis.h"

int Axis::TYPE = KConst3D::o3d_axisOX; // OX ?

ConeAxis::ConeAxis(IUnknown* pE, IDispatch* pD, const Face& coneFace) : Axis(pE, pD) {
	K5::ksEntityPtr entity = pEntity;
	K5::ksAxisConefaceDefinitionPtr def = pDefinition;
	def->SetFace((K5::ksEntityPtr)coneFace.pEntity);
	entity->Create();
}

int ConeAxis::TYPE = KConst3D::o3d_axisConeFace;