#include <Platform/PSP/Power.h>
#include <psppower.h>

namespace Stardust::Platform::PSP{
    auto GetBatteryPercentage() -> int {
        return scePowerGetBatteryLifePercent();
    }

    auto GetBatteryCharging() -> bool{
        return scePowerIsBatteryCharging();
    }

    auto GetBatteryLifeTime() -> std::string {
        int lifetime = scePowerGetBatteryLifeTime();
        std::string res = "";

        if(lifetime / 60 > 0){
            res += std::to_string(lifetime / 60) + "h";
        }
        res += std::to_string(lifetime % 60 + "m");
        return res;
    }
}
