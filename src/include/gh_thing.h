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

    $Id: gh_thing.h 618 2008-01-01 12:50:13Z goblinhack $
*/

#ifndef _GH_THING_H_
#define _GH_THING_H_

#include "gh_pixel.h"
#include "gh_model.h"

#define nDEBUG_THINGS
#define nDEBUG_THINGS_VERBOSE

#define GH_THING_REF_LIST_MACRO(macro)                          \
    macro(GH_THING_REF_TERMINAL,        "tmp-terminal"),        \
    macro(GH_THING_REF_DROP_ITEM,       "tmp-drop-item"),       \
    macro(GH_THING_REF_DESTROY_ITEM,    "tmp-destroy-item"),    \
    macro(GH_THING_REF_FIRST,           "tmp-first"),           \
    macro(GH_THING_REF_LAST,            "tmp-last"),            \
    macro(GH_THING_REF_TRANSMOG,        "tmp-transmog"),        \
    macro(GH_THING_REF_ACTION,          "action"),              \
    macro(GH_THING_REF_ALL_THINGS,      "all-things"),          \
    macro(GH_THING_REF_CURSOR,          "cursor"),              \
    macro(GH_THING_REF_DEAD,            "dead-things"),         \
    macro(GH_THING_REF_DOES_STUFF,      "does-stuff"),          \
                                                                \
    /*                                                          \
     * This item is being carried by anoterh.                   \
     */                                                         \
    macro(GH_THING_REF_IM_BEING_CARRIED,"carried-item"),        \
    macro(GH_THING_REF_MAP,             "map"),                 \
    macro(GH_THING_REF_TMP_FOLLOWER,    "follower"),            \
    macro(GH_THING_REF_FOLLOWER_HEAD,   "follower-head"),       \
                                                                \
    /*                                                          \
     * Used to place locks on heroes so that their weapons      \
     * have a valid pointer.                                    \
     */                                                         \
    macro(GH_THING_REF_WEAPON_WIELDER,  "weapon-wielder"),      \
                                                                \
    /*                                                          \
     * Used to place locks on weapons so that the hero has      \
     * a valid pointer to notify.                               \
     */                                                         \
    macro(GH_THING_REF_WEAPON,          "weapon"),              \
                                                                \
    /*                                                          \
     * Used to place multiple locks on heroes so that the       \
     * missiles a valid pointer to notify.                      \
     */                                                         \
    macro(GH_THING_REF_MISSILE_THROWER, "missile-thrower"),     \
                                                                \
    macro(GH_THING_REF_OWNER,           "owner"),               \
    macro(GH_THING_REF_PLAYER,          "player"),              \
    macro(GH_THING_REF_SHOPKEEPER,      "shopkeeper"),          \
    macro(GH_THING_REF_ED,              "editor")               \
                                                                                
DEFINE_ENUM(gh_thing_ref, GH_THING_REF)

#define GH_THING_MODE_LIST_MACRO(macro)             \
    macro(GH_THING_MODE_NORMAL,     "normal"),      \
    macro(GH_THING_MODE_SCARED,     "scared"),      \
    macro(GH_THING_MODE_CLOSED,     "closed"),      \
    macro(GH_THING_MODE_OPEN,       "open"),        \
    macro(GH_THING_MODE_SLEEPING,   "sleeping")     \
                                                                                
DEFINE_ENUM(gh_thing_mode, GH_THING_MODE)

#define GH_THING_DIR_LIST_MACRO(macro)              \
    macro(GH_THING_DIR_U,           "up"),          \
    macro(GH_THING_DIR_TR,          "tr"),          \
    macro(GH_THING_DIR_R,           "right"),       \
    macro(GH_THING_DIR_BR,          "br"),          \
    macro(GH_THING_DIR_D,           "down"),        \
    macro(GH_THING_DIR_BL,          "bl"),          \
    macro(GH_THING_DIR_L,           "left"),        \
    macro(GH_THING_DIR_TL,          "tl"),          \
    macro(GH_THING_DIR_ALL,         "all"),         \
    macro(GH_THING_DIR_NODE,        "node"),        \
    macro(GH_THING_DIR_UD,          "ud"),          \
    macro(GH_THING_DIR_LR,          "lr"),          \
    macro(GH_THING_DIR_L0,          "l0"),          \
    macro(GH_THING_DIR_L90,         "l90"),         \
    macro(GH_THING_DIR_L180,        "l180"),        \
    macro(GH_THING_DIR_L270,        "l270"),        \
    macro(GH_THING_DIR_T0,          "t0"),          \
    macro(GH_THING_DIR_T90,         "t90"),         \
    macro(GH_THING_DIR_T180,        "t180"),        \
    macro(GH_THING_DIR_T270,        "t270"),        \
    macro(GH_THING_DIR_NONE,        "none"),        \
    macro(GH_THING_DIR_X,           "x")            \

DEFINE_ENUM(gh_thing_dir, GH_THING_DIR)

#define GH_THING_GFX_LEVEL_MAX          2

typedef bool (*gh_thing_evaluator_callback)(class gh_thing *thing, void *arg);

typedef struct gh_anim_frame_cell_ {
    char            c;
    //
    // Foreground and background
    //
    uint8           fg;
    uint8           bg;
    //
    // When this cell is stored within a thing, indicates if this
    // cell should be used in damage collisions. i.e. one thing
    // hitting another.
    //
    uint8           can_hit_mask:1;
    //
    // When this cell is stored within a thing, indicates if this
    // cell should be used in obstacle collisions. i.e. does this
    // thing bump into a wall. This is different from the damage
    // mask to allow some things to hide (spikes) and not cause
    // damage (temporarily) and yet not be rendered invisible w.r.t.
    // collisions with walls.
    //
    uint8           obstacle_mask:1;
    //
    // Can we touch it, open it, etc...?
    //
    uint8           can_touch_mask:1;
} gh_anim_frame_cell;

