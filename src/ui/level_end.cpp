/*
    goblinhack.
    Copyright (C) 1999-2010 Neil McGill

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (look your option) any later version.

    This game is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this game; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Neil McGill

    $Id: level_end.cpp 1275 2009-08-07 13:01:53Z Ghack $
*/

#include <ghack.h>
#include <gh_term.h>
#include <gh_ui_event.h>
#include <gh_figlet.h>
#include <gh_level.h>
#include <gh_player.h>
#include <gh_game.h>
#include <gh_sdl.h>
#include <gh_screen.h>

class gh_myscreen {
public:
    gh_myscreen (void) { }
    ~gh_myscreen (void) { }

    void init(void);
    void fini(void);
    void tick(void);
    void display(void);
    int key_down(const SDL_keysym & key);
    int key_up(const SDL_keysym & key);
    int key_held(const SDL_keysym & key);
    int mouse_up(const size_t button);
    int mouse_down(const size_t button);
    int mouse_held(const size_t button);
    int mouse_movement(const gh_point & p, const gh_point & at);
    void reset(void);

    gh_screen myscreen;
    gh_game *game;
    gh_event_ret ret_code;

    vector<gh_bonus>::iterator start_bonus;
    vector<gh_bonus>::iterator end_bonus;
    vector<gh_bonus>::iterator a_bonus;
    vector<gh_bonus>::iterator curr_bonus;
};

static gh_myscreen screen;

void gh_myscreen::init (void)
{
    GH_FUNCTION()

    game->frame = 0;
}

void gh_myscreen::fini (void)
{
    GH_FUNCTION()
}

void gh_myscreen::tick (void)
{
    GH_FUNCTION()

    game->frame++;
}

void gh_myscreen::display (void)
{
    GH_FUNCTION()

    gh_term_cls();

    gh_level *level = game->player->thing->level;
    int my = GH_TERM_HEIGHT / 2;
    int f = (game->frame + 20) / 20;

    gh_ui_intro_border(game);

    gh_term_fgbg((gh_term_color)((f % (GH_TERM_COLOR_MAX - 2))+1),
                 GH_TERM_COLOR_BLACK);

    if (level->is_last_level) {
        font1->printat(my - 10, "Quest Completed!");

        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        font1->printat(my - 2, "Score:");

        gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
        string score = tostring(game->player->thing->score);
        font1->printat(my + 3, score.c_str());
    } else {
        font1->printat(my - 6, "Level Completed!");

        gh_term_goto(0, my + 2);
        gh_term_putfmx(
            "%%fg=random%%bg=black"
            "---======<<<<<<<*** "
            " Press SPACE if you dare, Mortal! "
            "***>>>>>>======---");
    }

    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
    string score = tostring(game->player->thing->score);
    font1->printat(my + 3, score.c_str());

    //
    // Drain each bonus one at a time.
    //
    if (curr_bonus != end_bonus) {
        //
        // Don't give the player more points than they earned when
        // close to the end of the bonus.
        //
        const int delta = GH_MIN(curr_bonus->score, 131);

        if (curr_bonus->score > 0) {
            game->player->thing->score += delta;
            curr_bonus->score -= delta;
        } else {
            //
            // Onto the next bonus.
            //
            curr_bonus++;
        }
    }

    //
    // Display all bonus applied and being applied.
    //
    a_bonus = start_bonus;
    int y = my + 11;

    for (;;) {
        if (a_bonus == end_bonus) {
            break;
        }

        if (a_bonus->score_orig) {
            gh_term_goto(0, y++);
            gh_term_putfmx(a_bonus->name.c_str());
        }

        if (a_bonus == curr_bonus) {
            break;
        }

        a_bonus++;
    }

    gh_term_goto(0, GH_TERM_HEIGHT - 3);

    if (level->is_last_level) {
        gh_term_putfmx(
            "%%fg=yellow%%bg=black"
            "Press "
            "%%fg1=black%%bg1=white"
            "%%fg2=white%%bg2=black"
            "SPACE"
            "%%fg=yellow%%bg=black"
            " to easily beat another quest");
    }
}

int gh_myscreen::key_held (const SDL_keysym & sym)
{
    GH_FUNCTION()

    return (true);
}

