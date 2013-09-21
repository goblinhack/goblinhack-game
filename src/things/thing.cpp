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

    $Id: thing.cpp 1298 2009-08-24 19:52:20Z Ghack $
*/

#include <ghack.h>
#include <hacklib.h>
#include <gh_thing.h>
#include <gh_unique_things.h>
#include <gh_surface.h>
#include <gh_thing_registration.h>
#include <gh_level.h>
#include <gh_room.h>
#include <gh_global.h>
#include <gh_templates.h>
#include <gh_collage.h>
#include <gh_math.h>
#include <gh_sdl.h>
#include <gh_player.h>
#include <hacklib.h>

//
// Prevents errors when loading a thing for the first time as part
// of load all, as we have no template thing yet.
//
static bool error_on_unknown_thing = true;

vector<class gh_thing *> gh_thing::things_sorted;

extern list <gh_thing*> leaked_things;

static uint32 gh_thing_global_id = 1;

//
// For the progress bar
//
int gh_things_loaded;

gh_level *gh_level_current;

gh_thing::gh_thing (gh_level *level_in,
                    const string config_file,
                    const gh_point3d born_at,
                    const gh_thing *template_thing,
                    gh_player *player_in)
{
    GH_FUNCTION()

    //
    // Linkage between the player and ourselves.
    //
    player                      = player_in;
    if (player_in) {
        player_in->thing = this;
    }

    memset(light_ray_length, 0, sizeof(light_ray_length));

    self                        = this;
    id                          = ++gh_thing_global_id;
    friend_id                   = 0;
    group_id                    = 0;
    name                        = "";
    text                        = "";
    debug_string                = "";
    full_name                   = config_file;
    short_name                  = config_file;

    string_replace(short_name, ".gz", "");
    string_rremove_to_start(short_name, "things/");

    gh_thing_ptr_add(this);

    killed_by                   = "";
    flash_color                 = gh_fpixel(1.0,1.0,1.0,1.0);
    level                       = NULL;
    room                        = NULL;

    //
    // References
    //
    ref                         = 0;
    GH_STRUCT_INIT(refc);
    ref_bitmap                  = 0;
    leaked_things_it_valid      = false;
    all_things_it_valid         = false;
    is_on_map                   = false;
    does_stuff_things_it_valid  = false;
    dead_things_it_valid        = false;
    missile_thrower             = NULL;
    owner                       = NULL;
    carrier                     = NULL;
    next                        = NULL;
    prev                        = NULL;
    limb_next                   = NULL;
    limb_prev                   = NULL;

    processed_in_loop           = false;

    //
    // Important these are set to sane defaults as we only update
    // them when a thing is rendered. So before the first render
    // we need to have *something* to do collision detection on.
    //
    dir                         = GH_THING_DIR_ALL;
    mode                        = GH_THING_MODE_NORMAL;

    frame                       = 0;
    at                          = born_at;
    cells_visited               = NULL;
    cell_max_age                = 0;
    cell_target_age             = 0;
    born                        = born_at;
    missile_at                  = gh_fpoint3d(0,0,0);
    missile_target              = gh_point3d(-1,-1,-1);
    missile_angle               = 0.0;
    missile_angle_delta         = 0.0;
    missile_spread              = 0;
    missile_spread_angle        = 0;
    weapon                      = NULL;
    weapon_spin                 = 0;
    weapon_wielder              = NULL;
    limbs                       = 0;
    join                        = GH_THING_DIR_NONE;
    direction                   = gh_point3d(0,0,0);
    direction_wanted            = gh_point3d(0,0,0);
    direction_last              = gh_point3d(0,0,0);
    target                      = gh_point3d(0,0,0);
    target_last_set_where       = gh_point3d(0,0,0);
    target_last_thing           = NULL;
    treasure                    = 0;
    cost                        = 0;
    keys                        = 0;
    score                       = 0;
    bravery                     = 0;
    bravery_curr                = 0;
    anim                        = gh_math::rand100();
    weight                      = 0;
    _strength                   = 0;
    strength_boost              = 0;
    strength_req                = 0;
    _water_proof                = 0;
    water_proof_boost           = 0;
    _fire_proof                 = 0;
    fire_proof_boost            = 0;
    _acid_proof                 = 0;
    acid_proof_boost            = 0;
    _cold_proof                 = 0;
    cold_proof_boost            = 0;
    _suffocation_proof          = 0;
    suffocation_proof_boost     = 0;
    _teleport_proof             = 0;
    teleport_proof_boost        = 0;
    _poison_proof               = 0;
    poison_proof_boost          = 0;
    _stone_proof                = 0;
    stone_proof_boost           = 0;
    _dex                        = 0;
    dex_boost                   = 0;
    dex_req                     = 0;
    lifespan                    = -1; // immortal
    _scared                     = 0;
    scared_boost                = 0;
    _sleeping                   = 0;
    sleeping_boost              = 0;
    _shielded                   = 0;
    shielded_boost              = 0;
    _invisible                  = 0;
    invisible_boost             = 0;
    _raging                     = 0;
    raging_boost                = 0;
    _poisoned                   = 0;
    poisoned_boost              = 0;
    _stoned                     = 0;
    stoned_boost                = 0;
    _metabolism                 = 0;
    metabolism_boost            = 0;
    _stamina                    = 0;
    stamina_boost               = 0;
    orig_stamina                = 0;
    _light_strength             = 0;
    light_strength_boost        = 0;
    _can_see_distance           = 0;
    can_see_distance_boost      = 0;
    health                      = 0;
    orig_health                 = 0;
    fate                        = 0;
    intelligence                = 0;
    intelligence_req            = 0;
    speed                       = 1;
    bite_damage                 = 0;
    bite_armour                 = 0;
    bite_treasure               = 0;
    //
    // Don't set this to non zero as powerups can inherit it.
    //
    bite_delay_in_tenths        = 0;
    bite_chance                 = 0;
    karma                       = 0;
    bite_karma_chance           = 0;
    bite_karma                  = 0;
    bite_intelligence_chance    = 0;
    bite_intelligence           = 0;
    bite_poison_chance          = 0;
    bite_poison                 = 0;
    bite_stone_chance           = 0;
    bite_stone                  = 0;
    bite_fate_chance            = 0;
    bite_fate                   = 0;
    bite_last_when              = gh_sdl_get_time_milli() + gh_math::rand1000();
    spawn_delay_in_tenths       = 0;
    spawn_last_when             = gh_sdl_get_time_milli() + gh_math::rand1000();
    follow_distance             = 0;
    transmog_delay_in_tenths    = 0;
    transmog_last_when          = gh_sdl_get_time_milli() + gh_math::rand1000();
    teleport_delay_in_tenths    = 30;
    teleport_last_when          = gh_sdl_get_time_milli() + gh_math::rand1000();
    teleport_chance             = 0;
    charges                     = 0;
    look_delay_in_tenths        = 2 + gh_math::rand10();
    look_last_when              = gh_sdl_get_time_milli() + gh_math::rand1000();
    armour                      = 0;

    //
    // Important the first tick is staggered so all monsts of
    // the same type dont tick together; bad for performance
    //
    tick_at                     = gh_math::rand100();
    last_boost_tick_when        = 0;
    quick_tick_when             = 0;
    last_slow_boost_tick_when   = 0;
    born_when                   = gh_sdl_get_time_milli();
    has_been_seen               = false;
    needs_fixup                 = false;
    cant_see_past               = false;
    collects_stuff_but_doesnt_use_it = false;
    does_stuff                  = false;
    cached_gfx                  = false;
    is_floor                    = false;
    is_dirt                     = false;
    is_wall                     = false;
    is_rock                     = false;
    is_hard                     = false;
    is_body_part                = false;
    is_chained_body_part        = false;
    stay_above_torso            = false;
    is_found_only_in_shops      = false;
    not_in_shops                = false;
    is_unpaid_for               = false;
    is_shopkeeper               = false;
    is_stolen                   = false;
    is_thief                    = false;
    is_explorer                 = false;
    shake_screen_on_death       = false;
    is_inventory_item           = false;
    is_auto_item                = false;
    is_weapon                   = false;
    is_internal                 = false;
    is_sword                    = false;
    is_staff                    = false;
    is_wand                     = false;
    is_axe                      = false;
    is_warhammer                = false;
    is_bow                      = false;
    is_gun                      = false;
    is_armour                   = false;
    is_amulet                   = false;
    is_cursed                   = false;
    curse_noticed               = false;
    can_see_cursed_items        = false;
    can_wield_weapons           = false;
    can_cast_spells             = false;
    can_insult                  = false;
    is_chargeable               = false;
    is_potion                   = false;
    is_spell                    = false;
    is_unable_to_move           = false;
    is_generator                = false;
    cannot_summon               = false;
    is_monst                    = false;
    is_destroyable              = false;
    is_missile                  = false;
    is_bouncy                   = false;
    is_boring                   = false;
    is_corridor                 = false;
    is_hero                     = false;
    is_teleport                 = false;
    is_teleport_attack          = false;
    is_exit                     = false;
    is_return_exit              = false;
    is_door                     = false;
    is_fire                     = false;
    hates_fire                  = false;
    loves_fire                  = false;
    is_acid                     = false;
    hates_acid                  = false;
    loves_acid                  = false;
    is_water                    = false;
    hates_water                 = false;
    loves_water                 = false;
    is_death                    = false;
    loves_death                 = false;
    is_cold                     = false;
    hates_cold                  = false;
    loves_cold                  = false;
    is_suffocation              = false;
    is_gas                      = false;
    is_meat                     = false;
    is_meat_eater               = false;
    is_edible                   = false;
    is_edible_eater             = false;
    is_treasure                 = false;
    is_treasure_eater           = false;
    is_trap                     = false;
    is_holy                     = false;
    is_undead                   = false;
    wants_to_hit                = false;
    is_unique                   = false;
    is_silver                   = false;
    is_joinable                 = false;
    is_transparent              = false;
    destroy_joined              = false;
    is_light_source             = false;
    is_movable                  = false;
    is_open                     = false;
    is_closed                   = false;
    is_chasing                  = false;
    is_dead                     = false;
    is_burdened                 = false;
    only_in_caves               = false;
    only_in_forests             = false;
    only_in_landscapes          = false;
    only_in_dungeons            = false;
    loves_void                  = false;
    only_in_hell                = false;
    loves_full_moon             = false;
    loves_midnight              = false;
    loves_night                 = false;
    loves_friday_13th           = false;
    loves_halloween             = false;
    never_slows_down            = false;
    moves_up_down               = false;
    moves_left_right            = false;
    moves_until_collision       = false;
    leaves_corpse               = false;
    casts_lightning             = false;
    tireless_swing              = false;
    tireless                    = false;
    can_self_teleport                   = false;
    can_self_teleport_once_per_level    = false;
    ability_can_rage                    = false;
    ability_can_auto_rage               = false;
    ability_can_regenerate1             = false;
    ability_can_regenerate2             = false;
    ability_can_regenerate3             = false;
    cluster_anywhere            = false;
    rendered                    = 0;
    render_depth                = 0;
    exit_name                   = "";
    exit_jump                   = 1;
    cluster_radius              = 0;
    cluster_amount              = 0;
    laser_range                 = 0;
    opacity                     = 0; // transparent
    level_mod                   = 0;
    anim_speed                  = gh_global::anim_speed;
    context                     = NULL;

    memset(&anim_info, 0, sizeof(gh_model_anim_info));

    model                       = NULL;
    tex                         = NULL;
    pixel                       = GH_WHITE;
    spin                        = 0.0;
    rot                         = 0.0;
    draw_as_hashed_cube         = false;
    casts_shadow                = false;
    is_solid                    = false;

    string short_name = config_file;
    string_replace(short_name, ".gz", "");
    string_rremove_to_start(short_name, "things/");

    //
    // Try and find a template for the thing
    //
    if (template_thing == NULL) {
        //
        // If loading from a file, this wont be set yet.
        //
        if (short_name != "") {
            template_thing = gh_thing::find(short_name,
                                            error_on_unknown_thing);
        }
    }

    //
    // Save our template pointer so we can get at shared stuff like anim
    // frames.
    //
    this->template_thing        = template_thing;

    //
    // Try to determine if this thing is being loaded as part of a saved
    // game.
    //
    is_saved_thing              = false;

    //
    // We'll try to set this below. Clear it for safety.
    //
    anim_frames                 = NULL;

    if (template_thing != NULL) {
        //
        // We have an existing template. Copy it.
        //
        copy(template_thing);
    } else {
        //
        // Read the config file.
        //
        if (config_file != "") {
            //
            // If this is a template, allocate space for the graphics,
            // shared by all things that reference this one.
            //
            anim_frames = new gh_anim;

            gh_read_file(this, config_file);

            render_cached_all();

            if (!string_contains(short_name, "powerup") &&
                !string_contains(short_name, "hoard")) {
                update_anim();
            }
        } else {
            //
            // No template and no config file? Saved thing.
            //
            // Is being loaded from a saved game. Will need some fixups.
            //
            is_saved_thing = true;
        }
    }

    //
    // The thing could have been copied from a template.
    // Need to initialize properly now.
    //
    ref                 = 0;
    GH_STRUCT_INIT(refc);
    ref_bitmap          = 0;

    //
    // This is a hack. If we load a thing from a file dynamically,
    // we have no level pointer saved - so just use the last one
    // assuming a thing has been loaded already from the map; which
    // is likely.
    //
    if (level_in == NULL) {
        level_in = gh_level_current;
    } else {
        if (!gh_level_current) {
            gh_level_current = level_in;
        }
    }

    level               = level_in;
    at                  = born_at;
    born                = born_at;

    //
    // Monsters start off moving!
    //
    if (is_monst) {
        direction                   = gh_point3d(0,0,0);
        target                      = gh_point3d(0,0,0);

        if (moves_up_down) {
            direction_wanted        = gh_point3d(0,speed,0);
        } else if (moves_left_right) {
            direction_wanted        = gh_point3d(speed,0,0);
        } else {
            direction_wanted        = gh_point3d(speed,0,0);
        }
    }

    if (charges) {
        is_chargeable = true;
    }

    if (!anim_speed) {
        anim_speed = gh_global::anim_speed;
    }

    if (!orig_health) {
        orig_health = health;
    }

    if (!orig_stamina) {
        orig_stamina = _stamina;
    }

    if (charges) {
        is_chargeable = true;
    }

    //
    // Can't poison the dead!
    //
    if (is_undead) {
        _poison_proof += 50;
    }

    add_leaked_things();

    if (does_stuff) {
        add_does_stuff_things();
    }

    //
    // Loaded things from saved games are already on the all things list.
    // But, they have no valid iterator yet. We'll fix that up later in
    // game/load.cpp
    //
    if (!is_saved_thing) {
        add_all_things();
    }

    reg = gh_thing_registration_get(short_name);
    if (!reg) {
        reg = gh_thing_registration_get("default");
        if (!reg) {
            debug();
            GH_THROW("no callbacks found for " + name + " or default thing");
        }
    }

    //
    // We add this ref so that if we swap weapons in loading our backback,
    // then it does not become invalid. So this is a ref on ourselves!
    //
    incref(GH_THING_REF_FIRST);

    //
    // If this thing begins life with some kit, add it to the carry list.
    //
    // But don't let hoards collect their own treasure, they're just
    // placeholders.
    //
    if ((is_monst || is_hero) && level_in && backpack.size()) {
        //
        // Only for real things being loaded onto a level.
        //
redo:
        vector<string>::iterator b = backpack.begin();

        while (b != backpack.end()) {
            string i = *b;

            //
            // See if this item has a chance of being in the backpack.
            //
            if (isdigit(i[0])) {

                //
                // Ok it is a number. Lets see if we want the next item.
                //
                int chance = strtol(i.c_str(), NULL, 10);

                if (gh_math::rand100() > chance) {
                    //
                    // Skip the number and item.
                    //
                    b++;
                    b++;
                    continue;
                }

                //
                // Else, just skip the number.
                //
                b++;

                //
                // Grab the name
                //
                i = *b;
            }

            gh_thing *t = new gh_thing(level_in, "things/" + i);
        
            //
            // This might be a hoard. If so, push this list onto our existing
            // list and continue.
            //
            if (t->backpack.size()) {
                vector<string>::iterator c = t->backpack.begin();

                vector<string>::iterator b = backpack.begin();
                erase(backpack);

                while (c != t->backpack.end()) {
                    backpack.push_back(*c);
                    c++;
                }

                t->decref(GH_THING_REF_FIRST);
                goto redo;
            }

            collect(t);

            t->decref(GH_THING_REF_FIRST);

            GH_THING_VERIFY(t);

            //
            // Skip the thing name
            //
            b++;
        }

        erase(backpack);
    }

    //
    // Start out able to teleport if enabled.
    //
    if (can_self_teleport_once_per_level) {
        can_self_teleport = true;
    }

    //
    // Too noisy and slows the game down on high levels.
    //
    if (gh_global::debug) {
        if (is_monst) {
            if (level) {
                GH_THING_LOG(this,
                             "born; all:" + tostring(level->all_things.size()) +
                             " dead:" + tostring(level->dead_things.size()) +
                             " map: " + tostring(level->map_things.size()));
            }
        }
    }

    if (gh_unique_things) {
        gh_unique_things->copy_a_named_thing(this);
    }

    //
    // Debug
    //
//    if (is_monst || is_hero) {
//      toughness();
//    }
}

