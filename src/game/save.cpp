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

    $Id: game.cpp 307 2006-12-03 15:20:21Z goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_thing.h>
#include <gh_player.h>
#include <gh_math.h>

void gh_game::save (void)
{
    GH_FUNCTION()

    //
    // We don't save the individual levels yet. Ideally we would move
    // them from the runtime to the saved directory.
    //
    gh_write_file(this, "saved", "your_last_saved_game.gz");
}

void gh_game::auto_save (void)
{
    GH_FUNCTION()

    gh_write_file(this, "saved", "auto_save.gz");
}

void gh_game::auto_save_remove (void)
{
    string name = gh_global::locate("saved/auto_save.gz");

    gh_global::file_unlink(name);
}

//
// Save the current level as a potential bones file.
//
void gh_game::bones_save (void)
{
    GH_FUNCTION()

    if (!player->thing) {
        GH_LOG("No player to save bones for");
        return;
    }

    //
    // Save bones files more often at higher levels.
    //
    if (gh_math::rand100() < player->thing->level->level_no) {
        //
        // No bones for you today 8(
        //
        GH_LOG("No bones are going to be saved for this level");
        return;
    }

    string name =
        tostring(player->thing->level->level_set_name) + "/" +
        tostring(player->thing->level->level_no, 3, '0') + "/" +
        tostring(player->thing->level->which_level) + ".gz";

    gh_write_file(player->thing->level, "bones", name);

    GH_LOG("Bones save " + name);
}

//
// Find and remove all bones files. We do this after we load a bones file.
// Else we get too many of them.
//
void gh_game::bones_remove (void)
{
    GH_FUNCTION()

    const string bones_dir = gh_global::locatedir("bones");
    vector <string> files = gh_global::listdir_recursive(bones_dir);
    vector <string>::iterator f;

    for (f = files.begin(); f != files.end(); f++) {
        gh_global::file_unlink(*f);

        GH_LOG("Bones remove " + *f);
    }
}

//
// Save the current level so it can be reload if the hero reenters this level.
//
void gh_game::runtime_save (void)
{
    GH_FUNCTION()

    string name =
        tostring(player->thing->level->level_set_name) + "/" +
        tostring(player->thing->level->level_no, 3, '0') + "/" +
        tostring(player->thing->level->which_level) + ".gz";

    gh_write_file(player->thing->level, "runtime", name);

    GH_LOG("Runtime save " + name);
}

//
// Find and remove all runtime files. We do this after and before each game.
//
void gh_game::runtime_remove (void)
{
    GH_FUNCTION()

    const string bones_dir = gh_global::locatedir("runtime");
    vector <string> files = gh_global::listdir_recursive(bones_dir);
    vector <string>::iterator f;

    for (f = files.begin(); f != files.end(); f++) {
        gh_global::file_unlink(*f);

        GH_LOG("Runtime remove " + *f);
    }
}
