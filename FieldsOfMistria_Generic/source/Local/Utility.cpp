#pragma once
#include <Local/Interfaces.hpp>
#include <YYToolkit/YYTK_Shared.hpp>
#include <fstream>

using namespace Aurie;
using namespace YYTK;

namespace ElfinMod::FieldsOfMistria::Utility
{
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
}