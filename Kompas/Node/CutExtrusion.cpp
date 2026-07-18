#include "../ComKompas.h"
#include "CutExtrusion.h"

CutExtrusion::CutExtrusion(IUnknown* pE, IDispatch* pD, Sketch& sketch, double depth1, double depth2, const std::optional<std::string>& name) : Node(pE, pD) {
	sketch.EndEdit();
	K5::ksEntityPtr entity = pEntity;
	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
	K5::ksCutExtrusionDefinitionPtr def = pDefinition;
	if (!def) throw Kompas3DException("Не могу получить ksCutExtrusionDefinition, entityType=" + std::to_string(entity->type));
	K5::ksEntityPtr sketchEntity = sketch.pEntity;
	if (!sketchEntity) throw Kompas3DException("Не могу получить Эскиз, entityType=" + std::to_string(sketchEntity->type));
	def->SetSketch(sketchEntity);
	K5::ksExtrusionParamPtr param = def->ExtrusionParam();
	if (!param) throw Kompas3DException("Не могу получить ksExtrusionParam");
	param->depthNormal = depth1;
	param->depthReverse = depth2;
	entity->Create();
}

CutExtrusion& CutExtrusion::SetDepth1(double depth) {
	K5::ksCutExtrusionDefinitionPtr def = pDefinition;
	K5::ksExtrusionParamPtr param = def->ExtrusionParam();
	param->depthNormal = depth;
	return *this;
}

CutExtrusion& CutExtrusion::SetDepth2(double depth) {
	K5::ksCutExtrusionDefinitionPtr def = pDefinition;
	K5::ksExtrusionParamPtr param = def->ExtrusionParam();
	param->depthReverse = depth;
	return *this;
}

int CutExtrusion::TYPE = KConst3D::o3d_cutExtrusion;