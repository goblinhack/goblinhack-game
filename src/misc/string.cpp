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

    $Id: string.cpp 746 2008-03-15 18:33:36Z goblinhack $
*/

#include <ghack.h>

//
// Does a string contain a substring.
//
bool string_contains (const string in, const string pattern)
{
    return (in.find(pattern) != string::npos);
}

//
// Remove all occurences of a pattern in a string
//
void string_replace (string &in, const string pattern, const string with)
{
    string::size_type at = 0;

    while ((at = in.find(pattern)) != string::npos) {
        in.replace(at, pattern.size(), with.c_str(), with.size());
    }
}

//
// Remove the sub string if found at the start of the string
//
bool string_replace_head (string &in, const string pattern, const string with)
{
    string::size_type at = 0;

    if ((at = in.find(pattern)) == 0) {
        in.replace(at, pattern.size(), with.c_str(), with.size());
        return (true);
    }
    return (false);
}

//
// Count occurrences of a character
//
int string_count_char (const string in, const char c)
{
    string::size_type at = 0;
    int cnt = 0;

    while ((at = in.find(c, at)) != string::npos) {
        cnt++;
        at++;
    }
    return (cnt);
}

//
// Remove from the last found char in a string to the end
//
bool string_rremove_to_end (string &in, const char c)
{
    string::size_type at = 0;

    at = in.rfind(c, string::npos);
        
    if (at != string::npos) {
        in.resize(at);
        return (true);
    }
    return (false);
}

//
// Remove from the first found char in a string to the end
//
bool string_remove_to_end (string &in, const char c)
{
    string::size_type at = 0;

    at = in.find(c, 0);
        
    if (at != string::npos) {
        in.replace(at, in.size() - at, "", 0);
        return (true);
    }
    return (false);
}

//
// Remove from the last found char in a string to the beginning
//
bool string_rremove_to_start (string &in, const char c)
{
    string::size_type at = 0;

    at = in.rfind(c, string::npos);
        
    if (at != string::npos) {
        in.replace(0, at + 1, "", 0);
        return (true);
    }
    return (false);
}

//
// Remove from the last found char in a string to the beginning
//
bool string_rremove_to_start (string &in, const string c)
{
    string::size_type at = 0;

    at = in.rfind(c, string::npos);
        
    if (at != string::npos) {
        in.replace(0, at + c.size(), "", 0);
        return (true);
    }
    return (false);
}

//
// Given an array of strings find where our value is
//
int enum_array_value (const char *const array[],
                      const string val, const int def)
{
    int value = 0;

    while (*array != NULL) {
        if (!strcmp(*array, val.c_str())) {
            return (value);
        }
        array++;
        value++;
    }
    return (def);
}

//
// Given a string, split it into arguments, seperated by a char
//
vector<string> string_split (string in, char splitter)
{
    string::iterator p = in.begin();
    vector<string> out;
    string tmp;

    if (in == "") {
        return (out);
    }

    tmp = "";
    while (p != in.end()) {
        char c = *p;

        if (*p == splitter) {
            out.push_back(tmp);
            tmp = "";
        } else {
            tmp += c;
        }

        p++;
    }

    if (tmp != "") {
        out.push_back(tmp);
    }

    return (out);
}

//
// Convert a string to an integer
//
int toint (const string s)
{
    int t;

    istringstream r(s);

    r >> t;

    return (t);
}
