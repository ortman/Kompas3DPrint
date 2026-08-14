#ifndef _Kompas3DPrint_Vertex_h_
#define _Kompas3DPrint_Vertex_h_

#include "../Node.h"

class Vertex : public Node {
private:
	class K3D_Vertex : public Node::K3D_Node {
	};
public:
	struct Point3D {
		double x, y, z;
	};
	static int TYPE;
	Vertex(const Node& node) : Node(node) {}
	Vertex(K3D_Vertex p) : Node(p) {}
	operator Point3D const ();
};

#endif
