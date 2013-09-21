/*
    retreat.
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

    $Id: surface.cpp 1239 2009-07-07 20:03:23Z Ghack $
*/

#include <ghack.h>
#include <gh_surface.h>
#ifdef HAVE_EXTGL
#include <extgl.h>
#endif

#define GH_NEED_DOUBLE_BUFFER
#define GH_REPORT_GRAPHICS_INFO_AT_START

gh_surface::hash_surfaces gh_surface::surfaces;

gh_surface::gh_surface (gh_size3d size, uint32 flags, enum gh_surfaceType type)
{
    //
    // This can be quicker on rubbish gfx cards
    //
    if (!size.d) {
        size.d = 16;
    }

    //
    // This is the primary video surface
    //
    if (type == isVideo) {
        sdlSurface = SDL_SetVideoMode(size.w, size.h, size.d, flags);

        GH_LOG("Video screen size " +
               tostring(size.w) + "x" +
               tostring(size.h) + "x" +
               tostring(size.d));

        if (sdlSurface == NULL) {
            GH_WARN(SDL_GetError());
            SDL_ClearError();
            gh_global::screensize = gh_size(1024, 800);
            gh_global::config_changed = true;
            size.w = gh_global::screensize.w;
            size.h = gh_global::screensize.h;

            GH_LOG("Retry video screen size " +
                   tostring(size.w) + "x" +
                   tostring(size.h) + "x" +
                   tostring(size.d));

            sdlSurface = SDL_SetVideoMode(size.w, size.h, size.d, flags);
            if (sdlSurface == NULL) {
                GH_WARN(SDL_GetError());
                SDL_ClearError();
                gh_global::screensize = gh_size(800, 640);
                size.w = gh_global::screensize.w;
                size.h = gh_global::screensize.h;

                GH_LOG("Retry video screen size " +
                       tostring(size.w) + "x" +
                       tostring(size.h) + "x" +
                       tostring(size.d));

                sdlSurface = SDL_SetVideoMode(size.w, size.h, size.d, flags);

                if (sdlSurface == NULL) {
                    GH_THROW(SDL_GetError());
                }
            }
        }

        SDL_ClearError();

        center_window();

        this->orig_size = this->size = size;

        tex = 0;

        if (flags & SDL_OPENGL) {
            if (openglprobe(size) != true) {
                GH_THROW("opengl init");
            }
        }
    } else {
        common(size, flags,
            SDL_GetVideoSurface()->format->Rmask,
            SDL_GetVideoSurface()->format->Gmask,
            SDL_GetVideoSurface()->format->Bmask,
            SDL_GetVideoSurface()->format->Amask);
    }
}

gh_surface::gh_surface (gh_size3d size, uint32 flags,
    uint32 rmask, uint32 gmask, uint32 bmask, uint32 amask)
{
    common(size, flags, rmask, gmask, bmask, amask);
}

gh_surface::gh_surface (string file)
{
    if (!file.size()) {
        GH_THROW("no file");
    }

    string filename = gh_global::locate(file);

    cout << "  \"" << filename << "\"" << endl;

    sdlSurface = IMG_Load((char*)filename.c_str());
    if (sdlSurface == NULL) {
        GH_THROW(("failed in loading " + filename));
    }

    //
    // Each file should be unique without the path
    //
    const char *sep;

    if ((sep = strrchr(filename.c_str(), '/')) != NULL) {
        short_name = sep + 1;
    } else {
        short_name = filename;
    }

    if (strstr(short_name.c_str(), ".gz")) {
        short_name.resize(short_name.size() - 3);
    }

    full_name = filename;

    size.w = sdlSurface->w;
    size.h = sdlSurface->h;
    size.d = sdlSurface->format->BitsPerPixel;

    orig_size = size;

    tex = 0;
}

//
// Nuke all textures
//
void gh_surface::unload_all (void)
{
    GH_FUNCTION()

    GH_LOG("Unloading surfaces");

    hash_surfaces::iterator s = surfaces.begin();

    while (s != surfaces.end()) {
        delete s->second;
        s++;
    }

    hash_surfaces::iterator so = surfaces.begin();
    hash_surfaces::iterator eo = surfaces.end();

    surfaces.erase(so, eo);
}

void gh_surface::load_all (void)
{
    GH_FUNCTION()

    string dir = gh_global::locatedir("gfx");

    if (dir == "") {
        GH_THROW("no gfx dir?");
        return;
    }

    GH_LOG("Load gfx");

    vector<string> files = gh_global::listdir_recursive(dir, "");

    vector<string>::iterator i;

    for (i = files.begin(); i != files.end(); i++) {
        string full_name = *i;
        string short_name = *i;

        if (!string_contains(short_name, ".png") &&
            !string_contains(short_name, ".jpg")) {
            continue;
        }

        if (strstr(short_name.c_str(), ".gz")) {
            short_name.resize(short_name.size() - 3);
        }

        const char *s = strrchr(short_name.c_str(), '/');

        if (s == NULL) {
            s = short_name.c_str();
        } else {
            s++;
        }

        if (surfaces[s]) {
            GH_THROW(("duplicate named object " + full_name));
        }

        surfaces[s] = new gh_surface(full_name);

        if (!surfaces[s]) {
            GH_THROW(("couldn't create object " + full_name));
        }

        surfaces[s]->texture(GL_RGB);
    }
}

