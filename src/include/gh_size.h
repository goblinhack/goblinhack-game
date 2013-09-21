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

    $Id: gh_size.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_SIZE_H_
#define _GH_SIZE_H_

template<class T> class gh_asize : public gh_stream
{
public:
    T w;
    T h;

    gh_inline gh_asize (void) : w(0), h(0) { }

    gh_inline gh_asize (T w, T h) : w(w), h(h) { }

    gh_inline gh_asize (const gh_asize &a) : w(a.w), h(a.h) { }

    gh_inline void operator+= (gh_asize a)
    {
        w += a.w; h += a.h;
    }

    gh_inline void operator-= (gh_asize a)
    {
        w -= a.w; h -= a.h;
    }

    gh_inline void operator/= (gh_asize a)
    {
        w /= a.w; h /= a.h;
    }

    gh_inline void operator*= (gh_asize a)
    {
        w *= a.w; h *= a.h;
    }

    gh_inline friend gh_asize operator/ (gh_asize a, T i)
    {
        return (gh_asize(a.w / i, a.h / i));
    }

    gh_inline friend gh_asize operator* (gh_asize a, T i)
    {
        return (gh_asize(a.w * i, a.h * i));
    }

    gh_inline friend gh_asize operator+ (gh_asize a, gh_asize b)
    {
        return (gh_asize(a.w + b.w, a.h + b.h));
    }

    gh_inline friend gh_asize operator- (gh_asize a, gh_asize b)
    {
        return (gh_asize(a.w - b.w, a.h - b.h));
    }

    gh_inline friend gh_asize operator/ (gh_asize a, gh_asize b)
    {
        return (gh_asize(a.w / b.w, a.h / b.h));
    }

    gh_inline friend gh_asize operator* (gh_asize a, gh_asize b)
    {
        return (gh_asize(a.w * b.w, a.h * b.h));
    }

    gh_inline friend bool operator== (gh_asize a, gh_asize b)
    {
        return (a.w == b.w) && (a.h == b.h);
    }

    gh_inline friend bool operator!= (gh_asize a, gh_asize b)
    {
        return (!(a==b));
    }

    //
    // Compare areas of a and b
    //
    gh_inline friend bool operator< (gh_asize a, gh_asize b)
    {
        return (a.w * a.h) < (b.w * b.h);
    }

    //
    // Compare areas of a and b
    //
    gh_inline friend bool operator> (gh_asize a, gh_asize b)
    {
        return (a.w * a.h) > (b.w * b.h);
    }

    friend void operator>> (gh_stream_in& is, gh_asize *me)
    {
        me->get_unnamed(is, &me->w);
        me->get_unnamed(is, &me->h);
    }

    friend void operator<< (gh_stream_out& os, const gh_asize *me)
    {
        me->put_unnamed(os, &me->w);
        me->put_unnamed(os, &me->h);
    }
};

typedef gh_asize<int> gh_size;
typedef gh_asize<float> gh_fsize;
typedef gh_asize<double> gh_dsize;

template<class T, class U> class gh_asize3d : public gh_stream
{
public:

    T w;
    T h;
    T d;

    gh_inline gh_asize3d (void) : w(0), h(0), d(0) { }

    gh_inline gh_asize3d (T w, T h) : w(w), h(h), d(0) { }

    gh_inline gh_asize3d (T w, T h, T d) : w(w), h(h), d(d) { }

    //
    // Allow promotion of 2d sizes to 3d by setting d=0
    //
    gh_inline gh_asize3d (U a) : w(a.w), h(a.h), d(0) { }

    gh_inline gh_asize3d (const gh_asize3d &a) : w(a.w), h(a.h), d(a.d) { }

    gh_inline void operator+= (gh_asize3d a)
    {
        w += a.w; h += a.h; d += a.d;
    }

    gh_inline void operator-= (gh_asize3d a)
    {
        w -= a.w; h -= a.h; d -= a.d;
    }

    gh_inline friend gh_asize3d operator/ (gh_asize3d a, T i)
    {
        return (gh_asize3d(a.w / i, a.h / i, a.d / i));
    }

    gh_inline friend gh_asize3d operator* (gh_asize3d a, T i)
    {
        return (gh_asize3d(a.w * i, a.h * i, a.d * i));
    }

    gh_inline friend gh_asize3d operator+ (gh_asize3d a, gh_asize3d b)
    {
        return (gh_asize3d(a.w + b.w, a.h + b.h, a.d + b.d));
    }

    gh_inline friend gh_asize3d operator- (gh_asize3d a, gh_asize3d b)
    {
        return (gh_asize3d(a.w - b.w, a.h - b.h, a.d - b.d));
    }

    gh_inline friend gh_asize3d operator/ (gh_asize3d a, gh_asize3d b)
    {
        return (gh_asize3d(a.w / b.w, a.h / b.h, a.d / b.d));
    }

    gh_inline friend gh_asize3d operator* (gh_asize3d a, gh_asize3d b)
    {
        return (gh_asize3d(a.w * b.w, a.h * b.h, a.d * b.d));
    }

    gh_inline friend bool operator== (gh_asize3d a, gh_asize3d b)
    {
        return (a.w == b.w) && (a.h == b.h) && (a.d == b.d);
    }

    gh_inline friend bool operator!= (gh_asize3d a, gh_asize3d b)
    {
        return (!(a==b));
    }

    //
    // Compare areas of a and b
    //
    // If either depth is zero treat as a 2 dimensional space
    //
    gh_inline friend bool operator> (gh_asize3d a, gh_asize3d b)
    {
        if ((a.d == 0) && (b.d == 0))
            return (a.w * a.h) > (b.w * b.h);
        else
            return (a.w * a.h * a.d) > (b.w * b.h * b.d);
    }

    //
    // Compare areas of a and b
    //
    // If either depth is zero treat as a 2 dimensional space
    //
    gh_inline friend bool operator< (gh_asize3d a, gh_asize3d b)
    {
        if ((a.d == 0) && (b.d == 0))
            return (a.w * a.h) < (b.w * b.h);
        else
            return (a.w * a.h * a.d) < (b.w * b.h * b.d);
    }

    friend void operator>> (gh_stream_in& is, gh_asize3d *me)
    {
        me->get_unnamed(is, &me->w);
        me->get_unnamed(is, &me->h);
        me->get_unnamed(is, &me->d);
    }

    friend void operator<< (gh_stream_out& os, const gh_asize3d *me)
    {
        me->put_unnamed(os, &me->w);
        me->put_unnamed(os, &me->h);
        me->put_unnamed(os, &me->d);
    }
};

typedef gh_asize3d<int, gh_size> gh_size3d;
typedef gh_asize3d<float, gh_fsize> gh_fsize3d;
typedef gh_asize3d<double, gh_dsize> gh_dsize3d;

#endif /* _GH_SIZE_H_ */
