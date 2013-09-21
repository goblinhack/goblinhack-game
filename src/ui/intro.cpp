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
    Foundation, Inscreen., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Neil McGill

    $Id: intro.cpp 1275 2009-08-07 13:01:53Z Ghack $
*/

#include <ghack.h>
#include <gh_term.h>
#include <gh_ui_event.h>
#include <gh_ui_hiscore.h>
#include <gh_game.h>
#include <gh_figlet.h>
#include <gh_screen.h>
#include <gh_sdl.h>
#include <gh_audio.h>
#include <gh_music.h>
#include <hacklib.h>

namespace {
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
};

static const uint32 intro_section[] = {
    0,
    // game demo
    800,
    // hi score
    1500,
    // ???
    2001,
    // key info
    2502,
    // credits
    5003,
    // credits2
    8003,
    // keys2
    11003,
    // dummy
    20000,
};

static const uint32 intro_section_max =
                intro_section[GH_ARRAY_SIZE(intro_section)-2];;

static gh_myscreen screen;

void gh_myscreen::init (void)
{
    GH_FUNCTION()

    static bool first = true;

    if (first) {
        first = false;
        game->frame = 0;
    } else {
        //
        // Start again at the hiscore when the level ends
        //
        game->frame = intro_section[4];
    }
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

    int x = GH_TERM_WIDTH/2 - 29;
    int y = GH_TERM_HEIGHT/2 - 6;
    int mx = GH_TERM_WIDTH / 2;

    gh_ui_intro_border(game);

    if ((game->frame >= intro_section[1]) &&
        (game->frame < intro_section[2])) {
        gh_ui_display_hiscores(game);
    } else {
        gh_term_fgbg(GH_TERM_COLOR_RANDOM, GH_TERM_COLOR_BLACK);

        if (xmas()) {
            font1->printat(mx - 30, 1, "Happy");
            font1->printat(mx + 5, 1, "Xmas!");
        } else {
            font1->printat(mx - 30, 1, "Goblin");
            font1->printat(mx + 5, 1, "Hack!");
        }
    }

    gh_term_goto(0, GH_TERM_HEIGHT - 3);

    if (gh_global::trace_mode) {
        if (gh_global::debug) {
            gh_term_putfmx("%%fg=yellowTRACE+DEBUG MODE");
        } else {
            gh_term_putfmx("%%fg=yellowTRACE MODE");
        }
    } else if (gh_global::debug) {
        gh_term_putfmx("%%fg=yellowDEBUG MODE");
    } else {
        gh_term_putfmx(
            "%%fg=yellow%%bg=black"
            "Press "
            "%%fg1=blue%%bg1=white"
            "%%fg2=white%%bg2=blue"
            "SPACE"
            "%%fg=yellow%%bg=black"
            " friend to enter the lair of the Goblins!");
    }

    if ((game->frame >= intro_section[0]) &&
        (game->frame < intro_section[3])) {

        y = GH_TERM_HEIGHT - 13;

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx("Version " GH_VERSION " (Fractal Forests!)");
        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx("%%fg=white"
"Goblins hoard in their foul dark hole");
        gh_term_goto(x, y++);
        gh_term_putfmx(
"Plotting and digging and hating all things");
        gh_term_goto(x, y++);
        gh_term_putfmx(
"They'll gnash on your bones and eat you whole");
        gh_term_goto(x, y++);
        gh_term_putfmx(
"And deliver your soul to the Goblin King");
        gh_term_goto(x, y++);
    }

    if ((game->frame >= intro_section[0]) &&
        (game->frame < intro_section[1])) {

        y = 8;

        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);

        gh_term_goto(0, y++);
        gh_term_fgbg(GH_TERM_COLOR_GREEN, GH_TERM_COLOR_BLACK);
        gh_term_putfmx("Thy mighty keys of destiny control:");
        gh_term_goto(0, y++);

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "   Arrow keys              "
        "%%fg=white%%bg=black"
        "to stalk the dungeon       ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "      SPACE                "
        "%%fg=white%%bg=black"
        "to slash, hack, and fire   ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "        o                  "
        "%%fg=white%%bg=black"
        "to open doors              ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "       a,z                 "
        "%%fg=white%%bg=black"
        "to select carried items    ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        " u,ENTER,l-SHIFT           "
        "%%fg=white%%bg=black"
        "use items                  ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "        d                  "
        "%%fg=white%%bg=black"
        "drop items                 ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "        p                  "
        "%%fg=white%%bg=black"
        "pause / pay when in shops  ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);

        if (gh_global::music && gh_global::sound) {
            gh_term_putfmx(
            "%%fg=green%%bg=black"
            "        m                  "
            "%%fg=white%%bg=black"
            "music OFF                  ");
        } else if (!gh_global::music && gh_global::sound) {
            gh_term_putfmx(
            "%%fg=green%%bg=black"
            "        m                  "
            "%%fg=white%%bg=black"
            "sound OFF                  ");
        } else {
            gh_term_putfmx(
            "%%fg=green%%bg=black"
            "        m                  "
            "%%fg=white%%bg=black"
            "music ON                   ");
        }

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);

        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "        e                  "
        "%%fg=white%%bg=black"
        "Level editor mode          ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "       l,s                 "
        "%%fg=white%%bg=black"
        "to load / save             ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "        c                  "
        "%%fg=white%%bg=black"
        "Setup                      ");
        gh_term_goto(0, y++);
        gh_term_goto(0, y++);

        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "        h                  "
        "%%fg=white%%bg=black"
        "Help                       ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
    }

    if ((game->frame >= intro_section[2]) &&
        (game->frame < intro_section[3])) {

        y = 10;

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);

        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "q                          "
        "%%fg=white%%bg=black"
        "To quit...                 ");

        gh_term_goto(0, y++);
    }

    if ((game->frame >= intro_section[3]) &&
        (game->frame < intro_section[4])) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        y = 10;

        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Diabolic Hints and Tricks");
        gh_term_goto(x, y++);

        gh_term_fgbg(GH_TERM_COLOR_GREEN, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Closing doors gives you back keys");

        gh_term_fgbg(GH_TERM_COLOR_GREEN, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Dig through walls to explore and find hidden exits");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "If you run out of keys, doors can be destroyed");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Teleports transport more than just heros.");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Some monsters eat each other.");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Items can be picked up and used against certain monster types.");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Don't leave your slime molds unattended!");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Food can be shot too.");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Some things can steal from you.");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "You can steal too.");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Shopkeepers can actually be killed. It's not easy.");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Watch your karma when angels are about.");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Stay out of the water! Armour can rust.");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Fate points save lives!");
    }

    if ((game->frame >= intro_section[4]) &&
        (game->frame < intro_section[5])) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        y = 7;

        gh_term_goto(0, y++);
        gh_term_putfmx(
        "Credits of Shame");
        gh_term_goto(x, y++);

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "Angela McGill        "
        "%%fg=white%%bg=black"
        "...  goblinhack.sourceforge.net");

        gh_term_goto(x, y++);
        gh_term_goto(x, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "Neil McGill          "
        "%%fg=white%%bg=black"
        "...  Programming goblin        ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "Charlotte + Ben      "
        "%%fg=white%%bg=black"
        "...  Playtesters               ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "Pascal Provost       "
        "%%fg=white%%bg=black"
        "...  Intro and in-game music   ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "And thanks to        "
        "%%fg=white%%bg=black"
        "...  Richard Franks, Joan Stark");

        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "                     "
        "%%fg=white%%bg=black"
        "...  Paul Somebody             ");

        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "                     "
        "%%fg=white%%bg=black"
        "...  alt.ascii-art, Daniel Au  ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "Audio                "
        "%%fg=white%%bg=black"
        "...  AtomSpltterAudio WavSource");

        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "                     "
        "%%fg=white%%bg=black"
        "...  AtomaTwelve SampleSwap    ");

        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "                     "
        "%%fg=white%%bg=black"
        "...  Acllivity The Bizniss     ");

        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "                     "
        "%%fg=white%%bg=black"
        "...  FunkyMuskrat              ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "Fonts                "
        "%%fg=white%%bg=black"
        "...  Figlet development team   ");
    }

    if ((game->frame >= intro_section[5]) &&
        (game->frame < intro_section[6])) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        y = 7;

        gh_term_goto(0, y++);
        gh_term_goto(x, y++);
        gh_term_goto(x, y++);
        gh_term_goto(x, y++);
        gh_term_goto(x, y++);
        gh_term_goto(x, y++);
        gh_term_goto(x, y++);

        gh_term_fgbg(GH_TERM_COLOR_GREEN, GH_TERM_COLOR_BLACK);
        gh_term_goto(x, y++);
        gh_term_putfmx(
        "Any plaguarism of AscII graphics is probable and possible.");
        gh_term_goto(x, y++);
        gh_term_goto(x, y++);
        gh_term_putfmx(
        "If a monster resembles you or something you created,");
        gh_term_goto(x, y++);
        gh_term_putfmx(
        "mail goblinhack@gmail.com.");
        gh_term_goto(x, y++);
        gh_term_goto(x, y++);
        gh_term_goto(x, y++);
        gh_term_putfmx(
        "We're not liable for any damage that this game may cause on your");
        gh_term_goto(x, y++);
        gh_term_putfmx(
        "computer, lifestyle or pet zombie.");

        gh_term_goto(x, y++);
        gh_term_goto(x, y++);
        gh_term_goto(x, y++);
        gh_term_putfmx(
        "Copyright (c) 2006-9 by Neil McGill");
        gh_term_goto(x, y++);
        gh_term_putfmx(
        "Ghack may be freely redistributed. See license for details.");
    }

    if ((game->frame >= intro_section[6]) &&
        (game->frame < intro_section[7])) {

        y = 8;

        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);

        gh_term_goto(0, y++);
        gh_term_fgbg(GH_TERM_COLOR_GREEN, GH_TERM_COLOR_BLACK);
        gh_term_putfmx("Thy lesser known keys of destiny control:");
        gh_term_goto(0, y++);

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "   SHIFT-F11               "
        "%%fg=white%%bg=black"
        "Screendump to local dir    ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "   SHIFT-F12               "
        "%%fg=white%%bg=black"
        "Quick quit                 ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "   ALT-f                   "
        "%%fg=white%%bg=black"
        "Frame counter              ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "   ALT-d                   "
        "%%fg=white%%bg=black"
        "Debug mode (SLOW)          ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=green%%bg=black"
        "   ALT-t                   "
        "%%fg=white%%bg=black"
        "Trace mode (SLOWER)        ");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_goto(0, y++);
        gh_term_putfmx(
        "%%fg=red%%bg=black"
        "Developers needed! Mail "
        "%%fg=red%%bg=black"
        "goblinhack@gmail.com");
    }

    if (game->frame >= intro_section_max) {
        game->frame = 0;
    }

    static int at;
    static int frame;

    static char s[] =
    "%%fg=green"
    "                                                                                                    "
    "Greetings Brave and Foolish Mortal.                "
    "Welcome to %%fg=redGhack.%%fg=green          "
    "                                                                                                    "
    "Look out for dragons, vampires, zombies, demons and worse...   "
    "                               "
    "Collect magical potions and weapons to boost your abilities.   "
    "                               "
    "Fate points are used when you are about to die and will save you from a blow that would have otherwise killed you. Some monsters can steal fate points."
    "                               "
    "Keep an eye on your soul and do not punish the good creatures that you encounter.   "
    "                               "
    "Purchase items from the generous shopkeepers you encounter. But do not encur their wrath.   "
    "                               "
    "%%fg=red"
    "If you are insane enough that you enjoyed this game, consider helping. "
    "Code snippets and ascii monsters are very welcome.   "
    "                               "
    "Many goblins died to bring you this production. Spare a thought for their thankless endeavours.   "
    "                               "
    "%%fg=purple"
    "                                           Go forth and Hack, friend.   "
    "%%fg=green"
    ;

    frame++;

    if (frame > 7) {
        frame = 0;
        at++;

        if (at > (int)strlen(s) + 50) {
            at = 0;
        }
    }

    gh_term_goto(-at, GH_TERM_HEIGHT - 1);
    gh_term_putf(s);
}

