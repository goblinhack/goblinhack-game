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

    $Id: gh_figlet.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_FIGLET_H_
#define _GH_FIGLET_H_

typedef long inchr;             /* "char" read from stdin */

typedef struct fc {
    inchr ord;
    char **thechar;
    struct fc *next;
} fcharnode;

class figlet {
public:
    fcharnode *fcharlist;
    char **currchar;
    int currcharwidth;
    int previouscharwidth;
    char **outputline;
    int outlinelen;
    inchr *inchrline;
    int inchrlinelen;
    int inchrlinelenlimit;
    int smushoverride;
    int outputwidth;
    int outlinelenlimit;
    char hardblank;
    int charheight;
    int justification;
    int right2left;
    int smushmode;

    figlet(string fontname);
    ~figlet(void);

    void print(const char *s);
    void printat(int x, int y, const char *s);
    void printat(int y, const char *s);
    string sprint(const char *s);

private:
    void skiptoeol(FILE * fp);
    char *myfgets(char *line, int maxlen, FILE * fp);
    void readmagic(FILE * fp, char *magic);
    void figlet_init(void);
    void clearline(void);
    void readfontchar(FILE * file, inchr theord, char *line, int maxlen);
    void linealloc(void);
    void linefree(void);
    void getletter(inchr c);
    char smushem(char lch, char rch);
    int smushamt(void);
    int addchar(inchr c);
    void putstring(char *string);
    string sputstring(char *string);
    void term_putstring(int x, int y, const char *string);
    void printline(void);
    string sprintline(void);
    void term_printline(int x, int y);
};

extern figlet *font1;
extern figlet *font2;

#endif /* _GH_FIGLET_H_ */
