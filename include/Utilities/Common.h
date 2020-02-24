#pragma once

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>

#ifdef __cplusplus
extern "C"
{
#endif


    // Exit callback
    int exit_callback(int arg1, int arg2, void* common);

    // Callback thread 
    int CallbackThread(SceSize args, void* argp);

    // Sets up the callback thread and returns its thread id
    int SetupCallbacks(void);

#ifdef __cplusplus
}
#endif