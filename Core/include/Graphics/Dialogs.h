#include <Graphics/RendererCore.h>

#include <malloc.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <psputility.h>
#include <string.h>

namespace Stardust::Graphics{
    void ConfigureDialog(pspUtilityMsgDialogParams* dialog, size_t dialog_size);

	void ShowMessage(const char* message);
	void ShowMessageError(const char* message, int error);
	int ShowMessageYesNo(const char* message);
	int ShowOSK(unsigned short* descritpion, unsigned short* outtext, int maxtextinput);
	bool ShowNetworkDialog();
}