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

    $Id: dir.cpp 1243 2009-07-12 14:44:00Z Ghack $
*/

#define MAIN
#include <ghack.h>
#include <gh_global.h>

#ifdef __MINGW32__
#include <io.h>
#include <tchar.h>
#include <wchar.h>
#endif
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

//
// Does the requested file exist?
//
const bool gh_global::file_exists (string filename)
{
    GH_FUNCTION()

    struct stat buf;

    if (stat(filename.c_str(), &buf) >= 0) {
        if (S_ISDIR(buf. st_mode)) {
            return (false);
        }

        return (true);
    }
    return (false);
}

//
// How large is the file?
//
const int gh_global::file_size (string filename)
{
    GH_FUNCTION()

    struct stat buf;

    if (stat(filename.c_str(), &buf) >= 0) {
        return (buf.st_size);
    }

    return (-1);
}

//
// Remove the file if it exists.
//
const bool gh_global::file_unlink (string filename)
{
    GH_FUNCTION()

    if (!file_exists(filename)) {
        return (false);
    }

    unlink(filename.c_str());

    return (true);
}

//
// Locate the file in the local dir or system paths
//
const string gh_global::locate (string filename,
                                bool quiet)
{
    GH_FUNCTION()

    list<string>::iterator p;

    if (filename == "") {
        GH_WARN("null filename?");
        return ("");
    }

    //
    // First look in the local dir
    //
    string filename_and_path = filename;

    struct stat buf;

    //
    // Get rid of any trailing chars that confuse stat on mingwin
    //
    uint32 size = filename.size();

    if (size && filename[size-1] == '/') {
        filename.resize(size-1);
    }

    //
    // Does the sucker exist?
    //
    if (stat(filename_and_path.c_str(), &buf) >= 0) {
        string_replace(filename_and_path, "//", "/");
        return (filename_and_path);
    }

    //
    // Now look in the system path
    //
    for (p = gh_global::systemPath.begin();
        p != gh_global::systemPath.end(); p++) {
        string filename_and_path = *p + "/" + filename;

        struct stat buf;

        //
        // Does the sucker exist?
        //
        if (stat(filename_and_path.c_str(), &buf) >= 0) {
            if (!S_ISDIR(buf.st_mode)) {
                string_replace(filename_and_path, "//", "/");
                return (filename_and_path);
            }
        }

        filename_and_path += ".gz";

        //
        // Does the sucker exist zipped?
        //
        if (stat(filename_and_path.c_str(), &buf) >= 0) {
            if (!S_ISDIR(buf.st_mode)) {
                string_replace(filename_and_path, "//", "/");
                return (filename_and_path);
            }
        }

        if (strstr(filename.c_str(), ".gz")) {
            string without_gz = filename;

            without_gz.resize(without_gz.size()-3);

            filename_and_path = *p + "/" + without_gz;

            //
            // Does the sucker exist zipped?
            //
            if (stat(filename_and_path.c_str(), &buf) >= 0) {
                if (!S_ISDIR(buf.st_mode)) {
                    string_replace(filename_and_path, "//", "/");
                    return (filename_and_path);
                }
            }
        }
    }

    if (quiet) {
        return ("");
    }

    string err = string("Can't find " + filename + ", tried:");

    GH_LOG(err);

    for (p = gh_global::systemPath.begin();
        p != gh_global::systemPath.end(); p++) {
        string filename_and_path = *p + "/" + filename;

        filename_and_path += ".gz";

        GH_LOG("  " + filename_and_path);
    }

    return ("");
}

