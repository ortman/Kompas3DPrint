/*
 * #include "Plane.h"
 * 
 * K5::ksSurfacePtr GetSurface(IUnknown* pDefinition) {
 * 	if (K5::ksDefaultObjectPtr def = pDefinition) return def->GetSurface();
 * 	if (K5::ksPlaneOffsetDefinitionPtr def = pDefinition) return def->GetSurface();
 * 	if (K5::ksPlaneAngleDefinitionPtr def = pDefinition) return def->GetSurface();
 * 	if (K5::ksPlane3PointsDefinitionPtr def = pDefinition) return def->GetSurface();
 * 	if (K5::ksPlaneNormalToSurfaceDefinitionPtr def = pDefinition) return def->GetSurface();
 * 	if (K5::ksPlaneTangentToSurfaceDefinitionPtr def = pDefinition) return def->GetSurface();
 * 	if (K5::ksPlaneEdgePointDefinitionPtr def = pDefinition) return def->GetSurface();
 * 	if (K5::ksPlaneParallelDefinitionPtr def = pDefinition) return def->GetSurface();
 * 	if (K5::ksPlanePerpendicularDefinitionPtr def = pDefinition) return def->GetSurface();
 * 	if (K5::ksPlaneLineToEdgeDefinitionPtr def = pDefinition) return def->GetSurface();
 * 	if (K5::ksPlaneLineToPlaneDefinitionPtr def = pDefinition) return def->GetSurface();
 * 	if (K5::ksPlaneMiddleDefinitionPtr def = pDefinition) return def->GetSurface();
 * 	return NULL;
 * }
 * 
 * Vertex::Point3D Plane::GetVector(AxisType type) {
 * 	K5::ksSurfacePtr surface = GetSurface(pDefinition);
 * 	K5::ksPlaneParamPtr param = surface->GetSurfaceParam();
 *   K5::ksPlacementPtr surfPl = param->GetPlacement();
 *   Vertex::Point3D res;
 *   if (surfPl->GetVector((long)type, &res.x, &res.y, &res.z)) {
 *     return res;
 *   }
 *   return {0., 0., 0.};
 * }
 * 
 * Plane::Point2D Plane::Projection(const Vertex::Point3D& point) {
 * 	K5::ksSurfacePtr surface = GetSurface(pDefinition);
 * 	K5::ksPlaneParamPtr param = surface->GetSurfaceParam();
 *   K5::ksPlacementPtr surfPl = param->GetPlacement();
 *   Point2D res;
 *   if (surfPl->PointProjection(point.x, point.y, point.z, &res.x, &res.y)) {
 *     return res;
 *   }
 *   return {0., 0.};
 * }
 * 
 * Vertex::Point3D Plane::Projection(const Plane::Point2D& point) {
 * 	K5::ksSurfacePtr surface = GetSurface(pDefinition);
 * 	K5::ksPlaneParamPtr param = surface->GetSurfaceParam();
 *   K5::ksPlacementPtr surfPl = param->GetPlacement();
 *   Vertex::Point3D res;
 *   if (surfPl->PointOn(point.x, point.y, &res.x, &res.y, &res.z)) {
 *     return res;
 *   }
 *   return {0., 0., 0.};
 * }
 * 
 * int Plane::TYPE = KConst3D::o3d_planeXOY; // XOY ?
 * 
 * ParallelPlane::ParallelPlane(IUnknown* pE, IDispatch* pD, const Face& planarFace, const Vertex& point, bool show) : Plane(pE, pD) {
 * 	K5::ksEntityPtr entity = pEntity;
 * 	K5::ksPlaneParallelDefinitionPtr def = pDefinition;
 * 	def->SetPlane((K5::ksEntityPtr)planarFace.pEntity);
 * 	def->SetPoint(point.pDefinition);
 * 	entity->hidden = !show;
 * 	entity->Create();
 * }
 * 
 * int ParallelPlane::TYPE = KConst3D::o3d_planeParallel;
 * 
 * EdgePointPlane::EdgePointPlane(IUnknown* pE, IDispatch* pD, const Axis& axis, const Vertex& point, bool show) : Plane(pE, pD) {
 * 	K5::ksEntityPtr entity = pEntity;
 * 	K5::ksPlaneEdgePointDefinitionPtr def = pDefinition;
 * 	def->SetEdge((K5::ksEntityPtr)axis.pEntity);
 * 	def->SetPoint(point.pDefinition);
 * 	entity->hidden = !show;
 * 	entity->Create();
 * }
 * 
 * int EdgePointPlane::TYPE = KConst3D::o3d_planeEdgePoint;
 */