typedef struct {
    //
    // The dimensions of the character array; we only really
    // need the width.
    //
    int width;
    int height;

    //
    // pixel width and height of the bounding box for this frame;
    // ignores ' ' chars. Can't be used for -w/2 in drawing as the
    // frame may not be symmetrical.
    //
    float pix_min_w;
    float pix_min_h;
    float pix_max_w;
    float pix_max_h;

    //
    // Min max bounds of the bounding box; use this for collisions
    //
    gh_fpoint pix_minv;
    gh_fpoint pix_maxv;

    //
    // for obstacles
    //
    gh_fpoint obs_pix_minv;
    gh_fpoint obs_pix_maxv;

    //
    // Where in the bigger texture this thing ends
    //
    float u;
    float v;
    int tex;
    int mask;

    // 
    // A graphics list for drawing this frame
    //
    int l;

    //
    // Characters in this frame
    //
    vector<gh_anim_frame_cell> cell;

    //
    // Is this frame empty?
    //
    uint8 invisible:1;
    uint8 can_touch:1;
    uint8 can_hit:1;
    uint8 obstacle:1;

} gh_anim_frame;

class gh_anim {
public:
    gh_anim(void) {}
    ~gh_anim(void) {}

    vector<gh_anim_frame> anim_frame[GH_THING_DIR_MAX][GH_THING_MODE_MAX][2];
};

class gh_thing : public gh_stream
{  
public:
    void * self;

    //
    // Me. A unique id. Shared for body parts.
    //
    int id;

    //
    // Things I'm associated with. Possessed monsts. Missiles.
    //
    int friend_id;

    //
    // Spawned from the same generator
    //
    int group_id;

    gh_thing(class gh_level *level = NULL,
             const string gh_cfg_file = "",
             const gh_point3d at = gh_point3d(0,0,0),
             const gh_thing *template_thing = NULL,
             gh_player *player = NULL);
    ~gh_thing(void);
    void copy(const gh_thing *thing);
    void init(void);

    //
    // ref.cpp
    //
    bool isref(gh_thing_ref);
    void incref(gh_thing_ref);
    void decref(gh_thing_ref);
    void add_leaked_things(void);
    bool rm_leaked_things(void);
    void add_all_things(void);
    bool rm_all_things(void);
    void add_map_things(void);
    bool rm_map_things(void);
    void add_does_stuff_things(void);
    bool rm_does_stuff_things(void);
    void add_dead_things(void);
    bool rm_dead_things(void);
    static void find_leaks(void);

    //
    // strength.cpp
    //
    // Strength is in pounds that can be carried
    //

    //
    // Find out the current "strength", taking the boost into account.
    //
    gh_inline int strength (void)
    {
        GH_FUNCTION()

        return (_strength + strength_boost);
    }

    void strength_boost_start(int boost);
    void strength_boost_tick(void);
    void strength_boost_stop(void);

    //
    // dex.cpp
    //
    // Strength is in pounds that can be carried
    //

    //
    // Find out the current "dex", taking the boost into account.
    //
    gh_inline int dex (void)
    {
        GH_FUNCTION()

        return (((int)_dex) + dex_boost);
    }

    void dex_boost_start(int boost);
    void dex_boost_tick(void);
    void dex_boost_stop(void);

    //
    // stamina.cpp
    //
    void stamina_decrease(void);
    void stamina_tick(void);

    //
    // Find out the current "stamina", taking the boost into account.
    //
    gh_inline int stamina (void)
    {
        GH_FUNCTION()

        int m = _stamina + stamina_boost;

        return (m);
    }

    void stamina_boost_start(int boost);
    void stamina_boost_tick(void);
    void stamina_boost_stop(void);

    //
    // lifespan.cpp
    //
    void lifespan_tick(void);

    //
    // scared.cpp
    //
    
    //
    // Find out the current "scared", taking the boost into account.
    //
    gh_inline int scared (void)
    {
        GH_FUNCTION()

        return (_scared + scared_boost);
    }

    void scared_boost_start(int boost);
    void scared_boost_tick(void);
    void scared_boost_stop(void);

    //
    // sleeping.cpp
    //
    
    //
    // Find out the current "sleeping", taking the boost into account.
    //
    gh_inline int sleeping (void)
    {
        GH_FUNCTION()

        return (_sleeping + sleeping_boost);
    }

    void sleeping_boost_start(int boost);
    void sleeping_boost_tick(void);
    void sleeping_boost_stop(void);

    //
    // shielded.cpp
    //
    
    //
    // Find out the current "shielded", taking the boost into account.
    //
    gh_inline int shielded (void)
    {
        GH_FUNCTION()

        return (_shielded + shielded_boost);
    }

    void shielded_boost_start(int boost);
    void shielded_boost_tick(void);
    void shielded_boost_stop(void);
    bool wake_reason(gh_thing *);

    //
    // invisible.cpp
    //
    
    //
    // Find out the current "invisible", taking the boost into account.
    //
    gh_inline int invisible (void)
    {
        GH_FUNCTION()

        return (_invisible + invisible_boost);
    }

    void invisible_boost_start(int boost);
    void invisible_boost_tick(void);
    void invisible_boost_stop(void);

    //
    // poisoned.cpp
    //
    
    //
    // Find out the current "poisoned", taking the boost into account.
    //
    gh_inline int poisoned (void)
    {
        GH_FUNCTION()

        return (_poisoned + poisoned_boost);
    }

    void poisoned_boost_start(int boost);
    void poisoned_boost_tick(void);
    void poisoned_boost_stop(void);

    //
    // stoned.cpp
    //

    //
    // Find out the current "stoned", taking the boost into account.
    //
    gh_inline int stoned (void)
    {
        GH_FUNCTION()

        return (_stoned + stoned_boost);
    }

    void stoned_boost_start(int boost);
    void stoned_boost_tick(void);
    void stoned_boost_stop(void);

    //
    // metabolism.cpp
    //

    //
    // Find out the current "metabolism", taking the boost into account.
    //
    gh_inline int metabolism (void)
    {
        GH_FUNCTION()

        int m = _metabolism + metabolism_boost;

        return (m);
    }

    void metabolism_boost_start(int boost);
    void metabolism_boost_tick(void);
    void metabolism_boost_stop(void);

    //
    // water_proof.cpp
    //
    int water_proof(void);
    bool is_water_proof(void);
    void water_proof_boost_start(int boost);
    void water_proof_boost_tick(void);
    void water_proof_boost_stop(void);

    //
    // fire_proof.cpp
    //
    int fire_proof(void);
    bool is_fire_proof(void);
    void fire_proof_boost_start(int boost);
    void fire_proof_boost_tick(void);
    void fire_proof_boost_stop(void);

