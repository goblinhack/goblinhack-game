/****************************************************************************

  FIGlet Copyright 1991, 1993, 1994 Glenn Chappell and Ian Chai
  FIGlet Copyright 1996, 1997, 1998, 1999, 2000, 2001 John Cowan
  FIGlet Copyright 2002 Christiaan Keet
  Portions written by Paul Burton and Christiaan Keet
  Internet: <info@figlet.org>
  FIGlet, along with the various FIGlet fonts and documentation, is
    copyrighted under the provisions of the Artistic License (as listed
    in the file "Artistic-license.txt" which is included in this package.
****************************************************************************/

#define DATE "13 July 2002"
#define VERSION "2.2.1"
#define VERSION_INT 20201

#include <gh_sys.h>
#include <gh_figlet.h>
#include <gh_term.h>

#define FONTFILESUFFIX ".flf"
#define FONTFILEMAGICNUMBER "flf2"
#define FSUFFIXLEN strlen(FONTFILESUFFIX)
#define DEFAULTCOLUMNS 512

/****************************************************************************

  Globals dealing with chars that are read

****************************************************************************/

static inchr deutsch[7] = { 196, 214, 220, 228, 246, 252, 223 };

  /*
   * Latin-1 codes for German letters, respectively:
   * LATIN CAPITAL LETTER A WITH DIAERESIS = A-umlaut
   * LATIN CAPITAL LETTER O WITH DIAERESIS = O-umlaut
   * LATIN CAPITAL LETTER U WITH DIAERESIS = U-umlaut
   * LATIN SMALL LETTER A WITH DIAERESIS = a-umlaut
   * LATIN SMALL LETTER O WITH DIAERESIS = o-umlaut
   * LATIN SMALL LETTER U WITH DIAERESIS = u-umlaut
   * LATIN SMALL LETTER SHARP S = ess-zed
   */

using namespace::std;

/****************************************************************************

  Globals dealing with command file storage

****************************************************************************/

typedef struct cfn
{
    char *thename;
    struct cfn *next;
} cfnamenode;

typedef struct cm
{
    int thecommand;
    inchr rangelo;
    inchr rangehi;
    inchr offset;
    struct cm *next;
} comnode;

/****************************************************************************

  Globals affected by command line options

****************************************************************************/


#define SM_SMUSH 128
#define SM_KERN 64
#define SM_EQUAL 1
#define SM_LOWLINE 2
#define SM_HIERARCHY 4
#define SM_PAIR 8
#define SM_BIGX 16
#define SM_HARDBLANK 32

#define SMO_NO 0                /* no command-line smushmode */
#define SMO_YES 1               /* use command-line smushmode, ignore font smushmode */
#define SMO_FORCE 2             /* logically OR command-line and font smushmodes */



/****************************************************************************

  skiptoeol

  Skips to the end of a line, given a stream.  Handles \r, \n, or \r\n.

****************************************************************************/

void figlet::skiptoeol (FILE * fp)
{
    int dummy;

    while (dummy = fgetc(fp), dummy != EOF) {
        if (dummy == '\n')
            return;
        if (dummy == '\r') {
            dummy = fgetc(fp);
            if (dummy != EOF && dummy != '\n')
                ungetc(dummy, fp);
            return;
        }
    }
}


/****************************************************************************

  myfgets

  Local version of fgets.  Handles \r, \n, and \r\n terminators.

****************************************************************************/

char *figlet::myfgets (char *line, int maxlen, FILE * fp)
{
    int c = 0;
    char *p;

    p = line;
    while ((c = fgetc(fp)) != EOF && maxlen) {
        *p++ = c;
        maxlen--;
        if (c == '\n')
            break;
        if (c == '\r') {
            c = fgetc(fp);
            if (c != EOF && c != '\n')
                ungetc(c, fp);
            *(p - 1) = '\n';
            break;
        }
    }
    *p = 0;
    return (c == EOF) ? NULL : line;
}

/****************************************************************************

  readmagic

  Reads a four-character magic string from a stream.

****************************************************************************/
void figlet::readmagic (FILE * fp, char *magic)
{
    int i;

    for (i = 0; i < 4; i++) {
        magic[i] = fgetc(fp);
    }
    magic[4] = 0;
}

/****************************************************************************

  figlet_init

  Handles all command-line parameters.  Puts all parameters within
  bounds.

****************************************************************************/

