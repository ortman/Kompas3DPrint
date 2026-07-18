#ifndef _Kompas3DPrint_Vertex_h_
#define _Kompas3DPrint_Vertex_h_

#include "../Node.h"

class Vertex : public Node {
public:
	static int TYPE;
	Vertex(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	Vertex(IUnknown* pEntity, IDispatch* pDefinition = NULL) : Node(pEntity, pDefinition) {}
};

#endif
