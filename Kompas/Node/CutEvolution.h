#ifndef _Kompas3DPrint_CutEvolution_h_
#define _Kompas3DPrint_CutEvolution_h_

#include "Sketch.h"
#include <vector>

class CutEvolution : public Node {
public:
	static int TYPE;
	CutEvolution(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	CutEvolution(IUnknown* pEntity, IDispatch* pDefinition, Sketch& sketch, const Node node, const std::optional<std::string>& name = std::nullopt);
	CutEvolution(IUnknown* pEntity, IDispatch* pDefinition, Sketch& sketch, const std::vector<Node>& nodes, const std::optional<std::string>& name = std::nullopt);
};

#endif
