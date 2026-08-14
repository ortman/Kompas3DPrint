#ifndef _Kompas3DPrint_Face_h_
#define _Kompas3DPrint_Face_h_

#include "../Node.h"

class Face : public Node {
private:
	class K3D_Face : public Node::K3D_Node {
	};

public:
	static int TYPE;
	Face(const Node& node) : Node(node) {}
	Face(K3D_Face p) : Node(p) {}
	bool IsPlanar();
	bool IsCylinder();
};

#endif
