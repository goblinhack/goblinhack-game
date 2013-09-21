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

    $Id: level.cpp 279 2006-11-24 21:49:26Z goblinhack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_player.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_map.h>
#include <gh_global.h>
#include <gh_templates.h>
#include <gh_maze.h>
#include <gh_zip.h>
#include <gh_math.h>
#include <gh_room.h>
#include <gh_fractal.h>
#include <gh_term.h>
#include <gh_ptr.h>
#include <gh_level_map.h>
#include <gh_thing_registration.h>

extern gh_level *gh_level_current;

gh_level::gh_level (class gh_game *game,
                    const int level_no,
                    const string which_level,
                    const string level_set_name,
                    const string level_name,
                    bool want_empty_level)
{
    GH_FUNCTION()

    GH_LOG("");
    GH_LOG("*** New level " +
           level_set_name + "/" +
           tostring(level_no, 3, '0') + "/" +
           which_level +
           " ***");
    GH_LOG("");

    self                        = this;
    gh_level_current            = this;
    valid                       = false;
    tick_at                     = 0;
    time_left                   = 0;
    this->level_no              = level_no;
    this->which_level           = which_level;
    this->level_set_name        = level_set_name;
    draw_start                  = gh_point3d(-1,-1,-1);
    draw_end                    = gh_point3d(-1,-1,-1);
    hero_start_where            = gh_point3d(-1,-1,-1);
    hero_end_where              = gh_point3d(-1,-1,-1);
    this->game                  = game;
    level_completed_bonus       = 10 * level_no;
    is_level_completed          = false;
    is_last_level               = false;
    is_dungeon                  = false;
    is_cave                     = false;
    is_forest                   = false;
    is_landscape                = false;
    is_void                     = false;
    is_hell                     = false;
    is_bonus                    = false;
    scare_monsts                = false;
    freeze_things               = false;
    has_shop                    = false;
    being_deleted               = false;
    first_enter                 = true;

    is_editing                  = false;

    memset(is_editing_which_thing, 0, sizeof(is_editing_which_thing));

    //
    // Start editing with rock.
    //
    is_editing_map_char         = 'x';
    is_editing_grid_res         = GH_RES;

    cursor                      = NULL;
    cursor_at                   = gh_point3d(0,0,0);

    corridor_name               = "";
    corridor_wall_name          = "";

    map.res = GH_RES;

    //
    // If this is an existing saved level, load it.
    //
    if (gh_global::exists(level_name)) {
        gh_read_file(this, level_name, false, false);
    }

    //
    // If the game is null, we're loading a saved game
    //
    if (game != NULL) {
        if (want_empty_level) {
            //
            // Reload the map chars so the level editor works.
            //
            load_maps();
        } else {
            //
            // Load a random or fixed level from char maps or a
            // pre-generated level.
            //
            load(level_name);
        }
    }

    //
    // We use this level creation time to allow the player a bit of
    // time before the monsters move.
    //
    created_when                = gh_get_time_milli();
    valid                       = true;
}

void gh_level::load_thing_from_map (gh_maze *m,
                                    gh_room *r,
                                    gh_point3d where, string thing_name)
{
    GH_FUNCTION()

    //
    // Ignore things we don't understand.
    //
    if (thing_name == "") {
        return;
    }

    int a_cluster_amount = 0;
    int a_cluster_radius = 0;
    gh_room *a_cluster_room = NULL;
    gh_point3d a_cluster_at(0,0,0);
    bool done_cluster = false;

    do {
        int cnt = 0;

        while (cnt < 200) {
            gh_thing *new_thing;

            if (string_contains(thing_name, "hero/")) {
                new_thing = game->player->thing;

                if (new_thing == NULL) {
                    game->player->thing = new_thing =
                                new gh_thing(this,
                                             thing_name,
                                             where,
                                             NULL, /* template */
                                             game->player);

                    //
                    // The player holds onto a ref so the this can't totally
                    // destroy it without telling us.
                    //
                    game->player->thing->incref(GH_THING_REF_PLAYER);
                } else {
                    //
                    // We need to reset some settings since the player
                    // was removed from the last level.
                    //
                    new_thing->at = where;
                    new_thing->born = where;
                    new_thing->level = this;
                    new_thing->incref(GH_THING_REF_FIRST);
                }
            } else {
                new_thing = new gh_thing(this, thing_name, where);
            }

            GH_THING_VERIFY(new_thing);

            //
            // Associate the thing with a room, if known.
            //
            if (!new_thing->is_wall) {
                new_thing->room = r;
            } else {
                new_thing->room = NULL;
            }

            //
            // If this is a new room, record it for this level.
            //
            if (r) {
                if (!contains(&rooms, r)) {
                    rooms.push_back(r);
                }
            }

            //
            // Start out with exits asleep so we can't leave too soon.
            //
            // Don't do this in the common thing code as summoned exits
            // want to be awake from the getgo.
            //
            if (new_thing->is_exit) {
                new_thing->sleeping_boost_start(10);
            }

            if (new_thing->is_return_exit) {
                new_thing->sleeping_boost_start(120);
            }

            //
            // Shuffle things around a bit, if allowed.
            //
            if (new_thing->is_movable) {
                if (r && r->is_shop) {
                    //
                    // Shop items stay where the shopkeeper placed them
                    //
                } else {
                    //
                    // If this object is in a cluster then create it from the
                    // same location.
                    //
                    if (a_cluster_amount && a_cluster_radius) {
                        new_thing->at = a_cluster_at;

                        new_thing->at.x += rand() % ((int)a_cluster_radius*2);
                        new_thing->at.y += rand() % ((int)a_cluster_radius*2);
                        new_thing->at.x -= rand() % ((int)a_cluster_radius*2);
                        new_thing->at.y -= rand() % ((int)a_cluster_radius*2);
                    } else {
                        new_thing->at.x += rand() % (map.res/2);
                        new_thing->at.y += rand() % (map.res/2);
                        new_thing->at.x -= rand() % (map.res/2);
                        new_thing->at.y -= rand() % (map.res/2);
                    }

                    new_thing->born = new_thing->at;
                }
            }

            GH_THING_VERIFY(new_thing);

            if (!new_thing->isref(GH_THING_REF_FIRST)) {
                GH_THROW("no first ref?");
            }

            if (!map.push(new_thing)) {
                new_thing->leaves_corpse = false;
                new_thing->dead("can't push thing onto map",
                                false /* explode */,
                                false /* drop items */);
                return;
            }

            new_thing->decref(GH_THING_REF_FIRST);

            GH_THING_VERIFY(new_thing);

            //
            // Shopkeepers MUST exist in their own shop. Else we hit
            // problems trying to associate their things.
            //
            if (new_thing->is_shopkeeper) {
                return;
            }

redo:
            bool collision = false;

            if (new_thing->is_rock ||
                new_thing->is_dirt ||
                new_thing->is_floor ||
                new_thing->is_door ||
                new_thing->is_corridor ||
                new_thing->is_wall) {
                //
                // Rocks and walls don't hit anything. They just are. Work
                // around them!
                //
                collision = false;
            } else {
                //
                // Other objects, check to see if we overlap.
                // 
                collision = new_thing->hit_obstacle(false /* ignore_hero */);
            }

            if (new_thing->is_hero || new_thing->is_monst) {
                collision |= new_thing->touching_anything();
            }

            //
            // Check that we can exist in this space on the map.
            // Also check there's room for all the things followers.
            //
            if (!collision && new_thing->monst_create_followers()) {
                //
                // Does this thing appear in clusters?
                //
                if (!done_cluster && !a_cluster_amount &&
                    new_thing->cluster_amount) {
                    //
                    // Add a bit of standard deviation to the amount in the
                    // cluster.
                    //
                    a_cluster_amount = (int)
                        (new_thing->cluster_amount / 2) +
                        (rand() % new_thing->cluster_amount);
                                                
                    a_cluster_radius = new_thing->cluster_radius;
                    a_cluster_at = new_thing->at;
                    done_cluster = true;
                }

                //
                // If this cluster began in a room, then make sure that
                // the rest of the things are in the same room. This
                // prevents leakage into shops or caves.
                //
                if (!new_thing->cluster_anywhere) {
                    if (!a_cluster_room) {
                        a_cluster_room = m->rooms[new_thing->at.x / GH_RES]
                                                 [new_thing->at.y / GH_RES];
                    }

                    if (a_cluster_room !=
                        m->rooms[new_thing->at.x / GH_RES]
                                [new_thing->at.y / GH_RES]) {

                        //
                        // Failed to place this thing in the same room as
                        // the start of the cluster.
                        //
                        new_thing->dead("failed to place in room");
                        new_thing = NULL;

                        //
                        // Try again.
                        //
                        cnt++;

                        continue;
                    }
                }

                break;
            }

            if ((++cnt < 200) && new_thing->is_movable) {
                gh_point3d delta(0,0,0); 

                delta.x += rand() % (map.res/4);
                delta.y += rand() % (map.res/4);
                delta.x -= rand() % (map.res/4);
                delta.y -= rand() % (map.res/4);

                new_thing->level->map.move(new_thing, delta);

                goto redo;
            }

            //
            // Failed to place this thing due to obstacles.
            //
            new_thing->dead("failed to place");
            new_thing = NULL;

            break;
        }
    } while (a_cluster_amount-- > 0);
}

