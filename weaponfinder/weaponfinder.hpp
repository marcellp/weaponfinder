#pragma once

#define PROGRAM_NAME "weaponfinder"
#define PROGRAM_VERSION "1.0"
#define PROGRAM_AUTHORS "Marcell Pek"
#define PROGRAM_WEBSITE "http://marcellp.github.io"
#define PROGRAM_BUGREPORT_EMAIL "marcellp0+weaponfinder@gmail.com"
#define HELP_OPTION_DESCRIPTION \
  "\t/weaponfinder help\n\t\tdisplay this help and exit\n"
#define VERSION_OPTION_DESCRIPTION \
  "\t/weaponfinder version\n\t\toutput version information and exit\n"

#include <windows.h>
#include <string>
#include <assert.h>
#include <process.h>
#include <time.h>

#include "SFAPI\SAMPFUNCS_API.h"
#include "game_api\game_api.h"

#include "main.hpp"
#include "render.hpp"

typedef struct weapon_node weapon_node;
struct weapon_node
{
	int modelid;
	const char *weapon;
	CVector pos;
	unsigned char rgb[3];
	weapon_node *next;
};