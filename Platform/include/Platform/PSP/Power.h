#include <Platform/Platform.h>
#include <malloc.h>
#if CURRENT_PLATFORM == PLATFORM_PSP
#include <pspkernel.h>
#include <pspctrl.h>
#include <psputility.h>
#else
#include <vitasdk.h>
#endif
#include <string.h>
#include <string>
#include <Utilities/Logger.h>
#undef PSP

namespace Stardust::Platform::PSP{
    int GetBatteryPercentage();
    bool GetBatteryCharging();
    std::string GetBatteryLifeTime();
}