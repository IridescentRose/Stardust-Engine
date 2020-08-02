#include <malloc.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <psputility.h>
#include <string.h>
#include <string>
#include <Utilities/Logger.h>
#undef PSP

namespace Stardust::Platform::PSP{
    int GetBatteryPercentage();
    bool GetBatteryCharging();
    std::string GetBatteryLifeTime();
}