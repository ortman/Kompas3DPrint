#ifndef _ComTest_Kompas3D_h_
#define _ComTest_Kompas3D_h_

#include "Doc3D.h"
#include "Node/Sketch.h"

#include "Node/NodeMacro.h"
#include "Node/BaseExtrusion.h"
#include "Node/CutExtrusion.h"
#include "Node/MeshCopy.h"
#include "Node/CircularCopy.h"
#include "Node/ThreadDesignation.h"

#include "Panel.h"

class Kompas3D {
public:
	static KompasEvent<bool(Doc3D& doc, int docType)>  WhenCreateDocument;
	static KompasEvent<bool(Doc3D& doc, int docType)>  WhenOpenDocument;
	
	Kompas3D() = delete;
	static bool Connect(bool open = false, bool visible = true);
	static void Disconnect();
	static Doc3D GetActiveDocument3D();
	static Doc3D Open3D(std::string path, bool visible = true);
	template <typename T>
	static T GetParamStruct(int type);
	template <typename T>
	static T ToApi7(IUnknown* k5);
	template <typename T>
	static T ToApi5(IUnknown* k7);
	static void Message(const std::string& txt);
	static void Error(const std::string& txt);
	static std::string SystemPath(long type);
	static std::string ConfigPath();
	
	friend class Panel;
private:
	static IUnknown* CreatePropertyManager();
};

#endif
