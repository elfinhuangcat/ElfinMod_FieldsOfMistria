#pragma once
#include <YYToolkit/YYTK_Shared.hpp>

using namespace YYTK;

namespace ElfinMod::FieldsOfMistria::Interfaces
{
    struct YYTKInterfaceLocator {
        
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
} // namespace ElfinMod::FieldsOfMistria::Interfaces