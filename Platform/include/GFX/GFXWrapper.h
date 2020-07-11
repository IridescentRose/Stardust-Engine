#pragma once
#include <Platform/Platform.h>

/**
 * Define your graphics related includes and whatnot here.
 */
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

#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
#include <Platform/PC/Window.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Internal Projection, View, and Model Matrixd.
 */
glm::mat4 _gfx_proj, _gfx_view, _gfx_model;
#endif

/**
 * A general purpose OpenGL-like Graphics Wrapper for Stardust.
 */
namespace Stardust::GFX {
    

/**
 * This block defines the three main buffers for GPU clearing.
 */
#if CURRENT_PLATFORM == PLATFORM_PSP
#define GFX_COLOR_BUFFER GU_COLOR_BUFFER_BIT
#define GFX_STENCIL_BUFFER GU_STENCIL_BUFFER_BIT
#define GFX_DEPTH_BUFFER GU_DEPTH_BUFFER_BIT
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
#define GFX_COLOR_BUFFER GL_COLOR_BUFFER_BIT
#define GFX_STENCIL_BUFFER GL_STENCIL_BUFFER_BIT
#define GFX_DEPTH_BUFFER GL_DEPTH_BUFFER_BIT
#else
#error No Graphics Clear Buffer Defines
#endif

    /**
     * Clear the current framebuffer.
     * 
     * \param flags - GFX_COLOR_BUFFER      - clears color buffer
     *              - GFX_DEPTH_BUFFER      - clears depth buffer
     *              - GFX_STENCIL_BUFFER    - clears stencil buffer
     */
    inline void gfxClear(int32_t flags){
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGuClear(GU_FAST_CLEAR_BIT | flags);
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        glClear(flags);
#else
#error No Graphics Clear Function
#endif
    }

    /**
     * Set the clear color.
     * 
     * \param r - Color value from 0.0 - 1.0
     * \param g - Color value from 0.0 - 1.0
     * \param b - Color value from 0.0 - 1.0
     * \param a - Color value from 0.0 - 1.0
     */
    inline void gfxClearColor(float r, float g, float b, float a){
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGuClearColor(GU_COLOR(r, g, b, a));
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        glClearColor(r, g, b, a);
#else
#error No Graphics Set Clear Function
#endif
    }

    inline void gfxSetOrtho(float bottom, float top, float left, float right, float znear, float zfar){
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();
        sceGumOrtho(0, 480, 272, 0, -30, 30);

        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();

        sceGumMatrixMode(GU_MODEL);
        sceGumLoadIdentity();
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        _gfx_proj = glm::ortho(bottom, top, left, right, znear, zfar);
#else
#error No Graphics Set Clear Function
#endif
    }

}