/*
 * #include "MeshCopy.h"
 * 
 * MeshCopy::MeshCopy(IUnknown* pE, IDispatch* pD, int count1, double step1,
 * 		int count2, double step2,
 * 		const std::vector<Node>& nodes,
 * 		const std::optional<std::string>& name) : Node(pE, pD) {
 * 	K5::ksEntityPtr entity = pEntity;
 * 	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
 * 	K5::ksMeshCopyDefinitionPtr def = pDefinition;
 * 	if (!def) throw Kompas3DException("Не могу получить ksMeshCopyDefinition, entityType=" + std::to_string(entity->type));
 * 	def->count1 = count1;
 * 	def->step1 = step1;
 * 	def->count2 = count2;
 * 	def->step2 = step2;
 * 	K5::ksEntityCollectionPtr operations = def->OperationArray();
 * 	if (!def) throw Kompas3DException("Не могу получить OperationArray в MeshCopy");
 * 	for (const Node& node : nodes) {
 * 		K5::ksEntityPtr nodeEntity = node.pEntity;
 * 		if (nodeEntity) operations->Add(nodeEntity);
 * 	}
 * 	entity->Create();
 * }
 * 
 * MeshCopy::MeshCopy(IUnknown* pE, IDispatch* pD, int count1, double step1,
 * 		int count2, double step2,
 * 		const Node node,
 * 		const std::optional<std::string>& name) : Node(pE, pD) {
 * 	K5::ksEntityPtr entity = pEntity;
 * 	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
 * 	K5::ksMeshCopyDefinitionPtr def = pDefinition;
 * 	if (!def) throw Kompas3DException("Не могу получить ksMeshCopyDefinition, entityType=" + std::to_string(entity->type));
 * 	def->count1 = count1;
 * 	def->step1 = step1;
 * 	def->count2 = count2;
 * 	def->step2 = step2;
 * 	K5::ksEntityCollectionPtr operations = def->OperationArray();
 * 	if (!def) throw Kompas3DException("Не могу получить OperationArray в MeshCopy");
 * 	K5::ksEntityPtr nodeEntity = node.pEntity;
 * 	if (nodeEntity) operations->Add(nodeEntity);
 * 	entity->Create();
 * }
 * 
 * MeshCopy& MeshCopy::SetParam1(int count, double step) {
 * 	K5::ksMeshCopyDefinitionPtr def = pDefinition;
 * 	if (!def) throw Kompas3DException("Не могу получить ksMeshCopyDefinition");
 * 	def->count1 = count;
 * 	def->step1 = step;
 * 	return *this;
 * }
 * 
 * MeshCopy& MeshCopy::SetParam2(int count, double step) {
 * 	K5::ksMeshCopyDefinitionPtr def = pDefinition;
 * 	if (!def) throw Kompas3DException("Не могу получить ksMeshCopyDefinition");
 * 	def->count2 = count;
 * 	def->step2 = step;
 * 	return *this;
 * }
 * 
 * int MeshCopy::TYPE = KConst3D::o3d_meshCopy;
 */
