#include "weaponfinder.hpp"

SAMPFUNCS *SF = new SAMPFUNCS();
stFontInfo *font;
bool overlay_toggle = false;

void pprintf(const char *format, ...)
{
	char buf[128];
	va_list args;

	va_start(args, format);

	sprintf_s(buf, 128, PROGRAM_NAME": ");
	vsnprintf(buf + strlen(buf), 128 - strlen(buf), format, args);

	va_end(args);

	SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(0xFF, 0xFF, 0xAFF), buf);
}

static void version()
{
	char buf[1024];

	sprintf_s(buf, 1024,
		PROGRAM_NAME " " PROGRAM_VERSION "\n\n"
		"Copyright (c) 2016 " PROGRAM_AUTHORS "\n"
		"\n"
		"This is free software: you are free to change and redistribute it.\n"
		"This project is released under the MIT license.\n"
		"\n"
		"Report bugs to: " PROGRAM_BUGREPORT_EMAIL "\n"
		"" PROGRAM_NAME " home page: <" PROGRAM_WEBSITE ">\n"
		);

	SF->getSAMP()->getDialog()->ShowDialog(1, 0, "weaponfinder", buf, "Hide", "");
	return;
}

static void usage()
{
	char buf[2048];

	sprintf_s(buf, 2048,
		PROGRAM_NAME "is a modification for detecting weapon objects on\n"
		"various San Andreas: Multiplayer servers.\n\n"
		"\t/weaponfinder toggle\n\t\tenable/disable the overlay\n"
		HELP_OPTION_DESCRIPTION
		VERSION_OPTION_DESCRIPTION
		);

	SF->getSAMP()->getDialog()->ShowDialog(1, 0, "weaponfinder", buf, "Hide", "");
	return;
}

static void toggle_overlay()
{
	overlay_toggle = !overlay_toggle;

	if (overlay_toggle)
		pprintf("{00FF00}Overlay enabled.");
	else
		pprintf("{FF0000}Overlay disabled.");
}

void CALLBACK cmd_weaponfinder(std::string param)
{
	const char *param_str = param.c_str();

	if (param.empty() || !strcmp(param_str, "help"))
		usage();
	else if (!strcmp(param_str, "version"))
		version();
	else if (!strcmp(param_str, "toggle"))
		toggle_overlay();
}

void CALLBACK mainloop()
{
	static bool init = false;
	if (!init)
	{
		if (GAME == nullptr)
			return;
		if (GAME->GetSystemState() != eSystemState::GS_PLAYING_GAME)
			return;
		if (!SF->getSAMP()->IsInitialized())
			return;

		pprintf("weaponfinder " PROGRAM_VERSION " has been loaded, use /weaponfinder for general help.");
		SF->getSAMP()->registerChatCommand("weaponfinder", cmd_weaponfinder);
		SF->getRender()->registerD3DCallback(eDirect3DDeviceMethods::D3DMETHOD_PRESENT, draw_text);

		font = SF->getRender()->CreateNewFont("Tahoma", 12, FCR_BORDER);
		init = true;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
	switch (dwReasonForCall)
	{
		case DLL_PROCESS_ATTACH:
			SF->initPlugin(mainloop, hModule);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
