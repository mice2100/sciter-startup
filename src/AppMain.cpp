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

sciter::string GetExecutablePath() {
    try {
        std::filesystem::path execPath;

#ifdef _WIN32
        wchar_t path[MAX_PATH] = { 0 };
        if (GetModuleFileNameW(NULL, path, MAX_PATH) == 0) {
            throw std::runtime_error("Failed to get module filename");
        }
        execPath = path;

#elif defined(__APPLE__)
        char path[PATH_MAX];
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) != 0) {
            throw std::runtime_error("Buffer too small");
        }
        char realPath[PATH_MAX];
        if (!realpath(path, realPath)) {
            throw std::runtime_error("Failed to resolve real path");
        }
        execPath = realPath;

#else
        char path[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
        if (count == -1) {
            throw std::runtime_error("Failed to read symbolic link");
        }
        path[count] = '\0';
        execPath = path;
#endif

        spdlog::info("Executable path: {}", execPath.string());
        return sciter::string(execPath.parent_path().wstring());
    }
    catch (const std::exception& e) {
        spdlog::error("Failed to get executable path: {}", e.what());
        throw;
    }
}

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
	sciter::string strRoot = GetExecutablePath();
	strRoot += L"/ui/main.htm";
	appBaseUrl = Path2Url(strRoot.c_str());
#else
	sciter::archive::instance().open(aux::elements_of(resources));
	appBaseUrl = sciter::string(L"this://app/main.htm");
#endif
	sciter::om::hasset<mainWnd> pMainWnd = new mainWnd();
	SciterSetGlobalAsset(pMainWnd);

	pMainWnd->load(appBaseUrl.c_str());

	int ret = run();

	return ret;
}