    //
    // acid_proof.cpp
    //
    int acid_proof(void);
    bool is_acid_proof(void);
    void acid_proof_boost_start(int boost);
    void acid_proof_boost_tick(void);
    void acid_proof_boost_stop(void);

    //
    // cold_proof.cpp
    //
    int cold_proof(void);
    bool is_cold_proof(void);
    void cold_proof_boost_start(int boost);
    void cold_proof_boost_tick(void);
    void cold_proof_boost_stop(void);

    //
    // suffocation_proof.cpp
    //
    int suffocation_proof(void);
    bool is_suffocation_proof(void);
    void suffocation_proof_boost_start(int boost);
    void suffocation_proof_boost_tick(void);
    void suffocation_proof_boost_stop(void);

    //
    // teleport_proof.cpp
    //
    int teleport_proof(void);
    bool is_teleport_proof(void);
    void teleport_proof_boost_start(int boost);
    void teleport_proof_boost_tick(void);
    void teleport_proof_boost_stop(void);

    //
    // poison_proof.cpp
    //
    int poison_proof(void);
    bool is_poison_proof(void);
    void poison_proof_boost_start(int boost);
    void poison_proof_boost_tick(void);
    void poison_proof_boost_stop(void);

    //
    // stone_proof.cpp
    //
    int stone_proof(void);
    bool is_stone_proof(void);
    void stone_proof_boost_start(int boost);
    void stone_proof_boost_tick(void);
    void stone_proof_boost_stop(void);

    //
    // Find out the current "light_strength", taking the boost into account.
    //
    gh_inline int light_strength (void)
    {
        GH_FUNCTION()

        int m = _light_strength + light_strength_boost;

        return (m);
    }

    void light_strength_boost_start(int boost);
    void light_strength_boost_tick(void);
    void light_strength_boost_stop(void);
    int can_see_distance(void);
    void can_see_distance_boost_start(int boost);
    void can_see_distance_boost_tick(void);
    void can_see_distance_boost_stop(void);

    //
    // bite.cpp
    //
    bool met_biter(void);
    bool met_hero(void);
    void try_to_hit(gh_thing *t);

    //
    // insult.cpp
    //
    void insult(gh_thing *target);

    //
    // Hitting
    //
    void hit(gh_thing *t,
             bool *killed,
             int bite_damage,
             int bite_armour,
             int bite_treasure,
             int bite_karma,
             int bite_karma_chance,
             int bite_intelligence,
             int bite_intelligence_chance,
             int bite_stone,
             int bite_stone_chance,
             int bite_poison,
             int bite_poison_chance,
             int bite_fate,
             int bite_fate_chance,
             gh_thing *weapon,
             bool mand_attack);

    //
    // debug.cpp
    //
    void debug(void);

    //
    // collect.cpp
    //
    bool collect(gh_thing *t);
    bool collect_item(gh_thing *t, bool force = false);

    //
    // room.cpp
    //
    void enter_room(gh_thing *t);
    void leave_room(void);

    //
    // shop.cpp
    //
    bool collect_in_shop(gh_thing *t);
    void leave_shop(void);
    bool pay_in_shop(void);
    void destroyed_item_in_shop(gh_thing *t);
    void used_unpaid_item_in_shop(gh_thing *t);
    
    //
    // drop.cpp
    //
    bool drop_item(gh_thing *t, bool force);
    bool drop_item_name(string name);
    void drop_all_items(void);

    //
    // item.cpp
    //
    int16 how_much_am_i_carrying(void);
    bool can_i_carry(gh_thing *t);
    void destroy_all_items(void);
    void reset_all_items(void);
    void heavy_all_items(void);
    void weightless_all_items(void);
    void destroy_item(gh_thing *t);
    
    //
    // util.cpp
    //
    gh_thing *get_owner(void);
    string named(void);
    string the_named(void);
    string char_to_name(string char_or_name);
    string char_to_summon_name(string char_or_name);
    bool matches(const string &s);
    void flash(gh_pixel);
    int hero_distance(void);
    int appearing_chance_on_level(gh_level *l);
    int toughness(void);

    //
    // weapon.cpp
    //
    void wield_next_best(void);
    void unwield(void);
    void rewield(void);
    void wield_new_weapon(gh_thing *weapon);
    void wield_existing_weapon(gh_thing *victim);
    void use_weapon(gh_thing *victim,
                    gh_thing *weapon,
                    bool switch_to_but_dont_activate);

    //
    // use.cpp
    //
    bool use_simple_item(gh_thing *target,
                         gh_thing *item,
                         bool switch_to_but_dont_activate = false);
    bool use_item(gh_thing *t);
    bool use_item_name(string name);

    //
    // actions.cpp
    //
    void hit_all_i_can_see(gh_thing *item, vector<string> action);

    //
    // spell.cpp
    //
    void spell_effect(void);
    void spell_fast(gh_thing *item);
    void spell_vision(gh_thing *item);
    void spell_info(gh_thing *item);
    void spell_weightless(gh_thing *item);
    void spell_throw(gh_thing *item, string what);
    void spell_teleport_self(gh_thing *item);
    void spell_remove_curse(gh_thing *item);
    void spell_heal(gh_thing *item);
    void spell_raise_dead(gh_thing *item);
    void spell_shield(gh_thing *item);
    void spell_invisible(gh_thing *item);

    void reset(void);
    void dead(string cause, bool explode=false, bool drop_things=true);

    //
    // anger.cpp
    //
    void anger_all(gh_thing *target) const;
    void anger(gh_thing *target,
               bool *first_time_angered = NULL,
               bool *first_time_irritated = NULL) const;

    //
    // possess.cpp
    //
    bool possess_monst(gh_thing *monst);
    void collect_followers(void);
    void deposit_followers(void);

    //
    // chasing.cpp
    //
    void found_something_to_chase(gh_thing *t);
    void found_something_to_run_from(gh_thing *t);
    int want_to_follow(gh_thing *t);

    //
    // attack.cpp
    //
    bool possible_to_attack(gh_thing *t);
    int want_to_attack(gh_thing *t);
    void try_to_attack_victim(gh_thing *t);
    void speed_up_attack(gh_thing *);
    void slow_down_attack(gh_thing *);
    void sleep_attack(gh_thing *);
    void paralysis_attack(gh_thing *);
    void bite_attack(gh_thing *);