void gh_level::load_random_level (void)
{
    GH_FUNCTION()

    unsigned start = (unsigned)time(NULL);
    int maze_number;

    srand(start);
    maze_number = rand();

    GH_LOG(" Random Maze number " + tostring(maze_number));

    gh_maze *m;
    m = new gh_maze(level_no, maze_number);

    if (is_dungeon) {
        m->maze_gen();

        place_water(m);
    } else if (is_cave) {
        m->cave_gen();

        place_water(m);
        place_water(m);
        place_water(m);
    } else if (is_forest) {
        m->forest_gen();
    } else if (is_landscape) {
        m->landscape_gen();
    } else if (is_hell) {
        m->hell_gen();
    } else if (is_bonus) {
        time_left = (rand() % 20) + 20;
        m->bonus_gen();
    } else if (is_void) {
        m->void_gen();
    } else {
        GH_THROW("unset level type");
    }

    //
    // First place all the things that can't move.
    //
    for (int y = 0; y < m->maze_h; y++) {
        for (int x = 0; x < m->maze_w; x++) {
            gh_point3d where(map.res * x, map.res * y, 0);
            gh_room *r = m->rooms[x][y];
            char c;

            where.x += map.res/2;
            where.y += map.res/2;

            c = m->map[x][y];

            string chosen_thing = map_char_to_random_thing(r, level_no, c);

            if (r) {
                //
                // Floor tiles are all the same for any one room.
                //
                if (c == GH_MAP_ROOM) {
                    if (r->floor_name == "") {
                        r->floor_name = chosen_thing;
                    } else {
                        chosen_thing = r->floor_name;
                    }
                }

                //
                // Wall tiles are all the same for any one room.
                //
                if (c == GH_MAP_ROOM_WALL) {
                    if (r->wall_name == "") {
                        r->wall_name = chosen_thing;
                    } else {
                        chosen_thing = r->wall_name;
                    }
                }
            }

            //
            // Wall tiles are all the same for any one level.
            //
            if (c == GH_MAP_CORR) {
                if (corridor_name == "") {
                    corridor_name = chosen_thing;
                } else {
                    chosen_thing = corridor_name;
                }
            }

            //
            // Wall tiles are all the same for any one level.
            //
            if (c == GH_MAP_CORR_WALL) {
                if (corridor_wall_name == "") {
                    corridor_wall_name = chosen_thing;
                } else {
                    chosen_thing = corridor_wall_name;
                }
            }

            load_thing_from_map(m, r, where, chosen_thing);
        }
    }

    //
    // First place all the things that can't move.
    //
    for (int y = 0; y < m->maze_h; y++) {
        for (int x = 0; x < m->maze_w; x++) {
            gh_point3d where(map.res * x, map.res * y, 0);
            gh_room *r = m->rooms[x][y];
            char c;

            where.x += map.res/2;
            where.y += map.res/2;

            c = m->map_obstacle[x][y];
            if (!c) {
                continue;
            }
            load_thing_from_map(m, r, where,
                                map_char_to_random_thing(r, level_no, c));
        }
    }

    //
    // Then dump the movable stuff on top.
    //
    // Doors first.
    //
    for (int y = 0; y < m->maze_h; y++) {
        for (int x = 0; x < m->maze_w; x++) {
            char c;

            c = m->map_obj[x][y];
            if (!c) {
                continue;
            }

            if (c != GH_MAP_DOOR) {
                continue;
            }

            gh_point3d where(map.res * x, map.res * y, 0);
            where.x += map.res/2;
            where.y += map.res/2;

            gh_room *r = m->rooms[x][y];
            string chosen_thing = map_char_to_random_thing(r, level_no, c);

            //
            // Door types are all the same for any one room.
            //
            if (r) {
                if (c == GH_MAP_DOOR) {
                    if (r->door_name == "") {
                        r->door_name = chosen_thing;
                    } else {
                        chosen_thing = r->door_name;
                    }
                }
            }

            load_thing_from_map(m, r, where, chosen_thing);
        }
    }

    //
    // Then other 'things'.
    //
    for (int y = 0; y < m->maze_h; y++) {
        for (int x = 0; x < m->maze_w; x++) {
            char c;

            c = m->map_obj[x][y];
            if (!c) {
                continue;
            }

            if (c == GH_MAP_DOOR) {
                continue;
            }
            
            if (c == GH_MAP_MONST) {
                continue;
            }

            gh_point3d where(map.res * x, map.res * y, 0);
            where.x += map.res/2;
            where.y += map.res/2;

            gh_room *r = m->rooms[x][y];
            string chosen_thing = map_char_to_random_thing(r, level_no, c);

            load_thing_from_map(m, r, where, chosen_thing);
        }
    }

    //
    // Then monsters and other things.
    //
    for (int y = 0; y < m->maze_h; y++) {
        for (int x = 0; x < m->maze_w; x++) {
            char c;

            c = m->map_obj[x][y];
            if (c != GH_MAP_MONST) {
                continue;
            }

            gh_point3d where(map.res * x, map.res * y, 0);
            where.x += map.res/2;
            where.y += map.res/2;

            gh_room *r = m->rooms[x][y];
            string chosen_thing = map_char_to_random_thing(r, level_no, c);

            load_thing_from_map(m, r, where, chosen_thing);
        }
    }

    map.update();

    delete m;
}

