#include <malloc.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <psputility.h>
#include <string.h>
#include <string>
#include <Utilities/Logger.h>
#undef PSP

namespace Stardust::Platform::PSP{
    auto GetBatteryPercentage() -> int;
    auto GetBatteryCharging() -> bool;
    auto GetBatteryLifeTime() -> std::string;
}
