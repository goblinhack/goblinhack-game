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

    $Id: model.h,v 1.6 2004/06/13 07:13:33 nmcgill Exp $
*/

#ifndef _GH_MODEL_H_
#define _GH_MODEL_H_

class gh_face
{   
public:
    gh_face(void) {}

    uint16 vertex_id[3];
    uint16 tex_uv_id[3];
    uint16 normal_id[3];
};

class gh_frame : public gh_stream
{
public:

    gh_frame(void) { }
    ~gh_frame() { }

    string              name;
    vector<gh_fpoint3d> vertices;   // The object's vertices
    vector<gh_fpoint>   tex_uv;     // The texture's UV coordinates
    vector<gh_fpoint3d> normals;    // The texture's UV coordinates
    vector<gh_face>     faces;      // The faces information of the object

    //
    // Bounding box for this frame
    //
    gh_fpoint3d         bounding_box[8];
    gh_fpoint3d         center;
};

class gh_model_anim_info
{
public:
    gh_model_anim_info (void)
    {
        elapsed_time = 0.0;
        last_time = 0.0;
        frame = 0;
        model_anim = 0;
    }

    //
    // Times used to step through the model_animations at a steady rate.
    //
    float               elapsed_time;
    float               last_time;

    uint32              frame;          // The current frame of the current model_anim 
    uint32              model_anim;     // The current index into model_anim list 
};

class gh_model_anim : public gh_stream
{
public:

    string name;
    uint32 start;     // This stores the first frame number for this model_animation
    uint32 end;       // This stores the last frame number for this model_animation

    gh_model_anim(void) {};
    gh_model_anim(const string name);
};

class gh_model : public gh_stream
{
public:
    enum model_type {
        MODEL_TYPE_MD2,
        MODEL_TYPE_MS3D,
    };

    //
    // model.cpp
    //
    gh_model(const string name);
    ~gh_model(void);

    void load(void);
    void save(void);

    void display(gh_model_anim_info *model_anim_info,
                 const bool wireframe = false,
                 gh_pixel pixel = GH_WHITE);

    void md2_display(gh_model_anim_info *model_anim_info,
                     const bool wireframe);

    void ms3d_display(gh_model_anim_info *model_anim_info,
                      const bool wireframe);

    void calculate_center(void);
    void calculate_bounding_box(void);
    void resize(const gh_fpoint3d size);
    void force_to_ground(void);

    //
    // Texture bank operations
    //
    typedef map<string, gh_model*> hash_models;

    static hash_models models;

    //
    // Find this model in the model bank
    //
    gh_inline static gh_model* find (const string &s1)
    {
        return (find(s1.c_str()));
    }

    //
    // Find this model in the model bank
    //
    gh_inline static gh_model* find (const char* s1,
                                     const bool err = true)
    {
        gh_model::hash_models::iterator m =
            models.find(s1);

        if (m == models.end()) {
            if (err) {
                GH_THROW("couldn't find " + string(s1) + " in loaded list");
            }
            return (NULL);
        }

        gh_model *model = m->second;

        if (model == NULL) {
            GH_THROW("null model " + string(s1) + " in loaded list");
        }

        return (model);
    }

    static void load_all(void);
    static void reload_all(void);
    static void unload_all(void);


    string              full_name;
    string              short_name;
    enum model_type     type;
    uint32              list;
    float               model_anim_speed;
    vector<gh_model_anim> model_anims;      // The list of model_animations 
    vector<gh_frame>    frames;             // The frames list for our model
    vector<long>        MD2_gl_commands;
    gh_fpoint3d         size;

    // only works for one model XXX
    float current_time(gh_model_anim_info *model_anim_info, uint32 frame_next);

    //
    // Simple models that can be made on the fly: 
    //
    static gh_model * model_rounded_square(gh_fsize size, gh_fsize radius);
    static gh_model * model_square(void);

    //
    // A single vertex
    //
    static gh_inline void gl_vertex2f (float x, float y)
    {
        glVertex2f(x, y);
    }

    static gh_inline void gl_vertex2uf (float x, float y, float u, float v)
    {
        glTexCoord2f(u, v);
        glVertex2f(x, y);
    }

    //
    // A flat square, no tex
    //
    static gh_inline void gl_square_no_tex (gh_fpoint tl, gh_fpoint br)
    {
        glBegin(GL_TRIANGLE_STRIP);

            gh_model::gl_vertex2f(tl.x, tl.y);
            gh_model::gl_vertex2f(br.x, tl.y);
            gh_model::gl_vertex2f(tl.x, br.y);
            gh_model::gl_vertex2f(br.x, br.y);

        glEnd();
    }