int gh_myscreen::key_down (const SDL_keysym & sym)
{
    GH_FUNCTION()

    gh_level *level = game->player->thing->level;

    switch (sym.sym) {

    case SDLK_ESCAPE:
    case 'q':
        //
        // Drain the bonuses
        //
        while (curr_bonus != end_bonus) {
            game->player->thing->score += curr_bonus->score;
            curr_bonus++;
        }

        ret_code = GH_UI_EVENT_RESTART;
        gh_global::done = true;
        break;

    case ' ':
        //
        // Drain the bonuses
        //
        while (curr_bonus != end_bonus) {
            game->player->thing->score += curr_bonus->score;
            curr_bonus++;
        }

        //
        // We can't go past the last level.
        //
        if (level->is_last_level) {
            ret_code = GH_UI_EVENT_RESTART;
            gh_global::done = true;
            break;
        }

        ret_code = GH_UI_EVENT_CONTINUE;
        gh_global::done = true;
        break;

    default:
        break;
    }

    return (false);
}

int gh_myscreen::key_up (const SDL_keysym & sym)
{
    GH_FUNCTION()

    return (true);
}

int gh_myscreen::mouse_up (const size_t button)
{
    GH_FUNCTION()

    return (true);
}

int gh_myscreen::mouse_down (const size_t button)
{
    GH_FUNCTION()

    return (true);
}

int gh_myscreen::mouse_held (const size_t button)
{
    GH_FUNCTION()

    return (true);
}

int gh_myscreen::mouse_movement (const gh_point & rel, const gh_point &at)
{
    GH_FUNCTION()

    gh_fpoint f_at(
                  ((float)at.x / (float)gh_screen::vid.w),
            1.0 - ((float)at.y / (float)gh_screen::vid.h));

    gh_fpoint pixel_at(
            (int)(f_at.x * gh_screen::vid.w),
            (int)(f_at.y * gh_screen::vid.h));

    return (false);
}

void gh_myscreen::reset (void)
{
    GH_FUNCTION()
}

gh_event_ret gh_ui_level_end_loop (gh_game *game)
{
    GH_FUNCTION()

    screen.game = game;
    screen.ret_code = GH_UI_EVENT_NONE;

    gh_level *level = game->player->thing->level;

    //
    // No bonuses on the end of this level? Just carry on.
    //
    // This happens if the exit is already open.
    //
    if (!level->bonuses.size()) {
        return (GH_UI_EVENT_CONTINUE);
    }

    screen.start_bonus = level->bonuses.begin();
    screen.end_bonus = level->bonuses.end();
    screen.a_bonus = screen.start_bonus;
    screen.curr_bonus = screen.start_bonus;

    screen.myscreen.name = __FILE__;
    screen.myscreen.classbase = &screen;
    screen.myscreen.priority = 100;

    screen.myscreen.init =
        (gh_screen::gh_screen_init)&gh_myscreen::init;
    screen.myscreen.fini =
        (gh_screen::gh_screen_fini)&gh_myscreen::fini;
    screen.myscreen.tick =
        (gh_screen::gh_screen_tick)&gh_myscreen::tick;
    screen.myscreen.display =
        (gh_screen::gh_screen_display)&gh_myscreen::display;
    screen.myscreen.reset =
        (gh_screen::gh_screen_reset)&gh_myscreen::reset;
    screen.myscreen.key_down =
        (gh_screen::gh_screen_key_down)&gh_myscreen::key_down;
    screen.myscreen.key_held =
        (gh_screen::gh_screen_key_held)&gh_myscreen::key_held;
    screen.myscreen.mouse_up =
        (gh_screen::gh_screen_mouse_up)&gh_myscreen::mouse_up;
    screen.myscreen.mouse_down =
        (gh_screen::gh_screen_mouse_down)&gh_myscreen::mouse_down;
    screen.myscreen.mouse_held =
        (gh_screen::gh_screen_mouse_held)&gh_myscreen::mouse_held;
    screen.myscreen.mouse_movement =
        (gh_screen::gh_screen_mouse_movement)&gh_myscreen::mouse_movement;

    screen.myscreen.show();

    gh_term_cls();

    gh_global::done = false;

    sdl_loop();

    gh_global::done = false;

    screen.myscreen.hide();

    return (screen.ret_code);
}
