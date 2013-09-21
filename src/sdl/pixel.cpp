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

    $Id: pixel.cpp 1236 2009-07-06 21:45:34Z Ghack $
*/

#include <ghack.h>
#include <gh_surface.h>
#include <gh_getput_pixel.h>
#include <gh_getput_pixel_macros.h>

gh_pixel::hash_pixel gh_pixel::pixels;
gh_pixel::hash_pixel2 gh_pixel::pixels2;

const gh_pixel GH_ALICE_BLUE          (240,248,255);
const gh_pixel GH_ALICEBLUE           (240,248,255);
const gh_pixel GH_ANTIQUEWHITE1       (255,239,219);
const gh_pixel GH_ANTIQUEWHITE2       (238,223,204);
const gh_pixel GH_ANTIQUEWHITE3       (205,192,176);
const gh_pixel GH_ANTIQUEWHITE4       (139,131,120);
const gh_pixel GH_ANTIQUE_WHITE       (250,235,215);
const gh_pixel GH_ANTIQUEWHITE        (250,235,215);
const gh_pixel GH_AQUAMARINE1         (127,255,212);
const gh_pixel GH_AQUAMARINE2         (118,238,198);
const gh_pixel GH_AQUAMARINE3         (102,205,170);
const gh_pixel GH_AQUAMARINE4         (69,139,116);
const gh_pixel GH_AQUAMARINE          (127,255,212);
const gh_pixel GH_AZURE1              (240,255,255);
const gh_pixel GH_AZURE2              (224,238,238);
const gh_pixel GH_AZURE3              (193,205,205);
const gh_pixel GH_AZURE4              (131,139,139);
const gh_pixel GH_AZURE               (240,255,255);
const gh_pixel GH_BEIGE               (245,245,220);
const gh_pixel GH_BISQUE1             (255,228,196);
const gh_pixel GH_BISQUE2             (238,213,183);
const gh_pixel GH_BISQUE3             (205,183,158);
const gh_pixel GH_BISQUE4             (139,125,107);
const gh_pixel GH_BISQUE              (255,228,196);
const gh_pixel GH_BLACK               (0,0,0);
const gh_pixel GH_BLANCHED_ALMOND     (255,235,205);
const gh_pixel GH_BLANCHEDALMOND      (255,235,205);
const gh_pixel GH_BLUE1               (0,0,255);
const gh_pixel GH_BLUE2               (0,0,238);
const gh_pixel GH_BLUE3               (0,0,205);
const gh_pixel GH_BLUE4               (0,0,139);
const gh_pixel GH_BLUE                (0,0,255);
const gh_pixel GH_BLUE_VIOLET         (138,43,226);
const gh_pixel GH_BLUEVIOLET          (138,43,226);
const gh_pixel GH_BROWN1              (255,64,64);
const gh_pixel GH_BROWN2              (238,59,59);
const gh_pixel GH_BROWN3              (205,51,51);
const gh_pixel GH_BROWN4              (139,35,35);
const gh_pixel GH_BROWN               (165,42,42);
const gh_pixel GH_BURLYWOOD1          (255,211,155);
const gh_pixel GH_BURLYWOOD2          (238,197,145);
const gh_pixel GH_BURLYWOOD3          (205,170,125);
const gh_pixel GH_BURLYWOOD4          (139,115,85);
const gh_pixel GH_BURLYWOOD           (222,184,135);
const gh_pixel GH_CADETBLUE1          (152,245,255);
const gh_pixel GH_CADETBLUE2          (142,229,238);
const gh_pixel GH_CADETBLUE3          (122,197,205);
const gh_pixel GH_CADETBLUE4          (83,134,139);
const gh_pixel GH_CADET_BLUE          (95,158,160);
const gh_pixel GH_CADETBLUE           (95,158,160);
const gh_pixel GH_CHARTREUSE1         (127,255,0);
const gh_pixel GH_CHARTREUSE2         (118,238,0);
const gh_pixel GH_CHARTREUSE3         (102,205,0);
const gh_pixel GH_CHARTREUSE4         (69,139,0);
const gh_pixel GH_CHARTREUSE          (127,255,0);
const gh_pixel GH_CHOCOLATE1          (255,127,36);
const gh_pixel GH_CHOCOLATE2          (238,118,33);
const gh_pixel GH_CHOCOLATE3          (205,102,29);
const gh_pixel GH_CHOCOLATE4          (139,69,19);
const gh_pixel GH_CHOCOLATE           (210,105,30);
const gh_pixel GH_CORAL1              (255,114,86);
const gh_pixel GH_CORAL2              (238,106,80);
const gh_pixel GH_CORAL3              (205,91,69);
const gh_pixel GH_CORAL4              (139,62,47);
const gh_pixel GH_CORAL               (255,127,80);
const gh_pixel GH_CORNFLOWER_BLUE     (100,149,237);
const gh_pixel GH_CORNFLOWERBLUE      (100,149,237);
const gh_pixel GH_CORNSILK1           (255,248,220);
const gh_pixel GH_CORNSILK2           (238,232,205);
const gh_pixel GH_CORNSILK3           (205,200,177);
const gh_pixel GH_CORNSILK4           (139,136,120);
const gh_pixel GH_CORNSILK            (255,248,220);
const gh_pixel GH_CYAN1               (0,255,255);
const gh_pixel GH_CYAN2               (0,238,238);
const gh_pixel GH_CYAN3               (0,205,205);
const gh_pixel GH_CYAN4               (0,139,139);
const gh_pixel GH_CYAN                (0,255,255);
const gh_pixel GH_DARK_BLUE           (0,0,139);
const gh_pixel GH_DARKBLUE            (0,0,139);
const gh_pixel GH_DARK_CYAN           (0,139,139);
const gh_pixel GH_DARKCYAN            (0,139,139);
const gh_pixel GH_DARKGOLDENROD1      (255,185,15);
const gh_pixel GH_DARKGOLDENROD2      (238,173,14);
const gh_pixel GH_DARKGOLDENROD3      (205,149,12);
const gh_pixel GH_DARKGOLDENROD4      (139,101,8);
const gh_pixel GH_DARK_GOLDENROD      (184,134,11);
const gh_pixel GH_DARKGOLDENROD       (184,134,11);
const gh_pixel GH_DARK_GRAY           (169,169,169);
const gh_pixel GH_DARKGRAY            (169,169,169);
const gh_pixel GH_DARK_GREEN          (0,100,0);
const gh_pixel GH_DARKGREEN           (0,100,0);
const gh_pixel GH_DARK_GREY           (169,169,169);
const gh_pixel GH_DARKGREY            (169,169,169);
const gh_pixel GH_DARK_KHAKI          (189,183,107);
const gh_pixel GH_DARKKHAKI           (189,183,107);
const gh_pixel GH_DARK_MAGENTA        (139,0,139);
const gh_pixel GH_DARKMAGENTA         (139,0,139);
const gh_pixel GH_DARKOLIVEGREEN1     (202,255,112);
const gh_pixel GH_DARKOLIVEGREEN2     (188,238,104);
const gh_pixel GH_DARKOLIVEGREEN3     (162,205,90);
const gh_pixel GH_DARKOLIVEGREEN4     (110,139,61);
const gh_pixel GH_DARK_OLIVE_GREEN    (85,107,47);
const gh_pixel GH_DARKOLIVEGREEN      (85,107,47);
const gh_pixel GH_DARKORANGE1         (255,127,0);
const gh_pixel GH_DARKORANGE2         (238,118,0);
const gh_pixel GH_DARKORANGE3         (205,102,0);
const gh_pixel GH_DARKORANGE4         (139,69,0);
const gh_pixel GH_DARK_ORANGE         (255,140,0);
const gh_pixel GH_DARKORANGE          (255,140,0);
const gh_pixel GH_DARKORCHID1         (191,62,255);
const gh_pixel GH_DARKORCHID2         (178,58,238);
const gh_pixel GH_DARKORCHID3         (154,50,205);
const gh_pixel GH_DARKORCHID4         (104,34,139);
const gh_pixel GH_DARK_ORCHID         (153,50,204);
const gh_pixel GH_DARKORCHID          (153,50,204);
const gh_pixel GH_DARK_RED            (139,0,0);
const gh_pixel GH_DARKRED             (139,0,0);
const gh_pixel GH_DARK_SALMON         (233,150,122);
const gh_pixel GH_DARKSALMON          (233,150,122);
const gh_pixel GH_DARKSEAGREEN1       (193,255,193);
const gh_pixel GH_DARKSEAGREEN2       (180,238,180);
const gh_pixel GH_DARKSEAGREEN3       (155,205,155);
const gh_pixel GH_DARKSEAGREEN4       (105,139,105);
const gh_pixel GH_DARK_SEA_GREEN      (143,188,143);
const gh_pixel GH_DARKSEAGREEN        (143,188,143);
const gh_pixel GH_DARK_SLATE_BLUE     (72,61,139);
const gh_pixel GH_DARKSLATEBLUE       (72,61,139);
const gh_pixel GH_DARKSLATEGRAY1      (151,255,255);
const gh_pixel GH_DARKSLATEGRAY2      (141,238,238);
const gh_pixel GH_DARKSLATEGRAY3      (121,205,205);
const gh_pixel GH_DARKSLATEGRAY4      (82,139,139);
const gh_pixel GH_DARK_SLATE_GRAY     (47,79,79);
const gh_pixel GH_DARKSLATEGRAY       (47,79,79);
const gh_pixel GH_DARK_SLATE_GREY     (47,79,79);
const gh_pixel GH_DARKSLATEGREY       (47,79,79);
const gh_pixel GH_DARK_TURQUOISE      (0,206,209);
const gh_pixel GH_DARKTURQUOISE       (0,206,209);
const gh_pixel GH_DARK_VIOLET         (148,0,211);
const gh_pixel GH_DARKVIOLET          (148,0,211);
const gh_pixel GH_DEEPPINK1           (255,20,147);
const gh_pixel GH_DEEPPINK2           (238,18,137);
const gh_pixel GH_DEEPPINK3           (205,16,118);
const gh_pixel GH_DEEPPINK4           (139,10,80);
const gh_pixel GH_DEEP_PINK           (255,20,147);
const gh_pixel GH_DEEPPINK            (255,20,147);
const gh_pixel GH_DEEPSKYBLUE1        (0,191,255);
const gh_pixel GH_DEEPSKYBLUE2        (0,178,238);
const gh_pixel GH_DEEPSKYBLUE3        (0,154,205);
const gh_pixel GH_DEEPSKYBLUE4        (0,104,139);
const gh_pixel GH_DEEP_SKY_BLUE       (0,191,255);
const gh_pixel GH_DEEPSKYBLUE         (0,191,255);
const gh_pixel GH_DIM_GRAY            (105,105,105);
const gh_pixel GH_DIMGRAY             (105,105,105);
const gh_pixel GH_DIM_GREY            (105,105,105);
const gh_pixel GH_DIMGREY             (105,105,105);
const gh_pixel GH_DODGERBLUE1         (30,144,255);
const gh_pixel GH_DODGERBLUE2         (28,134,238);
const gh_pixel GH_DODGERBLUE3         (24,116,205);
const gh_pixel GH_DODGERBLUE4         (16,78,139);
const gh_pixel GH_DODGER_BLUE         (30,144,255);
const gh_pixel GH_DODGERBLUE          (30,144,255);
const gh_pixel GH_FIREBRICK1          (255,48,48);
const gh_pixel GH_FIREBRICK2          (238,44,44);
const gh_pixel GH_FIREBRICK3          (205,38,38);
const gh_pixel GH_FIREBRICK4          (139,26,26);
const gh_pixel GH_FIREBRICK           (178,34,34);
const gh_pixel GH_FLORAL_WHITE        (255,250,240);
const gh_pixel GH_FLORALWHITE         (255,250,240);
const gh_pixel GH_FOREST_GREEN        (34,139,34);
const gh_pixel GH_FORESTGREEN         (34,139,34);
const gh_pixel GH_GAINSBORO           (220,220,220);
const gh_pixel GH_GHOST_WHITE         (248,248,255);
const gh_pixel GH_GHOSTWHITE          (248,248,255);
const gh_pixel GH_GOLD1               (255,215,0);
const gh_pixel GH_GOLD2               (238,201,0);
const gh_pixel GH_GOLD3               (205,173,0);
const gh_pixel GH_GOLD4               (139,117,0);
const gh_pixel GH_GOLDENROD1          (255,193,37);
const gh_pixel GH_GOLDENROD2          (238,180,34);
const gh_pixel GH_GOLDENROD3          (205,155,29);
const gh_pixel GH_GOLDENROD4          (139,105,20);
const gh_pixel GH_GOLDENROD           (218,165,32);
const gh_pixel GH_GOLD                (255,215,0);
const gh_pixel GH_GRAY10              (26,26,26);
const gh_pixel GH_GRAY11              (28,28,28);
const gh_pixel GH_GRAY12              (31,31,31);
const gh_pixel GH_GRAY13              (33,33,33);
const gh_pixel GH_GRAY14              (36,36,36);
const gh_pixel GH_GRAY15              (38,38,38);
const gh_pixel GH_GRAY16              (41,41,41);
const gh_pixel GH_GRAY17              (43,43,43);
const gh_pixel GH_GRAY18              (46,46,46);
const gh_pixel GH_GRAY19              (48,48,48);
const gh_pixel GH_GRAY1               (3,3,3);
const gh_pixel GH_GRAY20              (51,51,51);
const gh_pixel GH_GRAY21              (54,54,54);
const gh_pixel GH_GRAY22              (56,56,56);
const gh_pixel GH_GRAY23              (59,59,59);
const gh_pixel GH_GRAY24              (61,61,61);
const gh_pixel GH_GRAY25              (64,64,64);
const gh_pixel GH_GRAY26              (66,66,66);
const gh_pixel GH_GRAY27              (69,69,69);
const gh_pixel GH_GRAY28              (71,71,71);
const gh_pixel GH_GRAY29              (74,74,74);
const gh_pixel GH_GRAY2               (5,5,5);
const gh_pixel GH_GRAY30              (77,77,77);
const gh_pixel GH_GRAY31              (79,79,79);
const gh_pixel GH_GRAY32              (82,82,82);
const gh_pixel GH_GRAY33              (84,84,84);
const gh_pixel GH_GRAY34              (87,87,87);
const gh_pixel GH_GRAY35              (89,89,89);
const gh_pixel GH_GRAY36              (92,92,92);
const gh_pixel GH_GRAY37              (94,94,94);
const gh_pixel GH_GRAY38              (97,97,97);
const gh_pixel GH_GRAY39              (99,99,99);
const gh_pixel GH_GRAY3               (8,8,8);
const gh_pixel GH_GRAY40              (102,102,102);
const gh_pixel GH_GRAY41              (105,105,105);
const gh_pixel GH_GRAY42              (107,107,107);
const gh_pixel GH_GRAY43              (110,110,110);
const gh_pixel GH_GRAY44              (112,112,112);
const gh_pixel GH_GRAY45              (115,115,115);
const gh_pixel GH_GRAY46              (117,117,117);
const gh_pixel GH_GRAY47              (120,120,120);
const gh_pixel GH_GRAY48              (122,122,122);
const gh_pixel GH_GRAY49              (125,125,125);
const gh_pixel GH_GRAY4               (10,10,10);
const gh_pixel GH_GRAY50              (127,127,127);
const gh_pixel GH_GRAY51              (130,130,130);
const gh_pixel GH_GRAY52              (133,133,133);
const gh_pixel GH_GRAY53              (135,135,135);
const gh_pixel GH_GRAY54              (138,138,138);
const gh_pixel GH_GRAY55              (140,140,140);
const gh_pixel GH_GRAY56              (143,143,143);
const gh_pixel GH_GRAY57              (145,145,145);
const gh_pixel GH_GRAY58              (148,148,148);
const gh_pixel GH_GRAY59              (150,150,150);
const gh_pixel GH_GRAY5               (13,13,13);
const gh_pixel GH_GRAY60              (153,153,153);
const gh_pixel GH_GRAY61              (156,156,156);
const gh_pixel GH_GRAY62              (158,158,158);
const gh_pixel GH_GRAY63              (161,161,161);
const gh_pixel GH_GRAY64              (163,163,163);
const gh_pixel GH_GRAY65              (166,166,166);
const gh_pixel GH_GRAY66              (168,168,168);
const gh_pixel GH_GRAY67              (171,171,171);
const gh_pixel GH_GRAY68              (173,173,173);
const gh_pixel GH_GRAY69              (176,176,176);
const gh_pixel GH_GRAY6               (15,15,15);
const gh_pixel GH_GRAY70              (179,179,179);
const gh_pixel GH_GRAY71              (181,181,181);
const gh_pixel GH_GRAY72              (184,184,184);
const gh_pixel GH_GRAY73              (186,186,186);
const gh_pixel GH_GRAY74              (189,189,189);
const gh_pixel GH_GRAY75              (191,191,191);
const gh_pixel GH_GRAY76              (194,194,194);
const gh_pixel GH_GRAY77              (196,196,196);
const gh_pixel GH_GRAY78              (199,199,199);
const gh_pixel GH_GRAY79              (201,201,201);
const gh_pixel GH_GRAY7               (18,18,18);
const gh_pixel GH_GRAY80              (204,204,204);
const gh_pixel GH_GRAY81              (207,207,207);
const gh_pixel GH_GRAY82              (209,209,209);
const gh_pixel GH_GRAY83              (212,212,212);
const gh_pixel GH_GRAY84              (214,214,214);
const gh_pixel GH_GRAY85              (217,217,217);
const gh_pixel GH_GRAY86              (219,219,219);
const gh_pixel GH_GRAY87              (222,222,222);
const gh_pixel GH_GRAY88              (224,224,224);
const gh_pixel GH_GRAY89              (227,227,227);
const gh_pixel GH_GRAY8               (20,20,20);
const gh_pixel GH_GRAY90              (229,229,229);
const gh_pixel GH_GRAY91              (232,232,232);
const gh_pixel GH_GRAY92              (235,235,235);
const gh_pixel GH_GRAY93              (237,237,237);
const gh_pixel GH_GRAY94              (240,240,240);
const gh_pixel GH_GRAY95              (242,242,242);
const gh_pixel GH_GRAY96              (245,245,245);
const gh_pixel GH_GRAY97              (247,247,247);
const gh_pixel GH_GRAY98              (250,250,250);
const gh_pixel GH_GRAY99              (252,252,252);
const gh_pixel GH_GRAY9               (23,23,23);
const gh_pixel GH_GRAY                (190,190,190);
const gh_pixel GH_GREEN1              (0,255,0);
const gh_pixel GH_GREEN2              (0,238,0);
const gh_pixel GH_GREEN3              (0,205,0);
const gh_pixel GH_GREEN4              (0,139,0);
const gh_pixel GH_GREEN               (0,255,0);
const gh_pixel GH_GREEN_YELLOW        (173,255,47);
const gh_pixel GH_GREENYELLOW         (173,255,47);
const gh_pixel GH_GREY10              (26,26,26);
const gh_pixel GH_GREY11              (28,28,28);
const gh_pixel GH_GREY12              (31,31,31);
const gh_pixel GH_GREY13              (33,33,33);
const gh_pixel GH_GREY14              (36,36,36);
const gh_pixel GH_GREY15              (38,38,38);
const gh_pixel GH_GREY16              (41,41,41);
const gh_pixel GH_GREY17              (43,43,43);
const gh_pixel GH_GREY18              (46,46,46);
const gh_pixel GH_GREY19              (48,48,48);
const gh_pixel GH_GREY1               (3,3,3);
const gh_pixel GH_GREY20              (51,51,51);
const gh_pixel GH_GREY21              (54,54,54);
const gh_pixel GH_GREY22              (56,56,56);
const gh_pixel GH_GREY23              (59,59,59);
const gh_pixel GH_GREY24              (61,61,61);
const gh_pixel GH_GREY25              (64,64,64);
const gh_pixel GH_GREY26              (66,66,66);
const gh_pixel GH_GREY27              (69,69,69);
const gh_pixel GH_GREY28              (71,71,71);
const gh_pixel GH_GREY29              (74,74,74);
const gh_pixel GH_GREY2               (5,5,5);
const gh_pixel GH_GREY30              (77,77,77);
const gh_pixel GH_GREY31              (79,79,79);
const gh_pixel GH_GREY32              (82,82,82);
const gh_pixel GH_GREY33              (84,84,84);
const gh_pixel GH_GREY34              (87,87,87);
const gh_pixel GH_GREY35              (89,89,89);
const gh_pixel GH_GREY36              (92,92,92);
const gh_pixel GH_GREY37              (94,94,94);
const gh_pixel GH_GREY38              (97,97,97);
const gh_pixel GH_GREY39              (99,99,99);
const gh_pixel GH_GREY3               (8,8,8);
const gh_pixel GH_GREY40              (102,102,102);
const gh_pixel GH_GREY41              (105,105,105);
const gh_pixel GH_GREY42              (107,107,107);
const gh_pixel GH_GREY43              (110,110,110);
const gh_pixel GH_GREY44              (112,112,112);
const gh_pixel GH_GREY45              (115,115,115);
const gh_pixel GH_GREY46              (117,117,117);
const gh_pixel GH_GREY47              (120,120,120);
const gh_pixel GH_GREY48              (122,122,122);
const gh_pixel GH_GREY49              (125,125,125);
const gh_pixel GH_GREY4               (10,10,10);
const gh_pixel GH_GREY50              (127,127,127);
const gh_pixel GH_GREY51              (130,130,130);
const gh_pixel GH_GREY52              (133,133,133);
const gh_pixel GH_GREY53              (135,135,135);
const gh_pixel GH_GREY54              (138,138,138);
const gh_pixel GH_GREY55              (140,140,140);
const gh_pixel GH_GREY56              (143,143,143);
const gh_pixel GH_GREY57              (145,145,145);
const gh_pixel GH_GREY58              (148,148,148);
const gh_pixel GH_GREY59              (150,150,150);
const gh_pixel GH_GREY5               (13,13,13);
const gh_pixel GH_GREY60              (153,153,153);
const gh_pixel GH_GREY61              (156,156,156);
const gh_pixel GH_GREY62              (158,158,158);
const gh_pixel GH_GREY63              (161,161,161);
const gh_pixel GH_GREY64              (163,163,163);
const gh_pixel GH_GREY65              (166,166,166);
const gh_pixel GH_GREY66              (168,168,168);
const gh_pixel GH_GREY67              (171,171,171);
const gh_pixel GH_GREY68              (173,173,173);
const gh_pixel GH_GREY69              (176,176,176);
const gh_pixel GH_GREY6               (15,15,15);
const gh_pixel GH_GREY70              (179,179,179);
const gh_pixel GH_GREY71              (181,181,181);
const gh_pixel GH_GREY72              (184,184,184);
const gh_pixel GH_GREY73              (186,186,186);
const gh_pixel GH_GREY74              (189,189,189);
const gh_pixel GH_GREY75              (191,191,191);
const gh_pixel GH_GREY76              (194,194,194);
const gh_pixel GH_GREY77              (196,196,196);
const gh_pixel GH_GREY78              (199,199,199);
const gh_pixel GH_GREY79              (201,201,201);
const gh_pixel GH_GREY7               (18,18,18);
const gh_pixel GH_GREY80              (204,204,204);
const gh_pixel GH_GREY81              (207,207,207);
const gh_pixel GH_GREY82              (209,209,209);
const gh_pixel GH_GREY83              (212,212,212);
const gh_pixel GH_GREY84              (214,214,214);
const gh_pixel GH_GREY85              (217,217,217);
const gh_pixel GH_GREY86              (219,219,219);
const gh_pixel GH_GREY87              (222,222,222);
const gh_pixel GH_GREY88              (224,224,224);
const gh_pixel GH_GREY89              (227,227,227);
const gh_pixel GH_GREY8               (20,20,20);
const gh_pixel GH_GREY90              (229,229,229);
const gh_pixel GH_GREY91              (232,232,232);
const gh_pixel GH_GREY92              (235,235,235);
const gh_pixel GH_GREY93              (237,237,237);
const gh_pixel GH_GREY94              (240,240,240);
const gh_pixel GH_GREY95              (242,242,242);
const gh_pixel GH_GREY96              (245,245,245);
const gh_pixel GH_GREY97              (247,247,247);
const gh_pixel GH_GREY98              (250,250,250);
const gh_pixel GH_GREY99              (252,252,252);
const gh_pixel GH_GREY9               (23,23,23);
const gh_pixel GH_GREY                (190,190,190);
const gh_pixel GH_HONEYDEW1           (240,255,240);
const gh_pixel GH_HONEYDEW2           (224,238,224);
const gh_pixel GH_HONEYDEW3           (193,205,193);
const gh_pixel GH_HONEYDEW4           (131,139,131);
const gh_pixel GH_HONEYDEW            (240,255,240);
const gh_pixel GH_HOTPINK1            (255,110,180);
const gh_pixel GH_HOTPINK2            (238,106,167);
const gh_pixel GH_HOTPINK3            (205,96,144);
const gh_pixel GH_HOTPINK4            (139,58,98);
const gh_pixel GH_HOT_PINK            (255,105,180);
const gh_pixel GH_HOTPINK             (255,105,180);
const gh_pixel GH_INDIANRED1          (255,106,106);
const gh_pixel GH_INDIANRED2          (238,99,99);
const gh_pixel GH_INDIANRED3          (205,85,85);
const gh_pixel GH_INDIANRED4          (139,58,58);
const gh_pixel GH_INDIAN_RED          (205,92,92);
const gh_pixel GH_INDIANRED           (205,92,92);
const gh_pixel GH_IVORY1              (255,255,240);
const gh_pixel GH_IVORY2              (238,238,224);
const gh_pixel GH_IVORY3              (205,205,193);
const gh_pixel GH_IVORY4              (139,139,131);
const gh_pixel GH_IVORY               (255,255,240);
const gh_pixel GH_KHAKI1              (255,246,143);
const gh_pixel GH_KHAKI2              (238,230,133);
const gh_pixel GH_KHAKI3              (205,198,115);
const gh_pixel GH_KHAKI4              (139,134,78);
const gh_pixel GH_KHAKI               (240,230,140);
const gh_pixel GH_LAVENDERBLUSH1      (255,240,245);
const gh_pixel GH_LAVENDERBLUSH2      (238,224,229);
const gh_pixel GH_LAVENDERBLUSH3      (205,193,197);
const gh_pixel GH_LAVENDERBLUSH4      (139,131,134);
const gh_pixel GH_LAVENDER_BLUSH      (255,240,245);
const gh_pixel GH_LAVENDERBLUSH       (255,240,245);
const gh_pixel GH_LAVENDER            (230,230,250);
const gh_pixel GH_LAWN_GREEN          (124,252,0);
const gh_pixel GH_LAWNGREEN           (124,252,0);
const gh_pixel GH_LEMONCHIFFON1       (255,250,205);
const gh_pixel GH_LEMONCHIFFON2       (238,233,191);
const gh_pixel GH_LEMONCHIFFON3       (205,201,165);
const gh_pixel GH_LEMONCHIFFON4       (139,137,112);
const gh_pixel GH_LEMON_CHIFFON       (255,250,205);
const gh_pixel GH_LEMONCHIFFON        (255,250,205);
const gh_pixel GH_LIGHTBLUE1          (191,239,255);
const gh_pixel GH_LIGHTBLUE2          (178,223,238);
const gh_pixel GH_LIGHTBLUE3          (154,192,205);
const gh_pixel GH_LIGHTBLUE4          (104,131,139);
const gh_pixel GH_LIGHT_BLUE          (173,216,230);
const gh_pixel GH_LIGHTBLUE           (173,216,230);
const gh_pixel GH_LIGHT_CORAL         (240,128,128);
const gh_pixel GH_LIGHTCORAL          (240,128,128);
const gh_pixel GH_LIGHTCYAN1          (224,255,255);
const gh_pixel GH_LIGHTCYAN2          (209,238,238);
const gh_pixel GH_LIGHTCYAN3          (180,205,205);
const gh_pixel GH_LIGHTCYAN4          (122,139,139);
const gh_pixel GH_LIGHT_CYAN          (224,255,255);
const gh_pixel GH_LIGHTCYAN           (224,255,255);
const gh_pixel GH_LIGHTGOLDENROD1     (255,236,139);
const gh_pixel GH_LIGHTGOLDENROD2     (238,220,130);
const gh_pixel GH_LIGHTGOLDENROD3     (205,190,112);
const gh_pixel GH_LIGHTGOLDENROD4     (139,129,76);
const gh_pixel GH_LIGHT_GOLDENROD     (238,221,130);
const gh_pixel GH_LIGHTGOLDENROD      (238,221,130);
const gh_pixel GH_LIGHT_GOLDENROD_YELLOW (250,250,210);
const gh_pixel GH_LIGHTGOLDENRODYELLOW (250,250,210);
const gh_pixel GH_LIGHT_GRAY          (211,211,211);
const gh_pixel GH_LIGHTGRAY           (211,211,211);
const gh_pixel GH_LIGHT_GREEN         (144,238,144);
const gh_pixel GH_LIGHTGREEN          (144,238,144);
const gh_pixel GH_LIGHT_GREY          (211,211,211);
const gh_pixel GH_LIGHTGREY           (211,211,211);
const gh_pixel GH_LIGHTPINK1          (255,174,185);
const gh_pixel GH_LIGHTPINK2          (238,162,173);
const gh_pixel GH_LIGHTPINK3          (205,140,149);
const gh_pixel GH_LIGHTPINK4          (139,95,101);
const gh_pixel GH_LIGHT_PINK          (255,182,193);
const gh_pixel GH_LIGHTPINK           (255,182,193);
const gh_pixel GH_LIGHTSALMON1        (255,160,122);
const gh_pixel GH_LIGHTSALMON2        (238,149,114);
const gh_pixel GH_LIGHTSALMON3        (205,129,98);
const gh_pixel GH_LIGHTSALMON4        (139,87,66);
const gh_pixel GH_LIGHT_SALMON        (255,160,122);
const gh_pixel GH_LIGHTSALMON         (255,160,122);
const gh_pixel GH_LIGHT_SEA_GREEN     (32,178,170);
const gh_pixel GH_LIGHTSEAGREEN       (32,178,170);
const gh_pixel GH_LIGHTSKYBLUE1       (176,226,255);
const gh_pixel GH_LIGHTSKYBLUE2       (164,211,238);
const gh_pixel GH_LIGHTSKYBLUE3       (141,182,205);
const gh_pixel GH_LIGHTSKYBLUE4       (96,123,139);
const gh_pixel GH_LIGHT_SKY_BLUE      (135,206,250);
const gh_pixel GH_LIGHTSKYBLUE        (135,206,250);
const gh_pixel GH_LIGHT_SLATE_BLUE    (132,112,255);
const gh_pixel GH_LIGHTSLATEBLUE      (132,112,255);
const gh_pixel GH_LIGHT_SLATE_GRAY    (119,136,153);
const gh_pixel GH_LIGHTSLATEGRAY      (119,136,153);
const gh_pixel GH_LIGHT_SLATE_GREY    (119,136,153);
const gh_pixel GH_LIGHTSLATEGREY      (119,136,153);
const gh_pixel GH_LIGHTSTEELBLUE1     (202,225,255);
const gh_pixel GH_LIGHTSTEELBLUE2     (188,210,238);
const gh_pixel GH_LIGHTSTEELBLUE3     (162,181,205);
const gh_pixel GH_LIGHTSTEELBLUE4     (110,123,139);
const gh_pixel GH_LIGHT_STEEL_BLUE    (176,196,222);
const gh_pixel GH_LIGHTSTEELBLUE      (176,196,222);
const gh_pixel GH_LIGHTYELLOW1        (255,255,224);
const gh_pixel GH_LIGHTYELLOW2        (238,238,209);
const gh_pixel GH_LIGHTYELLOW3        (205,205,180);
const gh_pixel GH_LIGHTYELLOW4        (139,139,122);
const gh_pixel GH_LIGHT_YELLOW        (255,255,224);
const gh_pixel GH_LIGHTYELLOW         (255,255,224);
const gh_pixel GH_LIME_GREEN          (50,205,50);
const gh_pixel GH_LIMEGREEN           (50,205,50);
const gh_pixel GH_LIME                (50,205,50);
const gh_pixel GH_LINEN               (250,240,230);
const gh_pixel GH_MAGENTA1            (255,0,255);
const gh_pixel GH_MAGENTA2            (238,0,238);
const gh_pixel GH_MAGENTA3            (205,0,205);
const gh_pixel GH_MAGENTA4            (139,0,139);
const gh_pixel GH_MAGENTA             (255,0,255);
const gh_pixel GH_MAROON1             (255,52,179);
const gh_pixel GH_MAROON2             (238,48,167);
const gh_pixel GH_MAROON3             (205,41,144);
const gh_pixel GH_MAROON4             (139,28,98);
const gh_pixel GH_MAROON              (176,48,96);
const gh_pixel GH_MEDIUM_AQUAMARINE   (102,205,170);
const gh_pixel GH_MEDIUMAQUAMARINE    (102,205,170);
const gh_pixel GH_MEDIUM_BLUE         (0,0,205);
const gh_pixel GH_MEDIUMBLUE          (0,0,205);
const gh_pixel GH_MEDIUMORCHID1       (224,102,255);
const gh_pixel GH_MEDIUMORCHID2       (209,95,238);
const gh_pixel GH_MEDIUMORCHID3       (180,82,205);
const gh_pixel GH_MEDIUMORCHID4       (122,55,139);
const gh_pixel GH_MEDIUM_ORCHID       (186,85,211);
const gh_pixel GH_MEDIUMORCHID        (186,85,211);
const gh_pixel GH_MEDIUMPURPLE1       (171,130,255);
const gh_pixel GH_MEDIUMPURPLE2       (159,121,238);
const gh_pixel GH_MEDIUMPURPLE3       (137,104,205);
const gh_pixel GH_MEDIUMPURPLE4       (93,71,139);
const gh_pixel GH_MEDIUM_PURPLE       (147,112,219);
const gh_pixel GH_MEDIUMPURPLE        (147,112,219);
const gh_pixel GH_MEDIUM_SEA_GREEN    (60,179,113);
const gh_pixel GH_MEDIUMSEAGREEN      (60,179,113);
const gh_pixel GH_MEDIUM_SLATE_BLUE   (123,104,238);
const gh_pixel GH_MEDIUMSLATEBLUE     (123,104,238);
const gh_pixel GH_MEDIUM_SPRING_GREEN (0,250,154);
const gh_pixel GH_MEDIUMSPRINGGREEN   (0,250,154);
const gh_pixel GH_MEDIUM_TURQUOISE    (72,209,204);
const gh_pixel GH_MEDIUMTURQUOISE     (72,209,204);
const gh_pixel GH_MEDIUM_VIOLET_RED   (199,21,133);
const gh_pixel GH_MEDIUMVIOLETRED     (199,21,133);
const gh_pixel GH_MIDNIGHT_BLUE       (25,25,112);
const gh_pixel GH_MIDNIGHTBLUE        (25,25,112);
const gh_pixel GH_MINT_CREAM          (245,255,250);
const gh_pixel GH_MINTCREAM           (245,255,250);
const gh_pixel GH_MISTYROSE1          (255,228,225);
const gh_pixel GH_MISTYROSE2          (238,213,210);
const gh_pixel GH_MISTYROSE3          (205,183,181);
const gh_pixel GH_MISTYROSE4          (139,125,123);
const gh_pixel GH_MISTY_ROSE          (255,228,225);
const gh_pixel GH_MISTYROSE           (255,228,225);
const gh_pixel GH_MOCCASIN            (255,228,181);
const gh_pixel GH_NAVAJOWHITE1        (255,222,173);
const gh_pixel GH_NAVAJOWHITE2        (238,207,161);
const gh_pixel GH_NAVAJOWHITE3        (205,179,139);
const gh_pixel GH_NAVAJOWHITE4        (139,121,94);
const gh_pixel GH_NAVAJO_WHITE        (255,222,173);
const gh_pixel GH_NAVAJOWHITE         (255,222,173);
const gh_pixel GH_NAVY_BLUE           (0,0,128);
const gh_pixel GH_NAVYBLUE            (0,0,128);
const gh_pixel GH_NAVY                (0,0,128);
const gh_pixel GH_OLD_LACE            (253,245,230);
const gh_pixel GH_OLDLACE             (253,245,230);
const gh_pixel GH_OLIVEDRAB1          (192,255,62);
const gh_pixel GH_OLIVEDRAB2          (179,238,58);
const gh_pixel GH_OLIVEDRAB3          (154,205,50);
const gh_pixel GH_OLIVEDRAB4          (105,139,34);
const gh_pixel GH_OLIVE_DRAB          (107,142,35);
const gh_pixel GH_OLIVEDRAB           (107,142,35);
const gh_pixel GH_LIGHT_ORANGE        (255,200,40);
const gh_pixel GH_ORANGE1             (255,165,0);
const gh_pixel GH_ORANGE2             (238,154,0);
const gh_pixel GH_ORANGE3             (205,133,0);
const gh_pixel GH_ORANGE4             (139,90,0);
const gh_pixel GH_ORANGE              (255,165,0);
const gh_pixel GH_ORANGERED1          (255,69,0);
const gh_pixel GH_ORANGERED2          (238,64,0);
const gh_pixel GH_ORANGERED3          (205,55,0);
const gh_pixel GH_ORANGERED4          (139,37,0);
const gh_pixel GH_ORANGE_RED          (255,69,0);
const gh_pixel GH_ORANGERED           (255,69,0);
const gh_pixel GH_ORCHID1             (255,131,250);
const gh_pixel GH_ORCHID2             (238,122,233);
const gh_pixel GH_ORCHID3             (205,105,201);
const gh_pixel GH_ORCHID4             (139,71,137);
const gh_pixel GH_ORCHID              (218,112,214);
const gh_pixel GH_PALE_GOLDENROD      (238,232,170);
const gh_pixel GH_PALEGOLDENROD       (238,232,170);
const gh_pixel GH_PALEGREEN1          (154,255,154);
const gh_pixel GH_PALEGREEN2          (144,238,144);
const gh_pixel GH_PALEGREEN3          (124,205,124);
const gh_pixel GH_PALEGREEN4          (84,139,84);
const gh_pixel GH_PALE_GREEN          (152,251,152);
const gh_pixel GH_PALEGREEN           (152,251,152);
const gh_pixel GH_PALETURQUOISE1      (187,255,255);
const gh_pixel GH_PALETURQUOISE2      (174,238,238);
const gh_pixel GH_PALETURQUOISE3      (150,205,205);
const gh_pixel GH_PALETURQUOISE4      (102,139,139);
const gh_pixel GH_PALE_TURQUOISE      (175,238,238);
const gh_pixel GH_PALETURQUOISE       (175,238,238);
const gh_pixel GH_PALEVIOLETRED1      (255,130,171);
const gh_pixel GH_PALEVIOLETRED2      (238,121,159);
const gh_pixel GH_PALEVIOLETRED3      (205,104,137);
const gh_pixel GH_PALEVIOLETRED4      (139,71,93);
const gh_pixel GH_PALE_VIOLET_RED     (219,112,147);
const gh_pixel GH_PALEVIOLETRED       (219,112,147);
const gh_pixel GH_PAPAYA_WHIP         (255,239,213);
const gh_pixel GH_PAPAYAWHIP          (255,239,213);
const gh_pixel GH_PEACHPUFF1          (255,218,185);
const gh_pixel GH_PEACHPUFF2          (238,203,173);
const gh_pixel GH_PEACHPUFF3          (205,175,149);
const gh_pixel GH_PEACHPUFF4          (139,119,101);
const gh_pixel GH_PEACH_PUFF          (255,218,185);
const gh_pixel GH_PEACHPUFF           (255,218,185);
const gh_pixel GH_PERU                (205,133,63);
const gh_pixel GH_PINK1               (255,181,197);
const gh_pixel GH_PINK2               (238,169,184);
const gh_pixel GH_PINK3               (205,145,158);
const gh_pixel GH_PINK4               (139,99,108);
const gh_pixel GH_PINK                (255,192,203);
const gh_pixel GH_PLUM1               (255,187,255);
const gh_pixel GH_PLUM2               (238,174,238);
const gh_pixel GH_PLUM3               (205,150,205);
const gh_pixel GH_PLUM4               (139,102,139);
const gh_pixel GH_PLUM                (221,160,221);
const gh_pixel GH_POWDER_BLUE         (176,224,230);
const gh_pixel GH_POWDERBLUE          (176,224,230);
const gh_pixel GH_PURPLE1             (155,48,255);
const gh_pixel GH_PURPLE2             (145,44,238);
const gh_pixel GH_PURPLE3             (125,38,205);
const gh_pixel GH_PURPLE4             (85,26,139);
const gh_pixel GH_PURPLE              (160,32,240);
const gh_pixel GH_RED1                (255,0,0);
const gh_pixel GH_RED2                (238,0,0);
const gh_pixel GH_RED3                (205,0,0);
const gh_pixel GH_RED4                (139,0,0);
const gh_pixel GH_RED                 (255,0,0);
const gh_pixel GH_ROSYBROWN1          (255,193,193);
const gh_pixel GH_ROSYBROWN2          (238,180,180);
const gh_pixel GH_ROSYBROWN3          (205,155,155);
const gh_pixel GH_ROSYBROWN4          (139,105,105);
const gh_pixel GH_ROSY_BROWN          (188,143,143);
const gh_pixel GH_ROSYBROWN           (188,143,143);
const gh_pixel GH_ROYALBLUE1          (72,118,255);
const gh_pixel GH_ROYALBLUE2          (67,110,238);
const gh_pixel GH_ROYALBLUE3          (58,95,205);
const gh_pixel GH_ROYALBLUE4          (39,64,139);
const gh_pixel GH_ROYAL_BLUE          (65,105,225);
const gh_pixel GH_ROYALBLUE           (65,105,225);
const gh_pixel GH_SADDLE_BROWN        (139,69,19);
const gh_pixel GH_SADDLEBROWN         (139,69,19);
const gh_pixel GH_SALMON1             (255,140,105);
const gh_pixel GH_SALMON2             (238,130,98);
const gh_pixel GH_SALMON3             (205,112,84);
const gh_pixel GH_SALMON4             (139,76,57);
const gh_pixel GH_SALMON              (250,128,114);
const gh_pixel GH_SANDY_BROWN         (244,164,96);
const gh_pixel GH_SANDYBROWN          (244,164,96);
const gh_pixel GH_SEAGREEN1           (84,255,159);
const gh_pixel GH_SEAGREEN2           (78,238,148);
const gh_pixel GH_SEAGREEN3           (67,205,128);
const gh_pixel GH_SEAGREEN4           (46,139,87);
const gh_pixel GH_SEA_GREEN           (46,139,87);
const gh_pixel GH_SEAGREEN            (46,139,87);
const gh_pixel GH_SEASHELL1           (255,245,238);
const gh_pixel GH_SEASHELL2           (238,229,222);
const gh_pixel GH_SEASHELL3           (205,197,191);
const gh_pixel GH_SEASHELL4           (139,134,130);
const gh_pixel GH_SEASHELL            (255,245,238);
const gh_pixel GH_SIENNA1             (255,130,71);
const gh_pixel GH_SIENNA2             (238,121,66);
const gh_pixel GH_SIENNA3             (205,104,57);
const gh_pixel GH_SIENNA4             (139,71,38);
const gh_pixel GH_SIENNA              (160,82,45);
const gh_pixel GH_SKYBLUE1            (135,206,255);
const gh_pixel GH_SKYBLUE2            (126,192,238);
const gh_pixel GH_SKYBLUE3            (108,166,205);
const gh_pixel GH_SKYBLUE4            (74,112,139);
const gh_pixel GH_SKY_BLUE            (135,206,235);
const gh_pixel GH_SKYBLUE             (135,206,235);
const gh_pixel GH_SLATEBLUE1          (131,111,255);
const gh_pixel GH_SLATEBLUE2          (122,103,238);
const gh_pixel GH_SLATEBLUE3          (105,89,205);
const gh_pixel GH_SLATEBLUE4          (71,60,139);
const gh_pixel GH_SLATE_BLUE          (106,90,205);
const gh_pixel GH_SLATEBLUE           (106,90,205);
const gh_pixel GH_SLATEGRAY1          (198,226,255);
const gh_pixel GH_SLATEGRAY2          (185,211,238);
const gh_pixel GH_SLATEGRAY3          (159,182,205);
const gh_pixel GH_SLATEGRAY4          (108,123,139);
const gh_pixel GH_SLATE_GRAY          (112,128,144);
const gh_pixel GH_SLATEGRAY           (112,128,144);
const gh_pixel GH_SLATE_GREY          (112,128,144);
const gh_pixel GH_SLATEGREY           (112,128,144);
const gh_pixel GH_SNOW1               (255,250,250);
const gh_pixel GH_SNOW2               (238,233,233);
const gh_pixel GH_SNOW3               (205,201,201);
const gh_pixel GH_SNOW4               (139,137,137);
const gh_pixel GH_SNOW                (255,250,250);
const gh_pixel GH_SPRINGGREEN1        (0,255,127);
const gh_pixel GH_SPRINGGREEN2        (0,238,118);
const gh_pixel GH_SPRINGGREEN3        (0,205,102);
const gh_pixel GH_SPRINGGREEN4        (0,139,69);
const gh_pixel GH_SPRING_GREEN        (0,255,127);
const gh_pixel GH_SPRINGGREEN         (0,255,127);
const gh_pixel GH_STEELBLUE1          (99,184,255);
const gh_pixel GH_STEELBLUE2          (92,172,238);
const gh_pixel GH_STEELBLUE3          (79,148,205);
const gh_pixel GH_STEELBLUE4          (54,100,139);
const gh_pixel GH_STEEL_BLUE          (70,130,180);
const gh_pixel GH_STEELBLUE           (70,130,180);
const gh_pixel GH_TAN1                (255,165,79);
const gh_pixel GH_TAN2                (238,154,73);
const gh_pixel GH_TAN3                (205,133,63);
const gh_pixel GH_TAN4                (139,90,43);
const gh_pixel GH_TAN                 (210,180,140);
const gh_pixel GH_THISTLE1            (255,225,255);
const gh_pixel GH_THISTLE2            (238,210,238);
const gh_pixel GH_THISTLE3            (205,181,205);
const gh_pixel GH_THISTLE4            (139,123,139);
const gh_pixel GH_THISTLE             (216,191,216);
const gh_pixel GH_TOMATO1             (255,99,71);
const gh_pixel GH_TOMATO2             (238,92,66);
const gh_pixel GH_TOMATO3             (205,79,57);
const gh_pixel GH_TOMATO4             (139,54,38);
const gh_pixel GH_TOMATO              (255,99,71);
const gh_pixel GH_TURQUOISE1          (0,245,255);
const gh_pixel GH_TURQUOISE2          (0,229,238);
const gh_pixel GH_TURQUOISE3          (0,197,205);
const gh_pixel GH_TURQUOISE4          (0,134,139);
const gh_pixel GH_TURQUOISE           (64,224,208);
const gh_pixel GH_VIOLET              (238,130,238);
const gh_pixel GH_VIOLETRED1          (255,62,150);
const gh_pixel GH_VIOLETRED2          (238,58,140);
const gh_pixel GH_VIOLETRED3          (205,50,120);
const gh_pixel GH_VIOLETRED4          (139,34,82);
const gh_pixel GH_VIOLET_RED          (208,32,144);
const gh_pixel GH_VIOLETRED           (208,32,144);
const gh_pixel GH_WHEAT1              (255,231,186);
const gh_pixel GH_WHEAT2              (238,216,174);
const gh_pixel GH_WHEAT3              (205,186,150);
const gh_pixel GH_WHEAT4              (139,126,102);
const gh_pixel GH_WHEAT               (245,222,179);
const gh_pixel GH_WHITE               (255,255,255);
const gh_pixel GH_WHITE_SMOKE         (245,245,245);
const gh_pixel GH_WHITESMOKE          (245,245,245);
const gh_pixel GH_YELLOW1             (255,255,0);
const gh_pixel GH_YELLOW2             (238,238,0);
const gh_pixel GH_YELLOW3             (205,205,0);
const gh_pixel GH_YELLOW4             (139,139,0);
const gh_pixel GH_YELLOW_GREEN        (154,205,50);
const gh_pixel GH_YELLOWGREEN         (154,205,50);
const gh_pixel GH_YELLOW              (255,255,0);

