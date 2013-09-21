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

    $Id: gh_math.h 522 2007-06-10 23:54:33Z goblinhack $
*/

#ifndef _GH_MATH_H_
#define _GH_MATH_H_

#include <math.h>

#undef GH_SQUARED
#define GH_SQUARED(x) ((x) * (x))

#undef GH_DISTANCE
#define GH_DISTANCE(xa,ya,xb,yb) \
    sqrt((float)(GH_SQUARED((xb)-(xa))+GH_SQUARED((yb)-(ya))))

#undef GH_DISTANCE3f
#define GH_DISTANCE3f(xa,ya,za,xb,yb,zb) \
    sqrt((float)(GH_SQUARED((xb)-(xa))+\
                 GH_SQUARED((yb)-(ya))+\
                 GH_SQUARED((zb)-(za))))

#undef GH_DIFFERENCE
#define GH_DIFFERENCE(x,y) (((y)>(x)) ? ((y)-(x)) : ((x)-(y)))

#undef M_360
#define M_360       (2.0*3.14159265358979323846)  /* 2*pi */

#undef M_270
#define M_270       (M_180 + M_90)

#undef M_180
#define M_180       3.14159265358979323846  /* pi */

#undef M_90
#define M_90        1.57079632679489661923  /* pi/2 */

#undef M_45
#define M_45        0.78539816339744830962  /* pi/4 */

class gh_math
{
public:
    //
    // round() doesn't seem to be in <math.h>, this hopefully
    // does the same, using floor(), which should be in <math.h>
    //
    static inline int round (float f)
    {
        float fl = floor(f);
        int fli = (int)fl;

        if ((f - fl) < 0.5) {
            return (fli);
        } else {
            return (fli + 1);
        }
    }

    static float gauss(const float m, const float s);

    //
    // Get the next random number % x
    //
    static int rand10(void);
    static int rand20(void);
    static int rand100(void);
    static int rand1000(void);
    static int rand10000(void);
    static int rand255(void);
    static int randq(void);
    static void rand_init(void);
    static void rand10_init(void);
    static void rand20_init(void);
    static void rand100_init(void);
    static void rand1000_init(void);
    static void rand10000_init(void);
    static void rand255_init(void);
    static void randq_init(void);
    static void fsin_init(void);
    static void fcos_init(void);
    static float fsin(const float rad);
    static float fcos(const float rad);

    static gh_inline float rad (const float angle)
    {
        return (angle * (M_360 / 360.0));
    }

    static gh_inline float deg (const float rad)
    {
        return ((360.0 * rad) / M_360);
    }

    static vector<gh_point3d> line_vec (gh_point3d a, gh_point3d b);

    //
    // Create a shearing matrix (forced leaning to one side)
    //
    static gh_inline void gh_iso_adjust_shear (const int res,
                                               const int z)
    {
        static float shear = 0.125 * 2;
        const float shear_adjust = (res / 8);

        static const float m[16] = { 1.0,    0.0,    0.0,   0.0,
                                     0.0,    1.0,    0.0,   0.0,
                                     shear,  shear,  1.0,   0.0,
                                     0.0,    0.0,    0.0,   1.0 };

        glMultMatrixf(m);

        glTranslatef(shear_adjust * (z+1) * 2,
                     shear_adjust * (z+1) * 2, 0.0);
    }

    //
    // Create a matrix to project an object onto a surface.
    //
    // direction light, w = 0, positional w = 1
    //
    // This function takes an arbitrary plane, defined as 
    //
    //    Ax + By + Cz + D = 0
    //
    // and light position in homogeneous coordinates. If the 
    // light is directional, the w value should be 0. The function 
    // concatenates the shadow matrix onto the top element of the 
    // current matrix stack.
    //
    static gh_inline void gh_shadow_matrix (float ground[4], float light[4])
    {
        float  dot;
        float  shadowMat[4][4];

        dot = ground[0] * light[0] +
              ground[1] * light[1] +
              ground[2] * light[2] +
              ground[3] * light[3];
        
        shadowMat[0][0] = dot - light[0] * ground[0];
        shadowMat[1][0] = 0.0 - light[0] * ground[1];
        shadowMat[2][0] = 0.0 - light[0] * ground[2];
        shadowMat[3][0] = 0.0 - light[0] * ground[3];
        
        shadowMat[0][1] = 0.0 - light[1] * ground[0];
        shadowMat[1][1] = dot - light[1] * ground[1];
        shadowMat[2][1] = 0.0 - light[1] * ground[2];
        shadowMat[3][1] = 0.0 - light[1] * ground[3];
        
        shadowMat[0][2] = 0.0 - light[2] * ground[0];
        shadowMat[1][2] = 0.0 - light[2] * ground[1];
        shadowMat[2][2] = dot - light[2] * ground[2];
        shadowMat[3][2] = 0.0 - light[2] * ground[3];
        
        shadowMat[0][3] = 0.0 - light[3] * ground[0];
        shadowMat[1][3] = 0.0 - light[3] * ground[1];
        shadowMat[2][3] = 0.0 - light[3] * ground[2];
        shadowMat[3][3] = dot - light[3] * ground[3];

        glMultMatrixf((const GLfloat*)shadowMat);
    }

    //
    // Function for computing a plane equation given 3 points
    //
    static gh_inline void calculate_plane (gh_fpoint3d a,
                                           gh_fpoint3d b,
                                           gh_fpoint3d c,
                                           float *plane)
    {
        plane[0] = a.y*(b.z-c.z) + b.y*(c.z-a.z) + c.y*(a.z-b.z);

        plane[1] = a.z*(b.x-c.x) + b.z*(c.x-a.x) + c.z*(a.x-b.x);

        plane[2] = a.x*(b.y-c.y) + b.x*(c.y-a.y) + c.x*(a.y-b.y);

        plane[3] = -(a.x*(b.y*c.z - c.y*b.z) +
                     b.x*(c.y*a.z - a.y*c.z) +
                     c.x*(a.y*b.z - b.y*a.z) );
    }
};

#define GH_RAD_MAX 0xFFF
#define GH_RAD_STEP (M_360 / (float)GH_RAD_MAX)

gh_inline float gh_math::fsin (float rad)
{
    const int16 index = (int16)(rad / GH_RAD_STEP);
    extern float GH_FSIN[GH_RAD_MAX];

    return (GH_FSIN[index & (GH_RAD_MAX-1)]);
}

gh_inline float gh_math::fcos (float rad)
{
    const int16 index = (int16)(rad / GH_RAD_STEP);
    extern float GH_FCOS[GH_RAD_MAX];

    return (GH_FCOS[index & (GH_RAD_MAX-1)]);
}

#define MAX_RAND_VALUES 0xFFF

gh_inline int gh_math::rand100 (void)
{
    extern int rand100_at;
    extern uint8 rand100_val[MAX_RAND_VALUES];

    rand100_at++;

    if (rand100_at >= MAX_RAND_VALUES) {
        rand100_at = 0;
    }

    return (rand100_val[rand100_at]);
}

#endif /* _GH_MATH_H_ */
