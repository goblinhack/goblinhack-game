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

    $Id: hiscore.cpp 873 2008-05-17 22:44:38Z Ghack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_player.h>
#include <gh_hiscore.h>
#include <gh_global.h>
#include <gh_zip.h>

gh_hiscores::gh_hiscores (const string name)
{
    GH_FUNCTION()

    this->name = name;

    gh_read_file(this, "levels/" + name + "/hiscores.gz",
                 false /* noisy */,
                 false /* err */);

    if ((int)hiscores.size() > gh_hiscore::max) {
        hiscores.resize(gh_hiscore::max);
    } else {
        while ((int)hiscores.size() < gh_hiscore::max) {
            hiscores.push_back(gh_hiscore("Thy place awaits thee",
                                          0, // score
                                          0, // level_reached
                                          ""));
        }
    }
}

gh_hiscores::~gh_hiscores (void)
{
    GH_FUNCTION()

    if (hiscores.size()) {
        gh_write_file(this, "", "levels/" + name + "/hiscores.gz");
    }
}

void operator>> (gh_stream_in& is, gh_hiscores *me)
{
    GH_FUNCTION()

    me->get_class_container(is, &me->hiscores, "hiscores");
}

void operator<< (gh_stream_out& os, const gh_hiscores *me)
{
    GH_FUNCTION()

    me->put_class_container(os, &me->hiscores, "hiscores");
}

void gh_hiscores::add_new_hiscore (const gh_player *player,
                                   const string name)
{
    GH_FUNCTION()

    vector<gh_hiscore>::iterator h = hiscores.begin();

    while (h != hiscores.end()) {
        if (player->thing->score > h->score) {
            hiscores.insert(h,
                gh_hiscore(
                    name,
                    player->thing->score,
                    player->level_no,
                    player->thing->named() + ", " + player->thing->killed_by));

            hiscores.resize(gh_hiscore::max);

            return;
        }

        h++;
    }
}

bool gh_hiscores::is_new_hiscore (const gh_player *player)
{
    GH_FUNCTION()

    vector<gh_hiscore>::iterator h = hiscores.begin();

    if (player->quit) {
        //
        // Cowardly player.
        //
        return (false);
    }

    if (!player->thing) {
        return (false);
    }

    if (!player->thing->score) {
        //
        // Poor player.
        //
        return (false);
    }

    while (h != hiscores.end()) {
        if (player->thing->score > h->score) {
            return (true);
        }
        h++;
    }

    return (false);
}

bool gh_hiscores::is_new_highest_hiscore (const gh_player *player)
{
    GH_FUNCTION()

    vector<gh_hiscore>::iterator h = hiscores.begin();

    if (!player->thing->score) {
        //
        // Poor player.
        //
        return (false);
    }

    while (h != hiscores.end()) {
        if (player->thing->score > h->score) {
            return (true);
        }
        return (false);
    }
    return (false);
}

const char *gh_hiscores::place_str (const gh_player *player)
{
    GH_FUNCTION()

    const char *which[gh_hiscore::max_displayed] = {
        "First!",
        "Second",
        "Third",
        "Fourth",
        "Fifth",
        "Sixth",
        "Seventh",
        "Eighth",
        "Nineth",
        "Tenth",
    };

    if (!player->thing->score) {
        //
        // Poor player.
        //
        return ("");
    }

    vector<gh_hiscore>::iterator h = hiscores.begin();

    while (h != hiscores.end()) {
        if (player->thing->score > h->score) {
            return (which[h-hiscores.begin()]);
        }
        h++;
    }
    return ("");
}

void operator>> (gh_stream_in& is, gh_hiscore *me)
{
    GH_FUNCTION()

    me->get(is, &me->name, "name");
    me->get(is, &me->score, "score");
    me->get(is, &me->level_reached, "level_reached");
    string def = "";
    me->get_opt(is, &me->killed_by, "killed_by", def);
}

void operator<< (gh_stream_out& os, const gh_hiscore *me)
{
    GH_FUNCTION()

    me->put(os, &me->name, "name");
    me->put(os, &me->score, "score");
    me->put(os, &me->level_reached, "level_reached");
    me->put(os, &me->killed_by, "killed_by");
}
