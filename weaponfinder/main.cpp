#include "weaponfinder.hpp"

SAMPFUNCS *SF = new SAMPFUNCS();
stFontInfo *font;
bool overlay_toggle = false;
bool color_toggle = false;

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
		"\t/weaponfinder color\n\t\tenable/disable the heat map\n"
		"\t/weaponfinder fontsize\n\t\tchange the font size of the overlay\n"
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

static void toggle_color()
{
	color_toggle = !color_toggle;

	if (color_toggle)
		pprintf("{00FF00}Heat map enabled.");
	else
		pprintf("{FF0000}Heat map disabled.");
}

static bool str_to_int(char *str, int *val, int base)
{
	char *tmp;

	errno = 0;
	*val = strtol(str, &tmp, 10);

	if (tmp == str || *tmp != '\0' || ((*val == LONG_MIN || *val == LONG_MAX) && errno == ERANGE))
		return false;
	else
		return true;
}

void CALLBACK cmd_weaponfinder(std::string param)
{
	char *param_str = _strdup(param.c_str());
	char *tokens[3] = { NULL, NULL };
	int font_size = 0;

	tokens[0] = strtok(param_str, " ");

	if (param.empty() || !strcmpi(param_str, "help"))
		usage();
	else if (!_strcmpi(param_str, "version"))
		version();
	else if (!_strcmpi(param_str, "toggle") || !_strcmpi(param_str, "tog"))
		toggle_overlay();
	else if (!_strcmpi(param_str, "color"))
		toggle_color();
	else if (!_strcmpi(param_str, "fontsize")) {
		tokens[1] = strtok(NULL, "\0\n");
		if (tokens[1] == NULL || !str_to_int(tokens[1], &font_size, 10))
			pprintf("/weaponfinder fontsize [1-30]");
		else {
			change_font_size(font_size);
		}
	}
}

void CALLBACK mainloop()
{
	static bool init = false;

	if (!init) {
		if (GAME == nullptr)
			return;
		if (GAME->GetSystemState() != eSystemState::GS_PLAYING_GAME)
			return;
		if (!SF->getSAMP()->IsInitialized())
			return;

		pprintf("weaponfinder " PROGRAM_VERSION " has been loaded, use /weaponfinder for general help.");
		SF->getSAMP()->registerChatCommand("weaponfinder", cmd_weaponfinder);
		SF->getSAMP()->registerChatCommand("wf", cmd_weaponfinder);
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
