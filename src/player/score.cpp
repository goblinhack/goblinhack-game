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

    $Id: tick.cpp 3 2006-08-25 14:46:56Z goblinhack $
*/

#include <ghack.h>
#include <gh_player.h>
#include <gh_term.h>
#include <gh_thing.h>
#include <gh_level.h>
#include <gh_figlet.h>

void gh_player::display_score (void)
{
    GH_FUNCTION()

    if (!thing) {
        return;
    }

    char tmp_str[20];
    int score_at = GH_TERM_WIDTH - 22;
    int health_at = (GH_TERM_WIDTH / 2) - 11;

    //
    // Health / Armour
    //
    gh_term_goto(health_at, 0);
    gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
    gh_term_puts("Health Armour");

    gh_term_goto(health_at, 1);

    int armour = thing->how_much_armour_am_i_wearing();
    if (!armour) {
        gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
        sprintf(tmp_str, "  %03d   ", thing->health);
        gh_term_puts(tmp_str);
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_RED);
        sprintf(tmp_str, "%03d", armour);
        gh_term_puts(tmp_str);
    } else {
        gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
        sprintf(tmp_str, "  %03d   %03d", thing->health, armour);
        gh_term_puts(tmp_str);
    }

    int y = 0;

    //
    // Hack / level
    //
    gh_term_goto(score_at, y++);
    gh_term_fgbg(GH_TERM_COLOR_PURPLE, GH_TERM_COLOR_BLACK);
    sprintf(tmp_str, "Ghack!  %03d", level_no);
    gh_term_puts(tmp_str);

    y++;

    //
    // Score / Treasure
    //
    gh_term_goto(score_at, y++);
    gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
    gh_term_puts("Score   Zorkmids");

    gh_term_goto(score_at, y++);
    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
    sprintf(tmp_str, "%07d  %07d", thing->score, thing->treasure);
    gh_term_puts(tmp_str);

    y++;

    //
    // Strength / Weight
    //
    gh_term_goto(score_at, y++);
    gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
    gh_term_puts("Strength  Weight");

    gh_term_goto(score_at, y++);
    if (thing->is_burdened) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_RED);
    } else {
        gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
    }

    sprintf(tmp_str, "%3d lbs  %3d lbs",
            thing->strength(),
            thing->how_much_am_i_carrying());
    gh_term_puts(tmp_str);

    y++;

    //
    // Fate
    //
    gh_term_goto(score_at, y++);
    gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
    gh_term_puts("Fate     ");

    if (!thing->fate) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_RED);
    } else {
        gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
    }

    sprintf(tmp_str, "%3d", thing->fate);
    gh_term_puts(tmp_str);

    //
    // Experience
    //
    gh_term_goto(score_at, y++);
    gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
    gh_term_puts("Bravery");
    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);

    sprintf(tmp_str, "%5d", thing->bravery_curr);
    gh_term_puts(tmp_str);

    //
    // Stamina
    //
    string stamina_str;

    gh_term_goto(score_at, y++);
    gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
    gh_term_puts("Stamina ");
    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);

    sprintf(tmp_str, "%4d ", thing->stamina() / 10);
    gh_term_puts(tmp_str);

    if (thing->stamina() <= 2) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLACK);
        stamina_str = "ghastly";
    } else if (thing->stamina() <= 5) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_RED);
        stamina_str = "terrible";
    } else if (thing->stamina() <= 100) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        stamina_str = "vlow";
    } else if (thing->stamina() <= 200) {
        gh_term_fgbg(GH_TERM_COLOR_ORANGE, GH_TERM_COLOR_BLACK);
        stamina_str = "low";
    } else if (thing->stamina() <= 300) {
        gh_term_fgbg(GH_TERM_COLOR_ORANGE, GH_TERM_COLOR_BLACK);
        stamina_str = "tired";
    } else if (thing->stamina() <= 400) {
        stamina_str = "normal";
    } else if (thing->stamina() <= 500) {
        gh_term_fgbg(GH_TERM_COLOR_CYAN, GH_TERM_COLOR_BLACK);
        stamina_str = "great";
    } else if (thing->stamina() <= 600) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLACK);
        stamina_str = "superb";
    } else if (thing->stamina() <= 800) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLACK);
        stamina_str = "bionic";
    } else {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLACK);
        stamina_str = "legendary";
    }

    strcpy(tmp_str, stamina_str.c_str());
    gh_term_puts(tmp_str);

    //
    // Dexterity
    //
    string dex_str;

    gh_term_goto(score_at, y++);
    gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
    gh_term_puts("Skill   ");
    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);

    sprintf(tmp_str, "%4d ", thing->dex());
    gh_term_puts(tmp_str);

    if (thing->dex() <= 2) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        dex_str = "ghastly";
    } else if (thing->dex() <= 5) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        dex_str = "awful";
    } else if (thing->dex() <= 10) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        dex_str = "vlow";
    } else if (thing->dex() <= 20) {
        gh_term_fgbg(GH_TERM_COLOR_ORANGE, GH_TERM_COLOR_BLACK);
        dex_str = "low";
    } else if (thing->dex() <= 30) {
        gh_term_fgbg(GH_TERM_COLOR_ORANGE, GH_TERM_COLOR_BLACK);
        dex_str = "clumsy";
    } else if (thing->dex() <= 40) {
        gh_term_fgbg(GH_TERM_COLOR_ORANGE, GH_TERM_COLOR_BLACK);
        dex_str = "awkward";
    } else if (thing->dex() <= 50) {
        dex_str = "normal";
    } else if (thing->dex() <= 60) {
        gh_term_fgbg(GH_TERM_COLOR_CYAN, GH_TERM_COLOR_BLACK);
        dex_str = "good";
    } else if (thing->dex() <= 80) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLACK);
        dex_str = "accurate";
    } else if (thing->dex() <= 90) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLACK);
        dex_str = "superb";
    } else {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLACK);
        dex_str = "legendary";
    }

    strcpy(tmp_str, dex_str.c_str());
    gh_term_puts(tmp_str);

    //
    // Karma
    //
    string karma_str;

    gh_term_goto(score_at, y++);
    gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
    gh_term_puts("Karma   ");
    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);

    sprintf(tmp_str, "%4d ", thing->karma);
    gh_term_puts(tmp_str);

    if (thing->karma <= -900) {
        gh_term_fgbg(GH_TERM_COLOR_BLACK, GH_TERM_COLOR_WHITE);
        karma_str = "diabolic";
    } else if (thing->karma <= -800) {
        gh_term_fgbg(GH_TERM_COLOR_DARK_RED, GH_TERM_COLOR_WHITE);
        karma_str = "demonic";
    } else if (thing->karma <= -700) {
        gh_term_fgbg(GH_TERM_COLOR_DARK_RED, GH_TERM_COLOR_BLACK);
        karma_str = "brutal";
    } else if (thing->karma <= -600) {
        gh_term_fgbg(GH_TERM_COLOR_DARK_RED, GH_TERM_COLOR_BLACK);
        karma_str = "evil";
    } else if (thing->karma <= -500) {
        gh_term_fgbg(GH_TERM_COLOR_DARK_RED, GH_TERM_COLOR_BLACK);
        karma_str = "dastardly";
    } else if (thing->karma <= -400) {
        gh_term_fgbg(GH_TERM_COLOR_DARK_RED, GH_TERM_COLOR_BLACK);
        karma_str = "scheming";
    } else if (thing->karma <= -300) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        karma_str = "nasty";
    } else if (thing->karma <= -200) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        karma_str = "surly";
    } else if (thing->karma <= -150) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        karma_str = "testy";
    } else if (thing->karma <= -100) {
        gh_term_fgbg(GH_TERM_COLOR_BROWN, GH_TERM_COLOR_BLACK);
        karma_str = "bad";
    } else if (thing->karma <= -50) {
        gh_term_fgbg(GH_TERM_COLOR_ORANGE, GH_TERM_COLOR_BLACK);
        karma_str = "mean";
    } else if (thing->karma <= -10) {
        gh_term_fgbg(GH_TERM_COLOR_ORANGE, GH_TERM_COLOR_BLACK);
        karma_str = "grumpy";
    } else if (thing->karma >= 900) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLACK);
        karma_str = "angelic";
    } else if (thing->karma >= 800) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLACK);
        karma_str = "saintly";
    } else if (thing->karma >= 500) {
        gh_term_fgbg(GH_TERM_COLOR_CYAN, GH_TERM_COLOR_BLACK);
        karma_str = "holy";
    } else if (thing->karma >= 300) {
        gh_term_fgbg(GH_TERM_COLOR_CYAN, GH_TERM_COLOR_BLACK);
        karma_str = "goodly";
    } else if (thing->karma >= 200) {
        gh_term_fgbg(GH_TERM_COLOR_PINK, GH_TERM_COLOR_BLACK);
        karma_str = "nice";
    } else if (thing->karma >= 100) {
        gh_term_fgbg(GH_TERM_COLOR_PINK, GH_TERM_COLOR_BLACK);
        karma_str = "sweet";
    } else {
        karma_str = "neutral";
    }

    strcpy(tmp_str, karma_str.c_str());
    gh_term_puts(tmp_str);

    //
    // Intelligence
    //
    string intelligence_str;

    gh_term_goto(score_at, y++);
    gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
    gh_term_puts("Brain   ");
    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);

    if (thing->intelligence <= 0) {
        intelligence_str = "flatline";
    } else if (thing->intelligence <= 10) {
        intelligence_str = "turnip";
    } else if (thing->intelligence <= 20) {
        intelligence_str = "peanut";
    } else if (thing->intelligence <= 30) {
        intelligence_str = "chimp";
    } else if (thing->intelligence <= 40) {
        intelligence_str = "silly";
    } else if (thing->intelligence <= 50) {
        intelligence_str = "normal";
    } else if (thing->intelligence <= 60) {
        intelligence_str = "smart";
    } else if (thing->intelligence <= 70) {
        intelligence_str = "clever";
    } else if (thing->intelligence <= 80) {
        intelligence_str = "cunning";
    } else {
        intelligence_str = "weasel";
    }

    sprintf(tmp_str, "%4d %s", thing->intelligence, intelligence_str.c_str());
    gh_term_puts(tmp_str);

    //
    // Keys
    //
    string keys_str;

    gh_term_goto(score_at, y++);
    gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
    gh_term_puts("Keys    ");

    if (!thing->keys) {
        gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_RED);
    } else {
        gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
    }

    sprintf(tmp_str, "%4d", thing->keys);
    gh_term_puts(tmp_str);

    y = 0;

    gh_term_fgbg(GH_TERM_COLOR_BLUE, GH_TERM_COLOR_BLACK);

    if (thing->is_burdened) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("BURDENED");
    }

    if (thing->is_thief) {
        gh_term_goto(0, y++);
        gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
        gh_term_puts("THIEF");
    }

    if (thing->is_cursed) {
        gh_term_goto(0, y++);
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        gh_term_puts("CURSED");
    }

    int proof = thing->fire_proof();
    proof = min(100, proof);
    if (proof > 0) {
        sprintf(tmp_str, "%3d%% ", proof);
        gh_term_puts(tmp_str);
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("FIRE-PROOF");
    }

    proof = thing->teleport_proof();
    proof = min(100, proof);
    if (proof > 0) {
        sprintf(tmp_str, "%3d%% ", proof);
        gh_term_puts(tmp_str);
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("TELEPORT-PROOF");
    }

    proof = thing->cold_proof();
    proof = min(100, proof);
    if (proof > 0) {
        sprintf(tmp_str, "%3d%% ", proof);
        gh_term_puts(tmp_str);
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("COLD-PROOF");
    }

    proof = thing->suffocation_proof();
    proof = min(100, proof);
    if (proof > 0) {
        sprintf(tmp_str, "%3d%% ", proof);
        gh_term_puts(tmp_str);
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("SUFFOCATION-PROOF");
    }

    proof = thing->water_proof();
    proof = min(100, proof);
    if (proof > 0) {
        sprintf(tmp_str, "%3d%% ", proof);
        gh_term_puts(tmp_str);
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("WATER-PROOF");
    }

    proof = thing->poison_proof();
    proof = min(100, proof);
    if (proof > 0) {
        sprintf(tmp_str, "%3d%% ", proof);
        gh_term_puts(tmp_str);
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("POISON-PROOF");
    }

    proof = thing->stone_proof();
    proof = min(100, proof);
    if (proof > 0) {
        sprintf(tmp_str, "%3d%% ", proof);
        gh_term_puts(tmp_str);
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("STONE-PROOF");
    }

    proof = thing->acid_proof();
    proof = min(100, proof);
    if (proof > 0) {
        sprintf(tmp_str, "%3d%% ", proof);
        gh_term_puts(tmp_str);
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("ACID-PROOF");
    }

    if (thing->tireless_swing) {
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("TIRELESS-SWING");
    }

    if (thing->tireless) {
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("TIRELESS");
    }

    if (thing->can_see_cursed_items) {
        gh_term_goto(0, y++);
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_puts("DETECT-CURSED");
    }

    if (thing->can_regenerate3()) {
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("CAN-REGENERATE III");
    } else if (thing->can_regenerate2()) {
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("CAN-REGENERATE II");
    } else if (thing->can_regenerate1()) {
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("CAN-REGENERATE");
    }

    if (thing->can_self_teleport) {
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("CAN-TELEPORT (t)");
    }

    if (thing->can_rage()) {
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("CAN-RAGE (r)");
    }

    if (thing->can_auto_rage()) {
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("AUTO-RAGE (< 1/3 Health)");
    }

    if (thing->raging()) {
        gh_term_fgbg(GH_TERM_COLOR_ORANGE, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);
        gh_term_puts("RAGING GRR!!!");
    }

    if (thing->shielded()) {
        gh_term_fgbg(GH_TERM_COLOR_ORANGE, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);

        sprintf(tmp_str, "SHIELDED (%d)", thing->shielded());
        gh_term_puts(tmp_str);
    }

    if (thing->invisible()) {
        gh_term_fgbg(GH_TERM_COLOR_ORANGE, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);

        sprintf(tmp_str, "INVISIBLE (%d)", thing->invisible());
        gh_term_puts(tmp_str);
    }

    if (thing->poisoned()) {
        gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);

        sprintf(tmp_str, "POISONED (%d)", thing->poisoned());
        gh_term_puts(tmp_str);
    }

    if (thing->stoned()) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);

        sprintf(tmp_str, "TURNING TO STONE!!!");
        gh_term_puts(tmp_str);
    }

    if (thing->level && thing->level->time_left) {
        gh_term_fgbg(GH_TERM_COLOR_ORANGE, GH_TERM_COLOR_BLACK);
        font1->printat(4, (tostring(thing->level->time_left)).c_str());
    }

    if (gh_global::debug) {
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, y++);

        sprintf(tmp_str, "DEBUG MODE");
        gh_term_puts(tmp_str);
    }
}
