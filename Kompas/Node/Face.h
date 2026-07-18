#ifndef _Kompas3DPrint_Face_h_
#define _Kompas3DPrint_Face_h_

#include "../Node.h"

class Face : public Node {
public:
	static int TYPE;
	Face(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	Face(IUnknown* pEntity, IDispatch* pDefinition = NULL) : Node(pEntity, pDefinition) {}
	bool IsPlanar();
	bool IsCylinder();
};

#endif
