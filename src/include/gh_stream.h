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

    $Id: gh_stream.h 346 2006-12-30 23:46:20Z goblinhack $
*/

#ifndef _GH_STREAM_H_
#define _GH_STREAM_H_

#include <gh_templates.h>
#include <algorithm>
#include <typeinfo>

#define PARSER_EXTRA_SANITY(name) \
    if (is.node >= is.end_node) { \
        GH_THROW("Unexpected end, looking for " + string(name));\
    }

#ifdef GH_DO_PARSER_DEBUG
#define NODE_DEBUG(name, node) \
    PARSER_EXTRA_SANITY(name); \
    cout << *(node) << "\t\t" << __FUNCTION__ << endl;
#else
#define NODE_DEBUG(name, node) \
    PARSER_EXTRA_SANITY(name);
#endif

#define GH_INDENT_STR_SIZE 100

extern const char gh_indent_str[GH_INDENT_STR_SIZE];

#define gh_out(class) {\
    gh_stream_out out; \
    out << class; \
    out << ends; \
    cout << out.str() << endl; \
}

#define gh_err(class) {\
    gh_stream_out out; \
    out << class; \
    out << ends; \
    cerr << out.str() << endl; \
}

//////////////////////////////////////////////////////////////////////////////
//
// Routines to allow the dumping of classes to streams, files and then reading
// back in from a (zipped) file. e.g format:
//
//    a=1 c=24 b="hello" d={
//     a=12 b={
//      a="long string" b=[ {
//        a="string" b=[ 1 2 3 ] 
//       } {
//        a="long string" b=[ 4 5 6 ] 
//       } {
//        a="longer string" b=[ 7 8 9 ] 
//       } ] 
//     } 
//
// The aim is to be *fast* at the expense of robustness.
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// Node structure store class information after initial parsing.
//
//////////////////////////////////////////////////////////////////////////////

class gh_node {
public:
    typedef enum {
        GH_NODE_NONE,
        GH_NODE_INT,
        GH_NODE_FLOAT,
        GH_NODE_STRING,
        GH_NODE_NAME,
        GH_NODE_BRA,
        GH_NODE_KET,
        GH_NODE_EOF,
    } gh_node_type;

    gh_node_type type;
    int depth;

    union {
        float v_float; 
        long v_int; 
        char *v_string;
        char *v_name;
    } val;

