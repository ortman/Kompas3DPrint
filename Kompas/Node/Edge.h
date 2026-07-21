#ifndef _Kompas3DPrint_Edge_h_
#define _Kompas3DPrint_Edge_h_

#include "Face.h"
#include "Vertex.h"

class Edge : public Node {
public:
	static int TYPE;
	Edge(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	Edge(IUnknown* pEntity, IDispatch* pDefinition = NULL) : Node(pEntity, pDefinition) {}
	Face LeftFace() const;
	Face RightFace() const;
	Vertex GetBeginVertex() const;
	Vertex::Point3D GetOrigin() const;
};

#endif
