#ifndef _KompasAPI_CircularCopy_h_
#define _KompasAPI_CircularCopy_h_

#pragma once
#include "../Node.h"
#include "Axis.h"
#include <vector>

class CircularCopy : public Node {
public:
	static int TYPE;
	CircularCopy(const Node& node) : Node(node.GetEntity()) {}
	CircularCopy(IUnknown* pMesh,
		int CCount, double CStep, bool CFactor,
		int RCount, double RStep, bool RFactor,
		Axis axis,
		const std::vector<Node>& nodes,
		const std::optional<std::string>& name = std::nullopt);
	CircularCopy(IUnknown* pMesh,
		int CCount, double CStep, bool CFactor,
		int RCount, double RStep, bool RFactor,
		Axis axis,
		const Node node,
		const std::optional<std::string>& name = std::nullopt);
	CircularCopy& SetCircularParam(int count, double step, bool factor);
	CircularCopy& SetRadialParam(int count, double step, bool factor);
};

#endif
