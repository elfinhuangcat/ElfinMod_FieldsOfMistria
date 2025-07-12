#pragma once
#include "Local/Interfaces.hpp"
#include "Local/Constants.hpp"
#include <YYToolkit/YYTK_Shared.hpp>

using namespace YYTK;
using namespace Aurie;
using namespace ElfinMod::FieldsOfMistria;

// Alias the function©\pointer type you¡¯re hooking.
//  PFUNC_YYGMLScript is defined in YYTK_Shared.hpp as:
//  using PFUNC_YYGMLScript = RValue& (*)(CInstance*, CInstance*, RValue&, int, RValue**);
using ScriptFn = PFUNC_YYGMLScript;

namespace ElfinMod::FieldsOfMistria::Hook
{
    namespace
    {
        AurieStatus createHookHelper(std::string ScriptName, ScriptFn HookFunction)
        {
            /*
            IN PVOID SourceFunction,
                IN PVOID DestinationFunction,
                OUT OPTIONAL PVOID* Trampoline
            */
            CScript* destinationFunction = nullptr;

            // Retrieve the global YYTK interface and check if it is valid
            YYTKInterface* moduleInterface = Interfaces::YYTKInterfaceLocator::get();

            // Retrieve the destination function that needs to be hooked by the script name
            AurieStatus lastStatus = moduleInterface->GetNamedRoutinePointer(
                ScriptName.c_str(),
                (PVOID*)&destinationFunction
            );

            if (!AurieSuccess(lastStatus))
            {
                moduleInterface->Print(CM_LIGHTRED, "%s Failed to get script (%s)!", Constants::ModNameWithBrackets, ScriptName);
                return lastStatus;
            }

            lastStatus = MmCreateHook(
                g_ArSelfModule, // Probably a global variable instantiated by Aurie or YYTK
                ScriptName,     // Name of the script
                destinationFunction->m_Functions->m_ScriptFunction,
                reinterpret_cast<PVOID>(HookFunction),
                nullptr
            );

            if (!AurieSuccess(lastStatus))
            {
                moduleInterface->Print(CM_LIGHTRED, "%s Failed to hook script %s!", Constants::ModNameWithBrackets, ScriptName);
            }
            return lastStatus;
        }
    }
    // TODO: Currently the hook only intercepts the script and prints the arguments.
    RValue& GmlScriptSpawnRequestBoardHook(
        IN CInstance* Self,
        IN CInstance* Other,
        OUT RValue& Result,
        IN int ArgumentCount,
        IN RValue** Arguments
    )
    {
        // Retrieve the global YYTK interface and check if it is valid
        YYTKInterface* g_ModuleInterface = Interfaces::YYTKInterfaceLocator::get();
        if (g_ModuleInterface == nullptr)
        {
            // If the interface is not available, print an error message and return
            g_ModuleInterface->Print(CM_RED, "%s YYTK interface is not available.", Constants::ModNameWithBrackets);
            return Result; // Return early if the interface is not available
        }


        g_ModuleInterface->Print(CM_LIGHTBLUE, "elfintest - Entered the hook function <GmlScriptSpawnRequestBoardHook>.");

        g_ModuleInterface->Print(CM_LIGHTBLUE, "elfintest - Argument Count: %d. Arguments:", ArgumentCount);
        for (int i = 0; i < ArgumentCount; ++i)
        {
            g_ModuleInterface->Print(CM_LIGHTBLUE, "elfintest - Arg[%d] type: %s", i, Arguments[i]->GetKindName());
        }

        const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_spawn_request_board_menu"));
        original(
            Self,
            Other,
            Result,
            ArgumentCount,
            Arguments
        );

        g_ModuleInterface->Print(CM_LIGHTBLUE, "yaxinhtest - Result type: %s", Result.GetKindName());
        return Result;
    }

    RValue& GmlScriptGetStaminaHook(
        IN CInstance* Self,
        IN CInstance* Other,
        OUT RValue& Result,
        IN int ArgumentCount,
        IN RValue** Arguments
    )
    {
        // Retrieve the global YYTK interface and check if it is valid
        YYTKInterface* g_ModuleInterface = Interfaces::YYTKInterfaceLocator::get();
        if (g_ModuleInterface == nullptr)
        {
            // If the interface is not available, ignore the call and return
            g_ModuleInterface->Print(CM_RED, "%s YYTK interface is not available.", Constants::ModNameWithBrackets);
            return Result; // Return early if the interface is not available
        }

        g_ModuleInterface->Print(CM_LIGHTBLUE, "elfintest - Entered the hook function <GmlScriptGetStaminaHook>.");
        // If keyboard "F5" is pressed, it will enter the if-clause below:
        if (GetAsyncKeyState(VK_F5) & 1)
        {
            // Currently only take in an integer which will be the stamina value to add.
            // TODO: Expand more features!

            // Attempt to get a number from the user.
            YYTKInterface* moduleInterface = Interfaces::YYTKInterfaceLocator::get();
            double staminaToAdd = 20; // Default value for stamina to add

            moduleInterface->Print(CM_LIGHTGREEN, "elfintest - F5 has been pressed to trigger menu.");
            RValue userInput = moduleInterface->CallBuiltin(
                "get_integer",
                {
                    "Please input the stamina you want to add.\r\n"
                    "(Ex.: 20 - to save you a run to the fountain)\r\n"
                    "Minimum = 1, Maximum = 999",
                    staminaToAdd
                }
            );

            if (userInput.m_Kind == VALUE_UNDEFINED || userInput.m_Kind == VALUE_UNSET)
            {
                // User cancelled the input, do nothing.
                moduleInterface->Print(CM_LIGHTGREEN, "elfintest - Noting is entered.");
                return Result;
            }
            if (userInput.m_Real > 999.0 || userInput.m_Real < 1.0)
            {
                moduleInterface->Print(CM_LIGHTGREEN, "elfintest - Input value not valid");
                // User input is out of bounds, show an error message in the console.
                // The game will not crash, but the input will be ignored.
                moduleInterface->GetRunnerInterface().YYError(
                    "\r\nInvalid stamina input.\r\n"
                    "Minimum = 1, Maximum = 999\r\n"
                );
                return Result; // Return early if the input is invalid
            }
            /*
            virtual RValue CallGameScript(
            IN std::string_view ScriptName,
            IN const std::vector<RValue>& Arguments
        ) = 0;
            */

            // Call the script to add the stamina:
            moduleInterface->Print(CM_LIGHTGREEN, "elfintest - Going to call script");
            std::vector<RValue> args = { RValue(userInput) };
            RValue modifyStaminaResult = NULL;
            moduleInterface->CallGameScriptEx(modifyStaminaResult, "gml_Script_modify_stamina@Ari@Ari", Self, Other, args);

            moduleInterface->Print(CM_LIGHTGREEN, "elfintest - Finish calling script");
        } // End of if-clause for F5 key pressed.

        // Run the original function and return the result, either with modification or no F5 was pressed:
        const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_get_stamina@Ari@Ari"));
        original(
            Self,
            Other,
            Result,
            ArgumentCount,
            Arguments
        );
        return Result;
    }
    
    AurieStatus CreateHookGmlScriptGetStamina()
    {
        return createHookHelper(Constants::GMLScriptName::GetStamina, GmlScriptGetStaminaHook);
    }

    AurieStatus CreateHookGmlScriptSpawnRequestBoard()
    {
        return createHookHelper(Constants::GMLScriptName::SpawnRequestBoardMenu, GmlScriptSpawnRequestBoardHook);
    }
} // End of namespace Hook