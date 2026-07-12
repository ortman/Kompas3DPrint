#ifndef _Kompas3DPrint_StandardParts_hpp_
#define _Kompas3DPrint_StandardParts_hpp_
#include <CtrlLib/CtrlLib.h>
#include "../Resources.h"
#include "../Kompas/Kompas3D.h"

class StandardPartsSelector : public WithStandardPartsModelsLay<TopWindow>  {
public:
	struct Embodiments : Moveable<Embodiments> {
		struct Variables : Moveable<Variables> {
	    VectorMap<String, Vector<double>> data;
	    void Jsonize(JsonIO& json) {
				StringMap(json, data);
	    }
	    Variables() {}
	    Variables(const Variables& vs) : data(clone(vs.data)) {}
		};
		
		String name;
    VectorMap<String, Variables> data;
    void Jsonize(JsonIO& json) {
			StringMap(json, data);
    }
    Embodiments() {}
    Embodiments(const Embodiments& embs) : data(clone(embs.data)) {}
	};
	
	struct Item : Moveable<Item> {
		String name;
		Embodiments embodiments;
		
		Vector<Item> sub;
	
		void Jsonize(JsonIO& json) {
			json("name", name);
			if (json.IsLoading() || !sub.IsEmpty()) {
				json("sub", sub);
			}
			if (json.IsLoading() || !embodiments.data.IsEmpty()) {
				json("embodiments", embodiments);
			}
		}
	};
	
private:
	Item standardParts;

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
		bool isLoaded = LoadFromJsonFile(standardParts, AppendFileName(path, "index.json"));
		if (isLoaded) {
			models.SetRoot(CtrlImg::Dir(), standardParts.name);
			LoadTree(0, standardParts);
		} else {
			Kompas3D::Error("Load index is error");
		}
	}
	
	void LoadTree(int parent, const Item& item) {
		for (const Item& i : item.sub) {
			if (i.sub.GetCount()) {
				int p = models.Add(parent, CtrlImg::Dir(), Null, i.name, i.sub.GetCount() == 1);
				LoadTree(p, i);
			} else {
				models.Add(parent, CtrlImg::File(), RawToValue(i.embodiments), i.name);
			}
		}
	}
	
	Embodiments GetSelected() {
		Vector<int> sel = models.GetSel();
		if (sel.GetCount() == 1 && sel[0]) {
			Value e = models.Get(sel[0]);
			if (!e.IsNull())
				return e.To<Embodiments>();
		}
		return Embodiments();
	}
	
	void ScanDir(Item& dir, const String& path) {
		dir.name = GetFileName(path);
		FindFile ff;
		if (ff.Search(AppendFileName(path, "*"))) {
			do {
				if (ff.IsFile()) {
					if (ToLower(GetFileExt(ff.GetName())) == ".m3d") {
						AddModel(dir.sub.Add(), ff.GetPath());
					}
				} else if (ff.IsFolder()) {
					if (ff.GetName() != "." && ff.GetName() != "..") {
						ScanDir(dir.sub.Add(), ff.GetPath());
					}
				}
			} while (ff.Next());
		}
	}

private:
	
	void AddModel(Item& file, String path) {
		Doc3D	doc = Kompas3D::Open3D(path.ToStd(), false);
		try {
			file.name = GetFileName(path);
			int eCnt = doc.GetEmbodimentsCount();
			if (!eCnt) Kompas3D::Error("Нет исполнений");
			for (int i = 0; i < eCnt; ++i) {
				Part e = doc.GetEmbodiment(i);
				if (!e) throw Kompas3DException("Не могу получить исполнение " + std::to_string(i));
				Embodiments::Variables& variables = file.embodiments.data.Add(doc.GetEmbodimentName(i));
				auto vv = e.GetVariables();
				for (auto& v : vv) {
					Vector<String> sub = Split(v.comment.c_str(), ',');
					Vector<double>& d = variables.data.Add(v.name);
					for (const String& s : sub) d.Add(StrDbl(s));
				}
			}
		} catch (const Kompas3DException& e) {
			Kompas3D::Error(e.what());
		}
		doc.Close();
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
			PropertyList   l    {"Длина"};
		} params{"Параметры"};
	} panel{"Стандартные изделия"};
	
	StandardPartsSelector selector;

public:
	StandardParts() {
		try {
			if (!Kompas3D::Connect()) return;
			panel.Create();
			panel.main.model.WhenClick = [=]() {
				selector.Load();
				if (selector.Execute() == IDOK) {
					StandardPartsSelector::Embodiments emb = selector.GetSelected();
					if (emb.data.GetCount()) {
						panel.main.model.SetName("Model name");
						PropertyList& embList = panel.main.embodiment;
						embList.Clear();
						for (const String& key : emb.data.GetKeys()) {
							embList.Add(key.ToStd());
						}
					}
					//Kompas3D::Error(model.ToStd());
				}
			};
			panel.WhenButtonClick = [=](int buttonId) {
				try {
					if (buttonId == 1) {
						//todo: paste;
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
	
	void Run() {
		panel.Show();
	}
	
	void Scan() {
			try {
				if (!Kompas3D::Connect()) return;
				String path = AppendFileName(GetDocumentsFolder(), "Стандартные изделия");
				StandardPartsSelector::Item standardParts;
				selector.ScanDir(standardParts, path);
				StoreAsJsonFile(standardParts, AppendFileName(path, "index.json"), true);
			} catch (const Kompas3DException& e) {
				Kompas3D::Error(e.what());
			}
	}
};

#endif
