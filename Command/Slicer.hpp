#ifndef _Kompas3DPrint_Slicer_hpp_
#define _Kompas3DPrint_Slicer_hpp_

#include <Core/Core.h>

using namespace Upp;

class Slicer {
private:
	HANDLE hSlicerProcess = NULL;
	Vector<String> files;

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

	WString GetSlicerDir() {
		String programFiles = GetEnv("ProgramFiles");
		if (programFiles.IsEmpty()) {
			programFiles = "C:\\Program Files";
		}
		String mask = AppendFileName(programFiles, "UltiMaker Cura *");
		FindFile ff(mask);
		Vector<String> foundDirs;
		while (ff) {
			if (ff.IsFolder()) {
				foundDirs.Add(ff.GetPath());
			}
			ff.Next();
		}
		if (foundDirs.IsEmpty()) return WString();
		Sort(foundDirs);
		return foundDirs.Top().ToWString();
	}

	void Open(const String& filePath) {
		if (!IsSlicerRunning(hSlicerProcess)) files.Clear();
		files.Add(filePath);
		static const WString curaDir = GetSlicerDir();
		if (curaDir.IsEmpty()) {
			ErrorOK("Ошибка: UltiMaker Cura не найдена в Program Files!");
			return;
		}
		static const WString curaExe = curaDir + L"/UltiMaker-Cura.exe";
		WString command = L"\"" + curaExe + L"\" --single-instance";
		for (const String& file : files) {
			command << L" \"" << file.ToWString() << L"\"";
		}
		STARTUPINFOW si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOWNORMAL;

		ZeroMemory(&pi, sizeof(pi));
		Buffer<wchar_t> cmdBuf(command.GetLength() + 1);
		memcpy(cmdBuf, command.ToStd().c_str(), (command.GetLength() + 1) * sizeof(wchar_t));

		if (::CreateProcessW(
				NULL,
				cmdBuf,
				NULL,
				NULL,
				FALSE,
				0,
				NULL,
				curaDir.ToStd().c_str(),
				&si,
				&pi
		)) {
			if (!IsSlicerRunning(hSlicerProcess)) {
				if (hSlicerProcess) ::CloseHandle(hSlicerProcess);
				hSlicerProcess = pi.hProcess;
				::CloseHandle(pi.hThread);
			} else {
				::CloseHandle(pi.hProcess);
				::CloseHandle(pi.hThread);
			}
		}
	}
};

#endif
