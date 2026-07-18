#ifndef _ComTest_Axis_h_
#define _ComTest_Axis_h_

#include "../Node.h"

class Axis : public Node {
public:
	static int TYPE;
	Axis(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	Axis(IUnknown* pEntity, IDispatch* pDefinition = NULL) : Node(pEntity, pDefinition) {}
};

#endif
