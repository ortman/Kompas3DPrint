#include "ComKompas.h"
#include "Doc3D.h"
#include "Kompas3D.h"
#include <filesystem>
#include <numbers>

class Process3DNotifyLoc : public ComEvent {
private:
	KProcess3D* proc;
public:
	Process3DNotifyLoc(KProcess3D* p) : ComEvent(K7::DIID_ksProcess3DNotify), proc(p) {}
	
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
	                    DISPPARAMS* pDispParams, VARIANT* pVarResult,
	                    EXCEPINFO* pExcepInfo, UINT* puArgErr) override {
		switch ((int)dispIdMember) {
			case KConst::ksProcess3DFilterObjects:
				if (proc && proc->hasFilterObjectMethod) {
					VARIANT& obj = pDispParams->rgvarg[pDispParams->cArgs - 1];
					if (obj.vt == VT_DISPATCH) {
						K5::ksEntityPtr entity;
						K5::ksFaceDefinitionPtr face;
						K5::ksEdgeDefinitionPtr edge;
						IDispatch* definition = obj.pdispVal;
						K7::IModelObjectPtr model = definition;
						if (model) {
							int type = model->ModelObjectType;
							if (type == KConst3D::o3d_face) {
								if (face = Kompas3D::ToApi5<K5::ksFaceDefinitionPtr>(definition)) entity = face->GetEntity();
							} else if (type == KConst3D::o3d_edge) {
								if (edge = Kompas3D::ToApi5<K5::ksEdgeDefinitionPtr>(definition)) entity = edge->GetEntity();
							}
							VariantInit(pVarResult);
							pVarResult->vt = VT_BOOL;
							pVarResult->boolVal = proc->OnFilterObject(Node(entity.GetInterfacePtr()));
						}
					}
				}
				break;
			case KConst::ksProcess3DPlacementChanged:
				if (proc && proc->hasPlacementChangeMethod) {
					VARIANT& obj = pDispParams->rgvarg[pDispParams->cArgs - 1];
					if (obj.vt == VT_DISPATCH) {
						K5::ksEntityPtr entity;
						K5::ksFaceDefinitionPtr face;
						K5::ksEdgeDefinitionPtr edge;
						IDispatch* definition = obj.pdispVal;
						K7::IModelObjectPtr model = definition;
						if (model) {
							int type = model->ModelObjectType;
							if (type == KConst3D::o3d_face) {
								if (face = Kompas3D::ToApi5<K5::ksFaceDefinitionPtr>(definition)) entity = face->GetEntity();
							} else if (type == KConst3D::o3d_edge) {
								if (edge = Kompas3D::ToApi5<K5::ksEdgeDefinitionPtr>(definition)) entity = edge->GetEntity();
							}
							VariantInit(pVarResult);
							pVarResult->vt = VT_BOOL;
							bool res = proc->OnPlacementChange(Node(entity.GetInterfacePtr()));
							if (res) {
								// TODO: Оставляем висеть в памяти указатели, если объекты будут использованы,
								// но это приводит к утечке памяти, надо как-то решить эту проблему
								if (face) face.AddRef();
								if (edge) edge.AddRef();
							}
							pVarResult->boolVal = res;
						}
					}
				}
				break;
		}
		return S_OK;
	}
};

void KProcess3D::Init(Doc3D* doc) {
	this->doc = doc;
	K7::IKompasDocument3D1Ptr pDoc1 = Kompas3D::ToApi7<K7::IKompasDocument3D1Ptr>(doc->pDoc);
	if (!pDoc1) throw Kompas3DException("У процесса нет документа");
	K7::IProcess3DPtr proc3D = pDoc1->GetLibProcess(KConst::ksProcess3DPlacementAndEntity);
	if (!proc3D) throw Kompas3DException("Не могу создать процесс");
	comEvent = new Process3DNotifyLoc(this);
	pProc3D = (IUnknown*)proc3D.GetInterfacePtr();
	pProc3D->AddRef();
	HRESULT hr = comEvent->Subscribe(pProc3D);
	if (FAILED(hr)) throw Kompas3DException("Не могу подписать события на процесс");
	K7::IProcessPtr proc(pProc3D);
	if (!proc) throw Kompas3DException("Не могу получить базовый процесс от 3D");
	proc->Dynamic = true;
}