int gh_myscreen::key_held (const SDL_keysym & sym)
{
    GH_FUNCTION()

    return (false);
}

int gh_myscreen::key_down (const SDL_keysym & sym)
{
    GH_FUNCTION()

    size_t i;

    switch (sym.sym) {
    case SDLK_LEFT:
        gh_audio::play("Power_On_001");

        for (i = 1; i < GH_ARRAY_SIZE(intro_section)-1; i++) {
            if ((game->frame > intro_section[i]) &&
                (game->frame < intro_section[i+1])) {
                game->frame = intro_section[i-1];
                gh_term_cls();
                break;
            }
        }
        if (i == GH_ARRAY_SIZE(intro_section)-1) {
            game->frame = intro_section[GH_ARRAY_SIZE(intro_section) - 3];
            gh_term_cls();
        }
        break;

    case SDLK_RIGHT:
        gh_audio::play("Power_On_001");

        for (i = 0; i < GH_ARRAY_SIZE(intro_section) - 1; i++) {
            if ((game->frame > intro_section[i]) &&
                (game->frame < intro_section[i+1])) {
                game->frame = intro_section[i+1];
                gh_term_cls();
                break;
            }
        }
        if (i == GH_ARRAY_SIZE(intro_section) - 1) {
            game->frame = intro_section[0];
            gh_term_cls();
        }
        break;

    case 'm':
        if (gh_global::music && gh_global::sound) {
            gh_global::music = false;
            gh_global::sound = true;
        } else if (!gh_global::music && gh_global::sound) {
            gh_global::music = false;
            gh_global::sound = false;
        } else {
            gh_global::music = true;
            gh_global::sound = true;
        }

        if (!gh_global::music) {
            gh_music::halt();
        } else {
            gh_music::play("HackYourWay");
        }
        break;

    case 'e':
        gh_audio::play("Power_On_001");

        ret_code = ((gh_event_ret)'e');
        gh_global::done = true;
        break;

    case 'l':
        gh_audio::play("Power_On_001");

        ret_code = ((gh_event_ret)'l');
        gh_global::done = true;
        break;

    case 'c':
        gh_audio::play("Power_On_001");

        ret_code = ((gh_event_ret)'c');
        gh_global::done = true;
        break;

    case SDLK_ESCAPE:
    case 'q':
        gh_audio::play("Power_On_001");

        ret_code = GH_UI_EVENT_QUIT;
        gh_global::done = true;
        break;

    case 'h':
        gh_audio::play("Power_On_001");

        ret_code = ((gh_event_ret)'h');
        gh_global::done = true;
        break;

    case ' ':
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

    return (false);
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
};

void gh_ui_intro_border (gh_game *game)
{
    GH_FUNCTION()

    static float drip_weight[80];
    static bool drip_on[80];
    uint32 mx = GH_TERM_WIDTH / 2;
    uint32 x;

    for (x = mx - 28; x < mx - 30 + 28; x++) {
        drip_on[x] = true;
    }

    for (x = mx + 6; x < mx + 5 + 22; x++) {
        drip_on[x] = true;
    }

    for (x = 0; x < GH_ARRAY_SIZE(drip_weight) - 1; x++) {
        if (!drip_on[x]) {
            continue;
        }

        if (drip_weight[x] == 0.0) {
            if (december()) {
                drip_weight[x] = -(rand() % 150);
            } else {
                drip_weight[x] = -(rand() % 500);
            }
        }
    }

    for (x = 0; x < GH_ARRAY_SIZE(drip_weight) - 1; x++) {
        if (!drip_on[x]) {
            continue;
        }

        if (december()) {
            drip_weight[x] += 0.0005 * (float)((rand() % 1000)+1);
        } else {
            drip_weight[x] += 0.001 * (float)((rand() % 1000)+1);
        }
    }

    for (x = 0; x < GH_ARRAY_SIZE(drip_weight) - 1; x++) {
        if (!drip_on[x]) {
            continue;
        }

        if (drip_weight[x] > 1.0) {
            if (december()) {
            } else {
                drip_weight[x] += 0.00005 * ((float)(rand() % 1000)+1);
            }
        }

        if (drip_weight[x] > GH_TERM_HEIGHT) {
            drip_weight[x] = -(rand() % 300);
        }
    }

    if (december()) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLACK);
    } else {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
    }

    for (x = 0; x < GH_ARRAY_SIZE(drip_weight) - 1; x++) {
        if (!drip_on[x]) {
            continue;
        }

        if (drip_weight[x] < 5) {
            continue;
        }

        if (((int) drip_weight[x]) >= GH_TERM_HEIGHT - 2) {
            gh_term_goto(x - 3, (int) drip_weight[x]);

            if (december()) {
                gh_term_putc('*');
            } else {
                gh_term_puts("..\\*/..");
            }
        } else if (((int) drip_weight[x]) >= GH_TERM_HEIGHT / 2) {
            gh_term_goto(x, (int) drip_weight[x]);
            if (december()) {
                gh_term_putc('*');
            } else {
                gh_term_putc('|');
            }
        } else if (((int) drip_weight[x]) >= 4) {
            gh_term_goto(x, (int) drip_weight[x]);
            if (december()) {
                gh_term_putc('*');
            } else {
                gh_term_putc('+');
            }
        } else {
            gh_term_goto(x, (int) drip_weight[x]);
            gh_term_putc('O');
        }
    }
}

gh_event_ret gh_ui_intro_loop (gh_game *game)
{
    GH_FUNCTION()

    screen.game = game;
    screen.ret_code = GH_UI_EVENT_NONE;

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
