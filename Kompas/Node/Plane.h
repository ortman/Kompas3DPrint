#ifndef _ComTest_Plane_h_
#define _ComTest_Plane_h_

#pragma once
#include "../Node.h"

class Plane : public Node {
public:
	static int TYPE;
	Plane(IUnknown* pEntity) : Node(pEntity) {}
};

//class PlaneXOY : public Plane {
//public:
//	static int TYPE;
//	PlaneXOY(IUnknown* pEntity) : Plane(pEntity) {}
//};
//
//class PlaneXOZ : public Plane {
//public:
//	static int TYPE;
//	PlaneXOZ(IUnknown* pEntity) : Plane(pEntity) {}
//};
//
//class PlaneYOZ : public Plane {
//public:
//	static int TYPE;
//	PlaneYOZ(IUnknown* pEntity) : Plane(pEntity) {}
//};


#endif