void gh_level::place_water (gh_maze *m)
{
    GH_FUNCTION()

    const int water_width = 3; // 3 per cell
    const int width = m->maze_w * water_width;
    const int height = m->maze_h * water_width;
    const float pix_width = (float)map.res / (float)water_width;

    gh_fractal * f = new gh_fractal(
                            width,
                            height,
                            100, // max starting height
                            rand(),
                            10, // stdev
                            0.8);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {

            char c = m->map[x / water_width][y / water_width];
        
            gh_room *r = m->rooms[x / water_width][y / water_width];

            if (r) {
                //
                // No water in special rooms.
                //
                continue;
            }

            switch (c) {
            case GH_MAP_START:
                //
                // No water in rooms - got to give the player a chance
                //
                continue;

            case GH_MAP_EXIT:
                //
                // No water in rooms - got to give the player a chance
                //
                continue;

            default:
                //
                // Walls and rock - skip - we just get too many things.
                //
                continue;

            case GH_MAP_ROOM:
            case GH_MAP_CORR:
                //
                // A corridor or wall; add water.
                //
                float depth = (*f->map)[x][y];

                gh_thing *new_thing;

                gh_point3d where(
                             (int)((pix_width * (float)x) + (pix_width / 2)),
                             (int)((pix_width * (float)y) + (pix_width / 2)),
                             0);

                if (depth > 110) {
                    new_thing = new gh_thing(this, "room/water/3", where);
                } else if (depth > 100) {
                    //
                    // Shift around so it looks more random.
                    //
                    where.x += gh_math::rand10();
                    where.y += gh_math::rand10();
                    where.x -= gh_math::rand10();
                    where.y -= gh_math::rand10();

                    new_thing = new gh_thing(this, "room/water/2", where);
                } else if (depth > 90) {
                    //
                    // Shift around so it looks more random.
                    //
                    where.x += gh_math::rand10();
                    where.y += gh_math::rand10();
                    where.x -= gh_math::rand10();
                    where.y -= gh_math::rand10();

                    new_thing = new gh_thing(this, "room/water/1", where);
                } else {
                    //
                    // not deep enough
                    //
                    continue;
                }

                map.push(new_thing);

                new_thing->decref(GH_THING_REF_FIRST);
            }
        }
    }

    delete f;
}

void gh_level::load_pregenerated_level (string name)
{
    GH_FUNCTION()

    GH_LOG(" Pregenerated level " + name);

    //
    // Here we're loading a predefined level.
    //
    gh_read_file(this, name);

    if (game->player->thing == NULL) {
        if (game->new_player_class == "") {
            GH_THROW("no player class selected?");
        }

        game->player->thing = new gh_thing(this,
                                           game->new_player_class,
                                           hero_start_where,
                                           NULL, /* template */
                                           game->player);

        //
        // The player holds onto a ref so the this can't totally
        // destroy it without telling us.
        //
        game->player->thing->incref(GH_THING_REF_PLAYER);
    } else {
        //
        // We need to reset some settings since the player
        // was removed from the last level.
        //
        game->player->thing->level = this;
        game->player->thing->incref(GH_THING_REF_FIRST);
    }

    if (hero_end_where == gh_point3d(-1,-1,-1)) {
        game->player->thing->at = hero_start_where;
        game->player->thing->born = hero_start_where;
    } else {
        //
        // Back at a return exit?
        //
        game->player->thing->at = hero_end_where;
        game->player->thing->born = hero_end_where;
    }

    map.push(game->player->thing);

    //
    // We better be on the map!
    //
    game->player->thing->decref(GH_THING_REF_FIRST);

    //
    // Unflatten saved pointers.
    //
    fixup();

    //
    // Create shops if needed.
    //
    fixup_dynamic_room_for_shops();

    //
    // Set a level type if none is specified.
    //
    if (!is_dungeon &&
        !is_cave &&
        !is_forest &&
        !is_landscape &&
        !is_void &&
        !is_hell &&
        !is_bonus) {
        is_dungeon = true;
    }
}

//
// Load a random or pregenerated level; load the pregenerated ones randomly
// also.
//
void gh_level::load (string level_name)
{
    GH_FUNCTION()

    if (game->want_to_enter_level_editor_mode) {
        if (gh_global::exists(level_name)) {
            load_pregenerated_level(level_name);
            return;
        }
    }

    //
    // Is there a specific bones level we can load?
    //
    string bones = "bones/" + level_name;

    GH_LOG("Look for a bones level " + bones);
    if (gh_global::exists(bones)) {
        GH_LOG(" Got one! ");
        load_pregenerated_level(bones);
        game->bones_remove();
        return;
    }

    //
    // Is there a specific runtime level we can load?
    //
    string runtime = "runtime/" + level_name;

    GH_LOG("Look for a currently active level " + runtime);
    if (gh_global::exists(runtime)) {
        GH_LOG(" Got one! ");
        load_pregenerated_level(runtime);
        return;
    }

    GH_LOG("None found, generate a level");

    //
    // Reload the map chars so the level editor works.
    //
    load_maps();

    //
    // See what type of level we want to play here.
    //
    string which_level = game->level_maps->choose(level_no);

    is_dungeon = false;
    is_cave = false;
    is_forest = false;
    is_landscape = false;
    is_void = false;
    is_hell = false;
    is_bonus = false;

    if (which_level == "dungeon") {
        is_dungeon = true;
    } else if (which_level == "cave") {
        is_cave = true;
    } else if (which_level == "forest") {
        is_forest = true;
    } else if (which_level == "landscape") {
        is_landscape = true;
    } else if (which_level == "hell") {
        is_hell = true;
    } else if (which_level == "bonus") {
        is_bonus = true;
    } else if (which_level == "void") {
        is_void = true;
    } else if (which_level == "fixed") {
        is_dungeon = true;

        string dir = level_name;
        string_rremove_to_end(dir, '/');

        vector<string> listing =
            gh_global::list_files_only(gh_global::locatedir(dir));

        if (listing.size()) {
            which_level = one_of(listing);
            level_name = dir + "/" + which_level;

            if (which_level != "dummy_level") {
                load_pregenerated_level(level_name);
                return;
            }
        }
    } else {
        level_name = "levels/" + which_level;

        if (gh_global::exists(level_name)) {
            load_pregenerated_level(level_name);
            return;
        }

        GH_WARN("level " + level_name + " not found");

        is_dungeon = true;
    }

    //
    // Load a random 'didn't choose any' level.
    //
    which_level = "dummy";

    load_random_level();
}