    //
    // polymorph.cpp
    //
    void polycopy(const gh_thing *thing);
    void polymorph_attack(gh_thing *);

    //
    // throwing.cpp
    //
    list <gh_thing *> try_to_throw_something(gh_thing *target = NULL,
                                             gh_thing *owner = NULL,
                                             string throw_this = "",
                                             bool want_thrown_list = false,
                                             bool always_throw = false);

    void cast_lightning(gh_thing *new_missile,
                        string throw_this,
                        gh_point3d fork_start,
                        int fork_range,
                        float fork_angle,
                        float stddev);

    //
    // teleport.cpp
    //
    bool teleport_attack(gh_thing *victim);
    bool self_teleport(void);
    bool teleport(gh_thing *victim);

    //
    // rage.cpp
    //
    void rage(void);
    int raging(void);
    void raging_boost_start(int boost);
    void raging_boost_tick(void);
    void raging_boost_stop(void);

    //
    // armour.cpp
    //
    bool use_armour(gh_thing *t);
    void hit_armour(int damage);
    int how_much_armour_am_i_wearing(void);

    //
    // proof.cpp
    //
    bool can_rage(void);
    bool can_auto_rage(void);
    bool can_regenerate1(void);
    bool can_regenerate2(void);
    bool can_regenerate3(void);

    float light_ray_length[GH_LIGHT_RAYS_MONST_MAX];

    //
    // wear.cpp
    //
    bool wearer_hates_water(void);
    bool wearer_hates_fire(void);
    bool wearer_hates_acid(void);
    bool wearer_hates_cold(void);
    bool wearer_hates_suffocation(void);
    
    //
    // meet.cpp
    //
    void meet(gh_thing *t);
    void meet_all_at_new_position(void);

    //
    // exit.cpp
    //
    void hero_meet_exit(gh_thing *t);
    void hero_meet_return_exit(gh_thing *t);

    //
    // open.cpp
    //
    void open_this(gh_thing *target);
    void open_nearest_thing(void);
    bool open(gh_thing *initiator);

    //
    // bravery.cpp
    //
    int bravery_level(void);
    void bravery_add_bonus(gh_thing *target);

    //
    // move.cpp
    //
    // Look around, focusing on the direction we want to move, looking for
    // things that satisfy the callback function e.g. looking for things
    // that can be opened. The list returned is a sorted list of the things
    // the player is most likely looking/pointing at. If nothing is directly
    // being pointed at then we look in a circle, starting from the wanted 
    // direction.
    //
    vector <class gh_thing *>
                    get_action_things(gh_thing_evaluator_callback);
    vector <class gh_thing *>
                    get_action_things_in_direction(gh_point3d,
                                                   gh_thing_evaluator_callback);
    vector <class gh_thing *>
                    get_overlapping_things(gh_thing_evaluator_callback);

    bool hit_obstacle(bool ignore_hero);
    bool touching_anything(void);
    bool touching_self(void);
    bool move_delta(gh_point3d delta);
    bool move_to(const gh_point3d to);
    bool try_to_move_in_chosen_direction(bool try_other);
    gh_point3d direction_left(void);
    gh_point3d direction_right(void);
    gh_point3d direction_up(void);
    gh_point3d direction_down(void);
    bool direction_is_left(void);
    bool direction_is_right(void);
    bool direction_is_up(void);
    bool direction_is_down(void);
    bool try_to_move_left(void);
    bool try_to_move_right(void);
    bool try_to_move_up(void);
    bool try_to_move_down(void);
    bool try_to_move_tl(void);
    bool try_to_move_tr(void);
    bool try_to_move_bl(void);
    bool try_to_move_br(void);
    void common_tick(void);
    void tick(void);

    //
    // Hero routines
    //
    void hero_read_event(void);
    void hero_tick(void);

    //
    // Monster routines
    //
    void monst_look_around(void);
    void monst_move(void);
    void monst_choose_new_dir(void);
    void monst_tick(void);

    //
    // Spawn routines
    //
    bool monst_spawn(void);
    bool monst_summon_forced_at(string summon_what, gh_point3d summon_location);
    bool monst_summon_adjacent(string summon_what,
                               bool possess = false);
    bool monst_summon_in_level(string summon_what);
    bool monst_summon_wall(string summon_what,
                           int radius = 1,
                           bool ignore_obstacles = false,
                           bool ignore_monsters = false,
                           bool possess = false);

    //
    // followers.cpp
    //
    bool monst_create_followers(void);
    bool monst_create_limbs(void);
    void follow(gh_thing *thing);
    bool is_part_of_my_body(gh_thing *thing);
    class gh_thing *head(void);
    class gh_thing *tail(void);
    gh_thing *first_limb(void);
    gh_thing *last_limb(void);
    gh_thing *torso(void);
    void move_whole_body(gh_point3d delta);
    void dead_whole_body(string killed_by);

    //
    // Transmog routines
    //
    bool monst_transmog(void);

    //
    // Vision routines
    //
    void make_light_map(int how_many_rays, float *light_ray_length);
    void make_can_see_list(void);
    void what_can_i_see(void);

    //
    // Thing bank operations
    //
    typedef map<string, gh_thing*> hash_thing;
    static hash_thing things;
    static vector<class gh_thing *> things_sorted;

    //
    // Find this thing in the thing bank
    //
    static gh_thing *find(const string &s1, const bool err = true);
    static gh_thing *nth(const int n);
    static gh_thing *nth_sorted(const int n);
    static size_t how_many(void);
    static void load_all(void);
    static void reload_all(void);
    static void unload_all(void);

    //
    // gfx.cpp
    //
    gh_inline gh_anim_frame *get_anim (int level=0)
    {
        if (!anim_frames) {
            return (NULL);
        }

        return (&anim_frames->anim_frame[dir][mode][level][frame]);
    }

    gh_inline int get_anim_size (int level=0)
    {
        if (!anim_frames) {
            return (0);
        }

        return (anim_frames->anim_frame[dir][mode][level].size());
    }

