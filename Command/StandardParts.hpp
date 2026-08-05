#ifndef _Kompas3DPrint_StandardParts_hpp_
#define _Kompas3DPrint_StandardParts_hpp_
#include <CtrlLib/CtrlLib.h>
#include "../Resources.h"
#include "../Kompas/Kompas3D.h"

class StandardPartsSelector : public WithStandardPartsModelsLay<TopWindow>  {
public:
	struct ModelData : Moveable<ModelData> {
		typedef VectorMap<String, Vector<double>> Variables;
		String path;
    VectorMap<String, Variables> embodiments;
    
    ModelData() {}
    ModelData(const ModelData& d) : path(d.path), embodiments(clone(embodiments)) {}
    ModelData& operator=(const ModelData& d) {
      path = d.path;
      embodiments = clone(d.embodiments);
      return *this;
    }
    operator bool() const { return embodiments.GetCount(); }
	};

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
	
	const ModelData GetSelected() const {
		ModelData res;
		Vector<int> sel = models.GetSel();
		if (sel.GetCount() == 1 && sel[0]) {
			Value e = models.Get(sel[0]);
			if (!e.IsNull()) {
				res.path = e;
				Doc3D	doc = Kompas3D::Open3D(res.path.ToStd(), false);
				if (!doc) return res;
				try {
					int eCnt = doc.GetEmbodimentsCount();
					for (int i = 0; i < eCnt; ++i) {
						Part e = doc.GetEmbodiment(i);
						if (!e) throw Kompas3DException("Не могу получить исполнение " + std::to_string(i));
						ModelData::Variables& variables = res.embodiments.Add(doc.GetEmbodimentName(i));
						auto vv = e.GetVariables();
						for (auto& v : vv) {
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
							Vector<double>& d = variables.Add(name);
							for (const String& s : values) d.Add(StrDbl(s));
						}
					}
				} catch (const Kompas3DException& e) {
					Kompas3D::Error(e.what());
				}
				doc.Close();
			}
		}
		return res;
	}
};

class InsertModelProc : public Process3D<InsertModelProc> {
public:
	Face plane = nullptr;
	Face axis = nullptr;

	bool OnPlacementChange(const Node& node) override {
		if (node.GetType() != Face::TYPE) return false;
		Face face(node);
		if (face.IsPlanar()) {
			plane = face;
			return true;
		}
		if (face.IsCylinder()) {
			axis = face;
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
private:
	struct : Panel {
		struct : Panel::Tab {
			PropertyButton model      {"Выберите модель"};
			PropertyList   embodiment {"Исполнение"};
		} main{"Основные"};
		struct : Panel::Tab {
		} params{"Параметры"};
	} panel{"Стандартные изделия"};
	
	StandardPartsSelector selector;
	StandardPartsSelector::ModelData sel;
	Doc3D doc = nullptr;

public:
	StandardParts() {
		try {
			if (!Kompas3D::Connect()) return;
			panel.Create();
			panel.main.model.WhenClick = [=]() {
				selector.Load();
				if (selector.Execute() == IDOK) {
					panel.params.Clear();
					panel.main.embodiment.Clear();
					sel = selector.GetSelected();
					if (sel) {
						String name = GetFileName(sel.path);
						name.TrimLast(4);
						panel.main.model.SetName(name.ToStd());
						for (const String& key : sel.embodiments.GetKeys()) {
							panel.main.embodiment.Add(key.ToStd());
						}
					}
					panel.Update();
				}
			};
			panel.main.embodiment.WhenChange = [=]() {
				if (sel) {
					std::string val = std::get<std::string>((PropertyVariant)panel.main.embodiment);
					if (sel.embodiments.Find(val) < 0) return;
					StandardPartsSelector::ModelData::Variables& variables = sel.embodiments.Get(val);
					panel.params.Clear();
					for (const String& v : variables.GetKeys()) {
						PropertyList& var = panel.params.Create<PropertyList>(v.ToStd().c_str());
						for (double d : variables.Get(v)) var.Add(d);
					}
					panel.Update();
				}
			};
			panel.WhenButtonClick = [=](int buttonId) {
				try {
					if (buttonId == 1) {
						if (doc && sel) {
							Doc3D	stDoc = Kompas3D::Open3D(sel.path.ToStd(), false);
							if (stDoc) {
								Part emb = stDoc.GetEmbodiment(0);
								InsertModelProc& proc = doc.CreatePorcess<InsertModelProc>();
								proc.SetPhantom(emb);
								if (proc.Run(true, true)) {
									if (proc.plane && proc.axis) {
										if (Part newPart = doc.AddPart(emb)) {
											doc.AddMateConstraint(MateCoincidence, proc.plane, newPart.GetPlaneXOY(), MateDirSame, MateFixedNone);
											doc.AddMateConstraint(MateConcentric, proc.axis, newPart.GetAxisOZ(), MateDirUndefined, MateFixedNone);
										}
									}
								}
								stDoc.Close();
							}
						}
					} else {
						panel.Hide();
					}
				} catch (const Kompas3DException& e) {
					Kompas3D::Error(e.what());
				}
				return false;
			};
		} catch (const Kompas3DException&) {
		}
	}
	
	void Start() {
		panel.Show();
		doc = Kompas3D::GetActiveDocument3D();
	}
};

#endif
