#ifndef _Kompas3DPrint_StandardParts_hpp_
#define _Kompas3DPrint_StandardParts_hpp_

class StandardParts {
public:
	struct Embodiments {
		struct Variables : Moveable<Variables> {
	    VectorMap<String, Vector<double>> data;
	    void Jsonize(JsonIO& json) {
				StringMap(json, data);
	    }
		};
    VectorMap<String, Variables> data;
    void Jsonize(JsonIO& json) {
			StringMap(json, data);
    }
	};
	struct Item : Moveable<Item> {
		String name;
		Embodiments embodiments;
		
		Vector<Item> sub;
	
		void Jsonize(JsonIO& json) {
			json("name", name);
			if(json.IsLoading() || !sub.IsEmpty()) {
				json("sub", sub);
			}
			if(json.IsLoading() || !embodiments.data.IsEmpty()) {
					json("embodiments", embodiments);
			}
		}
	};
	
	StandardParts() {
		try {
			if (!Kompas3D::Connect()) return;
			
		} catch (const Kompas3DException&) {
		}
	}
	
	void Start() {
			try {
				if (!Kompas3D::Connect()) return;
				Item standardParts;
				String path = AppendFileName(GetDocumentsFolder(), "Стандартные изделия");
				ScanDir(standardParts, path);
				StoreAsJsonFile(standardParts, AppendFileName(path, "index.json"), true);
			} catch (const Kompas3DException& e) {
				Kompas3D::Error(e.what());
			}
	}
	
private:
	void ScanDir(Item& dir, const String& path) {
		dir.name = GetFileName(path);
		FindFile ff;
		if(ff.Search(AppendFileName(path, "*"))) {
			do {
				if(ff.IsFile()) {
					if(ToLower(GetFileExt(ff.GetName())) == ".m3d") {
						AddModel(dir.sub.Add(), ff.GetPath());
					}
				}
				else if(ff.IsFolder()) {
					if(ff.GetName() != "." && ff.GetName() != "..") {
						ScanDir(dir.sub.Add(), ff.GetPath());
					}
				}
			} while(ff.Next());
		}
	}
	
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

#endif