//
// Copy a template into a thing
//
void gh_thing::copy (const gh_thing *thing)
{
    GH_FUNCTION()

    id                              = ++gh_thing_global_id;
    friend_id                       = thing->friend_id;
    group_id                        = thing->group_id;
    self                            = this;
    name                            = thing->name;
    text                            = thing->text;
    full_name                       = thing->full_name;
    short_name                      = thing->short_name;
    level                           = thing->level;
    processed_in_loop               = thing->processed_in_loop;
    dir                             = thing->dir;
    mode                            = thing->mode;
    frame                           = thing->frame;
    at                              = thing->at;
    born                            = thing->born;
    missile_at                      = thing->missile_at;
    missile_target                  = thing->missile_target;
    missile_angle                   = thing->missile_angle;
    missile_angle_delta             = thing->missile_angle_delta;
    missile_spread                  = thing->missile_spread;
    missile_spread_angle            = thing->missile_spread_angle;
    weapon_spin                     = thing->weapon_spin;
    limbs                           = thing->limbs;
    join                            = thing->join;
    direction                       = thing->direction;
    direction_wanted                = thing->direction_wanted;
    direction_last                  = thing->direction_last;
    target                          = thing->target;
    score                           = thing->score;
    bravery                         = thing->bravery;
    bravery_curr                    = thing->bravery_curr;
    treasure                        = thing->treasure;
    cost                            = thing->cost;
    keys                            = thing->keys;
    weight                          = thing->weight;
    _strength                       = thing->_strength;
    strength_boost                  = thing->strength_boost;
    strength_req                    = thing->strength_req;
    _water_proof                    = thing->_water_proof;
    water_proof_boost               = thing->water_proof_boost;
    _fire_proof                     = thing->_fire_proof;
    fire_proof_boost                = thing->fire_proof_boost;
    _acid_proof                     = thing->_acid_proof;
    acid_proof_boost                = thing->acid_proof_boost;
    _cold_proof                     = thing->_cold_proof;
    cold_proof_boost                = thing->cold_proof_boost;
    _suffocation_proof              = thing->_suffocation_proof;
    suffocation_proof_boost         = thing->suffocation_proof_boost;
    _teleport_proof                 = thing->_teleport_proof;
    teleport_proof_boost            = thing->teleport_proof_boost;
    _poison_proof                   = thing->_poison_proof;
    poison_proof_boost              = thing->poison_proof_boost;
    _stone_proof                    = thing->_stone_proof;
    stone_proof_boost               = thing->stone_proof_boost;
    _dex                            = thing->_dex;
    dex_boost                       = thing->dex_boost;
    dex_req                         = thing->dex_req;
    lifespan                        = thing->lifespan;
    _scared                         = thing->_scared;
    scared_boost                    = thing->scared_boost;
    _sleeping                       = thing->_sleeping;
    sleeping_boost                  = thing->sleeping_boost;
    _shielded                       = thing->_shielded;
    shielded_boost                  = thing->shielded_boost;
    _invisible                      = thing->_invisible;
    invisible_boost                 = thing->invisible_boost;
    _raging                         = thing->_raging;
    raging_boost                    = thing->raging_boost;
    _poisoned                       = thing->_poisoned;
    poisoned_boost                  = thing->poisoned_boost;
    _stoned                         = thing->_stoned;
    stoned_boost                    = thing->stoned_boost;
    _stamina                        = thing->_stamina;
    stamina_boost                   = thing->stamina_boost;
    orig_stamina                    = thing->orig_stamina;
    _metabolism                     = thing->_metabolism;
    metabolism_boost                = thing->metabolism_boost;
    _light_strength                 = thing->_light_strength;
    light_strength_boost            = thing->light_strength_boost;
    _can_see_distance               = thing->_can_see_distance;
    can_see_distance_boost          = thing->can_see_distance_boost;
    health                          = thing->health;
    orig_health                     = thing->orig_health;
    fate                            = thing->fate;
    intelligence                    = thing->intelligence;
    intelligence_req                = thing->intelligence_req;
    speed                           = thing->speed;
    bite_damage                     = thing->bite_damage;
    bite_armour                     = thing->bite_armour;
    bite_treasure                   = thing->bite_treasure;
    bite_delay_in_tenths            = thing->bite_delay_in_tenths;
    bite_chance                     = thing->bite_chance;
    karma                           = thing->karma;
    bite_karma                      = thing->bite_karma;
    bite_karma_chance               = thing->bite_karma_chance;
    bite_intelligence               = thing->bite_intelligence;
    bite_intelligence_chance        = thing->bite_intelligence_chance;
    bite_poison                     = thing->bite_poison;
    bite_poison_chance              = thing->bite_poison_chance;
    bite_stone                      = thing->bite_stone;
    bite_stone_chance               = thing->bite_stone_chance;
    bite_fate                       = thing->bite_fate;
    bite_fate_chance                = thing->bite_fate_chance;
    spawn_delay_in_tenths           = thing->spawn_delay_in_tenths;
    follow_distance                 = thing->follow_distance;
    transmog_delay_in_tenths        = thing->transmog_delay_in_tenths;
    teleport_chance                 = thing->teleport_chance;
    teleport_delay_in_tenths        = thing->teleport_delay_in_tenths;
    charges                         = thing->charges;
    look_delay_in_tenths            = thing->look_delay_in_tenths;
    armour                          = thing->armour;
    explosion_type                  = thing->explosion_type;
    audio_on_dead                   = thing->audio_on_dead;
    audio_on_hit                    = thing->audio_on_hit;
    audio_on_open                   = thing->audio_on_open;
    audio_on_use                    = thing->audio_on_use;
    wield_as                        = thing->wield_as;
    tick_at                         = gh_math::rand100();
    cant_see_past                   = thing->cant_see_past;
    collects_stuff_but_doesnt_use_it= thing->collects_stuff_but_doesnt_use_it;
    does_stuff                      = thing->does_stuff;
    cached_gfx                      = thing->cached_gfx;
    is_floor                        = thing->is_floor;
    is_dirt                         = thing->is_dirt;
    is_wall                         = thing->is_wall;
    is_rock                         = thing->is_rock;
    is_hard                         = thing->is_hard;
    is_body_part                    = thing->is_body_part;
    is_chained_body_part            = thing->is_chained_body_part;
    stay_above_torso                = thing->stay_above_torso;
    is_shopkeeper                   = thing->is_shopkeeper;
    is_stolen                       = thing->is_stolen;
    is_unpaid_for                   = thing->is_unpaid_for;
    is_found_only_in_shops          = thing->is_found_only_in_shops;
    not_in_shops                    = thing->not_in_shops;
    is_thief                        = thing->is_thief;
    is_explorer                     = thing->is_explorer;
    shake_screen_on_death           = thing->shake_screen_on_death;
    is_inventory_item               = thing->is_inventory_item;
    is_auto_item                    = thing->is_auto_item;
    is_weapon                       = thing->is_weapon;
    is_internal                     = thing->is_internal;
    is_sword                        = thing->is_sword;
    is_staff                        = thing->is_staff;
    is_wand                         = thing->is_wand;
    is_axe                          = thing->is_axe;
    is_warhammer                    = thing->is_warhammer;
    is_bow                          = thing->is_bow;
    is_gun                          = thing->is_gun;
    is_armour                       = thing->is_armour;
    is_amulet                       = thing->is_amulet;
    is_cursed                       = thing->is_cursed;
    curse_noticed                   = thing->curse_noticed;
    can_see_cursed_items            = thing->can_see_cursed_items;
    can_wield_weapons               = thing->can_wield_weapons;
    can_cast_spells                 = thing->can_cast_spells;
    can_insult                      = thing->can_insult;
    is_chargeable                   = thing->is_chargeable;
    is_potion                       = thing->is_potion;
    is_spell                        = thing->is_spell;
    is_unable_to_move               = thing->is_unable_to_move;
    is_generator                    = thing->is_generator;
    cannot_summon                   = thing->cannot_summon;
    is_monst                        = thing->is_monst;
    is_destroyable                  = thing->is_destroyable;
    is_missile                      = thing->is_missile;
    is_bouncy                       = thing->is_bouncy;
    is_boring                       = thing->is_boring;
    is_corridor                     = thing->is_corridor;
    is_hero                         = thing->is_hero;
    is_teleport                     = thing->is_teleport;
    is_teleport_attack              = thing->is_teleport_attack;
    is_exit                         = thing->is_exit;
    is_return_exit                  = thing->is_return_exit;
    is_door                         = thing->is_door;
    is_fire                         = thing->is_fire;
    hates_fire                      = thing->hates_fire;
    loves_fire                      = thing->loves_fire;
    is_acid                         = thing->is_acid;
    hates_acid                      = thing->hates_acid;
    loves_acid                      = thing->loves_acid;
    is_water                        = thing->is_water;
    hates_water                     = thing->hates_water;
    loves_water                     = thing->loves_water;
    is_death                        = thing->is_death;
    loves_death                     = thing->loves_death;
    is_cold                         = thing->is_cold;
    hates_cold                      = thing->hates_cold;
    loves_cold                      = thing->loves_cold;
    is_suffocation                  = thing->is_suffocation;
    is_gas                          = thing->is_gas;
    is_meat                         = thing->is_meat;
    is_meat_eater                   = thing->is_meat_eater;
    is_edible                       = thing->is_edible;
    is_edible_eater                 = thing->is_edible_eater;
    is_treasure                     = thing->is_treasure;
    is_treasure_eater               = thing->is_treasure_eater;
    is_trap                         = thing->is_trap;
    is_undead                       = thing->is_undead;
    is_unique                       = thing->is_unique;
    is_silver                       = thing->is_silver;
    is_holy                         = thing->is_holy;
    is_joinable                     = thing->is_joinable;
    is_transparent                  = thing->is_transparent;
    destroy_joined                  = thing->destroy_joined;
    is_light_source                 = thing->is_light_source;
    is_movable                      = thing->is_movable;
    only_in_caves                           = thing->only_in_caves;
    only_in_forests                 = thing->only_in_forests;
    only_in_landscapes              = thing->only_in_landscapes;
    only_in_dungeons                = thing->only_in_dungeons;
    loves_void                      = thing->loves_void;
    only_in_hell                            = thing->only_in_hell;
    loves_full_moon                 = thing->loves_full_moon;
    loves_midnight                  = thing->loves_midnight;
    loves_night                     = thing->loves_night;
    loves_friday_13th               = thing->loves_friday_13th;
    loves_halloween                 = thing->loves_halloween;
    never_slows_down                = thing->never_slows_down;
    moves_up_down                   = thing->moves_up_down;
    moves_left_right                = thing->moves_left_right;
    moves_until_collision           = thing->moves_until_collision;
    leaves_corpse                   = thing->leaves_corpse;
    casts_lightning                 = thing->casts_lightning;
    tireless_swing                  = thing->tireless_swing;
    tireless                        = thing->tireless;
    ability_can_regenerate1         = thing->ability_can_regenerate1;
    ability_can_regenerate2         = thing->ability_can_regenerate2;
    ability_can_regenerate3         = thing->ability_can_regenerate3;
    can_self_teleport               = thing->can_self_teleport;
    can_self_teleport_once_per_level = thing->can_self_teleport_once_per_level;
    ability_can_rage                = thing->ability_can_rage;
    ability_can_auto_rage           = thing->ability_can_auto_rage;
    cluster_anywhere                = thing->cluster_anywhere;
    is_open                         = thing->is_open;
    is_closed                       = thing->is_closed;
    is_chasing                      = thing->is_chasing;
    render_depth                    = thing->render_depth;
    exit_jump                       = thing->exit_jump;
    exit_name                       = thing->exit_name;
    cluster_radius                  = thing->cluster_radius;
    cluster_amount                  = thing->cluster_amount;
    laser_range                     = thing->laser_range;
    opacity                         = thing->opacity;
    level_mod                       = thing->level_mod;
    anim_speed                      = thing->anim_speed;
    context                         = thing->context;
    anim_frames                     = thing->anim_frames;

    //
    // This is a hack for grenades to make them bouunce from anim
    // frame 0
    //
    if (is_missile) {
        anim                        = 0;
    } else {
        anim                        = rand();
    }

    //
    // These might be loaded from the saved file, so we don't
    // want to double them in size.
    //
    if (!appearing_chance.size()) {
        appearing_chance = thing->appearing_chance;
    }

    if (!throws.size()) {
        throws = thing->throws;
    }

    if (!attacks.size()) {
        attacks = thing->attacks;
    }

    if (!wake_reasons.size()) {
        wake_reasons = thing->wake_reasons;
    }

    if (!attack_chance.size()) {
        attack_chance = thing->attack_chance;
    }

    if (!follows.size()) {
        follows = thing->follows;
    }

    if (!follow_chance.size()) {
        follow_chance = thing->follow_chance;
    }

    if (!transmogrify_chance.size()) {
        transmogrify_chance = thing->transmogrify_chance;
    }

    if (!transmogrifies_into.size()) {
        transmogrifies_into = thing->transmogrifies_into;
    }

    if (!use_chance.size()) {
        use_chance = thing->use_chance;
    }

    if (!use_action.size()) {
        use_action = thing->use_action;
    }

    if (!bite_action.size()) {
        bite_action = thing->bite_action;
    }

    if (!followers.size()) {
        followers = thing->followers;
    }

    if (!spawns.size()) {
        spawns = thing->spawns;
    }

    if (!spawning_chance.size()) {
        spawning_chance = thing->spawning_chance;
    }

    if (!is_saved_thing) {
        //
        // The backpack is only ever loaded up at thing birth.
        //
        if (!backpack.size()) {
            backpack = thing->backpack;
        }
    }
}

