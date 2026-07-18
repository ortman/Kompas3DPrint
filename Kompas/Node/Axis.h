#ifndef _ComTest_Axis_h_
#define _ComTest_Axis_h_

#include "Face.h"

class Axis : public Node {
public:
	static int TYPE;
	Axis(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	Axis(IUnknown* pEntity, IDispatch* pDefinition = NULL) : Node(pEntity, pDefinition) {}
};

class ConeAxis : public Axis {
public:
	static int TYPE;
	ConeAxis(const Node& node) : Axis(node.pEntity, node.pDefinition) {}
	ConeAxis(IUnknown* pEntity, IDispatch* pDefinition, const Face& coneFace);
};

#endif
