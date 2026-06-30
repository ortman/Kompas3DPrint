#ifndef _ComTest_NodeMacro_h_
#define _ComTest_NodeMacro_h_

#include "../Node.h"

class NodeMacro : public Node {
public:
	static int TYPE;
	NodeMacro(IUnknown* pEntity, bool show = true, const std::optional<std::string>& name = std::nullopt);
	NodeMacro& Add(Node& node);
	template <typename T>
	NodeMacro& Add(std::unique_ptr<T>& node) {
		static_assert(std::is_base_of<Node, T>::value, "T must be derived from Node");
		return Add(*node);
	}
	NodeMacro& Update();
};

#endif