KProcess3D::~KProcess3D() {
	if (comEvent) {
		comEvent->Unsubscribe(pProc3D);
		delete comEvent;
	}
	if (pProc3D) {
		K7::IProcessPtr proc(pProc3D);
		proc->Stop();
		pProc3D->Release();
	}
}

bool KProcess3D::Run(bool modal, bool postMessage) {
	if (!pProc3D) return false;
	K7::IProcessPtr proc(pProc3D);
	return proc->Run(modal, postMessage);
}

bool KProcess3D::Stop() {
	if (!pProc3D) return false;
	K7::IProcessPtr proc(pProc3D);
	return proc->Stop();
}

void KProcess3D::SetPhantom(const Part& part) {
	if (!pProc3D) return;
	K7::IProcess3DPtr proc3D(pProc3D);
	if (proc3D) {
		K7::IPart7Ptr part7 = Kompas3D::ToApi7<K7::IPart7Ptr>(part.pPart);
		if (part7) {
			part7.AddRef(); //TODO: do remove?
			proc3D->PhantomObject = part7;
		}
	}
}

Part KProcess3D::GetPhantom() {
	if (pProc3D) {
		K7::IProcess3DPtr proc3D(pProc3D);
		K7::IModelObjectPtr phModel = proc3D->PhantomObject;
		if (phModel && phModel->ModelObjectType == KConst3D::o3d_part) {
			phModel.AddRef(); // TODO:
			K5::ksPartPtr part = Kompas3D::ToApi5<K5::ksPartPtr>(phModel);
			if (part) {
				return Part(doc->pDoc, part.GetInterfacePtr());
			}
		}
	}
	return Part(nullptr, nullptr);
}

void KProcess3D::SetCaption(const std::string& caption) {
	if (pProc3D) {
		K7::IProcessPtr proc(pProc3D);
		proc->Caption = Node::Utf8ToCp1251(caption).c_str();
	}
}

MateConstraint::MateConstraint(IUnknown* m, MateType t, MateDir d, MateFixed f, const Node& obj1, const Node& obj2, double val)
		: mate(m), type(t), dir(d), fixed(f), first(obj1), second(obj2), value(val) {
	if (mate) mate->AddRef();
}

MateConstraint::MateConstraint(const MateConstraint& m) : mate(m.mate), type(m.type), dir(m.dir)
		, fixed(m.fixed), first(m.first), second(m.second), value(m.value) {
	if (mate) mate->AddRef();
}

MateConstraint::~MateConstraint() {
	if (mate) mate->Release();
}

MateConstraint& MateConstraint::SetFirst(const Node& node) {
	if (mate && node) {
		first = node;
		K7::IMateConstraint3DPtr m(mate);
		if (m) {
			K7::IModelObjectPtr obj7 = Kompas3D::ToApi7<K7::IModelObjectPtr>(node.pDefinition);
			m->BaseObject1 = obj7;
		}
	}
	return *this;
}

MateConstraint& MateConstraint::SetSecond(const Node& node) {
	if (mate && node) {
		second = node;
		K7::IMateConstraint3DPtr m(mate);
		if (m) {
			K7::IModelObjectPtr obj7 = Kompas3D::ToApi7<K7::IModelObjectPtr>(node.pDefinition);
			m->BaseObject2 = obj7;
		}
	}
	return *this;
}

MateConstraint KProcess3D::AddMateConstraint(MateType type, const Node& object1, const Node& object2, MateDir direction, MateFixed fixed, double value) {
	//if (!pProc3D) return nullptr;
	K7::IProcess3DPtr proc3D(pProc3D);
	K7::IMateConstraints3DPtr mcs = proc3D->MateConstraints;
	K7::IMateConstraint3DPtr mc = mcs->Add((KConst3D::MateConstraintType)type);
	mc->Alignment = (KConst3D::ksMateConstraintAlignmentEnum) direction;
	mc->Fixed = (KConst3D::ksMateFixedTypeEnum) fixed;
	mc->ParamValue = value;
	mc.AddRef();
	return MateConstraint((IUnknown*)mc.GetInterfacePtr(), type, direction, fixed, object1, object2, value)
		.SetFirst(object1)
		.SetSecond(object2);
}

