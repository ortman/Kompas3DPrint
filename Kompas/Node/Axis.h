#ifndef _ComTest_Axis_h_
#define _ComTest_Axis_h_
#pragma once
#include "../Node.h"

class Axis : public Node {
public:
	static int TYPE;
	Axis(IUnknown* pEntity) : Node(pEntity) {}
};

#endif
