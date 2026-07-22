#include "../ComKompas.h"
#include "CylindricSpiral.h"

CylindricSpiral& CylindricSpiral::SetDiam(double d) {
	K5::ksCylindricSpiralDefinitionPtr def = pDefinition;
	def->diam = d;
	return *this;
}

CylindricSpiral& CylindricSpiral::SetStep(double step) {
	K5::ksCylindricSpiralDefinitionPtr def = pDefinition;
	def->step = step;
	return *this;
}

CylindricSpiral& CylindricSpiral::SetHeight(double height) {
	K5::ksCylindricSpiralDefinitionPtr def = pDefinition;
	def->height = height;
	return *this;
}

CylindricSpiral& CylindricSpiral::TurnDir(bool dir) {
	K5::ksCylindricSpiralDefinitionPtr def = pDefinition;
	def->turnDir = dir;
	return *this;
}

CylindricSpiral& CylindricSpiral::BuildDir(bool dir) {
	K5::ksCylindricSpiralDefinitionPtr def = pDefinition;
	def->buildDir = dir;
	return *this;
}

CylindricSpiral& CylindricSpiral::BuildMode(Mode mode) {
	K5::ksCylindricSpiralDefinitionPtr def = pDefinition;
	def->buildMode = mode;
	return *this;
}

CylindricSpiral& CylindricSpiral::SetPlane(const Plane& plane) {
	K5::ksCylindricSpiralDefinitionPtr def = pDefinition;
	K5::ksEntityPtr planeEntity = plane.pEntity;
	def->SetPlane(planeEntity);
	return *this;
}

CylindricSpiral& CylindricSpiral::SetLocation(double x, double y) {
	K5::ksCylindricSpiralDefinitionPtr def = pDefinition;
	def->SetLocation(x, y);
	return *this;
}

bool CylindricSpiral::Create(bool show) {
	K5::ksEntityPtr entity = pEntity;
	entity->hidden = !show;
	return entity->Create();
}

Vertex CylindricSpiral::GetBeginVertex() const {
	K5::ksEntityPtr entity = pEntity;
	K5::ksFeaturePtr feature = entity->GetFeature();
	if (!feature) throw Kompas3DException("Не могу получить Feature у CylindricSpiral");
	K5::ksEntityCollectionPtr vertexes = feature->EntityCollection(KConst3D::o3d_vertex);
	if (!vertexes) throw Kompas3DException("Не могу получить EntityCollection у CylindricSpiral");
	if (vertexes->GetCount() != 2) throw Kompas3DException("Количество vertexes у CylindricSpiral должно быть 2, но их " + std::to_string(vertexes->GetCount()));
	K5::ksEntityPtr v = vertexes->GetByIndex(1);
	if (!v) throw Kompas3DException("Не могу получить Vertex у CylindricSpiral");
	return Vertex(v);
}

int CylindricSpiral::TYPE = KConst3D::o3d_cylindricSpiral;