gh_thing::~gh_thing (void)
{
    GH_FUNCTION()

    GH_THING_VERIFY(this);

    //
    // This reference is to stop re-entry of this routine,
    // as rm_all_things for example when it undoes its reg
    // and sees no more will call delete. So, if the caller
    // had no refs of their own then this is very likely.
    //
    // We never undo this temporary last ref.
    //
    if (isref(GH_THING_REF_LAST)) {
        return;
    }

    incref(GH_THING_REF_LAST);

    if (prev) {
        GH_THING_LOG(this, "this");

        if (prev) {
            GH_THING_LOG(prev, "prev");
        }

        if (next) {
            GH_THING_LOG(next, "next");
        }

        GH_THROW("prev still set");
    }

    //
    // This can be set at level creation if we fail to place a thing
    // like a snake.
    //
    if (next) {
        GH_THING_LOG(this, "destroy chain");

        GH_THING_VERIFY(next);

        //
        // Unlink it from us first. We're gone.
        //
        next->prev = NULL;

        //
        // Now unlink our hook to it.
        //
        gh_thing *n = next;
        next = NULL;

        delete n;
    }

    if (limb_prev) {
        GH_THING_LOG(this, "this");

        if (limb_prev) {
            GH_THING_LOG(limb_prev, "limb_prev");
        }

        if (limb_next) {
            GH_THING_LOG(limb_next, "limb_next");
        }

        GH_THROW("limb_prev still set");
    }

    //
    // This can be set at level creation if we fail to place a thing
    // like a snake.
    //
    if (limb_next) {
        GH_THING_LOG(this, "destroy chain");

        GH_THING_VERIFY(limb_next);

        //
        // Unlink it from us first. We're gone.
        //
        limb_next->limb_prev = NULL;

        //
        // Now unlink our hook to it.
        //
        gh_thing *n = limb_next;
        limb_next = NULL;

        delete n;
    }

    string dead_string;

    if (!is_missile && !is_boring) {
        if (killed_by != "") {
            dead_string = "DESTROYED: " + killed_by;
        } else {
            dead_string = "DESTROYED";
        }
    } else {
        dead_string = "";
    }

    //
    // Do this at this level in the code to keep the debug indent in scope.
    //
    GH_THING_LOG(this, dead_string);

    //
    // Get rid of all that we're carrying. We can't take it where
    // we're going!
    //
    if (!is_hero) {
        //
        // If we're not the hero, dump it back on the level for someone
        // else to grab.
        //
        // Don't want to leak things if the level is being nuked.
        //
        if (!level) {
            //
            // Template.
            //
        } else {
            //
            // Monster.
            //
            if (!level->being_deleted) {
                destroy_all_items();
            }
        }
    }

    //
    // If something was throwing us, it is no longer; we're dead!
    //
    if (missile_thrower) {
        //
        // This allows the owner to be deleted.
        //
        missile_thrower->decref(GH_THING_REF_MISSILE_THROWER);
    }

    //
    // If something was owning us, it is no longer; we're dead!
    //
    if (owner) {
        //
        // This allows the owner to be deleted.
        //
        owner->decref(GH_THING_REF_OWNER);
    }

    //
    // Get rid of our weapon.
    //
    if (weapon) {
        unwield();
    }

    //
    // Or if we're a weapon, tell our owner.
    //
    if (is_weapon) {
        if (weapon_wielder) {
            weapon_wielder->unwield();
        }
    }

    //
    // Now we've unwielded weapons we can destroy what else we carry.
    //
    destroy_all_items();

    //
    // Release the thing's memory of what it did in life...
    //
    if (cells_visited) {
        free(cells_visited);
        cells_visited = NULL;
    }

    //
    // We can be on the map perhaps had a collision during placement,
    // where it is just simpler to call delete rather than undo all
    // the refs one by one.
    //
    if (is_on_map) {
        level->map.pop(this);
    }

    //
    // Clean up final refs. Basically lists we are on.
    //
    rm_leaked_things();
    rm_does_stuff_things();
    rm_all_things();

    if (leaked_things_it_valid) {
        leaked_things.erase(leaked_things_it);
    }

    if (all_things_it_valid) {
        GH_WARN("thing " + name + " still on all things list");
    }

    if (does_stuff_things_it_valid) {
        GH_WARN("thing " + name + " still on map active things list");
    }

    if (dead_things_it_valid) {
        GH_WARN("thing " + name + " still on dead things list");
    }

    //
    // If this is a template, it owns the graphics.
    //
    if (template_thing == NULL) {
        //
        // tmp things may hit here - like the tmp player thing when loading
        //
        if (anim_frames) {
            //
            // It's really a template.
            //
//          GH_THING_LOG(this, "deleting a template(" + killed_by + ")");
            delete anim_frames;
        }

        anim_frames = NULL;
    }

    //
    // Don't count our temp delete ref
    //
    if (ref > 1) {
        debug();
        GH_WARN("thing " + name + " still referenced");
    }

    health = 0;

    gh_thing_ptr_free(this);
}

//
// Step through the hash until the nth element
//
gh_thing *gh_thing::nth (int n)
{
    GH_FUNCTION()

    hash_thing::iterator f = things.begin();

    while (n-- > 0) {
        if (f == things.end()) {
            return (NULL);
        }
        f++;
    }

    if (f == things.end()) {
        return (NULL);
    }

    return (f->second);
}

//
// Return the nth thing in the global array
//
gh_thing *gh_thing::nth_sorted (int n)
{
    GH_FUNCTION()

    return (things_sorted[n % things_sorted.size()]);
}

//
// How many things are in the hash?
//
size_t gh_thing::how_many (void)
{
    GH_FUNCTION()

    return (things.size());
}

//
// Find this thing in the thing bank
//
gh_thing *gh_thing::find (const string &s1, const bool err)
{
    GH_FUNCTION()

    gh_thing::hash_thing::iterator m = things.find(s1);

    if (m == things.end()) {
        if (err) {
            GH_WARN("couldn't find " + string(s1) + " in loaded list");
        }
        return (NULL);
    }

    gh_thing *thing = m->second;

    if (thing == NULL) {
        if (err) {
            GH_WARN("null thing " + string(s1) + " in loaded list");
            return (NULL);
        }
    }

    return (thing);
}

//
// Nuke all things
//
void gh_thing::unload_all (void)
{
    GH_FUNCTION()

    GH_LOG("Unloading things");

redo:
    hash_thing::iterator s = things.begin();

    while (s != things.end()) {
        gh_thing *t = s->second;

        t->decref(GH_THING_REF_FIRST);

        things.erase(s);

        goto redo;
    }

    delete gh_unique_things;
    gh_unique_things = NULL;

    erase_all(things_sorted);

    gh_level_current = NULL;

    GH_LOG("Finding leaked things");

    find_leaks();

    gh_things_loaded = 0;
    gh_global::how_many_things = 0;
}

void gh_thing::load_all (void)
{
    GH_FUNCTION()

    GH_LOG("Load things");

    error_on_unknown_thing = false;

    string dir = gh_global::locatedir("things");

    if (dir == "") {
        GH_THROW("no things dir?");
        return;
    }

    vector<string> files = gh_global::listdir_recursive(dir, "");

    vector<string>::iterator i;

    //
    // For the progress meter.
    //
    for (i = files.begin(); i != files.end(); i++) {
        string full_name = *i;
        string short_name = *i;

        if (string_contains(short_name, ".map")) {
            continue;
        }

        if (string_contains(short_name, ".tsp")) {
            continue;
        }

        if (string_contains(short_name, ".bmp")) {
            continue;
        }

        if (string_contains(short_name, ".png")) {
            continue;
        }

        gh_global::how_many_things++;
    }

    for (i = files.begin(); i != files.end(); i++) {
        string full_name = *i;
        string short_name = *i;

        if (string_contains(short_name, ".map")) {
            continue;
        }

        if (string_contains(short_name, ".tsp")) {
            continue;
        }

        if (string_contains(short_name, ".bmp")) {
            continue;
        }

        if (string_contains(short_name, ".png")) {
            continue;
        }

        string_replace(short_name, ".gz", "");
        string_rremove_to_start(short_name, "things/");

//      GH_LOG("THING: " + short_name);

        if (things[short_name]) {
            GH_THROW(("duplicate named object " + full_name));
        }

        gh_thing *template_thing = new gh_thing(NULL, full_name);

        things[short_name] = template_thing;

        //
        // We use the sorted list in level editing, so skip out a
        // lot of the internal stuff like missile explosions and
        // powerups.
        //
        if (!string_contains(short_name, "powerup") &&
            !string_contains(short_name, "hoard") &&
            !template_thing->is_missile &&
            !template_thing->is_body_part &&
            !template_thing->is_internal) {
            things_sorted.push_back(things[short_name]);
        }
    }

    error_on_unknown_thing = true;

    if (!gh_unique_things) {
        gh_unique_things = new gh_unique_thing_list();
    }
}

