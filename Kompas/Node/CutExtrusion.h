#ifndef _ComTest_CutExtrusion_h_
#define _ComTest_CutExtrusion_h_

#pragma once
#include "../Sketch.h"

class CutExtrusion : public Node {
public:
	static int TYPE;
	CutExtrusion(IUnknown* pSketch, std::unique_ptr<Sketch>& sketch, double depth1, double depth2 = 0.0, const std::optional<std::string>& name = std::nullopt);
};

#endif
