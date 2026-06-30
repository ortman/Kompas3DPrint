#ifndef _ComTest_Kompas3D_h_
#define _ComTest_Kompas3D_h_

#include "Doc3D.h"
#include "Sketch.h"
#include "Node/NodeMacro.h"
#include "Node/BaseExtrusion.h"
#include "Node/CutExtrusion.h"
#include "Node/MeshCopy.h"
#include "Node/CircularCopy.h"
#include "Panel.h"

#pragma once

class Kompas3D {
public:
	static KompasEvent<bool(Doc3D& doc, int docType)>  WhenCreateDocument;
	static KompasEvent<bool(Doc3D& doc, int docType)>  WhenOpenDocument;
	
	Kompas3D() = delete;
	static bool Connect(bool open = true, bool visible = true);
	static void Disconnect();
	static std::unique_ptr<Doc3D> GetActiveDocument3D();
	template <typename T>
	static T GetParamStruct(int type);
	static Panel CreatePanel(const std::string& name);
	static void Message(const std::string& txt);
	static void Error(const std::string& txt);
};

#endif
