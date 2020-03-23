#pragma once
#include <Platform/Platform.h>

#if CURRENT_PLATFORM == PLATFORM_PSP

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <psppower.h>
#include <psptypes.h>
#include <pspge.h>
#include <psputils.h>

#endif

#include <Graphics/RenderTypes.h>

namespace Stardust::Graphics {

    class Texture;

    class RendererCore {
    public:
        RendererCore();
        ~RendererCore();

        void Init();
        void Clear();

#if CURRENT_PLATFORM == PLATFORM_PSP
        inline void SetClearColor(float r, float g, float b, float a) {
            clearColor = GU_COLOR(r, g, b, a);
            sceGuClearColor(clearColor);
        }
#endif

        void BeginCommands(bool dialog = false);
        void EndCommands(bool dialog = false);

        void RenderToTexture(Texture* tex);

        void Blit(int sx, int sy, int sw, int sh, int dx, int dy, int slice);

    private:
        unsigned int clearColor;

        char list[0x20000] __attribute__((aligned(64)));

        void* _fbp0;
        void* _fbp1;
        void* _zbp;
        void* _frameBuffer;
    };

}