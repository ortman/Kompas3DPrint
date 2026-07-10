#ifndef _ComTest_Document3D_h_
#define _ComTest_Document3D_h_

#include "Part.h"
#include "KompasEvent.h"
#include "Node/NodeMacro.h"

#define SETTINGS_LINEAR_MAX 1.0
#define SETTINGS_LINEAR_MIN 0.001
#define SETTINGS_ANGLE_MAX  90.0
#define SETTINGS_ANGLE_MIN  0.1
#define SETTINGS_RIDGE_MIN  0.01
#define SETTINGS_RIDGE_MAX  100.0

class DocumentFileNotifyLoc;
class Doc3D {
public:
	struct Format {
		enum Value {
			ACIS = 1,
			PARASOLID = 2,
			STEP = 3,
			IGES = 4,
			VRLM = 5,
			STL  = 6,
			JT   = 8,
			STEP_AP203 = 203,
			STEP_AP214 = 214,
			STEP_AP242 = 242
		};
		
		Value value;
		
		Format() : value(STL) {}
		
		constexpr Format(Value v) : value(v) {}
		
		constexpr operator int() const { return value; }
		
		constexpr friend bool operator==(Format lhs, Format rhs) noexcept {
	        return lhs.value == rhs.value;
	    }
	    
		constexpr const char* Ext() const {
			switch (value) {
				case STL : return ".stl";
				case STEP_AP203:
				case STEP_AP214:
				case STEP_AP242:
				case STEP: return ".step";
				case VRLM: return ".vrlm";
				case IGES: return ".igs";
				case ACIS: return ".sat";
				case PARASOLID: return ".x_t";
				case JT  : return ".jt";
				default  : return "";
			}
		}

		Format(const std::string& v) : value(STL) {
			if (v == "STEP") value = STEP;
			else if (v == "VRLM") value = VRLM;
			else if (v == "IGES") value = IGES;
			else if (v == "ACIS") value = ACIS;
			else if (v == "JT")   value = JT;
			else if (v == "PARASOLID")  value = PARASOLID;
			else if (v == "STEP_AP203") value = STEP_AP203;
			else if (v == "STEP_AP214") value = STEP_AP214;
			else if (v == "STEP_AP242") value = STEP_AP242;
		}

		constexpr const char* Name() const {
			switch (value) {
				case STL : return "STL";
				case STEP: return "STEP";
				case VRLM: return "VRLM";
				case IGES: return "IGES";
				case ACIS: return "ACIS";
				case JT  : return "JT";
				case PARASOLID : return "PARASOLID";
				case STEP_AP203: return "STEP_AP203";
				case STEP_AP214: return "STEP_AP214";
				case STEP_AP242: return "STEP_AP242";
				default  : return "";
			}
		}
	};
	
	enum Unit {
		SM = 0,
		MM = 1,
		DM = 2,
		M = 3,
		Document = 4
	};
	
	struct ExportParams {
		Format format;
		bool objBody;
		bool objSurface;
		Unit units;
		bool formatBIN;
		bool isLinear;
		double linearVal;
		bool isAngle;
		double angleVal;
		bool isRidge;
		double ridgeVal;
	};
	
private:
	IUnknown* pDoc;
	DocumentFileNotifyLoc *comEvent = nullptr;

public:
	KompasEvent<void()> WhenBeginCloseDocument;
	KompasEvent<void()> WhenCloseDocument;
	KompasEvent<void()> WhenBeginSaveDocument;
	KompasEvent<void()> WhenSaveDocument;
	KompasEvent<void()> WhenActiveDocument;
	
	Doc3D(IUnknown* pDoc);
	~Doc3D();
	std::string GetPath();
	Part GetTopPart();
	NodeMacro GetEditMacroObject();
	bool SaveAs(const ExportParams& params, const std::string& path);
	Doc3D& Reopen();
	void Close();
	int GetEmbodimentsCount();
	std::string GetEmbodimentName(int i);
	Part GetEmbodiment(int i);
	operator bool() const { return pDoc; }
};

#endif