    friend ostream& operator<< (ostream& os, const gh_node& obj)
    {
        os << (gh_indent_str + sizeof(gh_indent_str) - 1 - obj.depth);

        switch (obj.type) {
            case GH_NODE_INT:
                os << "int " << obj.val.v_int;
                break;
            case GH_NODE_FLOAT:
                os << "float " << obj.val.v_float;
                break;
            case GH_NODE_STRING:
                os << "string " << obj.val.v_string;
                break;
            case GH_NODE_NAME:
                os << "name \"" << obj.val.v_string << "\"";
                break;
            case GH_NODE_NONE:
                os << "error";
                break;
            case GH_NODE_BRA:
                os << "[";
                break;
            case GH_NODE_KET:
                os << "]";
                break;
            case GH_NODE_EOF:
                os << "EOF";
                break;
        }

        return (os);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// State machine class for reading a file, parsing it and dumping the results
// into a container of nodes.
//
//////////////////////////////////////////////////////////////////////////////

class gh_parse {
public:
    string file;
    static vector<gh_node> nodes;
    vector<gh_node>::iterator node;
    vector<gh_node>::iterator end_node;

    uint32 line;
    char *buf;
    char *end;
    size_t size;

    long tmp_int;
    int tmp_mul;
    float tmp_float;
    char *tmp_at;

    int depth;

    typedef enum {
        GH_PARSE_STATE_NONE,
        GH_PARSE_STATE_INT,
        GH_PARSE_STATE_STRING,
        GH_PARSE_STATE_NAME,
    } gh_parse_state;

    gh_parse_state state;

    gh_parse(const string infile);
    ~gh_parse(void);

    void push(const int depth, const float tmp_float);
    void push(const int depth, const long tmp_int);
    void push(const int depth, char *tmp_at);
    void alloc(uint32 size);
    void parse_error(string error, char *at);
    void remove_escape_characters(char *uncompressed);

    friend ostream& operator<< (ostream& os, const gh_parse& obj)
    {
        vector<gh_node>::const_iterator b = obj.nodes.begin();
        const vector<gh_node>::const_iterator e = obj.end_node;

        while (b != e) {
            cout << *b << endl;
            b++;
        }
        return (os);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// A structure which wraps gh_parse to provide a stream reading mechanism
// from a parsed file (which has been parsed into nodes)
//
// NOTE only limited sanity checks are done for speed
//
//////////////////////////////////////////////////////////////////////////////

class gh_stream_in : public gh_parse {
public:
    //
    // Parse a file into nodes.
    //
    gh_stream_in (string infile) : gh_parse(infile) { }

    //
    // Look at the next node without stepping forward.
    //
    gh_inline bool peek_next (gh_stream_in& is, const char *want)
    {
        if (node >= end_node) {
            return (false);
        }

        NODE_DEBUG("", node)
#ifdef GH_DO_PARSER_DEBUG
        cout << " want " << want << endl;
#endif

        //
        // Make sure we've got a string and not some junk.
        //
        switch (node->type) {
            case gh_node::GH_NODE_STRING:
            case gh_node::GH_NODE_NAME:
                break;

            //
            // Hit the end of a class or the start of a sub class?
            //
            case gh_node::GH_NODE_KET:
            case gh_node::GH_NODE_BRA:
                return (false);
                
            default:
                GH_THROW("Expecting a string when looking for " +
                         string(want) + " got " + tostring(node->type));

                return (false);
        }

        const char *name = node->val.v_name;

        if (name == NULL) {
            return (false);
        }

        return (!strcmp(name, want));
    }

    //
    // Look at the name of the next node. Assumes it is a string!
    //
    gh_inline void read (gh_stream_in& is, const char *want)
    {
        NODE_DEBUG(want, node)

        const char *name = (node++)->val.v_name;

        if (strcmp(name, want)) {
            GH_THROW("Looking for \"" + string(want) +
                     "\", found " + string(name));
        }
    }

    gh_inline void read_simple (gh_stream_in& is, int *t)
    {
        NODE_DEBUG("", node)

        *t = (node++)->val.v_int;
    }

    gh_inline void read_simple (gh_stream_in& is, uint8 *t)
    {
        NODE_DEBUG("", node)

        *t = (uint8) (node++)->val.v_int;
    }

    gh_inline void read_simple (gh_stream_in& is, uint16 *t)
    {
        NODE_DEBUG("", node)

        *t = (uint16) (node++)->val.v_int;
    }

    gh_inline void read_simple (gh_stream_in& is, uint32 *t)
    {
        NODE_DEBUG("", node)

        *t = (uint32) (node++)->val.v_int;
    }

    gh_inline void read_simple (gh_stream_in& is, int16 *t)
    {
        NODE_DEBUG("", node)

        *t = (node++)->val.v_int;
    }

    gh_inline void read_simple (gh_stream_in& is, bool *t)
    {
        NODE_DEBUG("", node)

        *t = (node++)->val.v_int;
    }

    gh_inline void read_simple (gh_stream_in& is, void **t)
    {
        NODE_DEBUG("", node)

        *t = (void*)(node++)->val.v_int;
    }

    gh_inline void read_simple (gh_stream_in& is, float *t)
    {
        NODE_DEBUG("", node)

        if (node->type == gh_node::GH_NODE_INT) {
            *t = (float)((node++)->val.v_int);
        } else {
            *t = (node++)->val.v_float;
        }
    }

    gh_inline void read_simple (gh_stream_in& is, double *t)
    {
        NODE_DEBUG("", node)

        if (node->type == gh_node::GH_NODE_INT) {
            *t = (float)((node++)->val.v_int);
        } else {
            *t = (node++)->val.v_float;
        }
    }

    gh_inline void read_simple (gh_stream_in& is, string *t)
    {
        NODE_DEBUG("", node)

        *t = (node++)->val.v_string;
    }

    gh_inline void read_simple (gh_stream_in& is, char **t)
    {
        NODE_DEBUG("", node)

        *t = (node++)->val.v_string;
    }

    template <class T> 
        gh_inline void read_class (gh_stream_in& is, T t)
    {
        NODE_DEBUG("", node)

        if ((node++)->type != gh_node::GH_NODE_BRA) {
            GH_THROW("Missing start [ of class");
        }

        is >> t;

        if ((node++)->type != gh_node::GH_NODE_KET) {
            GH_THROW("Missing end ] of class");
        }
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> 
        gh_inline void read_container (gh_stream_in& is, C<T> *v)
    {
        NODE_DEBUG("", node)

        const int depth = node->depth;

        if ((node++)->type != gh_node::GH_NODE_BRA) {
            GH_THROW("Missing container start");
        }

        //
        // Empty container?
        //
        if (node->type == gh_node::GH_NODE_KET) {
            NODE_DEBUG("", node)
            node++;
            return;
        }

        while ((node->depth != depth) && (node < end_node)) {
            T t;
            is.read_simple(is, &t);
            v->push_back(t);
        }

        if ((node++)->type != gh_node::GH_NODE_KET) {
            GH_THROW("Missing container end");
        }
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> 
        gh_inline void read_class_container (gh_stream_in& is, C<T> *v)
    {
        if ((node++)->type != gh_node::GH_NODE_BRA) {
            GH_THROW("Missing container start");
        }

        while (node->type == gh_node::GH_NODE_BRA) {
            T t;
            is.read_class(is, &t);
            v->push_back(t);
        }

        if ((node++)->type != gh_node::GH_NODE_KET) {
            GH_THROW("Missing container end");
        }
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> 
        gh_inline void read_pcontainer (gh_stream_in& is, C<T*> *v)
    {
        NODE_DEBUG("", node)

        const int depth = node->depth;

        if ((node++)->type != gh_node::GH_NODE_BRA) {
            GH_THROW("Missing container start");
        }

        //
        // Empty container?
        //
        while ((node->depth != depth) && (node < end_node)) {
            T* t = new T;
            is.read_simple(is, t);
            v->push_back(t);
        }

        if ((node++)->type != gh_node::GH_NODE_KET) {
            GH_THROW("Missing container end");
        }
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> 
        gh_inline void read_class_pcontainer (gh_stream_in& is, C<T*> *v)
    {
        NODE_DEBUG("", node)

        if ((node++)->type != gh_node::GH_NODE_BRA) {
            GH_THROW("Missing container start");
        }

        while (node->type == gh_node::GH_NODE_BRA) {
            T* t = new T;

            is.read_class(is, t);
            v->push_back(t);
        }

        if ((node++)->type != gh_node::GH_NODE_KET) {
            GH_THROW("Missing container end");
        }
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> 
        gh_inline void read_classpp_container (gh_stream_in& is, C<T*> *v)
    {
        NODE_DEBUG("", node)

        if ((node++)->type != gh_node::GH_NODE_BRA) {
            GH_THROW("Missing container start");
        }

        while (node->type != gh_node::GH_NODE_KET) {
            NODE_DEBUG("", node)

            T* x __attribute__((unused));
            v->push_back((decltype(x)) (is.node++)->val.v_int);
        }

        node++;
    }

    template <class T>
        gh_inline void read_classp (gh_stream_in& is, T **v)
    {
        *v = new T;
        is.read_class(is, *v);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// A structure which wraps strstream to provide an extra layer which allows 
// to keep track of the indent level associated with the stream.
//
//////////////////////////////////////////////////////////////////////////////

class gh_stream_out : public ostringstream {
    int depth;

public:
    //
    // dynamic buffer mode
    //
    gh_stream_out (void) : ostringstream(), depth(0) { }

    //
    // static buffer mode
    //
    gh_stream_out (char *buf) : ostringstream(buf), depth(0) { }

    gh_inline void indent (int i)
    {
        depth += i;
    }

    gh_inline const char* indent_str (void)
    {
        return (gh_indent_str + sizeof(gh_indent_str) - 1 - depth);
    }
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class gh_stream {
public:
    ///////////////////////////////////////////////////////////////////////////
    // Put routines
    ///////////////////////////////////////////////////////////////////////////

    gh_inline
    void put_name (gh_stream_out& os, const char *name) const
    {
        if (name && *name) {
            os << name << "=";
        }
    }

    //
    // Put named types
    //
    gh_inline
    void put (gh_stream_out& os, const uint8 *t, const char *name) const
    {
        put_name(os, name);
        os << (unsigned int)*t << " ";
    }

    gh_inline
    void put (gh_stream_out& os, const uint16 *t, const char *name) const
    {
        put_name(os, name);
        os << (unsigned int)*t << " ";
    }

    gh_inline
    void put (gh_stream_out& os, const uint32 *t, const char *name) const
    {
        put_name(os, name);
        os << (unsigned int)*t << " ";
    }

    gh_inline
    void put (gh_stream_out& os, const int *t, const char *name) const
    {
        put_name(os, name);
        os << *t << " ";
    }

    gh_inline
    void put (gh_stream_out& os, const int16 *t, const char *name) const
    {
        put_name(os, name);
        os << *t << " ";
    }

    gh_inline
    void put (gh_stream_out& os, const bool *t, const char *name) const
    {
        put_name(os, name);
        os << *t << " ";
    }

    gh_inline
    void put (gh_stream_out& os, void *const*t, const char *name) const
    {
        put_name(os, name);
        os << (long)*t << " ";
    }

    gh_inline
    void put (gh_stream_out& os, const char *t, const char *name) const
    {
        put_name(os, name);

        os << "\"" << t << "\" ";
    }

    gh_inline
    void put (gh_stream_out& os, char *const*t, const char *name) const
    {
        put_name(os, name);

        os << "\"" << *t << "\" ";
    }

    gh_inline
    void put (gh_stream_out& os, const string *t, const char *name) const
    {
        put_name(os, name);

        os << "\"" << *t << "\" ";
    }

    gh_inline
    void put (gh_stream_out& os, const float *t, const char *name) const
    {
        put_name(os, name);

        if (isnan(*t) || isinf(*t)) {
            os << "0.0 ";
        } else {
            os << *t << " ";
        }
    }

    gh_inline
    void put (gh_stream_out& os, const double *t, const char *name) const
    {
        put_name(os, name);

        if (isnan(*t) || isinf(*t)) {
            os << "0.0 ";
        } else {
            os << *t << " ";
        }
    }

    template <class T>
    gh_inline
    void put_opt (gh_stream_out& os, const T *t, const char *name, const T def) const
    {
        if (*t != def) {
            put(os, t, name);
        }
    }

    template <class T>
    gh_inline
    void put_opt_named_flag (gh_stream_out& os, const T *t, const char *name, const T def) const
    {
        if (*t != def) {
            os << name << " ";
        }
    }

    template <class T>
    gh_inline
    void put_named_flag (gh_stream_out& os, const T *t, const char *name) const
    {
        if (*t) {
            os << name << " ";
        }
    }

    //
    // Put unnamed types
    //
    gh_inline
    void put_unnamed (gh_stream_out& os, const unsigned int *t) const
    {
        os << *t << " ";
    }

    gh_inline
    void put_unnamed (gh_stream_out& os, 
        vector<uint8>::const_iterator t) const
    {
        os << (int)*t << " ";
    }

    gh_inline
    void put_unnamed (gh_stream_out& os, 
        vector<int>::const_iterator t) const
    {
        os << *t << " ";
    }

    gh_inline
    void put_unnamed (gh_stream_out& os, 
        vector<string>::const_iterator t) const
    {
        os << "\"" << *t << "\" ";
    }

    gh_inline
    void put_unnamed (gh_stream_out& os, const int *t) const
    {
        os << *t << " ";
    }

    gh_inline
    void put_unnamed (gh_stream_out& os, const int16 *t) const
    {
        os << *t << " ";
    }

    gh_inline
    void put_unnamed (gh_stream_out& os, const bool *t) const
    {
        os << *t << " ";
    }

    gh_inline
    void put_unnamed (gh_stream_out& os, const char *t) const
    {
        os << "\"" << t << "\" ";
    }

    gh_inline
    void put_unnamed (gh_stream_out& os, const float *t) const
    {
        os << *t << " ";
    }

    gh_inline
    void put_unnamed (gh_stream_out& os, const double *t) const
    {
        os <<  *t << " ";
    }

    gh_inline
    void put_unnamed (gh_stream_out& os, const string *t) const
    {
        os << "\"" << *t << "\" ";
    }

    template <class T> 
        void put_class (gh_stream_out& os, const T *t, const char *name) const
    {
        os << endl;
        os << os.indent_str();
        put_name(os, name);
        os << "[" << endl;
        os.indent(1);
        os << os.indent_str();

        os << t;
            
        os.indent(-1);
        os << endl;
        os << os.indent_str();
        os << "] ";
    }

    template <class T> 
        void put_classpp (gh_stream_out& os, const T *t, const char *name) const
    {
        put_name(os, name);

        os << (long)*t << " ";
    }

    template <class T>
    gh_inline
    void put_opt_class (gh_stream_out& os, const T *t, const char *name, const T def) const
    {
        if (*t != def) {
            put_class(os, t, name);
        }
    }

    template <class T>
    gh_inline
    void put_opt_classpp (gh_stream_out& os, const T *t, const char *name) const
    {
        if (t && *t) {
            put_classpp(os, t, name);
        }
    }

    template <class T> 
        void put_class_unnamed (gh_stream_out& os, const T *t) const
    {
        os << endl;
        os << os.indent_str();
        os << "[" << endl;
        os.indent(1);
        os << os.indent_str();

        os << t;
            
        os.indent(-1);
        os << endl;
        os << os.indent_str();
        os << "] ";
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C>
        void put_class_container (gh_stream_out& os, const C<T> *v,
                                  const char *name) const
    {
        typename vector<T>::const_iterator c;

        if (!v->size()) {
            return;
        }

        os << endl;
        os << os.indent_str();
        put_name(os, name);
        os << "[";
        os.indent(1);

        for (c = v->begin(); c != v->end(); c++) {
            const T *d = &(*c);
            put_class_unnamed(os, d);
        }

        os.indent(-1);
        os << endl;
        os << os.indent_str();
        os << "] ";
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C>
        void put_container (gh_stream_out& os, const C<T> *v,
                            const char *name) const
    {
        typename vector<T>::const_iterator c;

        if (!v->size()) {
            return;
        }

        os << endl;
        os << os.indent_str();
        put_name(os, name);
        os << "[" << endl;
        os.indent(1);
        os << os.indent_str();

        for (c = v->begin(); c != v->end(); c++) {
            put_unnamed(os, c);
        }

        os.indent(-1);
        os << endl;
        os << os.indent_str();
        os << "] ";
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C>
        void put_class_pcontainer (gh_stream_out& os, const C<T*> *v,
                                   const char *name) const
    {
        typename C<T*>::const_iterator c;

        if (!v->size()) {
            return;
        }

        os << endl;
        os << os.indent_str();
        put_name(os, name);
        os << "[";
        os.indent(1);

        for (c = v->begin(); c != v->end(); c++) {
            put_class_unnamed(os, *c);
        }

        os.indent(-1);
        os << endl;
        os << os.indent_str();
        os << "] ";
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C>
        void put_classpp_container (gh_stream_out& os, const C<T*> *v,
                                    const char *name) const
    {
        typename C<T*>::const_iterator c;

        if (!v->size()) {
            return;
        }

        os << endl;
        os << os.indent_str();
        put_name(os, name);
        os << "[" << endl;
        os.indent(1);
        os << os.indent_str();

        for (c = v->begin(); c != v->end(); c++) {
            os << (long)*c << " ";
        }

        os.indent(-1);
        os << endl;
        os << os.indent_str();
        os << "] ";
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C>
        void put_pcontainer (gh_stream_out& os, const C<T*> &v, const char *) const
    {
        typename C<T*>::const_iterator c;

        if (!v->size()) {
            return;
        }

        os << endl;
        os << os.indent_str();
        os << "[" << endl;
        os.indent(1);
        os << os.indent_str();

        for (c = v->begin(); c != v->end(); c++) {
            put_unnamed(os, *c);
        }

        os.indent(-1);
        os << endl;
        os << os.indent_str();
        os << "] ";
    }

    ///////////////////////////////////////////////////////////////////////////
    // Get routines
    ///////////////////////////////////////////////////////////////////////////

    template <class T> gh_inline
        void get_unnamed (gh_stream_in& is, T t)
    {
        NODE_DEBUG("", is.node)
        is.read_simple(is, t);
    }

    template <class T> gh_inline
        void get (gh_stream_in& is, T t, const char* name)
    {
        NODE_DEBUG(name, is.node)
        is.read(is, name);
        NODE_DEBUG(name, is.node)
        is.read_simple(is, t);
    }

    template <class T> gh_inline
        void get_class (gh_stream_in& is, T t, const char* name)
    {
        NODE_DEBUG(name, is.node)
        is.read(is, name);
        NODE_DEBUG(name, is.node)
        is.read_class(is, t);
    }

    template <class T> gh_inline
        void get_classp (gh_stream_in& is, T **t, const char* name)
    {
        NODE_DEBUG(name, is.node)
        is.read(is, name);
        NODE_DEBUG(name, is.node)
        is.read_classp(is, t);
    }

    template <class T> gh_inline
        void get_classpp (gh_stream_in& is, T **t, const char* name)
    {
        NODE_DEBUG(name, is.node)
        is.read(is, name);
        NODE_DEBUG(name, is.node)

        T x __attribute__((unused));
        *t = (decltype(&x)) (is.node++)->val.v_int;
    }

    template <class T> gh_inline
        bool get_opt_classp (gh_stream_in& is, T** t, const char* name)
    {
        if (is.peek_next(is, name)) {
            NODE_DEBUG(name, is.node)
            get_classp(is, t, name);
            return (true);
        }
        return (false);
    }

    template <class T> gh_inline
        bool get_opt_classpp (gh_stream_in& is, T** t, const char* name)
    {
        if (is.peek_next(is, name)) {
            NODE_DEBUG(name, is.node)
            get_classpp(is, t, name);
            return (true);
        }
        return (false);
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> gh_inline
        void get_container (gh_stream_in& is, C<T> *t, const char* name)
    {
        NODE_DEBUG(name, is.node)
        is.read(is, name);
        NODE_DEBUG(name, is.node)
        is.read_container(is, t);
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> gh_inline
        void get_class_container (gh_stream_in& is, C<T> *t, const char* name)
    {
        NODE_DEBUG(name, is.node)
        is.read(is, name);
        NODE_DEBUG(name, is.node)
        is.read_class_container(is, t);
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> gh_inline
        void get_pcontainer (gh_stream_in& is, C<T*> *t, const char* name)
    {
        NODE_DEBUG(name, is.node)
        is.read(is, name);
        NODE_DEBUG(name, is.node)
        is.read_pcontainer(is, t);
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> gh_inline
        void get_class_pcontainer (gh_stream_in& is, C<T*> *t, const char* name)
    {
        NODE_DEBUG(name, is.node)
        is.read(is, name);
        NODE_DEBUG(name, is.node)
        is.read_class_pcontainer(is, t);
    }

    template <class T> gh_inline
        bool get_opt (gh_stream_in& is, T *t, const char* name, const T def)
    {
        if (is.peek_next(is, name)) {
            NODE_DEBUG(name, is.node)
            is.node++;
            is.read_simple(is, t);
            return (true);
        } else {
            *t = def;
            return (false);
        }
    }

    template <class T> gh_inline
        bool get_opt (gh_stream_in& is, T *t, const char* name)
    {
        if (is.peek_next(is, name)) {
            NODE_DEBUG(name, is.node)
            is.node++;
            is.read_simple(is, t);
            return (true);
        } else {
            return (false);
        }
    }

    template <class T> gh_inline
        bool get_opt_class (gh_stream_in& is, T* t, const char* name, const T def)
    {
        if (is.peek_next(is, name)) {
            NODE_DEBUG(name, is.node)
            get_class(is, t, name);
            return (true);
        } else {
            *t = def;
            return (false);
        }
    }

    template <class T> gh_inline
        bool get_opt_class (gh_stream_in& is, T* t, const char* name)
    {
        if (is.peek_next(is, name)) {
            NODE_DEBUG(name, is.node)
            get_class(is, t, name);
            return (true);
        } else {
            return (false);
        }
    }

    template <class T> gh_inline
        bool get_opt_named_flag (gh_stream_in& is, T *t, const char* name, const T def)
    {
        if (is.peek_next(is, name)) {
            NODE_DEBUG(name, is.node)
            is.node++;
            *t = true;
            return (true);
        } else {
            *t = def;
            return (false);
        }
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> gh_inline
        bool get_opt_class_container (gh_stream_in& is, C<T> *t, const char* name)
    {
        if (is.peek_next(is, name)) {
            NODE_DEBUG(name, is.node)
            is.read(is, name);
            NODE_DEBUG(name, is.node)
            is.read_class_container(is, t);
            return (true);
        } else {
            return (false);
        }
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> gh_inline
        bool get_opt_container (gh_stream_in& is, C<T> *t, const char* name)
    {
        if (is.peek_next(is, name)) {
            NODE_DEBUG(name, is.node)
            is.read(is, name);
            NODE_DEBUG(name, is.node)
            is.read_container(is, t);
            return (true);
        } else {
            return (false);
        }
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> gh_inline
        bool get_opt_class_pcontainer (gh_stream_in& is, C<T*> *t, const char* name)
    {
        if (is.peek_next(is, name)) {
            NODE_DEBUG(name, is.node)
            is.read(is, name);
            NODE_DEBUG(name, is.node)
            is.read_class_pcontainer(is, t);
            return (true);
        } else {
            return (false);
        }
    }

    template <class T, template <typename ELEM, typename ALLOC = std::allocator<ELEM> > class C> gh_inline
        bool get_opt_classpp_container (gh_stream_in& is, C<T*> *t, const char* name)
    {
        if (is.peek_next(is, name)) {
            NODE_DEBUG(name, is.node)
            is.read(is, name);
            NODE_DEBUG(name, is.node)
            is.read_classpp_container(is, t);
            return (true);
        } else {
            return (false);
        }
    }
};

#endif
