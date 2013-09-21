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

    $Id: chasing.cpp 576 2007-12-01 22:23:00Z goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_templates.h>
#include <gh_subsys.h>
#include <gh_math.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_thing.h>

//
// Copy only relevant fields for polymorphing
//
void gh_thing::polycopy (const gh_thing *thing)
{
    GH_FUNCTION()

    name                            = thing->name;
    text                            = thing->text;
    full_name                       = thing->full_name;
    short_name                      = thing->short_name;
    dir                             = thing->dir;
    mode                            = thing->mode;
    frame                           = thing->frame;
    missile_at                      = thing->missile_at;
    missile_target                  = thing->missile_target;
    missile_angle                   = thing->missile_angle;
    missile_angle_delta             = thing->missile_angle_delta;
    missile_spread                  = thing->missile_spread;
    missile_spread_angle            = thing->missile_spread_angle;
    weapon_spin                     = thing->weapon_spin;
    join                            = thing->join;
    direction                       = thing->direction;
    direction_wanted                = thing->direction_wanted;
    direction_last                  = thing->direction_last;
    target                          = thing->target;
    bravery                         = thing->bravery;
    bravery_curr                    = thing->bravery_curr;
    cost                            = thing->cost;
    weight                          = thing->weight;
    _strength                       = thing->_strength;
    strength_boost                  = thing->strength_boost;
    strength_req                    = thing->strength_req;
    _poison_proof                   = thing->_poison_proof;
    _teleport_proof                 = thing->_teleport_proof;
    _suffocation_proof              = thing->_suffocation_proof;
    _cold_proof                     = thing->_cold_proof;
    _acid_proof                     = thing->_acid_proof;
    _fire_proof                     = thing->_fire_proof;
    _water_proof                    = thing->_water_proof;
    _stone_proof                    = thing->_stone_proof;
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
    only_in_caves                   = thing->only_in_caves;
    only_in_forests                 = thing->only_in_forests;
    only_in_landscapes              = thing->only_in_landscapes;
    only_in_dungeons                = thing->only_in_dungeons;
    loves_void                      = thing->loves_void;
    only_in_hell                    = thing->only_in_hell;
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
    appearing_chance = thing->appearing_chance;
    throws = thing->throws;
    attacks = thing->attacks;
    wake_reasons = thing->wake_reasons;
    attack_chance = thing->attack_chance;
    follows = thing->follows;
    follow_chance = thing->follow_chance;
    transmogrify_chance = thing->transmogrify_chance;
    transmogrifies_into = thing->transmogrifies_into;
    use_chance = thing->use_chance;
    use_action = thing->use_action;
    bite_action = thing->bite_action;
    followers = thing->followers;
    spawns = thing->spawns;
    spawning_chance = thing->spawning_chance;

    /*
     * Be nasty and don't allow these things to reset.
     *
    _poisoned                       = thing->_poisoned;
    poisoned_boost                  = thing->poisoned_boost;

    _stoned                         = thing->_stoned;
    stoned_boost                    = thing->stoned_boost;

    is_thief                        = thing->is_thief;
    is_cursed                       = thing->is_cursed;
     */
}

void gh_thing::polymorph_attack (gh_thing *target)
{
    GH_FUNCTION()

    if (target->is_hero && 
        (target->health < target->orig_health / 5)) {

        level->game->new_message("%%fg=redYou die of system shock%%fg=green");

        target->dead("self polymorph");
        return;
    }

    if (target->is_monst && 
        (target->health < target->orig_health / 5)) {
        if (is_hero) {
            level->game->new_message("%%fg=red" + target->the_named() + 
                                     " dies of system shock%%fg=green");
        }

        target->dead("polymorph attack");
        return;
    }

    string poly_thing_name = level->map_char_polymorph(target->short_name);
    if (poly_thing_name == "") {
        if (is_hero) {
            level->game->new_message("%%fg=redThe " + target->named() + 
                                     " resists polymorphings%%fg=green");
        }

        GH_THING_LOG(this, " polymorph failed on " + target->named());
        return;
    }

    GH_THING_LOG(this, "is polymorphing " + target->named() + 
                 " into " + poly_thing_name);

    if (target->is_hero) {
        level->game->new_message("%%fg=purpleYou feel like a new " + 
                                 named() + "%%fg=green",
                                 true /* unique */);

        gh_thing *template_thing = gh_thing::find(poly_thing_name,
                                                  true /* error */);
    
        target->polycopy(template_thing);

        target->unwield();
        target->wield_next_best();
    } else {
        if (!monst_summon_forced_at(poly_thing_name, target->at)) {
            level->game->new_message(
                 "%%fg=red" + target->the_named() +
                 " resists the polyattack%%fg=green");
            return;
        }

        if (target->is_monst) {
            level->game->new_message(
                 "%%fg=purple" + target->the_named() + 
                 " looks confused%%fg=green");
        }

        if (is_monst) {
            unwield();
            wield_next_best();
        }

        target->leaves_corpse = false;
        target->dead("polymorphed");
    }
}