void gh_level::fixup (void)
{
    GH_FUNCTION()

    GH_LOG("");
    GH_LOG("*** Level fixup *** ");
    GH_LOG("");

    //
    // Search the saved things for the hero thing
    //
    gh_thing *tmp_thing = game->player->thing;
    if (!tmp_thing) {
        GH_THROW("no player thing to fixup");
    }

    list<gh_thing*>::iterator it;
    list<gh_room*>::iterator ir;

    bool got_hero = true;

    for (it = all_things.begin(); it != all_things.end(); it++) {
        gh_thing *t = *it;

        //
        // If we loaded a hero from the saved file, we need to associate
        // it with the player.
        //
        // If this was a predefined level, then we have a hero already.
        //
        if (t->is_hero) {
            if (t->needs_fixup) {
                //
                // Replace the player linkeage
                //
                game->player->thing = t;
                game->player->thing->incref(GH_THING_REF_PLAYER);
                t->player = game->player;

                //
                // Destroy the old fake hero
                //
                tmp_thing->level = NULL;
                tmp_thing = NULL;
                got_hero = true;
            } else {
                //
                // This was a predefined level. No need of this tmp hero.
                //
                tmp_thing = NULL;
                got_hero = true;
            }

            break;
        }
    }

    if (!got_hero) {
        //
        // If loading a pre generated level, we want to push the player
        // onto the start point. This check below is only for game recovery
        // and saved games.
        //
        GH_THROW("no hero on the save game?");
    }

    GH_LOG("fixup: found hero");

    //
    // The things were loaded onto the all thing list earlier, but
    // they have no valid iterator. Remove and read them to get this
    // set now.
    //
    for (it = all_things.begin(); it != all_things.end(); it++) {
        gh_thing *t = *it;

        if (!t->needs_fixup) {
            //
            // Already fixed up.
            //
            continue;
        }

        GH_THING_LOG(t, " fixup");

        t->level = this;

        //
        // We're already in the all_things list, but don't know where.
        //
        t->all_things_it = it;
        t->all_things_it_valid = true;
        t->incref(GH_THING_REF_ALL_THINGS);

        //
        // Restore the thing to some of its lists
        //
        if (t->is_on_map) {
            t->is_on_map = false;
            map.push(t);
        }

        if (t->is_dead) {
            t->add_dead_things();
        }

        if (t->does_stuff) {
            t->add_does_stuff_things();
        }

        t->reg = gh_thing_registration_get(t->short_name);
        if (!t->reg) {
            t->reg = gh_thing_registration_get("default");
            if (!t->reg) {
                t->debug();
                GH_THROW("no callbacks found for " + t->named() +
                         " or default thing");
            }
        }
    }

    GH_LOG("fixup: set things onto level");

    //
    // Save all the old pointers and associate them with their new owners.
    //
    // First things
    //
    for (it = all_things.begin(); it != all_things.end(); it++) {
        gh_thing *t = *it;

        add_ptr(/* old */ t->self, /* new */ t);
        t->self = t;

        GH_THING_LOG(t, " Recovered, "
                     " old ptr " + tostring((long)t->self) +
                     " new ptr " + tostring((long)t));
    }

    GH_LOG("fixup: saved thing pointers");

    //
    // Then rooms
    //
    for (ir = rooms.begin(); ir != rooms.end(); ir++) {
        gh_room *r = *ir;

        add_ptr(/* old */ r->self, /* new */ r);
        r->self = r;
    }

    GH_LOG("fixup: saved room pointers");

    //
    // Then the level
    //
    add_ptr(/* old */ self, /* new */ this);
    self = this;

    GH_LOG("fixup: saved level pointers");

    //
    // Restore thing pointers
    //
    for (it = all_things.begin(); it != all_things.end(); it++) {
        gh_thing *t = *it;

        if (!t->needs_fixup) {
            //
            // Already fixed up.
            //
            continue;
        }

        if (t->weapon) {
            t->weapon = (class gh_thing *) get_ptr(t->weapon, "weapon");
            t->weapon->incref(GH_THING_REF_WEAPON);
        }

        if (t->room) {
            t->room = (class gh_room *) get_ptr(t->room, "thing room");
        }

        if (t->owner) {
            t->owner = (class gh_thing *) get_ptr(t->owner, "owner");
            t->owner->incref(GH_THING_REF_OWNER);
        }

        if (t->missile_thrower) {
            t->missile_thrower =
                (class gh_thing *) get_ptr(t->missile_thrower, "missile_thrower");
            t->missile_thrower->incref(GH_THING_REF_MISSILE_THROWER);
        }

        if (t->weapon_wielder) {
            t->weapon_wielder =
                (class gh_thing *) get_ptr(t->weapon_wielder, "weapon_wielder");
            t->weapon_wielder->incref(GH_THING_REF_WEAPON_WIELDER);
        }

        if (t->next) {
            t->next = (class gh_thing *) get_ptr(t->next, "next");
        }

        if (t->prev) {
            t->prev = (class gh_thing *) get_ptr(t->prev, "prev");
        }

        if (t->limb_next) {
            t->limb_next = (class gh_thing *) get_ptr(t->limb_next, "limb_next");
        }

        if (t->limb_prev) {
            t->limb_prev = (class gh_thing *) get_ptr(t->limb_prev, "limb_prev");
        }

        //
        // Repair carried things
        //
        list<gh_thing*>::iterator ic;
        list<gh_thing*> carrying;

        for (ic = t->carrying.begin(); ic != t->carrying.end(); ic++) {

            gh_thing *i = (class gh_thing *) get_ptr(*ic, "item");

            if (!i->carrier) {
                GH_WARN("thing " + i->name + " is not being carried by " + t->name + " who thinks it is");
            } else {
                //
                // I'm being carried now.
                //
                i->incref(GH_THING_REF_IM_BEING_CARRIED);
                i->carrier = (class gh_thing *) get_ptr(i->carrier, "carrier");

                carrying.push_back(i);
            }
        }

        erase(t->carrying);
        t->carrying = carrying;

        //
        // Repair visited rooms
        //
        list<gh_room*> visited;

        for (ir = t->visited.begin(); ir != t->visited.end(); ir++) {

            gh_room *r = (class gh_room *) get_ptr(*ir, "visited room");
            visited.push_back(r);
        }

        erase(t->visited);
        t->visited = visited;
    }

    GH_LOG("fixup: restored thing pointers");

    //
    // Restore room pointers
    //
    for (ir = rooms.begin(); ir != rooms.end(); ir++) {
        gh_room *r = *ir;

        if (r->shopkeeper) {
            r->shopkeeper = (class gh_thing *)
                            get_ptr(r->shopkeeper, "shopkeeper");
            r->shopkeeper->incref(GH_THING_REF_SHOPKEEPER);
        }
    }

    GH_LOG("fixup: restored room pointers");

    //
    // Update weapons being wielded to use the correct graphics.
    //
    for (it = all_things.begin(); it != all_things.end(); it++) {
        gh_thing *t = *it;

        //
        // Check the linkage, of things with body parts, is sane.
        //
        if (t->next) {
            gh_thing *n = t;

            while (n) {
                if (n->next) {
                    if (n->next->prev != n) {
                        GH_THING_LOG(n, " n");
                        GH_THING_LOG(n->next, " n->next");
                        GH_THING_LOG(n->next->prev, " n->next->prev");
                        GH_THROW("next != this");
                    }
                }

                if (n->prev) {
                    if (n->prev->next != n) {
                        GH_THING_LOG(n, " n");
                        GH_THING_LOG(n->prev, " n->prev");
                        GH_THING_LOG(n->prev->next, " n->prev->next");
                        GH_THROW("prev != this");
                    }
                }

                n = n->next;
            }
        }

        //
        // Check the linkage, of things with body parts, is sane.
        //
        if (t->limb_next) {
            gh_thing *n = t;

            while (n) {
                if (n->limb_next) {
                    if (n->limb_next->limb_prev != n) {
                        GH_THING_LOG(n, " n");
                        GH_THING_LOG(n->limb_next, " n->limb_next");
                        GH_THING_LOG(n->limb_next->limb_prev, " n->limb_next->limb_prev");
                        GH_THROW("limb_next != this");
                    }
                }

                if (n->limb_prev) {
                    if (n->limb_prev->limb_next != n) {
                        GH_THING_LOG(n, " n");
                        GH_THING_LOG(n->limb_prev, " n->limb_prev");
                        GH_THING_LOG(n->limb_prev->limb_next, " n->limb_prev->limb_next");
                        GH_THROW("limb_prev != this");
                    }
                }

                n = n->limb_next;
            }
        }

        if (!t->needs_fixup) {
            //
            // Already fixed up.
            //
            continue;
        }

        //
        // Fixup is done.
        //
        t->needs_fixup = false;

        if (t->weapon) {
            t->rewield();
        }

        t->update_anim();

        //
        // Repair carried things
        //
        list<gh_thing*>::iterator ic;

        for (ic = t->carrying.begin(); ic != t->carrying.end(); ic++) {

            gh_thing *i = *ic;

            i->update_anim();
        }

        t->decref(GH_THING_REF_FIRST);
    }

    GH_LOG("fixup: restored thing graphics");

    GH_LOG("");
    GH_LOG("*** Load fixup done *** ");
    GH_LOG("");

    //
    // If this was recovering an auto saved file, remove it now.
    //
    game->auto_save_remove();

    //
    // Reload the map chars so the level editor works.
    //
    load_maps();

    for (it = all_things.begin(); it != all_things.end(); it++) {
        gh_thing *t = *it;

        GH_THING_LOG(t, "Recovered");

        //
        // Start out with exits asleep so we can't leave too soon.
        //
        // Don't do this in the common thing code as summoned exits
        // want to be awake from the getgo.
        //
        if (t->is_exit) {
            t->sleeping_boost_start(10);
        }

        if (t->is_return_exit) {
            t->sleeping_boost_start(120);
        }

        //
        // Ensure multiple torsos are not seen.
        //
        if (t->limb_prev) {
            t->anim_frames = NULL;
        }
    }

redo:
    //
    // The level editor seems to make duplicate things. Work around for now.
    //
    for (it = all_things.begin(); it != all_things.end(); it++) {
        gh_thing *t = *it;
        gh_thing *d = t->level->map.find_first_thing_same_at(t->at, t);

        if (d) {
            GH_THING_VERIFY(d);

            d->incref(GH_THING_REF_TERMINAL);
            d->level->map.pop(d);
            d->debug();
            d->dead("dup", false /* explode */, false /* drop_things */);
            d->rm_all_things();
            d->decref(GH_THING_REF_TERMINAL);
            goto redo;
        }
    }
}