void operator>> (gh_stream_in& is, gh_thing *me)
{
    GH_FUNCTION()

    string tmp;
    int tmp_int;
        
    //
    // If loading purely from a saved file, we wont have this set yet.
    // We need it for the template lookup.
    //
    if (me->short_name == "") {
        me->get_unnamed(is, &me->short_name);
    } else {
        me->get_unnamed(is, &me->name);
    }

    //
    // If we know about this thing then load its defaults settings
    //
    me->template_thing = gh_thing::find(me->short_name,
                                        error_on_unknown_thing);

    //
    // Do we need to load the graphics for this thing? Or can we share
    // them from the template?
    //
    if (me->template_thing != NULL) {
        me->copy(me->template_thing);
    } else {
        //
        // Load the map file that tells us which tiles to use in anim
        // frames
        //
        
        //
        // Powerups and hoards have no map files - they never appear on the 
        // map
        //
        if (!string_contains(me->short_name, "powerup") &&
            !string_contains(me->short_name, "hoard")) {

            string map_file = gh_global::locate("things/" +
                                                me->short_name + ".map");

            if (map_file == "") {
                GH_THROW("no thing map file found for " + me->short_name);
            }

            me->parse(map_file, 0);

            map_file = gh_global::locate("things/" + me->short_name + ".map2",
                                         true /* quiet */);

            if (map_file != "") {
                me->parse(map_file, 1);
            }
        }
    }

    me->get_opt_class(is, &me->at, "at", gh_point3d(0, 0, 0));
    me->get_opt_class(is, &me->born, "born", gh_point3d(0, 0, 0));

    bool tmp_bool;
    bool got_one                        = true;

    bool id_set                         = false;
    bool friend_id_set                  = false;
    bool group_id_set                   = false;
    bool self_set                       = false;
    bool room_set                       = false;
    bool owner_set                      = false;
    bool carrier_set                    = false;
    bool missile_thrower_set            = false;
    bool next_set                       = false;
    bool prev_set                       = false;
    bool limb_next_set                  = false;
    bool limb_prev_set                  = false;
    bool carrying_set                   = false;
    bool visited_set                    = false;
    bool killed_by_set                  = false;
    bool strength_set                   = false;
    bool strength_req_set               = false;
    bool water_proof_set                = false;
    bool fire_proof_set                 = false;
    bool acid_proof_set                 = false;
    bool cold_proof_set                 = false;
    bool suffocation_proof_set          = false;
    bool teleport_proof_set             = false;
    bool poison_proof_set               = false;
    bool stone_proof_set                = false;
    bool dex_set                        = false;
    bool dex_req_set                    = false;
    bool lifespan_set                   = false;
    bool scared_set                     = false;
    bool sleeping_set                   = false;
    bool shielded_set                   = false;
    bool invisible_set                  = false;
    bool raging_set                     = false;
    bool poisoned_set                   = false;
    bool stoned_set                     = false;
    bool metabolism_set                 = false;
    bool stamina_set                    = false;
    bool orig_stamina_set               = false;
    bool light_strength_set             = false;
    bool can_see_distance_set           = false;
    bool missile_at_set                 = false;
    bool missile_target_set             = false;
    bool missile_angle_set              = false;
    bool missile_angle_delta_set        = false;
    bool missile_spread_set             = false;
    bool missile_spread_angle_set       = false;
    bool weapon_set                     = false;
    bool weapon_wielder_set             = false;
    bool weapon_spin_set                = false;
    bool limbs_set                      = false;
    bool join_set                       = false;
    bool target_set                     = false;
    bool direction_set                  = false;
    bool direction_wanted_set           = false;
    bool direction_last_set             = false;
    bool score_set                      = false;
    bool bravery_set                    = false;
    bool bravery_curr_set               = false;
    bool health_set                     = false;
    bool orig_health_set                = false;
    bool fate_set                       = false;
    bool intelligence_set               = false;
    bool intelligence_req_set           = false;
    bool armour_set                     = false;
    bool treasure_set                   = false;
    bool cost_set                       = false;
    bool weight_set                     = false;
    bool keys_set                       = false;
    bool anim_set                       = false;
    bool anim_speed_set                 = false;
    bool dir_set                        = false;
    bool mode_set                       = false;
    bool frame_set                      = false;
    bool bite_delay_in_tenths_set       = false;
    bool bite_chance_set                = false;
    bool bite_damage_set                = false;
    bool bite_armour_set                = false;
    bool bite_treasure_set              = false;
    bool karma_set                      = false;
    bool bite_karma_set                 = false;
    bool bite_karma_chance_set          = false;
    bool bite_intelligence_set          = false;
    bool bite_intelligence_chance_set   = false;
    bool bite_poison_set                = false;
    bool bite_poison_chance_set         = false;
    bool bite_stone_set                 = false;
    bool bite_stone_chance_set          = false;
    bool bite_fate_set                  = false;
    bool bite_fate_chance_set           = false;
    bool spawn_delay_in_tenths_set      = false;
    bool spawning_chance_set            = false;
    bool spawns_set                     = false;
    bool backpack_set                   = false;
    bool followers_set                  = false;
    bool follow_distance_set            = false;
    bool charges_set                    = false;
    bool transmogrify_chance_set        = false;
    bool transmogrifies_into_set        = false;
    bool transmog_delay_in_tenths_set   = false;
    bool teleport_chance_set            = false;
    bool teleport_delay_in_tenths_set   = false;
    bool use_chance_set                 = false;
    bool use_action_set                 = false;
    bool bite_action_set                = false;
    bool look_delay_in_tenths_set       = false;
    bool explosion_type_set             = false;
    bool audio_on_use_set               = false;
    bool audio_on_dead_set              = false;
    bool audio_on_hit_set               = false;
    bool audio_on_open_set              = false;
    bool wield_as_set                   = false;
    bool does_stuff_set                 = false;
    bool is_hero_set                    = false;
    bool cant_see_past_set              = false;
    bool collects_stuff_but_doesnt_use_it_set = false;
    bool cached_gfx_set                 = false;
    bool is_floor_set                   = false;
    bool is_dirt_set                    = false;
    bool is_wall_set                    = false;
    bool is_rock_set                    = false;
    bool is_hard_set                    = false;
    bool is_body_part_set               = false;
    bool is_chained_body_part_set       = false;
    bool stay_above_torso_set           = false;
    bool is_found_only_in_shops_set     = false;
    bool not_in_shops_set               = false;
    bool is_shopkeeper_set              = false;
    bool is_stolen_set                  = false;
    bool is_unpaid_for_set              = false;
    bool is_internal_set                = false;
    bool is_thief_set                   = false;
    bool is_explorer_set                = false;
    bool shake_screen_on_death_set      = false;
    bool is_inventory_item_set          = false;
    bool is_auto_item_set               = false;
    bool is_weapon_set                  = false;
    bool is_sword_set                   = false;
    bool is_staff_set                   = false;
    bool is_wand_set                    = false;
    bool is_axe_set                     = false;
    bool is_warhammer_set               = false;
    bool is_bow_set                     = false;
    bool is_gun_set                     = false;
    bool is_armour_set                  = false;
    bool is_amulet_set                  = false;
    bool is_cursed_set                  = false;
    bool curse_noticed_set              = false;
    bool can_see_cursed_items_set       = false;
    bool can_wield_weapons_set          = false;
    bool can_cast_spells_set            = false;
    bool can_insult_set                 = false;
    bool is_chargeable_set              = false;
    bool is_potion_set                  = false;
    bool is_spell_set                   = false;
    bool is_unable_to_move_set          = false;
    bool is_generator_set               = false;
    bool cannot_summon_set              = false;
    bool is_monst_set                   = false;
    bool is_destroyable_set             = false;
    bool is_missile_set                 = false;
    bool is_bouncy_set                  = false;
    bool is_boring_set                  = false;
    bool is_corridor_set                = false;
    bool is_teleport_set                = false;
    bool is_teleport_attack_set         = false;
    bool is_exit_set                    = false;
    bool is_return_exit_set             = false;
    bool is_door_set                    = false;
    bool is_fire_set                    = false;
    bool loves_fire_set                 = false;
    bool hates_fire_set                 = false;
    bool is_acid_set                    = false;
    bool loves_acid_set                 = false;
    bool hates_acid_set                 = false;
    bool is_water_set                   = false;
    bool loves_water_set                = false;
    bool hates_water_set                = false;
    bool is_death_set                   = false;
    bool loves_death_set                = false;
    bool is_cold_set                    = false;
    bool loves_cold_set                 = false;
    bool hates_cold_set                 = false;
    bool is_suffocation_set             = false;
    bool is_gas_set                     = false;
    bool is_meat_set                    = false;
    bool is_edible_set                  = false;
    bool is_treasure_set                = false;
    bool is_meat_eater_set              = false;
    bool is_trap_set                    = false;
    bool is_holy_set                    = false;
    bool is_undead_set                  = false;
    bool is_unique_set                  = false;
    bool is_silver_set                  = false;
    bool is_treasure_eater_set          = false;
    bool is_edible_eater_set            = false;
    bool is_joinable_set                = false;
    bool is_transparent_set             = false;
    bool destroy_joined_set             = false;
    bool is_light_source_set            = false;
    bool is_movable_set                 = false;
    bool only_in_caves_set              = false;
    bool only_in_forests_set            = false;
    bool only_in_landscapes_set         = false;
    bool only_in_dungeons_set           = false;
    bool loves_void_set                 = false;
    bool only_in_hell_set                       = false;
    bool loves_full_moon_set            = false;
    bool loves_midnight_set             = false;
    bool loves_night_set                = false;
    bool loves_friday_13th_set          = false;
    bool loves_halloween_set            = false;
    bool never_slows_down_set           = false;
    bool moves_up_down_set              = false;
    bool moves_left_right_set           = false;
    bool moves_until_collision_set      = false;
    bool leaves_corpse_set              = false;
    bool casts_lightning_set            = false;
    bool tireless_swing_set             = false;
    bool tireless_set                   = false;
    bool ability_can_regenerate1_set    = false;
    bool ability_can_regenerate2_set    = false;
    bool ability_can_regenerate3_set    = false;
    bool can_self_teleport_set          = false;
    bool can_self_teleport_once_per_level_set = false;
    bool ability_can_rage_set           = false;
    bool ability_can_auto_rage_set      = false;
    bool is_open_set                    = false;
    bool is_closed_set                  = false;
    bool is_chasing_set                 = false;
    bool is_dead_set                    = false;
    bool is_on_map_set                  = false;
    bool has_been_seen_set              = false;
    bool render_depth_set               = false;
    bool exit_jump_set                  = false;
    bool exit_name_set                  = false;
    bool cluster_radius_set             = false;
    bool cluster_amount_set             = false;
    bool laser_range_set                = false;
    bool appearing_chance_set           = false;
    bool throws_set                     = false;
    bool attack_chance_set              = false;
    bool attacks_set                    = false;
    bool wake_reasons_set               = false;
    bool follow_chance_set              = false;
    bool follows_set                    = false;
    bool level_mod_set                  = false;
    bool opacity_set                    = false;
    bool text_set                       = false;
    bool cluster_anywhere_set           = false;

    while (got_one) {
        got_one = false;

        //
        // self
        //
        if (!self_set) {
            self_set = me->get_opt(is, &me->self, "self", (void*)NULL);
            got_one |= self_set;
        }

        //
        // id
        //
        if (!id_set) {
            id_set = me->get_opt(is, &me->id, "id", 0);
            got_one |= id_set;

            if (id_set) {
                gh_thing_global_id = me->id + 100000;
            }
        }

        //
        // friend_id
        //
        if (!friend_id_set) {
            friend_id_set = me->get_opt(is, &me->friend_id, "friend_id", 0);
            got_one |= friend_id_set;
        }

        //
        // group_id
        //
        if (!group_id_set) {
            group_id_set = me->get_opt(is, &me->group_id, "group_id", 0);
            got_one |= group_id_set;
        }

        //
        // room
        //
        if (!room_set) {
            room_set = me->get_opt_classpp(is, &me->room, "room");
            got_one |= room_set;
        }

        //
        // owner
        //
        if (!owner_set) {
            owner_set = me->get_opt_classpp(is, &me->owner, "owner");
            got_one |= owner_set;
        }

        //
        // carrier
        //
        if (!carrier_set) {
            carrier_set = me->get_opt_classpp(is, &me->carrier, "carrier");
            got_one |= carrier_set;
        }

        //
        // missile_thrower
        //
        if (!missile_thrower_set) {
            missile_thrower_set =
                me->get_opt_classpp(is, &me->missile_thrower,
                                    "missile_thrower");
            got_one |= missile_thrower_set;
        }

        //
        // prev
        //
        if (!prev_set) {
            prev_set = me->get_opt_classpp(is, &me->prev, "prev");
            got_one |= prev_set;
        }

        //
        // next
        //
        if (!next_set) {
            next_set = me->get_opt_classpp(is, &me->next, "next");
            got_one |= next_set;
        }

        //
        // limb_prev
        //
        if (!limb_prev_set) {
            limb_prev_set = me->get_opt_classpp(is, &me->limb_prev, "limb_prev");
            got_one |= limb_prev_set;
        }

        //
        // limb_next
        //
        if (!limb_next_set) {
            limb_next_set = me->get_opt_classpp(is, &me->limb_next, "limb_next");
            got_one |= limb_next_set;
        }
        
        //
        // carrying
        //
        if (!carrying_set) {
            carrying_set =
                me->get_opt_classpp_container(is, &me->carrying, "carrying");
            got_one |= carrying_set;
        }

        //
        // weapon
        //
        if (!weapon_set) {
            weapon_set = me->get_opt_classpp(is, &me->weapon, "weapon");
            got_one |= weapon_set;
        }

        //
        // weapon_wielder
        //
        if (!weapon_wielder_set) {
            weapon_wielder_set =
                me->get_opt_classpp(is, &me->weapon_wielder, "weapon_wielder");
            got_one |= weapon_wielder_set;
        }

        //
        // visited
        //
        if (!visited_set) {
            visited_set =
                me->get_opt_classpp_container(is, &me->visited, "visited");
            got_one |= visited_set;
        }
        
        //
        // killed_by
        //
        if (!killed_by_set) {
            killed_by_set = me->get_opt(is, &me->killed_by, "killed_by");
            got_one |= killed_by_set;
        }

        //
        // strength
        //
        if (!strength_set) {
            strength_set = me->get_opt(is, &me->_strength, "strength");
            got_one |= strength_set;
        }

        //
        // strength_req
        //
        if (!strength_req_set) {
            strength_req_set =
                me->get_opt(is, &me->strength_req, "strength_req");

            got_one |= strength_req_set;
        }

        //
        // water_proof
        //
        if (!water_proof_set) {
            water_proof_set = me->get_opt(is, &me->_water_proof, "water_proof");
            got_one |= water_proof_set;
        }

        //
        // fire_proof
        //
        if (!fire_proof_set) {
            fire_proof_set = me->get_opt(is, &me->_fire_proof, "fire_proof");
            got_one |= fire_proof_set;
        }

        //
        // acid_proof
        //
        if (!acid_proof_set) {
            acid_proof_set = me->get_opt(is, &me->_acid_proof, "acid_proof");
            got_one |= acid_proof_set;
        }

        //
        // cold_proof
        //
        if (!cold_proof_set) {
            cold_proof_set = me->get_opt(is, &me->_cold_proof, "cold_proof");
            got_one |= cold_proof_set;
        }

        //
        // suffocation_proof
        //
        if (!suffocation_proof_set) {
            suffocation_proof_set = me->get_opt(is, &me->_suffocation_proof, "suffocation_proof");
            got_one |= suffocation_proof_set;
        }

        //
        // teleport_proof
        //
        if (!teleport_proof_set) {
            teleport_proof_set = me->get_opt(is, &me->_teleport_proof, "teleport_proof");
            got_one |= teleport_proof_set;
        }

        //
        // poison_proof
        //
        if (!poison_proof_set) {
            poison_proof_set = me->get_opt(is, &me->_poison_proof, "poison_proof");
            got_one |= poison_proof_set;
        }

        //
        // stone_proof
        //
        if (!stone_proof_set) {
            stone_proof_set = me->get_opt(is, &me->_stone_proof, "stone_proof");
            got_one |= stone_proof_set;
        }

        //
        // dex
        //
        if (!dex_set) {
            dex_set = me->get_opt(is, &me->_dex, "dex");
            got_one |= dex_set;
        }

        //
        // dex_req
        //
        if (!dex_req_set) {
            dex_req_set =
                me->get_opt(is, &me->dex_req, "dex_req");

            got_one |= dex_req_set;
        }

        //
        // lifespan
        //
        if (!lifespan_set) {
            lifespan_set = me->get_opt(is, &me->lifespan, "lifespan");
            got_one |= lifespan_set;
        }

        //
        // scared
        //
        if (!scared_set) {
            scared_set = me->get_opt(is, &me->_scared, "scared");
            got_one |= scared_set;
        }

        //
        // sleeping
        //
        if (!sleeping_set) {
            sleeping_set = me->get_opt(is, &me->_sleeping, "sleeping");
            got_one |= sleeping_set;
        }

        //
        // shielded
        //
        if (!shielded_set) {
            shielded_set = me->get_opt(is, &me->_shielded, "shielded");
            got_one |= shielded_set;
        }

        //
        // invisible
        //
        if (!invisible_set) {
            invisible_set = me->get_opt(is, &me->_invisible, "invisible");
            got_one |= invisible_set;
        }

        //
        // raging
        //
        if (!raging_set) {
            raging_set = me->get_opt(is, &me->_raging, "raging");
            got_one |= raging_set;
        }

        //
        // poisoned
        //
        if (!poisoned_set) {
            poisoned_set = me->get_opt(is, &me->_poisoned, "poisoned");
            got_one |= poisoned_set;
        }

        //
        // stoned
        //
        if (!stoned_set) {
            stoned_set = me->get_opt(is, &me->_stoned, "stoned");
            got_one |= stoned_set;
        }

        //
        // metabolism
        //
        if (!metabolism_set) {
            metabolism_set = me->get_opt(is, &me->_metabolism, "metabolism");
            got_one |= metabolism_set;
        }

        //
        // stamina
        //
        if (!stamina_set) {
            stamina_set = me->get_opt(is, &me->_stamina, "stamina");
            got_one |= stamina_set;
        }

        //
        // orig_stamina
        //
        if (!orig_stamina_set) {
            orig_stamina_set = me->get_opt(is, &me->orig_stamina, "orig_stamina");
            got_one |= orig_stamina_set;
        }

        //
        // light_strength
        //
        if (!light_strength_set) {
            light_strength_set = me->get_opt(is, &me->_light_strength,"light_strength");
            got_one |= light_strength_set;
        }

        //
        // can_see_distance
        //
        if (!can_see_distance_set) {
            can_see_distance_set = me->get_opt(is, &me->_can_see_distance,
                                               "can_see_distance");
            got_one |= can_see_distance_set;
        }

        //
        // missile_at
        //
        if (!missile_at_set) {
            missile_at_set = me->get_opt_class(is, &me->missile_at, "missile_at");
            got_one |= missile_at_set;
        }

        //
        // missile_target
        //
        if (!missile_target_set) {
            missile_target_set = me->get_opt_class(is, &me->missile_target,
                                                   "missile_target");
            got_one |= missile_target_set;
        }

        //
        // missile_angle
        //
        if (!missile_angle_set) {
            missile_angle_set = me->get_opt(is, &me->missile_angle,
                                            "missile_angle");
            got_one |= missile_angle_set;
        }

        //
        // missile_angle_delta
        //
        if (!missile_angle_delta_set) {
            missile_angle_delta_set = me->get_opt(is, &me->missile_angle_delta,
                                                  "missile_angle_delta");
            got_one |= missile_angle_delta_set;
        }

        //
        // missile_spread
        //
        if (!missile_spread_set) {
            missile_spread_set = me->get_opt(is, &me->missile_spread,
                                             "missile_spread");
            got_one |= missile_spread_set;
        }

        //
        // missile_spread_angle
        //
        if (!missile_spread_angle_set) {
            missile_spread_angle_set = me->get_opt(is, &me->missile_spread_angle,
                                                   "missile_spread_angle");
            got_one |= missile_spread_angle_set;
        }

        //
        // weapon_spin
        //
        if (!weapon_spin_set) {
            weapon_spin_set = me->get_opt(is, &me->weapon_spin, "weapon_spin");
            got_one |= weapon_spin_set;
        }

        //
        // limbs
        //
        if (!limbs_set) {
            limbs_set = me->get_opt(is, &me->limbs, "limbs");
            got_one |= limbs_set;
        }

        //
        // join
        //
        if (!join_set) {
            join_set = me->get_opt(is, &tmp_int, "join");
            if (join_set) {
                me->join = (gh_thing_dir)tmp_int;
            }

            got_one |= join_set;
        }

        //
        // target
        //
        if (!target_set) {
            target_set = me->get_opt_class(is, &me->target, "target");
            got_one |= target_set;
        }

        //
        // direction
        //
        if (!direction_set) {
            direction_set = me->get_opt_class(is, &me->direction, "direction");
            got_one |= direction_set;
        }

        //
        // direction_wanted
        //
        if (!direction_wanted_set) {
            direction_wanted_set = me->get_opt_class(is, &me->direction_wanted,
                                                     "direction_wanted");
            got_one |= direction_wanted_set;
        }

        //
        // direction_last
        //
        if (!direction_last_set) {
            direction_last_set = me->get_opt_class(is, &me->direction_last,
                                                   "direction_last");
            got_one |= direction_last_set;
        }

        //
        // score
        //
        if (!score_set) {
            score_set = me->get_opt(is, &me->score, "score");
            got_one |= score_set;
        }

        //
        // bravery
        //
        if (!bravery_set) {
            bravery_set = me->get_opt(is, &me->bravery, "bravery");
            got_one |= bravery_set;
        }

        //
        // bravery_curr
        //
        if (!bravery_curr_set) {
            bravery_curr_set = me->get_opt(is, &me->bravery_curr, "bravery_curr");
            got_one |= bravery_curr_set;
        }

        //
        // health
        //
        if (!health_set) {
            health_set = me->get_opt(is, &me->health, "health");
            got_one |= health_set;
        }

        //
        // orig_health
        //
        if (!orig_health_set) {
            orig_health_set = me->get_opt(is, &me->orig_health, "orig_health");
            got_one |= orig_health_set;
        }

        //
        // fate
        //
        if (!fate_set) {
            fate_set = me->get_opt(is, &me->fate, "fate");
            got_one |= fate_set;
        }

        //
        // intelligence
        //
        if (!intelligence_set) {
            intelligence_set = me->get_opt(is, &me->intelligence, "intelligence");
            got_one |= intelligence_set;
        }

        //
        // intelligence_req
        //
        if (!intelligence_req_set) {
            intelligence_req_set =
                me->get_opt(is, &me->intelligence_req, "intelligence_req");

            got_one |= intelligence_req_set;
        }

        //
        // armour
        //
        if (!armour_set) {
            armour_set = me->get_opt(is, &me->armour, "armour");
            got_one |= armour_set;
        }

        //
        // treasure
        //
        if (!treasure_set) {
            treasure_set = me->get_opt(is, &me->treasure, "treasure");
            got_one |= treasure_set;
        }

        //
        // cost
        //
        if (!cost_set) {
            cost_set = me->get_opt(is, &me->cost, "cost");
            got_one |= cost_set;
        }

        //
        // weight
        //
        if (!weight_set) {
            weight_set = me->get_opt(is, &me->weight, "weight");
            got_one |= weight_set;
        }

        //
        // keys
        //
        if (!keys_set) {
            keys_set = me->get_opt(is, &me->keys, "keys");
            got_one |= keys_set;
        }

        //
        // anim
        //
        if (!anim_set) {
            anim_set = me->get_opt(is, &me->anim, "anim");

            got_one |= anim_set;
        }

        //
        // anim_speed
        //
        if (!anim_speed_set) {
            anim_speed_set =
                me->get_opt(is, &me->anim_speed, "anim_speed");

            got_one |= anim_speed_set;
        }

        //
        // dir
        //
        if (!dir_set) {
            dir_set =
                me->get_opt(is, &tmp_int, "dir");
            if (dir_set) {
                me->dir = (gh_thing_dir)tmp_int;
            }

            got_one |= dir_set;
        }

        //
        // mode
        //
        if (!mode_set) {
            mode_set = me->get_opt(is, &tmp_int, "mode");
            if (mode_set) {
                me->mode = (gh_thing_mode)tmp_int;
            }

            got_one |= mode_set;
        }

        //
        // frame
        //
        if (!frame_set) {
            frame_set =
                me->get_opt(is, &me->frame, "frame");
            got_one |= frame_set;
        }

        //
        // bite_damage
        //
        if (!bite_damage_set) {
            bite_damage_set = me->get_opt(is, &me->bite_damage, "bite_damage");
            got_one |= bite_damage_set;
        }

        //
        // bite_armour
        //
        if (!bite_armour_set) {
            bite_armour_set = me->get_opt(is, &me->bite_armour, "bite_armour");
            got_one |= bite_armour_set;
        }

        //
        // bite_treasure
        //
        if (!bite_treasure_set) {
            bite_treasure_set = me->get_opt(is, &me->bite_treasure,
                                            "bite_treasure");
            got_one |= bite_treasure_set;
        }

        //
        // bite_delay_in_tenths
        //
        if (!bite_delay_in_tenths_set) {
            bite_delay_in_tenths_set = me->get_opt(is, &me->bite_delay_in_tenths,
                                                   "bite_delay_in_tenths");
            got_one |= bite_delay_in_tenths_set;
        }

        //
        // bite_chance
        //
        if (!bite_chance_set) {
            bite_chance_set = me->get_opt(is, &me->bite_chance, "bite_chance");
            got_one |= bite_chance_set;
        }

        //
        // bite_karma
        //
        if (!bite_karma_set) {
            bite_karma_set = me->get_opt(is, &me->bite_karma, "bite_karma");
            got_one |= bite_karma_set;
        }

        //
        // bite_karma_chance
        //
        if (!bite_karma_chance_set) {
            bite_karma_chance_set = me->get_opt(is, &me->bite_karma_chance,
                                                "bite_karma_chance");
            got_one |= bite_karma_chance_set;
        }

        //
        // bite_intelligence
        //
        if (!bite_intelligence_set) {
            bite_intelligence_set = me->get_opt(is, &me->bite_intelligence, "bite_intelligence");
            got_one |= bite_intelligence_set;
        }

        //
        // bite_intelligence_chance
        //
        if (!bite_intelligence_chance_set) {
            bite_intelligence_chance_set = me->get_opt(is, &me->bite_intelligence_chance,
                                                "bite_intelligence_chance");
            got_one |= bite_intelligence_chance_set;
        }

        //
        // bite_poison
        //
        if (!bite_poison_set) {
            bite_poison_set = me->get_opt(is, &me->bite_poison, "bite_poison");
            got_one |= bite_poison_set;
        }

        //
        // bite_poison_chance
        //
        if (!bite_poison_chance_set) {
            bite_poison_chance_set = me->get_opt(is, &me->bite_poison_chance,
                                                 "bite_poison_chance");
            got_one |= bite_poison_chance_set;
        }

        //
        // bite_stone
        //
        if (!bite_stone_set) {
            bite_stone_set = me->get_opt(is, &me->bite_stone, "bite_stone");
            got_one |= bite_stone_set;
        }

        //
        // bite_stone_chance
        //
        if (!bite_stone_chance_set) {
            bite_stone_chance_set = me->get_opt(is, &me->bite_stone_chance,
                                                 "bite_stone_chance");
            got_one |= bite_stone_chance_set;
        }

        //
        // bite_fate
        //
        if (!bite_fate_set) {
            bite_fate_set = me->get_opt(is, &me->bite_fate, "bite_fate");
            got_one |= bite_fate_set;
        }

        //
        // bite_fate_chance
        //
        if (!bite_fate_chance_set) {
            bite_fate_chance_set = me->get_opt(is, &me->bite_fate_chance,
                                               "bite_fate_chance");
            got_one |= bite_fate_chance_set;
        }

        //
        // karma
        //
        if (!karma_set) {
            karma_set = me->get_opt(is, &me->karma, "karma");
            got_one |= karma_set;
        }

        //
        // spawn_delay_in_tenths
        //
        if (!spawn_delay_in_tenths_set) {
            spawn_delay_in_tenths_set = me->get_opt(is,
                                                &me->spawn_delay_in_tenths,
                                                "spawn_delay_in_tenths");
            got_one |= spawn_delay_in_tenths_set;
        }

        //
        // spawning_chance
        //
        if (!spawning_chance_set) {
            vector<int> a;
            spawning_chance_set = me->get_opt_container(is, &a, "spawning_chance");
            if (a.size()) {
                me->spawning_chance = a; // erase the old list by assignment
            }

            got_one |= spawning_chance_set;
        }

        //
        // spawns
        //
        if (!spawns_set) {
            vector<string> a;
            spawns_set = me->get_opt_container(is, &a, "spawns");
            if (a.size()) {
                me->spawns = a; // erase the old list by assignment
            }

            got_one |= spawns_set;
        }

        //
        // backpack
        //
        if (!backpack_set) {
            vector<string> a;
            backpack_set = me->get_opt_container(is, &a, "backpack");
            if (a.size()) {
                me->backpack = a; // erase the old list by assignment
            }

            got_one |= backpack_set;
        }

        //
        // followers
        //
        if (!followers_set) {
            vector<string> a;
            followers_set = me->get_opt_container(is, &a, "followers");
            if (a.size()) {
                me->followers = a; // erase the old list by assignment
            }

            got_one |= followers_set;
        }

        //
        // follow_distance
        //
        if (!follow_distance_set) {
            follow_distance_set = me->get_opt(is, &me->follow_distance,
                                                "follow_distance");
            got_one |= follow_distance_set;
        }

        //
        // transmog_delay_in_tenths
        //
        if (!transmog_delay_in_tenths_set) {
            transmog_delay_in_tenths_set = me->get_opt(is,
                                                &me->transmog_delay_in_tenths,
                                                "transmog_delay_in_tenths");
            got_one |= transmog_delay_in_tenths_set;
        }

        //
        // transmogrify_chance
        //
        if (!transmogrify_chance_set) {
            vector<int> a;
            transmogrify_chance_set = me->get_opt_container(is, &a, "transmogrify_chance");
            if (a.size()) {
                me->transmogrify_chance = a; // erase the old list by assignment
            }

            got_one |= transmogrify_chance_set;
        }

        //
        // transmogrifies_into
        //
        if (!transmogrifies_into_set) {
            vector<string> a;
            if (a.size()) {
                me->transmogrifies_into = a; // erase the old list by assignment
            }
            transmogrifies_into_set = me->get_opt_container(is, &a, "transmogrifies_into");
            if (a.size()) {
                me->transmogrifies_into = a; // erase the old list by assignment
            }

            got_one |= transmogrifies_into_set;
        }

        //
        // teleport_delay_in_tenths
        //
        if (!teleport_delay_in_tenths_set) {
            teleport_delay_in_tenths_set = me->get_opt(is,
                                                &me->teleport_delay_in_tenths,
                                                "teleport_delay_in_tenths");
            got_one |= teleport_delay_in_tenths_set;
        }

        //
        // teleport_chance
        //
        if (!teleport_chance_set) {
            teleport_chance_set = me->get_opt(is, &me->teleport_chance,
                                      "teleport_chance");

            got_one |= teleport_chance_set;
        }

        //
        // use_chance
        //
        if (!use_chance_set) {
            vector<int> a;
            use_chance_set = me->get_opt_container(is, &a, "use_chance");
            if (a.size()) {
                me->use_chance = a; // erase the old list by assignment
            }

            got_one |= use_chance_set;
        }

        //
        // use_action
        //
        if (!use_action_set) {
            vector<string> a;
            use_action_set = me->get_opt_container(is, &a, "use_action");
            if (a.size()) {
                me->use_action = a; // erase the old list by assignment
            }

            got_one |= use_action_set;
        }

        //
        // bite_action
        //
        if (!bite_action_set) {
            vector<string> a;
            bite_action_set = me->get_opt_container(is, &a, "bite_action");
            if (a.size()) {
                me->bite_action = a; // erase the old list by assignment
            }

            got_one |= bite_action_set;
        }

        //
        // charges
        //
        if (!charges_set) {
            charges_set = me->get_opt(is, &me->charges, "charges");
            got_one |= charges_set;
        }

        //
        // look_delay_in_tenths
        //
        if (!look_delay_in_tenths_set) {
            look_delay_in_tenths_set = me->get_opt(is,
                                                &me->look_delay_in_tenths,
                                                "look_delay_in_tenths");
            got_one |= look_delay_in_tenths_set;
        }

        //
        // explosion_type
        //
        if (!explosion_type_set) {
            explosion_type_set = me->get_opt(is, &me->explosion_type,
                                             "explosion_type");
            got_one |= explosion_type_set;
        }

        //
        // audio_on_use
        //
        if (!audio_on_use_set) {
            audio_on_use_set = me->get_opt(is, &me->audio_on_use,
                                             "audio_on_use");
            got_one |= audio_on_use_set;
        }

        //
        // audio_on_dead
        //
        if (!audio_on_dead_set) {
            audio_on_dead_set = me->get_opt(is, &me->audio_on_dead,
                                             "audio_on_dead");
            got_one |= audio_on_dead_set;
        }

        //
        // audio_on_hit
        //
        if (!audio_on_hit_set) {
            audio_on_hit_set = me->get_opt(is, &me->audio_on_hit,
                                             "audio_on_hit");
            got_one |= audio_on_hit_set;
        }

        //
        // audio_on_open
        //
        if (!audio_on_open_set) {
            audio_on_open_set = me->get_opt(is, &me->audio_on_open,
                                             "audio_on_open");
            got_one |= audio_on_open_set;
        }

        //
        // wield_as
        //
        if (!wield_as_set) {
            wield_as_set = me->get_opt(is, &me->wield_as, "wield_as");
            got_one |= wield_as_set;
        }

        //
        // is_hero
        //
        if (!is_hero_set) {
            is_hero_set =
                me->get_opt_named_flag(is, &tmp_bool, "is_hero",
                    (bool) (me->template_thing ?
                        me->template_thing->is_hero : false));

            got_one |= is_hero_set;
            me->is_hero = tmp_bool;
        }

#define GET_OPT_NAMED_FLAG(__FLAG__)                                    \
        if (!__FLAG__ ## _set) {                                        \
            __FLAG__ ## _set =                                          \
                me->get_opt_named_flag(                                 \
                       is,                                              \
                       &tmp_bool,                                       \
                       #__FLAG__,                                       \
                       (bool) (me->template_thing ?                     \
                               me->template_thing->__FLAG__ : false));  \
                                                                        \
            got_one |= __FLAG__ ## _set;                                \
            me->__FLAG__ = tmp_bool;                                    \
        }                                                               \

        GET_OPT_NAMED_FLAG(cant_see_past)
        GET_OPT_NAMED_FLAG(collects_stuff_but_doesnt_use_it)
        GET_OPT_NAMED_FLAG(does_stuff)
        GET_OPT_NAMED_FLAG(cached_gfx)
        GET_OPT_NAMED_FLAG(is_floor)
        GET_OPT_NAMED_FLAG(is_dirt)
        GET_OPT_NAMED_FLAG(is_wall)
        GET_OPT_NAMED_FLAG(is_rock)
        GET_OPT_NAMED_FLAG(is_hard)
        GET_OPT_NAMED_FLAG(is_body_part)
        GET_OPT_NAMED_FLAG(is_chained_body_part)
        GET_OPT_NAMED_FLAG(stay_above_torso)
        GET_OPT_NAMED_FLAG(is_found_only_in_shops)
        GET_OPT_NAMED_FLAG(not_in_shops)
        GET_OPT_NAMED_FLAG(is_shopkeeper)
        GET_OPT_NAMED_FLAG(is_stolen)
        GET_OPT_NAMED_FLAG(is_unpaid_for)
        GET_OPT_NAMED_FLAG(is_internal)
        GET_OPT_NAMED_FLAG(is_thief)
        GET_OPT_NAMED_FLAG(is_explorer)
        GET_OPT_NAMED_FLAG(shake_screen_on_death)
        GET_OPT_NAMED_FLAG(is_inventory_item)
        GET_OPT_NAMED_FLAG(is_auto_item)
        GET_OPT_NAMED_FLAG(is_weapon)
        GET_OPT_NAMED_FLAG(is_sword)
        GET_OPT_NAMED_FLAG(is_staff)
        GET_OPT_NAMED_FLAG(is_wand)
        GET_OPT_NAMED_FLAG(is_axe)
        GET_OPT_NAMED_FLAG(is_warhammer)
        GET_OPT_NAMED_FLAG(is_bow)
        GET_OPT_NAMED_FLAG(is_gun)
        GET_OPT_NAMED_FLAG(is_armour)
        GET_OPT_NAMED_FLAG(is_amulet)
        GET_OPT_NAMED_FLAG(is_cursed)
        GET_OPT_NAMED_FLAG(curse_noticed)
        GET_OPT_NAMED_FLAG(can_see_cursed_items)
        GET_OPT_NAMED_FLAG(can_wield_weapons)
        GET_OPT_NAMED_FLAG(can_cast_spells)
        GET_OPT_NAMED_FLAG(can_insult)
        GET_OPT_NAMED_FLAG(is_chargeable)
        GET_OPT_NAMED_FLAG(is_potion)
        GET_OPT_NAMED_FLAG(is_spell)
        GET_OPT_NAMED_FLAG(is_unable_to_move)
        GET_OPT_NAMED_FLAG(is_generator)
        GET_OPT_NAMED_FLAG(cannot_summon)
        GET_OPT_NAMED_FLAG(is_monst)
        GET_OPT_NAMED_FLAG(is_destroyable)
        GET_OPT_NAMED_FLAG(is_missile)
        GET_OPT_NAMED_FLAG(is_bouncy)
        GET_OPT_NAMED_FLAG(is_boring)
        GET_OPT_NAMED_FLAG(is_corridor)
        GET_OPT_NAMED_FLAG(is_teleport)
        GET_OPT_NAMED_FLAG(is_teleport_attack)
        GET_OPT_NAMED_FLAG(is_exit)
        GET_OPT_NAMED_FLAG(is_return_exit)
        GET_OPT_NAMED_FLAG(is_door)
        GET_OPT_NAMED_FLAG(is_fire)
        GET_OPT_NAMED_FLAG(hates_fire)
        GET_OPT_NAMED_FLAG(loves_fire)
        GET_OPT_NAMED_FLAG(is_acid)
        GET_OPT_NAMED_FLAG(hates_acid)
        GET_OPT_NAMED_FLAG(loves_acid)
        GET_OPT_NAMED_FLAG(is_water)
        GET_OPT_NAMED_FLAG(hates_water)
        GET_OPT_NAMED_FLAG(loves_water)
        GET_OPT_NAMED_FLAG(is_death)
        GET_OPT_NAMED_FLAG(loves_death)
        GET_OPT_NAMED_FLAG(is_cold)
        GET_OPT_NAMED_FLAG(hates_cold)
        GET_OPT_NAMED_FLAG(loves_cold)
        GET_OPT_NAMED_FLAG(is_suffocation)
        GET_OPT_NAMED_FLAG(is_meat)
        GET_OPT_NAMED_FLAG(is_gas)
        GET_OPT_NAMED_FLAG(is_edible)
        GET_OPT_NAMED_FLAG(is_treasure)
        GET_OPT_NAMED_FLAG(is_meat_eater)
        GET_OPT_NAMED_FLAG(is_trap)
        GET_OPT_NAMED_FLAG(is_holy)
        GET_OPT_NAMED_FLAG(is_undead)
        GET_OPT_NAMED_FLAG(is_unique)
        GET_OPT_NAMED_FLAG(is_silver)
        GET_OPT_NAMED_FLAG(is_treasure_eater)
        GET_OPT_NAMED_FLAG(is_edible_eater)
        GET_OPT_NAMED_FLAG(is_joinable)
        GET_OPT_NAMED_FLAG(is_transparent)
        GET_OPT_NAMED_FLAG(destroy_joined)
        GET_OPT_NAMED_FLAG(is_light_source)
        GET_OPT_NAMED_FLAG(is_movable)
        GET_OPT_NAMED_FLAG(only_in_caves);
        GET_OPT_NAMED_FLAG(only_in_forests);
        GET_OPT_NAMED_FLAG(only_in_landscapes);
        GET_OPT_NAMED_FLAG(only_in_dungeons);
        GET_OPT_NAMED_FLAG(loves_void);
        GET_OPT_NAMED_FLAG(only_in_hell);
        GET_OPT_NAMED_FLAG(loves_full_moon);
        GET_OPT_NAMED_FLAG(loves_midnight);
        GET_OPT_NAMED_FLAG(loves_night);
        GET_OPT_NAMED_FLAG(loves_friday_13th);
        GET_OPT_NAMED_FLAG(loves_halloween);
        GET_OPT_NAMED_FLAG(never_slows_down);
        GET_OPT_NAMED_FLAG(moves_up_down);
        GET_OPT_NAMED_FLAG(moves_left_right);
        GET_OPT_NAMED_FLAG(moves_until_collision);
        GET_OPT_NAMED_FLAG(leaves_corpse);
        GET_OPT_NAMED_FLAG(casts_lightning);
        GET_OPT_NAMED_FLAG(tireless_swing);
        GET_OPT_NAMED_FLAG(tireless);
        GET_OPT_NAMED_FLAG(ability_can_regenerate1);
        GET_OPT_NAMED_FLAG(ability_can_regenerate2);
        GET_OPT_NAMED_FLAG(ability_can_regenerate3);
        GET_OPT_NAMED_FLAG(can_self_teleport);
        GET_OPT_NAMED_FLAG(can_self_teleport_once_per_level);
        GET_OPT_NAMED_FLAG(ability_can_rage);
        GET_OPT_NAMED_FLAG(ability_can_auto_rage);
        GET_OPT_NAMED_FLAG(is_open)
        GET_OPT_NAMED_FLAG(is_closed)
        GET_OPT_NAMED_FLAG(is_chasing)
        GET_OPT_NAMED_FLAG(is_dead)
        GET_OPT_NAMED_FLAG(is_on_map)
        GET_OPT_NAMED_FLAG(has_been_seen)
        GET_OPT_NAMED_FLAG(cluster_anywhere)

        //
        // render_depth
        //
        if (!render_depth_set) {
            render_depth_set =
                me->get_opt(is, &me->render_depth, "render_depth");

            got_one |= render_depth_set;
        }

        //
        // exit_jump
        //
        if (!exit_jump_set) {
            exit_jump_set =
                me->get_opt(is, &me->exit_jump, "exit_jump");

            got_one |= exit_jump_set;
        }

        //
        // exit_name
        //
        if (!exit_name_set) {
            exit_name_set =
                me->get_opt(is, &me->exit_name, "exit_name");

            got_one |= exit_name_set;
        }

        //
        // cluster_radius
        //
        if (!cluster_radius_set) {
            cluster_radius_set =
                me->get_opt(is, &me->cluster_radius, "cluster_radius");

            got_one |= cluster_radius_set;
        }

        //
        // cluster_amount
        //
        if (!cluster_amount_set) {
            cluster_amount_set =
                me->get_opt(is, &me->cluster_amount, "cluster_amount");

            got_one |= cluster_amount_set;
        }

        //
        // laser_range
        //
        if (!laser_range_set) {
            laser_range_set =
                me->get_opt(is, &me->laser_range, "laser_range");

            got_one |= laser_range_set;
        }

        //
        // appearing_chance
        //
        if (!appearing_chance_set) {
            vector<int> a;
            appearing_chance_set = me->get_opt_container(is, &a, "appearing_chance");
            if (a.size()) {
                me->appearing_chance = a; // erase the old list by assignment
            }

            got_one |= appearing_chance_set;
        }

        //
        // attack_chance
        //
        if (!attack_chance_set) {
            vector<int> a;
            attack_chance_set = me->get_opt_container(is, &a, "attack_chance");
            if (a.size()) {
                me->attack_chance = a; // erase the old list by assignment
            }

            got_one |= attack_chance_set;
        }

        //
        // attacks
        //
        if (!attacks_set) {
            vector<string> a;
            attacks_set = me->get_opt_container(is, &a, "attacks");
            if (a.size()) {
                me->attacks = a; // erase the old list by assignment
            }

            got_one |= attacks_set;
        }

        //
        // wake_reasons
        //
        if (!wake_reasons_set) {
            vector<string> a;
            wake_reasons_set = me->get_opt_container(is, &a, "wake_reasons");
            if (a.size()) {
                me->wake_reasons = a; // erase the old list by assignment
            }

            got_one |= wake_reasons_set;
        }

        //
        // follow_chance
        //
        if (!follow_chance_set) {
            vector<int> a;
            follow_chance_set = me->get_opt_container(is, &a, "follow_chance");
            if (a.size()) {
                me->follow_chance = a; // erase the old list by assignment
            }

            got_one |= follow_chance_set;
        }

        //
        // follows
        //
        if (!follows_set) {
            vector<string> a;
            follows_set = me->get_opt_container(is, &a, "follows");
            if (a.size()) {
                me->follows = a; // erase the old list by assignment
            }

            got_one |= follows_set;
        }

        //
        // throws
        //
        if (!throws_set) {
            vector<string> a;
            throws_set = me->get_opt_container(is, &a, "throws");
            if (a.size()) {
                me->throws = a; // erase the old list by assignment
            }

            got_one |= throws_set;
        }

        //
        // opacity
        //
        if (!opacity_set) {
            opacity_set = me->get_opt(is, &me->opacity, "opacity");

            got_one |= opacity_set;
        }

        //
        // level_mod
        //
        if (!level_mod_set) {
            level_mod_set = me->get_opt(is, &me->level_mod, "level_mod");

            got_one |= level_mod_set;
        }

        //
        // text
        //
        if (!text_set) {
            text_set = me->get_opt(is, &me->text, "text");

            got_one |= text_set;
        }

        if (is.node->type == gh_node::GH_NODE_KET) {
            goto end;

        }
        if (is.node->type == gh_node::GH_NODE_EOF) {
            goto end;
        }

        if (!got_one) {
            me->get(is, &tmp_bool, "unknown-field");
        }
    }
end:
    if (me->is_monst) {
        if (!opacity_set) {
            if (!me->opacity) {
                me->opacity = 2;
            }
        }
    }

    if (!me->anim_speed) {
        me->anim_speed = gh_global::anim_speed;
    }

    //
    // Modify the new born thing for the current time of day.
    //
    vector<int>::iterator c =  me->appearing_chance.begin();
    int ci = 0;
    while (c != me->appearing_chance.end()) {

        if (me->loves_full_moon) {
            if (phase_of_the_moon() == GH_FULL_MOON) {
                me->appearing_chance[ci] *= 2;
            }
        }

        if (me->loves_midnight) {
            if (midnight()) {
                me->appearing_chance[ci] *= 2;
            }
        }

        if (me->loves_friday_13th) {
            if (friday_13th()) {
                me->appearing_chance[ci] *= 2;
            }
        }

        if (me->loves_halloween) {
            if (halloween()) {
                me->appearing_chance[ci] *= 2;
            }
        }

        if (me->loves_night) {
            if (night()) {
                me->appearing_chance[ci] *= 2;
            }
        }

        ci++;
        c++;
    }

    //
    // Finished loading this thing (from a saved game possibly).
    //
    me->is_saved_thing = false;

    //
    // Need to restore the pointers for this thing.
    //
    if (me->template_thing != NULL) {
        me->needs_fixup = true;
    }
}

