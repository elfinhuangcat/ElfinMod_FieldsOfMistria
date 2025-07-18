#pragma once

#include <YYToolkit/YYTK_Shared.hpp>
#include "Local/Interfaces.hpp"
#include "Local/Constants.hpp"
#include "Local/Utility.hpp"
#include <fstream>
#include <sstream>
#include <string>

using namespace Aurie;
using namespace YYTK;
using namespace ElfinMod::FieldsOfMistria::Constants;

namespace ElfinMod::FieldsOfMistria::Utility
{
    // Forward declaration of the helper functions that are privite to this namespace:
    bool _fileExists(const std::filesystem::path& p);
    void _saveModConfig(const std::string& path);

    // Implementation of the Configuration class constructor
    Configuration::Configuration(int staminaToModify, int goldToModify)
        : modifyStaminaValue(staminaToModify)   // initialize const member
        , modifyGoldValue(goldToModify)         // initialize const member
    {
        // nothing else to do here, all members are const and set above
    }

    // Implementation of the ModLogger
    ModLogger::ModLogger(const std::string& filePath)
    {
        // Open the log file for writing
        logFile.open(filePath, std::ios::out | std::ios::app);
    }
    void ModLogger::Log(const std::string& message)
    {
        if (logFile.is_open())
        {
            // Write the message to the log file
            logFile << Constants::ModNameWithBrackets << " " << message << std::endl;
        }
    }
    void ModLogger::CloseLogFile()
    {
        if (logFile.is_open())
        {
            // Close the log file
            logFile.close();
        }
    }
    

    bool InitializeYYTKInterface()
    {
        YYTKInterface* moduleInterface = nullptr;
        AurieStatus last_status = AURIE_SUCCESS;

        // Retrieve the interface to the YYTK module system, which is the base for all modules.
        last_status = ObGetInterface(
            "YYTK_Main",
            (AurieInterfaceBase*&)(moduleInterface)
        );

        if (!AurieSuccess(last_status) || moduleInterface == nullptr)
        {
            return false;
        }
        else
        {
            // Set the retrieved interface to the global variable.
            ElfinMod::FieldsOfMistria::Interfaces::YYTKInterfaceLocator::set(moduleInterface);
            return true;
        }
    }

    std::string ReturnRValueAsString(RValue& value)
    {
        if (value.m_Kind == VALUE_UNDEFINED)
        {
            return "undefined";
        }
        else if (value.m_Kind == VALUE_NULL)
        {
            return "null";
        }
        else if (value.m_Kind == VALUE_REAL)
        {
            return std::to_string(value.m_Real);
        }
        else if (value.m_Kind == VALUE_INT32)
        {
            return std::to_string(value.m_i32);
        }
        else if (value.m_Kind == VALUE_INT64)
        {
            return std::to_string(value.m_i64);
        }
        else if (value.m_Kind == VALUE_BOOL)
        {
            return (value.m_Real >= 1.0) ? "true" : "false";
        }
        else if (value.m_Kind == VALUE_STRING)
        {
            return value.ToString();
        }
        else
        {
            return "Unknown type: " + std::to_string(value.m_Kind);
        }
    }

    bool DumpGMLScriptNamesToFile(YYTKInterface* g_ModuleInterface, const char* filePath)
    {
        if (g_ModuleInterface == nullptr || filePath == nullptr)
        {
            return false;
        }

        auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));

        // Create a new txt file in the path: filePath
        std::ofstream file(filePath, std::ios::out | std::ios::trunc);

        // Assuming there is no more than 100000 scripts.
        // We are iterating through all possible integers since we don't know the script IDs.
        for (int id = 0; id < 100000; id++)
        {
            CScript* script = nullptr;
            auto success = g_ModuleInterface->GetScriptData(id, script);
            if (!AurieSuccess(success)) continue;

            if (script == nullptr) continue;

            auto functions = script->m_Functions;
            if (functions == nullptr)continue;

            auto func = functions->m_ScriptFunction;
            if (func == nullptr)continue;

            auto rva = reinterpret_cast<uintptr_t>(func) - base;
            file << script->m_Name << " " << std::hex << rva << std::endl;
        }

        file.close();
    }

    

    // Deserialize
    bool LoadModConfig(const std::string& path) 
    {

        if (_fileExists(path)) 
        {
            // If the file exists, load it.
            // Open the file for reading:
            std::ifstream configFile(path);
            std::string line;

            // tempoary variables to hold the values:
            int modifyStaminaValue = Constants::ModConfig::DefaultModifyStaminaValue;
            int modifyGoldValue = Constants::ModConfig::DefaultModifyGoldValue;
            int modifyHealthValue = Constants::ModConfig::DefaultModifyHealthValue;
            int modifyManaValue = Constants::ModConfig::DefaultModifyManaValue;


            while (std::getline(configFile, line)) 
            {
                if (auto pos = line.find('='); pos != std::string::npos) 
                {
                    std::string key = line.substr(0, pos);
                    std::string val = line.substr(pos + 1);

                    if (Constants::ModConfig::ModifyStaminaKey == key)
                    {
                        modifyStaminaValue = std::stoi(val);
                    } else if (Constants::ModConfig::ModifyGoldKey == key)
                    {
                        modifyGoldValue = std::stoi(val);
                    } else if (Constants::ModConfig::ModifyHealthKey == key) 
                    {
                        modifyHealthValue = std::stoi(val);
                    } else if (Constants::ModConfig::ModifyManaKey == key) 
                    {
                        modifyManaValue = std::stoi(val);
                    }
                    else 
                    {
                        Interfaces::YYTKInterfaceLocator::get()->Print(
                            CM_RED, "%s Unrecognize mod configuration line: %s", 
                            Constants::ModNameWithBrackets, line);
                    }
                }
            }
            auto modConfig = Configuration(
                modifyStaminaValue,
                modifyGoldValue
            );

            Interfaces::ModConfigLocator::set(&modConfig);
            return true;
        } 
        else 
        {
            // If the file does not exist, create it with default values.
            _saveModConfig(path);
            auto defaultConfig = Configuration(
                Constants::ModConfig::DefaultModifyStaminaValue,
                Constants::ModConfig::DefaultModifyGoldValue
            );
            Interfaces::ModConfigLocator::set(&defaultConfig);
            return true;
        }
    }

    /**
    * @brief Checks if a file exists at the given path.
    **/
    bool _fileExists(const std::filesystem::path& p) 
    {
        return std::filesystem::exists(p)
            && std::filesystem::is_regular_file(p);
    }

    /**
    * @brief Saves the mod configuration to the specified path (used when config file does not exist).
    *        Note: It does not check if the file already exists, it will overwrite it.
    **/
    void _saveModConfig(const std::string& path) 
    {
        // Create the config file with the path provided.
        std::ofstream configFile(path);

        configFile << Constants::ModConfig::ModifyStaminaKey << "=" << Constants::ModConfig::DefaultModifyStaminaValue << "\n";
        configFile << Constants::ModConfig::ModifyGoldKey << "=" << Constants::ModConfig::DefaultModifyGoldValue << "\n";

        configFile.close();
    }
}