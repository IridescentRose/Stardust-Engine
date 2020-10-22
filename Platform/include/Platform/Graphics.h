#include "Platform.h"
#include <GFX/GFXWrapper.h>

namespace Stardust::Platform::detail::Graphics{

    auto initGraphicsContext() -> void;
    auto cleanupGraphicsContext() -> void;

    auto beginFrame() -> void;
    auto endFrame() -> void;

#if CURRENT_PLATFORM == PLATFORM_PSP
    auto beginFrameDialog() -> void;
    auto endFrameDialog() -> void;
#endif
}

