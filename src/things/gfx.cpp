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

    $Id: gfx.cpp 1251 2009-07-28 13:06:28Z Ghack $
*/

#include <ghack.h>
#include <gh_term.h>
#include <gh_thing.h>
#include <gh_console.h>
#include <gh_math.h>
#include <gh_figlet.h>
#include <gh_math.h>
#include <gh_templates.h>
#include <gh_audio.h>
#include <gh_music.h>

//
// For the progress bar
//
extern int gh_things_loaded;

void gh_thing::render (const gh_point p, int dir, int mode, int frame)
{
    GH_FUNCTION()

    gh_pixel shade(255, 255, 255, 255);

    if ((flash_color.r < 1.0) ||
        (flash_color.g < 1.0) || 
        (flash_color.b < 1.0) || 
        (flash_color.a < 1.0)) {

        shade.r = (uint8)(flash_color.r * 255.0);
        shade.g = (uint8)(flash_color.g * 255.0);
        shade.b = (uint8)(flash_color.b * 255.0);
        shade.a = (uint8)(flash_color.a * 255.0);
    }

    for (int level = 0; level < GH_THING_GFX_LEVEL_MAX; level++) {

        if (!anim_frames->anim_frame[dir][mode][level].size()) {
            continue;
        }

        const gh_anim_frame *a = &anim_frames->anim_frame[dir][mode][level][frame];
        const int width = a->width;
        const int height = a->height;
        const vector<gh_anim_frame_cell> *cells = &a->cell;
        bool need_random_colors = true;

        const float pix_ch_w = GH_RES_CHAR;
        const float pix_ch_h = GH_RES_CHAR;
        const float pix_mid_w = (pix_ch_w * width) / 2;
        const float pix_mid_h = (pix_ch_h * height) / 2;

        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                const gh_anim_frame_cell *acell = &((*cells)[h * width + w]);
                const char c = acell->c;

                //
                // Important to skip spaces so that shapes with spaces all
                // around get trimmed.
                //
                if (c == ' ') {
                    if (acell->bg == GH_TERM_COLOR_BLACK) {
                        continue;
                    }
                }

                //
                // Pallid dead bodies!
                //
                uint8 fg;
                uint8 bg;

                if (is_dead) {
                    fg = GH_TERM_COLOR_GRAY_4;
                    bg = GH_TERM_COLOR_BLACK;
                } else {
                    fg = acell->fg;
                    bg = acell->bg;

                    if (need_random_colors &&
                        ((fg == GH_TERM_COLOR_RANDOM) ||
                         (bg == GH_TERM_COLOR_RANDOM))) {
                        need_random_colors = false;
                        console.random_color.r = gh_math::rand255();
                        console.random_color.g = gh_math::rand255();
                        console.random_color.b = gh_math::rand255();
                    }
                }

                gh_fpoint tl(- pix_mid_w + pix_ch_w * w,
                             - pix_mid_h + pix_ch_h * h);

                gh_fpoint br(tl.x + pix_ch_w,
                             tl.y + pix_ch_h );

                console.char_at(c, tl, br, fg, bg,
                                level > 0 ? true : false /* blend */,
                                shade);
            }
        }
    }
}

void gh_thing::get_size (int *w, int *h)
{
    GH_FUNCTION()

    for (int level = 0; level < GH_THING_GFX_LEVEL_MAX; level++) {

        if (!anim_frames->anim_frame[dir][mode][level].size()) {
            continue;
        }

        const gh_anim_frame *a =
                        &anim_frames->anim_frame[dir][mode][level][frame];
        *w = a->width;
        *h = a->height;

        return;
    }
}

