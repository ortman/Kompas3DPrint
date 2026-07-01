#ifndef _ComTest_BaseExtrusion_h_
#define _ComTest_BaseExtrusion_h_

#pragma once
#include "../Sketch.h"

class BaseExtrusion : public Node {
public:
	static int TYPE;
	BaseExtrusion(const Node& node) : Node(node.GetEntity()) {}
	BaseExtrusion(IUnknown* pSketch, std::unique_ptr<Sketch>& sketch, double depth1, double depth2 = 0.0, const std::optional<std::string>& name = std::nullopt);
	BaseExtrusion& SetDepth1(double depth);
	BaseExtrusion& SetDepth2(double depth);
};

#endif
