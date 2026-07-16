#ifndef _ComTest_Plane_h_
#define _ComTest_Plane_h_

#include "../Node.h"
#include "Face.h"

class Plane : public Node {
public:
	static int TYPE;
	Plane(const Node& node) : Node(node.GetEntity()) {}
	Plane(IUnknown* pEntity) : Node(pEntity) {}
};

class ParallelPlane : public Plane {
public:
	static int TYPE;
	ParallelPlane(const Node& node) : Plane(node.GetEntity()) {}
	ParallelPlane(IUnknown* pEntity, const Face& planarFace/*, point */);
};

#endif
