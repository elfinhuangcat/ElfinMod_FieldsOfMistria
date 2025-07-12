#include <Local/Utility.hpp>
#include <Local/Interfaces.hpp>
#include <Local/Constants.hpp>
#include <Local/Hook.hpp>
#include <YYToolkit/YYTK_Shared.hpp>
#include <fstream>
using namespace Aurie;
using namespace YYTK;
using namespace ElfinMod::FieldsOfMistria;

// The YYTK interface which is needed by everything in the YYTK mods.
//static YYTKInterface* g_ModuleInterface = nullptr;

EXPORTED AurieStatus ModuleInitialize(
    IN AurieModule* Module,
    IN const fs::path& ModulePath
)
{
    UNREFERENCED_PARAMETER(ModulePath);
    if (!Utility::InitializeYYTKInterface())
    {
        // Quick exit if we cannot initialize the YYTK interface.
        return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
    }

    // Now we can use the module interface for our modding needs.
    YYTKInterface* moduleInterface = Interfaces::YYTKInterfaceLocator::get();
    AurieStatus lastStatus = AURIE_SUCCESS;

    // TODO: Remove this line when not debugging the module.
    moduleInterface->Print(CM_LIGHTGREEN, "%s Entered ModuleMain.", Constants::ModNameWithBrackets); 

    lastStatus = Hook::CreateHookGmlScriptGetStamina();

    if (lastStatus != AURIE_SUCCESS)
    {
        moduleInterface->Print(CM_RED, "%s Failed to create hook for get stamina.", Constants::ModNameWithBrackets);
        return lastStatus;
    }

    lastStatus = Hook::CreateHookGmlScriptSpawnRequestBoard();

    if (lastStatus != AURIE_SUCCESS)
    {
        moduleInterface->Print(CM_RED, "%s Failed to create hook for spawn request board.", Constants::ModNameWithBrackets);
        return lastStatus;
    }

    return AURIE_SUCCESS;

}