void gh_level::load_maps (void)
{
    GH_FUNCTION()

    load_map("levels/map");

    if (level_set_name.size()) {
        load_map("levels/" + level_set_name + "/map");
    }
}

void gh_level::load_map (const string map_name)
{
    GH_FUNCTION()

    if (!gh_global::exists(map_name)) {
        return;
    }

    GH_LOG("Load character map file " + map_name + " maps");

    string file = gh_global::locate(map_name);

    size_t size;
    char *buf = gh_zip::instance()->gzreadall(file, &size, 32768, false /* binary */);
    char *at;
    char *end;

    if (size == 0) {
        return;
    }

    if (buf == NULL) {
        GH_LOG("  Can't open map file \"" + file + "\"");
        return;
    }

    at = buf;
    end = buf + size;

    char map_char;
    string thing_name;

    while (at < end) {
        //
        // skip comments
        //
        if ((*at == '#') || (*at == '"')) {
            while ((at < end) && (*at != '\n')) {
                at++;
            }

            at++;
            continue;
        }

        //
        // Only expect this format in the map files currently.
        //
        at += sizeof(":map");

        map_char = *at++;
        at++;

        //
        // Skip to the end of the line.
        //
        while ((at < end) && (*at != '\n')) {
            thing_name += *at++;
        }
        at++;

        string log("");
        
        log += map_char;
        
//      GH_LOG("MAP \"" + log + "\" -> " + thing_name);

        map_chars.push_front(gh_map_char(this, map_char, thing_name, -1, -1));

        thing_name = "";
    }

    delete [] buf;
}