    gh_inline bool is_invisible_anim (void)
    {
        gh_anim_frame *a = get_anim();
        if (!a) {
            return (false);
        }

        return (a->invisible);
    }

    gh_inline bool is_evil (void)
    {
        return (karma <= - 500);
    }

    gh_inline bool is_nice (void)
    {
        return (karma >= 500);
    }

    //
    // parse.cpp
    //
    void parse(const string file, int level);
    void render(gh_point at, int dir, int mode, int frame);
    void get_size(int *w, int *h);
    void console_render(gh_point at, int dir, int mode, int frame);
    void render_cached_all(void);
    void render_cached(const int frame, const int dir, const int mode,
                       const bool must_render_slowly);

    bool parse_block(const int dir,
                     const int mode,
                     const int level,
                     const char **atp,
                     const char*const end);

    //
    // anim.cpp
    //
    bool get_dir_and_mode(gh_thing_dir *dir, gh_thing_mode *mode);
    bool update_anim(void);

    //
    // audio.coo
    //
    void play(string name, int volume=64); // full 128

    //////////////////////////////////////////////////////////////////
    //
    // Fields that we don't save
    //
    //////////////////////////////////////////////////////////////////
    
    const class gh_thing *template_thing;

    //
    // reference counts
    //
    uint32              ref;
    uint32              ref_bitmap;
    uint16              refc[GH_THING_REF_MAX];

    //
    // timers
    //
    uint32              bite_last_when;
    uint32              spawn_last_when;
    uint32              transmog_last_when;
    uint32              teleport_last_when;
    uint32              look_last_when;
    uint32              last_boost_tick_when;
    uint32              quick_tick_when;
    uint32              last_slow_boost_tick_when;
    uint32              born_when;

    //
    // Allows a thing to have its own bit of real estate
    //
    void                *context;

    //
    // Indicates if this thing has been seen in this frame. Save us
    // having to clear them all out each frame.
    //
    uint32              rendered;
    uint32              vision_seen;

    //
    // The thing tick which determines when it can move next.
    //
    static const uint8  tick_max = 101;
    uint8               tick_at;

    //
    // The last time this thing was looked at. Used in loops
    // to make sure we only process the thing once per loop.
    //
    uint32              processed_in_loop:1;

    //////////////////////////////////////////////////////////////////
    //
    // Fields we need to restore upon saving
    //
    //////////////////////////////////////////////////////////////////
    
    //
    // Which level is this thing in?
    //
    class gh_level      *level;

    //
    // What am I currently using.
    //
    class gh_thing      *weapon;

    //
    // Which room is this thing in?
    //
    class gh_room       *room;

    //
    // Which rooms have we been to?
    //
    list<class gh_room *> visited;

    //
    // This list should be empty when we finish the game.
    //
    list<class gh_thing*>::iterator leaked_things_it;

    //
    // Where the thing lives in various level lists
    //

    //
    // All things bright and beautiful.
    //
    list<class gh_thing*>::iterator all_things_it;

    //
    // All things on the map, not necessarily all things though
    //
    list<class gh_thing*>::iterator map_things_it;

    //
    // All things that we run on each tick.
    //
    // So this filters boring stuff like rocks that don't do anything.
    //
    list<class gh_thing*>::iterator does_stuff_things_it;

    //
    // Things not on the map, but in the afterlife
    //
    list<class gh_thing*>::iterator dead_things_it;

    //
    // Who created a thing; usually used by missiles to work out
    // who gets the points for a shot
    //
    class gh_thing      *missile_thrower;

    //
    // Who is wielding me?
    //
    class gh_thing      *weapon_wielder;

    //
    // For example, a shopkeeper.
    //
    class gh_thing      *owner;

    //
    // But who is carrying me?
    //
    class gh_thing      *carrier;

    //
    // Linkeage for snakes and the like. prev points towards the
    // head of the thing and next to the tail.
    //
    class gh_thing      *next;
    class gh_thing      *prev;

    // 
    // In multi limbed creatures, the tail can be chained to another
    // creature.
    //
    class gh_thing      *limb_next;
    class gh_thing      *limb_prev;

    //
    //
    // What am I carrying
    //
    list<gh_thing *>    carrying;

    //
    // Only if hero.
    //
    class gh_player     *player;

    //////////////////////////////////////////////////////////////////
    //
    // And those that we do
    //
    //////////////////////////////////////////////////////////////////
    
    string              name;       // ghost3
    string              text;       // "cave entrance"
    string              debug_string;// "attack ghost"
    string              full_name;  // ../resources/things/monst/ghost/3
    string              short_name; // monst/ghost/3

    //
    // Where the thing is at.
    //
    gh_point3d          at;
    
    //
    // Where the thing started out from and where they return
    // to when they die sometimes.
    //
    gh_point3d          born;

    //
    // How dost thy die?
    //
    string              killed_by;

    //
    // Used to show things being hit and flashing red.
    //
    gh_fpixel           flash_color;

    //
    // Boosts initially bump up the attribute and then tick down to 0.
    //
    int16               lifespan;

    int16               _scared;
    int16               scared_boost;

    int16               _sleeping;
    int16               sleeping_boost;

    int16               _shielded;
    int16               shielded_boost;

    int16               _invisible;
    int16               invisible_boost;

    int16               _raging;
    int16               raging_boost;

    int16               _poisoned;
    int16               poisoned_boost;

    int16               _stoned;
    int16               stoned_boost;

    int16               _metabolism;
    int16               metabolism_boost;

    int16               _water_proof;
    int16               water_proof_boost;

    int16               _fire_proof;
    int16               fire_proof_boost;

    int16               _acid_proof;
    int16               acid_proof_boost;

    int16               _cold_proof;
    int16               cold_proof_boost;

    int16               _suffocation_proof;
    int16               suffocation_proof_boost;

    int16               _teleport_proof;
    int16               teleport_proof_boost;

    int16               _poison_proof;
    int16               poison_proof_boost;

    int16               _stone_proof;
    int16               stone_proof_boost;

    int16               _light_strength;
    int16               light_strength_boost;

    int16               _can_see_distance;
    int16               can_see_distance_boost;

    int16               _stamina;
    int16               stamina_boost;
    int16               orig_stamina;

    //
    // For missiles, we need accurate positioning, which gets rounded
    // down into "at" above.
    //
    gh_fpoint3d         missile_at;
    gh_point3d          missile_target;
    float               missile_angle;
    float               missile_angle_delta;

