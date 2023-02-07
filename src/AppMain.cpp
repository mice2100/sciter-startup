#include "stdafx.h"
#include "mainWnd.h"
extern HINSTANCE ghInstance;

#ifdef LOCAL_MODE
const unsigned char resources[] = {0x00};
#else
#include "resources.cpp"
#endif

#ifdef WIN32
sciter::string GetAppPath()
{
	WCHAR path[PATH_MAX] = {0};
	GetModuleFileNameW(NULL, path, PATH_MAX);
	*wcsrchr(path, L'\\') = '\0';
	return sciter::string(path);
}
#else
sciter::string GetAppPath()
{
	sciter::string appPath = sciter::application::argv()[0];
	std::size_t found = appPath.find_last_of(u"/");
	return appPath.substr(0, found);
}
#endif

sciter::string Path2Url(LPCWSTR path)
{
	sciter::string url = WSTR("file://");
	url = url + sciter::string(path);
	std::replace(std::begin(url), std::end(url), L'\\', L'/');
	return url;
}

int uimain(std::function<int()> run)
{
	UINT script_options = ALLOW_FILE_IO | ALLOW_SOCKET_IO | ALLOW_SYSINFO | SCITER_SET_SCRIPT_RUNTIME_FEATURES | ALLOW_EVAL;
	SciterSetOption(nullptr, SCITER_SET_SCRIPT_RUNTIME_FEATURES, script_options);

#ifdef DEBUG_WINDOW
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
#ifdef LOCAL_MODE
	sciter::string strRoot = GetAppPath();
	strRoot += L"/ui/main.htm";
	appBaseUrl = Path2Url(strRoot.c_str());
#else
	sciter::archive::instance().open(aux::elements_of(resources));
	appBaseUrl = sciter::string(L"this://app/main.htm");
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