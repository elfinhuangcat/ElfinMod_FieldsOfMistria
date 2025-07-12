#include "Local/Interfaces.hpp"
#include <YYToolkit/YYTK_Shared.hpp>

using namespace YYTK;

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
} // namespace ElfinMod::FieldsOfMistria::Interfaces