void gh_surface::reload_all (void)
{
    GH_FUNCTION()

    GH_LOG("Reload gfx");

    hash_surfaces::iterator f;

    for (f = surfaces.begin(); f != surfaces.end(); f++) {
        gh_surface *s = f->second;

        s->reset();

        GH_LOG("  Reloading " + s->full_name);

        s->sdlSurface = IMG_Load((char*)s->full_name.c_str());

        if (s->sdlSurface == NULL) {
            GH_THROW(("couldn't reload " + s->full_name));
        }

        s->size.w = s->sdlSurface->w;
        s->size.h = s->sdlSurface->h;
        s->size.d = s->sdlSurface->format->BitsPerPixel;

        s->texture(GL_RGB);
    }
}

void gh_surface::common (gh_size3d size, uint32 flags,
    uint32 rmask, uint32 gmask, uint32 bmask, uint32 amask)
{
    GH_FUNCTION()

    this->orig_size = this->size = size;

    if (!size.d) {
        size.d = SDL_GetVideoSurface()->format->BitsPerPixel;
    }

    if (!size.d) {
        GH_THROW("no default depth set");
    }

    if (!flags) {
        flags = SDL_GetVideoSurface()->flags;
    }

    sdlSurface = SDL_AllocSurface(flags, size.w, size.h, size.d,
        rmask, gmask, bmask, amask);

    if (sdlSurface == NULL) {
        GH_THROW(SDL_GetError());
    }

    if (size.w != sdlSurface->w) {
        GH_THROW(SDL_GetError());
    }

    if (size.h != sdlSurface->h) {
        GH_THROW(SDL_GetError());
    }

    if (sdlSurface->format != NULL) {
        //
        // Depth of zero allows you to specify the default depth
        //
        if (size.d && (size.d != sdlSurface->format->BitsPerPixel)) {
            GH_THROW(SDL_GetError());
        }
    } else {
        if (size.d) {
            GH_THROW(SDL_GetError());
        }
    }

    tex = 0;

    cachePutPixel = getCacheLowerPutPixel();
    cacheGetPixel = getCacheLowerGetPixel();
}

gh_surface::~gh_surface ()
{
    GH_FUNCTION()

    if (full_name == "") {
        return;
    }

//  cout << "GFX~ \"" << full_name << "\"" << endl;

    reset();
}

void gh_surface::reset (void)
{
    GH_FUNCTION()

    if (tex) {
#if defined WIN32 || defined __CYGWIN__
        //
        // resetting the gfx subsystem seems to nuke the textures
        // prior to use deleting them...
        //
#else
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, (GLuint*) &tex);
#endif
        tex = 0;
    }

    if (sdlSurface != NULL) {
        SDL_FreeSurface(sdlSurface);
        sdlSurface = NULL;
    }
}

void gh_surface::resize (gh_size3d size, uint32 flags_new)
{
    GH_FUNCTION()

//  bool fail = false;

    SDL_Surface * sdlSurface =
        SDL_SetVideoMode(size.w, size.h, size.d, flags_new);

    GH_LOG("New screen size " +
           tostring(size.w) + "x" +
           tostring(size.h) + "x" +
           tostring(size.d));

    if (sdlSurface == NULL) {
        GH_THROW(SDL_GetError());
    }

    if (size.w != sdlSurface->w) {
        GH_WARN(SDL_GetError());
    }

    if (size.h != sdlSurface->h) {
        GH_WARN(SDL_GetError());
    }

    SDL_ClearError();

    center_window();

    if (SDL_GetVideoSurface()->flags & SDL_OPENGL) {
        if (openglprobe(size) != true) {
            GH_THROW("opengl init");
        }
    }
}

void gh_surface::openglinit (void)
{
#ifdef HAVE_EXTGL
#ifdef GH_REPORT_GRAPHICS_INFO_AT_START
    printf("\nExtensions on card:\n");
#endif
    extern int ext_glInitialize(void);

    ext_glInitialize();
#endif

    //
    // Quicker for loading screen
    //
    smooth_scroll(false);
}