void figlet::figlet_init (void)
{
    fcharlist = 0;
    currchar = 0;
    currcharwidth = 0;
    previouscharwidth = 0;
    outputline = 0;
    outlinelen = 0;
    inchrline = 0;
    inchrlinelen = 0;
    inchrlinelenlimit = 0;
    hardblank = 0;
    charheight = 0;
    smushoverride = SMO_NO;
    justification = -1;
    right2left = -1;
    outputwidth = DEFAULTCOLUMNS;
    outlinelenlimit = outputwidth - 1;
    justification = 0;
    right2left = 0;
    smushmode = 0;
}


/****************************************************************************

  clearline


****************************************************************************/

void figlet::clearline (void)
{
    int i;

    for (i = 0; i < charheight; i++) {
        outputline[i][0] = '\0';
    }
    outlinelen = 0;
    inchrlinelen = 0;
}


/****************************************************************************

  readfontchar

  Reads a font character from the font file, and places it in a
  newly-allocated entry in the list.

****************************************************************************/

void figlet::readfontchar (FILE * file, inchr theord, char *line, int maxlen)
{
    int row, k;
    char endchar;
    fcharnode *fclsave;

    fclsave = fcharlist;
    fcharlist = (fcharnode *) malloc(sizeof (fcharnode));
    fcharlist->ord = theord;
    fcharlist->thechar = (char **) malloc(sizeof (char *) * charheight);
    fcharlist->next = fclsave;
    for (row = 0; row < charheight; row++) {
        if (myfgets(line, maxlen + 1, file) == NULL) {
            line[0] = '\0';
        }
        k = strlen(line) - 1;
        while (k >= 0 && isspace(line[k])) {
            k--;
        }
        if (k >= 0) {
            endchar = line[k];
            while (k >= 0 ? line[k] == endchar : 0) {
                k--;
            }
        }
        line[k + 1] = '\0';
        fcharlist->thechar[row] = (char *) malloc(sizeof (char) * (k + 2));
        strcpy(fcharlist->thechar[row], line);
    }
}

/****************************************************************************

  readfont

  Allocates memory, initializes variables, and reads in the font.
  Called near beginning of main().

****************************************************************************/

figlet::figlet (string fontname)
{
    figlet_init();

#define MAXFIRSTLINELEN 1000
    int i, row, numsread;
    inchr theord;
    int maxlen, cmtlines, ffright2left;
    int smush, smush2;
    char *fileline, magicnum[5];
    FILE *fontfile;

    fontfile = NULL;

    fontfile = fopen(fontname.c_str(), "rb");
    if (fontfile == NULL) {
        fprintf(stderr, "Unable to open font file: %s\n", fontname.c_str());
        exit(1);
    }

    readmagic(fontfile, magicnum);
    fileline = (char *) malloc(sizeof (char) * (MAXFIRSTLINELEN + 1));
    if (myfgets(fileline, MAXFIRSTLINELEN + 1, fontfile) == NULL) {
        fileline[0] = '\0';
    }
    if (strlen(fileline) > 0 ? fileline[strlen(fileline) - 1] != '\n' : 0) {
        skiptoeol(fontfile);
    }
    numsread = sscanf(fileline, "%*c%c %d %*d %d %d %d %d %d",
                      &hardblank, &charheight, &maxlen, &smush, &cmtlines,
                      &ffright2left, &smush2);
    free(fileline);
    if (strcmp(magicnum, FONTFILEMAGICNUMBER) || numsread < 5) {
        fprintf(stderr, "Not a FIGlet 2 font file: %s\n", fontname.c_str());
        exit(1);
    }
    for (i = 1; i <= cmtlines; i++) {
        skiptoeol(fontfile);
    }

    if (numsread < 6) {
        ffright2left = 0;
    }

    if (numsread < 7) {         /* if no smush2, decode smush into smush2 */
        if (smush == 0)
            smush2 = SM_KERN;
        else if (smush < 0)
            smush2 = 0;
        else
            smush2 = (smush & 31) | SM_SMUSH;
    }

    if (charheight < 1) {
        charheight = 1;
    }

    if (maxlen < 1) {
        maxlen = 1;
    }

    maxlen += 100;              /* Give ourselves some extra room */

    if (smushoverride == SMO_NO)
        smushmode = smush2;
    else if (smushoverride == SMO_FORCE)
        smushmode |= smush2;

    if (right2left < 0) {
        right2left = ffright2left;
    }

    if (justification < 0) {
        justification = 2 * right2left;
    }

    fileline = (char *) malloc(sizeof (char) * (maxlen + 1));
    /*
     * Allocate "missing" character
     */
    fcharlist = (fcharnode *) malloc(sizeof (fcharnode));
    fcharlist->ord = 0;
    fcharlist->thechar = (char **) malloc(sizeof (char *) * charheight);
    fcharlist->next = NULL;
    for (row = 0; row < charheight; row++) {
        fcharlist->thechar[row] = (char *) malloc(sizeof (char));
        fcharlist->thechar[row][0] = '\0';
    }
    for (theord = ' '; theord <= '~'; theord++) {
        readfontchar(fontfile, theord, fileline, maxlen);
    }
    for (theord = 0; theord <= 6; theord++) {
        readfontchar(fontfile, deutsch[theord], fileline, maxlen);
    }
    while (myfgets(fileline, maxlen + 1, fontfile) == NULL ? 0 :
           sscanf(fileline, "%li", &theord) == 1) {
        readfontchar(fontfile, theord, fileline, maxlen);
    }
    fclose(fontfile);
    free(fileline);
}

