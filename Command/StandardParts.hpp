#ifndef _Kompas3DPrint_StandardParts_hpp_
#define _Kompas3DPrint_StandardParts_hpp_
#include <CtrlLib/CtrlLib.h>
#include "../Resources.h"
#include "../Kompas/Kompas3D.h"

class StandardPartsSelector : public WithStandardPartsModelsLay<TopWindow>  {
private:
	Doc3D selDoc = nullptr;

public:
	StandardPartsSelector() {
		CtrlLayout(*this, t_("Select model"));
		Zoomable().Sizeable();
		models.WhenLeftDouble = [=]() {
			Vector<int> sel = models.GetSel();
			if (sel.GetCount() == 1 && sel[0] && !models.Get(sel[0]).IsNull()) {
				AcceptBreak(IDOK);
			}
		};
	}
	
	~StandardPartsSelector() {
		if (selDoc) selDoc.Close();
	}
	
	void Load() {
		String path = AppendFileName(GetDocumentsFolder(), "Стандартные изделия");
		models.Clear();
		models.NoRoot();
		LoadTree(0, path);
	}

	void LoadTree(int parent, const String& folder) {
		FindFile ff;
		if (ff.Search(AppendFileName(folder, "*"))) {
			int count = 0;
			do {
				if (ff.IsFile()) {
					if (ToLower(GetFileExt(ff.GetName())) == ".m3d") {
						String name = ff.GetName();
						name.TrimLast(4);
						models.Add(parent, CtrlImg::File(), ff.GetPath(), name);
						++count;
					}
				} else if (ff.IsFolder()) {
					if (ff.GetName() != "." && ff.GetName() != "..") {
						int p = models.Add(parent, CtrlImg::Dir(), Null, ff.GetName());
						LoadTree(p, ff.GetPath());
						++count;
					}
				}
			} while (ff.Next());
			models.Open(parent, count <= 1);
		}
	}
	
	Doc3D& GetSelected() {
		if (selDoc) return selDoc;
		Vector<int> sel = models.GetSel();
		if (sel.GetCount() == 1 && sel[0]) {
			Value e = models.Get(sel[0]);
			if (!e.IsNull()) {
				selDoc = std::move(Kompas3D::Open3D(String(e).ToStd(), false));
			}
		}
		return selDoc;
	}
};

class InsertModelProc : public Process3D<InsertModelProc> {
public:
	struct : Panel::Tab {
		PropertyButton model      {"Выберите модель"};
		PropertyList   embodiment {"Исполнение"};
	} mainTab {"Основные"};
	struct : Panel::Tab {
	} paramsTab {"Параметры"};
	
	MateConstraint planeMate;
	MateConstraint axisMate;

	bool OnPlacementChange(const Node& node) override {
		if (node.GetType() != Face::TYPE) return false;
		Face face(node);
		if (face.IsPlanar()) {
			if (!planeMate) {
				Part phantom = GetPhantom();
				planeMate = AddMateConstraint(MateCoincidence, phantom ? phantom.GetPlaneXOY() : nullptr, nullptr, MateDirSame);
			} else if (!planeMate.GetFirst() && GetPhantom()) {
				planeMate.SetFirst(GetPhantom().GetPlaneXOY());
			}
			planeMate.SetSecond(face);
			return true;
		}
		if (face.IsCylinder()) {
			if (!axisMate) {
				Part phantom = GetPhantom();
				axisMate = AddMateConstraint(MateConcentric, phantom ? phantom.GetAxisOZ() : nullptr, nullptr, MateDirUndefined);
			} else if (!axisMate.GetFirst() && GetPhantom()) {
				planeMate.SetFirst(GetPhantom().GetAxisOZ());
			}
			axisMate.SetSecond(face);
			return true;
		}
		return false;
	}

	bool OnFilterObject(const Node& node) override {
		if (node.GetType() != Face::TYPE) return false;
		Face face(node);
		return face.IsCylinder() || face.IsPlanar();
	}
};
	
class StandardParts {
	StandardPartsSelector selector;
	InsertModelProc* proc = nullptr;
	Part embodiment;
	Doc3D doc;

public:
	void Start() {
		//if (!Kompas3D::Connect()) return;
		doc = std::move(Kompas3D::GetActiveDocument3D());
		if (!doc) return;
		proc = &doc.CreatePorcess<InsertModelProc>();
		proc->SetCaption("Стандартные изделия");

		proc->mainTab.model.WhenClick = [=]() {
			selector.Load();
			if (selector.Execute() == IDOK) {
				proc->paramsTab.Clear();
				proc->mainTab.embodiment.Clear();
				Doc3D& sel = selector.GetSelected();
				if (sel) {
					String name = GetFileName(sel.GetPath().c_str());
					name.TrimLast(4);
					proc->mainTab.model.SetName(name.ToStd());
					int embCount = sel.GetEmbodimentsCount();
					for (int i = 0; i < embCount; ++i) {
						proc->mainTab.embodiment.Add(sel.GetEmbodimentName(i));
					}
				}
			}
		};

		proc->mainTab.embodiment.WhenChange = [=]() {
			proc->Stop();
			Doc3D& sel = selector.GetSelected();
			int embIndex = proc->mainTab.embodiment.Find(proc->mainTab.embodiment);
			if (sel && embIndex >= 0) {
				sel.SetCurrentEmbodiment(embIndex);
				embodiment = sel.GetEmbodiment(embIndex);
				proc->SetPhantom(embodiment);
				proc->Run(false, true);
				for (const Part::Variable& v : embodiment.GetVariables(true)) {
					String name = v.name;
					Vector<String> values;
					Vector<String> nameVal = Split(v.comment.c_str(), ':');
					if (nameVal.GetCount() == 2) {
						name = nameVal[0] + "(" + name + ")";
						values = Split(nameVal[1], ',');
					} else {
						values = Split(v.comment.c_str(), ',');
						if (values.GetCount() == 1) {
							values.Clear();
							name = v.comment + "(" + name + ")";
						}
					}
					PropertyList& var = proc->paramsTab.Create<PropertyList>(name.Begin());
					for (const String& s : values) var.Add(StrDbl(s));
				}
			}
		};
		
		proc->WhenButtonClick = [=](int buttonId) {
			if (buttonId != 1 || !embodiment || !proc->planeMate || !proc->axisMate) return false;
			try {
				if (Part newPart = doc.AddPart(embodiment)) {
					doc.AddMateConstraint(MateCoincidence, proc->planeMate.GetSecond(),
							newPart.GetPlaneXOY(), MateDirSame, MateFixedNone);
					doc.AddMateConstraint(MateConcentric, proc->axisMate.GetSecond(),
							newPart.GetAxisOZ(), MateDirUndefined, MateFixedNone);
				}
			} catch (const Kompas3DException& e) {
				Kompas3D::Error(e.what());
			}
			Doc3D& sel = selector.GetSelected();
			if (sel) sel.Close();
			embodiment = Part(nullptr, nullptr);
			return true;
		};
		proc->Run(false, true);
	}
};

#endif
