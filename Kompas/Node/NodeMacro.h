#ifndef _ComTest_NodeMacro_h_
#define _ComTest_NodeMacro_h_

//#pragma once
#include "../Node.h"
#include <vector>

class NodeMacro : public Node {
public:
	static int TYPE;
	NodeMacro(const Node& node) : Node(node.GetEntity()) {}
	NodeMacro(IUnknown* pEntity, bool show = true, const std::optional<std::string>& name = std::nullopt);
	NodeMacro& Add(Node& node);
	template <typename T>
	NodeMacro& Add(std::unique_ptr<T>& node) {
		static_assert(std::is_base_of<Node, T>::value, "T must be derived from Node");
		return Add(*node);
	}
	std::vector<std::unique_ptr<Node>> GetNodes();
	bool SetUserParam(void* param, size_t size, int cmd);
	bool GetUserParam(void* param, size_t size);
	NodeMacro& Show(bool show = true);
	NodeMacro& Hide() { return Show(false); }
};

#endif
