#pragma once
#include <YYToolkit/YYTK_Shared.hpp>
using namespace Aurie;
using namespace YYTK;

namespace ElfinMod::FieldsOfMistria::Callback
{
	/**
	* @brief This callback is plugin into the Aurie framework to listen for request to open the mod menu.
    *        The mod menu is opened by pressing the F5 key.
	**/
	void ListenToMenuRequest(FWFrame& FrameContext);
}