#ifndef __MAINWND_H__
#define __MAINWND_H__

#include "sciter-x-window.hpp"

class mainWnd : public sciter::window
{
public:
    mainWnd() : window(SW_TITLEBAR | SW_RESIZEABLE | SW_CONTROLS | SW_MAIN | SW_ENABLE_DEBUG) {}

    // sciter::asset interface declarations.

    // These native functions are introduced to JS through assetInterface.
    // Call them in JS as
    //
    //   Window.this.assetInterface.integerSum(12,24)
    //

    std::string stringSum(const std::string a, const std::string b) { return a + "+" + b; }

    // virtual property
    int get_windowHandle()
    {
        return (int)(intptr_t)get_hwnd();
    }

    SOM_PASSPORT_BEGIN_EX(assetInterface, mainWnd)
        SOM_FUNCS(
            SOM_FUNC(stringSum) )
        SOM_PROPS(
            SOM_RO_VIRTUAL_PROP(windowHandle, get_windowHandle) )
    SOM_PASSPORT_END

    // behavioral "named call" handler demo:
    // test in script:
    //
    //  Window.this.xcall("debug",12,24);
    //
    // "mapped" implementation of
    //   virtual bool sciter::event_handler::on_script_call(HELEMENT he, LPCSTR name, UINT argc, const sciter::value* argv, sciter::value& retval)

    BEGIN_FUNCTION_MAP
    FUNCTION_1("xcallTest", xcallTest);
    END_FUNCTION_MAP

    sciter::value xcallTest(sciter::value data)
    {
        return data.get<int>() * 2;
    }
};

#endif
