#ifndef _Kompas3DPrint_ThreadDesignation_h_
#define _Kompas3DPrint_ThreadDesignation_h_

#include "Edge.h"

class ThreadDesignation : public Node {
public:
	static int TYPE;
	ThreadDesignation(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	double GetLength() const;
	double GetPitch() const;
	double GetDiameter() const;
	bool IsForwardDir() const;
	bool IsOutside() const;
	Edge GetBeginEdge() const;
	Edge GetEndEdge() const;
};

#endif
