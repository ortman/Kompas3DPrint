#define _USE_MATH_DEFINES
#include "ComKompas.h"
#include "Doc3D.h"
#include "Kompas3D.h"
#include <filesystem>

class DocumentFileNotifyLoc : public ComEvent {
private:
	Doc3D* doc;
public:
	DocumentFileNotifyLoc(Doc3D* d) : ComEvent(K5::DIID_ksDocumentFileNotify), doc(d) {}
	
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
	                    DISPPARAMS* pDispParams, VARIANT* pVarResult,
	                    EXCEPINFO* pExcepInfo, UINT* puArgErr) override {
		switch((int)dispIdMember) {
			case KConst::kdBeginCloseDocument:
				if (doc) doc->WhenBeginCloseDocument();
				break;
			case KConst::kdCloseDocument:
				if (doc) doc->WhenCloseDocument();
				break;
			case KConst::kdBeginSaveDocument:
				if (doc) doc->WhenBeginSaveDocument();
				break;
			case KConst::kdSaveDocument:
				if (doc) doc->WhenSaveDocument();
				break;
			case KConst::kdActiveDocument:
				if (doc) doc->WhenActiveDocument();
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
	if (!doc) throw Kompas3DException("Потерян указатель на документ");
	return Node::Cp1251ToUtf8(doc->fileName);
}

Part Doc3D::GetTopPart() {
	K5::ksDocument3DPtr doc = pDoc;
	if (!doc) throw Kompas3DException("Потерян указатель на документ");
	K5::ksPartPtr top = doc->GetPart(KConst3D::pTop_Part);
	if (!top) throw Kompas3DException("Не могу получить Top Part");
	return Part(pDoc, top.GetInterfacePtr());
}

NodeMacro Doc3D::GetEditMacroObject() {
	K5::ksDocument3DPtr doc = pDoc;
	if (!doc || !doc->IsEditMode()) return nullptr;
	IUnknown* macro = doc->GetEditMacroObject();
	return macro ? NodeMacro(macro) : NodeMacro(nullptr);
}

Doc3D& Doc3D::Reopen() {
	K5::ksDocument3DPtr doc = pDoc;
	if (!doc) throw Kompas3DException("Потерян указатель на документ");
	std::string path = GetPath();
	if (!path.empty()) {
		Close();
	}
	return *this;
}

void Doc3D::Close() {
	K5::ksDocument3DPtr doc = pDoc;
	if (doc) doc->close();
}

bool Doc3D::SaveAs(const ExportParams& params, const std::string& path) {
	Kompas3D::Error(path);
	K5::ksDocument3DPtr doc = pDoc;
	if (!doc) throw Kompas3DException("Потерян указатель на документ");
	K5::ksAdditionFormatParamPtr formatParam = doc->AdditionFormatParam();
	formatParam->Init();
	formatParam->SetObjectsOptions(KConst3D::ksD3COBodyes, params.objBody);
	formatParam->SetObjectsOptions(KConst3D::ksD3COSurfaces, params.objSurface);
	formatParam->format = (int)params.format;
	formatParam->topolgyIncluded = false;
	formatParam->lengthUnits = params.units;
	formatParam->formatBinary = !params.formatBIN;
	long stepType = 0;
	if (params.isLinear) {
		stepType |= KConst3D::ksSpaceStep;
		formatParam->step = params.linearVal;
	} else {
		formatParam->step = SETTINGS_LINEAR_MAX;
	}
	if (params.isAngle) {
		stepType |= KConst3D::ksDeviationStep;
		formatParam->angle = params.angleVal * M_PI / 180.0;
	} else {
		formatParam->angle = SETTINGS_ANGLE_MAX * M_PI / 180.0;
	}
	if (params.isRidge) {
		stepType |= KConst3D::ksMetricStep;
		formatParam->length = params.ridgeVal;
	} else {
		formatParam->length = SETTINGS_RIDGE_MAX;
	}
	formatParam->stepType = stepType;
	
	std::filesystem::path fp(path);
	fp.replace_extension(params.format.Ext());
	return doc->SaveAsToAdditionFormat(Node::Utf8ToCp1251(fp.string()).c_str(), formatParam);
}

int Doc3D::GetEmbodimentsCount() {
	K7::IEmbodimentsManagerPtr em = Kompas3D::ToApi7<K7::IEmbodimentsManagerPtr>(pDoc);
	return em ? em->EmbodimentCount : 0;
}

std::string Doc3D::GetEmbodimentName(int i) {
	if (K7::IEmbodimentsManagerPtr em = Kompas3D::ToApi7<K7::IEmbodimentsManagerPtr>(pDoc)) {
		if (i < em->EmbodimentCount) {
			if (K7::IEmbodimentPtr e = em->Embodiment[i]) {
				return Node::Cp1251ToUtf8(e->Name);
			}
		}
	}
	return std::string();
}

Part Doc3D::GetEmbodiment(int i) {
	if (K7::IEmbodimentsManagerPtr em = Kompas3D::ToApi7<K7::IEmbodimentsManagerPtr>(pDoc)) {
		if (i < em->EmbodimentCount) {
			if (K7::IEmbodimentPtr e = em->Embodiment[i]) {
				K5::ksPartPtr part = Kompas3D::ToApi5<K5::ksPartPtr>(e->Part);
				return Part(pDoc, part.GetInterfacePtr());
			}
		}
	}
	return Part(pDoc, nullptr);
}
