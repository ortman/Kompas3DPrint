#ifndef _ComTest_Document3D_h_
#define _ComTest_Document3D_h_

#pragma once
#include "Part.h"
#include "KompasEvent.h"

class DocumentFileNotifyLoc;
class Doc3D {
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
	
};

#endif
