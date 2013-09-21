/*
    goblinhack.
    Copyright (C) 1999-2010 Neil McGill

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This game is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this game; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Neil McGill

    $Id: main.cpp 1231 2009-07-05 02:21:28Z Goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <signal.h>
#include <unistd.h>
#include <gh_thing.h>
#include <gh_room.h>
#include <gh_zip.h>
#include <gh_surface.h>
#include <gh_audio.h>
#include <gh_music.h>
#include <gh_sdl.h>

#if defined __linux__
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */
#endif

void gh_ctrlc (int arg)
{
    GH_FUNCTION()

    SDL_Quit();

    GH_LOG("Exit(ctrl-c)");

    exit(1);
}

int main (int argc, char *argv[]) 
{
    gh_backtrace_init();

    GH_FUNCTION()

    signal(SIGINT, gh_ctrlc);

    GH_LOG("Redirect stdout/stderr to files");

    //
    // Download the next version of the game. If already downloaded, kick
    // off the .exe to do the install.
    //
    gh_main_installer();

    if (!freopen("stdout.txt", "w", stdout)) {
	GH_LOG("Failed to open stdout.txt");
    }

    if (!freopen("stderr.txt", "w", stderr)) {
	GH_LOG("Failed to open stderr.txt");
    }

    __debugit_flag__on = 1;

    for(int i=1;i<argc;i++) {

	char *arg = argv[i];

	if (*arg != '-') {
	    continue;
	}

	arg++;

	if (*arg == '-') {
	    arg++;
	}

	if ( (strcmp(arg, "f")==0) ||
	     (strcmp(arg, "fullscreen")==0) ) {
	    gh_global::fullscreen = true;
	}

	if ( (strcmp(arg, "w")==0) ||
	     (strcmp(arg, "window")==0) ) {
	    gh_global::fullscreen = false;
	}

	if ( (strcmp(arg, "h")==0)    ||
	     (strcmp(arg, "help")==0) ||
	     (strcmp(arg, "?")==0)) {

	    printf("GoblinHack!, Version " GH_VERSION "\n");
	    printf("\n");
	    printf("  -f, --fulscreen     -- start in fullscreen mode\n");
	    printf("  -w, --window        -- start in windowed mode\n");
	    printf("  -h, --help          -- this useless help screen\n");
	    printf("\n");
	    printf("Go forth friend and enter!\n");
	    printf("\n");

	    exit(1);
	}
    }

redo:
    gh_start_thread();

    SDL_Quit();

    gh_global::done = false;

    if (gh_global::config_changed) {
	gh_global::config_changed = false;
	goto redo;
    }

    exit(0);
}

int gh_main_thread (void *unused)
{
    GH_FUNCTION()

    try {
	gh_game	*game;

	game = new gh_game("game.gz");

	GH_LOG("");
	GH_LOG("*** Starting ***");
	GH_LOG("");

	sdl_loop_init();

	gh_thing::load_all();
	gh_room::load_all();

	for (;;) {

	    GH_LOG("");
	    GH_LOG("*** Starting game loop ***");
	    GH_LOG("");

	    game->loop();

	    GH_LOG("");
	    GH_LOG("*** Finished game loop ***");
	    GH_LOG("");

	    string new_player_name = game->new_player_name;
	    if (new_player_name == "") {
		break;
	    }

	    delete game;

	    if (gh_global::config_changed) {
		GH_LOG("");
		GH_LOG("*** Config changed");
		GH_LOG("");
		break;
	    }

	    GH_LOG("");
	    GH_LOG("*** Loading saved game " + new_player_name + "***");
	    GH_LOG("");

	    game = new gh_game(new_player_name);

	    game->fixup();
	}

	GH_LOG("");
	GH_LOG("*** Exiting ***");
	GH_LOG("");

	sdl_loop_fini();

	//
	// Lets make sure the player does not exist. We just want to
	// save settings here.
	//
	if (game->player) {
	    GH_WARN("Player still exists at game exit?");
	}

	game->player = NULL;

	gh_write_file(game, "", "game.gz");

	delete game;

    } catch (exception &e) {
	GH_LOG("");
	GH_LOG("*** Exiting (error) ***");
	GH_EXIT(e.what());
	GH_LOG("");
    }

    gh_thing::unload_all();
    gh_room::unload_all();
    gh_surface::unload_all();
    gh_audio::unload_all();
    gh_music::unload_all();

    gh_audio::fini();
    gh_music::fini();

    return (0);
}
