#pragma once

#define PROGRAM_NAME "weaponfinder"
#define PROGRAM_VERSION "2.0"
#define PROGRAM_AUTHORS "Marcell Pek"
#define PROGRAM_WEBSITE "http://marcellp.net"
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