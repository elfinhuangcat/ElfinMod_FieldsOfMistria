#pragma once
#include <YYToolkit/YYTK_Shared.hpp>
#include <fstream>

using namespace Aurie;
using namespace YYTK;

namespace ElfinMod::FieldsOfMistria::Utility
{
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
}
