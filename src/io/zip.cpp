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

    $Id: zip.cpp 829 2008-04-27 14:09:45Z goblinhack $
*/

#include "ghack.h"
#include "gh_zip.h"
#include "zip.h"

gh_zip::gh_zip() {

}

gh_zip::~gh_zip() {

}

void gh_zip::dos2unix (char * data, size_t *size)
{
    char *c = data;
    char *d = data;
    size_t sz = *size;

    while (c < data + sz) {
        if (*c == '\r') {
            c++;
            (*size)--;
            continue;
        }

        *d = *c;

        d++;
        c++;
    }
}

int gh_zip::gzwriteall (string name, char *buf, size_t size, size_t compression)
{
    int wrote;

    if (compression) {
        char mode[10];
        snprintf(mode, sizeof(mode), "wb%d", (int)compression);

        gzFile stream;
        stream = gzopen(name.c_str(), mode);

        if (stream == NULL) {
            return (-1);
        }

        wrote = gzwrite(stream, buf, size);
        gzclose(stream);
    } else {
        FILE *stream;
        stream = fopen(name.c_str(), "w");

        if (stream == NULL) {
            return (-1);
        }

        wrote = fwrite(buf, 1, size, stream);
        fclose(stream);
    }

    return (wrote);
}

/*
 * Used to unzip a .gz or .tgz file
 */
char *gh_zip::gzreadall (string name, size_t *size, size_t chunklet,
                 bool binary)
{
    gzFile file;

    file = gzopen(name.c_str(), "rb");

    if (file == NULL) {
        return (NULL);
    }

    vector <char*> chunks;
    vector <size_t> chunks_sizes;
    size_t chunks_total = 0;

    try {
        for (;;) {
            char *chunk = new char [chunklet];
            size_t chunk_size;

            if (chunk == NULL) {
                throw std::bad_alloc();
            }

            chunk_size = gzread(file, chunk, chunklet);

            if (chunk_size <= 0) {
                delete [] chunk;
                break;
            }

            chunks_total += chunk_size;
            chunks.push_back(chunk);
            chunks_sizes.push_back(chunk_size);

            if (chunk_size < chunklet) {
                break;
            }
        }

        char *base;

        if (chunks.size() == 1) {
            base = chunks[0];
        } else {
            char *data = new char [chunks_total];

            base = data;

            if (data == NULL) {
                throw std::bad_alloc();
            }

            for (uint32 i=0; i<chunks.size(); i++) {
                memcpy(data, chunks[i], chunks_sizes[i]);
                data += chunks_sizes[i];
                delete [] chunks[i];
            }
        }

        *size = chunks_total;

        gzclose(file);

        if (binary) {
            return (base);
        }

        dos2unix(base, size);

        return (base);

    } catch (...) {

        vector <char*>::iterator f;

        for (f = chunks.begin(); f != chunks.end(); f++) {
            delete [] *f;
        }

        gzclose(file);

        return (NULL);
    }
}


gh_zip *gh_zip::mpInstance = NULL;


gh_zip *gh_zip::instance() {
    if (mpInstance == NULL) {
        mpInstance = new gh_zip();
    }

    return mpInstance;
}