void gh_surface::smooth_scroll (bool on)
{
#ifdef WIN32
    typedef void (*wglSwapIntervalEXTPROC)(int interval);

    //
    // Force smooth scrolling
    //
    // 1 - is wait for vsync - but it also depends on the graphics
    // card allowing this
    //
    wglSwapIntervalEXTPROC fn = (wglSwapIntervalEXTPROC)
                    SDL_GL_GetProcAddress("wglSwapIntervalEXT");
    if (fn) {
        (*fn)(on == true ? 1 : 0);
    }
#endif
}

bool gh_surface::openglprobe (const gh_size3d videoSz)
{
#ifdef GH_NEED_DOUBLE_BUFFER
    int value;
#endif

    static bool first = false;

    if (!first) {
        first = true;

#ifdef GH_REPORT_GRAPHICS_INFO_AT_START
        printf("  Screen                   : %d bitplanes\n",
            SDL_GetVideoSurface()->format->BitsPerPixel);
        printf("  Vendor                   : %s\n", glGetString(GL_VENDOR));
        printf("  Renderer                 : %s\n", glGetString(GL_RENDERER));
        printf("  Version                  : %s\n", glGetString(GL_VERSION));
        printf("  Extensions               : %s\n", glGetString(GL_EXTENSIONS));
#endif

#ifdef GH_NEED_DOUBLE_BUFFER
        SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
        if (value != 1)  {
            printf("  Double buffer, requested, got no\n");
        } else {
            printf("  Double buffer            : %u", value);
        }
#endif
        openglinit();

#ifdef GH_REPORT_GRAPHICS_INFO_AT_START
        printf("\n");
#endif
    }

    glViewport(0, 0, videoSz.w, videoSz.h);

    // Set up our 2D view
    //
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0,gh_global::screensize.w,gh_global::screensize.h,0,-100,100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //
    // Clear the screen early on to hide garbage from the cygwin console.
    //
    return (true);
}

