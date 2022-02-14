#include "stdafx.h"
#include "mainWnd.h"
extern HINSTANCE ghInstance;
// #define SC_DEV_MODE

#ifdef DEV_MODE
const unsigned char resources[] = {0x00};
#else 
#include "resources.cpp"
#endif

int uimain(std::function<int()> run )
{
	UINT script_options = ALLOW_FILE_IO | ALLOW_SOCKET_IO | ALLOW_SYSINFO | SCITER_SET_SCRIPT_RUNTIME_FEATURES | ALLOW_EVAL;
	SciterSetOption(nullptr, SCITER_SET_SCRIPT_RUNTIME_FEATURES, script_options);

#ifdef DEV_MODE
	// spdlog::set_level(spdlog::level::debug);
	sciter::debug_output_console console;
	SciterSetOption(nullptr, SCITER_SET_DEBUG_MODE, TRUE);
#endif
#ifdef WIN32
	HICON hIcon = ::LoadIcon(ghInstance, L"IDI_ICON_LOGO");
#else
	SciterSetOption(nullptr, SCITER_SET_GFX_LAYER, GFX_LAYER_SKIA);
#endif

	sciter::string appBaseUrl;
#ifdef DEV_MODE
	appBaseUrl = WSTR("ui/default.htm");
#else
	sciter::archive::instance().open(aux::elements_of(resources));
	appBaseUrl = WSTR("this://app/default.htm");
#endif
	sciter::om::hasset<mainWnd> pMainWnd = new mainWnd();
	SciterSetGlobalAsset(pMainWnd);

	pMainWnd->load(appBaseUrl.c_str());
#ifndef _WIN32
	// extern void initWindow(HWINDOW);
	// initWindow(pMainWnd->get_hwnd());
#endif
	pMainWnd->expand();
	int ret = run();
	
#ifdef _WIN32
	DestroyIcon(hIcon);
#endif

	return ret;
}