void gh_thing::console_render (const gh_point tl,
                               int dir, int mode, int frame)
{
    GH_FUNCTION()

    for (int level = 0; level < GH_THING_GFX_LEVEL_MAX; level++) {

        if (!anim_frames->anim_frame[dir][mode][level].size()) {
            continue;
        }

        const gh_anim_frame *a =
                        &anim_frames->anim_frame[dir][mode][level][frame];
        const int width = a->width;
        const int height = a->height;
        const vector<gh_anim_frame_cell> *cells = &a->cell;
        bool need_random_colors = true;

        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                const gh_anim_frame_cell *acell = &((*cells)[h * width + w]);
                char c = acell->c;

                //
                // Pallid dead bodies!
                //
                uint8 fg;
                uint8 bg;

                if (is_dead) {
                    fg = GH_TERM_COLOR_GRAY_4;
                    bg = GH_TERM_COLOR_BLACK;
                } else {
                    fg = acell->fg;
                    bg = acell->bg;

                    if (need_random_colors &&
                        ((fg == GH_TERM_COLOR_RANDOM) ||
                         (bg == GH_TERM_COLOR_RANDOM))) {
                        need_random_colors = false;
                        console.random_color.r = gh_math::rand255();
                        console.random_color.g = gh_math::rand255();
                        console.random_color.b = gh_math::rand255();
                    }
                }

                //
                // Important to skip spaces so that shapes with spaces all
                // around get trimmed.
                //
                if (c == ' ') {
                    fg = GH_TERM_COLOR_GRAY_1;
                    bg = GH_TERM_COLOR_GRAY_1;
                    c = '_';
                }

                gh_term_goto(tl.x + w - (width / 2),
                             tl.y + h - (height / 2));
                gh_term_fgbg((gh_term_color)fg, (gh_term_color)bg);
                gh_term_putc(c);
            }
        }
    }
}

