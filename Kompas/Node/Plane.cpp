#include "../ComKompas.h"
#include "Plane.h"

int Plane::TYPE = KConst3D::o3d_planeXOY; // XOY ?

ParallelPlane::ParallelPlane(IUnknown* pE, IDispatch* pD, const Face& planarFace, const Vertex& point) : Plane(pE, pD) {
	K5::ksEntityPtr entity = pEntity;
	K5::ksPlaneParallelDefinitionPtr def = pDefinition;
	def->SetPlane((K5::ksEntityPtr)planarFace.pEntity);
	def->SetPoint(point.pDefinition);
	entity->Create();
}

int ParallelPlane::TYPE = KConst3D::o3d_planeParallel;

EdgePointPlane::EdgePointPlane(IUnknown* pE, IDispatch* pD, const Axis& axis, const Vertex& point) : Plane(pE, pD) {
	K5::ksEntityPtr entity = pEntity;
	K5::ksPlaneEdgePointDefinitionPtr def = pDefinition;
	def->SetEdge((K5::ksEntityPtr)axis.pEntity);
	def->SetPoint(point.pDefinition);
	entity->Create();
}

int EdgePointPlane::TYPE = KConst3D::o3d_planeEdgePoint;
