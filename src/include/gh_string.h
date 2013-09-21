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

    $Id: gh_string.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_STRING_H_
#define _GH_STRING_H_

//  
// Does a string contain a substring.
//  
bool string_contains(const string in, const string pattern);

//  
// Remove all occurences of a pattern in a string
//  
void string_replace(string &in, const string pattern, const string with);

//  
// Remove the sub string if found at the start of the string
//  
bool string_replace_head(string &in, const string pattern, const string with);

//
// Count occurrences of a character
//
int string_count_char(const string in, const char c);

//
// Remove from the last found char in a string to the end
//
bool string_rremove_to_end(string &in, const char c);

//
// Remove from the first found char in a string to the end
//
bool string_remove_to_end(string &in, const char c);

//
// Remove from the last found char in a string to the beginning
//
bool string_rremove_to_start(string &in, const char c);

//
// Remove from the last found char in a string to the beginning
//
bool string_rremove_to_start(string &in, const string c);

//
// Given an array of strings find where our value is
//
int enum_array_value(const char *const array[],
                     const string val, const int def);

//
// Given a string, split it into arguments, seperated by space
//
vector<string> string_split(string in, char splitter = ' ');

#endif /* _GH_STRING_H_ */
