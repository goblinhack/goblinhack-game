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

    $Id: hit.cpp 1168 2009-02-09 00:17:52Z Ghack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_level.h>
#include <gh_game.h>
#include <gh_templates.h>

//
// One thing is hitting another thing, t
//
void gh_thing::insult (gh_thing *target)
{                       
    static vector<string> a;
    static vector<string> b;
    static vector<string> c;
    static bool first = true;

    if (first) {
        a.push_back("cursed");
        a.push_back("dratted");
        a.push_back("smelly");
        a.push_back("vile");
        a.push_back("unfair");
        a.push_back("nasty");
        a.push_back("loathsome");
        a.push_back("reguritant");
        a.push_back("bileous");
        a.push_back("flatulant");
        a.push_back("incontinent");
        a.push_back("deranged");
        a.push_back("mental");
        a.push_back("blasted");
        a.push_back("thrice-cursed");
        a.push_back("angel-loving");
        a.push_back("pasty-faced");
        a.push_back("feeble");
        a.push_back("pathetic");
        a.push_back("bumbling");
        a.push_back("stumbling");
        a.push_back("fumbling");
        a.push_back("useless");
        a.push_back("irksome");
        a.push_back("annoying");
        a.push_back("half-constructed");
        a.push_back("mad");
        a.push_back("insane");
        a.push_back("crazy");
        a.push_back("green-skinned");
        a.push_back("squirming");
        a.push_back("slime-sucking");
        a.push_back("hairy");
        a.push_back("bepoxed");
        a.push_back("bescurved");
        a.push_back("betrixed");
        a.push_back("orc-brained`");
        a.push_back("bat-bitten`");
        a.push_back("rabid");
        a.push_back("pixie-kicked");
        a.push_back("half-witted");
        a.push_back("smelly");
        a.push_back("stupid");
        a.push_back("ignorant");
        a.push_back("yellow-bellied");
        a.push_back("pink-eyed");
        a.push_back("green-skinned");
        a.push_back("pixie-eared");
        a.push_back("long-haired");
        a.push_back("evil");
        a.push_back("nasty");
        a.push_back("bald");
        a.push_back("fluff-brained");
        a.push_back("semi-brained");
        a.push_back("cross-eyed");

        b.push_back("frog-headed");
        b.push_back("dungeon-pedling");
        b.push_back("bile-faced");
        b.push_back("maze-muddling");
        b.push_back("toad-skinned");
        b.push_back("human");
        b.push_back("primitive");
        b.push_back("barbarian");
        b.push_back("duck-spawned");
        b.push_back("duck-billed");
        b.push_back("goat-horned");
        b.push_back("monkey-spawned");
        b.push_back("orc-spawned");
        b.push_back("cheese-eating");
        b.push_back("blood-sucking");
        b.push_back("sword-polishing");
        b.push_back("sword-worshipping");
        b.push_back("gobbling");
        b.push_back("blithering");
        b.push_back("little");
        b.push_back("diminutive");
        b.push_back("ankle-biting");
        b.push_back("wartish");
        b.push_back("camel-sniffing");
        b.push_back("excuse for a");
        b.push_back("mind-flayed");
        b.push_back("rat-infested");
        b.push_back("dungeon-nit-infested");
        b.push_back("pixie-slapping");
        b.push_back("seething");
        b.push_back("sly");
        b.push_back("hat-wearing");
        b.push_back("gibbering");
        b.push_back("slavering");
        b.push_back("drooling");
        b.push_back("foaming");
        b.push_back("stinking");
        b.push_back("dribbling");
        b.push_back("cowardly");
        b.push_back("cheese-eating");
        b.push_back("spider-eyed");
        b.push_back("cross-boned");
        b.push_back("rat-tailed");
        b.push_back("mammal-raised");

        c.push_back("nerf-herder");
        c.push_back("scoundrel");
        c.push_back("halfblood");
        c.push_back("piece of dragon-dung");
        c.push_back("pile of dragon-dung");
        c.push_back("pile of dragon-droppings");
        c.push_back("bile of demon-spawn");
        c.push_back("heap of demon-droppings");
        c.push_back("heap of bat-droppings");
        c.push_back("fairy-prince");
        c.push_back("fairy-lover");
        c.push_back("elf-lover");
        c.push_back("dragon-dribbler");
        c.push_back("scraping from a troll hole");
        c.push_back("scraping from a orc pit");
        c.push_back("scraping from a kobolds");
        c.push_back("piece of orc muck");
        c.push_back("piece of goblin goo");
        c.push_back("surface-lover");
        c.push_back("goblin-licker");
        c.push_back("troll-licker");
        c.push_back("slime-sucker");
        c.push_back("camel-licker");
        c.push_back("weakling");
        c.push_back("reject from the surface");
        c.push_back("pansy");
        c.push_back("midget");
        c.push_back("eejit");
        c.push_back("nerdling");
        c.push_back("sasenach");
        c.push_back("warthog-impersonator");
        c.push_back("dirge-rat");
        c.push_back("roach");
        c.push_back("witch look-a-like");
        c.push_back("mouth-breather");
        c.push_back("bat-killer");
        c.push_back("dog-killer");
        c.push_back("vampire-spawn");
        c.push_back("bogart");
        c.push_back("bat");
        c.push_back("xenophobe");
        c.push_back("lawyer");
        c.push_back("thingling");
        c.push_back("spider-murderer");
        c.push_back("globuloid");
        c.push_back("goblin-lover");
        c.push_back("slug");

        first = false;
    }

    level->game->new_message(target->the_named() + " calls YOU a " +
                             one_of(a) + " " +
                             one_of(b) + " " +
                             one_of(c));
}