figlet::~figlet (void)
{
    int row;
    fcharnode *fclsave;
    fcharnode *list = fcharlist;

    while (list) {

        fclsave = list->next;

        for (row = 0; row < charheight; row++) {
            free(list->thechar[row]);
        }

        free(list->thechar);
        free(list);

        list = fclsave;
    }
}

/****************************************************************************

  linealloc

  Allocates & clears outputline, inchrline. Sets inchrlinelenlimit.
  Called near beginning of main().

****************************************************************************/

void figlet::linealloc (void)
{
    int row;

    outputline = (char **) malloc(sizeof (char *) * charheight);
    for (row = 0; row < charheight; row++) {
        outputline[row] =
            (char *) malloc(sizeof (char) * (outlinelenlimit + 1));
    }
    inchrlinelenlimit = outputwidth * 4 + 100;
    inchrline = (inchr *) malloc(sizeof (inchr) * (inchrlinelenlimit + 1));
    clearline();
}

void figlet::linefree (void)
{
    int row;

    for (row = 0; row < charheight; row++) {
        free(outputline[row]);
    }
    free(outputline);
    outputline = 0;

    free(inchrline);
    inchrline = 0;
}

/****************************************************************************

  getletter

  Sets currchar to point to the font entry for the given character.
  Sets currcharwidth to the width of this character.

****************************************************************************/

void figlet::getletter (inchr c)
{
    fcharnode *charptr;

    for (charptr = fcharlist; charptr == NULL ? 0 : charptr->ord != c;
         charptr = charptr->next);
    if (charptr != NULL) {
        currchar = charptr->thechar;
    } else {
        for (charptr = fcharlist; charptr == NULL ? 0 : charptr->ord != 0;
             charptr = charptr->next);
        currchar = charptr->thechar;
    }
    previouscharwidth = currcharwidth;
    currcharwidth = strlen(currchar[0]);
}

/****************************************************************************

  smushem

  Given 2 characters, attempts to smush them into 1, according to
  smushmode.  Returns smushed character or '\0' if no smushing can be
  done.

  smushmode values are sum of following (all values smush blanks):
    1: Smush equal chars (not hardblanks)
    2: Smush '_' with any char in hierarchy below
    4: hierarchy: "|", "/\", "[]", "{}", "()", "<>"
       Each class in hier. can be replaced by later class.
    8: [ + ] -> |, { + } -> |, ( + ) -> |
   16: / + \ -> X, > + < -> X (only in that order)
   32: hardblank + hardblank -> hardblank

****************************************************************************/