typedef struct {
    gh_pixel pixel;
    const char *name;
} gh_pixel_map;

static void add (gh_pixel_map m)
{
    gh_pixel::pixels[m.name] = m.pixel;
    gh_pixel::pixels2[
            (m.pixel.r << 24) |
            (m.pixel.g << 16) |
            (m.pixel.b << 8) |
            (m.pixel.a)
        ] = m.name;
}

void gh_pixel::init (void)
{
    gh_pixel_map maps[] = {
        { GH_ALICE_BLUE, "alice blue" },
        { GH_ALICEBLUE, "aliceblue" },
        { GH_ANTIQUEWHITE1, "antiquewhite1" },
        { GH_ANTIQUEWHITE2, "antiquewhite2" },
        { GH_ANTIQUEWHITE3, "antiquewhite3" },
        { GH_ANTIQUEWHITE4, "antiquewhite4" },
        { GH_ANTIQUE_WHITE, "antique white" },
        { GH_ANTIQUEWHITE, "antiquewhite" },
        { GH_AQUAMARINE1, "aquamarine1" },
        { GH_AQUAMARINE2, "aquamarine2" },
        { GH_AQUAMARINE3, "aquamarine3" },
        { GH_AQUAMARINE4, "aquamarine4" },
        { GH_AQUAMARINE, "aquamarine" },
        { GH_AZURE1, "azure1" },
        { GH_AZURE2, "azure2" },
        { GH_AZURE3, "azure3" },
        { GH_AZURE4, "azure4" },
        { GH_AZURE, "azure" },
        { GH_BEIGE, "beige" },
        { GH_BISQUE1, "bisque1" },
        { GH_BISQUE2, "bisque2" },
        { GH_BISQUE3, "bisque3" },
        { GH_BISQUE4, "bisque4" },
        { GH_BISQUE, "bisque" },
        { GH_BLACK, "black" },
        { GH_BLANCHED_ALMOND, "blanched almond" },
        { GH_BLANCHEDALMOND, "blanchedalmond" },
        { GH_BLUE1, "blue1" },
        { GH_BLUE2, "blue2" },
        { GH_BLUE3, "blue3" },
        { GH_BLUE4, "blue4" },
        { GH_BLUE, "blue" },
        { GH_BLUE_VIOLET, "blue violet" },
        { GH_BLUEVIOLET, "blueviolet" },
        { GH_BROWN1, "brown1" },
        { GH_BROWN2, "brown2" },
        { GH_BROWN3, "brown3" },
        { GH_BROWN4, "brown4" },
        { GH_BROWN, "brown" },
        { GH_BURLYWOOD1, "burlywood1" },
        { GH_BURLYWOOD2, "burlywood2" },
        { GH_BURLYWOOD3, "burlywood3" },
        { GH_BURLYWOOD4, "burlywood4" },
        { GH_BURLYWOOD, "burlywood" },
        { GH_CADETBLUE1, "cadetblue1" },
        { GH_CADETBLUE2, "cadetblue2" },
        { GH_CADETBLUE3, "cadetblue3" },
        { GH_CADETBLUE4, "cadetblue4" },
        { GH_CADET_BLUE, "cadet blue" },
        { GH_CADETBLUE, "cadetblue" },
        { GH_CHARTREUSE1, "chartreuse1" },
        { GH_CHARTREUSE2, "chartreuse2" },
        { GH_CHARTREUSE3, "chartreuse3" },
        { GH_CHARTREUSE4, "chartreuse4" },
        { GH_CHARTREUSE, "chartreuse" },
        { GH_CHOCOLATE1, "chocolate1" },
        { GH_CHOCOLATE2, "chocolate2" },
        { GH_CHOCOLATE3, "chocolate3" },
        { GH_CHOCOLATE4, "chocolate4" },
        { GH_CHOCOLATE, "chocolate" },
        { GH_CORAL1, "coral1" },
        { GH_CORAL2, "coral2" },
        { GH_CORAL3, "coral3" },
        { GH_CORAL4, "coral4" },
        { GH_CORAL, "coral" },
        { GH_CORNFLOWER_BLUE, "cornflower blue" },
        { GH_CORNFLOWERBLUE, "cornflowerblue" },
        { GH_CORNSILK1, "cornsilk1" },
        { GH_CORNSILK2, "cornsilk2" },
        { GH_CORNSILK3, "cornsilk3" },
        { GH_CORNSILK4, "cornsilk4" },
        { GH_CORNSILK, "cornsilk" },
        { GH_CYAN1, "cyan1" },
        { GH_CYAN2, "cyan2" },
        { GH_CYAN3, "cyan3" },
        { GH_CYAN4, "cyan4" },
        { GH_CYAN, "cyan" },
        { GH_DARK_BLUE, "dark blue" },
        { GH_DARKBLUE, "darkblue" },
        { GH_DARK_CYAN, "dark cyan" },
        { GH_DARKCYAN, "darkcyan" },
        { GH_DARKGOLDENROD1, "darkgoldenrod1" },
        { GH_DARKGOLDENROD2, "darkgoldenrod2" },
        { GH_DARKGOLDENROD3, "darkgoldenrod3" },
        { GH_DARKGOLDENROD4, "darkgoldenrod4" },
        { GH_DARK_GOLDENROD, "dark goldenrod" },
        { GH_DARKGOLDENROD, "darkgoldenrod" },
        { GH_DARK_GRAY, "dark gray" },
        { GH_DARKGRAY, "darkgray" },
        { GH_DARK_GREEN, "dark green" },
        { GH_DARKGREEN, "darkgreen" },
        { GH_DARK_GREY, "dark grey" },
        { GH_DARKGREY, "darkgrey" },
        { GH_DARK_KHAKI, "dark khaki" },
        { GH_DARKKHAKI, "darkkhaki" },
        { GH_DARK_MAGENTA, "dark magenta" },
        { GH_DARKMAGENTA, "darkmagenta" },
        { GH_DARKOLIVEGREEN1, "darkolivegreen1" },
        { GH_DARKOLIVEGREEN2, "darkolivegreen2" },
        { GH_DARKOLIVEGREEN3, "darkolivegreen3" },
        { GH_DARKOLIVEGREEN4, "darkolivegreen4" },
        { GH_DARK_OLIVE_GREEN, "dark olive green" },
        { GH_DARKOLIVEGREEN, "darkolivegreen" },
        { GH_DARKORANGE1, "darkorange1" },
        { GH_DARKORANGE2, "darkorange2" },
        { GH_DARKORANGE3, "darkorange3" },
        { GH_DARKORANGE4, "darkorange4" },
        { GH_DARK_ORANGE, "dark orange" },
        { GH_DARKORANGE, "darkorange" },
        { GH_DARKORCHID1, "darkorchid1" },
        { GH_DARKORCHID2, "darkorchid2" },
        { GH_DARKORCHID3, "darkorchid3" },
        { GH_DARKORCHID4, "darkorchid4" },
        { GH_DARK_ORCHID, "dark orchid" },
        { GH_DARKORCHID, "darkorchid" },
        { GH_DARK_RED, "dark red" },
        { GH_DARKRED, "darkred" },
        { GH_DARK_SALMON, "dark salmon" },
        { GH_DARKSALMON, "darksalmon" },
        { GH_DARKSEAGREEN1, "darkseagreen1" },
        { GH_DARKSEAGREEN2, "darkseagreen2" },
        { GH_DARKSEAGREEN3, "darkseagreen3" },
        { GH_DARKSEAGREEN4, "darkseagreen4" },
        { GH_DARK_SEA_GREEN, "dark sea green" },
        { GH_DARKSEAGREEN, "darkseagreen" },
        { GH_DARK_SLATE_BLUE, "dark slate blue" },
        { GH_DARKSLATEBLUE, "darkslateblue" },
        { GH_DARKSLATEGRAY1, "darkslategray1" },
        { GH_DARKSLATEGRAY2, "darkslategray2" },
        { GH_DARKSLATEGRAY3, "darkslategray3" },
        { GH_DARKSLATEGRAY4, "darkslategray4" },
        { GH_DARK_SLATE_GRAY, "dark slate gray" },
        { GH_DARKSLATEGRAY, "darkslategray" },
        { GH_DARK_SLATE_GREY, "dark slate grey" },
        { GH_DARKSLATEGREY, "darkslategrey" },
        { GH_DARK_TURQUOISE, "dark turquoise" },
        { GH_DARKTURQUOISE, "darkturquoise" },
        { GH_DARK_VIOLET, "dark violet" },
        { GH_DARKVIOLET, "darkviolet" },
        { GH_DEEPPINK1, "deeppink1" },
        { GH_DEEPPINK2, "deeppink2" },
        { GH_DEEPPINK3, "deeppink3" },
        { GH_DEEPPINK4, "deeppink4" },
        { GH_DEEP_PINK, "deep pink" },
        { GH_DEEPPINK, "deeppink" },
        { GH_DEEPSKYBLUE1, "deepskyblue1" },
        { GH_DEEPSKYBLUE2, "deepskyblue2" },
        { GH_DEEPSKYBLUE3, "deepskyblue3" },
        { GH_DEEPSKYBLUE4, "deepskyblue4" },
        { GH_DEEP_SKY_BLUE, "deep sky blue" },
        { GH_DEEPSKYBLUE, "deepskyblue" },
        { GH_DIM_GRAY, "dim gray" },
        { GH_DIMGRAY, "dimgray" },
        { GH_DIM_GREY, "dim grey" },
        { GH_DIMGREY, "dimgrey" },
        { GH_DODGERBLUE1, "dodgerblue1" },
        { GH_DODGERBLUE2, "dodgerblue2" },
        { GH_DODGERBLUE3, "dodgerblue3" },
        { GH_DODGERBLUE4, "dodgerblue4" },
        { GH_DODGER_BLUE, "dodger blue" },
        { GH_DODGERBLUE, "dodgerblue" },
        { GH_FIREBRICK1, "firebrick1" },
        { GH_FIREBRICK2, "firebrick2" },
        { GH_FIREBRICK3, "firebrick3" },
        { GH_FIREBRICK4, "firebrick4" },
        { GH_FIREBRICK, "firebrick" },
        { GH_FLORAL_WHITE, "floral white" },
        { GH_FLORALWHITE, "floralwhite" },
        { GH_FOREST_GREEN, "forest green" },
        { GH_FORESTGREEN, "forestgreen" },
        { GH_GAINSBORO, "gainsboro" },
        { GH_GHOST_WHITE, "ghost white" },
        { GH_GHOSTWHITE, "ghostwhite" },
        { GH_GOLD1, "gold1" },
        { GH_GOLD2, "gold2" },
        { GH_GOLD3, "gold3" },
        { GH_GOLD4, "gold4" },
        { GH_GOLDENROD1, "goldenrod1" },
        { GH_GOLDENROD2, "goldenrod2" },
        { GH_GOLDENROD3, "goldenrod3" },
        { GH_GOLDENROD4, "goldenrod4" },
        { GH_GOLDENROD, "goldenrod" },
        { GH_GOLD, "gold" },
        { GH_GRAY10, "gray10" },
        { GH_GRAY11, "gray11" },
        { GH_GRAY12, "gray12" },
        { GH_GRAY13, "gray13" },
        { GH_GRAY14, "gray14" },
        { GH_GRAY15, "gray15" },
        { GH_GRAY16, "gray16" },
        { GH_GRAY17, "gray17" },
        { GH_GRAY18, "gray18" },
        { GH_GRAY19, "gray19" },
        { GH_GRAY1, "gray1" },
        { GH_GRAY20, "gray20" },
        { GH_GRAY21, "gray21" },
        { GH_GRAY22, "gray22" },
        { GH_GRAY23, "gray23" },
        { GH_GRAY24, "gray24" },
        { GH_GRAY25, "gray25" },
        { GH_GRAY26, "gray26" },
        { GH_GRAY27, "gray27" },
        { GH_GRAY28, "gray28" },
        { GH_GRAY29, "gray29" },
        { GH_GRAY2, "gray2" },
        { GH_GRAY30, "gray30" },
        { GH_GRAY31, "gray31" },
        { GH_GRAY32, "gray32" },
        { GH_GRAY33, "gray33" },
        { GH_GRAY34, "gray34" },
        { GH_GRAY35, "gray35" },
        { GH_GRAY36, "gray36" },
        { GH_GRAY37, "gray37" },
        { GH_GRAY38, "gray38" },
        { GH_GRAY39, "gray39" },
        { GH_GRAY3, "gray3" },
        { GH_GRAY40, "gray40" },
        { GH_GRAY41, "gray41" },
        { GH_GRAY42, "gray42" },
        { GH_GRAY43, "gray43" },
        { GH_GRAY44, "gray44" },
        { GH_GRAY45, "gray45" },
        { GH_GRAY46, "gray46" },
        { GH_GRAY47, "gray47" },
        { GH_GRAY48, "gray48" },
        { GH_GRAY49, "gray49" },
        { GH_GRAY4, "gray4" },
        { GH_GRAY50, "gray50" },
        { GH_GRAY51, "gray51" },
        { GH_GRAY52, "gray52" },
        { GH_GRAY53, "gray53" },
        { GH_GRAY54, "gray54" },
        { GH_GRAY55, "gray55" },
        { GH_GRAY56, "gray56" },
        { GH_GRAY57, "gray57" },
        { GH_GRAY58, "gray58" },
        { GH_GRAY59, "gray59" },
        { GH_GRAY5, "gray5" },
        { GH_GRAY60, "gray60" },
        { GH_GRAY61, "gray61" },
        { GH_GRAY62, "gray62" },
        { GH_GRAY63, "gray63" },
        { GH_GRAY64, "gray64" },
        { GH_GRAY65, "gray65" },
        { GH_GRAY66, "gray66" },
        { GH_GRAY67, "gray67" },
        { GH_GRAY68, "gray68" },
        { GH_GRAY69, "gray69" },
        { GH_GRAY6, "gray6" },
        { GH_GRAY70, "gray70" },
        { GH_GRAY71, "gray71" },
        { GH_GRAY72, "gray72" },
        { GH_GRAY73, "gray73" },
        { GH_GRAY74, "gray74" },
        { GH_GRAY75, "gray75" },
        { GH_GRAY76, "gray76" },
        { GH_GRAY77, "gray77" },
        { GH_GRAY78, "gray78" },
        { GH_GRAY79, "gray79" },
        { GH_GRAY7, "gray7" },
        { GH_GRAY80, "gray80" },
        { GH_GRAY81, "gray81" },
        { GH_GRAY82, "gray82" },
        { GH_GRAY83, "gray83" },
        { GH_GRAY84, "gray84" },
        { GH_GRAY85, "gray85" },
        { GH_GRAY86, "gray86" },
        { GH_GRAY87, "gray87" },
        { GH_GRAY88, "gray88" },
        { GH_GRAY89, "gray89" },
        { GH_GRAY8, "gray8" },
        { GH_GRAY90, "gray90" },
        { GH_GRAY91, "gray91" },
        { GH_GRAY92, "gray92" },
        { GH_GRAY93, "gray93" },
        { GH_GRAY94, "gray94" },
        { GH_GRAY95, "gray95" },
        { GH_GRAY96, "gray96" },
        { GH_GRAY97, "gray97" },
        { GH_GRAY98, "gray98" },
        { GH_GRAY99, "gray99" },
        { GH_GRAY9, "gray9" },
        { GH_GRAY, "gray" },
        { GH_GREEN1, "green1" },
        { GH_GREEN2, "green2" },
        { GH_GREEN3, "green3" },
        { GH_GREEN4, "green4" },
        { GH_GREEN, "green" },
        { GH_GREEN_YELLOW, "green yellow" },
        { GH_GREENYELLOW, "greenyellow" },
        { GH_GREY10, "grey10" },
        { GH_GREY11, "grey11" },
        { GH_GREY12, "grey12" },
        { GH_GREY13, "grey13" },
        { GH_GREY14, "grey14" },
        { GH_GREY15, "grey15" },
        { GH_GREY16, "grey16" },
        { GH_GREY17, "grey17" },
        { GH_GREY18, "grey18" },
        { GH_GREY19, "grey19" },
        { GH_GREY1, "grey1" },
        { GH_GREY20, "grey20" },
        { GH_GREY21, "grey21" },
        { GH_GREY22, "grey22" },
        { GH_GREY23, "grey23" },
        { GH_GREY24, "grey24" },
        { GH_GREY25, "grey25" },
        { GH_GREY26, "grey26" },
        { GH_GREY27, "grey27" },
        { GH_GREY28, "grey28" },
        { GH_GREY29, "grey29" },
        { GH_GREY2, "grey2" },
        { GH_GREY30, "grey30" },
        { GH_GREY31, "grey31" },
        { GH_GREY32, "grey32" },
        { GH_GREY33, "grey33" },
        { GH_GREY34, "grey34" },
        { GH_GREY35, "grey35" },
        { GH_GREY36, "grey36" },
        { GH_GREY37, "grey37" },
        { GH_GREY38, "grey38" },
        { GH_GREY39, "grey39" },
        { GH_GREY3, "grey3" },
        { GH_GREY40, "grey40" },
        { GH_GREY41, "grey41" },
        { GH_GREY42, "grey42" },
        { GH_GREY43, "grey43" },
        { GH_GREY44, "grey44" },
        { GH_GREY45, "grey45" },
        { GH_GREY46, "grey46" },
        { GH_GREY47, "grey47" },
        { GH_GREY48, "grey48" },
        { GH_GREY49, "grey49" },
        { GH_GREY4, "grey4" },
        { GH_GREY50, "grey50" },
        { GH_GREY51, "grey51" },
        { GH_GREY52, "grey52" },
        { GH_GREY53, "grey53" },
        { GH_GREY54, "grey54" },
        { GH_GREY55, "grey55" },
        { GH_GREY56, "grey56" },
        { GH_GREY57, "grey57" },
        { GH_GREY58, "grey58" },
        { GH_GREY59, "grey59" },
        { GH_GREY5, "grey5" },
        { GH_GREY60, "grey60" },
        { GH_GREY61, "grey61" },
        { GH_GREY62, "grey62" },
        { GH_GREY63, "grey63" },
        { GH_GREY64, "grey64" },
        { GH_GREY65, "grey65" },
        { GH_GREY66, "grey66" },
        { GH_GREY67, "grey67" },
        { GH_GREY68, "grey68" },
        { GH_GREY69, "grey69" },
        { GH_GREY6, "grey6" },
        { GH_GREY70, "grey70" },
        { GH_GREY71, "grey71" },
        { GH_GREY72, "grey72" },
        { GH_GREY73, "grey73" },
        { GH_GREY74, "grey74" },
        { GH_GREY75, "grey75" },
        { GH_GREY76, "grey76" },
        { GH_GREY77, "grey77" },
        { GH_GREY78, "grey78" },
        { GH_GREY79, "grey79" },
        { GH_GREY7, "grey7" },
        { GH_GREY80, "grey80" },
        { GH_GREY81, "grey81" },
        { GH_GREY82, "grey82" },
        { GH_GREY83, "grey83" },
        { GH_GREY84, "grey84" },
        { GH_GREY85, "grey85" },
        { GH_GREY86, "grey86" },
        { GH_GREY87, "grey87" },
        { GH_GREY88, "grey88" },
        { GH_GREY89, "grey89" },
        { GH_GREY8, "grey8" },
        { GH_GREY90, "grey90" },
        { GH_GREY91, "grey91" },
        { GH_GREY92, "grey92" },
        { GH_GREY93, "grey93" },
        { GH_GREY94, "grey94" },
        { GH_GREY95, "grey95" },
        { GH_GREY96, "grey96" },
        { GH_GREY97, "grey97" },
        { GH_GREY98, "grey98" },
        { GH_GREY99, "grey99" },
        { GH_GREY9, "grey9" },
        { GH_GREY, "grey" },
        { GH_HONEYDEW1, "honeydew1" },
        { GH_HONEYDEW2, "honeydew2" },
        { GH_HONEYDEW3, "honeydew3" },
        { GH_HONEYDEW4, "honeydew4" },
        { GH_HONEYDEW, "honeydew" },
        { GH_HOTPINK1, "hotpink1" },
        { GH_HOTPINK2, "hotpink2" },
        { GH_HOTPINK3, "hotpink3" },
        { GH_HOTPINK4, "hotpink4" },
        { GH_HOT_PINK, "hot pink" },
        { GH_HOTPINK, "hotpink" },
        { GH_INDIANRED1, "indianred1" },
        { GH_INDIANRED2, "indianred2" },
        { GH_INDIANRED3, "indianred3" },
        { GH_INDIANRED4, "indianred4" },
        { GH_INDIAN_RED, "indian red" },
        { GH_INDIANRED, "indianred" },
        { GH_IVORY1, "ivory1" },
        { GH_IVORY2, "ivory2" },
        { GH_IVORY3, "ivory3" },
        { GH_IVORY4, "ivory4" },
        { GH_IVORY, "ivory" },
        { GH_KHAKI1, "khaki1" },
        { GH_KHAKI2, "khaki2" },
        { GH_KHAKI3, "khaki3" },
        { GH_KHAKI4, "khaki4" },
        { GH_KHAKI, "khaki" },
        { GH_LAVENDERBLUSH1, "lavenderblush1" },
        { GH_LAVENDERBLUSH2, "lavenderblush2" },
        { GH_LAVENDERBLUSH3, "lavenderblush3" },
        { GH_LAVENDERBLUSH4, "lavenderblush4" },
        { GH_LAVENDER_BLUSH, "lavender blush" },
        { GH_LAVENDERBLUSH, "lavenderblush" },
        { GH_LAVENDER, "lavender" },
        { GH_LAWN_GREEN, "lawn green" },
        { GH_LAWNGREEN, "lawngreen" },
        { GH_LEMONCHIFFON1, "lemonchiffon1" },
        { GH_LEMONCHIFFON2, "lemonchiffon2" },
        { GH_LEMONCHIFFON3, "lemonchiffon3" },
        { GH_LEMONCHIFFON4, "lemonchiffon4" },
        { GH_LEMON_CHIFFON, "lemon chiffon" },
        { GH_LEMONCHIFFON, "lemonchiffon" },
        { GH_LIGHTBLUE1, "lightblue1" },
        { GH_LIGHTBLUE2, "lightblue2" },
        { GH_LIGHTBLUE3, "lightblue3" },
        { GH_LIGHTBLUE4, "lightblue4" },
        { GH_LIGHT_BLUE, "light blue" },
        { GH_LIGHTBLUE, "lightblue" },
        { GH_LIGHT_CORAL, "light coral" },
        { GH_LIGHTCORAL, "lightcoral" },
        { GH_LIGHTCYAN1, "lightcyan1" },
        { GH_LIGHTCYAN2, "lightcyan2" },
        { GH_LIGHTCYAN3, "lightcyan3" },
        { GH_LIGHTCYAN4, "lightcyan4" },
        { GH_LIGHT_CYAN, "light cyan" },
        { GH_LIGHTCYAN, "lightcyan" },
        { GH_LIGHTGOLDENROD1, "lightgoldenrod1" },
        { GH_LIGHTGOLDENROD2, "lightgoldenrod2" },
        { GH_LIGHTGOLDENROD3, "lightgoldenrod3" },
        { GH_LIGHTGOLDENROD4, "lightgoldenrod4" },
        { GH_LIGHT_GOLDENROD, "light goldenrod" },
        { GH_LIGHTGOLDENROD, "lightgoldenrod" },
        { GH_LIGHT_GOLDENROD_YELLOW, "light goldenrod yellow" },
        { GH_LIGHTGOLDENRODYELLOW, "lightgoldenrodyellow" },
        { GH_LIGHT_GRAY, "light gray" },
        { GH_LIGHTGRAY, "lightgray" },
        { GH_LIGHT_GREEN, "light green" },
        { GH_LIGHTGREEN, "lightgreen" },
        { GH_LIGHT_GREY, "light grey" },
        { GH_LIGHTGREY, "lightgrey" },
        { GH_LIGHTPINK1, "lightpink1" },
        { GH_LIGHTPINK2, "lightpink2" },
        { GH_LIGHTPINK3, "lightpink3" },
        { GH_LIGHTPINK4, "lightpink4" },
        { GH_LIGHT_PINK, "light pink" },
        { GH_LIGHTPINK, "lightpink" },
        { GH_LIGHTSALMON1, "lightsalmon1" },
        { GH_LIGHTSALMON2, "lightsalmon2" },
        { GH_LIGHTSALMON3, "lightsalmon3" },
        { GH_LIGHTSALMON4, "lightsalmon4" },
        { GH_LIGHT_SALMON, "light salmon" },
        { GH_LIGHTSALMON, "lightsalmon" },
        { GH_LIGHT_SEA_GREEN, "light sea green" },
        { GH_LIGHTSEAGREEN, "lightseagreen" },
        { GH_LIGHTSKYBLUE1, "lightskyblue1" },
        { GH_LIGHTSKYBLUE2, "lightskyblue2" },
        { GH_LIGHTSKYBLUE3, "lightskyblue3" },
        { GH_LIGHTSKYBLUE4, "lightskyblue4" },
        { GH_LIGHT_SKY_BLUE, "light sky blue" },
        { GH_LIGHTSKYBLUE, "lightskyblue" },
        { GH_LIGHT_SLATE_BLUE, "light slate blue" },
        { GH_LIGHTSLATEBLUE, "lightslateblue" },
        { GH_LIGHT_SLATE_GRAY, "light slate gray" },
        { GH_LIGHTSLATEGRAY, "lightslategray" },
        { GH_LIGHT_SLATE_GREY, "light slate grey" },
        { GH_LIGHTSLATEGREY, "lightslategrey" },
        { GH_LIGHTSTEELBLUE1, "lightsteelblue1" },
        { GH_LIGHTSTEELBLUE2, "lightsteelblue2" },
        { GH_LIGHTSTEELBLUE3, "lightsteelblue3" },
        { GH_LIGHTSTEELBLUE4, "lightsteelblue4" },
        { GH_LIGHT_STEEL_BLUE, "light steel blue" },
        { GH_LIGHTSTEELBLUE, "lightsteelblue" },
        { GH_LIGHTYELLOW1, "lightyellow1" },
        { GH_LIGHTYELLOW2, "lightyellow2" },
        { GH_LIGHTYELLOW3, "lightyellow3" },
        { GH_LIGHTYELLOW4, "lightyellow4" },
        { GH_LIGHT_YELLOW, "light yellow" },
        { GH_LIGHTYELLOW, "lightyellow" },
        { GH_LIME_GREEN, "lime green" },
        { GH_LIMEGREEN, "limegreen" },
        { GH_LIME, "lime" },
        { GH_LINEN, "linen" },
        { GH_MAGENTA1, "magenta1" },
        { GH_MAGENTA2, "magenta2" },
        { GH_MAGENTA3, "magenta3" },
        { GH_MAGENTA4, "magenta4" },
        { GH_MAGENTA, "magenta" },
        { GH_MAROON1, "maroon1" },
        { GH_MAROON2, "maroon2" },
        { GH_MAROON3, "maroon3" },
        { GH_MAROON4, "maroon4" },
        { GH_MAROON, "maroon" },
        { GH_MEDIUM_AQUAMARINE, "medium aquamarine" },
        { GH_MEDIUMAQUAMARINE, "mediumaquamarine" },
        { GH_MEDIUM_BLUE, "medium blue" },
        { GH_MEDIUMBLUE, "mediumblue" },
        { GH_MEDIUMORCHID1, "mediumorchid1" },
        { GH_MEDIUMORCHID2, "mediumorchid2" },
        { GH_MEDIUMORCHID3, "mediumorchid3" },
        { GH_MEDIUMORCHID4, "mediumorchid4" },
        { GH_MEDIUM_ORCHID, "medium orchid" },
        { GH_MEDIUMORCHID, "mediumorchid" },
        { GH_MEDIUMPURPLE1, "mediumpurple1" },
        { GH_MEDIUMPURPLE2, "mediumpurple2" },
        { GH_MEDIUMPURPLE3, "mediumpurple3" },
        { GH_MEDIUMPURPLE4, "mediumpurple4" },
        { GH_MEDIUM_PURPLE, "medium purple" },
        { GH_MEDIUMPURPLE, "mediumpurple" },
        { GH_MEDIUM_SEA_GREEN, "medium sea green" },
        { GH_MEDIUMSEAGREEN, "mediumseagreen" },
        { GH_MEDIUM_SLATE_BLUE, "medium slate blue" },
        { GH_MEDIUMSLATEBLUE, "mediumslateblue" },
        { GH_MEDIUM_SPRING_GREEN, "medium spring green" },
        { GH_MEDIUMSPRINGGREEN, "mediumspringgreen" },
        { GH_MEDIUM_TURQUOISE, "medium turquoise" },
        { GH_MEDIUMTURQUOISE, "mediumturquoise" },
        { GH_MEDIUM_VIOLET_RED, "medium violet red" },
        { GH_MEDIUMVIOLETRED, "mediumvioletred" },
        { GH_MIDNIGHT_BLUE, "midnight blue" },
        { GH_MIDNIGHTBLUE, "midnightblue" },
        { GH_MINT_CREAM, "mint cream" },
        { GH_MINTCREAM, "mintcream" },
        { GH_MISTYROSE1, "mistyrose1" },
        { GH_MISTYROSE2, "mistyrose2" },
        { GH_MISTYROSE3, "mistyrose3" },
        { GH_MISTYROSE4, "mistyrose4" },
        { GH_MISTY_ROSE, "misty rose" },
        { GH_MISTYROSE, "mistyrose" },
        { GH_MOCCASIN, "moccasin" },
        { GH_NAVAJOWHITE1, "navajowhite1" },
        { GH_NAVAJOWHITE2, "navajowhite2" },
        { GH_NAVAJOWHITE3, "navajowhite3" },
        { GH_NAVAJOWHITE4, "navajowhite4" },
        { GH_NAVAJO_WHITE, "navajo white" },
        { GH_NAVAJOWHITE, "navajowhite" },
        { GH_NAVY_BLUE, "navy blue" },
        { GH_NAVYBLUE, "navyblue" },
        { GH_NAVY, "navy" },
        { GH_OLD_LACE, "old lace" },
        { GH_OLDLACE, "oldlace" },
        { GH_OLIVEDRAB1, "olivedrab1" },
        { GH_OLIVEDRAB2, "olivedrab2" },
        { GH_OLIVEDRAB3, "olivedrab3" },
        { GH_OLIVEDRAB4, "olivedrab4" },
        { GH_OLIVE_DRAB, "olive drab" },
        { GH_OLIVEDRAB, "olivedrab" },
        { GH_ORANGE1, "orange1" },
        { GH_ORANGE2, "orange2" },
        { GH_ORANGE3, "orange3" },
        { GH_ORANGE4, "orange4" },
        { GH_ORANGE, "orange" },
        { GH_ORANGERED1, "orangered1" },
        { GH_ORANGERED2, "orangered2" },
        { GH_ORANGERED3, "orangered3" },
        { GH_ORANGERED4, "orangered4" },
        { GH_ORANGE_RED, "orange red" },
        { GH_ORANGERED, "orangered" },
        { GH_ORCHID1, "orchid1" },
        { GH_ORCHID2, "orchid2" },
        { GH_ORCHID3, "orchid3" },
        { GH_ORCHID4, "orchid4" },
        { GH_ORCHID, "orchid" },
        { GH_PALE_GOLDENROD, "pale goldenrod" },
        { GH_PALEGOLDENROD, "palegoldenrod" },
        { GH_PALEGREEN1, "palegreen1" },
        { GH_PALEGREEN2, "palegreen2" },
        { GH_PALEGREEN3, "palegreen3" },
        { GH_PALEGREEN4, "palegreen4" },
        { GH_PALE_GREEN, "pale green" },
        { GH_PALEGREEN, "palegreen" },
        { GH_PALETURQUOISE1, "paleturquoise1" },
        { GH_PALETURQUOISE2, "paleturquoise2" },
        { GH_PALETURQUOISE3, "paleturquoise3" },
        { GH_PALETURQUOISE4, "paleturquoise4" },
        { GH_PALE_TURQUOISE, "pale turquoise" },
        { GH_PALETURQUOISE, "paleturquoise" },
        { GH_PALEVIOLETRED1, "palevioletred1" },
        { GH_PALEVIOLETRED2, "palevioletred2" },
        { GH_PALEVIOLETRED3, "palevioletred3" },
        { GH_PALEVIOLETRED4, "palevioletred4" },
        { GH_PALE_VIOLET_RED, "pale violet red" },
        { GH_PALEVIOLETRED, "palevioletred" },
        { GH_PAPAYA_WHIP, "papaya whip" },
        { GH_PAPAYAWHIP, "papayawhip" },
        { GH_PEACHPUFF1, "peachpuff1" },
        { GH_PEACHPUFF2, "peachpuff2" },
        { GH_PEACHPUFF3, "peachpuff3" },
        { GH_PEACHPUFF4, "peachpuff4" },
        { GH_PEACH_PUFF, "peach puff" },
        { GH_PEACHPUFF, "peachpuff" },
        { GH_PERU, "peru" },
        { GH_PINK1, "pink1" },
        { GH_PINK2, "pink2" },
        { GH_PINK3, "pink3" },
        { GH_PINK4, "pink4" },
        { GH_PINK, "pink" },
        { GH_PLUM1, "plum1" },
        { GH_PLUM2, "plum2" },
        { GH_PLUM3, "plum3" },
        { GH_PLUM4, "plum4" },
        { GH_PLUM, "plum" },
        { GH_POWDER_BLUE, "powder blue" },
        { GH_POWDERBLUE, "powderblue" },
        { GH_PURPLE1, "purple1" },
        { GH_PURPLE2, "purple2" },
        { GH_PURPLE3, "purple3" },
        { GH_PURPLE4, "purple4" },
        { GH_PURPLE, "purple" },
        { GH_RED1, "red1" },
        { GH_RED2, "red2" },
        { GH_RED3, "red3" },
        { GH_RED4, "red4" },
        { GH_RED, "red" },
        { GH_ROSYBROWN1, "rosybrown1" },
        { GH_ROSYBROWN2, "rosybrown2" },
        { GH_ROSYBROWN3, "rosybrown3" },
        { GH_ROSYBROWN4, "rosybrown4" },
        { GH_ROSY_BROWN, "rosy brown" },
        { GH_ROSYBROWN, "rosybrown" },
        { GH_ROYALBLUE1, "royalblue1" },
        { GH_ROYALBLUE2, "royalblue2" },
        { GH_ROYALBLUE3, "royalblue3" },
        { GH_ROYALBLUE4, "royalblue4" },
        { GH_ROYAL_BLUE, "royal blue" },
        { GH_ROYALBLUE, "royalblue" },
        { GH_SADDLE_BROWN, "saddle brown" },
        { GH_SADDLEBROWN, "saddlebrown" },
        { GH_SALMON1, "salmon1" },
        { GH_SALMON2, "salmon2" },
        { GH_SALMON3, "salmon3" },
        { GH_SALMON4, "salmon4" },
        { GH_SALMON, "salmon" },
        { GH_SANDY_BROWN, "sandy brown" },
        { GH_SANDYBROWN, "sandybrown" },
        { GH_SEAGREEN1, "seagreen1" },
        { GH_SEAGREEN2, "seagreen2" },
        { GH_SEAGREEN3, "seagreen3" },
        { GH_SEAGREEN4, "seagreen4" },
        { GH_SEA_GREEN, "sea green" },
        { GH_SEAGREEN, "seagreen" },
        { GH_SEASHELL1, "seashell1" },
        { GH_SEASHELL2, "seashell2" },
        { GH_SEASHELL3, "seashell3" },
        { GH_SEASHELL4, "seashell4" },
        { GH_SEASHELL, "seashell" },
        { GH_SIENNA1, "sienna1" },
        { GH_SIENNA2, "sienna2" },
        { GH_SIENNA3, "sienna3" },
        { GH_SIENNA4, "sienna4" },
        { GH_SIENNA, "sienna" },
        { GH_SKYBLUE1, "skyblue1" },
        { GH_SKYBLUE2, "skyblue2" },
        { GH_SKYBLUE3, "skyblue3" },
        { GH_SKYBLUE4, "skyblue4" },
        { GH_SKY_BLUE, "sky blue" },
        { GH_SKYBLUE, "skyblue" },
        { GH_SLATEBLUE1, "slateblue1" },
        { GH_SLATEBLUE2, "slateblue2" },
        { GH_SLATEBLUE3, "slateblue3" },
        { GH_SLATEBLUE4, "slateblue4" },
        { GH_SLATE_BLUE, "slate blue" },
        { GH_SLATEBLUE, "slateblue" },
        { GH_SLATEGRAY1, "slategray1" },
        { GH_SLATEGRAY2, "slategray2" },
        { GH_SLATEGRAY3, "slategray3" },
        { GH_SLATEGRAY4, "slategray4" },
        { GH_SLATE_GRAY, "slate gray" },
        { GH_SLATEGRAY, "slategray" },
        { GH_SLATE_GREY, "slate grey" },
        { GH_SLATEGREY, "slategrey" },
        { GH_SNOW1, "snow1" },
        { GH_SNOW2, "snow2" },
        { GH_SNOW3, "snow3" },
        { GH_SNOW4, "snow4" },
        { GH_SNOW, "snow" },
        { GH_SPRINGGREEN1, "springgreen1" },
        { GH_SPRINGGREEN2, "springgreen2" },
        { GH_SPRINGGREEN3, "springgreen3" },
        { GH_SPRINGGREEN4, "springgreen4" },
        { GH_SPRING_GREEN, "spring green" },
        { GH_SPRINGGREEN, "springgreen" },
        { GH_STEELBLUE1, "steelblue1" },
        { GH_STEELBLUE2, "steelblue2" },
        { GH_STEELBLUE3, "steelblue3" },
        { GH_STEELBLUE4, "steelblue4" },
        { GH_STEEL_BLUE, "steel blue" },
        { GH_STEELBLUE, "steelblue" },
        { GH_TAN1, "tan1" },
        { GH_TAN2, "tan2" },
        { GH_TAN3, "tan3" },
        { GH_TAN4, "tan4" },
        { GH_TAN, "tan" },
        { GH_THISTLE1, "thistle1" },
        { GH_THISTLE2, "thistle2" },
        { GH_THISTLE3, "thistle3" },
        { GH_THISTLE4, "thistle4" },
        { GH_THISTLE, "thistle" },
        { GH_TOMATO1, "tomato1" },
        { GH_TOMATO2, "tomato2" },
        { GH_TOMATO3, "tomato3" },
        { GH_TOMATO4, "tomato4" },
        { GH_TOMATO, "tomato" },
        { GH_TURQUOISE1, "turquoise1" },
        { GH_TURQUOISE2, "turquoise2" },
        { GH_TURQUOISE3, "turquoise3" },
        { GH_TURQUOISE4, "turquoise4" },
        { GH_TURQUOISE, "turquoise" },
        { GH_VIOLET, "violet" },
        { GH_VIOLETRED1, "violetred1" },
        { GH_VIOLETRED2, "violetred2" },
        { GH_VIOLETRED3, "violetred3" },
        { GH_VIOLETRED4, "violetred4" },
        { GH_VIOLET_RED, "violet red" },
        { GH_VIOLETRED, "violetred" },
        { GH_WHEAT1, "wheat1" },
        { GH_WHEAT2, "wheat2" },
        { GH_WHEAT3, "wheat3" },
        { GH_WHEAT4, "wheat4" },
        { GH_WHEAT, "wheat" },
        { GH_WHITE, "white" },
        { GH_WHITE_SMOKE, "white smoke" },
        { GH_WHITESMOKE, "whitesmoke" },
        { GH_YELLOW1, "yellow1" },
        { GH_YELLOW2, "yellow2" },
        { GH_YELLOW3, "yellow3" },
        { GH_YELLOW4, "yellow4" },
        { GH_YELLOW_GREEN, "yellow green" },
        { GH_YELLOWGREEN, "yellowgreen" },
        { GH_YELLOW, "yellow" },
    };

    for_each(maps, maps + GH_ARRAY_SIZE(maps), add);
}

