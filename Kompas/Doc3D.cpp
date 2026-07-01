#include "ComKompas.h"
#include "Doc3D.h"

class DocumentFileNotifyLoc : public ComEvent {
private:
	Doc3D* doc;
public:
	DocumentFileNotifyLoc(Doc3D* d) : ComEvent(K5::DIID_ksDocumentFileNotify), doc(d) {}
	
    STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
                        DISPPARAMS* pDispParams, VARIANT* pVarResult,
                        EXCEPINFO* pExcepInfo, UINT* puArgErr) override {
		switch((int)dispIdMember) {
			case KConst::kdCloseDocument:
				if (doc) doc->WhenCloseDocument();
				break;
			case KConst::kdSaveDocument:
				if (doc) doc->WhenSaveDocument();
				break;
		}
        return S_OK;
    }
};

Doc3D::Doc3D(IUnknown* d) : pDoc(d) {
	if (pDoc) pDoc->AddRef();
	comEvent = new DocumentFileNotifyLoc(this);
	comEvent->Subscribe(pDoc);
}

Doc3D::~Doc3D() {
    if (comEvent) {
		comEvent->Unsubscribe(pDoc);
		delete comEvent;
    }
    if (pDoc) pDoc->Release();
}

std::string Doc3D::GetPath() {
	K5::ksDocument3DPtr doc = pDoc;
	if (!doc) return std::string();
	return Node::Cp1251ToUtf8(doc->fileName);
}

std::unique_ptr<Part> Doc3D::GetTopPart() {
	K5::ksDocument3DPtr doc = pDoc;
	if (!doc) return nullptr;
	//doc->AddRef();
	K5::ksPartPtr top = doc->GetPart(KConst3D::pTop_Part);
	if (!top) throw Kompas3DException("Не могу получить Top Part");
	return std::make_unique<Part>(pDoc, top.GetInterfacePtr());
}

//bool Doc3D::IsEditObject() {
//	K5::ksDocument3DPtr doc = pDoc;
//	return doc && doc->IsEditMode();
//}

std::unique_ptr<NodeMacro> Doc3D::GetEditMacroObject() {
	K5::ksDocument3DPtr doc = pDoc;
	if (!doc || !doc->IsEditMode()) return nullptr;
	IUnknown* macro = doc->GetEditMacroObject();
	if (!macro) return nullptr;
	return std::make_unique<NodeMacro>(macro);
}


std::string_view Doc3D::GetExt(Format format) {
	switch(format) {
		case SAT : return "sat";
		case XT  : return "x_t";
		case IGES: return "igs";
		case VRLM: return "vrlm";
		case JT  : return "jt";
		case STL : return "stl";
		case STEP:
		case STEP_AP214:
		case STEP_AP242: return "step";
		default: return "";
	}
}