#include "../ComKompas.h"
#include "CircularCopy.h"


CircularCopy::CircularCopy(IUnknown* p,
		int CCount, double CStep, bool CFactor,
		int RCount, double RStep, bool RFactor,
		std::unique_ptr<Axis> axis,
		const std::vector<std::unique_ptr<Node>>& nodes,
		const std::optional<std::string>& name) : Node(p) {
	K5::ksEntityPtr entity = pEntity;
	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
	K5::ksCircularCopyDefinitionPtr def = entity->GetDefinition();
	if (!def) throw Kompas3DException("Не могу получить CircularCopyDefinition, entityType=" + std::to_string(entity->type));
	def->count1 = RCount;
	def->step1 = RStep;
	def->factor1 = RFactor;
	def->count2 = CCount;
	def->step2 = CStep;
	def->factor2 = CFactor;
	K5::ksEntityPtr axisEntity = axis->GetEntity();
	if (!axisEntity) throw Kompas3DException("Не могу получить Ось для Эскиза");
	def->SetAxis(axisEntity);
	K5::ksEntityCollectionPtr operations = def->GetOperationArray();
	if (!def) throw Kompas3DException("Не могу получить OperationArray в CircularCopy");
	for (const std::unique_ptr<Node>& node : nodes) {
		K5::ksEntityPtr nodeEntity = node->GetEntity();
		if (nodeEntity) operations->Add(nodeEntity);
	}

	entity->Create();
}

CircularCopy::CircularCopy(IUnknown* p,
		int CCount, double CStep, bool CFactor,
		int RCount, double RStep, bool RFactor,
		std::unique_ptr<Axis> axis,
		const std::unique_ptr<Node> node,
		const std::optional<std::string>& name) : Node(p) {
	K5::ksEntityPtr entity = pEntity;
	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
	K5::ksCircularCopyDefinitionPtr def = entity->GetDefinition();
	if (!def) throw Kompas3DException("Не могу получить CircularCopyDefinition, entityType=" + std::to_string(entity->type));
	def->count1 = RCount;
	def->step1 = RStep;
	def->factor1 = RFactor;
	def->count2 = CCount;
	def->step2 = CStep;
	def->factor2 = CFactor;
	K5::ksEntityPtr axisEntity = axis->GetEntity();
	if (!axisEntity) throw Kompas3DException("Не могу получить Ось для Эскиза");
	def->SetAxis(axisEntity);
	K5::ksEntityCollectionPtr operations = def->GetOperationArray();
	if (!def) throw Kompas3DException("Не могу получить OperationArray в CircularCopy");
	K5::ksEntityPtr nodeEntity = node->GetEntity();
	if (nodeEntity) operations->Add(nodeEntity);
	entity->Create();
}

int CircularCopy::TYPE = KConst3D::o3d_circularCopy;
