#ifndef _ComTest_CutExtrusion_h_
#define _ComTest_CutExtrusion_h_

#include "Sketch.h"

class CutExtrusion : public Node {
public:
	static int TYPE;
	CutExtrusion(const Node& node) : Node(node.GetEntity()) {}
	CutExtrusion(IUnknown* pEntity, Sketch& sketch, double depth1, double depth2 = 0.0, const std::optional<std::string>& name = std::nullopt);
	CutExtrusion& SetDepth1(double depth);
	CutExtrusion& SetDepth2(double depth);
};

#endif
