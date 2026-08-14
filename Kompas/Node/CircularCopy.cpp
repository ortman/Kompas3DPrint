/*
 * #include "CircularCopy.h"
 * 
 * CircularCopy::CircularCopy(IUnknown* pE, IDispatch* pD,
 * 		int CCount, double CStep, bool CFactor,
 * 		int RCount, double RStep, bool RFactor,
 * 		Axis axis,
 * 		const std::vector<Node>& nodes,
 * 		const std::optional<std::string>& name) : Node(pE, pD) {
 * 	K5::ksEntityPtr entity = pEntity;
 * 	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
 * 	K5::ksCircularCopyDefinitionPtr def = pDefinition;
 * 	if (!def) throw Kompas3DException("Не могу получить CircularCopyDefinition, entityType=" + std::to_string(entity->type));
 * 	def->count1 = RCount;
 * 	def->step1 = RStep;
 * 	def->factor1 = RFactor;
 * 	def->count2 = CCount;
 * 	def->step2 = CStep;
 * 	def->factor2 = CFactor;
 * 	K5::ksEntityPtr axisEntity = axis.pEntity;
 * 	if (!axisEntity) throw Kompas3DException("Не могу получить Ось для Эскиза");
 * 	def->SetAxis(axisEntity);
 * 	K5::ksEntityCollectionPtr operations = def->GetOperationArray();
 * 	if (!def) throw Kompas3DException("Не могу получить OperationArray в CircularCopy");
 * 	for (const Node& node : nodes) {
 * 		K5::ksEntityPtr nodeEntity = node.pEntity;
 * 		if (nodeEntity) operations->Add(nodeEntity);
 * 	}
 * 
 * 	entity->Create();
 * }
 * 
 * CircularCopy::CircularCopy(IUnknown* pE, IDispatch* pD,
 * 		int CCount, double CStep, bool CFactor,
 * 		int RCount, double RStep, bool RFactor,
 * 		Axis axis,
 * 		const Node node,
 * 		const std::optional<std::string>& name) : Node(pE, pD) {
 * 	K5::ksEntityPtr entity = pEntity;
 * 	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
 * 	K5::ksCircularCopyDefinitionPtr def = pDefinition;
 * 	if (!def) throw Kompas3DException("Не могу получить CircularCopyDefinition, entityType=" + std::to_string(entity->type));
 * 	def->count1 = RCount;
 * 	def->step1 = RStep;
 * 	def->factor1 = RFactor;
 * 	def->count2 = CCount;
 * 	def->step2 = CStep;
 * 	def->factor2 = CFactor;
 * 	K5::ksEntityPtr axisEntity = axis.pEntity;
 * 	if (!axisEntity) throw Kompas3DException("Не могу получить Ось для Эскиза");
 * 	def->SetAxis(axisEntity);
 * 	K5::ksEntityCollectionPtr operations = def->GetOperationArray();
 * 	if (!def) throw Kompas3DException("Не могу получить OperationArray в CircularCopy");
 * 	K5::ksEntityPtr nodeEntity = node.pEntity;
 * 	if (nodeEntity) operations->Add(nodeEntity);
 * 	entity->Create();
 * }
 * 
 * CircularCopy& CircularCopy::SetCircularParam(int count, double step, bool factor) {
 * 	K5::ksCircularCopyDefinitionPtr def = pDefinition;
 * 	if (!def) throw Kompas3DException("Не могу получить ksMeshCopyDefinition");
 * 	def->count2 = count;
 * 	def->step2 = step;
 * 	def->factor2 = factor;
 * 	return *this;
 * }
 * 
 * CircularCopy& CircularCopy::SetRadialParam(int count, double step, bool factor) {
 * 	K5::ksCircularCopyDefinitionPtr def = pDefinition;
 * 	if (!def) throw Kompas3DException("Не могу получить ksMeshCopyDefinition");
 * 	def->count1 = count;
 * 	def->step1 = step;
 * 	def->factor1 = factor;
 * 	return *this;
 * }
 * 
 * int CircularCopy::TYPE = KConst3D::o3d_circularCopy;
 */
