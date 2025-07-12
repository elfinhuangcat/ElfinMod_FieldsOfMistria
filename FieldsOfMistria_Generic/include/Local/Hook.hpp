#pragma once
#include <YYToolkit/YYTK_Shared.hpp>

using namespace YYTK;

namespace ElfinMod::FieldsOfMistria::Hook
{
    /**
    * @brief Creates a hook for the gml_Script_get_stamina@Ari@Ari function.
    *        Since this function is called at a fixed and frequent interval in the game,
    *        it is used as a "frame event callback" workaround to supply "Self" and "Other" 
    *        parameters to calls to "CallScriptEx" in YYTK.
    **/
    AurieStatus CreateHookGmlScriptGetStamina();

    /**
    * TODO: testing
    **/
    AurieStatus CreateHookGmlScriptSpawnRequestBoard();

    /**
    * @brief Hook for the gml_Script_spawn_request_board_menu function. Doing nothing for now.
    **/
    RValue& GmlScriptSpawnRequestBoardHook(
        IN CInstance* Self,
        IN CInstance* Other,
        OUT RValue& Result,
        IN int ArgumentCount,
        IN RValue** Arguments
    );

    /**
    * @brief Hook for the gml_Script_get_stamina@Ari@Ari function.
    *        Note: This hook is used as a "frame event callback" to supply "Self" and "Other" 
    *              parameters when needing to call "CallScriptEx" in YYTK, because this GML
    *              script is called in the game with a fixed and frequent interval (similar to a frame event).
    **/
    RValue& GmlScriptGetStaminaHook(
        IN CInstance* Self,
        IN CInstance* Other,
        OUT RValue& Result,
        IN int ArgumentCount,
        IN RValue** Arguments
    );
}