#pragma once
#include "Local/Interfaces.hpp"
#include "Local/Constants.hpp"
#include "Local/Hook.hpp"
#include <YYToolkit/YYTK_Shared.hpp>
#include<vector>

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

        AurieStatus modifyPalyerValueHelper(
            CInstance* Self,
            CInstance* Other,
            std::string_view scriptName,
            double valueToAdd) 
        {
            // Call the script to modify the player value
            YYTKInterface* moduleInterface = Interfaces::YYTKInterfaceLocator::get();
            std::vector<RValue> args = { RValue(valueToAdd) };
            RValue modifyPlayerValueResult = NULL;
            return moduleInterface->CallGameScriptEx(modifyPlayerValueResult, scriptName, Self, Other, args);
        }

        AurieStatus promptToModifyPlayerValue(
            std::string_view scriptName,
            double valueUpperBoundNonInclusive,
            double valueLowerBoundNonInclusive,
            CInstance* Self,
            CInstance* Other,
            std::string popUpMessageForInput,
            std::string errorMessageToDisplay)
        {
            // Display a pop-up message to get the user input for the value to add:
            YYTKInterface* moduleInterface = Interfaces::YYTKInterfaceLocator::get();
            double valueToAdd = 100; // Default value to add. Please make it safe for all functions that use this!!!
            RValue userInput = moduleInterface->CallBuiltin(
                "get_integer",
                {
                    popUpMessageForInput.c_str(),
                    valueToAdd
                }
            );
            if (userInput.m_Kind == VALUE_UNDEFINED || userInput.m_Kind == VALUE_UNSET)
            {
                // User cancelled the input, do nothing.
                return AURIE_SUCCESS;
            }
            if (userInput.m_Real > valueUpperBoundNonInclusive || userInput.m_Real < valueLowerBoundNonInclusive)
            {
                // User input is out of bounds, show an error message in the console.
                moduleInterface->GetRunnerInterface().YYError(errorMessageToDisplay.c_str());
                return AURIE_SUCCESS; // Do nothing, as the user input is invalid.
            }
            // Call the script to modify player value
            AurieStatus status = modifyPalyerValueHelper(Self, Other, scriptName, userInput.m_Real);
            if (!AurieSuccess(status)) {
                moduleInterface->Print(CM_LIGHTRED, 
                    "%s Failed to modify value using script <%s>!", 
                    Constants::ModNameWithBrackets, scriptName);
            }
            return status;
        }

        /**
        * TODO: This is currently now working
        * @brief Show the mod menu using GameMaker's built-in show_menu function.
        **/
        AurieStatus showModMenuHelper(
            IN CInstance* Self,
            IN CInstance* Other) {
            YYTKInterface* moduleInterface = ElfinMod::FieldsOfMistria::Interfaces::YYTKInterfaceLocator::get();

            // Build arguments: "Option1|Option2|Option3", default index=0, cancel=-1
            std::vector<RValue> args(3);
            // args[0]: the pipe-separated option list (a GML string)
            args[0].m_Kind = VALUE_STRING;
            args[0] = "Add Stamina|Add Gold"; // Should work, as it is overlodaed
            // args[1]: the default highlighted index (0 for first option, 1 for second, etc.)
            args[1].m_Kind = VALUE_REAL;
            args[1].m_Real = 0.0;
            // args[2]: the cancel value (Set it to 99 to allow "Esc" to cancel. If needed, -1 for no cancel option)
            args[2].m_Kind = VALUE_REAL;
            args[2].m_Real = 99.0;

            // Call show_menu and get the result
            RValue result;
            RValue userInput = moduleInterface->CallBuiltin(
                "show_menu",
                args
            );

            // Determine user's choice of menu option. By default using the first option (index 0).
            int choice = (VALUE_REAL == result.m_Kind ? (int)result.m_Real : 0);

            switch (choice) {
            case 0:
            {
                double upperBoundStamina = 999.0; // Upper bound for stamina
                double lowerBoundStamina = 1.0;   // Lower bound for stamina
                std::string popUpMessage = std::format("Please input the stamina you want to add.\r\nMinimum = %d, Maximum = %d",
                    int(lowerBoundStamina), int(upperBoundStamina));
                std::string errorMessage = std::format("\r\nInvalid stamina input.\r\nMinimum = %d, Maximum = %d",
                    int(lowerBoundStamina), int(upperBoundStamina));
                return promptToModifyPlayerValue(
                    Constants::GMLScriptName::GetStamina,
                    upperBoundStamina, // Upper bound for stamina addition value
                    lowerBoundStamina,   // Lower bound for stamina addition value
                    Self,
                    Other,
                    popUpMessage,
                    errorMessage
                );
            }
            case 1:
            {
                double upperBoundGold = 9999.0; // Upper bound for gold
                double lowerBoundGold = 1.0;      // Lower bound for gold
                std::string popUpMessage = std::format("Please input the gold amount you want to add.\r\nMinimum = %d, Maximum = %d",
                    int(lowerBoundGold), int(upperBoundGold));
                std::string errorMessage = std::format("\r\nInvalid gold input.\r\nMinimum = %d, Maximum = %d",
                    int(lowerBoundGold), int(upperBoundGold));
                return promptToModifyPlayerValue(
                    Constants::GMLScriptName::ModifyGold,
                    upperBoundGold, // Upper bound for gold addition value
                    lowerBoundGold,   // Lower bound for gold addition value
                    Self,
                    Other,
                    popUpMessage,
                    errorMessage
                );
            }
            case 99:
                // Cancel option, do nothing
                return AURIE_SUCCESS;
            default: break;
            }
            return AURIE_SUCCESS;
        }
    } // End of anonymous namespace

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

        // If keyboard "F5" is pressed, it will enter the if-clause below:
        if (GetAsyncKeyState(VK_F5) & 1)
        {
            AurieStatus status = promptToModifyPlayerValue(
                Constants::GMLScriptName::ModifyGold,
                9999.0, // Upper bound for gold addition value
                1.0,   // Lower bound for gold addition value
                Self,
                Other,
                "Please input the gold you want to add.\r\nMinimum = 1, Maximum = 9999",
                "\r\nInvalid gold input.\r\nMinimum = 1, Maximum = 9999"
            );
        }
        else if (GetAsyncKeyState(VK_F6) & 1)
        {
            AurieStatus status = promptToModifyPlayerValue(
                Constants::GMLScriptName::ModifyStamina,
                999.0, // Upper bound for stamina addition value
                1.0,   // Lower bound for stamina addition value
                Self,
                Other,
                "Please input the stamina you want to add.\r\nMinimum = 1, Maximum = 999",
                "\r\nInvalid stamina input.\r\nMinimum = 1, Maximum = 999"
            );
        }
        // Run the original function and return the result, either with modification or no F5 was pressed:
        const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(
            g_ArSelfModule, Constants::GMLScriptName::GetStamina));
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
} // End of namespace Hook