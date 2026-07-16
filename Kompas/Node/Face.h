#ifndef _Kompas3DPrint_Face_h_
#define _Kompas3DPrint_Face_h_

#include "../Node.h"

class Face : public Node {
public:
	static int TYPE;
	Face(const Node& node) : Node(node.GetEntity()) {}
	Face(IUnknown* pEntity) : Node(pEntity) {}
	bool IsPlanar();
	bool IsCylinder();
};

#endif
