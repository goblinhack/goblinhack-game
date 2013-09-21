/*
    ghack.
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

    $Id: gfx.cpp 1289 2009-08-12 15:53:40Z Goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_game.h>
#include <gh_thing.h>
#include <gh_room.h>
#include <gh_term.h>
#include <gh_player.h>
#include <gh_ui_thing.h>
#include <gh_math.h>

#include <stdlib.h>
#include <gh_surface.h>
#include <gh_console.h>
#include <gh_console_screen.h>

//
// Preserve the old scale so we can tell when it changes.
//
static float old_scale;

//
// Per frame count; used to make things flicker every other frame.
//
static int map_frame_count;

//
// Work out what map cell the hero char is sitting in
//
static gh_thing *hero;

//
// Where a particular thing is being drawn, centered.
//
static gh_point center_pixel_to_point;

//
// We use this to tag things that we've seen in this frame for
// the vision algo.
//
uint32 gh_rendered_this_frame;

static float gh_sleep_scroll = 3;

//
// Hacks to see how slow the rendering is going
//
uint32 gh_map_gfx_slow;
uint32 gh_map_gfx_cached;

//
// Render an individual thing with all its attributes, like invisibility,
// raging etc...
//
static void render_thing (gh_thing *t)
{
    gh_level *level = t->level;
    const int res = level->map.res;

    const uint32 frames_sz = t->get_anim_size();
    if (!frames_sz) {
	return;
    }

    const gh_anim_frame *a = t->get_anim();

    if (!level->is_editing) {
	//
	// If shielded, flicker.
	//
	if (t->shielded()) {
	    if (map_frame_count & 1) {
		return;
	    }
	}

	//
	// Invisible? As in has no cells to draw.
	//
	if (a->invisible) {
	    return;
	}
    }

    //
    // If invisible via a spell, green
    //
    if (!t->is_dead) {
	if (t->invisible()) {
	    t->flash(GH_GREEN);
	}

	//
	// Animate this thing. No wriggling dead bodies!
	//
        if (!(t->is_monst && t->sleeping())) {
	    if (t->anim_speed != 255) {
		t->frame = ((++t->anim) / t->anim_speed) % frames_sz;
	    } else {
		t->frame = ((t->anim) / t->anim_speed) % frames_sz;
	    }
	}
    }

    //
    // Move this thing to its offset form the center pixel for this cell.
    //
    gh_point p(t->at.x % res, t->at.y % res);

    p = p + center_pixel_to_point;

    glTranslatef(p.x, p.y, t->render_depth);

    //
    // We use the hero as a kind of cursor in the level editing mode. Don't 
    // display them and instead just show a square as where we're drawing.
    //
    if (level->is_editing) {
	if (t->is_hero || (t->is_weapon && (t->weapon_wielder == hero))) {

    string editing = level->map_char_nth(level->is_editing_map_char,
				     level->is_editing_which_thing[
				     level->is_editing_map_char]);

    level->map.pop(t);
    gh_thing *existing_thing = level->map.find_first_thing_same_at(t->at,
								   NULL);
    if (existing_thing) {
	existing_thing->flash(GH_RED);
    }
    level->map.push(t);

	    glBindTexture(GL_TEXTURE_2D,0);
	    GH_GREEN.gl();

	    glLineWidth(5.0);

	    glBegin(GL_LINES);
		glVertex3f(0, -GH_RES,100);
		glVertex3f(0, +GH_RES,100);
	    glEnd();

	    glBegin(GL_LINES);
		glVertex3f(-GH_RES, 0,100);
		glVertex3f(+GH_RES, 0,100);
	    glEnd();

	    glLineWidth(1.0);

	    if (t->text != "") {
		console.putf_at(t->text, 
				gh_point3d(0, (int)-a->pix_max_h/2,0));
	    }

	    if (gh_global::debug) {
		if (t->debug_string != "") {
		    console.putf_at(t->debug_string, 
				    gh_point3d(0, (int)-a->pix_max_h,0));
		}
	    }

	    glTranslatef(-p.x, -p.y, -t->render_depth);
	    return;
	}
    }

    bool cache = true;

    if (t->is_transparent) {
	//
	// This thing is partially visible. Blend it.
	//
	glEnable(GL_BLEND);

	if (t->is_gas) {
	    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR);
	} else {
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	glColor4ub(255,255,255, (int)(2.5 * (100 - t->opacity)));

    } else if ((t->flash_color.r < 1.0) ||
	       (t->flash_color.g < 1.0) ||
	       (t->flash_color.b < 1.0) ||
	       (t->flash_color.a < 1.0)) {
	//
	// The flash colors always try to return to 1.0, so you start
	// at say RED (1,0,0) and try to return to (1,1,1)
	//
	
	//
	// Fully visible, but colored.
	//
	t->flash_color.gl();

	//
	// Make the color fade to white again.
	//
	
	if (t->flash_color.r < 1.00) {
	    t->flash_color.r += (1.0 - t->flash_color.r) / 50.0;
	    if (t->flash_color.r > 0.95) {
		t->flash_color.r = 1.00;
	    }
	}

	if (t->flash_color.g < 1.00) {
	    t->flash_color.g += (1.0 - t->flash_color.g) / 50.0;
	    if (t->flash_color.g > 0.95) {
		t->flash_color.g = 1.00;
	    }
	}

	if (t->flash_color.b < 1.00) {
	    t->flash_color.b += (1.0 - t->flash_color.b) / 50.0;
	    if (t->flash_color.b > 0.95) {
		t->flash_color.b = 1.00;
	    }
	}

	if (t->flash_color.a < 1.00) {
	    t->flash_color.a += (1.0 - t->flash_color.a) / 50.0;
	    if (t->flash_color.a > 0.95) {
		t->flash_color.a = 1.00;
	    }
	}
    } else {
	//
	// Fully visible, not colored.
	//
	GH_WHITE.gl();
    }

    if (cache && a->l && !t->is_dead) {
	t->flash_color.gl();
	//
	// Draw it quickly with cached tiles.
	//
	glCallList(a->l);
	GH_WHITE.gl();

	gh_map_gfx_cached++;
    } else {
	//
	// Draw it slowly.
	//
	t->render(p, t->dir, t->mode, t->frame);

	gh_map_gfx_slow++;
    }

    if (t->is_transparent) {
	glDisable(GL_BLEND);
    }

    //
    // Does this thing have a name string?
    //
    if (t->is_dead && (t->is_monst || t->is_hero) && (t->text != "")) {
	console.putf_at("%%fg=greyR.I.P.",
			gh_point3d(0, (int)-a->pix_max_h/2,0));
    } else if (t->is_cursed && hero->can_see_cursed_items) {
	console.putf_at("%%fg=yellowcursed",
			gh_point3d(0, (int)-a->pix_max_h/2,0));
    }

    if (t->text != "") {
	glTranslatef(0, 0, 30);
	console.putf_at(t->text, gh_point3d(0, (int)-(a->pix_max_h/5)*6,0));
	glTranslatef(0, 0, -30);
    }

    if ((t->is_hero || t->is_monst) && t->sleeping()) {
	glTranslatef(0, 0, 30);
	console.putf_at("%%fg=purpleZzz...", gh_point3d(0, (int)-a->pix_max_h - gh_sleep_scroll,0));
	glTranslatef(0, 0, -30);
    }

    if (gh_global::debug && t->is_monst && !t->is_dead) {

	if (t->debug_string != "") {
	    glTranslatef(0, 0, 30);
	    console.putf_at(t->debug_string,gh_point3d(0, (int)-(a->pix_max_h/5)*8,0));
	    glTranslatef(0, 0, -30);
	}

	//
	// Extra debug.
	//
	for (int i = 0; i < GH_LIGHT_RAYS_MONST_MAX; i++) {
	    float th = (M_360 / GH_LIGHT_RAYS_MONST_MAX) * ((float)i);

	    glBindTexture(GL_TEXTURE_2D,0);
	    GH_ORANGE.gl();

	    glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(t->light_ray_length[i] * cos(th),
			   t->light_ray_length[i] * sin(th));
	    glEnd();
	}
    }

    glTranslatef(-p.x, -p.y, -t->render_depth);
}

void gh_player::display_map (void)
{
    GH_FUNCTION()

    gh_map_gfx_slow = 0;
    gh_map_gfx_cached = 0;

    gh_sleep_scroll += 0.1;

    if (gh_sleep_scroll > 10) {
	gh_sleep_scroll = 3;
    }

    //
    // We use this to tag things that we've seen in this frame for
    // the vision algo.
    //
    gh_rendered_this_frame++;

    map_frame_count++;

    //
    // Perhaps the map forgot to add a player in it?
    //
    if (!thing) {
	GH_THROW("no player to draw map around?!");
    }

    gh_level *level = thing->level;
    if (!level) {
	GH_THROW("no level to draw?!");
	return;
    }

    //
    // Push a scaling matrix.
    //
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // {

    //
    // Shake the screen if needed.
    // 
    if (((int)gh_global::screen_shake) > 0.0) {
	glTranslatef(rand() % (int)gh_global::screen_shake, 
		     rand() % (int)gh_global::screen_shake, 0.0);

	gh_global::screen_shake *= 0.96;
    }

    //
    // Work out what can be seen. If we don't have the hero yet, we just
    // pluck the max number we expect.
    //
    static int can_see_distance = 54;

    //
    // And how much to shrink/grow the map by
    //
    float scale = 2.0 - (1.0 / 54.0) * can_see_distance;

    //
    // Don't zoom in too close
    //
    if (scale > 1.918) {
	scale = 1.918;
    }

    //
    // Or out too far
    //
    if (scale < 0.01) {
	scale = 0.01;
    }

    if (gh_global::screensize.w >= 1280) {
	//
	// 1280x1024
	//
	if (gh_global::screensize.h >= 1024) {
	    //
	    // scale is ok
	    //
	} else {
	    //
	    // Fancy laptop, 1280x800
	    //
	    //
	    // scale is ok
	    //
	}
    } else if (gh_global::screensize.w >= 1024) {
	//
	// 1024x800
	//
	
	//
	// scale is ok
	//
    } else if (gh_global::screensize.w >= 800) {
	//
	// 800x600
	//
	scale /= 1.5;
    } else {
	//
	// 640x480
	//
	scale /= 2;
    }

    static bool first = true;
    if (first) {
	first = false;
	old_scale = scale;
    }

    glScalef(scale, scale, scale);

    //
    // How close to the edge we allow the player to get before scrolling
    //
    static const gh_point3d smooth_scroll_border(350, 300, 0);

    //
    // The bit to the right for scores and stuff
    //
    int rhs_border = gh_global::screensize.w / 4;

    const int res = level->map.res;
    const int pixel_width = (int)
		    ((float)gh_global::screensize.w * (1.0 / scale));
    const int pixel_height = (int)
		    ((float)gh_global::screensize.h * (1.0 / scale));

    gh_point screen_middle_pixel((pixel_width - rhs_border) / 2,
				 (pixel_height - 0) / 2);

    screen_middle_pixel.x += res;
    gh_point3d map_iter(0,0,0);

    //
    // Keep resizing and scolling the screen until the player
    // is at a reasonable spot.
    //
    if ((level->draw_start == gh_point3d(-1,-1,-1)) &&
	(level->draw_end == gh_point3d(-1,-1,-1))) {

	level->draw_start = thing->born / res;
	level->draw_end = thing->born / res;

	//
	// We start in the middle and can see enough blocks to fill
	// the screen
	//
	level->draw_start.x -= screen_middle_pixel.x / res;
	level->draw_start.y -= screen_middle_pixel.y / res;
	level->draw_end.x += screen_middle_pixel.x / res;
	level->draw_end.y += screen_middle_pixel.y / res;

	//
	// Account for rounding errors that mean we might not fill the screen.
	//
	level->draw_end.y ++;
	level->draw_end.y ++;
	level->draw_end.y ++;
    }

    //
    // If at the map edge, scroll it until the player can see some more.
    //
    gh_point3d map_midpoint = (level->draw_end + level->draw_start) / 2;

    gh_point3d at;

    at = thing->at;

    //
    // How far away from the middle block is the player?
    //
    // Translate it to pixels, then compare with the borders.
    //
    gh_point pixel_at(
	screen_middle_pixel.x - (res * (map_midpoint.x - (at.x / res))),
	screen_middle_pixel.y - (res * (map_midpoint.y - (at.y / res))));

    //
    // The pixel offset for smooth scrolling
    //
    gh_point *smooth_scroll = &level->map.smooth_scroll;

    pixel_at -= *smooth_scroll;
    pixel_at.x += at.x % res;
    pixel_at.y += at.y % res;

    if (!level->map.max_scroll_speed) {
	level->map.max_scroll_speed = 1;
    }

    //
    // If past the extremes of the map (teleport) then scroll rapidly.
    //
    if (pixel_at.x < 0) {
	level->map.max_scroll_speed += 40;
    } else if (pixel_at.y < 0) {
	level->map.max_scroll_speed += 40;
    } else if (pixel_at.x > pixel_width - (rhs_border / 2)) {
	level->map.max_scroll_speed += 40;
    } else if (pixel_at.y > pixel_height) {
	level->map.max_scroll_speed += 40;
    }

    if (pixel_at.x < smooth_scroll_border.x) {
	level->map.max_scroll_speed ++;
    } else if (pixel_at.y < smooth_scroll_border.y) {
	level->map.max_scroll_speed ++;
    } else if (pixel_at.x > pixel_width - rhs_border - smooth_scroll_border.x) {
	level->map.max_scroll_speed ++;
    } else if (pixel_at.y > pixel_height - smooth_scroll_border.y) {
	level->map.max_scroll_speed ++;
    }

    if (pixel_at.x < smooth_scroll_border.x/2) {
	level->map.max_scroll_speed ++;
    } else if (pixel_at.y < smooth_scroll_border.y/2) {
	level->map.max_scroll_speed ++;
    } else if (pixel_at.x > pixel_width - rhs_border - smooth_scroll_border.x/2) {
	level->map.max_scroll_speed ++;
    } else if (pixel_at.y > pixel_height - smooth_scroll_border.y/2) {
	level->map.max_scroll_speed ++;
    }

    if (pixel_at.x < smooth_scroll_border.x/4) {
	level->map.max_scroll_speed ++;
    } else if (pixel_at.y < smooth_scroll_border.y/4) {
	level->map.max_scroll_speed ++;
    } else if (pixel_at.x > pixel_width - rhs_border - smooth_scroll_border.x/4) {
	level->map.max_scroll_speed ++;
    } else if (pixel_at.y > pixel_height - smooth_scroll_border.y/4) {
	level->map.max_scroll_speed ++;
    }

    if (level->map.max_scroll_speed > 20) {
	level->map.max_scroll_speed /= 2;
    }

    if (level->map.max_scroll_speed) {
	level->map.max_scroll_speed--;
    }

    for (int scroll_speed = 0; scroll_speed < level->map.max_scroll_speed;
	 scroll_speed++) {

	if (pixel_at.x < smooth_scroll_border.x + res) {
	    if (!smooth_scroll->x) {
		level->draw_start.x --;
		level->draw_end.x --;
		smooth_scroll->x = res - 1;
	    } else {
		smooth_scroll->x --;
	    }
	}

	if (pixel_at.y < smooth_scroll_border.y) {
	    if (!smooth_scroll->y) {
		level->draw_start.y --;
		level->draw_end.y --;
		smooth_scroll->y = res - 1;
	    } else {
		smooth_scroll->y --;
	    }
	}

	if (pixel_at.x > pixel_width - rhs_border - smooth_scroll_border.x) {
	    if (smooth_scroll->x == (int)res - 1) {
		level->draw_start.x ++;
		level->draw_end.x ++;
		smooth_scroll->x = 0;
	    } else {
		smooth_scroll->x ++;
	    }
	}

	if (pixel_at.y > pixel_height - smooth_scroll_border.y) {
	    if (smooth_scroll->y == (int)res - 1) {
		level->draw_start.y ++;
		level->draw_end.y ++;
		smooth_scroll->y = 0;
	    } else {
		smooth_scroll->y ++;
	    }
	}
    }

    map_midpoint = (level->draw_end + level->draw_start) / 2;

    hero = thing;
    hero->has_been_seen = true;
    hero->rendered = gh_rendered_this_frame;
    if (hero) {
	level->map.hero_map_at = gh_point3d(hero->at.x / GH_RES,
					    hero->at.y / GH_RES,
					    0);
	level->map.hero_at = hero->at;
    }

    int how_many_light_rays;

    if (gh_global::really_slow) {
	how_many_light_rays = GH_LIGHT_RAYS_HERO_MAX / 8;
    } else if (gh_global::too_slow) {
	how_many_light_rays = GH_LIGHT_RAYS_HERO_MAX / 4;
    } else {
	how_many_light_rays = GH_LIGHT_RAYS_HERO_MAX;
    }

    const float theta = M_360 / how_many_light_rays;
    static float radii[GH_LIGHT_RAYS_HERO_MAX];

    if (hero) {
	can_see_distance = hero->can_see_distance();

	hero->make_light_map(how_many_light_rays, radii);
    }

    glScissor(0,
	      0,
	      (int)((float)gh_global::screensize.w - rhs_border),
	      (int)((float)gh_global::screensize.h));

    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);

    //
    // I don't want to use opengl depth stuff as it makes blending harder.
    // Just enforce ordering.
    //
    bool need_transparent_loop = false;

    for (map_iter.y = level->draw_start.y;
	 map_iter.y < level->draw_end.y; map_iter.y++) {

	if ((map_iter.y < 0) || (map_iter.y >= level->map.size.h)) {
	    continue;
	}

	for (map_iter.x = level->draw_start.x;
	     map_iter.x < level->draw_end.x; map_iter.x++) {

	    if ((map_iter.x < 0) || (map_iter.x >= level->map.size.w)) {
		continue;
	    }

	    vector<gh_thing*> * v = level->map.isat(map_iter);

	    if (v == NULL) {
		continue;
	    }

	    if (v->size() == 0) {
		continue;
	    }

	    center_pixel_to_point.x =
		screen_middle_pixel.x -
		    (res * (map_midpoint.x - map_iter.x)) - res/2;

	    center_pixel_to_point.y =
		screen_middle_pixel.y -
		    (res * (map_midpoint.y - map_iter.y)) - res/2;

	    center_pixel_to_point -= *smooth_scroll;

	    vector<gh_thing*>::iterator it;

	    for (it = v->begin(); it != v->end(); it++) {
		gh_thing *t = *it;

		if ((t->room && t->room->is_lit)) {
		    //
		    // This room is bright!
		    //
		} else {
		    if (t->is_transparent) {
			need_transparent_loop = true;
			continue;
		    }

		    if (!hero) {
			//
			// No hero to update the rendered this frame
			// field, so just display whatever has been
			// seen. This could mean watching a monster
			// wander out of vision, but should be ok 8)
			//
			if (!t->has_been_seen) {
			    continue;
			}
		    } else if (t->rendered == gh_rendered_this_frame) {
			//
			// Has seen the light!
			//
		    } else if (t->has_been_seen) {
			//
			// If this thing has been seen already then
			// render it albeit weakly.
			//
			if (!t->is_water && !t->is_boring &&
			    !t->is_door && !t->is_exit) {
			    //
			    // But only for boring things like walls.
			    // No sneak peeks at monsters!
			    //
			    continue;
			}
		    } else {
			//
			// We can't see it.
			//
			continue;
		    }
		}

		if (t->is_light_source) {
		    t->render_depth += 50;
		    render_thing(t);
		    t->render_depth -= 50;
		} else {
		    render_thing(t);
		}
	    }
	}
    }

    if (scale < 0.5) {
	need_transparent_loop = false;
    }

    if (need_transparent_loop) {
	for (map_iter.y = level->draw_start.y;
	     map_iter.y < level->draw_end.y; map_iter.y++) {

	    if ((map_iter.y < 0) || (map_iter.y >= level->map.size.h)) {
		continue;
	    }

	    for (map_iter.x = level->draw_start.x;
		 map_iter.x < level->draw_end.x; map_iter.x++) {

		if ((map_iter.x < 0) || (map_iter.x >= level->map.size.w)) {
		    continue;
		}

		vector<gh_thing*> * v = level->map.isat(map_iter);

		if (v == NULL) {
		    continue;
		}

		if (v->size() == 0) {
		    continue;
		}

		center_pixel_to_point.x =
		    screen_middle_pixel.x -
			(res * (map_midpoint.x - map_iter.x)) - res/2;

		center_pixel_to_point.y =
		    screen_middle_pixel.y -
			(res * (map_midpoint.y - map_iter.y)) - res/2;

		center_pixel_to_point -= *smooth_scroll;

		vector<gh_thing*>::iterator it;

		for (it = v->begin(); it != v->end(); it++) {
		    gh_thing *t = *it;

		    if (!t->is_transparent) {
			continue;
		    }

		    if (!hero) {
			//
			// No hero to update the rendered this frame
			// field, so just display whatever has been
			// seen. This could mean watching a monster
			// wander out of vision, but should be ok 8)
			//
			if (!t->has_been_seen) {
			    continue;
			}
		    } else if (t->rendered == gh_rendered_this_frame) {
			//
			// Has seen the light!
			//
		    } else if (t->has_been_seen) {
			//
			// If this thing has been seen already then
			// render it albeit weakly.
			//
			if (!t->is_water && !t->is_boring &&
			    !t->is_door && !t->is_exit) {
			    //
			    // But only for boring things like walls.
			    // No sneak peeks at monsters!
			    //
			    continue;
			}
		    } else {
			//
			// We can't see it.
			//
			continue;
		    }

		    render_thing(t);
		}
	    }
	}
    }

    //
    // Shade out areas we can't see
    //
    center_pixel_to_point.x = 
	screen_middle_pixel.x -
	    (res * (map_midpoint.x - level->map.hero_map_at.x)) - res/2;

    center_pixel_to_point.y = 
	screen_middle_pixel.y -
	    (res * (map_midpoint.y - level->map.hero_map_at.y)) - res/2;

    center_pixel_to_point -= *smooth_scroll;

    gh_point p(level->map.hero_at.x % res, level->map.hero_at.y % res);
    p = p + center_pixel_to_point;

    glBindTexture(GL_TEXTURE_2D,0);

    //
    // Now draw an interpolated set of shadows given the above radii
    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBegin(GL_TRIANGLE_STRIP);

	//
	// Overkill, but we make the extremes of vision look like
	// torch light
	//
	float base_r = 1.0;
	float base_g = 1.0;
	float base_b = 1.0;

	static const gh_pixel cols[] = {
	    GH_FIREBRICK,
	    GH_ORANGE,
	    GH_RED4,
	    GH_RED,
	    GH_RED,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_RED,
	    GH_RED1,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_RED2,
	    GH_RED4,
	    GH_RED3,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_RED4,
	    GH_RED2,
	    GH_RED1,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_RED,
	    GH_ORANGERED,
	    GH_RED2,
	    GH_RED4,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_RED1,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_RED,
	    GH_RED1,
	    GH_RED,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_RED2,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_RED3,
	    GH_RED3,
	    GH_ORANGE,
	    GH_RED3,
	    GH_RED,
	    GH_ORANGERED,
	    GH_RED2,
	    GH_RED2,
	    GH_RED1,
	    GH_RED,
	    GH_ORANGE,
	    GH_RED2,
	    GH_RED3,
	    GH_ORANGERED,
	    GH_RED3,
	    GH_ORANGERED,
	    GH_RED4,
	    GH_ORANGE,
	    GH_RED,
	    GH_ORANGERED4,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_ORANGERED3,
	    GH_RED2,
	    GH_RED1,
	    GH_ORANGERED,
	    GH_RED,
	    GH_ORANGERED,
	    GH_RED2,
	    GH_RED3,
	    GH_ORANGE,
	    GH_RED4,
	    GH_RED2,
	    GH_ORANGE,
	    GH_ORANGERED4,
	    GH_ORANGERED,
	    GH_RED4,
	    GH_RED2,
	    GH_ORANGERED4,
	    GH_ORANGERED,
	    GH_RED4,
	    GH_RED2,
	    GH_ORANGERED3,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_ORANGERED,
	    GH_ORANGERED4,
	    GH_ORANGE,
	    GH_ORANGERED4,
	    GH_ORANGE,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_ORANGERED4,
	    GH_ORANGERED3,
	    GH_RED4,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_ORANGERED4,
	    GH_ORANGERED4,
	    GH_RED4,
	    GH_RED4,
	    GH_RED,
	    GH_RED1,
	    GH_RED,
	    GH_RED2,
	    GH_RED2,
	    GH_ORANGE,
	    GH_RED1,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_ORANGERED,
	    GH_RED,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_RED2,
	    GH_ORANGE,
	    GH_RED3,
	    GH_RED1,
	    GH_RED,
	    GH_RED2,
	    GH_RED3,
	    GH_RED3,
	    GH_RED3,
	    GH_RED4,
	    GH_RED,
	    GH_RED3,
	    GH_RED4,
	    GH_RED,
	    GH_RED2,
	    GH_RED2,
	    GH_ORANGE,
	    GH_RED1,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_RED,
	    GH_RED1,
	    GH_RED,
	    GH_RED2,
	    GH_RED3,
	    GH_RED4,
	    GH_RED3,
	    GH_RED,
	    GH_RED2,
	    GH_RED3,
	    GH_ORANGERED,
	    GH_RED4,
	    GH_ORANGERED,
	    GH_RED3,
	    GH_RED,
	    GH_RED4,
	    GH_ORANGERED3,
	    GH_ORANGERED,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_RED4,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_ORANGERED4,
	    GH_ORANGERED4,
	    GH_ORANGE4,
	    GH_ORANGERED4,
	    GH_RED,
	    GH_RED4,
	    GH_RED1,
	    GH_RED,
	    GH_ORANGE,
	    GH_RED2,
	    GH_ORANGE,
	    GH_RED3,
	    GH_RED4,
	    GH_ORANGE,
	    GH_RED4,
	    GH_RED,
	    GH_RED3,
	    GH_RED3,
	    GH_RED4,
	    GH_RED,
	    GH_RED4,
	    GH_RED,
	    GH_ORANGERED,
	    GH_RED4,
	    GH_RED2,
	    GH_ORANGERED4,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_ORANGERED4,
	    GH_ORANGERED4,
	    GH_ORANGE,
	    GH_RED1,
	    GH_ORANGE,
	    GH_RED,
	    GH_RED4,
	    GH_ORANGE,
	    GH_RED2,
	    GH_ORANGERED4,
	    GH_ORANGERED,
	    GH_RED4,
	    GH_RED2,
	    GH_ORANGERED4,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_ORANGERED,
	    GH_ORANGERED4,
	    GH_ORANGERED4,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_ORANGERED4,
	    GH_RED4,
	    GH_RED2,
	    GH_ORANGERED4,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_ORANGERED,
	    GH_ORANGERED4,
	    GH_ORANGERED,
	    GH_ORANGERED4,
	    GH_ORANGERED4,
	    GH_RED2,
	    GH_RED3,
	    GH_RED1,
	    GH_RED,
	    GH_RED2,
	    GH_ORANGE,
	    GH_RED3,
	    GH_RED1,
	    GH_ORANGE,
	    GH_RED,
	    GH_ORANGE,
	    GH_RED2,
	    GH_ORANGE,
	    GH_RED3,
	    GH_RED3,
	    GH_RED3,
	    GH_RED,
	    GH_RED3,
	    GH_ORANGE,
	    GH_RED4,
	    GH_RED,
	    GH_ORANGERED,
	    GH_ORANGE,
	    GH_ORANGERED,
	    GH_RED4,
	    GH_ORANGERED,
	    GH_RED4,
	    GH_ORANGERED,
	    GH_RED,
	    GH_RED4,
	    GH_RED,
	    GH_ORANGE,
	    GH_RED4,
	};

	for (int ray = 0; ray <= how_many_light_rays; ray++) {

	    static int col_at = 0;
	    static int col_at_next = 1;

	    static int steps = 50;
	    static int step_at = 0;

	    float step_r = ((float)cols[col_at].r -
			    (float)cols[col_at_next].r) / steps;
	    float step_g = ((float)cols[col_at].g -
			    (float)cols[col_at_next].g) / steps;
	    float step_b = ((float)cols[col_at].b -
			    (float)cols[col_at_next].b) / steps;

	    base_r = cols[col_at].r - (step_at * step_r);
	    base_g = cols[col_at].g - (step_at * step_g) / 2;
	    base_b = cols[col_at].b - (step_at * step_b) / 2;

	    if (++step_at >= steps) {
		col_at++;
		col_at_next++;
		if (col_at == GH_ARRAY_SIZE(cols)) {
		    col_at = 0;
		}
		if (col_at_next == GH_ARRAY_SIZE(cols)) {
		    col_at_next = 0;
		}
		step_at = 0;
	    }

	    int next_seg = (ray + 1) % how_many_light_rays;
	    float angle = theta * ray;
	    float radius = radii[ray % how_many_light_rays];
	    float next_radius = radii[next_seg];

	    //
	    // red line jump removal
	    //
	    if (abs((int)(radius-next_radius)) > 10) {
		//
		// If we jump to avoid Richard's very clever scheme
		// then alas it reinroduces the red line. Need to
		// stop the strip and restart.
		//
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);

		continue;
	    }

	    float c1 = gh_math::fcos(angle);
	    float s1 = gh_math::fsin(angle);

	    radius -= GH_RES / 8;
	    glColor4ub((uint8)base_r,(uint8)base_g,(uint8)base_b, 255);
	    glVertex3f(p.x + radius * c1, p.y + radius * s1, 10);

	    radius += GH_RES / 4;
	    glColor4ub((uint8)base_r,(uint8)base_g,(uint8)base_b, 50);
	    glVertex3f(p.x + radius * c1, p.y + radius * s1, 10);
	}

    glEnd();

    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR);

    //
    // The red glow!
    //
    glBegin(GL_TRIANGLE_STRIP);

	//
	// Using a ray step of 2 makes it a lot faster!
	//
	for (int ray = 0; ray <= how_many_light_rays; ray+=2) {

	    float angle = theta * ray;
	    float radius = radii[ray % how_many_light_rays];

	    radius += GH_RES / 8;

	    float c1 = gh_math::fcos(angle);
	    float s1 = gh_math::fsin(angle);

	    glColor4ub(70,10,10,100);
	    glVertex3f(p.x + radius * c1, p.y + radius * s1, 10);

	    radius += 2000;

	    glColor4ub(0,0,0,0);
	    glVertex3f(p.x + radius * c1, p.y + radius * s1, 10);
	}

    glEnd();

    glDisable(GL_BLEND);

    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);

    //
    // Display the level editor overlay.
    //
    if (level->is_editing) {
	level_editor_display_map();
    }

    //
    // If the scale of the screen changed, we might need to adjust
    // our screen bounds.
    //
    if (scale < old_scale) {
	//
	// For really big jumps like eagle vision, just reset the bounds
	//
	if (old_scale - scale > 0.1) {
	    level->draw_start = gh_point3d(-1,-1,-1);
	    level->draw_end = gh_point3d(-1,-1,-1);
	} else {
	    level->draw_start.x -= 4;
	    level->draw_start.y -= 4;
	    level->draw_end.x += 4;
	    level->draw_end.y += 4;
	}
    }

    if (scale > old_scale) {
	//
	// For really big jumps like eagle vision, just reset the bounds
	//
	if (scale - old_scale > 0.1) {
	    level->draw_start = gh_point3d(-1,-1,-1);
	    level->draw_end = gh_point3d(-1,-1,-1);
	} else {
	    //
	    // If we get in too close, this scaling just doesn't work.
	    // Probably needs to be floating point.
	    //
	    if (scale <= 0.1) {
		level->draw_start.x += 4;
		level->draw_start.y += 4;
		level->draw_end.x -= 4;
		level->draw_end.y -= 4;
	    }
	}
    }

    old_scale = scale;

    glPopMatrix(); // }
}

void gh_player::level_editor_display_map (void)
{
    gh_level *level = thing->level;
    if (!level) {
	return;
    }

    int thing_selected_index =
	level->is_editing_which_thing[level->is_editing_map_char];
    int y = 0;

    int how_many =
	    level->map_char_how_many(level->is_editing_map_char);

    if (thing_selected_index >= how_many) {
	thing_selected_index = how_many - 1;
	level->is_editing_which_thing[level->is_editing_map_char] =
			thing_selected_index;
    }

    if (thing_selected_index < 0) {
	thing_selected_index = 0;
	level->is_editing_which_thing[level->is_editing_map_char] =
			thing_selected_index;
    }

    //
    // Draw all the things in the current palette
    //
    for (;;) {

	if (!how_many) {
	    break;
	}

	if (thing_selected_index >= how_many) {
	    break;
	}

	//
	// Get the nth thing of the selected character.
	//
	string editing = level->map_char_nth(level->is_editing_map_char,
					     thing_selected_index);
	if (editing == "") {
	    break;
	}

	gh_thing *chosen_thing = gh_thing::find(editing);
	if (!chosen_thing) {
	    GH_THROW("no thing to edit, called " + editing);
	}

	//
	// Can this thing fit on the screen?
	//
	int w;
	int h;
	chosen_thing->get_size(&w, &h);

	if (y >= GH_TERM_HEIGHT) {
	    break;
	}

	//
	// Draw a box around our thing; highlighting the one we're using
	//
	if (thing_selected_index == (int)
	    level->is_editing_which_thing[level->is_editing_map_char]) {
	    gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_RED);
	} else {
	    gh_term_fgbg(GH_TERM_COLOR_GRAY_1, GH_TERM_COLOR_BLACK);
	}

	int xb = GH_TERM_WIDTH - 25;

	for (int x1 = xb + 1; x1 <= xb + w; x1++) {
	    gh_term_goto(x1, y);
	    gh_term_puts("-");
	    gh_term_goto(x1, y + h + 1);
	    gh_term_puts("-");
	}

	gh_term_goto(xb, y);
	gh_term_puts("+");
	gh_term_goto(xb, y + h + 1);
	gh_term_puts("+");
	gh_term_goto(xb + w + 1, y);
	gh_term_puts("+");
	gh_term_goto(xb + w + 1, y + h + 1);
	gh_term_puts("+");

	for (int y1 = 1; y1 <= h; y1++) {
	    gh_term_goto(xb, y + y1);
	    gh_term_puts("|");
	    gh_term_goto(xb + w + 1, y + y1);
	    gh_term_puts("|");
	}

	//
	// And draw the thing in a box with its name under it
	//
	chosen_thing->console_render(
	    gh_point(xb + (w / 2) + 1, y + (h / 2) + 1),
	    chosen_thing->dir,
	    chosen_thing->mode,
	    chosen_thing->frame);

	//
	// Animate the chosen thing.
	//
	const uint32 frames_sz = chosen_thing->get_anim_size();
	if (!frames_sz) {
	    continue;
	}

	if (chosen_thing->anim_speed != 255) {
	    chosen_thing->frame =
		((++chosen_thing->anim) / chosen_thing->anim_speed) % frames_sz;
	} else {
	    chosen_thing->frame =
		((chosen_thing->anim) / chosen_thing->anim_speed) % frames_sz;
	}

	//
	// Draw the border.
	//
	gh_term_goto(xb, y + h + 2);

	if (thing_selected_index ==
	    (int) level->is_editing_which_thing[level->is_editing_map_char]) {
	    gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_RED);
	} else {
	    gh_term_fgbg(GH_TERM_COLOR_GRAY_3, GH_TERM_COLOR_BLACK);
	}

	gh_term_puts(chosen_thing->name.c_str());

	y += h + 3;

	thing_selected_index++;

	gh_term_goto(xb + w + 1, y + h + 1);
    }

    char tmp[80];
    y = 0;

    gh_term_goto(0, y++);
    gh_term_fgbg(GH_TERM_COLOR_PURPLE, GH_TERM_COLOR_BLACK);
    snprintf(tmp, sizeof(tmp) - 1,
	     "Level Set     %s", level->level_set_name.c_str());
    gh_term_puts(tmp);

    gh_term_goto(0, y++);
    gh_term_fgbg(GH_TERM_COLOR_PURPLE, GH_TERM_COLOR_BLACK);
    snprintf(tmp, sizeof(tmp) - 1,
	     "Level         %03d", level->level_no);
    gh_term_puts(tmp);

    gh_term_goto(0, y++);
    gh_term_fgbg(GH_TERM_COLOR_PURPLE, GH_TERM_COLOR_BLACK);
    snprintf(tmp, sizeof(tmp) - 1,
	     "Map grid res  %03d", level->is_editing_grid_res);
    gh_term_puts(tmp);

    gh_term_goto(0, y++);
    gh_term_fgbg(GH_TERM_COLOR_PURPLE, GH_TERM_COLOR_BLACK);
    snprintf(tmp, sizeof(tmp) - 1,
	     "Block at      (%d,%d)",
	     level->map.hero_map_at.x,
	     level->map.hero_map_at.y);

    gh_term_goto(0, y++);
    gh_term_fgbg(GH_TERM_COLOR_PURPLE, GH_TERM_COLOR_BLACK);
    snprintf(tmp, sizeof(tmp) - 1,
	     "Pixel at      (%d,%d)",
	     level->map.hero_at.x,
	     level->map.hero_at.y);

    gh_term_puts(tmp);

    y = GH_TERM_HEIGHT - 8;

    static vector<string> key;
    static vector<string> action;

    if (!key.size()) {
	key.push_back("c-n");
	action.push_back("ew");
	key.push_back("c-s");
	action.push_back("ave");
	key.push_back("c-q");
	action.push_back("uit");
	key.push_back("c-f");
	action.push_back("ill");
	key.push_back("c-g");
	action.push_back("rid");
	key.push_back("c-t");
	action.push_back("title");
	key.push_back("c-j");
	action.push_back("ump");
	key.push_back("");
	action.push_back("");

	key.push_back(".");
	action.push_back("floor");
	key.push_back("#");
	action.push_back("corr");
	key.push_back("X");
	action.push_back("wall");
	key.push_back("x");
	action.push_back("secrtw");
	key.push_back(",");
	action.push_back("dirt");
	key.push_back("?");
	action.push_back("cave");
	key.push_back("r");
	action.push_back("ock");
	key.push_back("&");
	action.push_back("hdrock");
	key.push_back("\"");
	action.push_back("corr");
	key.push_back("");
	action.push_back("");

	key.push_back("D");
	action.push_back("oor");
	key.push_back("e");
	action.push_back("xit");
	key.push_back("E");
	action.push_back("exits");
	key.push_back("@");
	action.push_back("border");
	key.push_back("_");
	action.push_back("trap");
	key.push_back("T");
	action.push_back("telep");
	key.push_back("L");
	action.push_back("ava");
	key.push_back(";:");
	action.push_back("void");
	key.push_back("");
	action.push_back("");

	key.push_back("$");
	action.push_back("treas");
	key.push_back("*");
	action.push_back("gems");
	key.push_back("c");
	action.push_back("oins");
	key.push_back("f");
	action.push_back("ood");
	key.push_back("A");
	action.push_back("rmor");
	key.push_back("W");
	action.push_back("ands");
	key.push_back("i");
	action.push_back("item");
	key.push_back("p");
	action.push_back("otion");
	key.push_back("!-");
	action.push_back("movwall");
	key.push_back("=");
	action.push_back("elecwll");
	key.push_back("^");
	action.push_back("castle");
	key.push_back("");
	action.push_back("");

	key.push_back("1");
	action.push_back("wands");
	key.push_back("2");
	action.push_back("guns");
	key.push_back("3");
	action.push_back("bombs");
	key.push_back("4");
	action.push_back("bows");
	key.push_back("5");
	action.push_back("staffs");
	key.push_back("6");
	action.push_back("swords");
	key.push_back("");
	action.push_back("");

	key.push_back("7");
	action.push_back("axes");
	key.push_back("m");
	action.push_back("onst");
	key.push_back("M");
	action.push_back("bigmon");
	key.push_back("d");
	action.push_back("ragon");
	key.push_back("k");
	action.push_back("knight");
	key.push_back("s");
	action.push_back("nake");
	key.push_back("o");
	action.push_back("orc");
	key.push_back("g");
	action.push_back("oblin");
	key.push_back("u");
	action.push_back("ndead");
	key.push_back("");
	action.push_back("");

	key.push_back("G");
	action.push_back("enerat");
	key.push_back("w");
	action.push_back("mazmon");
	key.push_back("K");
	action.push_back("shopkp");
	key.push_back("S");
	action.push_back("hero");
	key.push_back("/\\|");
	action.push_back("water");
	key.push_back("t");
	action.push_back("ree");
	key.push_back("F");
	action.push_back("grass");
	key.push_back("%%");
	action.push_back("land");
	action.push_back("");
    }

    string key_line;
    bool first = true;

    for (size_t i=0; i < key.size(); i++) {
	char key_action[60];

	if (first) {
	    snprintf(key_action, sizeof(key_action) - 1,
		     "%%%%fg=red%3s)%%%%fg=green%-7s",
		     key[i].c_str(), action[i].c_str());
	} else {
	    snprintf(key_action, sizeof(key_action) - 1,
		     "%%%%fg=cyan%3s)%%%%fg=green%-7s",
		     key[i].c_str(), action[i].c_str());
	}

	key_line = key_line + key_action;

	if ((key_line.size() > 400) || (key[i] == "")) {
	    gh_term_goto(0, y++);
	    gh_term_putf(key_line.c_str());
	    key_line = "";
	    first = false;
	}
    }

    if (key_line.size()) {
	gh_term_goto(0, y++);
	gh_term_putf(key_line.c_str());
	key_line = "";
    }

    if (gh_global::debug) {
	gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
	gh_term_goto(0, y++);

	gh_term_puts("DEBUG MODE");
    }
}

void gh_player::display_game (void)
{
    GH_FUNCTION()

    display_map();

    if (thing->level->is_editing) {
	//
	// Always have the messages on top
	//
	thing->level->game->display_messages();
	return;
    }

    display_score();
    display_item_list();

    //
    // Always have the messages on top
    //
    thing->level->game->display_messages();
}
