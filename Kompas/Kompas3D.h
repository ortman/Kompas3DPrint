#ifndef _ComTest_Kompas3D_h_
#define _ComTest_Kompas3D_h_

#include "Doc3D.h"
//#include "Node/Sketch.h"

//#include "Node/NodeMacro.h"
//#include "Node/BaseExtrusion.h"
//#include "Node/CutExtrusion.h"
//#include "Node/CutEvolution.h"
//#include "Node/CutRotated.h"
//#include "Node/MeshCopy.h"
//#include "Node/CircularCopy.h"
//#include "Node/ThreadDesignation.h"
//#include "Node/CylindricSpiral.h"

//#include "Panel.h"

class Kompas3D {
public:
	class K3D_Kompas3D {
	public:
		virtual Doc3D GetActiveDocument3D() { return Doc3D(); }
		virtual void Message(const std::wstring& txt) {}
		virtual void Error(const std::wstring& txt) {}
		virtual std::wstring SystemPath(long type) { return std::wstring(); }
		virtual ~K3D_Kompas3D() = default;
	};
	class K3D_KS_Kompas3D;
	class K3D_API7_Kompas3D;

	inline static KompasEvent<bool(Doc3D& doc, int docType)>  WhenCreateDocument;
	inline static KompasEvent<bool(Doc3D& doc, int docType)>  WhenOpenDocument;
	
	Kompas3D() = delete;
	static Doc3D GetActiveDocument3D() { return kompas->GetActiveDocument3D(); }
	static Doc3D Open3D(std::string path, bool visible = true);
/*
 * 	template <typename T>
 * 	static T GetParamStruct(int type);
 * 	template <typename T>
 * 	static T ToApi7(IUnknown* k5);
 * 	template <typename T>
 * 	static T ToApi5(IUnknown* k7);
 */
	static void Message(const std::wstring& txt) { kompas->Message(txt); }
	static void Error(const std::wstring& txt) { kompas->Error(txt); }
	static std::wstring SystemPath(long type) { return kompas->SystemPath(type); }
	static std::wstring ConfigPath();
	static void RunCommand(uint32_t comm);
	
//	friend class Panel;
//	friend class KProcess3D;
	friend void SetKompas(std::unique_ptr<Kompas3D::K3D_Kompas3D>);

private:
	inline static std::unique_ptr<K3D_Kompas3D> kompas = std::make_unique<K3D_Kompas3D>();
//	static IUnknown* CreatePropertyManager();
//	static IUnknown* CreateProcessParam();
};

#endif
