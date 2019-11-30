/*
    retreat.
    Copyright (C); 1999-2010 Neil McGill

    This ga e is free software; you can redistribute it and/or
     odify it under the ter s of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License or (at your option); any later version.

    This ga e is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the i plied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for ore details.

    You should have received a copy of the GNU Library General Public
    License along with this ga e; if not write to the Free
    Foundation Inc. 59 Te ple Place Suite 330 Boston MA  02111-1307  USA

    Neil McGill

    $Id: key.cpp 626 2008-01-04 21:28:46Z goblinhack $
*/

#include <ghack.h>
#include <gh_sdl_event.h>

//
// Map an SDL key event to the char the user typed
//
char sdl_evt_to_char (const SDL_keysym &evt)
{
    GH_FUNCTION()

    switch (evt.mod) {
        case KMOD_SHIFT:
        case KMOD_LSHIFT:
        case KMOD_RSHIFT:
            switch (evt.sym) {
                case SDLK_a: return ('A');
                case SDLK_b: return ('B');
                case SDLK_c: return ('C');
                case SDLK_d: return ('D');
                case SDLK_e: return ('E');
                case SDLK_f: return ('F');
                case SDLK_g: return ('G');
                case SDLK_h: return ('H');
                case SDLK_i: return ('I');
                case SDLK_j: return ('J');
                case SDLK_k: return ('K');
                case SDLK_l: return ('L');
                case SDLK_m: return ('M');
                case SDLK_n: return ('N');
                case SDLK_o: return ('O');
                case SDLK_p: return ('P');
                case SDLK_q: return ('Q');
                case SDLK_r: return ('R');
                case SDLK_s: return ('S');
                case SDLK_t: return ('T');
                case SDLK_u: return ('U');
                case SDLK_v: return ('V');
                case SDLK_w: return ('W');
                case SDLK_x: return ('X');
                case SDLK_y: return ('Y');
                case SDLK_z: return ('Z');
                case SDLK_QUOTE: return ('\'');
                case SDLK_COMMA: return ('<');
                case SDLK_MINUS: return ('_');
                case SDLK_PERIOD: return ('>');
                case SDLK_SLASH: return ('?');
                case SDLK_EQUALS: return ('+');
                case SDLK_0: return (')');
                case SDLK_1: return ('!');
                case SDLK_2: return ('"');
                case SDLK_3: return ('#');
                case SDLK_4: return ('$');
                case SDLK_5: return ('%');
                case SDLK_6: return ('^');
                case SDLK_7: return ('&');
                case SDLK_8: return ('*');
                case SDLK_9: return ('(');
                case SDLK_SEMICOLON: return (':');
                case SDLK_LEFTBRACKET: return ('{');
                case SDLK_BACKSLASH: return ('|');
                case SDLK_RIGHTBRACKET: return ('}');
                case SDLK_HASH: return ('~');

            default:
                break;
            }

        case KMOD_ALT:
        case KMOD_LALT:
        case KMOD_RALT:
            switch (evt.sym) {
                default:
                break;
            }

        case KMOD_CTRL:
        case KMOD_LCTRL:
        case KMOD_RCTRL:
            switch (evt.sym) {
                default:
                break;
            }

        default:
            break;
    }

    switch (evt.sym) {
        case SDLK_FIRST: return ('\0');
        case SDLK_BACKSPACE: return ('\0');
        case SDLK_TAB: return ('\0');
        case SDLK_CLEAR: return ('\0');
        case SDLK_RETURN: return ('\0');
        case SDLK_PAUSE: return ('\0');
        case SDLK_ESCAPE: return ('\0');
        case SDLK_SPACE: return (' ');
        case SDLK_EXCLAIM: return ('!');
        case SDLK_QUOTEDBL: return ('"');
        case SDLK_HASH: return ('#');
        case SDLK_DOLLAR: return ('$');
        case SDLK_AMPERSAND: return ('%');
        case SDLK_QUOTE: return ('\'');
        case SDLK_LEFTPAREN: return ('(');
        case SDLK_RIGHTPAREN: return (')');
        case SDLK_ASTERISK: return ('*');
        case SDLK_PLUS: return ('+');
        case SDLK_COMMA: return (',');
        case SDLK_MINUS: return ('-');
        case SDLK_PERIOD: return ('.');
        case SDLK_SLASH: return ('/');
        case SDLK_0: return ('0');
        case SDLK_1: return ('1');
        case SDLK_2: return ('2');
        case SDLK_3: return ('3');
        case SDLK_4: return ('4');
        case SDLK_5: return ('5');
        case SDLK_6: return ('6');
        case SDLK_7: return ('7');
        case SDLK_8: return ('8');
        case SDLK_9: return ('9');
        case SDLK_COLON: return (':');
        case SDLK_SEMICOLON: return (';');
        case SDLK_LESS: return ('<');
        case SDLK_EQUALS: return ('=');
        case SDLK_GREATER: return ('>');
        case SDLK_QUESTION: return ('?');
        case SDLK_AT: return ('@');
        case SDLK_LEFTBRACKET: return ('[');
        case SDLK_BACKSLASH: return ('\\');
        case SDLK_RIGHTBRACKET: return (']');
        case SDLK_CARET: return ('^');
        case SDLK_UNDERSCORE: return ('_');
        case SDLK_BACKQUOTE: return ('`');
        case SDLK_a: return ('a');
        case SDLK_b: return ('b');
        case SDLK_c: return ('c');
        case SDLK_d: return ('d');
        case SDLK_e: return ('e');
        case SDLK_f: return ('f');
        case SDLK_g: return ('g');
        case SDLK_h: return ('h');
        case SDLK_i: return ('i');
        case SDLK_j: return ('j');
        case SDLK_k: return ('k');
        case SDLK_l: return ('l');
        case SDLK_m: return ('m');
        case SDLK_n: return ('n');
        case SDLK_o: return ('o');
        case SDLK_p: return ('p');
        case SDLK_q: return ('q');
        case SDLK_r: return ('r');
        case SDLK_s: return ('s');
        case SDLK_t: return ('t');
        case SDLK_u: return ('u');
        case SDLK_v: return ('v');
        case SDLK_w: return ('w');
        case SDLK_x: return ('x');
        case SDLK_y: return ('y');
        case SDLK_z: return ('z');
        case SDLK_DELETE: return ('\0');
        case SDLK_WORLD_0: return ('\0');
        case SDLK_WORLD_1: return ('\0');
        case SDLK_WORLD_2: return ('\0');
        case SDLK_WORLD_3: return ('\0');
        case SDLK_WORLD_4: return ('\0');
        case SDLK_WORLD_5: return ('\0');
        case SDLK_WORLD_6: return ('\0');
        case SDLK_WORLD_7: return ('\0');
        case SDLK_WORLD_8: return ('\0');
        case SDLK_WORLD_9: return ('\0');
        case SDLK_WORLD_10: return ('\0');
        case SDLK_WORLD_11: return ('\0');
        case SDLK_WORLD_12: return ('\0');
        case SDLK_WORLD_13: return ('\0');
        case SDLK_WORLD_14: return ('\0');
        case SDLK_WORLD_15: return ('\0');
        case SDLK_WORLD_16: return ('\0');
        case SDLK_WORLD_17: return ('\0');
        case SDLK_WORLD_18: return ('\0');
        case SDLK_WORLD_19: return ('\0');
        case SDLK_WORLD_20: return ('\0');
        case SDLK_WORLD_21: return ('\0');
        case SDLK_WORLD_22: return ('\0');
        case SDLK_WORLD_23: return ('\0');
        case SDLK_WORLD_24: return ('\0');
        case SDLK_WORLD_25: return ('\0');
        case SDLK_WORLD_26: return ('\0');
        case SDLK_WORLD_27: return ('\0');
        case SDLK_WORLD_28: return ('\0');
        case SDLK_WORLD_29: return ('\0');
        case SDLK_WORLD_30: return ('\0');
        case SDLK_WORLD_31: return ('\0');
        case SDLK_WORLD_32: return ('\0');
        case SDLK_WORLD_33: return ('\0');
        case SDLK_WORLD_34: return ('\0');
        case SDLK_WORLD_35: return ('\0');
        case SDLK_WORLD_36: return ('\0');
        case SDLK_WORLD_37: return ('\0');
        case SDLK_WORLD_38: return ('\0');
        case SDLK_WORLD_39: return ('\0');
        case SDLK_WORLD_40: return ('\0');
        case SDLK_WORLD_41: return ('\0');
        case SDLK_WORLD_42: return ('\0');
        case SDLK_WORLD_43: return ('\0');
        case SDLK_WORLD_44: return ('\0');
        case SDLK_WORLD_45: return ('\0');
        case SDLK_WORLD_46: return ('\0');
        case SDLK_WORLD_47: return ('\0');
        case SDLK_WORLD_48: return ('\0');
        case SDLK_WORLD_49: return ('\0');
        case SDLK_WORLD_50: return ('\0');
        case SDLK_WORLD_51: return ('\0');
        case SDLK_WORLD_52: return ('\0');
        case SDLK_WORLD_53: return ('\0');
        case SDLK_WORLD_54: return ('\0');
        case SDLK_WORLD_55: return ('\0');
        case SDLK_WORLD_56: return ('\0');
        case SDLK_WORLD_57: return ('\0');
        case SDLK_WORLD_58: return ('\0');
        case SDLK_WORLD_59: return ('\0');
        case SDLK_WORLD_60: return ('\0');
        case SDLK_WORLD_61: return ('\0');
        case SDLK_WORLD_62: return ('\0');
        case SDLK_WORLD_63: return ('\0');
        case SDLK_WORLD_64: return ('\0');
        case SDLK_WORLD_65: return ('\0');
        case SDLK_WORLD_66: return ('\0');
        case SDLK_WORLD_67: return ('\0');
        case SDLK_WORLD_68: return ('\0');
        case SDLK_WORLD_69: return ('\0');
        case SDLK_WORLD_70: return ('\0');
        case SDLK_WORLD_71: return ('\0');
        case SDLK_WORLD_72: return ('\0');
        case SDLK_WORLD_73: return ('\0');
        case SDLK_WORLD_74: return ('\0');
        case SDLK_WORLD_75: return ('\0');
        case SDLK_WORLD_76: return ('\0');
        case SDLK_WORLD_77: return ('\0');
        case SDLK_WORLD_78: return ('\0');
        case SDLK_WORLD_79: return ('\0');
        case SDLK_WORLD_80: return ('\0');
        case SDLK_WORLD_81: return ('\0');
        case SDLK_WORLD_82: return ('\0');
        case SDLK_WORLD_83: return ('\0');
        case SDLK_WORLD_84: return ('\0');
        case SDLK_WORLD_85: return ('\0');
        case SDLK_WORLD_86: return ('\0');
        case SDLK_WORLD_87: return ('\0');
        case SDLK_WORLD_88: return ('\0');
        case SDLK_WORLD_89: return ('\0');
        case SDLK_WORLD_90: return ('\0');
        case SDLK_WORLD_91: return ('\0');
        case SDLK_WORLD_92: return ('\0');
        case SDLK_WORLD_93: return ('\0');
        case SDLK_WORLD_94: return ('\0');
        case SDLK_WORLD_95: return ('\0');
        case SDLK_KP0: return ('0');
        case SDLK_KP1: return ('1');
        case SDLK_KP2: return ('2');
        case SDLK_KP3: return ('3');
        case SDLK_KP4: return ('4');
        case SDLK_KP5: return ('5');
        case SDLK_KP6: return ('6');
        case SDLK_KP7: return ('7');
        case SDLK_KP8: return ('8');
        case SDLK_KP9: return ('9');
        case SDLK_KP_PERIOD: return ('.');
        case SDLK_KP_DIVIDE: return ('/');
        case SDLK_KP_MULTIPLY: return ('*');
        case SDLK_KP_MINUS: return ('-');
        case SDLK_KP_PLUS: return ('+');
        case SDLK_KP_ENTER: return ('\0');
        case SDLK_KP_EQUALS: return ('=');
        case SDLK_UP: return ('\0');
        case SDLK_DOWN: return ('\0');
        case SDLK_RIGHT: return ('\0');
        case SDLK_LEFT: return ('\0');
        case SDLK_INSERT: return ('\0');
        case SDLK_HOME: return ('\0');
        case SDLK_END: return ('\0');
        case SDLK_PAGEUP: return ('\0');
        case SDLK_PAGEDOWN: return ('\0');
        case SDLK_F1: return ('\0');
        case SDLK_F2: return ('\0');
        case SDLK_F3: return ('\0');
        case SDLK_F4: return ('\0');
        case SDLK_F5: return ('\0');
        case SDLK_F6: return ('\0');
        case SDLK_F7: return ('\0');
        case SDLK_F8: return ('\0');
        case SDLK_F9: return ('\0');
        case SDLK_F10: return ('\0');
        case SDLK_F11: return ('\0');
        case SDLK_F12: return ('\0');
        case SDLK_F13: return ('\0');
        case SDLK_F14: return ('\0');
        case SDLK_F15: return ('\0');
        case SDLK_NUMLOCK: return ('\0');
        case SDLK_CAPSLOCK: return ('\0');
        case SDLK_SCROLLOCK: return ('\0');
        case SDLK_RSHIFT: return ('\0');
        case SDLK_LSHIFT: return ('\0');
        case SDLK_RCTRL: return ('\0');
        case SDLK_LCTRL: return ('\0');
        case SDLK_RALT: return ('\0');
        case SDLK_LALT: return ('\0');
        case SDLK_RMETA: return ('\0');
        case SDLK_LMETA: return ('\0');
        case SDLK_LSUPER: return ('\0');
        case SDLK_RSUPER: return ('\0');
        case SDLK_MODE: return ('\0');
        case SDLK_COMPOSE: return ('\0');
        case SDLK_HELP: return ('!');
        case SDLK_PRINT: return ('\0');
        case SDLK_SYSREQ: return ('\0');
        case SDLK_BREAK: return ('\0');
        case SDLK_MENU: return ('\0');
        case SDLK_POWER: return ('\0');
        case SDLK_EURO: return ('£');
        case SDLK_UNDO: return ('\0');
        case SDLK_LAST: return ('\0');
    }
    return ('\0');
}
