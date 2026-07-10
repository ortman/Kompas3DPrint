#ifndef _ComTest_MeshCopy_h_
#define _ComTest_MeshCopy_h_

#pragma once
#include "../Node.h"
#include <vector>

class MeshCopy : public Node {
public:
	static int TYPE;
	MeshCopy(const Node& node) : Node(node.GetEntity()) {}
	MeshCopy(IUnknown* pMesh, int count1, double step1, int count2, double step2, const std::vector<Node>& nodes, const std::optional<std::string>& name = std::nullopt);
	MeshCopy(IUnknown* pMesh, int count1, double step1, int count2, double step2, const Node node, const std::optional<std::string>& name = std::nullopt);
	MeshCopy& SetParam1(int count, double step);
	MeshCopy& SetParam2(int count, double step);
};

#endif
