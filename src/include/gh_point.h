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

    $Id: gh_point.h 477 2007-03-10 22:40:46Z goblinhack $
*/

#ifndef _GH_POINT_H_
#define _GH_POINT_H_

template<class T, class U> class gh_apoint : public gh_stream
{
public:

    T x;
    T y;

    gh_apoint (void) : x(0), y(0) {};

    gh_apoint (T x, T y) : x(x), y(y) { }

    gh_apoint (const gh_apoint &a) : x(a.x), y(a.y) { }

    //
    // Allow conversion from unsigned sizes to signed points
    //
    gh_apoint (const U &a) : x(a.w), y(a.h) { }

    gh_inline void operator+= (gh_apoint a)
    {
        x += a.x; y += a.y;
    }

    gh_inline void operator-= (gh_apoint a)
    {
        x -= a.x; y -= a.y;
    }

    gh_inline void operator/= (gh_apoint a)
    {
        x /= a.x; y /= a.y;
    }

    gh_inline void operator*= (gh_apoint a)
    {
        x *= a.x; y *= a.y;
    }

    gh_inline void operator*= (T a)
    {
        x *= a; y *= a;
    }

    gh_inline void operator/= (T a)
    {
        x /= a; y /= a;
    }

    gh_inline friend gh_apoint operator+ (gh_apoint a, gh_apoint b)
    {
        return (gh_apoint(a.x + b.x, a.y + b.y));
    }

    gh_inline friend gh_apoint operator- (gh_apoint a, gh_apoint b)
    {
        return (gh_apoint(a.x - b.x, a.y - b.y));
    }

    gh_inline friend gh_apoint operator/ (gh_apoint a, gh_apoint b)
    {
        return (gh_apoint(a.x / b.x, a.y / b.y));
    }

    gh_inline friend gh_apoint operator/ (gh_apoint a, T b)
    {
        return (gh_apoint(a.x / b, a.y / b));
    }

    gh_inline friend gh_apoint operator* (gh_apoint a, gh_apoint b)
    {
        return (gh_apoint(a.x * b.x, a.y * b.y));
    }

    gh_inline friend bool operator== (gh_apoint a, gh_apoint b)
    {
        return (a.x == b.x) && (a.y == b.y);
    }

    gh_inline friend bool operator!= (gh_apoint a, gh_apoint b)
    {
        return (!(a==b));
    }

    gh_inline friend T distance (gh_apoint a, gh_apoint b)
    {
        return ((T) GH_DISTANCE((float)a.x,(float)a.y,(float)b.x,(float)b.y)); 
    }

    gh_inline T length (void) const
    {
        return (sqrt(x*x + y*y));
    }

    gh_inline void unit (void)
    {
        T length = sqrt(x*x + y*y);

        x /= length;
        y /= length;
    }

    gh_inline friend gh_apoint unit (const gh_apoint p)
    {
        const T length = sqrt(p.x*p.x + p.y*p.y);

        return (gh_apoint(p.x / length, p.y / length));
    }

    gh_inline T dot (const gh_apoint b) const
    {
        return (x*b.x + y*b.y);
    }

    //
    // Yields an angle between 0 and 180 deg radians
    //
    gh_inline friend T angle (const gh_apoint A, const gh_apoint B)
    {
        const T a = sqrt(A.x*A.x + A.y*A.y);
        const T b = sqrt(B.x*B.x + B.y*B.y);

        return (acos((A.x*B.x + A.y*B.y) / (a * b)));
    }

    //
    // Yields an angle between 0 and 180 deg radians
    //
    gh_inline T angle (const gh_apoint A) const
    {
        const T a = sqrt(A.x*A.x + A.y*A.y);
        const T b = sqrt(x*x + y*y);

        return (acos((A.x*x + A.y*y) / (a * b)));
    }

    //
    // Yields an angle between 0 and 360 deg radians - essentially, how much
    // this point has been rotated about the origin.
    //
    gh_inline T anglerot (void) const
    {
        T theta = asin(y / length());

        if (x > 0) {
            if (y > 0) {
                return (theta);
            } else {
                return (M_360 + theta);
            }
        } else {
            return (M_180 - theta);
        }
    }

    friend void operator>> (gh_stream_in& is, gh_apoint *me)
    {
        me->get_unnamed(is, &me->x);
        me->get_unnamed(is, &me->y);
    }

    friend void operator<< (gh_stream_out& os, const gh_apoint *me)
    {
        me->put_unnamed(os, &me->x);
        me->put_unnamed(os, &me->y);
    }
};

typedef gh_apoint<int, gh_size> gh_point;
typedef gh_apoint<float, gh_fsize> gh_fpoint;
typedef gh_apoint<double, gh_dsize> gh_dpoint;

