#ifndef _ComTest_Plane_h_
#define _ComTest_Plane_h_

#include "../Node.h"
#include "Face.h"
#include "Vertex.h"
#include "Axis.h"

class Plane : public Node {
public:
	struct Point2D {
		double x, y;
	};
	enum AxisType {
		OX = 0,
		OY = 1,
		OZ = 2
	};
	static int TYPE;
	Plane(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	Plane(IUnknown* pEntity, IDispatch* pDefinition = NULL) : Node(pEntity, pDefinition) {}
	Vertex::Point3D GetVector(AxisType type);
	Plane::Point2D Projection(const Vertex::Point3D& point);
	Vertex::Point3D Projection(const Plane::Point2D& point);
};

class ParallelPlane : public Plane {
public:
	static int TYPE;
	ParallelPlane(const Node& node) : Plane(node.pEntity, node.pDefinition) {}
	ParallelPlane(IUnknown* pEntity, IDispatch* pDefinition, const Face& planarFace, const Vertex& point, bool show = false);
};

class EdgePointPlane : public Plane {
public:
	static int TYPE;
	EdgePointPlane(const Node& node) : Plane(node.pEntity, node.pDefinition) {}
	EdgePointPlane(IUnknown* pEntity, IDispatch* pDefinition, const Axis& axis, const Vertex& point, bool show = false);
};

#endif
