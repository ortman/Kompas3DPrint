#ifndef _Kompas3DPrint_Export_hpp_
#define _Kompas3DPrint_Export_hpp_
#include "../Kompas/Kompas3D.h"

class Export {
private:
	FileSel saveDlg;

public:
	Export() {
		//saveDlg.Types("");
	}
	void Slicer() {}
	void STL()  { SaveAs(Doc3D::STL);  }
	void STEP() { SaveAs(Doc3D::STEP); }
	void IGES() { SaveAs(Doc3D::IGES); }
	void XT()   { SaveAs(Doc3D::XT);   }
	void SAT()  { SaveAs(Doc3D::SAT);  }
	void VRLM() { SaveAs(Doc3D::VRLM); }
	
private:
	void SaveAs(Doc3D::Format format) {
		std::unique_ptr<Doc3D> doc = Kompas3D::GetActiveDocument3D();
		if (!doc) {
			Kompas3D::Error("Не обноружен активный 3D документ");
			return;
		}
		std::string path = doc->GetPath();
		if (path.empty()) path = "Деталь.m3d";
		if (saveDlg.ExecuteSaveAs()) {
			//..
			//doc->SaveAs(format, "file_path");
		}
	}
};

#endif