//
// Make the current SDL surface into a OpenGl surface
//
void gh_surface::texture (uint32 type)
{
    GH_FUNCTION()

    const uint32 orig_type = type;

//      GH_LOG("    size " + tostring(size.w) + "x" + tostring(size.h));

        SDL_Surface * larger_surface = SDL_CreateRGBSurface(
                    sdlSurface->flags,
                    next_pow_of_2(sdlSurface->w),
                    next_pow_of_2(sdlSurface->h),
                    sdlSurface->format->BitsPerPixel,
                    sdlSurface->format->Rmask,
                    sdlSurface->format->Gmask,
                    sdlSurface->format->Bmask,
                    sdlSurface->format->Amask);

        SDL_BlitSurface(sdlSurface, NULL, larger_surface, NULL);
        SDL_FreeSurface(sdlSurface);
        sdlSurface = larger_surface;

        size.w = sdlSurface->w;
        size.h = sdlSurface->h;


    if (tex) {
        GH_THROW("already a texture");
    }

    //
    // Create Texture   
    //
    glGenTextures(1, (GLuint*) &tex);

    if (glGetError() != GL_NO_ERROR) {
        GH_THROW("OpenGL error (glGenTextures): " + (int) glGetError());
    }

    if (!tex) {
        GH_THROW("failed to generate texture");
    }

    //
    // 2d texture (x and y size)
    //
    glBindTexture(GL_TEXTURE_2D, tex);

    if (glGetError() != GL_NO_ERROR) {
        glDeleteTextures(1, (GLuint*) &tex);
        tex = 0;
        GH_THROW("OpenGL error (glBindTexture): " + (int) glGetError());
    }

    //
    // NEAREST is quicker than LINEAR
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glEnable(GL_TEXTURE_2D);

    SDL_PixelFormat o={0};

    if (orig_type == GL_RGBA) {
        o.BitsPerPixel=32;
        o.BytesPerPixel=4;
        o.Rloss=0;
        o.Gloss=0;
        o.Bloss=0;
        o.Aloss=0;
        o.Rshift=16;
        o.Gshift=8;
        o.Bshift=0;
        o.Ashift=24;
        o.Amask=0xff000000;
        o.Rmask=0x00ff0000;
        o.Gmask=0x0000ff00;
        o.Bmask=0x000000ff;
    } else {
        o.BitsPerPixel=24;
        o.BytesPerPixel=3;
        o.Rloss=0;
        o.Gloss=0;
        o.Bloss=0;
        o.Rshift=16;
        o.Gshift=8;
        o.Bshift=0;
        o.Rmask=0xff0000;
        o.Gmask=0x00ff00;
        o.Bmask=0x0000ff;
    }

    int internal_format;

    if (orig_type == GL_RGBA) {
        internal_format = GL_RGBA;
    } else {
        internal_format = GL_RGB;
    }

    SDL_Surface * tmp = sdlSurface;
    SDL_Surface * gl = sdlSurface;

    gl = SDL_ConvertSurface(tmp, &o, 0);

    if (gl == NULL) {
        glDeleteTextures(1, (GLuint*) &tex);
        tex = 0;
        GH_THROW(SDL_GetError());
    }

    sdlSurface = gl;
    SDL_FreeSurface(tmp);

    size.w = sdlSurface->w;
    size.h = sdlSurface->h;
    size.d = sdlSurface->format->BitsPerPixel;

    if (orig_type == GL_RGBA) {
        if (size.d != 32) {
            glDeleteTextures(1, (GLuint*) &tex);
            tex = 0;
            GH_THROW(SDL_GetError());
        }

        //
        // GL surfaces are upsidedown and RGB, not BGR :-)
        //
        uint8 tmpbuf[sdlSurface->pitch];
        uint8 * rowhi = (uint8 *)sdlSurface->pixels;
        uint8 * rowlo = rowhi + (sdlSurface->h * sdlSurface->pitch) -
            sdlSurface->pitch;

        for (int i=0; i<sdlSurface->h/2; ++i) {
            for (int j=0; j<sdlSurface->w; ++j) {
                uint8 tmpch = rowhi[j*4];
                rowhi[j*4] = rowhi[j*4+2];
                rowhi[j*4+2] = tmpch;
                tmpch = rowlo[j*4];
                rowlo[j*4] = rowlo[j*4+2];
                rowlo[j*4+2] = tmpch;
            }
            memcpy(tmpbuf, rowhi, sdlSurface->pitch);
            memcpy(rowhi, rowlo, sdlSurface->pitch);
            memcpy(rowlo, tmpbuf, sdlSurface->pitch);
            rowhi += sdlSurface->pitch;
            rowlo -= sdlSurface->pitch;
        }
    } else {

        if (size.d != 24) {
            glDeleteTextures(1, (GLuint*) &tex);
            tex = 0;
            GH_THROW(SDL_GetError());
        }

        //
        // GL surfaces are upsidedown and RGB, not BGR :-)
        //
        uint8 tmpbuf[sdlSurface->pitch];
        uint8 * rowhi = (uint8 *)sdlSurface->pixels;
        uint8 * rowlo = rowhi + (sdlSurface->h * sdlSurface->pitch) -
            sdlSurface->pitch;

        for (int i=0; i<sdlSurface->h/2; ++i) {
            for (int j=0; j<sdlSurface->w; ++j) {
                uint8 tmpch = rowhi[j*3];
                rowhi[j*3] = rowhi[j*3+2];
                rowhi[j*3+2] = tmpch;
                tmpch = rowlo[j*3];
                rowlo[j*3] = rowlo[j*3+2];
                rowlo[j*3+2] = tmpch;
            }
            memcpy(tmpbuf, rowhi, sdlSurface->pitch);
            memcpy(rowhi, rowlo, sdlSurface->pitch);
            memcpy(rowlo, tmpbuf, sdlSurface->pitch);
            rowhi += sdlSurface->pitch;
            rowlo -= sdlSurface->pitch;
        }
    }

    // 2d texture,
    // level of detail 0 (normal),
    // 3 components (red, green, blue),
    // x size from image, y size from image,
    // border 0 (normal), rgb color data, unsigned byte datau
    // and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, type, size.w, size.h,
        0, GL_RGB, GL_UNSIGNED_BYTE, sdlSurface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1);

//  gluBuild2DMipmaps(GL_TEXTURE_2D, internal_format, size.w, size.h,
//      orig_type, GL_UNSIGNED_BYTE, sdlSurface->pixels);
}

void gh_surface::clear (void)
{
    GH_BLACK.gl();

    glBindTexture(GL_TEXTURE_2D, 0);

    glBegin(GL_TRIANGLE_STRIP);

    gh_point br(gh_global::screensize.w, gh_global::screensize.h);

    glVertex2d(0,0);
    glVertex2d(br.x, 0);
    glVertex2d(0, br.y);
    glVertex2d(br.x, br.y);

    glEnd();
}

void gh_surface::clear (const gh_fpoint &tl, const gh_fpoint &br)
{
    glBindTexture(GL_TEXTURE_2D, tex);

    GH_BLACK.gl();

    glBindTexture(GL_TEXTURE_2D, 0);

    glBegin(GL_TRIANGLE_STRIP);

    glVertex2f(tl.x, tl.y);
    glVertex2f(br.x, tl.y);
    glVertex2f(tl.x, br.y);
    glVertex2f(br.x, br.y);

    glEnd();
}

void gh_surface::clear (const gh_point &tl, const gh_point &br)
{
    glBindTexture(GL_TEXTURE_2D, tex);

    GH_BLACK.gl();

    glBindTexture(GL_TEXTURE_2D, 0);

    glBegin(GL_TRIANGLE_STRIP);

    glVertex2d(tl.x, tl.y);
    glVertex2d(br.x, tl.y);
    glVertex2d(tl.x, br.y);
    glVertex2d(br.x, br.y);

    glEnd();
}