template<class T, class U> class gh_apoint3d : public gh_stream
{
public:

    T x;
    T y;
    T z;

    gh_apoint3d (void) : x(0), y(0), z(0) { }

    gh_apoint3d (T x, T y, T z) : x(x), y(y), z(z) { }

    gh_apoint3d (const gh_apoint3d &a) : x(a.x), y(a.y), z(a.z) { }

    //
    // Allow conversion from unsigned sizes to signed points
    //
    gh_apoint3d (const U &a) : x(a.w), y(a.h), z(a.d) { }

    gh_inline void operator+= (gh_apoint3d a)
    {
        x += a.x; y += a.y; z += a.z;
    }

    gh_inline void operator-= (gh_apoint3d a)
    {
        x -= a.x; y -= a.y; z -= a.z;
    }

    gh_inline friend gh_apoint3d operator+ (gh_apoint3d a, gh_apoint3d b)
    {
        return (gh_apoint3d(a.x + b.x, a.y + b.y, a.z + b.z));
    }

    gh_inline friend gh_apoint3d operator- (gh_apoint3d a, gh_apoint3d b)
    {
        return (gh_apoint3d(a.x - b.x, a.y - b.y, a.z - b.z));
    }

    gh_inline friend gh_apoint3d operator/ (gh_apoint3d a, gh_apoint3d b)
    {
        return (gh_apoint3d(a.x / b.x, a.y / b.y, a.z / b.z));
    }

    gh_inline friend gh_apoint3d operator* (gh_apoint3d a, gh_apoint3d b)
    {
        return (gh_apoint3d(a.x * b.x, a.y * b.y, a.z * b.z));
    }

    gh_inline friend gh_apoint3d operator/ (gh_apoint3d a, T b)
    {
        return (gh_apoint3d(a.x / b, a.y / b, a.z / b));
    }

    gh_inline void operator*= (T a)
    {
        x *= a; y *= a; z *= a;
    }

    gh_inline void operator/= (T a)
    {
        x /= a; y /= a; z /= a;
    }

    gh_inline friend bool operator== (gh_apoint3d a, gh_apoint3d b)
    {
        return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
    }

    gh_inline friend bool operator!= (gh_apoint3d a, gh_apoint3d b)
    {
        return (!(a==b));
    }

    gh_inline void unit (void)
    {
        T length = sqrt(x*x + y*y + z*z);

        x /= length;
        y /= length;
        z /= length;
    }

    gh_inline friend T gh_distance (gh_apoint3d a, gh_apoint3d b)
    {
        return ((T) GH_DISTANCE3f((float)a.x,(float)a.y,(float)a.z,
                                (float)b.x,(float)b.y,(float)b.z));
    }

    gh_inline T length (void)
    {
        return (sqrt(x*x + y*y + z*z));
    }

    gh_inline friend gh_apoint3d unit (const gh_apoint3d p)
    {
        const T length = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);

        return (gh_apoint3d(p.x / length, p.y / length, p.z / length));
    }

    gh_inline T dot (const gh_apoint3d b)
    {
        return (x*b.x + y*b.y + z*b.z);
    }

    //
    // Yields an angle between 0 and 180 deg radians
    //
    gh_inline friend T angle (const gh_apoint3d A, const gh_apoint3d B)
    {
        const T a = sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
        const T b = sqrt(B.x*B.x + B.y*B.y + B.z*B.z);

        return (acos((A.x*B.x + A.y*B.y + A.z*B.z) / (a * b)));
    }

    //
    // Yields an angle between 0 and 180 deg radians
    //
    gh_inline T angle (const gh_apoint3d A) const
    {
        const T a = sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
        const T b = sqrt(x*x + y*y + z*z);

        return (acos((A.x*x + A.y*y + A.z*z) / (a * b)));
    }

    gh_inline void cross (const gh_apoint3d b) const
    {
        T xn = y*b.z - b.y*z;
        T yn = z*b.x - x*b.z;
        T zn = x*b.y - y*b.x;

        x = xn;
        y = yn;
        z = zn;
    }

    gh_inline friend gh_apoint3d cross (
        const gh_apoint3d a, const gh_apoint3d b)
    {
        return gh_apoint3d(a.y*b.z - b.y*a.z, a.z*b.x - a.x*b.z,
                           a.x*b.y - a.y*b.x);
    }

    gh_inline void scale (const T b)
    {
        x *= b;
        y *= b;
        z *= b;
    }

    gh_inline friend gh_apoint3d scale (const gh_apoint3d a, const T b)
    {
        return (gh_apoint3d(a.x * b, a.y * b, a.z * b));
    }

    friend void operator>> (gh_stream_in& is, gh_apoint3d *me)
    {
        me->get_unnamed(is, &me->x);
        me->get_unnamed(is, &me->y);
        me->get_unnamed(is, &me->z);
    }

    friend void operator<< (gh_stream_out& os, const gh_apoint3d *me)
    {
        me->put_unnamed(os, &me->x);
        me->put_unnamed(os, &me->y);
        me->put_unnamed(os, &me->z);
    }
};

typedef gh_apoint3d<int, gh_size3d> gh_point3d;
typedef gh_apoint3d<float, gh_fsize3d> gh_fpoint3d;
typedef gh_apoint3d<double, gh_dsize3d> gh_dpoint3d;

#endif /* _GH_POINT_H_ */