void operator<< (gh_stream_out& os, const gh_thing *me)
{
    GH_FUNCTION()

    bool tmp_bool;
    int tmp_int;

    me->put_unnamed(os, me->short_name.c_str());

    me->put_class(os, &me->at, "at");
    me->put_class(os, &me->born, "born");

    //
    // self
    //
    me->put(os, &me->self, "self");

    //
    // id
    //
    me->put(os, &me->id, "id");

    //
    // friend_id
    //
    me->put_opt(os, &me->friend_id, "friend_id", 0);

    //
    // group_id
    //
    me->put_opt(os, &me->group_id, "group_id", 0);

    //
    // room
    //
    me->put_opt_classpp(os, &me->room, "room");

    //
    // owner
    //
    me->put_opt_classpp(os, &me->owner, "owner");

    //
    // carrier
    //
    me->put_opt_classpp(os, &me->carrier, "carrier");

    //
    // missile_thrower
    //
    me->put_opt_classpp(os, &me->missile_thrower, "missile_thrower");

    //
    // next
    //
    me->put_opt_classpp(os, &me->next, "next");

    //
    // prev
    //
    me->put_opt_classpp(os, &me->prev, "prev");

    //
    // limb_next
    //
    me->put_opt_classpp(os, &me->limb_next, "limb_next");

    //
    // limb_prev
    //
    me->put_opt_classpp(os, &me->limb_prev, "limb_prev");

    //
    // carrying
    //
    me->put_classpp_container(os, &me->carrying, "carrying");

    //
    // visited
    //
    me->put_classpp_container(os, &me->visited, "visited");

    if (!me->template_thing) {
        return;
    }

    //
    // killed_by
    //
    if (me->killed_by != me->template_thing->killed_by) {
        me->put_opt(os, &me->killed_by, "killed_by",
                    (decltype(me->killed_by))"");
    }

    //
    // strength
    //
    if (me->_strength != me->template_thing->_strength) {
        me->put_opt(os, &me->_strength, "strength",
                    (decltype(me->_strength))0);
    }

    //
    // strength_req
    //
    if (me->strength_req != me->template_thing->strength_req) {
        me->put_opt(os, &me->strength_req, "strength_req",
                    (decltype(me->strength_req))0);
    }

    //
    // water_proof
    //
    if (me->_water_proof != me->template_thing->_water_proof) {
        me->put_opt(os, &me->_water_proof, "water_proof",
                    (decltype(me->_water_proof))0);
    }

    //
    // fire_proof
    //
    if (me->_fire_proof != me->template_thing->_fire_proof) {
        me->put_opt(os, &me->_fire_proof, "fire_proof",
                    (decltype(me->_fire_proof))0);
    }

    //
    // acid_proof
    //
    if (me->_acid_proof != me->template_thing->_acid_proof) {
        me->put_opt(os, &me->_acid_proof, "acid_proof",
                    (decltype(me->_acid_proof))0);
    }

    //
    // cold_proof
    //
    if (me->_cold_proof != me->template_thing->_cold_proof) {
        me->put_opt(os, &me->_cold_proof, "cold_proof",
                    (decltype(me->_cold_proof))0);
    }

    //
    // suffocation_proof
    //
    if (me->_suffocation_proof != me->template_thing->_suffocation_proof) {
        me->put_opt(os, &me->_suffocation_proof, "suffocation_proof",
                    (decltype(me->_suffocation_proof))0);
    }

    //
    // teleport_proof
    //
    if (me->_teleport_proof != me->template_thing->_teleport_proof) {
        me->put_opt(os, &me->_teleport_proof, "teleport_proof",
                    (decltype(me->_teleport_proof))0);
    }

    //
    // poison_proof
    //
    if (me->_poison_proof != me->template_thing->_poison_proof) {
        me->put_opt(os, &me->_poison_proof, "poison_proof",
                    (decltype(me->_poison_proof))0);
    }

    //
    // dex
    //
    if (me->_dex != me->template_thing->_dex) {
        me->put_opt(os, &me->_dex, "dex",
                    (decltype(me->_dex))0);
    }

    //
    // dex_req
    //
    if (me->dex_req != me->template_thing->dex_req) {
        me->put_opt(os, &me->dex_req, "dex_req",
                    (decltype(me->dex_req))0);
    }

    //
    // lifespan
    //
    if (me->lifespan != me->template_thing->lifespan) {
        me->put_opt(os, &me->lifespan, "lifespan",
                    (decltype(me->lifespan))-1);
    }

    //
    // scared
    //
    if (me->_scared != me->template_thing->_scared) {
        me->put_opt(os, &me->_scared, "scared",
                    (decltype(me->_scared))0);
    }

    //
    // sleeping
    //
    if (me->_sleeping != me->template_thing->_sleeping) {
        me->put_opt(os, &me->_sleeping, "sleeping",
                    (decltype(me->_sleeping))0);
    }

    //
    // invisible
    //
    if (me->_invisible != me->template_thing->_invisible) {
        me->put_opt(os, &me->_invisible, "invisible",
                    (decltype(me->_invisible))0);
    }

    //
    // shielded
    //
    if (me->_shielded != me->template_thing->_shielded) {
        me->put_opt(os, &me->_shielded, "shielded",
                    (decltype(me->_shielded))0);
    }

    //
    // raging
    //
    if (me->_raging != me->template_thing->_raging) {
        me->put_opt(os, &me->_raging, "raging",
                    (decltype(me->_raging))0);
    }

    //
    // poisoned
    //
    if (me->_poisoned != me->template_thing->_poisoned) {
        me->put_opt(os, &me->_poisoned, "poisoned",
                    (decltype(me->_poisoned))0);
    }

    //
    // stoned
    //
    if (me->_stoned != me->template_thing->_stoned) {
        me->put_opt(os, &me->_stoned, "stoned",
                    (decltype(me->_stoned))0);
    }

    //
    // metabolism
    //
    if (me->_metabolism != me->template_thing->_metabolism) {
        me->put_opt(os, &me->_metabolism, "metabolism",
                    (decltype(me->_metabolism))0);
    }

    //
    // stamina
    //
    if (me->_stamina != me->template_thing->_stamina) {
        me->put_opt(os, &me->_stamina, "stamina",
                    (decltype(me->_stamina))0);
    }

    //
    // orig_stamina
    //
    if (me->orig_stamina != me->template_thing->orig_stamina) {
        me->put_opt(os, &me->orig_stamina, "orig_stamina",
                    (decltype(me->orig_stamina))0);
    }

    //
    // light_strength
    //
    if (me->_light_strength != me->template_thing->_light_strength) {
        me->put_opt(os, &me->_light_strength, "light_strength",
                    (decltype(me->_light_strength))0);
    }

    //
    // can_see_distance
    //
    if (me->_can_see_distance != me->template_thing->_can_see_distance) {
        me->put_opt(os, &me->_can_see_distance, "can_see_distance",
                    (decltype(me->_can_see_distance))0);
    }

    //
    // missile_at
    //
    if (me->missile_at != me->template_thing->missile_at) {
        me->put_opt_class(os, &me->missile_at, "missile_at",
                          gh_fpoint3d(0,0,0));
    }

    //
    // missile_target
    //
    if (me->missile_target != me->template_thing->missile_target) {
        me->put_opt_class(os, &me->missile_target, "missile_target", 
                          gh_point3d(-1,-1,-1));
    }

    //
    // missile_angle
    //
    if (me->missile_angle != me->template_thing->missile_angle) {
        me->put_opt(os, &me->missile_angle, "missile_angle",
                    (decltype(me->missile_angle))0);
    }

    //
    // missile_angle_delta
    //
    if (me->missile_angle_delta != me->template_thing->missile_angle_delta) {
        me->put_opt(os, &me->missile_angle_delta, "missile_angle_delta",
                    (decltype(me->missile_angle_delta))0);
    }

    //
    // missile_spread
    //
    if (me->missile_spread != me->template_thing->missile_spread) {
        me->put_opt(os, &me->missile_spread, "missile_spread",
            (decltype(me->missile_spread))0);
    }

    //
    // missile_spread_angle
    //
    if (me->missile_spread_angle != me->template_thing->missile_spread_angle) {
        me->put_opt(os, &me->missile_spread_angle, "missile_spread_angle",
            (decltype(me->missile_spread_angle))0);
    }

    //
    // weapon
    //
    me->put_opt_classpp(os, &me->weapon, "weapon");

    //
    // weapon_wielder
    //
    me->put_opt_classpp(os, &me->weapon_wielder, "weapon_wielder");

    //
    // weapon_spin
    //
    if (me->weapon_spin != me->template_thing->weapon_spin) {
        me->put_opt(os, &me->weapon_spin, "weapon_spin", (decltype(me->weapon_spin))0);
    }

    //
    // limbs
    //
    if (me->limbs != me->template_thing->limbs) {
        me->put_opt(os, &me->limbs, "limbs", (decltype(me->limbs))0);
    }

    //
    // join
    //
    tmp_int = me->join;
    if (me->join != me->template_thing->join) {
        me->put_opt(os, &tmp_int, "join", (decltype(tmp_int))0);
    }

    //
    // target
    //
    if (me->target != me->template_thing->target) {
        me->put_opt_class(os, &me->target, "target", gh_point3d(0,0,0));
    }

    //
    // direction
    //
    if (me->direction != me->template_thing->direction) {
        me->put_opt_class(os, &me->direction, "direction", gh_point3d(0,0,0));
    }

    //
    // direction_wanted
    //
    if (me->direction_wanted != me->template_thing->direction_wanted) {
        me->put_opt_class(os, &me->direction_wanted, "direction_wanted",
                          gh_point3d(0,0,0));
    }

    //
    // direction_last
    //
    if (me->direction_last != me->template_thing->direction_last) {
        me->put_opt_class(os, &me->direction_last, "direction_last",
                          gh_point3d(0,0,0));
    }

    //
    // score
    //
    if (me->score != me->template_thing->score) {
        me->put_opt(os, &me->score, "score", (decltype(me->score))0);
    }

    //
    // bravery
    //
    if (me->bravery != me->template_thing->bravery) {
        me->put_opt(os, &me->bravery, "bravery", (decltype(me->bravery))0);
    }

    //
    // bravery_curr
    //
    if (me->bravery_curr != me->template_thing->bravery_curr) {
        me->put_opt(os, &me->bravery_curr, "bravery_curr",
                    (decltype(me->bravery_curr))0);
    }

    //
    // health
    //
    if (me->health != me->template_thing->health) {
        me->put_opt(os, &me->health, "health", (decltype(me->health))0);
    }

    //
    // orig_health
    //
    if (me->orig_health != me->template_thing->orig_health) {
        me->put_opt(os, &me->orig_health, "orig_health",
                    (decltype(me->orig_health))0);
    }

    //
    // fate
    //
    if (me->fate != me->template_thing->fate) {
        me->put_opt(os, &me->fate, "fate", (decltype(me->fate))0);
    }

    //
    // intelligence
    //
    if (me->intelligence != me->template_thing->intelligence) {
        me->put_opt(os, &me->intelligence, "intelligence",
                    (decltype(me->intelligence))0);
    }

    //
    // intelligence_req
    //
    if (me->intelligence_req != me->template_thing->intelligence_req) {
        me->put_opt(os, &me->intelligence_req, "intelligence_req",
                    (decltype(me->intelligence_req))0);
    }

    //
    // armour
    //
    if (me->armour != me->template_thing->armour) {
        me->put_opt(os, &me->armour, "armour", (decltype(me->armour))0);
    }

    //
    // treasure
    //
    if (me->treasure != me->template_thing->treasure) {
        me->put_opt(os, &me->treasure, "treasure", (decltype(me->treasure))0);
    }

    //
    // cost
    //
    if (me->cost != me->template_thing->cost) {
        me->put_opt(os, &me->cost, "cost", (decltype(me->cost))0);
    }

    //
    // weight
    //
    if (me->weight != me->template_thing->weight) {
        me->put_opt(os, &me->weight, "weight", (decltype(me->weight))0);
    }

    //
    // keys
    //
    if (me->keys != me->template_thing->keys) {
        me->put_opt(os, &me->keys, "keys", (decltype(me->keys))0);
    }

    //
    // anim_speed
    //
    if (me->anim_speed != me->template_thing->anim_speed) {
        me->put_opt(os, &me->anim_speed, "anim_speed",
                    (decltype(me->anim_speed))0);
    }

    //
    // dir
    //
    tmp_int = me->dir;
    if (me->dir != me->template_thing->dir) {
        me->put_opt(os, &tmp_int, "dir", (decltype(tmp_int))0);
    }

    //
    // mode
    //
    tmp_int = me->mode;
    if (me->mode != me->template_thing->mode) {
        me->put_opt(os, &tmp_int, "mode", (decltype(tmp_int))0);
    }

    //
    // frame
    //
    if (me->frame != me->template_thing->frame) {
        me->put_opt(os, &me->frame, "frame", (decltype(me->frame))0);
    }

    //
    // bite_damage
    //
    if (me->bite_damage != me->template_thing->bite_damage) {
        me->put_opt(os, &me->bite_damage, "bite_damage",
                    (decltype(me->bite_damage))0);
    }

    //
    // bite_armour
    //
    if (me->bite_armour != me->template_thing->bite_armour) {
        me->put_opt(os, &me->bite_armour, "bite_armour",
                    (decltype(me->bite_armour))0);
    }

    //
    // bite_treasure
    //
    if (me->bite_treasure != me->template_thing->bite_treasure) {
        me->put_opt(os, &me->bite_treasure, "bite_treasure",
                    (decltype(me->bite_treasure))0);
    }

    //
    // bite_delay_in_tenths
    //
    if (me->bite_delay_in_tenths != me->template_thing->bite_delay_in_tenths) {
        me->put_opt(os, &me->bite_delay_in_tenths, "bite_delay_in_tenths",
                    (decltype(me->bite_delay_in_tenths))0);
    }

    //
    // bite_chance
    //
    if (me->bite_chance != me->template_thing->bite_chance) {
        me->put_opt(os, &me->bite_chance, "bite_chance",
                    (decltype(me->bite_chance))0);
    }

    //
    // bite_karma
    //
    if (me->bite_karma != me->template_thing->bite_karma) {
        me->put_opt(os, &me->bite_karma, "bite_karma",
                    (decltype(me->bite_karma))0);
    }

    //
    // bite_karma_chance
    //
    if (me->bite_karma_chance != me->template_thing->bite_karma_chance) {
        me->put_opt(os, &me->bite_karma_chance, "bite_karma_chance",
                    (decltype(me->bite_karma_chance))0);
    }

    //
    // bite_intelligence
    //
    if (me->bite_intelligence != me->template_thing->bite_intelligence) {
        me->put_opt(os, &me->bite_intelligence, "bite_intelligence",
                    (decltype(me->bite_intelligence))0);
    }

    //
    // bite_intelligence_chance
    //
    if (me->bite_intelligence_chance != me->template_thing->bite_intelligence_chance) {
        me->put_opt(os, &me->bite_intelligence_chance, "bite_intelligence_chance",
                    (decltype(me->bite_intelligence_chance))0);
    }

    //
    // bite_poison
    //
    if (me->bite_poison != me->template_thing->bite_poison) {
        me->put_opt(os, &me->bite_poison, "bite_poison",
                    (decltype(me->bite_poison))0);
    }

    //
    // bite_poison_chance
    //
    if (me->bite_poison_chance != me->template_thing->bite_poison_chance) {
        me->put_opt(os, &me->bite_poison_chance, "bite_poison_chance",
                    (decltype(me->bite_poison_chance))0);
    }

    //
    // bite_stone
    //
    if (me->bite_stone != me->template_thing->bite_stone) {
        me->put_opt(os, &me->bite_stone, "bite_stone",
                    (decltype(me->bite_stone))0);
    }

    //
    // bite_stone_chance
    //
    if (me->bite_stone_chance != me->template_thing->bite_stone_chance) {
        me->put_opt(os, &me->bite_stone_chance, "bite_stone_chance",
                    (decltype(me->bite_stone_chance))0);
    }

    //
    // bite_fate
    //
    if (me->bite_fate != me->template_thing->bite_fate) {
        me->put_opt(os, &me->bite_fate, "bite_fate",
                    (decltype(me->bite_fate))0);
    }

    //
    // bite_fate_chance
    //
    if (me->bite_fate_chance != me->template_thing->bite_fate_chance) {
        me->put_opt(os, &me->bite_fate_chance, "bite_fate_chance",
                    (decltype(me->bite_fate_chance))0);
    }

    //
    // karma
    //
    if (me->karma != me->template_thing->karma) {
        me->put_opt(os, &me->karma, "karma",
                    (decltype(me->karma))0);
    }

    //
    // spawn_delay_in_tenths
    //
    if (me->spawn_delay_in_tenths != me->template_thing->spawn_delay_in_tenths) {
        me->put_opt(os, &me->spawn_delay_in_tenths, "spawn_delay_in_tenths",
                    (decltype(me->spawn_delay_in_tenths))0);
    }

    //
    // spawing_chance
    //
    if (me->spawning_chance != me->template_thing->spawning_chance) {
        me->put_container(os, &me->spawning_chance, "spawning_chance");
    }

    //
    // spawns
    //
    if (me->spawns != me->template_thing->spawns) {
        me->put_container(os, &me->spawns, "spawns");
    }

    //
    // backpack
    //
    me->put_container(os, &me->backpack, "backpack");

    //
    // followers
    //
    if (me->followers != me->template_thing->followers) {
        me->put_container(os, &me->followers, "followers");
    }

    //
    // follow_distance
    //
    if (me->follow_distance != me->template_thing->follow_distance) {
        me->put_opt(os, &me->follow_distance, "follow_distance",
                    (decltype(me->follow_distance))0);
    }

    //
    // transmog_delay_in_tenths
    //
    if (me->transmog_delay_in_tenths != me->template_thing->transmog_delay_in_tenths) {
        me->put_opt(os, &me->transmog_delay_in_tenths, "transmog_delay_in_tenths",
                    (decltype(me->transmog_delay_in_tenths))0);
    }

    //
    // transmogrify_chance
    //
    if (me->transmogrify_chance != me->template_thing->transmogrify_chance) {
        me->put_container(os, &me->transmogrify_chance, "transmogrify_chance");
    }

    //
    // transmogrifies_into
    //
    if (me->transmogrifies_into != me->template_thing->transmogrifies_into) {
        me->put_container(os, &me->transmogrifies_into, "transmogrifies_into");
    }

    //
    // teleport_delay_in_tenths
    //
    if (me->teleport_delay_in_tenths != me->template_thing->teleport_delay_in_tenths) {
        me->put_opt(os, &me->teleport_delay_in_tenths, "teleport_delay_in_tenths",
                    (decltype(me->teleport_delay_in_tenths))0);
    }

    //
    // teleport_chance
    //
    if (me->teleport_chance != me->template_thing->teleport_chance) {
        me->put_opt(os, &me->teleport_chance, "teleport_chance",
                    (decltype(me->teleport_chance))0);
    }

    //
    // use_chance
    //
    if (me->use_chance != me->template_thing->use_chance) {
        me->put_container(os, &me->use_chance, "use_chance");
    }

    //
    // use_action
    //
    if (me->use_action != me->template_thing->use_action) {
        me->put_container(os, &me->use_action, "use_action");
    }

    //
    // bite_action
    //
    if (me->bite_action != me->template_thing->bite_action) {
        me->put_container(os, &me->bite_action, "bite_action");
    }

    //
    // charges
    //
    if (me->charges != me->template_thing->charges) {
        me->put_opt(os, &me->charges, "charges", (decltype(me->charges))0);
    }

    //
    // look_delay_in_tenths
    //
    if (me->look_delay_in_tenths != me->template_thing->look_delay_in_tenths) {
        me->put_opt(os, &me->look_delay_in_tenths, "look_delay_in_tenths",
                    (decltype(me->look_delay_in_tenths))0);
    }

    //
    // explosion_type
    //
    if (me->explosion_type != me->template_thing->explosion_type) {
        me->put_opt(os, &me->explosion_type, "explosion_type",
                    (decltype(me->explosion_type))"");
    }

    //
    // audio_on_use
    //
    if (me->audio_on_use != me->template_thing->audio_on_use) {
        me->put_opt(os, &me->audio_on_use, "audio_on_use",
                    (decltype(me->audio_on_use))"");
    }

    //
    // audio_on_dead
    //
    if (me->audio_on_dead != me->template_thing->audio_on_dead) {
        me->put_opt(os, &me->audio_on_dead, "audio_on_dead",
                    (decltype(me->audio_on_dead))"");
    }

    //
    // audio_on_hit
    //
    if (me->audio_on_hit != me->template_thing->audio_on_hit) {
        me->put_opt(os, &me->audio_on_hit, "audio_on_hit",
                    (decltype(me->audio_on_hit))"");
    }

    //
    // audio_on_open
    //
    if (me->audio_on_open != me->template_thing->audio_on_open) {
        me->put_opt(os, &me->audio_on_open, "audio_on_open",
                    (decltype(me->audio_on_open))"");
    }

    //
    // wield_as
    //
    if (me->wield_as != me->template_thing->wield_as) {
        me->put_opt(os, &me->wield_as, "wield_as",
                    (decltype(me->wield_as))"");
    }

    //
    // We use this flag when loading as an anchor point hence
    // always print it even if different from the template.
    //
    tmp_bool = me->is_hero;
    me->put_named_flag(os, &tmp_bool, "is_hero");

#define PUT_OPT_NAMED_FLAG(__FLAG__)                                    \
    tmp_bool = me->__FLAG__;                                            \
    me->put_opt_named_flag(os, &tmp_bool, #__FLAG__,                    \
                           me->template_thing ?                         \
                           (bool)me->template_thing->__FLAG__ : false);

    PUT_OPT_NAMED_FLAG(cant_see_past)
    PUT_OPT_NAMED_FLAG(collects_stuff_but_doesnt_use_it)
    PUT_OPT_NAMED_FLAG(does_stuff)
    PUT_OPT_NAMED_FLAG(cached_gfx)
    PUT_OPT_NAMED_FLAG(is_floor)
    PUT_OPT_NAMED_FLAG(is_dirt)
    PUT_OPT_NAMED_FLAG(is_wall)
    PUT_OPT_NAMED_FLAG(is_rock)
    PUT_OPT_NAMED_FLAG(is_hard)
    PUT_OPT_NAMED_FLAG(is_body_part)
    PUT_OPT_NAMED_FLAG(is_chained_body_part)
    PUT_OPT_NAMED_FLAG(stay_above_torso)
    PUT_OPT_NAMED_FLAG(not_in_shops)
    PUT_OPT_NAMED_FLAG(is_shopkeeper)
    PUT_OPT_NAMED_FLAG(is_stolen)
    PUT_OPT_NAMED_FLAG(is_unpaid_for)
    PUT_OPT_NAMED_FLAG(is_internal)
    PUT_OPT_NAMED_FLAG(is_thief)
    PUT_OPT_NAMED_FLAG(is_explorer)
    PUT_OPT_NAMED_FLAG(shake_screen_on_death)
    PUT_OPT_NAMED_FLAG(is_inventory_item)
    PUT_OPT_NAMED_FLAG(is_auto_item)
    PUT_OPT_NAMED_FLAG(is_weapon)
    PUT_OPT_NAMED_FLAG(is_sword)
    PUT_OPT_NAMED_FLAG(is_staff)
    PUT_OPT_NAMED_FLAG(is_wand)
    PUT_OPT_NAMED_FLAG(is_axe)
    PUT_OPT_NAMED_FLAG(is_warhammer)
    PUT_OPT_NAMED_FLAG(is_bow)
    PUT_OPT_NAMED_FLAG(is_gun)
    PUT_OPT_NAMED_FLAG(is_armour)
    PUT_OPT_NAMED_FLAG(is_amulet)
    PUT_OPT_NAMED_FLAG(is_cursed)
    PUT_OPT_NAMED_FLAG(curse_noticed)
    PUT_OPT_NAMED_FLAG(can_see_cursed_items)
    PUT_OPT_NAMED_FLAG(can_wield_weapons)
    PUT_OPT_NAMED_FLAG(can_cast_spells)
    PUT_OPT_NAMED_FLAG(can_insult)
    PUT_OPT_NAMED_FLAG(is_chargeable)
    PUT_OPT_NAMED_FLAG(is_potion)
    PUT_OPT_NAMED_FLAG(is_spell)
    PUT_OPT_NAMED_FLAG(is_unable_to_move)
    PUT_OPT_NAMED_FLAG(is_generator)
    PUT_OPT_NAMED_FLAG(cannot_summon)
    PUT_OPT_NAMED_FLAG(is_monst)
    PUT_OPT_NAMED_FLAG(is_destroyable)
    PUT_OPT_NAMED_FLAG(is_missile)
    PUT_OPT_NAMED_FLAG(is_bouncy)
    PUT_OPT_NAMED_FLAG(is_boring)
    PUT_OPT_NAMED_FLAG(is_corridor)
    PUT_OPT_NAMED_FLAG(is_teleport)
    PUT_OPT_NAMED_FLAG(is_teleport_attack)
    PUT_OPT_NAMED_FLAG(is_exit)
    PUT_OPT_NAMED_FLAG(is_return_exit)
    PUT_OPT_NAMED_FLAG(is_door)
    PUT_OPT_NAMED_FLAG(is_fire)
    PUT_OPT_NAMED_FLAG(hates_fire)
    PUT_OPT_NAMED_FLAG(loves_fire)
    PUT_OPT_NAMED_FLAG(is_acid)
    PUT_OPT_NAMED_FLAG(hates_acid)
    PUT_OPT_NAMED_FLAG(loves_acid)
    PUT_OPT_NAMED_FLAG(is_water)
    PUT_OPT_NAMED_FLAG(hates_water)
    PUT_OPT_NAMED_FLAG(loves_water)
    PUT_OPT_NAMED_FLAG(is_death)
    PUT_OPT_NAMED_FLAG(loves_death)
    PUT_OPT_NAMED_FLAG(is_cold)
    PUT_OPT_NAMED_FLAG(hates_cold)
    PUT_OPT_NAMED_FLAG(loves_cold)
    PUT_OPT_NAMED_FLAG(is_suffocation)
    PUT_OPT_NAMED_FLAG(is_meat)
    PUT_OPT_NAMED_FLAG(is_gas)
    PUT_OPT_NAMED_FLAG(is_edible)
    PUT_OPT_NAMED_FLAG(is_treasure)
    PUT_OPT_NAMED_FLAG(is_meat_eater)
    PUT_OPT_NAMED_FLAG(is_holy)
    PUT_OPT_NAMED_FLAG(is_undead)
    PUT_OPT_NAMED_FLAG(is_unique)
    PUT_OPT_NAMED_FLAG(is_silver)
    PUT_OPT_NAMED_FLAG(is_trap)
    PUT_OPT_NAMED_FLAG(is_treasure_eater)
    PUT_OPT_NAMED_FLAG(is_edible_eater)
    PUT_OPT_NAMED_FLAG(is_joinable)
    PUT_OPT_NAMED_FLAG(is_transparent)
    PUT_OPT_NAMED_FLAG(destroy_joined)
    PUT_OPT_NAMED_FLAG(is_light_source)
    PUT_OPT_NAMED_FLAG(is_movable)
    PUT_OPT_NAMED_FLAG(only_in_caves);
    PUT_OPT_NAMED_FLAG(only_in_forests);
    PUT_OPT_NAMED_FLAG(only_in_landscapes);
    PUT_OPT_NAMED_FLAG(only_in_dungeons);
    PUT_OPT_NAMED_FLAG(loves_void);
    PUT_OPT_NAMED_FLAG(only_in_hell);
    PUT_OPT_NAMED_FLAG(loves_full_moon)
    PUT_OPT_NAMED_FLAG(loves_midnight)
    PUT_OPT_NAMED_FLAG(loves_night)
    PUT_OPT_NAMED_FLAG(loves_friday_13th)
    PUT_OPT_NAMED_FLAG(loves_halloween)
    PUT_OPT_NAMED_FLAG(never_slows_down);
    PUT_OPT_NAMED_FLAG(moves_up_down);
    PUT_OPT_NAMED_FLAG(moves_left_right);
    PUT_OPT_NAMED_FLAG(moves_until_collision);
    PUT_OPT_NAMED_FLAG(leaves_corpse);
    PUT_OPT_NAMED_FLAG(casts_lightning);
    PUT_OPT_NAMED_FLAG(tireless_swing);
    PUT_OPT_NAMED_FLAG(tireless);
    PUT_OPT_NAMED_FLAG(ability_can_regenerate1);
    PUT_OPT_NAMED_FLAG(ability_can_regenerate2);
    PUT_OPT_NAMED_FLAG(ability_can_regenerate3);
    PUT_OPT_NAMED_FLAG(can_self_teleport);
    PUT_OPT_NAMED_FLAG(can_self_teleport_once_per_level);
    PUT_OPT_NAMED_FLAG(ability_can_rage);
    PUT_OPT_NAMED_FLAG(ability_can_auto_rage);
    PUT_OPT_NAMED_FLAG(is_open)
    PUT_OPT_NAMED_FLAG(is_closed)
    PUT_OPT_NAMED_FLAG(is_chasing)
    PUT_OPT_NAMED_FLAG(is_dead)
    PUT_OPT_NAMED_FLAG(is_on_map)
    PUT_OPT_NAMED_FLAG(has_been_seen)
    PUT_OPT_NAMED_FLAG(cluster_anywhere)

    //
    // render_depth
    //
    if (me->render_depth != me->template_thing->render_depth) {
        me->put_opt(os, &me->render_depth, "render_depth",
                    (decltype(me->render_depth))0);
    }

    //
    // exit_jump
    //
    if (me->exit_jump != me->template_thing->exit_jump) {
        me->put_opt(os, &me->exit_jump, "exit_jump",
                    (decltype(me->exit_jump))0);
    }

    //
    // exit_name
    //
    if (me->exit_name != me->template_thing->exit_name) {
        me->put_opt(os, &me->exit_name, "exit_name",
                    (decltype(me->exit_name))"");
    }

    //
    // cluster_radius
    //
    if (me->cluster_radius != me->template_thing->cluster_radius) {
        me->put_opt(os, &me->cluster_radius, "cluster_radius",
                    (decltype(me->cluster_radius))0);
    }

    //
    // cluster_amount
    //
    if (me->cluster_amount != me->template_thing->cluster_amount) {
        me->put_opt(os, &me->cluster_amount, "cluster_amount",
                    (decltype(me->cluster_amount))0);
    }

    //
    // laser_range
    //
    if (me->laser_range != me->template_thing->laser_range) {
        me->put_opt(os, &me->laser_range, "laser_range",
                    (decltype(me->laser_range))0);
    }

    //
    // attack_chance
    //
    if (me->attack_chance != me->template_thing->attack_chance) {
        me->put_container(os, &me->attack_chance, "attack_chance");
    }

    //
    // attacks
    //
    if (me->attacks != me->template_thing->attacks) {
        me->put_container(os, &me->attacks, "attacks");
    }

    //
    // wake_reasons
    //
    if (me->wake_reasons != me->template_thing->wake_reasons) {
        me->put_container(os, &me->wake_reasons, "wake_reasons");
    }

    //
    // follow_chance
    //
    if (me->follow_chance != me->template_thing->follow_chance) {
        me->put_container(os, &me->follow_chance, "follow_chance");
    }

    //
    // follows
    //
    if (me->follows != me->template_thing->follows) {
        me->put_container(os, &me->follows, "follows");
    }

    //
    // throws
    //
    if (me->throws != me->template_thing->throws) {
        me->put_container(os, &me->throws, "throws");
    }

    //
    // opacity
    //
    if (me->opacity != me->template_thing->opacity) {
        me->put_opt(os, &me->opacity, "opacity",
                    (decltype(me->opacity))0);
    }

    //
    // level_mod
    //
    if (me->level_mod != me->template_thing->level_mod) {
        me->put_opt(os, &me->level_mod, "level_mod",
                    (decltype(me->level_mod))0);
    }

    //
    // text
    //
    if (me->text != me->template_thing->text) {
        me->put_opt(os, &me->text, "text",
                    (decltype(me->text))"");
    }
}
