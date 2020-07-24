#include <Platform/Graphics.h>

#undef PSP
namespace Stardust::Platform::PSP{
    pspUtilityMsgDialogParams dialog;
	pspUtilityNetconfData networkData;

	void ConfigureDialog(pspUtilityMsgDialogParams* dialog, size_t dialog_size)
	{
		memset(dialog, 0, dialog_size);

		dialog->base.size = dialog_size;
		sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &dialog->base.language); // Prompt language
		sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &dialog->base.buttonSwap); // X/O button swap

		dialog->base.graphicsThread = 0x11;
		dialog->base.accessThread = 0x13;
		dialog->base.fontThread = 0x12;
		dialog->base.soundThread = 0x10;
	}

	void ShowMessage(const char* message)
	{
		ConfigureDialog(&dialog, sizeof(dialog));
		dialog.mode = PSP_UTILITY_MSGDIALOG_MODE_TEXT;
		dialog.options = PSP_UTILITY_MSGDIALOG_OPTION_TEXT;

		strcpy(dialog.message, message);

		sceUtilityMsgDialogInitStart(&dialog);

		for (;;)
		{

			
			Platform::detail::Graphics::beginFrameDialog();

			switch (sceUtilityMsgDialogGetStatus()) {

			case 2:
				sceUtilityMsgDialogUpdate(1);
				break;

			case 3:
				sceUtilityMsgDialogShutdownStart();
				break;

			case 0:
				return;

			}


			Platform::detail::Graphics::endFrameDialog();
		}
	}

	bool ShowNetworkDialog()
	{
		bool done = true;
		int result = -1;

		memset(&networkData, 0, sizeof(pspUtilityNetconfData));
		networkData.base.size = sizeof(networkData);
		sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &networkData.base.language);
		sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &networkData.base.buttonSwap);
		networkData.base.graphicsThread = 17;
		networkData.base.accessThread = 19;
		networkData.base.fontThread = 18;
		networkData.base.soundThread = 16;
		networkData.action = PSP_NETCONF_ACTION_CONNECTAP;

		struct pspUtilityNetconfAdhoc adhocparam;
		memset(&adhocparam, 0, sizeof(adhocparam));
		networkData.adhocparam = &adhocparam;

		sceUtilityNetconfInitStart(&networkData);

		while (done)
		{
			Platform::detail::Graphics::beginFrameDialog();
			sceGuClear(GU_COLOR_BUFFER_BIT);
			switch (sceUtilityNetconfGetStatus())
			{
			case PSP_UTILITY_DIALOG_NONE:
			{
				result = networkData.base.result;
				done = false;
			}
			break;

			case PSP_UTILITY_DIALOG_VISIBLE:
				sceUtilityNetconfUpdate(1);
				break;

			case PSP_UTILITY_DIALOG_QUIT:
				sceUtilityNetconfShutdownStart();
				break;

			case PSP_UTILITY_DIALOG_FINISHED:
				break;

			default:
				break;
			}

			Platform::detail::Graphics::endFrameDialog();
		}

		if (result == 0)
			return true;

		return false;
	}

	int ShowMessageYesNo(const char* message)
	{
		ConfigureDialog(&dialog, sizeof(dialog));
		dialog.mode = PSP_UTILITY_MSGDIALOG_MODE_TEXT;
		dialog.options = PSP_UTILITY_MSGDIALOG_OPTION_TEXT;
		dialog.options |= PSP_UTILITY_MSGDIALOG_OPTION_YESNO_BUTTONS | PSP_UTILITY_MSGDIALOG_OPTION_DEFAULT_NO;

		strcpy(dialog.message, message);

		sceUtilityMsgDialogInitStart(&dialog);

		bool done = true;

		while (done)
		{
			
			Platform::detail::Graphics::beginFrameDialog();;
			sceGuClear(GU_COLOR_BUFFER_BIT);

			switch (sceUtilityMsgDialogGetStatus()) {

			case 2:
				sceUtilityMsgDialogUpdate(1);
				break;

			case 3:
				sceUtilityMsgDialogShutdownStart();
				break;

			case 0:
				done = false;
				break;

			}

			
			Platform::detail::Graphics::endFrameDialog();
		}

		//the return value
		if (dialog.buttonPressed == PSP_UTILITY_MSGDIALOG_RESULT_YES)
			return 1;
		else if (dialog.buttonPressed == PSP_UTILITY_MSGDIALOG_RESULT_NO)
			return -1;
		else
			return 0;
	}

	int ShowOSK(unsigned short* descritpion, unsigned short* outtext, int maxtextinput)
	{
		//osk params
		SceUtilityOskData oskData;
		SceUtilityOskParams oskParams;
		//init osk data
		memset(&oskData, 0, sizeof(SceUtilityOskData));
		oskData.language = PSP_UTILITY_OSK_LANGUAGE_DEFAULT; // Use system default for text input
		oskData.lines = 1;
		oskData.unk_24 = 1;
		oskData.inputtype = PSP_UTILITY_OSK_INPUTTYPE_ALL; // Allow all input types
		oskData.desc = descritpion;
		//oskData.intext = intext[i];
		oskData.outtextlength = maxtextinput;
		oskData.outtextlimit = 32; // Limit input to 32 characters
		oskData.outtext = outtext;

		//init osk dialog
		memset(&oskParams, 0, sizeof(SceUtilityOskParams));
		oskParams.base.size = sizeof(oskParams);
		sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &oskParams.base.language);
		sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &oskParams.base.buttonSwap);
		oskParams.base.graphicsThread = 17;
		oskParams.base.accessThread = 19;
		oskParams.base.fontThread = 18;
		oskParams.base.soundThread = 16;
		oskParams.datacount = 1;
		oskParams.data = &oskData;

		sceUtilityOskInitStart(&oskParams);

		bool done = true;

		while (done)
		{
			
			Platform::detail::Graphics::beginFrameDialog();;
			sceGuClear(GU_COLOR_BUFFER_BIT);

			switch (sceUtilityOskGetStatus())
			{
			case PSP_UTILITY_DIALOG_INIT:
				break;

			case PSP_UTILITY_DIALOG_VISIBLE:
				sceUtilityOskUpdate(1);
				break;

			case PSP_UTILITY_DIALOG_QUIT:
				sceUtilityOskShutdownStart();
				break;

			case PSP_UTILITY_DIALOG_FINISHED:
				break;

			case PSP_UTILITY_DIALOG_NONE:
				done = false;

			default:
				break;
			}

			
			Platform::detail::Graphics::endFrameDialog();
		}

		if (oskData.result == PSP_UTILITY_OSK_RESULT_CANCELLED)
			return -1;

		return 0;
	}

	void ShowMessageError(const char* message, int error)
	{
		ConfigureDialog(&dialog, sizeof(dialog));
		dialog.mode = PSP_UTILITY_MSGDIALOG_MODE_ERROR;
		dialog.options = PSP_UTILITY_MSGDIALOG_OPTION_ERROR;
		dialog.errorValue = error;
		strcpy(dialog.message, message);

		sceUtilityMsgDialogInitStart(&dialog);

		bool done = true;

		while (done)
		{

			
			Platform::detail::Graphics::beginFrameDialog();;
			sceGuClear(GU_COLOR_BUFFER_BIT);

			int test = sceUtilityMsgDialogGetStatus();

			switch (test)
			{

			case 2:
				sceUtilityMsgDialogUpdate(1);
				break;

			case 3:
				sceUtilityMsgDialogShutdownStart();
				break;

			case 0:
				done = false;
				break;

			}

			
			Platform::detail::Graphics::endFrameDialog();
		}
	}
}