#ifndef _Kompas3DPrint_StandardParts_hpp_
#define _Kompas3DPrint_StandardParts_hpp_

class StandardParts {
public:
	StandardParts() {
		try {
			if (!Kompas3D::Connect()) return;
			
		} catch (const Kompas3DException&) {
		}
	}
	
	void Start() {
			try {
				if (!Kompas3D::Connect()) return;
				ScanDir(AppendFileName(GetDocumentsFolder(), "Стандартные изделия"));
			} catch (const Kompas3DException& e) {
				Kompas3D::Error(e.what());
			}
	}
	
private:
	void ScanDir(const String& dir) {
		FindFile ff;
		if(ff.Search(AppendFileName(dir, "*"))) {
			do {
				if(ff.IsFile()) {
					if(ToLower(GetFileExt(ff.GetName())) == ".m3d") {
						AddModel(ff.GetPath());
					}
				}
				else if(ff.IsFolder()) {
					if(ff.GetName() != "." && ff.GetName() != "..") {
						ScanDir(ff.GetPath());
					}
				}
			} while(ff.Next());
		}
	}
	
	void AddModel(String path) {
		Doc3D doc = Kompas3D::Open3D(path.ToStd(), false);
		try {
			int eCnt = doc.GetEmbodimentsCount();
			if (!eCnt) Kompas3D::Error("Нет исполнений");
			for (int i = 0; i < eCnt; ++i) {
				Part e = doc.GetEmbodiment(i);
				if (!e) throw Kompas3DException("не могу получить исполнение " + std::to_string(i));
				auto vv = e.GetVariables();
				//if (!vv.size()) Kompas3D::Error("Нет переменных в исполнении " + std::to_string(i));
				//for (auto& v : vv) {
				//	Kompas3D::Error("[" + std::to_string(i) + "]" + v.name + "=" + std::to_string(v.value) + " " + v.comment);
				//}
			}
		} catch (const Kompas3DException& e) {
			Kompas3D::Error(e.what());
		}
		doc.Close();
	}
};

#endif
