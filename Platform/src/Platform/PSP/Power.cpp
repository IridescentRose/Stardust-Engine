#include <Platform/PSP/Power.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <psppower.h>
#else
#include <vitasdk.h>
#endif

namespace Stardust::Platform::PSP{
    int GetBatteryPercentage(){
        return scePowerGetBatteryLifePercent();
    }

    bool GetBatteryCharging(){
        return scePowerIsBatteryCharging();
    }

    std::string GetBatteryLifeTime(){
        int lifetime = scePowerGetBatteryLifeTime();
        std::string res = "";

        if(lifetime / 60 > 0){
            res += std::to_string(lifetime / 60) + "h";
        }
        res += std::to_string(lifetime % 60) + "m";
        return res;
    }
}