//
// Locate the file in the local dir or system paths
//
bool gh_global::exists (string filename)
{
    GH_FUNCTION()

    list<string>::iterator p;

    if (filename == "") {
        return (false);
    }

    //
    // First look in the local dir
    //
    string filename_and_path = filename;

    struct stat buf;

    //
    // Get rid of any trailing chars that confuse stat on mingwin
    //
    uint32 size = filename.size();

    if (size && filename[size-1] == '/') {
        filename.resize(size-1);
    }

    //
    // Does the sucker exist?
    //
    if (stat(filename_and_path.c_str(), &buf) >= 0) {
        return (true);
    }

    //
    // Now look in the system path
    //
    for (p = gh_global::systemPath.begin();
        p != gh_global::systemPath.end(); p++) {
        string filename_and_path = *p + "/" + filename;

        struct stat buf;

        //
        // Does the sucker exist?
        //
        if (stat(filename_and_path.c_str(), &buf) >= 0) {
            return (true);
        }

        filename_and_path += ".gz";

        //
        // Does the sucker exist zipped?
        //
        if (stat(filename_and_path.c_str(), &buf) >= 0) {
            return (true);
        }

        if (strstr(filename.c_str(), ".gz")) {
            string without_gz = filename;

            without_gz.resize(without_gz.size()-3);

            filename_and_path = *p + "/" + without_gz;

            //
            // Does the sucker exist zipped?
            //
            if (stat(filename_and_path.c_str(), &buf) >= 0) {
                return (true);
            }
        }
    }

    return (false);
}

//
// Locate the dir in the local dir or system paths
//
const string gh_global::locatedir (string subdir)
{
    GH_FUNCTION()

    list<string>::iterator p;
    struct stat buf;

    //
    // Get rid of any trailing chars that confuse stat on mingwin
    //
    uint32 size = subdir.size();

    if (size && subdir[size-1] == '/') {
        subdir.resize(size-1);
    }

    for (p = gh_global::systemPath.begin();
        p != gh_global::systemPath.end(); p++) {

        string d;

        if (subdir == "") {
            d = *p;
        } else {
            d = *p + "/" + subdir;
        }

        //
        // Does the sucker exist?
        //
        if (stat(d.c_str(), &buf) >= 0) {
            if (S_ISDIR(buf. st_mode)) {
                return (d + "/");
            }
        }
    }

    string err("can't find directory " + subdir + ", tried:");

    GH_WARN(err);

    for (p = gh_global::systemPath.begin();
        p != gh_global::systemPath.end(); p++) {
        string d;

        if (subdir == "") {
            d = *p;
        } else {
            d = *p + "/" + subdir;
        }

        GH_LOG(d);
    }

    return ("");
}

//
// Recursively scan the given directory, for the file type wanted, and return
// a sorted list of files.
//
vector<string> gh_global::listdir_recursive (const string dir,
                                             const string suffix,
                                             const string exclude_suffix)
{
    GH_FUNCTION()

    vector<string> files;

    struct dirent * e;
    DIR * d;

    d = opendir(dir.c_str());

    if (d == NULL) {
        GH_WARN("no directory named \"" + dir + "\"");
        return (files);
    }

    files.clear();

    while ((e = readdir(d))) {
        struct stat s;

        if (*e->d_name == '.') {
            continue;
        }

        if (!strcmp(e->d_name, "dummy_level")) {
            continue;
        }

        if (!strcmp(e->d_name, "CVS")) {
            continue;
        }

        if (strstr(e->d_name, "svn")) {
            continue;
        }

        if (strstr(e->d_name, "Makefile")) {
            continue;
        }

        if (strstr(e->d_name, ".txt")) {
            continue;
        }

        if (strstr(e->d_name, ".cpp")) {
            continue;
        }

        if (strstr(e->d_name, ".tsp")) {
            continue;
        }

        if (strstr(e->d_name, ".h")) {
            continue;
        }

        if (strstr(e->d_name, ".o")) {
            continue;
        }

        string entry = dir + "/" + string(e->d_name);
        
        if (stat(entry.c_str(), &s) < 0) {
            continue;
        }

        if (S_ISDIR(s.st_mode)) {
            vector<string> subfiles = listdir_recursive(entry,
                                                        suffix,
                                                        exclude_suffix);

            copy(subfiles.begin(), subfiles.end(),
                back_insert_iterator<vector <string> > (files));

            continue;
        }

        if (suffix.size()) {
            if (!strstr(e->d_name, suffix.c_str())) {
                continue;
            }
        }

        if (exclude_suffix.size()) {
            if (strstr(e->d_name, exclude_suffix.c_str())) {
                continue;
            }
        }

        if (strstr(e->d_name, "~")) {
            continue;
        }

        string_replace(entry, "//", "/");

        files.push_back(entry);
    }

    closedir(d);

    sort(files.begin(), files.end());

    return (files);
}

