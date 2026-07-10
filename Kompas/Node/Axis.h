#ifndef _ComTest_Axis_h_
#define _ComTest_Axis_h_

#include "../Node.h"

class Axis : public Node {
public:
	static int TYPE;
	Axis(const Node& node) : Node(node.GetEntity()) {}
	Axis(IUnknown* pEntity) : Node(pEntity) {}
};

#endif