char figlet::smushem (char lch, char rch)
{
    if (lch == ' ')
        return rch;
    if (rch == ' ')
        return lch;

    if (previouscharwidth < 2 || currcharwidth < 2)
        return '\0';
    /*
     * Disallows overlapping if the previous character
     */
    /*
     * or the current character has a width of 1 or zero.
     */

    if ((smushmode & SM_SMUSH) == 0)
        return '\0';            /* kerning */

    if ((smushmode & 63) == 0) {
        /*
         * This is smushing by universal overlapping.
         */
        if (lch == ' ')
            return rch;
        if (rch == ' ')
            return lch;
        if (lch == hardblank)
            return rch;
        if (rch == hardblank)
            return lch;
        /*
         * Above four lines ensure overlapping preference to
         */
        /*
         * visible characters.
         */
        if (right2left == 1)
            return lch;
        /*
         * Above line ensures that the dominant (foreground)
         */
        /*
         * fig-character for overlapping is the latter in the
         */
        /*
         * user's text, not necessarily the rightmost character.
         */
        return rch;
        /*
         * Occurs in the absence of above exceptions.
         */
    }

    if (smushmode & SM_HARDBLANK) {
        if (lch == hardblank && rch == hardblank)
            return lch;
    }

    if (lch == hardblank || rch == hardblank)
        return '\0';

    if (smushmode & SM_EQUAL) {
        if (lch == rch)
            return lch;
    }

    if (smushmode & SM_LOWLINE) {
        if (lch == '_' && strchr("|/\\[]{}()<>", rch))
            return rch;
        if (rch == '_' && strchr("|/\\[]{}()<>", lch))
            return lch;
    }

    if (smushmode & SM_HIERARCHY) {
        if (lch == '|' && strchr("/\\[]{}()<>", rch))
            return rch;
        if (rch == '|' && strchr("/\\[]{}()<>", lch))
            return lch;
        if (strchr("/\\", lch) && strchr("[]{}()<>", rch))
            return rch;
        if (strchr("/\\", rch) && strchr("[]{}()<>", lch))
            return lch;
        if (strchr("[]", lch) && strchr("{}()<>", rch))
            return rch;
        if (strchr("[]", rch) && strchr("{}()<>", lch))
            return lch;
        if (strchr("{}", lch) && strchr("()<>", rch))
            return rch;
        if (strchr("{}", rch) && strchr("()<>", lch))
            return lch;
        if (strchr("()", lch) && strchr("<>", rch))
            return rch;
        if (strchr("()", rch) && strchr("<>", lch))
            return lch;
    }

    if (smushmode & SM_PAIR) {
        if (lch == '[' && rch == ']')
            return '|';
        if (rch == '[' && lch == ']')
            return '|';
        if (lch == '{' && rch == '}')
            return '|';
        if (rch == '{' && lch == '}')
            return '|';
        if (lch == '(' && rch == ')')
            return '|';
        if (rch == '(' && lch == ')')
            return '|';
    }

    if (smushmode & SM_BIGX) {
        if (lch == '/' && rch == '\\')
            return '|';
        if (rch == '/' && lch == '\\')
            return 'Y';
        if (lch == '>' && rch == '<')
            return 'X';
        /*
         * Don't want the reverse of above to give 'X'.
         */
    }

    return '\0';
}

/****************************************************************************

  smushamt

  Returns the maximum amount that the current character can be smushed
  into the current line.

****************************************************************************/

int figlet::smushamt (void)
{
    int maxsmush, amt;
    int row, linebd, charbd;
    char ch1, ch2;

    if ((smushmode & (SM_SMUSH | SM_KERN)) == 0) {
        return 0;
    }
    maxsmush = currcharwidth;
    for (row = 0; row < charheight; row++) {
        if (right2left) {
            for (charbd = strlen(currchar[row]);
                 ch1 = currchar[row][charbd], (charbd > 0
                                               && (!ch1 || ch1 == ' '));
                 charbd--);
            for (linebd = 0; ch2 = outputline[row][linebd], ch2 == ' ';
                 linebd++);
            amt = linebd + currcharwidth - 1 - charbd;
        } else {
            for (linebd = strlen(outputline[row]);
                 ch1 = outputline[row][linebd], (linebd > 0
                                                 && (!ch1 || ch1 == ' '));
                 linebd--);
            for (charbd = 0; ch2 = currchar[row][charbd], ch2 == ' ';
                 charbd++);
            amt = charbd + outlinelen - 1 - linebd;
        }
        if (!ch1 || ch1 == ' ') {
            amt++;
        } else if (ch2) {
            if (smushem(ch1, ch2) != '\0') {
                amt++;
            }
        }
        if (amt < maxsmush) {
            maxsmush = amt;
        }
    }
    return maxsmush;
}

/****************************************************************************

  addchar

  Attempts to add the given character onto the end of the current line.
  Returns 1 if this can be done, 0 otherwise.

****************************************************************************/

