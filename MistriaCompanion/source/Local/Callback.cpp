#pragma once
#include "Local/Interfaces.hpp"
#include "Local/Callback.hpp"
#include <YYToolkit/YYTK_Shared.hpp>
using namespace Aurie;
using namespace YYTK;
using namespace ElfinMod::FieldsOfMistria;

namespace ElfinMod::FieldsOfMistria::Callback
{
    // This method assumes the YYTKInterface is already initialized and the game is running.
	void ListenToMenuRequest(FWFrame& FrameContext)
	{
        // At any frame, if keyboard "F5" is pressed, it will enter the if-clause below:
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
                return;
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
			moduleInterface->CallGameScript("gml_Script_modify_stamina@Ari@Ari", args);

			moduleInterface->Print(CM_LIGHTGREEN, "elfintest - Finish calling script");
		}
    }
}