#include "../ComKompas.h"
#include "NodeMacro.h"

NodeMacro::NodeMacro(IUnknown* p, bool show, const std::optional<std::string>& name) : Node(p) {
	K5::ksEntityPtr entity = pEntity;
	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
	K5::ksMacro3DDefinitionPtr def = entity->GetDefinition();
	if (!def) throw Kompas3DException("Не могу получить Macro3DDefinition, entityType=" + std::to_string(entity->type));
	def->StaffVisible = show;
	entity->Create();
}

NodeMacro& NodeMacro::Add(Node& node) {
	K5::ksEntityPtr entity = pEntity;
	if (entity) {
		K5::ksMacro3DDefinitionPtr def = entity->GetDefinition();
		K5::ksEntityPtr entity = node.GetEntity();
		if (def && entity) def->Add(entity);
	}
	return *this;
}

NodeMacro& NodeMacro::Update() {
	K5::ksEntityPtr entity = pEntity;
	if (entity) entity->Update();
	return *this;
}

int NodeMacro::TYPE = KConst3D::o3d_MacroObject;