#ifdef DEBUG_RENDER
void blit_quad (const gh_fpoint3d &at, const float size_x, const float size_y)
{
    GH_FUNCTION()

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(at.x, at.y, at.z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(at.x+size_x, at.y, at.z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(at.x+size_x, at.y+size_y, at.z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(at.x, at.y+size_y, at.z);

    glEnd();
}
#endif

//
// Render this thing to the screen and generate a texture for it
//
void gh_thing::render_cached (const int frame,
                              const int dir,
                              const int mode,
                              const bool must_render_slowly)
{
    GH_FUNCTION()

    bool invisible;

    if (must_render_slowly) {
        if (gh_global::debug) {
            GH_LOG("    frame " + tostring(frame) + " is rendered slowly");
        }
    }

    if (!anim_frames->anim_frame[dir][mode][0].size()) {
        return;
    }

    gh_anim_frame *a = &anim_frames->anim_frame[dir][mode][0][frame];

    const float pix_ch_w = GH_RES_CHAR;
    const float pix_ch_h = GH_RES_CHAR;
    const float pix_max_w = pix_ch_w * a->width;
    const float pix_max_h = pix_ch_h * a->height;
    const float pix_mid_w = pix_max_w / 2;
    const float pix_mid_h = pix_max_h / 2;

    vector<gh_anim_frame_cell> *cells = &a->cell;

    gh_fpoint pix_minv(9999, 9999);
    gh_fpoint pix_maxv(-9999, -9999);
    gh_fpoint obs_pix_minv(9999, 9999);
    gh_fpoint obs_pix_maxv(-9999, -9999);
    bool got_one = false;

    a->can_touch = 0;
    a->can_hit = 0;
    a->obstacle = 0;

    //
    // First time around, render the shape and create the textures
    // needed for the background/foreground combo
    //
    for (int h = 0; h < a->height; h++) {
        for (int w = 0; w < a->width; w++) {
            gh_anim_frame_cell *acell = &((*cells)[h * a->width + w]);

            const char c = acell->c;

            a->can_touch |= acell->can_touch_mask;
            a->can_hit |= acell->can_hit_mask;
            a->obstacle |= acell->obstacle_mask;

            //
            // Important to skip spaces so that shapes with spaces all
            // around get trimmed.
            //
            if (c == ' ') {
                if ((acell->fg == GH_TERM_COLOR_BLACK) &&
                    (acell->bg == GH_TERM_COLOR_BLACK)) {
                    continue;
                }
            }

            gh_fpoint tl(
                         pix_ch_w * w - pix_mid_w,
                         pix_ch_h * h - pix_mid_h);
            gh_fpoint br(
                         pix_ch_w * w - pix_mid_w + pix_ch_w,
                         pix_ch_h * h - pix_mid_h + pix_ch_h);

            GH_WHITE.gl();

            console.char_at(c,
                            tl,
                            br,
                            acell->fg,
                            acell->bg);

            pix_minv.x = min(tl.x, pix_minv.x);
            pix_minv.y = min(tl.y, pix_minv.y);
            pix_minv.x = min(br.x, pix_minv.x);
            pix_minv.y = min(br.y, pix_minv.y);

            pix_maxv.x = max(tl.x, pix_maxv.x);
            pix_maxv.y = max(tl.y, pix_maxv.y);
            pix_maxv.x = max(br.x, pix_maxv.x);
            pix_maxv.y = max(br.y, pix_maxv.y);

            got_one = true;
        }
    }

    //
    // Now for the foreground level, if any
    //
    if (anim_frames->anim_frame[dir][mode][1].size()) {
        gh_anim_frame *a = &anim_frames->anim_frame[dir][mode][1][frame];

        vector<gh_anim_frame_cell> *cells = &a->cell;

        //
        // First time around, render the shape and create the textures
        // needed for the background/foreground combo
        //
        for (int h = 0; h < a->height; h++) {
            for (int w = 0; w < a->width; w++) {
                gh_anim_frame_cell *acell = &((*cells)[h * a->width + w]);

                const char c = acell->c;

                //
                // Important to skip spaces so that shapes with spaces all
                // around get trimmed.
                //
                if (c == ' ') {
                    if ((acell->fg == GH_TERM_COLOR_BLACK) &&
                        (acell->bg == GH_TERM_COLOR_BLACK)) {
                        continue;
                    }
                }

                gh_fpoint tl(
                             pix_ch_w * w - pix_mid_w,
                             pix_ch_h * h - pix_mid_h);
                gh_fpoint br(
                             pix_ch_w * w - pix_mid_w + pix_ch_w,
                             pix_ch_h * h - pix_mid_h + pix_ch_h);

                GH_WHITE.gl();

                console.char_at(c,
                                tl,
                                br,
                                acell->fg,
                                acell->bg,
                                true /* blend */);
            }
        }
    }

    if (!got_one) {
        pix_minv.x = 0;
        pix_minv.y = 0;
        pix_minv.x = 0;
        pix_minv.y = 0;
        invisible = true;
    } else {
        invisible = false;
    }

    got_one = false;

    //
    // Do the same again, but for the collision mask. This is needed for
    // invisible things that have no drawn pixels.
    //
    for (int h = 0; h < a->height; h++) {
        for (int w = 0; w < a->width; w++) {
            gh_anim_frame_cell *acell = &((*cells)[h * a->width + w]);

            if (!acell->obstacle_mask) {
                continue;
            }

            gh_fpoint tl(
                         pix_ch_w * w - pix_mid_w,
                         pix_ch_h * h - pix_mid_h);
            gh_fpoint br(
                         pix_ch_w * w - pix_mid_w + pix_ch_w,
                         pix_ch_h * h - pix_mid_h + pix_ch_h);

            obs_pix_minv.x = min(tl.x, obs_pix_minv.x);
            obs_pix_minv.y = min(tl.y, obs_pix_minv.y);
            obs_pix_minv.x = min(br.x, obs_pix_minv.x);
            obs_pix_minv.y = min(br.y, obs_pix_minv.y);

            obs_pix_maxv.x = max(tl.x, obs_pix_maxv.x);
            obs_pix_maxv.y = max(tl.y, obs_pix_maxv.y);
            obs_pix_maxv.x = max(br.x, obs_pix_maxv.x);
            obs_pix_maxv.y = max(br.y, obs_pix_maxv.y);

            got_one = true;
        }
    }

    if (!got_one) {
        obs_pix_minv.x = 0;
        obs_pix_minv.y = 0;
        obs_pix_minv.x = 0;
        obs_pix_minv.y = 0;
    }

    //
    // The shape was rendered centered in the screen, work out its position
    // and add to the max bounds
    //
    gh_point p(gh_global::screensize.w / 2, gh_global::screensize.h / 2);

    int pix_min_w = (int)(pix_maxv.x - pix_minv.x);
    int pix_min_h = (int)(pix_maxv.y - pix_minv.y);
    int tex_w = next_pow_of_2(pix_min_w);
    int tex_h = next_pow_of_2(pix_min_h);

    //
    // Store pixel size info for later rendering and collision detection
    //
    a->invisible = invisible;
    a->pix_min_w = pix_min_w;
    a->pix_min_h = pix_min_h;
    a->pix_max_w = pix_max_w;
    a->pix_max_h = pix_max_h;
    a->pix_minv = pix_minv;
    a->pix_maxv = pix_maxv;
    a->obs_pix_minv = obs_pix_minv;
    a->obs_pix_maxv = obs_pix_maxv;

    //
    // Ok, min and max are the bounds of this shape. However, in collision
    // detection we use these as inclusive bounds, so we end up with being one
    // pixel too big on one side. 0 lives on the right hand side, so to speak,
    // so subtract one from the max to include this pixel at x == 0.
    //
    if (a->pix_maxv.x) {
        a->pix_maxv.x--;
    }
    if (a->pix_maxv.y) {
        a->pix_maxv.y--;
    }
    if (a->obs_pix_maxv.x) {
        a->obs_pix_maxv.x--;
    }
    if (a->obs_pix_maxv.y) {
        a->obs_pix_maxv.y--;
    }

    a->u = (float)pix_min_w / (float)tex_w;
    a->v = (float)pix_min_h / (float)tex_h;

    if (must_render_slowly) {
        //
        // We came here just to work out the sizes.
        //
        return;
    }

    //
    // Work out where on the screen to grab pixels
    //
    pix_minv.x += p.x;
    pix_maxv.x += p.x;
    pix_minv.y += p.y;
    pix_maxv.y += p.y;

    //
    // Now make a list out of these cached textures.
    //
    a->l = glGenLists(1);

    glNewList(a->l, GL_COMPILE);

        for (int level = 0; level < GH_THING_GFX_LEVEL_MAX; level++) {
            gh_anim_frame *a = &anim_frames->anim_frame[dir][mode][level][frame];

            vector<gh_anim_frame_cell> *cells = &a->cell;

            if (!anim_frames->anim_frame[dir][mode][level].size()) {
                continue;
            }

            for (int h = 0; h < a->height; h++) {
                for (int w = 0; w < a->width; w++) {
                    gh_anim_frame_cell *acell = &((*cells)[h * a->width + w]);

                    const char c = acell->c;

                    //
                    // Important to skip spaces so that shapes with spaces all
                    // around get trimmed.
                    //
                    if (c == ' ') {
                        if ((acell->fg == GH_TERM_COLOR_BLACK) &&
                            (acell->bg == GH_TERM_COLOR_BLACK)) {
                            continue;
                        }
                    }

                    gh_fpoint tl(
                                 pix_ch_w * w - pix_mid_w,
                                 pix_ch_h * h - pix_mid_h);
                    gh_fpoint br(
                                 pix_ch_w * w - pix_mid_w + pix_ch_w,
                                 pix_ch_h * h - pix_mid_h + pix_ch_h);

                    console.char_at(c,
                                    tl,
                                    br,
                                    acell->fg,
                                    acell->bg,
                                    level > 0 ? true : false /* blend */);
                }
            }
        }

    glEndList();

    //
    // If we don't need to cache this thing as a texture, then just cache
    // its list to speed up rendering
    //
    if (!cached_gfx) {
        int old_list = a->l;

        a->l = glGenLists(1);
        glNewList(a->l, GL_COMPILE);
            glCallList(old_list);
        glEndList();
        return;
    }

    gh_point tl = 
        gh_point((int)pix_minv.x,
                 gh_global::screensize.h - (int)pix_minv.y - pix_min_h);
    gh_point br = tl + gh_point(tex_w, tex_h); 

    gh_sdl_screen->clear(tl, br);

    //
    // Render the shape and then make a texture from it
    //
    glTranslatef(p.x, p.y, 0.0);
    glCallList(a->l);
    glTranslatef(-p.x, -p.y, 0.0);

    int tex;
    glGenTextures(1, (GLuint*) &tex);
    if (!tex) {
        GH_THROW("failed to generate texture");
    }

    a->tex = tex;

    //
    // 2d texture (x and y size)
    //
    glBindTexture(GL_TEXTURE_2D, tex);
    GH_WHITE.gl();

    //
    // NEAREST is quicker than LINEAR
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glCopyTexImage2D(GL_TEXTURE_2D, 0, // level of detail
                     GL_RGB,
                     (int)pix_minv.x,
                     gh_global::screensize.h - (int)pix_minv.y - pix_min_h,
                     tex_w, tex_h, // width x height
                     0 // border
                     );
    GL_ERROR_CHECK();

#ifdef DEBUG_RENDER
    blit_quad(gh_fpoint3d(0,0,0), tex_w, tex_h);
    SDL_GL_SwapBuffers();
    SDL_Delay(10);
#endif

    a->l = glGenLists(1);

    glNewList(a->l, GL_COMPILE);

        glBindTexture(GL_TEXTURE_2D, tex);

        gh_fpoint3d at(-pix_min_w/2, -pix_min_h/2, 0);
        gh_fsize size(pix_min_w, pix_min_h);

        glBegin(GL_TRIANGLE_STRIP);
        
            glTexCoord2f(0.0f, a->v);
            glVertex3f(at.x, at.y, at.z);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(at.x, at.y + size.h, at.z);
            glTexCoord2f(a->u, a->v);
            glVertex3f(at.x + size.w, at.y, at.z);
            glTexCoord2f(a->u, 0.0f);
            glVertex3f(at.x + size.w, at.y + size.h, at.z);
        
        glEnd();

    glEndList();
}

//
// Render all combinations for this thing into textures
//
void gh_thing::render_cached_all (void)
{
    GH_FUNCTION()

    static bool first = true;
    static int which_message;

    if (first) {
        first = false;
        which_message = gh_math::rand100();

        gh_music::play("HackYourWay");
    }

    int mx = (GH_TERM_WIDTH / 2);

    //
    // Progress Bar - it slows the game down a bit when loading.
    //
    gh_things_loaded++;

    if (!(gh_things_loaded % 3)) {
        gh_term_cls();

        random_fg = (gh_term_color)((gh_math::rand100() & 15) + 2);
        random_bg = (gh_term_color)((gh_math::rand100() & 15) + 2);

        gh_term_fgbg(GH_TERM_COLOR_RANDOM, GH_TERM_COLOR_BLACK);
        font1->printat(mx - 30, 1, "Goblin");
        font1->printat(mx + 5, 1, "Hack!");

        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        font1->printat(mx - 25, 15, "Is Loading,");
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        font1->printat(mx - 32, 22, "Lowly Mortal...");

        gh_term_fgbg(GH_TERM_COLOR_PURPLE, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, GH_TERM_HEIGHT - 12);

        gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, GH_TERM_HEIGHT - 8);
        gh_term_putfmx("Rescue little Princess Ascii from 100 levels of dungeon doom!");

        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, GH_TERM_HEIGHT - 7);
        gh_term_putfmx("(or just slay everything in sight!)");

        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        gh_term_goto(0, GH_TERM_HEIGHT - 1);
        gh_term_putfmx("Warning, this epic has flashing, terrible, graphics.");

        //
        // See how many things we've loaded. This will work only once.
        //
        int percent = (int)(
            (float)(GH_TERM_WIDTH-8) *
            ((float)gh_things_loaded / (float)gh_global::how_many_things));

        gh_term_fg(GH_TERM_COLOR_GRAY);
        gh_term_goto(percent, GH_TERM_HEIGHT - 4);
        gh_term_putf(" /\\ ");
        gh_term_goto(percent, GH_TERM_HEIGHT - 3);
        gh_term_putf("[%%fg=whiteoo%%fg=gray]");
        gh_term_goto(percent, GH_TERM_HEIGHT - 2);
        gh_term_putf("%%fg=red ~~ ");

        gh_term_fg(GH_TERM_COLOR_GREEN);
        gh_term_goto(percent-25 + percent/5, GH_TERM_HEIGHT - 4);
        gh_term_putf(" /\\ ");
        gh_term_goto(percent-25 + percent/5, GH_TERM_HEIGHT - 3);
        gh_term_putf("(%%fg=whiteoo%%fg=green)");
        gh_term_goto(percent-25 + percent/5, GH_TERM_HEIGHT - 2);
        gh_term_putf("%%fg=red V~ ");

        gh_term_fg(GH_TERM_COLOR_PINK);
        gh_term_goto(GH_TERM_WIDTH-4, GH_TERM_HEIGHT - 4);
        gh_term_putf(" /\\ ");
        gh_term_goto(GH_TERM_WIDTH-4, GH_TERM_HEIGHT - 3);
        gh_term_putf("%%fg=pink(%%fg=cyanoo%%fg=pink)");
        gh_term_goto(GH_TERM_WIDTH-4, GH_TERM_HEIGHT - 2);
        gh_term_putf("%%fg=pink/%%fg=red~~%%fg=pink\\");

        glClear(GL_COLOR_BUFFER_BIT);   
        gh_term_refresh();
        SDL_GL_SwapBuffers();
    }

    //
    // First check all directions for this thing and look for special
    // effects. If we have any, we can't cache this frame for this
    // thing and must render it slowly.
    //
    for (int dir = 0; dir < GH_THING_DIR_MAX; dir++) {
        for (int mode = 0; mode < GH_THING_MODE_MAX; mode++) {

            bool must_render_slowly = false;

            const int frames = anim_frames->anim_frame[dir][mode][0].size();
            if (!frames) {
                continue;
            }

            for (int frame = 0; frame < frames; frame++) {

                vector<gh_anim_frame_cell> *cells =
                        &anim_frames->anim_frame[dir][mode][0][frame].cell;
                const int width =
                        anim_frames->anim_frame[dir][mode][0][frame].width;
                const int height =
                        anim_frames->anim_frame[dir][mode][0][frame].height;

                for (int h = 0; h < height; h++) {
                    for (int w = 0; w < width; w++) {
                        gh_anim_frame_cell *acell = &((*cells)[h * width + w]);

                        if (acell->fg == GH_TERM_COLOR_RANDOM) {
                            must_render_slowly = true;
                        }

                        if (acell->bg == GH_TERM_COLOR_RANDOM) {
                            must_render_slowly = true;
                        }

                        if (must_render_slowly) {
                            break;
                        }
                    }

                    if (must_render_slowly) {
                        break;
                    }
                }

                //
                // Pre-render this frame, we can do this fast.
                //
                render_cached(frame, dir, mode, must_render_slowly);
            }
        }
    }
}
