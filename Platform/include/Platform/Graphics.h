#include "Platform.h"
#include <GFX/GFXWrapper.h>

namespace Stardust::Platform::detail::Graphics{

    void initGraphicsContext();
    void cleanupGraphicsContext();

    void beginFrame();
    void endFrame();

#if CURRENT_PLATFORM == PLATFORM_PSP
    void beginFrameDialog();
    void endFrameDialog();
#endif
}

