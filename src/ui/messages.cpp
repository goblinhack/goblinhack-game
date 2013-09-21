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

    $Id: messages.cpp 1229 2009-06-29 15:47:55Z Ghack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_term.h>
#include <gh_templates.h>
#include <gh_subsys.h>
#include <gh_player.h>

void gh_game::display_messages (void)
{
    GH_FUNCTION()

    list<gh_game_message>::reverse_iterator i;
    int y = GH_TERM_HEIGHT - 1;
    int cnt = 0;

    for (i = messages.rbegin(); i != messages.rend(); i++) {
        gh_term_goto(0, y);
        y--;

        gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
        gh_term_puts("> ");
        gh_term_fgbg(GH_TERM_COLOR_GREEN, GH_TERM_COLOR_BLACK);

        if (i->count) {
            gh_term_putf((i->message + " (repeated " +
                          tostring((int)i->count) + " times)").c_str());
        } else {
            gh_term_putf(i->message.c_str());
        }

        if (cnt >= 6) {
            break;
        }

        cnt++;
    }
}

gh_game_message::gh_game_message (const string message)
{
    GH_FUNCTION()

    this->message = message;
    created_when = gh_get_time_milli();
    count = 0;
}

void gh_game::new_message (const string message,
                           const bool uniq)
{
    GH_FUNCTION()

    list<gh_game_message>::reverse_iterator i;
    int cnt = 0;

    if (!uniq) {
        for (i = messages.rbegin(); i != messages.rend(); i++) {
            gh_game_message *n = &(*i);

            if (n->message == message) {
                n->count++;
                return;
            }

            if (cnt >= 5) {
                break;
            }

            cnt++;
        }
    }

    //
    // unique message
    //
    gh_game_message m(message);

    messages.push_back(m);

    gh_thing_log_mand(player->thing, "MSG: \"" + message + "\"");
}

void gh_game::replace_last_message (const string message)
{
    if (messages.size()) {
        messages.pop_back();
    }

    messages.push_back(message);
}

string gh_game::get_last_message (void)
{
    if (!messages.size()) {
        return ("");
    }

    list<gh_game_message>::iterator i;

    i = messages.end();
    i--;

    return ((*i).message);
}

void gh_game::delete_all_messages (void)
{
    GH_FUNCTION()

    erase(messages);
}