void operator>> (gh_stream_in& is, gh_pixel *me)
{
    string name;
    int t;

    if (me->get_opt(is, &name, "name", string(""))) {
        *me = gh_pixel::find(name);
    } else {
        *me = GH_WHITE;
    }

    if (me->get_opt(is, &t, "r", (decltype(t))me->r)) {
        me->r = t;
    }

    if (me->get_opt(is, &t, "g", (decltype(t))me->g)) {
        me->g = t;
    }

    if (me->get_opt(is, &t, "b", (decltype(t))me->b)) {
        me->b = t;
    }

    if (me->get_opt(is, &t, "a", (decltype(t))me->a)) {
        me->a = t;
    }
}

void operator<< (gh_stream_out& os, const gh_pixel *me)
{
    int t;
    string name;

    int i = (me->r << 24) | (me->g << 16) | (me->b << 8) | (me->a);

    name = gh_pixel::pixels2[i];

    if (name != "") {
        me->put(os, &name, "name");
    }

    t = me->r;
    me->put(os, &t, "r");

    t = me->g;
    me->put(os, &t, "g");

    t = me->b;
    me->put(os, &t, "b");

    t = me->a;
    if (t != 255) {
        me->put(os, &t, "a");
    }
}

void operator>> (gh_stream_in& is, gh_fpixel *me)
{
    string name;
    float t;

    if (me->get_opt(is, &t, "r", (decltype(t))me->r)) {
        me->r = t;
    }

    if (me->get_opt(is, &t, "g", (decltype(t))me->g)) {
        me->g = t;
    }

    if (me->get_opt(is, &t, "b", (decltype(t))me->b)) {
        me->b = t;
    }

    if (me->get_opt(is, &t, "a", (decltype(t))me->a)) {
        me->a = t;
    }
}

void operator<< (gh_stream_out& os, const gh_fpixel *me)
{
    float t;

    t = me->r;
    me->put(os, &t, "r");

    t = me->g;
    me->put(os, &t, "g");

    t = me->b;
    me->put(os, &t, "b");

    t = me->a;
    me->put(os, &t, "a");
}
