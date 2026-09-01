#ifndef _Kompas3DPrint_Export_hpp_
#define _Kompas3DPrint_Export_hpp_

#include <CtrlLib/CtrlLib.h>
#include "../KompasAPI/Include/Kompas3D.h"
#include <filesystem>

class Export {
private:
	FileSel saveDlg;
	std::vector<Doc3D::Format> types;

public:
	Export() {
		types = {
			Doc3D::Format::STL,
			Doc3D::Format::STEP,
			Doc3D::Format::VRLM,
			Doc3D::Format::IGES,
			Doc3D::Format::STEP_AP203,
			Doc3D::Format::STEP_AP214,
			Doc3D::Format::STEP_AP242,
			Doc3D::Format::ACIS,
			Doc3D::Format::PARASOLID,
			Doc3D::Format::JT
		};
		for (const Doc3D::Format& type : types) {
			saveDlg.Type(type.Name(), String("*") + type.Ext());
		}
	}
	
	void Slicer(Doc3D::ExportParams& params, const String& slicerPath) {
		if (!FileExists(slicerPath)) {
		  Kompas3D::Message(("Неправильно указан путь до слайсера :" + slicerPath).ToStd());
		}
		LocalProcess slicer;
		slicer.Start(slicerPath, {"C:/Users/User/Documents/Деталь.stl"}, NULL, GetFileFolder(slicerPath));
		slicer.Detach();
	}
	
	void SaveAs(Doc3D::ExportParams& params) {
		Doc3D doc = Kompas3D::GetActiveDocument3D();
		if (!doc) {
			Kompas3D::Error("Не обноружен активный 3D документ");
			return;
		}
		std::string path = doc.GetPath();
		if (path.empty()) path = "Деталь.m3d";
		std::filesystem::path fp(path);
		fp.replace_extension(params.format.Ext());
		saveDlg.ActiveDir(fp.parent_path().string());
		saveDlg.DefaultName(fp.filename().string());
		saveDlg.ActiveType(FormatIndex(params.format));
		if (saveDlg.ExecuteSaveAs()) {
			int typeIdx = saveDlg.GetActiveType();
			params.format = types[typeIdx];
			doc.SaveAs(params, saveDlg.Get().ToStd());
		}
	}
	
	const std::vector<Doc3D::Format>& GetTypes() {
		return types;
	}
	
private:
	const int FormatIndex(Doc3D::Format format) {
		auto it = std::find(types.begin(), types.end(), format);
		if (it != types.end()) {
			return (int)std::distance(types.begin(), it);
		}
		return -1;
	}
};

#endif
