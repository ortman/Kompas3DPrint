#include "../ComKompas.h"
#include "BaseExtrusion.h"

BaseExtrusion::BaseExtrusion(IUnknown* p, std::unique_ptr<Sketch>& sketch, double depth1, double depth2, const std::optional<std::string>& name) : Node(p) {
	sketch->EndEdit();
	K5::ksEntityPtr entity = pEntity;
	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
	K5::ksBaseExtrusionDefinitionPtr def = entity->GetDefinition();
	if (!def) throw Kompas3DException("Не могу получить ksBaseExtrusionDefinition, entityType=" + std::to_string(entity->type));
	K5::ksEntityPtr sketchEntity = sketch->GetEntity();
	if (!sketchEntity) throw Kompas3DException("Не могу получить Эскиз, entityType=" + std::to_string(sketchEntity->type));
	def->SetSketch(sketchEntity);
	K5::ksExtrusionParamPtr param = def->ExtrusionParam();
	if (!param) throw Kompas3DException("Не могу получить ksExtrusionParam");
	param->depthNormal = depth1;
	param->depthReverse = depth2;
	entity->Create();
}

int BaseExtrusion::TYPE = KConst3D::o3d_baseExtrusion;