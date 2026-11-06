#ifndef _Kompas3DPrint_Slicer_hpp_
#define _Kompas3DPrint_Slicer_hpp_

#include <Core/Core.h>

using namespace Upp;

class Slicer {
public:
	struct Installation : Moveable<Installation> {
		String name;
		String path;
	};

private:
	HANDLE hSlicerProcess = NULL;
	Vector<String> files;
	String activeSlicerPath;

	static bool IsSamePath(const String& a, const String& b) {
		return ToLower(a) == ToLower(b);
	}

	static void Add(Vector<Installation>& found, const String& name, const String& path) {
		if (path.IsEmpty() || !FileExists(path)) return;
		for (const Installation& item : found) {
			if (IsSamePath(item.path, path)) return;
		}
		Installation& item = found.Add();
		item.name = name;
		item.path = path;
	}

	static void AddRoot(Vector<String>& roots, const String& root) {
		if (root.IsEmpty()) return;
		for (const String& item : roots) {
			if (IsSamePath(item, root)) return;
		}
		roots.Add(root);
	}

	static String GetRegisteredPath(const wchar_t* executable) {
		const HKEY roots[] = {HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE};
		const DWORD views[] = {RRF_SUBKEY_WOW6432KEY, RRF_SUBKEY_WOW6464KEY};
		WString subkey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\";
		subkey << executable;
		for (HKEY root : roots) {
			for (DWORD view : views) {
				wchar_t path[32768] = {};
				DWORD size = sizeof(path);
				if (RegGetValueW(root, subkey.ToStd().c_str(), NULL,
				                 RRF_RT_REG_SZ | view, NULL, path, &size) == ERROR_SUCCESS) {
					String result = WString(path).ToString();
					if (result.GetCount() >= 2 && result[0] == '"' && result[result.GetCount() - 1] == '"') {
						result = result.Mid(1, result.GetCount() - 2);
					}
					return result;
				}
			}
		}
		return String();
	}

	static void AddVersioned(Vector<Installation>& found, const String& name,
	                         const String& root, const String& directoryMask,
	                         const String& executable) {
		FindFile ff(AppendFileName(root, directoryMask));
		while (ff) {
			if (ff.IsFolder()) Add(found, name, AppendFileName(ff.GetPath(), executable));
			ff.Next();
		}
	}

	bool IsSlicerRunning(HANDLE hProcess) {
		if (!hProcess) return false;
		DWORD exitCode;
		if (::GetExitCodeProcess(hProcess, &exitCode)) {
			return exitCode == STILL_ACTIVE;
		}
		return false;
	}

public:
	~Slicer() {
		if (hSlicerProcess) {
			::CloseHandle(hSlicerProcess);
			hSlicerProcess = NULL;
		}
	}

	static Vector<Installation> FindInstalled() {
		Vector<Installation> found;
		Add(found, "UltiMaker Cura", GetRegisteredPath(L"UltiMaker-Cura.exe"));
		Add(found, "OrcaSlicer", GetRegisteredPath(L"orca-slicer.exe"));
		Add(found, "PrusaSlicer", GetRegisteredPath(L"prusa-slicer.exe"));
		Add(found, "Bambu Studio", GetRegisteredPath(L"bambu-studio.exe"));
		Add(found, "Simplify3D", GetRegisteredPath(L"Simplify3D.exe"));

		Vector<String> roots;
		AddRoot(roots, GetEnv("ProgramFiles"));
		AddRoot(roots, GetEnv("ProgramW6432"));
		AddRoot(roots, GetEnv("ProgramFiles(x86)"));
		String localPrograms = GetEnv("LOCALAPPDATA");
		if (!localPrograms.IsEmpty()) AddRoot(roots, AppendFileName(localPrograms, "Programs"));
		if (roots.IsEmpty()) AddRoot(roots, "C:\\Program Files");

		for (const String& root : roots) {
			AddVersioned(found, "UltiMaker Cura", root, "UltiMaker Cura*", "UltiMaker-Cura.exe");
			AddVersioned(found, "OrcaSlicer", root, "OrcaSlicer", "orca-slicer.exe");
			AddVersioned(found, "PrusaSlicer", root, "PrusaSlicer*", "prusa-slicer.exe");
			AddVersioned(found, "PrusaSlicer", AppendFileName(root, "Prusa3D"), "PrusaSlicer*", "prusa-slicer.exe");
			AddVersioned(found, "Bambu Studio", root, "Bambu Studio", "bambu-studio.exe");
			AddVersioned(found, "Simplify3D", root, "Simplify3D*", "Simplify3D.exe");
		}
		return found;
	}

	void Open(const String& filePath, const String& slicerPath) {
		if (slicerPath.IsEmpty()) {
			ErrorOK("Слайсер не выбран. Укажите его в настройках.");
			return;
		}
		if (!FileExists(slicerPath)) {
			ErrorOK("Исполняемый файл слайсера не найден: " + slicerPath);
			return;
		}

		bool isCura = ToLower(GetFileName(slicerPath)) == "ultimaker-cura.exe";
		if (!IsSamePath(activeSlicerPath, slicerPath) || !IsSlicerRunning(hSlicerProcess)) {
			files.Clear();
			if (hSlicerProcess) ::CloseHandle(hSlicerProcess);
			hSlicerProcess = NULL;
			activeSlicerPath = slicerPath;
		}
		if (!isCura) files.Clear();
		files.Add(filePath);

		WString command = L"\"" + slicerPath.ToWString() + L"\"";
		if (isCura) command << L" --single-instance";
		for (const String& file : files) command << L" \"" << file.ToWString() << L"\"";

		STARTUPINFOW si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOWNORMAL;
		ZeroMemory(&pi, sizeof(pi));
		Buffer<wchar_t> cmdBuf(command.GetLength() + 1);
		memcpy(cmdBuf, command.ToStd().c_str(), (command.GetLength() + 1) * sizeof(wchar_t));
		WString workingDir = GetFileFolder(slicerPath).ToWString();

		if (::CreateProcessW(NULL, cmdBuf, NULL, NULL, FALSE, 0, NULL,
		                     workingDir.ToStd().c_str(), &si, &pi)) {
			if (!IsSlicerRunning(hSlicerProcess)) {
				if (hSlicerProcess) ::CloseHandle(hSlicerProcess);
				hSlicerProcess = pi.hProcess;
				::CloseHandle(pi.hThread);
			} else {
				::CloseHandle(pi.hProcess);
				::CloseHandle(pi.hThread);
			}
		} else {
			ErrorOK("Не удалось запустить слайсер: " + slicerPath);
		}
	}
};

#endif
