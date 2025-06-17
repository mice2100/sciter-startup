#include "sciter-x-api.h"
#include "sciter-x-window.hpp"
#include "mainWnd.h"
#include "spdlog/spdlog.h"

#ifdef LOCAL_MODE
const unsigned char resources[] = {0x00};
#else
#include "resources.cpp"
#endif

#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif

#ifdef WIN32
	sciter::string GetAppPath()
	{
		WCHAR path[_MAX_PATH] = {0};
		GetModuleFileNameW(NULL, path, _MAX_PATH);
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

#ifdef CONSOLE
	spdlog::set_level(spdlog::level::debug);
	spdlog::debug("debug mode");
	sciter::debug_output_console console;
	SciterSetOption(nullptr, SCITER_SET_DEBUG_MODE, TRUE);
#endif

	sciter::string appBaseUrl;
#ifdef LOCAL_MODE
	sciter::string strRoot = GetAppPath();
	strRoot += WSTR("/ui/main.htm");
	appBaseUrl = Path2Url(strRoot.c_str());
#else
	sciter::archive::instance().open(aux::elements_of(resources));
	appBaseUrl = WSTR("this://app/main.htm");
#endif
	sciter::om::hasset<mainWnd> pMainWnd = new mainWnd();
	SciterSetGlobalAsset(pMainWnd);

	pMainWnd->load(appBaseUrl.c_str());

	int ret = run();

	return ret;
}