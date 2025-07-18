#include "Local/Utility.hpp"
#include "Local/Interfaces.hpp"
#include "Local/Constants.hpp"
#include "Local/Hook.hpp"
#include <YYToolkit/YYTK_Shared.hpp>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include <fstream>
using namespace Aurie;
using namespace YYTK;
using namespace ElfinMod::FieldsOfMistria;


// The YYTK interface which is needed by everything in the YYTK mods.
//static YYTKInterface* g_ModuleInterface = nullptr;

static WNDPROC   g_OriginalWndProc = nullptr;
static HWND      g_GameWindow = nullptr;
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


//------------------------------------------------------------------------------
// 2) Our subclassed WndProc
//------------------------------------------------------------------------------
static LRESULT CALLBACK MySubclassedWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // First, give the Win32 backend a crack at it
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;    // ImGui wants to capture this event
    }

    // Otherwise, call the game¡¯s original WndProc
    return ::CallWindowProcW(g_OriginalWndProc, hWnd, msg, wParam, lParam);
}

EXPORTED AurieStatus ModuleInitialize(
    IN AurieModule* Module,
    IN const fs::path& ModulePath
)
{
    UNREFERENCED_PARAMETER(ModulePath);

    // Initialize the mod log file:
    Utility::ModLogger logger = Utility::ModLogger::ModLogger(Constants::ModLogFilePath);
    Interfaces::ModLoggerLocator::set(&logger);

    if (!Utility::InitializeYYTKInterface())
    {
        // Quick exit if we cannot initialize the YYTK interface.
        return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
    }

    // TODO
    Interfaces::ModLoggerLocator::get()->Log("Successfully loaded YYTK interface.");

    // Initialize the mod configuration
    Utility::LoadModConfig(Constants::ModConfigFilePath);

    // TODO
    Interfaces::ModLoggerLocator::get()->Log("Successfully loaded mod configuration.");

    // Now we can use the module interface for our modding needs.
    YYTKInterface* moduleInterface = Interfaces::YYTKInterfaceLocator::get();
    AurieStatus lastStatus = AURIE_SUCCESS;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    Interfaces::ModLoggerLocator::get()->Log("IMGUI check version done");
    ImGui::CreateContext();
    Interfaces::ModLoggerLocator::get()->Log("IMGUI create context done");
    ImGuiIO& io = ImGui::GetIO();
    Interfaces::ModLoggerLocator::get()->Log("IMGUI Get IO done");
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    Interfaces::ModLoggerLocator::get()->Log("IMGUI enable keyboard controls done");
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    Interfaces::ModLoggerLocator::get()->Log("IMGUI enable gamepad control done");

    // Setup Platform/Renderer backends
    // 1a) Find the game¡¯s main window (you can adjust the title/class as needed)
    g_GameWindow = ::FindWindowW(nullptr, L"Fields of Mistria");
    Interfaces::ModLoggerLocator::get()->Log("IMGUI Find window done");
    if (!g_GameWindow)
    { 
        // return AURIE_MODULE_INTERNAL_ERROR;

        moduleInterface->Print(CM_RED, "%s Failed to find game window.", Constants::ModNameWithBrackets);
        Interfaces::ModLoggerLocator::get()->Log("IMGUI cannot find game window");
    } 
    else
    {
        g_OriginalWndProc = (WNDPROC)
            ::SetWindowLongPtrW(g_GameWindow, GWLP_WNDPROC,
                (LONG_PTR)MySubclassedWndProc);
        Interfaces::ModLoggerLocator::get()->Log("IMGUI original wnd proc");
        ImGui_ImplWin32_Init(g_GameWindow);
        Interfaces::ModLoggerLocator::get()->Log("IMGUI implwin32 init");

        // TODO: This is currently failing so I am disabling it for now.
        // ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
        // Interfaces::ModLoggerLocator::get()->Log("IMGUI pd3d device");
    }
    

    Interfaces::ModLoggerLocator::get()->Log("Finish context setup. Entered main");

    lastStatus = Hook::CreateHookGmlScriptGetStamina();

    Interfaces::ModLoggerLocator::get()->Log("Finished create hook");

    if (lastStatus != AURIE_SUCCESS)
    {
        moduleInterface->Print(CM_RED, "%s Failed to create hook for get stamina.", Constants::ModNameWithBrackets);
        Interfaces::ModLoggerLocator::get()->Log("Failed to create hook for get stamina");
        return lastStatus;
    }

    return AURIE_SUCCESS;

}

EXPORTED AurieStatus ModuleUnload(
    IN AurieModule* Module,
    IN const fs::path& ModulePath
)
{
    UNREFERENCED_PARAMETER(Module);
    UNREFERENCED_PARAMETER(ModulePath);

    Interfaces::ModLoggerLocator::get()->Log("Triggering cleanup in module unload");
    Interfaces::ModLoggerLocator::get()->CloseLogFile();

    // Close the console window only AFTER freeing it, otherwise we close the game as well!
    HWND console_window = GetConsoleWindow();
    FreeConsole();
    /**
    CmWriteLogOutput("[%s:%d] Unloading.", __FILE__, __LINE__);

    YYTK::CmpCloseLogFile();
    **/

    return AURIE_SUCCESS;
}