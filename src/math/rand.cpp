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

    $Id: rand.cpp 1113 2008-12-16 23:36:44Z goblinhack $
*/

#include <ghack.h>
#include <gh_math.h>

uint8 rand10_val[MAX_RAND_VALUES];
int rand10_at;
uint8 rand20_val[MAX_RAND_VALUES];
int rand20_at;
uint8 rand100_val[MAX_RAND_VALUES];
int rand100_at;
uint16 rand1000_val[MAX_RAND_VALUES];
int rand1000_at;
uint16 rand10000_val[MAX_RAND_VALUES];
int rand10000_at;
uint8 rand255_val[MAX_RAND_VALUES];
int rand255_at;
uint32 rand_val[MAX_RAND_VALUES];
int rand_at;

//
// Get the next rand % 100
//
void gh_math::rand100_init (void)
{
    srand((unsigned)time(NULL));

    for (int i = 0; i < MAX_RAND_VALUES; i++) {
        rand100_val[i] = rand() % 100;
    }
}

//
// Get the next rand % 10
//
void gh_math::rand10_init (void)
{
    srand((unsigned)time(NULL));

    for (int i = 0; i < MAX_RAND_VALUES; i++) {
        rand10_val[i] = rand() % 10;
    }
}

int gh_math::rand10 (void)
{
    rand10_at++;

    if (rand10_at >= MAX_RAND_VALUES) {
        rand10_at = 0;
    }

    return (rand10_val[rand10_at]);
}

//
// Get the next rand % 20
//
void gh_math::rand20_init (void)
{
    srand((unsigned)time(NULL));

    for (int i = 0; i < MAX_RAND_VALUES; i++) {
        rand20_val[i] = rand() % 20;
    }
}

int gh_math::rand20 (void)
{
    rand20_at++;

    if (rand20_at >= MAX_RAND_VALUES) {
        rand20_at = 0;
    }

    return (rand20_val[rand20_at]);
}

//
// Get the next rand % 1000
//
void gh_math::rand1000_init (void)
{
    srand((unsigned)time(NULL));

    for (int i = 0; i < MAX_RAND_VALUES; i++) {
        rand1000_val[i] = rand() % 1000;
    }
}

int gh_math::rand1000 (void)
{
    rand1000_at++;

    if (rand1000_at >= MAX_RAND_VALUES) {
        rand1000_at = 0;
    }

    return (rand1000_val[rand1000_at]);
}

//
// Get the next rand % 10000
//
void gh_math::rand10000_init (void)
{
    srand((unsigned)time(NULL));

    for (int i = 0; i < MAX_RAND_VALUES; i++) {
        rand10000_val[i] = rand() % 1000;
    }
}

int gh_math::rand10000 (void)
{
    rand10000_at++;

    if (rand10000_at >= MAX_RAND_VALUES) {
        rand10000_at = 0;
    }

    return (rand10000_val[rand1000_at]);
}

//
// Get the next rand % 255
//
void gh_math::rand255_init (void)
{
    srand((unsigned)time(NULL));

    for (int i = 0; i < MAX_RAND_VALUES; i++) {
        rand255_val[i] = rand() % 255;
    }
}

int gh_math::rand255 (void)
{
    rand255_at++;

    if (rand255_at >= MAX_RAND_VALUES) {
        rand255_at = 0;
    }

    return (rand255_val[rand255_at]);
}

//
// Get the next rand %
//
void gh_math::randq_init (void)
{
    srand((unsigned)time(NULL));

    for (int i = 0; i < MAX_RAND_VALUES; i++) {
        rand_val[i] = rand();
    }
}

int gh_math::randq (void)
{
    rand_at++;

    if (rand_at >= MAX_RAND_VALUES) {
        rand_at = 0;
    }

    return (rand_val[rand_at]);
}

void gh_math::rand_init (void)
{
    rand10_init();
    rand20_init();
    rand100_init();
    rand1000_init();
    rand10000_init();
    rand255_init();
    randq_init();
}

float gh_math::gauss (const float m, const float s)
{
    static int use_last = 0;
    static float y2;
    float x1, x2, w, y1;

    if (use_last) {
        y1 = y2;
    } else {
        do { // ming don't have random
            x1 = 2.0 * ((float)gh_math::randq() / (float)RAND_MAX) - 1.0;
            x2 = 2.0 * ((float)gh_math::randq() / (float)RAND_MAX) - 1.0;
            w = x1 * x1 + x2 * x2;
        } while (w >= 1.0);

        w = sqrt((-2.0 * log(w)) / w);
        y1 = x1 * w;
        y2 = x2 * w;
    }

    use_last = !use_last;

    return (m + y1 * s);
}
