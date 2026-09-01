#ifndef _Kompas3DPrint_h
#define _Kompas3DPrint_h

#include "Resources.h"

#define DllExport extern "C" __declspec(dllexport)

DllExport int DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
DllExport int LIBRARYID() { return IDR_LIBID; }

#endif
