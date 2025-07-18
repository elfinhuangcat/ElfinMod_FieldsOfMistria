#include "Local/Interfaces.hpp"
#include "Local/Utility.hpp"
#include <YYToolkit/YYTK_Shared.hpp>

using namespace YYTK;
using namespace ElfinMod::FieldsOfMistria;

namespace ElfinMod::FieldsOfMistria::Interfaces
{
    YYTKInterface* YYTKInterfaceLocator::interface_ = nullptr;

    void YYTKInterfaceLocator::set(YYTKInterface* inst)
    {
        if (interface_ != nullptr)
        {
            // This error should never happen, as the interface should only be set once.
            throw std::runtime_error("YYTKInterface has already been set.");
        }
        interface_ = inst;
    }

    Utility::Configuration* ModConfigLocator::configuration_ = nullptr;
    void ModConfigLocator::set(Utility::Configuration* config)
    {
        if (configuration_ != nullptr)
        {
            // This error should never happen, as the configuration should only be set once.
            throw std::runtime_error("Mod configuration has already been set.");
        }
        configuration_ = config;
    }

    Utility::ModLogger* ModLoggerLocator::logger_ = nullptr;
    void ModLoggerLocator::set(Utility::ModLogger* logger)
    {
        if (logger_ != nullptr)
        {
            // This error should never happen, as the logger should only be set once.
            throw std::runtime_error("Mod logger has already been set.");
        }
        logger_ = logger;
    }
} // namespace ElfinMod::FieldsOfMistria::Interfaces