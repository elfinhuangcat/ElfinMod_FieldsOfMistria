#pragma once

namespace ElfinMod::FieldsOfMistria::Constants
{
    // Mod name
    constexpr auto ModName = "Mistria Companion";
    // Mod name with brackets for logging
    constexpr auto ModNameWithBrackets = "[Mistria Companion";
    // Mod version
    constexpr auto ModVersion = "0.0.1";
    // Mod author
    constexpr auto ModAuthor = "elfinhuangcat";
    // Mod configuration file path
    constexpr auto ModConfigFilePath = "mods/MistriaCompanion.ini";
    // Mod log file path
    constexpr auto ModLogFilePath = "MistriaCompanion.log";
};

namespace ElfinMod::FieldsOfMistria::Constants::GMLScriptName
{
    constexpr auto GetStamina = "gml_Script_get_stamina@Ari@Ari";
    constexpr auto ModifyStamina = "gml_Script_modify_stamina@Ari@Ari";
    constexpr auto ModifyGold = "gml_Script_modify_gold@Ari@Ari";
    constexpr auto SpawnRequestBoardMenu = "gml_Script_spawn_request_board_menu";
};

namespace ElfinMod::FieldsOfMistria::Constants::ModConfig
{
    constexpr auto ModifyStaminaKey = "DefaultModifyStaminaValue";
    const int DefaultModifyStaminaValue = 50;

    constexpr auto ModifyHealthKey = "DefaultModifyHealthValue";
    const int DefaultModifyHealthValue = 50;
    
    constexpr auto ModifyGoldKey = "DefaultModifyGoldValue";
    const int DefaultModifyGoldValue = 1000;

    constexpr auto ModifyManaKey = "DefaultModifyManaValue";
    const int DefaultModifyManaValue = 1;
};