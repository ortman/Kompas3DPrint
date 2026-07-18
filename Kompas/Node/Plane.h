#ifndef _ComTest_Plane_h_
#define _ComTest_Plane_h_

#include "../Node.h"
#include "Face.h"
#include "Vertex.h"

class Plane : public Node {
public:
	static int TYPE;
	Plane(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	Plane(IUnknown* pEntity, IDispatch* pDefinition = NULL) : Node(pEntity, pDefinition) {}
};

class ParallelPlane : public Plane {
public:
	static int TYPE;
	ParallelPlane(const Node& node) : Plane(node.pEntity, node.pDefinition) {}
	ParallelPlane(IUnknown* pEntity, IDispatch* pDefinition, const Face& planarFace, const Vertex& point);
};

#endif
