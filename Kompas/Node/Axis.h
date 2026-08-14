#ifndef _ComTest_Axis_h_
#define _ComTest_Axis_h_

#include "Face.h"
#include "Vertex.h"

class Axis : public Node {
private:
	class K3D_Axis : public Node::K3D_Node {
	};

public:
	static int TYPE;
	Axis(const Node& node) : Node(node) {}
	Axis(K3D_Axis p) : Node(p) {}
	Vertex::Point3D GetFirstPoint();
	Vertex::Point3D GetLastPoint();
};

/*
class ConeAxis : public Axis {
public:
	static int TYPE;
	ConeAxis(const Node& node) : Axis(node.pEntity, node.pDefinition) {}
	ConeAxis(IUnknown* pEntity, IDispatch* pDefinition, const Face& coneFace, bool show = false);
};
*/

#endif
