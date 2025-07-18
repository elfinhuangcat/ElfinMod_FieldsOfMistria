#pragma once
#include "Local/Utility.hpp"
#include <YYToolkit/YYTK_Shared.hpp>

using namespace YYTK;
using namespace ElfinMod::FieldsOfMistria;

namespace ElfinMod::FieldsOfMistria::Interfaces
{
    struct YYTKInterfaceLocator 
    {
        
        /**
        * @brief Sets the YYTKInterface instance as a singleton.
        **/
        static void set(YYTKInterface* inst);

        /**
        * @brief Gets the YYTKInterface instance.
        **/
        static YYTKInterface* get() { return interface_; }
    private:
        static YYTKInterface* interface_;
    };

    struct ModConfigLocator 
    {
        
        /**
        * @brief Sets the Utility::Configuration instance as a singleton.
        **/
        static void set(Utility::Configuration* inst);
        /**
        * @brief Gets the Utility::Configuration instance.
        **/
        static Utility::Configuration* get() { return configuration_; }
    private:
        static Utility::Configuration* configuration_;
    };

    struct ModLoggerLocator 
    {
        /**
        * @brief Sets the Utility::Logger instance as a singleton.
        **/
        static void set(Utility::ModLogger* inst);
        /**
        * @brief Gets the Utility::Logger instance.
        **/
        static Utility::ModLogger* get() { return logger_; }

    private:
        static Utility::ModLogger* logger_;
    };
} // namespace ElfinMod::FieldsOfMistria::Interfaces