string gh_level::map_char_to_thing (const int level_no,
                                    const char map_char,
                                    const int line,
                                    const int column)
{
    GH_FUNCTION()

    list <gh_map_char>::iterator i;

    for (i = map_chars.begin(); i != map_chars.end(); i++) {
        gh_map_char *m = &(*i);

        if (m->map_char == map_char) {
            if (m->line != -1) {
                if (m->line != line) {
                    return (m->thing_name);
                }
            }

            if (m->column <= column) {
                if (m->line == -1) {
                    return map_char_to_random_thing(NULL, level_no, map_char);
                } else {
                    return (m->thing_name);
                }
            }
        }
    }

    string s;
    s += map_char;

    GH_WARN("no mapping for char \'" + s + "\'");

    return ("");
}

string gh_level::map_char_to_random_thing (gh_room *r,
                                           const int level_no,
                                           const char map_char)
{
    GH_FUNCTION()

    list <gh_map_char>::iterator i;
    vector <string> possible;
    int possible_matches = 0;
    gh_map_char *last_possible = NULL;
    string thing_name;
    int cnt = 0;

redo:
    if (cnt++ > 1000) {
        string s;
        s += map_char;

        GH_WARN("failed to find a random mapping for char \'" + s + "\'");
        return ("");
    }

    bool got_one = false;

    //
    // If the user has selected a class for the hero, use that.
    //
    if (map_char == GH_MAP_START) {
        if (game->new_player_class != "") {
            return (game->new_player_class);
        }
    }

    //
    // First try all the room mappings
    //
    if (r) {
        for (i = r->map_chars.begin(); i != r->map_chars.end(); i++) {
            gh_map_char *m = &(*i);

            if (m->map_char != map_char) {
                continue;
            }

            if (r->is_shop) {
                //
                // Wants to be placed in shops only?
                // Shopkeeper?
                //
                if (m->is_found_only_in_shops) {
                    got_one = true;
                    possible.push_back(m->thing_name);
                    continue;
                }

                if (m->not_in_shops) {
                    continue;
                }

                //
                // No monsters in shops!
                //
                if (m->is_monst) {
                    return ("");
                }
            }

            //
            // Some thing like shopkeepers, just don't appear randomly.
            //
            if (!m->appearing_chance) {
                continue;
            }

            got_one = true;

            possible_matches++;
            last_possible = m;

            if (gh_math::rand1000() < m->appearing_chance) {
                possible.push_back(m->thing_name);
                continue;
            }
        }
    }

    //
    // If none, then try the level mappings
    //
    if (!got_one) {
        for (i = map_chars.begin(); i != map_chars.end(); i++) {
            gh_map_char *m = &(*i);

            if (m->map_char != map_char) {
                continue;
            }

            if (r) {
                if (r->is_shop) {
                    //
                    // Wants to be placed in shops only?
                    // Shopkeeper?
                    //
                    if (m->is_found_only_in_shops) {
                        got_one = true;
                        possible.push_back(m->thing_name);
                        continue;
                    }

                    if (m->not_in_shops) {
                        continue;
                    }

                    //
                    // No monsters in shops!
                    //
                    if (m->is_monst) {
                        return ("");
                    }
                }
            }

            //
            // Some things, like shopkeepers, just don't appear randomly.
            //
            if (!m->appearing_chance) {
                continue;
            }

            //
            // Filter monsters for their environment.
            //
            if (m->is_monst) {
                gh_thing *t = gh_thing::find(m->thing_name,
                                             true /* err */);

                if (is_dungeon && !t->only_in_dungeons) {
                    continue;
                }

                if (is_cave && !t->only_in_caves) {
                    continue;
                }

                if (is_forest && !t->only_in_forests) {
                    continue;
                }

                if (is_landscape && !t->only_in_landscapes) {
                    continue;
                }

                if (is_void && !t->loves_void) {
                    continue;
                }

                if (is_hell && !t->only_in_hell) {
                    continue;
                }
            }

            got_one = true;

            possible_matches++;
            last_possible = m;

            if (gh_math::rand1000() < m->appearing_chance) {
                possible.push_back(m->thing_name);
                continue;
            }
        }
    }

    if (!got_one) {
        string s;
        s += map_char;

        GH_ROOM_LOG(r, "no mapping for char \'" + s + "\'");
        return ("");
    }

    if (!possible.size()) {
        //
        // If there's only one match possible, just return that.
        //
        if (last_possible && (possible_matches == 1)) {
            return (last_possible->thing_name);
        }

        //
        // There are more possibilities. Try again.
        //
        goto redo;
    }

    thing_name = (possible[(rand() % possible.size())]);

    return (thing_name);
}

//
// Find the xth thing of the given character filter name.
//
string gh_level::map_char_nth (const char filter, int index)
{
    list<gh_map_char>::iterator i = map_chars.begin();
    int count = 0;

    while (i != map_chars.end()) {
        gh_map_char m = *i;

        if ((m.map_char == filter) || (filter == '\0')) {
            if (count == index) {
                return (m.thing_name);
            }

            count++;
        }

        i++;
    }

    return ("");
}

//
// Given a thing name, work out what filters it belongs and choose one at
// random.
//
string gh_level::map_char_polymorph (const string thing_name)
{
    list<gh_map_char>::iterator i;

    int score = 0;
    int best;
    char best_filter = '\0';
    string best_thing = "";

    //
    // First find the best filter.
    //
    i = map_chars.begin();
    best = -1;
    while (i != map_chars.end()) {
        gh_map_char m = *i;

        if (m.thing_name == thing_name) {
            score = gh_math::rand100();

            if (score > best) {
                best = score;
                best_filter = m.map_char;
            }
        }

        i++;
    }

    if (best_filter == '\0') {
        return ("");
    }

    //
    // First find the best thing in this new filter.
    //
    i = map_chars.begin();
    best = -1;
    while (i != map_chars.end()) {
        gh_map_char m = *i;

        if (m.thing_name != thing_name) {
            if (m.map_char == best_filter) {
                score = gh_math::rand100();

                if (score > best) {
                    best = score;
                    best_thing = m.thing_name;
                }
            }
        }

        i++;
    }

    return (best_thing);
}

//
// Given a thing name, work out what filters it belongs and choose one at
// random.
//
char gh_level::map_char_thing_to_char (const string thing_name)
{
    list<gh_map_char>::iterator i;

    int score = 0;
    int best;
    char best_filter = '\0';
    string best_thing = "";

    //
    // First find the best filter.
    //
    i = map_chars.begin();
    best = -1;
    while (i != map_chars.end()) {
        gh_map_char m = *i;

        if (m.thing_name == thing_name) {
            score = gh_math::rand100();

            if (score > best) {
                best = score;
                best_filter = m.map_char;
            }
        }

        i++;
    }

    return (best_filter);
}

//
// How many characters of the given filter exist?
//
int gh_level::map_char_how_many (const char filter)
{
    list<gh_map_char>::iterator i = map_chars.begin();
    int count = 0;

    while (i != map_chars.end()) {
        gh_map_char m = *i;

        if ((m.map_char == filter) || (filter == '\0')) {
            count++;
        }

        i++;
    }

    return (count);
}