int figlet::addchar (inchr c)
{
    int smushamount, row, k;
    char *templine;

    getletter(c);
    smushamount = smushamt();
    if (outlinelen + currcharwidth - smushamount > outlinelenlimit
        || inchrlinelen + 1 > inchrlinelenlimit) {
        return 0;
    }

    templine = (char *) malloc(sizeof (char) * (outlinelenlimit + 1));
    for (row = 0; row < charheight; row++) {
        if (right2left) {
            strcpy(templine, currchar[row]);
            for (k = 0; k < smushamount; k++) {
                templine[currcharwidth - smushamount + k] =
                    smushem(templine[currcharwidth - smushamount + k],
                            outputline[row][k]);
            }
            strcat(templine, outputline[row] + smushamount);
            strcpy(outputline[row], templine);
        } else {
            for (k = 1; k < smushamount; k++) {
                outputline[row][outlinelen - smushamount + k] =
                    smushem(outputline[row][outlinelen - smushamount + k],
                            currchar[row][k]);
            }
            strcat(outputline[row], currchar[row] + smushamount);
        }
    }
    free(templine);
    outlinelen = strlen(outputline[0]);
    inchrline[inchrlinelen++] = c;
    return 1;
}

/****************************************************************************

  putstring

  Prints out the given null-terminated string, substituting blanks
  for hardblanks.  If outputwidth is 1, prints the entire string;
  otherwise prints at most outputwidth-1 characters.  Prints a newline
  at the end of the string.  The string is left-justified, centered or
  right-justified (taking outputwidth as the screen width) if
  justification is 0, 1 or 2, respectively.

****************************************************************************/

void figlet::putstring (char *string)
{
    int i, len;

    len = strlen(string);
    if (outputwidth > 1) {
        if (len > outputwidth - 1) {
            len = outputwidth - 1;
        }
        if (justification > 0) {
            for (i = 1;
                 (3 - justification) * i + len + justification - 2 <
                 outputwidth; i++) {
                putchar(' ');
            }
        }
    }
    for (i = 0; i < len; i++) {
        putchar(string[i] == hardblank ? ' ' : string[i]);
    }
    putchar('\n');
}

void figlet::term_putstring (int x, int y, const char *string)
{
    int i, len;

    gh_term_goto(x, y);

    len = strlen(string);
    if (outputwidth > 1) {
        if (len > outputwidth - 1) {
            len = outputwidth - 1;
        }
        if (justification > 0) {
            for (i = 1;
                 (3 - justification) * i + len + justification - 2 <
                 outputwidth; i++) {
		gh_term_cursor_right();
            }
        }
    }
    for (i = 0; i < len; i++) {
	if (string[i] == hardblank) {
	    gh_term_cursor_right();
	} else {
	    if (string[i] == ' ') {
		gh_term_putc(' ');
	    } else {
		gh_term_putc(string[i]);
	    }
	}
    }
}

string figlet::sputstring (char *string)
{
    std::string s;
    int i, len;

    len = strlen(string);
    if (outputwidth > 1) {
        if (len > outputwidth - 1) {
            len = outputwidth - 1;
        }
        if (justification > 0) {
            for (i = 1;
                 (3 - justification) * i + len + justification - 2 <
                 outputwidth; i++) {
                s += ' ';
            }
        }
    }
    for (i = 0; i < len; i++) {
        s += (string[i] == hardblank ? ' ' : string[i]);
    }
    s += "\n";
    return (s);
}

/****************************************************************************

  printline

  Prints outputline using putstring, then clears the current line.

****************************************************************************/

void figlet::printline (void)
{
    int i;

    for (i = 0; i < charheight; i++) {
        putstring(outputline[i]);
    }
    clearline();
}

void figlet::term_printline (int x, int y)
{
    int i;

    for (i = 0; i < charheight; i++) {
        term_putstring(x, y + i, outputline[i]);
    }
    clearline();
}

string figlet::sprintline (void)
{
    string s;
    int i;

    for (i = 0; i < charheight; i++) {
        s += sputstring(outputline[i]);
    }
    clearline();

    return (s);
}

void figlet::print (const char *s)
{
    char c;

    linealloc();

    while ((c = *s++)) {
        addchar(c);
    }

    printline();
    linefree();
}

void figlet::printat (int x, int y, const char *s)
{
    char c;

    linealloc();

    while ((c = *s++)) {
        addchar(c);
    }

    term_printline(x, y);
    linefree();
}

void figlet::printat (int y, const char *s)
{
    gh_term_goto(0, y);

    gh_term_cursor_move_only = true;
    printat(0, y, s);
    gh_term_cursor_move_only = false;

    printat(GH_TERM_WIDTH/2 - gh_term_x/2, y, s);
}

string figlet::sprint (const char *s)
{
    std::string st;
    char c;

    linealloc();

    while ((c = *s++)) {
        addchar(c);
    }

    st = sprintline();

    linefree();

    return (st);
}
