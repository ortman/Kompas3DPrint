#ifndef _Kompas3DPrint_Vertex_h_
#define _Kompas3DPrint_Vertex_h_

#include "../Node.h"

class Vertex : public Node {
public:
	struct Point3D {
		double x, y, z;
	};
	static int TYPE;
	Vertex(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	Vertex(IUnknown* pEntity, IDispatch* pDefinition = NULL) : Node(pEntity, pDefinition) {}
	operator Point3D const ();
};

#endif
