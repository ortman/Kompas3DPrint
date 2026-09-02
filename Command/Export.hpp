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

	String SaveAs(Doc3D::ExportParams& params, bool isTmp = false) {
		Doc3D doc = Kompas3D::GetActiveDocument3D();
		if (doc) {
			String path = doc.GetPath();
			if (path.IsEmpty()) {
				path = AppendFileName(Upp::GetDocumentsFolder(), "Деталь.m3d");
			}
			path = ForceExt(path, params.format.Ext());
			if (isTmp) {
				path = AppendFileName(GetTempDirectory(), GetFileName(path));
			} else {
				saveDlg.ActiveDir(GetFileDirectory(path));
				saveDlg.DefaultName(GetFileName(path));
				saveDlg.ActiveType(FormatIndex(params.format));
				if (!saveDlg.ExecuteSaveAs()) return String();
				int typeIdx = saveDlg.GetActiveType();
				params.format = types[typeIdx];
				path = ForceExt(path, params.format.Ext());
			}
			if (doc.SaveAs(params, path.ToStd())) {
				return path;
			}
		} else {
			Kompas3D::Error("Не обноружен активный 3D документ");
		}
		return String();
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
