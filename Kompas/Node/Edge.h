#ifndef _Kompas3DPrint_Edge_h_
#define _Kompas3DPrint_Edge_h_

#include "Face.h"

class Edge : public Node {
public:
	static int TYPE;
	Edge(const Node& node) : Node(node.GetEntity()) {}
	Edge(IUnknown* pEntity) : Node(pEntity) {}
	Face LeftFace() const;
	Face RightFace() const;
};

#endif
