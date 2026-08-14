/*
 * #include "Process3D.h"
 * #include "Kompas3D.h"
 * 
 * MateConstraint::MateConstraint(IUnknown* m, MateType t, MateDir d, MateFixed f, const Node& obj1, const Node& obj2, double val)
 * 		: mate(m), type(t), dir(d), fixed(f), first(obj1), second(obj2), value(val) {
 * 	if (mate) mate->AddRef();
 * }
 * 
 * MateConstraint::MateConstraint(const MateConstraint& m) : mate(m.mate), type(m.type), dir(m.dir)
 * 		, fixed(m.fixed), first(m.first), second(m.second), value(m.value) {
 * 	if (mate) mate->AddRef();
 * }
 * 
 * MateConstraint::~MateConstraint() {
 * 	if (mate) mate->Release();
 * }
 * 
 * MateConstraint& MateConstraint::SetFirst(const Node& node) {
 * 	if (mate && node) {
 * 		first = node;
 * 		K7::IMateConstraint3DPtr m(mate);
 * 		if (m) {
 * 			K7::IModelObjectPtr obj7 = Kompas3D::ToApi7<K7::IModelObjectPtr>(node.pDefinition);
 * 			m->BaseObject1 = obj7;
 * 		}
 * 	}
 * 	return *this;
 * }
 * 
 * MateConstraint& MateConstraint::SetSecond(const Node& node) {
 * 	if (mate && node) {
 * 		second = node;
 * 		K7::IMateConstraint3DPtr m(mate);
 * 		if (m) {
 * 			K7::IModelObjectPtr obj7 = Kompas3D::ToApi7<K7::IModelObjectPtr>(node.pDefinition);
 * 			m->BaseObject2 = obj7;
 * 		}
 * 	}
 * 	return *this;
 * }
 * 
 * MateConstraint KProcess3D::AddMateConstraint(MateType type, const Node& object1, const Node& object2, MateDir direction, MateFixed fixed, double value) {
 * 	//if (!pProc3D) return nullptr;
 * 	K7::IProcess3DPtr proc3D(pProc3D);
 * 	K7::IMateConstraints3DPtr mcs = proc3D->MateConstraints;
 * 	K7::IMateConstraint3DPtr mc = mcs->Add((KConst3D::MateConstraintType)type);
 * 	mc->Alignment = (KConst3D::ksMateConstraintAlignmentEnum) direction;
 * 	mc->Fixed = (KConst3D::ksMateFixedTypeEnum) fixed;
 * 	mc->ParamValue = value;
 * 	mc.AddRef();
 * 	return MateConstraint((IUnknown*)mc.GetInterfacePtr(), type, direction, fixed, object1, object2, value)
 * 		.SetFirst(object1)
 * 		.SetSecond(object2);
 * }
 * 
 * class Process3DNotifyLoc : public ComEvent {
 * private:
 * 	KProcess3D* proc;
 * public:
 * 	Process3DNotifyLoc(KProcess3D* p) : ComEvent(K7::DIID_ksProcess3DNotify), proc(p) {}
 * 	
 * 	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
 * 	                    DISPPARAMS* pDispParams, VARIANT* pVarResult,
 * 	                    EXCEPINFO* pExcepInfo, UINT* puArgErr) override {
 * 		switch ((int)dispIdMember) {
 * 			case KConst::ksProcess3DFilterObjects:
 * 				if (proc && proc->hasFilterObjectMethod) {
 * 					VARIANT& obj = pDispParams->rgvarg[pDispParams->cArgs - 1];
 * 					if (obj.vt == VT_DISPATCH) {
 * 						K5::ksEntityPtr entity;
 * 						K5::ksFaceDefinitionPtr face;
 * 						K5::ksEdgeDefinitionPtr edge;
 * 						IDispatch* definition = obj.pdispVal;
 * 						K7::IModelObjectPtr model = definition;
 * 						if (model) {
 * 							int type = model->ModelObjectType;
 * 							if (type == KConst3D::o3d_face) {
 * 								if (face = Kompas3D::ToApi5<K5::ksFaceDefinitionPtr>(definition)) entity = face->GetEntity();
 * 							} else if (type == KConst3D::o3d_edge) {
 * 								if (edge = Kompas3D::ToApi5<K5::ksEdgeDefinitionPtr>(definition)) entity = edge->GetEntity();
 * 							}
 * 							VariantInit(pVarResult);
 * 							pVarResult->vt = VT_BOOL;
 * 							pVarResult->boolVal = proc->OnFilterObject(Node(entity.GetInterfacePtr()));
 * 						}
 * 					}
 * 				}
 * 				break;
 * 			case KConst::ksProcess3DPlacementChanged:
 * 				if (proc && proc->hasPlacementChangeMethod) {
 * 					VARIANT& obj = pDispParams->rgvarg[pDispParams->cArgs - 1];
 * 					if (obj.vt == VT_DISPATCH) {
 * 						K5::ksEntityPtr entity;
 * 						K5::ksFaceDefinitionPtr face;
 * 						K5::ksEdgeDefinitionPtr edge;
 * 						IDispatch* definition = obj.pdispVal;
 * 						K7::IModelObjectPtr model = definition;
 * 						if (model) {
 * 							int type = model->ModelObjectType;
 * 							if (type == KConst3D::o3d_face) {
 * 								if (face = Kompas3D::ToApi5<K5::ksFaceDefinitionPtr>(definition)) entity = face->GetEntity();
 * 							} else if (type == KConst3D::o3d_edge) {
 * 								if (edge = Kompas3D::ToApi5<K5::ksEdgeDefinitionPtr>(definition)) entity = edge->GetEntity();
 * 							}
 * 							VariantInit(pVarResult);
 * 							pVarResult->vt = VT_BOOL;
 * 							bool res = proc->OnPlacementChange(Node(entity.GetInterfacePtr()));
 * 							if (res) {
 * 								// TODO: Оставляем висеть в памяти указатели, если объекты будут использованы,
 * 								// но это приводит к утечке памяти, надо как-то решить эту проблему
 * 								if (face) face.AddRef();
 * 								if (edge) edge.AddRef();
 * 							}
 * 							pVarResult->boolVal = res;
 * 						}
 * 					}
 * 				}
 * 				break;
 * 		}
 * 		return S_OK;
 * 	}
 * };
 * 
 * void KProcess3D::Init(Doc3D* doc) {
 * 	currentPanel = nullptr;
 * 	this->doc = doc;
 * 	K7::IKompasDocument3D1Ptr pDoc1 = Kompas3D::ToApi7<K7::IKompasDocument3D1Ptr>(doc->pDoc);
 * 	if (!pDoc1) throw Kompas3DException("У процесса нет документа");
 * 	K7::IProcess3DPtr proc3D = pDoc1->GetLibProcess(KConst::ksProcess3DPlacementAndEntity);
 * 	if (!proc3D) throw Kompas3DException("Не могу создать процесс");
 * 	procEvent = new Process3DNotifyLoc(this);
 * 	pProc3D = (IUnknown*)proc3D.GetInterfacePtr();
 * 	pProc3D->AddRef();
 * 	HRESULT hr = procEvent->Subscribe(pProc3D);
 * 	if (FAILED(hr)) throw Kompas3DException("Не могу подписать события на процесс");
 * 	K7::IProcessPtr proc(pProc3D);
 * 	if (!proc) throw Kompas3DException("Не могу получить базовый процесс от 3D");
 * 	proc->Dynamic = true;
 * 	
 * 	K7::IProcessParamPtr procParam = Kompas3D::CreateProcessParam();
 * 	procParam->AutoReduce = false;
 * 	procParam->SpecToolbar = KConst::pnEnterEscHelp;
 * 	K7::IPropertyTabsPtr pTabs = procParam->PropertyTabs;
 * 	if (!pTabs) throw Kompas3DException("Can not get PropertyTabs of Process3D");
 * 	for (Panel::Tab* t : tabs) {
 * 		K7::IPropertyTabPtr pTab = pTabs->Add(Node::Utf8ToCp1251(t->name).c_str());
 * 		if (pTab) {
 * 			pTab.AddRef();
 * 			t->pTab = pTab.GetInterfacePtr();
 * 			for (Panel::Property* p : t->props) {
 * 				t->Create(p);
 * 			}
 * 		}
 * 	}
 * 	proc->ProcessParam = procParam;
 * 	CreatePropertyManagerNotify(procParam);
 * }
 * 
 * KProcess3D::~KProcess3D() {
 * 	RemovePropertyManagerNotify(pProc3D);
 * 	if (procEvent) {
 * 		procEvent->Unsubscribe(pProc3D);
 * 		delete procEvent;
 * 	}
 * 	if (pProc3D) {
 * 		K7::IProcessPtr proc(pProc3D);
 * 		proc->Stop();
 * 		pProc3D->Release();
 * 	}
 * }
 * 
 * bool KProcess3D::Run(bool modal, bool postMessage) {
 * 	if (!pProc3D) return false;
 * 	K7::IProcessPtr proc(pProc3D);
 * 	return proc->Run(modal, postMessage);
 * }
 * 
 * bool KProcess3D::Stop() {
 * 	if (!pProc3D) return false;
 * 	K7::IProcessPtr proc(pProc3D);
 * 	return proc->Stop();
 * }
 * 
 * void KProcess3D::SetPhantom(const Part& part) {
 * 	if (!pProc3D) return;
 * 	K7::IProcess3DPtr proc3D(pProc3D);
 * 	if (proc3D) {
 * 		K7::IPart7Ptr part7 = Kompas3D::ToApi7<K7::IPart7Ptr>(part.pPart);
 * 		if (part7) {
 * 			part7.AddRef(); //TODO: do remove?
 * 			proc3D->PhantomObject = part7;
 * 			//Update();
 * 		}
 * 	}
 * }
 * 
 * Part KProcess3D::GetPhantom() {
 * 	if (pProc3D) {
 * 		K7::IProcess3DPtr proc3D(pProc3D);
 * 		K7::IModelObjectPtr phModel = proc3D->PhantomObject;
 * 		if (phModel && phModel->ModelObjectType == KConst3D::o3d_part) {
 * 			phModel.AddRef(); // TODO:
 * 			K5::ksPartPtr part = Kompas3D::ToApi5<K5::ksPartPtr>(phModel);
 * 			if (part) {
 * 				return Part(doc->pDoc, part.GetInterfacePtr());
 * 			}
 * 		}
 * 	}
 * 	return Part(nullptr, nullptr);
 * }
 * 
 * void KProcess3D::Update() {
 * 	if (!pProc3D) return;
 * 	K7::IProcessPtr proc(pProc3D);
 * 	proc->Update();
 * }
 * 
 * void KProcess3D::SetCaption(const std::string& caption) {
 * 	if (pProc3D) {
 * 		K7::IProcessPtr proc(pProc3D);
 * 		K7::IProcessParamPtr procParam = proc->ProcessParam;
 * 		if (procParam) {
 * 			procParam->Caption = Node::Utf8ToCp1251(caption).c_str();
 * 		} else {
 * 			proc->Caption = Node::Utf8ToCp1251(caption).c_str(); //TODO: unused!
 * 		}
 * 	}
 * }
 */
