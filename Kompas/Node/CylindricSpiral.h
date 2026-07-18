#ifndef _Kompas3DPrint_CylindricSpiral_h_
#define _Kompas3DPrint_CylindricSpiral_h_

#include "Plane.h"

class CylindricSpiral : public Node {
public:
	enum Mode {
		ModeStepHeight = 1
	};
	
	static int TYPE;
	CylindricSpiral(const Node& node) : Node(node.pEntity, node.pDefinition) {}
	CylindricSpiral(IUnknown* pEntity, IDispatch* pDefinition = NULL) : Node(pEntity, pDefinition) {}
	CylindricSpiral& SetDiam(double d);
	CylindricSpiral& SetStep(double step);
	CylindricSpiral& SetHeight(double height);
	CylindricSpiral& TurnDir(bool dir);
	CylindricSpiral& BuildDir(bool dir);
	CylindricSpiral& BuildMode(Mode mode);
	CylindricSpiral& SetPlane(const Plane& plane);
	CylindricSpiral& SetLocation(double x, double y);
	bool Create();
	Vertex GetBeginVertex() const;
};

#endif