class DocumentFileNotifyLoc : public ComEvent {
private:
	Doc3D* doc;
public:
	DocumentFileNotifyLoc(Doc3D* d) : ComEvent(K5::DIID_ksDocumentFileNotify), doc(d) {}
	
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
	                    DISPPARAMS* pDispParams, VARIANT* pVarResult,
	                    EXCEPINFO* pExcepInfo, UINT* puArgErr) override {
		switch ((int)dispIdMember) {
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
	friend class Doc3D;
};

Doc3D::Doc3D(IUnknown* d) : pDoc(d) {
	if (pDoc) {
		pDoc->AddRef();
		comEvent = new DocumentFileNotifyLoc(this);
		comEvent->Subscribe(pDoc);
	}
}

Doc3D::Doc3D(Doc3D&& doc) noexcept : Doc3D(doc.pDoc) {
	*this = std::move(doc);
}

Doc3D& Doc3D::operator=(Doc3D&& doc) noexcept {
	if (this == &doc) return *this;
	pDoc = doc.pDoc;
	comEvent = doc.comEvent;
	proc3D = doc.proc3D;
	if (comEvent) comEvent->doc = this;
	if (proc3D) proc3D->doc = this;
	WhenBeginCloseDocument = doc.WhenBeginCloseDocument;
	WhenCloseDocument = doc.WhenCloseDocument;
	WhenBeginSaveDocument = doc.WhenBeginSaveDocument;
	WhenSaveDocument = doc.WhenSaveDocument;
	WhenActiveDocument = doc.WhenActiveDocument;
	
	doc.proc3D = nullptr;
	doc.comEvent = nullptr;
	doc.pDoc = nullptr;
	doc.WhenBeginCloseDocument.Clear();
	doc.WhenCloseDocument.Clear();
	doc.WhenBeginSaveDocument.Clear();
	doc.WhenSaveDocument.Clear();
	doc.WhenActiveDocument.Clear();
	return *this;
}

Doc3D::~Doc3D() {
	if (proc3D) delete proc3D;
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
		formatParam->angle = params.angleVal * std::numbers::pi / 180.0;
	} else {
		formatParam->angle = SETTINGS_ANGLE_MAX * std::numbers::pi / 180.0;
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
				std::string name = Node::Cp1251ToUtf8(e->GetMarking(KConst::ksVMEmbodimentNumber, false));
				size_t eName = name.find_last_not_of(" \t\n\r");
				if (eName != std::string::npos) name = name.substr(0, eName + 1);
				size_t sName = name.find_first_not_of(" \t\n\r-");
				if (sName != std::string::npos) return name.substr(sName);
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

bool Doc3D::AddMateConstraint(MateType type, const Node& object1, const Node& object2, MateDir direction, MateFixed fixed, double value) {
	if (!object1.pEntity || !object2.pEntity) return false;
	K5::ksDocument3DPtr doc = pDoc;
	K5::ksEntityPtr obj1 = object1.pEntity;
	K5::ksEntityPtr obj2 = object2.pEntity;
	return doc->AddMateConstraint(type, obj1, obj2, direction, fixed, value);
}

Part Doc3D::AddPart(const Part& part, const std::optional<std::string>& filePath) {
	K5::ksDocument3DPtr doc = pDoc;
	if (part.pPart) {
		bool isPath = filePath.has_value();
		if (doc->SetPartFromFile((isPath ? Node::Utf8ToCp1251(filePath.value()).c_str() : ""), K5::ksPartPtr(part.pPart), isPath)) {
			int partIdx = 0;
			K5::ksPartPtr p, newPart;
			while (p = doc->GetPart(partIdx++)) newPart = p;
			return Part(pDoc, newPart.GetInterfacePtr());
		}
	}
	return Part(nullptr, nullptr);
}
