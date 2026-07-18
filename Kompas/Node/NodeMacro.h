#ifndef _ComTest_NodeMacro_h_
#define _ComTest_NodeMacro_h_

#include "../Node.h"
#include <vector>

class NodeMacro : public Node {
public:
	static int TYPE;
	NodeMacro(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	NodeMacro(IUnknown* pEntity, IDispatch* pDefinition = NULL, bool show = true, const std::optional<std::string>& name = std::nullopt);
	NodeMacro& Add(Node node);
	std::vector<Node> GetNodes();
	bool SetUserParam(void* param, size_t size, int cmd);
	bool GetUserParam(void* param, size_t size);
	NodeMacro& Show(bool show = true);
	NodeMacro& Hide() { return Show(false); }
};

#endif
