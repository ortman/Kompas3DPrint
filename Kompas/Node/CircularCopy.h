#ifndef _KompasAPI_CircularCopy_h_
#define _KompasAPI_CircularCopy_h_

#pragma once
#include "../Node.h"
#include "Axis.h"
#include <vector>

class CircularCopy : public Node {
public:
	static int TYPE;
	CircularCopy(IUnknown* pMesh,
		int CCount, double CStep, bool CFactor,
		int RCount, double RStep, bool RFactor,
		std::unique_ptr<Axis> axis,
		const std::vector<std::unique_ptr<Node>>& nodes,
		const std::optional<std::string>& name = std::nullopt);
	CircularCopy(IUnknown* pMesh,
		int CCount, double CStep, bool CFactor,
		int RCount, double RStep, bool RFactor,
		std::unique_ptr<Axis> axis,
		const std::unique_ptr<Node> node,
		const std::optional<std::string>& name = std::nullopt);
};

#endif
