#ifndef _Kompas3DPrint_Kompas3DPrint_h
#define _Kompas3DPrint_Kompas3DPrint_h
#include "Kompas/Kompas3D.h"

#define IDR_LIBID 100
#define MENU_SETTINGS                   1
#define MENU_OPEN_CURA                  2
#define MENU_EXPORT_STL                 3
#define MENU_EXPORT_STEP                4
#define MENU_EXPORT_IGS                 5
#define MENU_EXPORT_X_T                 6
#define MENU_EXPORT_ACIS                7
#define MENU_EXPORT_VRLM                8
#define MENU_ABOUT                      20
#define DllExport extern "C" __declspec(dllexport)

DllExport int DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
DllExport int LIBRARYID();
DllExport void LIBRARYENTRY(unsigned int comm);
DllExport bool LibInterfaceNotifyEntry(IDispatch *application);

#endif
