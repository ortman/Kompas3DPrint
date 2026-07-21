#ifndef _Kompas3DPrint_CutRotated_h_
#define _Kompas3DPrint_CutRotated_h_

#include "Sketch.h"
#include "Axis.h"

class CutRotated : public Node {
public:
	static int TYPE;
	CutRotated(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	CutRotated(IUnknown* pEntity, IDispatch* pDefinition, Sketch& sketch, const Axis& axis, const std::optional<std::string>& name = std::nullopt);
};

#endif
