#include "../ComKompas.h"
#include "Plane.h"

int Plane::TYPE = KConst3D::o3d_planeXOY; // XOY ?

ParallelPlane::ParallelPlane(IUnknown* pEntity, const Face& planarFace/*, point */) : Plane(pEntity) {
	K5::ksEntityPtr entity = pEntity;
	entity->Create();
}

int ParallelPlane::TYPE = KConst3D::o3d_planeParallel;