gh_level::~gh_level (void)
{
    GH_FUNCTION()

    GH_LOG("");
    GH_LOG("*** End level " + level_set_name + " " +
           tostring(level_no, 3, '0') + " ***");
    GH_LOG("");

    //
    // Had some strange bug where some of the lists weren't being
    // initialized correctly by the compiler
    //
    if (!valid) {
        GH_THROW("not created");
    }

    if (cursor) {
        map.pop(cursor);
        cursor->decref(GH_THING_REF_CURSOR);
        cursor = NULL;
    }

    //
    // Destroy all rooms and things they were referring (shops/shopkeepers)
    //
    erase_and_delete(rooms);

    //
    // Indicate to things being deleted that there is no point dropping
    // anything onto the level.
    //
    being_deleted = true;

    //
    // Finally any association with this dead level; including say things
    // being carried.
    //
    list<gh_thing*>::iterator c;

    int got = 1;

    while (got) {
        got = 0;

        c = dead_things.begin();
        if (c != dead_things.end()) {
        
            GH_THING_VERIFY(*c);

            GH_THING_LOG(*c, "dead clean up");

            (*c)->rm_dead_things();

            //
            // *c may be invalid.
            //
            got++;
            continue;
        }

        c = does_stuff_things.begin();
        if (c != does_stuff_things.end()) {

            GH_THING_VERIFY(*c);

            GH_THING_LOG(*c, "does stuff clean up");

            //
            // Heros live on beyond the level end.
            //
            if ((*c)->is_dead || (*c)->is_hero ||
                //
                // Monster followers?
                //
                (*c)->isref(GH_THING_REF_IM_BEING_CARRIED)) {
                //
                // Still on the does stuff list? Remove it. Might be
                // a corpse.
                //
                if ((*c)->does_stuff_things_it_valid) {
                    (*c)->rm_does_stuff_things();
                }
            } else {
                //
                // This should add us to the dead list and then remove
                // us from the does stuff list. It cleans up possessions
                // and other thing linkage.
                //
                (*c)->dead("level end", false /* explode */);
            }

            //
            // *c may be invalid.
            //
            got++;
            continue;
        }

        c = all_things.begin();
        if (c != all_things.end()) {
            GH_THING_VERIFY(*c);

            (*c)->rm_all_things();
            //
            // *c may be invalid.
            //
            got++;
            continue;
        }
    }

    //
    // Destroy the map and unhook all things from it.
    //
    // Popping off the map can be the last ref for some things; hence
    // killed above and below to make really sure.
    //
    map.deletemap();

    //
    // Finally any association with this dead level; including say things
    // being carried.
    //
    got = 1;

    while (got) {
        got = 0;

        c = dead_things.begin();
        if (c != dead_things.end()) {
        
            GH_THING_VERIFY(*c);

            GH_THING_LOG(*c, "dead clean up");

            (*c)->rm_dead_things();

            //
            // *c may be invalid.
            //
            got++;
            continue;
        }

        c = does_stuff_things.begin();
        if (c != does_stuff_things.end()) {

            GH_THING_VERIFY(*c);

            GH_THING_LOG(*c, "does stuff clean up");

            //
            // Heros live on beyond the level end.
            //
            if ((*c)->is_dead || (*c)->is_hero ||
                //
                // Monster followers?
                //
                (*c)->isref(GH_THING_REF_IM_BEING_CARRIED)) {
                //
                // Still on the does stuff list? Remove it. Might be
                // a corpse.
                //
                if ((*c)->does_stuff_things_it_valid) {
                    (*c)->rm_does_stuff_things();
                }
            } else {
                //
                // This should add us to the dead list and then remove
                // us from the does stuff list. It cleans up possessions
                // and other thing linkage.
                //
                (*c)->dead("level end", false /* explode */);
            }

            //
            // *c may be invalid.
            //
            got++;
            continue;
        }

        c = all_things.begin();
        if (c != all_things.end()) {
            GH_THING_VERIFY(*c);

            (*c)->rm_all_things();
            //
            // *c may be invalid.
            //
            got++;
            continue;
        }
    }
}

//
// Add a thing to the dead list. We can then kill or resurrect
// it outside of the tick loop.
//
void gh_level::add_dead (gh_thing *t)
{
    GH_FUNCTION()

    if (t->dead_things_it_valid) {
        return;
    }

    t->add_dead_things();
}

//
// Kill any dead things.
//
void gh_level::handle_dead_all (void)
{
    GH_FUNCTION()

    list<gh_thing*>::iterator c;
    gh_thing *t;

redo:
    c = dead_things.begin();
    while (c != dead_things.end()) {
        t = *c;
        c++;

        GH_THING_VERIFY(t);

        if (!t->dead_things_it_valid) {
            t->debug();
            GH_THROW("no dead iterator but on dead list");
        }

        if (t->does_stuff_things_it_valid) {
            //
            // No point being on both lists.
            //
            t->rm_does_stuff_things();
        }

        t->rm_dead_things();

        goto redo;
    }
}

void gh_level::tick_things_all (void)
{
    GH_FUNCTION()

    list<gh_thing*>::iterator t = does_stuff_things.begin();

    tick_at++;

    gh_global::monst_count = 0;

    while (t != does_stuff_things.end()) {
        gh_thing *thing = *t;

        GH_THING_VERIFY(thing);

        if (thing->is_hero) {
            t++;
            continue;
        }

        thing->tick();

        t++;
    }
}

void gh_level::scare_monst_all (void)
{
    GH_FUNCTION()

    scare_monsts = false;

    list<gh_thing*>::iterator t = does_stuff_things.begin();

    while (t != does_stuff_things.end()) {
        gh_thing *thing = *t;

        if (thing->is_hero) {
            t++;
            continue;
        }

        thing->scared_boost_start(10);

        t++;
    }
}

void gh_level::tick_things (void)
{
    GH_FUNCTION()

    if (freeze_things) {
        return;
    }

    if (is_editing) {
        if (!gh_global::debug) {
            return;
        }
    }

    tick_things_all();
}

void gh_level::tick_hero (void)
{
    if (freeze_things) {
        return;
    }

    if (is_editing) {
        return;
    }

    GH_FUNCTION()

    game->player->thing->tick();
}

void gh_level::reset_all (void)
{
    GH_FUNCTION()

    //
    // Reset all things on the map
    //
    list<gh_thing*>::iterator t = map_things.begin();

    while (t != map_things.end()) {
        gh_thing *thing = *t;

        thing->reset();

        t++;
    }

    //
    // We use this level creation time to allow the player a bit of
    // time before the monsters move.
    //
    created_when = gh_get_time_milli();
}

