#include "../ComKompas.h"
#include "CutEvolution.h"

CutEvolution::CutEvolution(IUnknown* pE, IDispatch* pD, Sketch& sketch, const Node node, const std::optional<std::string>& name) : Node(pE, pD) {
	sketch.EndEdit();
	K5::ksEntityPtr entity = pEntity;
	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
	K5::ksCutEvolutionDefinitionPtr def = pDefinition;
	if (!def) throw Kompas3DException("Не могу получить CutEvolutionDefinition, entityType=" + std::to_string(entity->type));
	K5::ksEntityPtr sketchEntity = sketch.pEntity;
	if (!sketchEntity) throw Kompas3DException("Не могу получить Эскиз");
	def->SetSketch(sketchEntity);
	K5::ksEntityCollectionPtr path = def->PathPartArray();
	if (!def) throw Kompas3DException("Не могу получить PathPartArray в CutEvolution");
	K5::ksEntityPtr nodeEntity = node.pEntity;
	if (nodeEntity) path->Add(nodeEntity);
	entity->Create();
}

CutEvolution::CutEvolution(IUnknown* pE, IDispatch* pD, Sketch& sketch, const std::vector<Node>& nodes, const std::optional<std::string>& name) : Node(pE, pD) {
	sketch.EndEdit();
	K5::ksEntityPtr entity = pEntity;
	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
	K5::ksCutEvolutionDefinitionPtr def = pDefinition;
	if (!def) throw Kompas3DException("Не могу получить CutEvolutionDefinition, entityType=" + std::to_string(entity->type));
	K5::ksEntityPtr sketchEntity = sketch.pEntity;
	if (!sketchEntity) throw Kompas3DException("Не могу получить Эскиз");
	def->SetSketch(sketchEntity);
	K5::ksEntityCollectionPtr path = def->PathPartArray();
	if (!def) throw Kompas3DException("Не могу получить PathPartArray в CutEvolution");
	for (const Node& node : nodes) {
		K5::ksEntityPtr nodeEntity = node.pEntity;
		if (nodeEntity) path->Add(nodeEntity);
	}
	entity->Create();
}

int CutEvolution::TYPE = KConst3D::o3d_cutEvolution;