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

    $Id: gh_io.h 612 2007-12-21 22:13:22Z goblinhack $
*/

#ifndef _GH_IO_H_
#define _GH_IO_H_

#include <gh_global.h>

#include <gh_stream.h>
#include "../io/zip.h"

template <class T> void gh_write_file (const T *obj,
                                       const string dir,
                                       const string file,
                                       const string file_backup = "")
{
    GH_FUNCTION();

    string found = file;

    if (gh_global::file_exists(file)) {
        /*
         * No need to look.
         */
    } else {
        /*
         * Search for it.
         */
        string founddir = gh_global::locatedir(dir);

        if (founddir == "") {
            GH_THROW("Can't find directory " + dir + " to write " +
                     file + " into");
        }

        found = founddir + file;
    }

    GH_LOG("Writing " + found);

    gh_stream_out *out = new gh_stream_out();

    (*out) << obj;

    if (strstr(file.c_str(), ".gz")) {
        if (gh_zip::instance()->gzwriteall(found,
                       (char*)out->str().c_str(),
                       out->str().size(),
                       (size_t)1) != (int)out->str().size()) {

            GH_WARN("Write error for " + found);
        }
    } else {
        if (gh_zip::instance()->gzwriteall(found,
                       (char*)out->str().c_str(),
                       out->str().size(),
                       (size_t)0) != (int)out->str().size()) {

            GH_WARN("Write error for " + found);
        }
    }

    if (file_backup != "") {
        if (gh_global::file_exists(file_backup)) {
            /*
             * No need to look.
             */
        } else {
            /*
             * Search for it.
             */
            string founddir = gh_global::locatedir(dir);

            if (founddir == "") {
                GH_THROW("Can't find directory " + dir + " to write " +
                         file_backup + " into");
            }

            found = founddir + file_backup;
        }

        GH_LOG("Writing " + found);
        if (strstr(file_backup.c_str(), ".gz")) {
            if (gh_zip::instance()->gzwriteall(found,
                           (char*)out->str().c_str(),
                           out->str().size(),
                           (size_t)9) != (int)out->str().size()) {

                GH_WARN("Write error for " + found);
            }
        } else {
            if (gh_zip::instance()->gzwriteall(found,
                           (char*)out->str().c_str(),
                           out->str().size(),
                           (size_t)0) != (int)out->str().size()) {

                GH_WARN("Write error for " + found);
            }
        }
    }

//  (*out) << ends;

//  cout << out->str() << endl; // copy the buffer to stdout

    delete out;
}

template <class T> bool gh_read_file (T *obj, const string &name,
                                      const bool noisy = true,
                                      const bool err = true)
{
    GH_FUNCTION();

    if (obj == NULL) {
        GH_THROW("no obj to load file " + name);
    }

    if (!noisy && !err) {
        if (!gh_global::exists(name)) {
            return (false);
        }
    }

    string found = gh_global::locate(name);

    if (found == "") {
        if (err) {
            GH_THROW("Couldn't find file " + name);
        } else {
            return (false);
        }
    }

    if (noisy) {
        if (gh_global::debug) {
            GH_LOG("Loading \"" + found + "\"");
        }
    }

    try {
        gh_stream_in in(found);

        in >> obj;
    } catch (...) {
        if (err) {
            GH_THROW("Couldn't read file " + name);
            return (false);
        }
    }
    return (true);
}
#endif
