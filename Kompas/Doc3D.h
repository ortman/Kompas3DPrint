#ifndef _ComTest_Document3D_h_
#define _ComTest_Document3D_h_

#pragma once
#include "Part.h"
#include "KompasEvent.h"
#include "Node/NodeMacro.h"

class DocumentFileNotifyLoc;
class Doc3D {
public:
	enum Format {
		SAT  = 1,
		XT   = 2,
		STEP = 3,
		IGES = 4,
		VRLM = 5,
		STL  = 6,
		JT   = 8,
		STEP_AP214 = 214,
		STEP_AP242 = 242
	};
private:
	IUnknown* pDoc;
	DocumentFileNotifyLoc *comEvent = nullptr;

public:
	KompasEvent<void()>  WhenCloseDocument;
	KompasEvent<void()>  WhenSaveDocument;
	
	Doc3D(IUnknown* pDoc);
	~Doc3D();
	std::string GetPath();
	std::unique_ptr<Part> GetTopPart();
	std::unique_ptr<NodeMacro> GetEditMacroObject();
	bool SaveAs(Format format, const std::string& path) {
		return false;
	}
	static std::string_view GetExt(Format format);
};

#endif
