#pragma once
#include <YYToolkit/YYTK_Shared.hpp>
#include <Local/Utility.hpp>
#include <fstream>

using namespace Aurie;
using namespace YYTK;

namespace ElfinMod::FieldsOfMistria::Utility
{
    class Configuration 
    {
    private:
        int modifyStaminaValue;
        int modifyGoldValue;

    public:
        // constructor
        Configuration(int staminaToModify, int goldToModify);

        // getter
        const int getModifyStaminaValue() const { return modifyStaminaValue; }
        const int getModifyGoldValue() const { return modifyGoldValue; }
    };

    class ModLogger
    {
    private:
        std::ofstream logFile;
    public:
        ModLogger(const std::string& filePath);
        
        // Logging with mod name prefix and new line
        void Log(const std::string& message);

        // Close log file
        void CloseLogFile();
    };

    /**
    * @brief Initializes the YYTK interface for the Fields of Mistria game. 
    *        This function should be called once and only once at the start of this program.
    **/
    bool InitializeYYTKInterface();

    /**
    * @brief Stolen from Discord (author unknown).
    *        Dumps the names of all GML scripts to a file. This is for debugging and mod development purposes only.
    *        Prints the name + relative address of each script relative to the game process base address. (I guess)
    * @param g_ModuleInterface Pointer to the YYTK interface for accessing GML scripts, which should not be null.
    * @param filePath The path to the file where the script names will be dumped.
    * @return Returns true if the operation was successful, false otherwise.
    **/
    bool DumpGMLScriptNamesToFile(YYTKInterface* g_ModuleInterface, const char* filePath);


    /**
    * @brief Converts a GML RValue to a string representation. This is only used for debugging purposes.
    * @param value The RValue to convert.
    **/
    std::string ReturnRValueAsString(RValue& value);


    /**
    * @brief Loads the mod configuration from a .ini like file and returns a Configuration object.
    * @param path The path to the configuration file.
    * @return Returns true if the configuration was loaded successfully, false otherwise.
    **/
    bool LoadModConfig(const std::string& path);
}
