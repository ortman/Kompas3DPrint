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

	String SaveAs(Doc3D::ExportParams& params, bool isTranslate = false, bool isTmp = false) {
		Doc3D doc = Kompas3D::GetActiveDocument3D();
		if (doc) {
			String path = doc.GetPath();
			if (path.IsEmpty()) {
				path = AppendFileName(Upp::GetDocumentsFolder(), "Деталь.m3d");
			}
			path = ForceExt(path, params.format.Ext());
			String fileName = GetFileName(path);
			if (isTranslate) fileName = TransliterateFileName(fileName);
			if (isTmp) {
				path = AppendFileName(GetTempDirectory(), fileName);
			} else {
				saveDlg.ActiveDir(GetFileDirectory(path));
				saveDlg.DefaultName(fileName);
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
	String TransliterateFileName(const String& src) {
		static VectorMap<wchar_t, WString> transMap = {
			{ L'а', L"a" },   { L'б', L"b" },   { L'в', L"v" },   { L'г', L"g" },
			{ L'д', L"d" },   { L'е', L"e" },   { L'ё', L"yo" },  { L'ж', L"zh" },
			{ L'з', L"z" },   { L'и', L"i" },   { L'й', L"y" },   { L'к', L"k" },
			{ L'л', L"l" },   { L'м', L"m" },   { L'н', L"n" },   { L'о', L"o" },
			{ L'п', L"p" },   { L'р', L"r" },   { L'с', L"s" },   { L'т', L"t" },
			{ L'у', L"u" },   { L'ф', L"f" },   { L'х', L"kh" },  { L'ц', L"ts" },
			{ L'ч', L"ch" },  { L'ш', L"sh" },  { L'щ', L"shch" }, { L'ы', L"y" },
			{ L'э', L"e" },   { L'ю', L"yu" },  { L'я', L"ya" },
			{ L'ь', L"" },    { L'ъ', L"" },    { L' ', L"_" }
		};
		WString wsrc = ToLower(src.ToWString());
		WString wres;
		for (int i = 0; i < wsrc.GetLength(); i++) {
			wchar_t c = wsrc[i];
			int idx = transMap.Find(c);
			if (idx >= 0)
				wres.Cat(transMap[idx]);
			else
				wres.Cat(c);
		}
		return wres.ToString();
	}
};

#endif