    //
    // For when you're doing triple shot or more
    //
    uint8               missile_spread;
    uint8               missile_spread_angle;

    //
    // If a weapon is being hit, make its animation spin.
    //
    uint8               weapon_spin;

    //
    // How many limbs does the thing have? This is set on the first head.
    //
    uint8               limbs;

    //
    // Is this thing joined to a neighbour? If so, how?
    //
    gh_thing_dir        join;

    //
    // Where the thing wants to go.
    //
    gh_point3d          target;

    //
    // Where we last saw this target. We use this to determine stuckness.
    //
    gh_point3d          target_last_set_where;
    gh_thing            *target_last_thing;

    //
    // If this thing can explore, here is the maze of cells it has visited
    // along with the age of that cell. Low is old.
    //
    uint32              *cells_visited;

    //
    // This is a forever increasing number that we use to tag cels on
    // the map to indicate 'has been seen recently'.
    //
    uint32              cell_max_age;

    //
    // How old the target cell is. We use this to avoid selecting new
    // targets that are not as good.
    //
    uint32              cell_target_age;

    //
    // Where the thing is currently heading.
    //
    gh_point3d          direction;

    //
    // Where the thing wants to move.
    //
    gh_point3d          direction_wanted;
    gh_point3d          direction_last;

    //
    // Only heroes really amount a score.
    //
    int                 score;

    //
    // How much since we last spent
    //
    int32               bravery_curr;

    //
    // Total disregarding spending
    //
    int32               bravery;

    //
    // How long do we have?
    //
    int16               health;
    int16               orig_health;
    int16               armour;
    uint8               fate;

    //
    // Strength
    //
    int16               _strength;
    int16               strength_boost;

    //
    // How strong you need to be to wield something
    //
    uint8               strength_req;

    //
    // Strength
    //
    int16               _dex;
    int16               dex_boost;

    //
    // How dextrous you need to be to wield something
    //
    uint8               dex_req;

    //
    // Intelligence
    //
    int16               intelligence;
    int16               bite_intelligence;
    int16               bite_intelligence_chance;

    //
    // How smart you need to be to use something
    //
    uint8               intelligence_req;

    //
    // How much it gives you
    //
    int                 treasure;

    //
    // And how much it costs you
    //
    int16               cost;

    //
    // In pounds
    //
    int16               weight;

    //
    // How many we're carrying
    //
    int16               keys;

    //
    // Animation frame.
    //
    uint16              anim;
    uint8               anim_speed;

    //
    // These are cached values from get_dir_and_mode, which is called once
    // per render.
    //
    uint8               frame;
    gh_thing_dir        dir;
    gh_thing_mode       mode;

    //
    // This is the current speed and is bounded by the metabolism.
    //
    uint16              speed;

    //
    // Bite related
    //
    int16               bite_damage;
    int16               bite_armour;
    int16               bite_treasure;
    int16               bite_delay_in_tenths;
    int16               bite_chance;

    //
    // Karma!
    //
    int16               karma;
    int16               bite_karma;
    int16               bite_karma_chance;

    //
    // Poison!
    //
    int16               bite_poison;
    int16               bite_poison_chance;

    int16               bite_stone;
    int16               bite_stone_chance;

    //
    // Fate point bites. Really nasty.
    //
    int16               bite_fate;
    int16               bite_fate_chance;

    //
    // Spawn related; i.e. generators and slime molds.
    //
    int16               spawn_delay_in_tenths;

    //
    // What are the odds of this thing spawning something on this level?
    //
    vector<int>         spawning_chance;

    //
    // What does it spawn?
    //
    vector<string>      spawns;

    //
    // What the thing starts out life with
    //
    vector<string>      backpack;

    //
    // A things followers e.g. snake body
    //
    vector<string>      followers;
    uint8               follow_distance;

    //
    // What are the odds of this thing transmogrify something on this level?
    //
    vector<int>         transmogrify_chance;

    //
    // What does it transmogrify into?
    //
    vector<string>      transmogrifies_into;

    //
    // Transmogrify related; i.e. changing into another thing.
    //
    int16               transmog_delay_in_tenths;

    //
    // Teleport related;
    //
    int16               teleport_delay_in_tenths;

    //
    // What are the odds of this thing teleporting on this level?
    //
    int16               teleport_chance;

    //
    // Which action will we run on using this object
    //
    vector<int>         use_chance;

    //
    // The various actions, one of wich is ran, upon using
    //
    vector<string>      use_action;

    //
    // Complex bite when you attack
    //
    vector<string>      bite_action;

    //
    // How many times this item can be used.
    //
    uint16              charges;

    //
    // Looking around related
    //
    int16               look_delay_in_tenths;

    string              explosion_type;

    //
    // The order to display this thing. 0 being first, 2 being last
    //
    uint8               render_depth;

    //
    // How many levels an exit can push you
    //
    uint8               exit_jump;

    //
    // Or explicitly to which new dimension?
    //
    string              exit_name;

    //
    // A percentage of how much this thing blocks light. Typically,
    // we choose about 25 so that it allows enough light to pass
    // to actually allow the whole thing to be seen; and not just
    // an edge.
    //
    uint8               opacity;

    uint8               level_mod;

    //
    // For objects that appear in clusters - how far apart
    // and how many.
    //
    uint16              cluster_radius;
    uint8               cluster_amount;

    //
    // How far a laser effect burns.
    //
    uint8               laser_range;

    //
    // What are the odds of this thing appearing on this level?
    //
    vector<int>         appearing_chance;

    //
    // What are the odds of this thing throwing something on this level?
    //
    vector<int>         attack_chance;

    //
    // What does it throw at?
    //
    vector<string>      attacks;

    //
    // Things to wake up for.
    //
    vector<string>      wake_reasons;

    //
    // What does it throw at?
    //
    vector<string>      follows;

    //
    // What are the odds of this thing throwing something on this level?
    //
    vector<int>         follow_chance;

    //
    // What does it throw?
    //
    vector<string>      throws;

    //
    // All our frames of animation, shared with the template thing.
    //
    gh_anim             *anim_frames;

    class gh_thing_registration *reg;

