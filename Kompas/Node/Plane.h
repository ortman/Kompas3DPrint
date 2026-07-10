#ifndef _ComTest_Plane_h_
#define _ComTest_Plane_h_

#include "../Node.h"

class Plane : public Node {
public:
	static int TYPE;
	Plane(const Node& node) : Node(node.GetEntity()) {}
	Plane(IUnknown* pEntity) : Node(pEntity) {}
};

#endif
