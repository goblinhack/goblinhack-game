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

    $Id: gh_templates.h 418 2007-01-27 14:54:29Z goblinhack $
*/

#ifndef _GH_TEMPLATE_H_
#define _GH_TEMPLATE_H_

//
// Used for simple classes which can't be extended - like ints
//
template <class T>

    string tostring (const T i)
    {
        ostringstream r;

        r << i;

        return (r.str());
    }

template <class T>

    string tostring (const T i, int size, char pad)
    {
        ostringstream r;

        r << setfill(pad) << setw(size) << i;

        return (r.str());
    }

extern int toint (const string s);

//
// Below here, stick any generically useful function templates
//

template <class C>

    void erase_and_delete (C &c)
    {
        typename C::iterator so = c.begin();
        typename C::iterator eo = c.end();
        typename C::iterator s = so;

        while (s != eo) {
            delete *s;
            s++;
        }
        c.erase(so, eo);
    }

template <class C>

    void erase (C &c)
    {
        typename C::iterator so = c.begin();
        typename C::iterator eo = c.end();

        c.erase(so, eo);
    }

template <class C>

    void null_out (C &c)
    {
        typename C::iterator so = c.begin();
        typename C::iterator eo = c.end();
        typename C::iterator s = so;

        while (s != eo) {
            *s = NULL;
            s++;
        }
    }

template <class C>

    void erase_all (C &c)
    {
        typename C::iterator so = c.begin();
        typename C::iterator eo = c.end();

        c.erase(so, eo);
    }

template <class C>

    //
    // Warning O(n^2)
    //
    void erase_dups (C &c)
    {
        for (;;) {
        redo:
            typename C::iterator i = c.begin();
            while (i != c.end()) {

                typename C::iterator j = c.begin();
                while (j != c.end()) {

                    if (i != j) {
                        if (*i == *j) {
                            c.erase(i);
                            goto redo;
                        }
                    }
                    j++;
                }
                i++;
            }
            break;
        }
    }

template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C>

    bool contains (C<T> *c, T t)
    {
        typename C<T>::iterator i = c->begin();

        while (i != c->end()) {

            if (*i == t) {
                return (true);
            }

            i++;
        }

        return (false);
    }

template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C>

    T one_of (C<T> &c)
    {
        typename C<T>::iterator so = c.begin();
        typename C<T>::iterator eo = c.end();

        size_t sz = eo - so;

        return *(so + (rand() % sz));
    }

template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C>

    C<T> reverse (C<T> &c)
    {
        typename C<T>::iterator so = c.end();
        typename C<T>::iterator eo = c.begin();
        C<T> rev;

        while (so != eo) {
            rev.push_back(*--so);
        }

        return rev;
    }

template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C>

    void push_back_if_unique (C<T> &c, T d)
    {
        typename C<T>::iterator so = c.begin();
        typename C<T>::iterator eo = c.end();

        while (so != eo) {
            T t = *so;

            if (t == d) {
                return;
            }

            so++;
        }

        c.push_back(d);
    }

#endif /* _GH_TEMPLATE_H_ */
