#include "Platform.h"

namespace Stardust::Platform::detail::Graphics{

    void initGraphicsContext();
    void cleanupGraphicsContext();

    void clearScreen();

    void clearBuffers();
}