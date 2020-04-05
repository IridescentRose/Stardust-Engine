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
#include <stdarg.h>
#include <intraFont.h>
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
        inline void SetClearColor(int r, int g, int b, int a) {
            clearColor = GU_RGBA(r, g, b, a);
            sceGuClearColor(clearColor);
        }
#endif

        void BeginCommands(bool dialog = false);
        void EndCommands(bool dialog = false);

        void RenderToTexture(Texture* tex);

        void Blit(int sx, int sy, int sw, int sh, int dx, int dy, int slice);

#ifdef MC_PSP
        intraFont* debugFont;
        intraFont* numFont;
        intraFont* engFont;
        intraFont* rusFont;
        intraFont* jpn0;
        intraFont* kr0;
        intraFont* arib;
        intraFont* chn;
        intraFont* defFont;
        float fontVerticalShift;
        void InitDebugFont();

        void SetFontStyle(float size, unsigned int color, unsigned int shadowColor, unsigned int options, float angle);
        void DebugPrint(int x, int y, const char* message, ...);
#endif

    private:
        unsigned int clearColor;

        char list[0x20000] __attribute__((aligned(64)));

        void* _fbp0;
        void* _fbp1;
        void* _zbp;
        void* _frameBuffer;
    };

    extern RendererCore g_RenderCore;

}