    //
    // As above, but for co-ordinates in the 0..1 range where the co-ordinate
    //
    // A flat square
    //
    static gh_inline void gl_square (gh_fpoint tl,
                                     gh_fpoint br,
                                     gh_fsize uv_tl = gh_fsize(0,1),
                                     gh_fsize uv_br = gh_fsize(1,0))
    {
        glBegin(GL_TRIANGLE_STRIP);

            gh_model::gl_vertex2uf(tl.x, tl.y, uv_tl.w, uv_tl.h);
            gh_model::gl_vertex2uf(br.x, tl.y, uv_br.w, uv_tl.h);
            gh_model::gl_vertex2uf(tl.x, br.y, uv_tl.w, uv_br.h);
            gh_model::gl_vertex2uf(br.x, br.y, uv_br.w, uv_br.h);

        glEnd();
    }

    //
    // As above, but for co-ordinates in the 0..1 range where the co-ordinate
    // can also be used to index the texture, with a scale factor, uv
    //
    // A single vertex
    //
    static gh_inline void gl_vertex2uf_uv (double x, double y, gh_fsize uv)
    {
        glTexCoord2f(x * uv.w, y * uv.h);
        glVertex2f(x, y);
    }

    //
    // A flat square
    //
    static gh_inline void gl_square_uv (gh_fpoint tl,
                                        gh_fpoint br,
                                        gh_fsize uv)
    {
        glBegin(GL_TRIANGLE_STRIP);
    
            gh_model::gl_vertex2uf_uv(tl.x, tl.y, uv);
            gh_model::gl_vertex2uf_uv(br.x, tl.y, uv);
            gh_model::gl_vertex2uf_uv(tl.x, br.y, uv);
            gh_model::gl_vertex2uf_uv(br.x, br.y, uv);

        glEnd();
    }

    //
    // A flat quad
    //
    static gh_inline void gl_quad_uv (gh_fpoint a,
                                      gh_fpoint b,
                                      gh_fpoint c,
                                      gh_fpoint d,
                                      gh_fsize uv)
    {
        glBegin(GL_TRIANGLE_STRIP);
    
            gh_model::gl_vertex2uf_uv(a.x, a.y, uv);
            gh_model::gl_vertex2uf_uv(b.x, b.y, uv);
            gh_model::gl_vertex2uf_uv(d.x, d.y, uv);
            gh_model::gl_vertex2uf_uv(c.x, c.y, uv);

        glEnd();
    }

    static uint32 gl_list_square(gh_fpoint tl,
                               gh_fpoint br,
                               gh_fsize uv,
                               gh_pixel hi,
                               gh_pixel med,
                               gh_pixel lo);

    static uint32 gl_list_square_bevelled(gh_fpoint tl,
                                        gh_fpoint br,
                                        gh_fsize uv,
                                        gh_pixel hi,
                                        gh_pixel med,
                                        gh_pixel lo,
                                        gh_fsize bevel);

    static uint32 gl_list_square_bevelled_plain(gh_fpoint tl,
                                              gh_fpoint br,
                                              gh_fsize uv,
                                              gh_pixel hi,
                                              gh_pixel med,
                                              gh_pixel lo,
                                              gh_fsize bevel);

    static uint32 gl_list_rounded_square_bevelled(gh_fpoint tl,
                                                gh_fpoint br,
                                                gh_fsize uv,
                                                gh_pixel hi,
                                                gh_pixel med,
                                                gh_pixel lo,
                                                gh_fsize bevel,
                                                gh_fsize radius,
                                                int sides);

    static uint32 gl_list_rounded_square(gh_fpoint tl,
                                       gh_fpoint br,
                                       gh_fsize uv,
                                       gh_pixel hi,
                                       gh_pixel med,
                                       gh_pixel lo,
                                       gh_fsize radius,
                                       int sides);

    static uint32 gl_list_resize_icon(gh_fpoint tl,
                                    gh_fpoint br,
                                    gh_fsize uv,
                                    gh_pixel hi,
                                    gh_pixel med,
                                    gh_pixel lo);

    static uint32 gl_list_close_icon(gh_fpoint tl, gh_fpoint br, gh_fsize uv,
                                   gh_pixel hi, gh_pixel med, gh_pixel lo);

    static void gl_list_shape(const char *shape[16], const gh_fsize uv,
                              const char start, const char end);
};

#endif /* _GH_MODEL_H_ */