//
// A one level file scan.
//
vector<string> gh_global::list_files_only (const string dir,
                                           const string suffix,
                                           const string exclude_suffix)
{
    GH_FUNCTION()

    vector<string> files;

    struct dirent * e;
    DIR * d;

    d = opendir(dir.c_str());

    if (d == NULL) {
        GH_WARN("no directory named \"" + dir + "\"");
        return (files);
    }

    files.clear();

    while ((e = readdir(d))) {
        struct stat s;

        if (*e->d_name == '.') {
            continue;
        }

        if (!strcmp(e->d_name, "dummy_level")) {
            continue;
        }

        if (!strcmp(e->d_name, "CVS")) {
            continue;
        }

        if (strstr(e->d_name, "svn")) {
            continue;
        }

        if (strstr(e->d_name, "Makefile")) {
            continue;
        }

        if (strstr(e->d_name, ".txt")) {
            continue;
        }

        if (strstr(e->d_name, ".cpp")) {
            continue;
        }

        if (strstr(e->d_name, ".tsp")) {
            continue;
        }

        if (strstr(e->d_name, ".h")) {
            continue;
        }

        if (strstr(e->d_name, ".o")) {
            continue;
        }

        string entry = dir + "/" + string(e->d_name);
        
        if (stat(entry.c_str(), &s) < 0) {
            continue;
        }

        if (S_ISDIR(s.st_mode)) {
            continue;
        }

        if (suffix.size()) {
            if (!strstr(e->d_name, suffix.c_str())) {
                continue;
            }
        }

        if (exclude_suffix.size()) {
            if (strstr(e->d_name, exclude_suffix.c_str())) {
                continue;
            }
        }

        if (strstr(e->d_name, "~")) {
            continue;
        }

        string_replace(entry, "//", "/");

        string_rremove_to_start(entry, '/');

        files.push_back(entry);
    }

    closedir(d);

    sort(files.begin(), files.end());

    return (files);
}

//
// List a directory, returning a sorted list of directories.
//
vector<string> gh_global::list_dirs_only (const string dir)
{
    GH_FUNCTION()

    vector<string> dirs;

    struct dirent * e;
    DIR * d;

    dirs.clear();

    d = opendir(dir.c_str());

    if (d == NULL) {
        GH_WARN("no directory named \"" + dir + "\"");
        return (dirs);
    }

    while ((e = readdir(d))) {
        struct stat s;

        if (*e->d_name == '.') {
            continue;
        }

        if (!strcmp(e->d_name, "dummy_level")) {
            continue;
        }

        if (!strcmp(e->d_name, "CVS")) {
            continue;
        }

        string entry = dir + "/" + string(e->d_name);
        
        if (stat(entry.c_str(), &s) < 0) {
            continue;
        }

        if (!S_ISDIR(s.st_mode)) {
            continue;
        }

        entry = string(e->d_name) + "/";

        string_replace(entry, "//", "/");

        dirs.push_back(entry);
    }

    closedir(d);

    sort(dirs.begin(), dirs.end());

    return (dirs);
}

//
// list all directories and files, sorted
//
vector<string> gh_global::listdir (const string dir)
{
    vector <string> dirs = list_dirs_only(dir);
    vector <string> files = list_files_only(dir);

    copy(files.begin(), files.end(),
         back_insert_iterator<vector <string> > (dirs));

    return (dirs);
}
