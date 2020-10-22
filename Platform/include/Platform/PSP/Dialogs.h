#include <malloc.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <psputility.h>
#include <string.h>
#undef PSP
namespace Stardust::Platform::PSP{
    auto ConfigureDialog(pspUtilityMsgDialogParams* dialog, size_t dialog_size) -> void;
	auto ShowMessage(const char* message) -> void;
	auto ShowMessageError(const char* message, int error) -> void;
	auto ShowMessageYesNo(const char* message) -> int;
	auto ShowOSK(unsigned short* descritpion, unsigned short* outtext, int maxtextinput) -> int;
	auto ShowNetworkDialog() -> bool;
}