    //
    // What to play when this thing is used.
    // 
    string              audio_on_use;

    //
    // What to play when this thing is used.
    // 
    string              audio_on_dead;

    //
    // What to play when this thing is opened.
    // 
    string              audio_on_open;

    //
    // What to play when this thing is hit.
    // 
    string              audio_on_hit;

    //
    // If this is a wands/sword, how to animate it
    //
    string              wield_as;

    //
    // Thing attributes.
    //
    uint32                      cached_gfx:1;
    uint32                      can_cast_spells:1;
    uint32                      can_insult:1;
    uint32                      cannot_summon:1;
    uint32                      can_see_cursed_items:1;
    uint32                      cant_see_past:1;
    uint32                      can_wield_weapons:1;
    uint32                      casts_lightning:1;
    uint32                      collects_stuff_but_doesnt_use_it:1;
    uint32                      curse_noticed:1;
    uint32                      destroy_joined:1;
    uint32                      does_stuff:1;
    uint32                      hates_acid:1;
    uint32                      hates_cold:1;
    uint32                      hates_fire:1;
    uint32                      hates_suffocation:1;
    uint32                      hates_water:1;
    uint32                      is_acid:1;
    uint32                      is_amulet:1;
    uint32                      is_armour:1;
    uint32                      is_auto_item:1;
    uint32                      is_axe:1;
    uint32                      is_body_part:1;
    uint32                      is_boring:1;
    uint32                      is_bouncy:1;
    uint32                      is_bow:1;
    uint32                      is_chained_body_part:1;
    uint32                      is_chargeable:1;
    uint32                      is_closed:1;
    uint32                      is_cold:1;
    uint32                      is_corridor:1;
    uint32                      is_cursed:1;
    uint32                      is_death:1;
    uint32                      is_destroyable:1;
    uint32                      is_dirt:1;
    uint32                      is_door:1;
    uint32                      is_edible:1;
    uint32                      is_edible_eater:1;
    uint32                      is_exit:1;
    uint32                      is_explorer:1;
    uint32                      is_fire:1;
    uint32                      is_floor:1;
    uint32                      is_found_only_in_shops:1;
    uint32                      is_gas:1;
    uint32                      is_generator:1;
    uint32                      is_gun:1;
    uint32                      is_hard:1;
    uint32                      is_hero:1;
    uint32                      is_holy:1;
    uint32                      is_internal:1;
    uint32                      is_inventory_item:1;
    uint32                      is_joinable:1;
    uint32                      is_light_source:1;
    uint32                      is_meat:1;
    uint32                      is_meat_eater:1;
    uint32                      is_missile:1;
    uint32                      is_monst:1;
    uint32                      is_movable:1;
    uint32                      is_open:1;
    uint32                      is_potion:1;
    uint32                      is_return_exit:1;
    uint32                      is_rock:1;
    uint32                      is_saved_thing:1;
    uint32                      is_shopkeeper:1;
    uint32                      is_silver:1;
    uint32                      is_spell:1;
    uint32                      is_staff:1;
    uint32                      is_stolen:1;
    uint32                      is_suffocation:1;
    uint32                      is_sword:1;
    uint32                      is_teleport:1;
    uint32                      is_teleport_attack:1;
    uint32                      is_thief:1;
    uint32                      is_transparent:1;
    uint32                      is_trap:1;
    uint32                      is_treasure:1;
    uint32                      is_treasure_eater:1;
    uint32                      is_unable_to_move:1;
    uint32                      is_undead:1;
    uint32                      is_unique:1;
    uint32                      is_unpaid_for:1;
    uint32                      is_wall:1;
    uint32                      is_wand:1;
    uint32                      is_warhammer:1;
    uint32                      is_water:1;
    uint32                      is_weapon:1;
    uint32                      leaves_corpse:1;
    uint32                      loves_acid:1;
    uint32                      loves_cold:1;
    uint32                      loves_death:1;
    uint32                      loves_fire:1;
    uint32                      loves_friday_13th:1;
    uint32                      loves_full_moon:1;
    uint32                      loves_halloween:1;
    uint32                      loves_midnight:1;
    uint32                      loves_night:1;
    uint32                      loves_void:1;
    uint32                      loves_water:1;
    uint32                      moves_left_right:1;
    uint32                      moves_until_collision:1;
    uint32                      moves_up_down:1;
    uint32                      never_slows_down:1;
    uint32                      not_in_shops:1;
    uint32                      only_in_caves:1;
    uint32                      only_in_dungeons:1;
    uint32                      only_in_forests:1;
    uint32                      only_in_hell:1;
    uint32                      only_in_landscapes:1;
    uint32                      shake_screen_on_death:1;
    uint32                      stay_above_torso:1;
    uint32                      wants_to_hit:1;

    //
    // Can run forever.
    //
    uint32                      tireless:1;

    //
    // Can swing swords forever
    //
    uint32                      tireless_swing:1;

    //
    // Recovers 1 point every second.
    //
    uint32                      ability_can_regenerate1:1;
    uint32                      ability_can_regenerate2:1;
    uint32                      ability_can_regenerate3:1;

    //
    // Can control own teleport
    //
    uint32                      can_self_teleport:1;
    uint32                      can_self_teleport_once_per_level:1;

    //
    // Can go into a rage
    //
    uint32                      ability_can_rage:1;
    uint32                      ability_can_auto_rage:1;

    //
    // Clusters can spill over walls.
    //
    uint32                      cluster_anywhere:1;

    //
    // Status settings. Modify reset() when adding.
    //
    uint32                      is_chasing:1;
    uint32                      is_dead:1;

    //
    // A temporary setting we don't need to save.
    //
    uint32                      is_burdened:1;

    //
    // Has this thing been seen?
    //
    uint32                      has_been_seen:1;

    //
    // Have we been loaded from a file and need restoring of our pointers?
    //
    uint32                      needs_fixup:1;

    //
    // Curse c++ and its lack of a null iterator.
    //
    uint32                      leaked_things_it_valid:1;
    uint32                      all_things_it_valid:1;
    uint32                      is_on_map:1;
    uint32                      does_stuff_things_it_valid:1;
    uint32                      dead_things_it_valid:1;