void gh_level::level_completed (void)
{
    GH_FUNCTION()

    if (!is_level_completed) {
        level_completed_bonus = 0;
        return;
    }

    if (!level_completed_bonus) {
        return;
    }

    if (game->player->thing->treasure / 100) {
        bonuses.push_back(
            gh_bonus(
                "%%fg1=yellow%%bg=black"
                "%%fg2=white%%bg=black"
                "Unspent treasure bonus "
                "%%fg=random"
                + tostring(game->player->thing->treasure / 100) +
                " points"
                , game->player->thing->treasure / 100));
    }

    int health_bonus = game->player->thing->health;

    bonuses.push_back(
        gh_bonus(
            "%%fg1=yellow%%bg=black"
            "%%fg2=white%%bg=black"
            "Remaining health bonus "
            "%%fg=random"
            + tostring(health_bonus / 10) +
            " points"
            , health_bonus / 10));

    int fate_bonus = game->player->thing->fate * 10;

    bonuses.push_back(
        gh_bonus(
            "%%fg1=yellow%%bg=black"
            "%%fg2=white%%bg=black"
            "Remaining fate points bonus "
            "%%fg=random"
            + tostring(fate_bonus) +
            " points"
            , fate_bonus));

    if (level_completed_bonus) {
        bonuses.push_back(
            gh_bonus(
                "%%fg1=yellow%%bg=black"
                "%%fg2=white%%bg=black"
                "Level Completed "
                "%%fg=random"
                + tostring(level_completed_bonus) +
                " points"
                , level_completed_bonus));
    }

    level_completed_bonus = 0;
}

void gh_level::tick (void)
{
    GH_FUNCTION()
}

void operator>> (gh_stream_in& is, gh_level *me)
{
    bool tmp_bool;

#define GET_OPT_NAMED_FLAG(__FLAG__)                                    \
            me->get_opt_named_flag(                                     \
                   is,                                                  \
                   &tmp_bool,                                           \
                   #__FLAG__,                                           \
                   false);                                              \
                                                                        \
            me->__FLAG__ = tmp_bool;                                    \

    GH_FUNCTION()
                
    me->get(is, &me->desc, "desc");
    me->get_opt(is, &me->self, "self", (void*)NULL);
    me->get_opt(is, &me->level_no, "level_no", me->level_no);
    me->get_opt(is, &me->level_set_name, "level_set_name");
    me->get_opt(is, &me->level_completed_bonus, "level_completed_bonus", 0);
    me->get_opt(is, &me->time_left, "time_left", 0);

    me->is_last_level = false;
    me->is_dungeon = false;
    me->is_cave = false;
    me->is_forest = false;
    me->is_landscape = false;
    me->is_void = false;
    me->is_hell = false;
    me->is_bonus = false;

    GET_OPT_NAMED_FLAG(is_last_level);
    GET_OPT_NAMED_FLAG(is_dungeon);
    GET_OPT_NAMED_FLAG(is_cave);
    GET_OPT_NAMED_FLAG(is_forest);
    GET_OPT_NAMED_FLAG(is_landscape);
    GET_OPT_NAMED_FLAG(is_void);
    GET_OPT_NAMED_FLAG(is_hell);
    GET_OPT_NAMED_FLAG(is_bonus);
    GET_OPT_NAMED_FLAG(first_enter);

    me->get_opt_class(is, &me->draw_start, "draw_start");
    me->get_opt_class(is, &me->draw_end, "draw_end");
    me->get_opt_class(is, &me->hero_start_where, "hero_start_where");
    me->get_opt_class(is, &me->hero_end_where, "hero_end_where");

    list<gh_thing*>::iterator it;

    me->get_opt_class_pcontainer(is, &me->all_things, "all_things");
    me->get_opt_class_pcontainer(is, &me->rooms, "rooms");
    me->get_opt_class(is, &me->map, "map");
}

void operator<< (gh_stream_out& os, const gh_level *me)
{
    bool tmp_bool;

#define PUT_OPT_NAMED_FLAG(__FLAG__)                                    \
    tmp_bool = me->__FLAG__;                                            \
    me->put_opt_named_flag(os, &tmp_bool, #__FLAG__, false);

    GH_FUNCTION()

    me->put(os, &me->desc, "desc");
    me->put(os, &me->self, "self");
    me->put(os, &me->level_no, "level_no");
    me->put(os, &me->level_set_name, "level_set_name");
    me->put(os, &me->level_completed_bonus, "level_completed_bonus");
    me->put(os, &me->time_left, "time_left");

    PUT_OPT_NAMED_FLAG(is_last_level)
    PUT_OPT_NAMED_FLAG(is_dungeon)
    PUT_OPT_NAMED_FLAG(is_cave)
    PUT_OPT_NAMED_FLAG(is_forest)
    PUT_OPT_NAMED_FLAG(is_landscape)
    PUT_OPT_NAMED_FLAG(is_void)
    PUT_OPT_NAMED_FLAG(is_hell)
    PUT_OPT_NAMED_FLAG(is_bonus)
    PUT_OPT_NAMED_FLAG(first_enter);

    if (!me->is_editing) {
        me->put_class(os, &me->draw_start, "draw_start");
        me->put_class(os, &me->draw_end, "draw_end");
    }

    me->put_class(os, &me->hero_start_where, "hero_start_where");
    me->put_class(os, &me->hero_end_where, "hero_end_where");

    list<gh_thing*>::const_iterator it;

    //
    // Ignore anything in level edit mode when saving which is not on
    // the map. e.g. player posessions.
    //
    os << endl;
    os << os.indent_str();
    me->put_name(os, "all_things");
    os << "[";
    os.indent(1);

    for (it = me->all_things.begin(); it != me->all_things.end(); it++) {
        gh_thing *t = *it;

        const gh_thing *owner = t->get_owner();
        
        if (owner) {
            if (owner->is_on_map) {
                //
                // If the owner is on the map, save the possessions too
                //
            } else {
                if (owner->is_hero) {
                    //
                    // The owner is not on the map. Hide the possessions.
                    //
                    continue;
                } else {
                    //
                    // Play safe and save.
                    //
                }
            }
        } else {
            if (!t->is_on_map) {
                //
                // Check if the hero is on this level. If not, ignore it.
                //
                if (t->is_hero) {
                    //
                    // An unowned thing and not on the map? Ignore it.
                    //
                    continue;
                } else {
                    //
                    // Play safe and save.
                    //
                }
            }
        }

        me->put_class_unnamed(os, t);
    }

    os.indent(-1);
    os << endl;
    os << os.indent_str();
    os << "]";

    me->put_class_pcontainer(os, &me->rooms, "rooms");
    me->put_class(os, &me->map, "map");
}
