#ifndef _Kompas3DPrint_Vertex_h_
#define _Kompas3DPrint_Vertex_h_

#include "../Node.h"

class Vertex : public Node {
public:
	static int TYPE;
	Vertex(const Node& node) : Node(node.GetEntity()) {}
	Vertex(IUnknown* pEntity) : Node(pEntity) {}
};

#endif