    //
    // Graphcs stuff
    //
    gh_model_anim_info          anim_info;
    gh_model                    *model;
    gh_surface                  *tex;
    gh_pixel                    pixel;
    float                       rot;
    float                       spin;
    uint32                      draw_as_hashed_cube:1;
    uint32                      casts_shadow:1;
    uint32                      is_solid:1;

    friend void operator>> (gh_stream_in& is, gh_thing *me);
    friend void operator<< (gh_stream_out& os, const gh_thing *me);
};

gh_inline bool is_true (gh_thing *thing, void *arg)
{
    return (true);
}

gh_inline bool is_hard (gh_thing *thing, void *arg)
{
    return (thing->is_hard != 0);
}

gh_inline bool cant_see_past (gh_thing *thing, void *arg)
{
    return (thing->cant_see_past != 0);
}

gh_inline bool does_stuff (gh_thing *thing, void *arg)
{
    return (thing->does_stuff != 0);
}

gh_inline bool is_unable_to_move (gh_thing *thing, void *arg)
{
    return (thing->is_unable_to_move != 0);
}

gh_inline bool is_monst (gh_thing *thing, void *arg)
{
    return (thing->is_monst != 0);
}

gh_inline bool is_destroyable (gh_thing *thing, void *arg)
{
    return (thing->is_destroyable != 0);
}

gh_inline bool is_boring (gh_thing *thing, void *arg)
{
    return (thing->is_boring != 0);
}

gh_inline bool is_corridor (gh_thing *thing, void *arg)
{
    return (thing->is_corridor != 0);
}

gh_inline bool is_floor (gh_thing *thing, void *arg)
{
    return (thing->is_floor != 0);
}

gh_inline bool is_dirt (gh_thing *thing, void *arg)
{
    return (thing->is_dirt != 0);
}

gh_inline bool is_hero (gh_thing *thing, void *arg)
{
    return (thing->is_hero != 0);
}

gh_inline bool is_not_hero (gh_thing *thing, void *arg)
{
    return (!thing->is_hero);
}

gh_inline bool is_teleport (gh_thing *thing, void *arg)
{
    return ((thing->is_teleport != 0) && !thing->sleeping());
}

gh_inline bool is_teleport_awake (gh_thing *thing, void *arg)
{
    return ((thing->is_teleport != 0) && !thing->sleeping());
}

gh_inline bool is_exit (gh_thing *thing, void *arg)
{
    return (thing->is_exit != 0);
}

gh_inline bool is_return_exit (gh_thing *thing, void *arg)
{
    return (thing->is_return_exit != 0);
}

gh_inline bool is_meat (gh_thing *thing, void *arg)
{
    return (thing->is_meat != 0);
}

gh_inline bool is_meat_eater (gh_thing *thing, void *arg)
{
    return (thing->is_meat_eater != 0);
}

gh_inline bool is_edible (gh_thing *thing, void *arg)
{
    return (thing->is_edible != 0);
}

gh_inline bool is_edible_eater (gh_thing *thing, void *arg)
{
    return (thing->is_edible_eater != 0);
}

gh_inline bool is_treasure (gh_thing *thing, void *arg)
{
    return (thing->is_treasure != 0);
}

gh_inline bool is_treasure_eater (gh_thing *thing, void *arg)
{
    return (thing->is_treasure_eater != 0);
}

gh_inline bool is_undead (gh_thing *thing, void *arg)
{
    return (thing->is_undead != 0);
}

//
// collision.cpp
//
extern bool gh_thing_gfx_is_obstacle_internal(gh_thing *a, gh_thing *b);
extern bool gh_thing_gfx_can_touch_internal(gh_thing *a, gh_thing *b);
extern bool gh_thing_gfx_can_hit_internal(gh_thing *a, gh_thing *b);

//
// debug.cpp
//
extern int gh_thing_ref_string_index(const char *str);
extern const char *gh_thing_ref_string(const int index);

//
// map.cpp
//
extern uint32 gh_rendered_this_frame;

typedef void (*for_each_body_part_fn)(class gh_thing *thing, void *arg);

extern void gh_thing_for_each_body_part(gh_thing *t,
                                        for_each_body_part_fn f,
                                        void *arg);
//
// Are the 2 things overlapping?
//
gh_inline bool gh_thing_gfx_is_obstacle (gh_thing *a, gh_thing *b)
{
    GH_FUNCTION()

    if (!a->get_anim_size()) {
        return (false);
    }

    if (!b->get_anim_size()) {
        return (false);
    }

    const gh_anim_frame *anim_a = a->get_anim();
    const gh_anim_frame *anim_b = b->get_anim();

    if (!anim_a->obstacle || !anim_b->obstacle) {
        return (false);
    }

    if (a->is_dead) {
        return (false);
    }

    if (b->is_dead) {
        return (false);
    }

    return (gh_thing_gfx_is_obstacle_internal(a, b));
}

//
// Are the 2 things overlapping?
//
gh_inline bool gh_thing_gfx_can_touch (gh_thing *a, gh_thing *b)
{
    GH_FUNCTION()

    if (!a->get_anim_size()) {
        return (false);
    }

    if (!b->get_anim_size()) {
        return (false);
    }

    const gh_anim_frame *anim_a = a->get_anim();
    const gh_anim_frame *anim_b = b->get_anim();

    if (!anim_a->can_touch || !anim_b->can_touch) {
        return (false);
    }

    if (a->is_dead) {
        return (false);
    }

    if (b->is_dead) {
        return (false);
    }

    return (gh_thing_gfx_can_touch_internal(a, b));
}

//
// Are the 2 things biting? hit mask overlaps touch mask?
//
gh_inline bool gh_thing_gfx_can_hit (gh_thing *a, gh_thing *b)
{
    GH_FUNCTION()

    if (!a->get_anim_size()) {
        return (false);
    }

    if (!b->get_anim_size()) {
        return (false);
    }

    const gh_anim_frame *anim_a = a->get_anim();
    const gh_anim_frame *anim_b = b->get_anim();

    if (!anim_a->can_hit || !anim_b->can_hit) {
        return (false);
    }

    if (a->is_dead) {
        return (false);
    }

    if (b->is_dead) {
        return (false);
    }

    return (gh_thing_gfx_can_hit_internal(a, b));
}

#include "gh_thing_ptr.h"

#endif /* _GH_THING_H_ */
