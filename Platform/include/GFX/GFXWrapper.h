#pragma once
#include <Platform/Platform.h>

/**
 * Define your graphics related includes and whatnot here.
 */
#if CURRENT_PLATFORM == PLATFORM_PSP

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <psppower.h>
#include <psptypes.h>
#include <pspge.h>
#include <psputils.h>
#include <stdarg.h>
#include <intraFont.h>

#include <Platform/PSP/vram.h>
#include <malloc.h>

#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
#include <Platform/PC/Window.h>
#include <iostream>
#include <sstream>
#include <fstream>

#endif
#include <stb/stb_image.h>
#include <map>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * A general purpose OpenGL-like Graphics Wrapper for Stardust.
 */
namespace Stardust::GFX {

    /**
     * Internal Projection, View, and Model Matrixd.
     */
#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
    extern glm::mat4 _gfx_proj, _gfx_view, _gfx_model;
    extern GLuint program;
    extern std::vector<glm::mat4> _matrixStack;
#endif

/**
 * This block defines the three main buffers for GPU clearing.
 */
#if CURRENT_PLATFORM == PLATFORM_PSP
#define GFX_COLOR_BUFFER GU_COLOR_BUFFER_BIT
#define GFX_STENCIL_BUFFER GU_STENCIL_BUFFER_BIT
#define GFX_DEPTH_BUFFER GU_DEPTH_BUFFER_BIT
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
#define GFX_COLOR_BUFFER GL_COLOR_BUFFER_BIT
#define GFX_STENCIL_BUFFER GL_STENCIL_BUFFER_BIT
#define GFX_DEPTH_BUFFER GL_DEPTH_BUFFER_BIT
#else
#error No Graphics Clear Buffer Defines
#endif

    /**
     * Clear the current framebuffer.
     * 
     * \param flags - GFX_COLOR_BUFFER      - clears color buffer
     *              - GFX_DEPTH_BUFFER      - clears depth buffer
     *              - GFX_STENCIL_BUFFER    - clears stencil buffer
     */
    inline void gfxClear(int32_t flags){
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGuClearStencil(0);
        sceGuClearDepth(0);
        sceGuClear(GU_FAST_CLEAR_BIT | flags);
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        glClear(flags);
#else
#error No Graphics Clear Function
#endif
    }

    /**
     * Set the clear color.
     * 
     * \param r - Color value from 0.0 - 1.0
     * \param g - Color value from 0.0 - 1.0
     * \param b - Color value from 0.0 - 1.0
     * \param a - Color value from 0.0 - 1.0
     */
    inline void gfxClearColor(float r, float g, float b, float a){
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGuClearColor(GU_COLOR(r, g, b, a));
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        glClearColor(r, g, b, a);
#else
#error No Graphics Set Clear Function
#endif
    }

    /**
     * Orthographic projection.
     * 
     * \param bottom - Bottom coord
     * \param top - Top coord
     * \param left - Left coord
     * \param right - Right coord
     * \param znear - Z Near Coord
     * \param zfar - Z Far Coord
     */
    inline void gfxSetOrtho(float left, float right, float bottom, float top, float znear, float zfar){
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();
        sceGumOrtho(left, right, bottom, top, znear, zfar);

        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();

        sceGumMatrixMode(GU_MODEL);
        sceGumLoadIdentity();
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        _gfx_proj = glm::ortho(left, right, bottom, top, znear, zfar);
        _gfx_view = glm::mat4(1.0f);
        _gfx_model = glm::mat4(1.0f);
#else
#error No Graphics Set Clear Function
#endif
    }


    /**
     * These are internal shaders.
     */
#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
    inline GLuint compileShader(const char* source, GLenum shaderType){
        auto shader = glCreateShader(shaderType);

        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint status = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if(!status){
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            throw std::runtime_error("Shader compile failed: " + std::string(log));
        }

        return shader;
    }

    inline GLuint linkShaders(GLuint vshader, GLuint fshader){
        auto prog = glCreateProgram();
        glAttachShader(prog, vshader);
        glAttachShader(prog, fshader);

        glLinkProgram(prog);

        GLint status = 0;
        glGetProgramiv(prog, GL_LINK_STATUS, &status);

        if(!status){
            char log[512];
            glGetProgramInfoLog(prog, 512, nullptr, log);
            throw std::runtime_error("Shader linking failed: " + std::string(log));
        }

        return prog;
    }

    inline std::string getFile(std::string path){
        std::ifstream file(path);
        std::stringstream str;

        str << file.rdbuf();
        return str.str();
    }

    inline GLuint loadShaders(std::string vs, std::string fs) {
        std::string vss = getFile(vs);
        std::string fss = getFile(fs);
        GLuint vertShader, fragShader;

        vertShader = compileShader(vss.c_str(), GL_VERTEX_SHADER);
        fragShader = compileShader(fss.c_str(), GL_FRAGMENT_SHADER);

        auto programID = linkShaders(vertShader, fragShader);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return programID;
    }
#endif


#if CURRENT_PLATFORM == PLATFORM_PSP
    typedef struct
    {
        float u, v;
        unsigned int color;
        float x, y, z;
    }__attribute__((packed)) Vertex;
#endif

    /**
     * Graphics Meshes, which define data arrays for position, color, uv, and indices.
     */
    struct Mesh{
        std::vector<float> position;
        std::vector<float> color;
        std::vector<float> uv;
        std::vector<unsigned int> indices;
    };

    /**
     * Graphics Models, which define data arrays and their enabled properties.
     */
    class Model{
    public:
        Model() = default;
        Model(const Mesh& mesh){
            addData(mesh);
        }
        ~Model(){
            deleteData();
        }

#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
    private:
        inline void genBO(int dims, const std::vector<float>& data){
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(buffer_count, dims, GL_FLOAT, GL_FALSE, 0, 0);
            
            glEnableVertexAttribArray(buffer_count++);

            buffers.push_back(vbo);
        }

        inline void genEBO(const std::vector<GLuint>& indices){
            GLuint ebo;
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
            indicesCount = indices.size();
        }

    public:
#endif

        inline void addData(const Mesh& mesh) {

#if CURRENT_PLATFORM == PLATFORM_PSP
            //Generate data into relevant structure
            deleteData();

            for (int i = 0; i < mesh.position.size() / 3; i++) {
                Vertex v;
                v.x = mesh.position[i * 3 + 0];
                v.y = mesh.position[i * 3 + 1];
                v.z = mesh.position[i * 3 + 2];

                v.u = mesh.uv[i * 2 + 0];
                v.v = mesh.uv[i * 2 + 1];
        
                v.color = GU_COLOR(mesh.color[i * 4 + 0], mesh.color[i * 4 + 1], mesh.color[i * 4 + 2], mesh.color[i * 4 + 3]);
                verts.push_back(v);
            }

            indicesCount = mesh.indices.size();

            for (int i = 0; i < indicesCount; i++) {
                indices.push_back(mesh.indices[i]);
            }

            sceKernelDcacheWritebackInvalidateAll();
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
            //Generate VAO & buffers
            buffer_count = 0;

            if (vao != 0) {
                deleteData();
            }

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            //Default VBO.
            genBO(3, mesh.position);
            genBO(2, mesh.uv);
            genBO(4, mesh.color);
            genEBO(mesh.indices);
#else
#error No Graphics Model Data Handling
#endif
        }

        inline void deleteData(){
#if CURRENT_PLATFORM == PLATFORM_PSP
            //Delete structure
            verts.clear();
            indicesCount = 0;
            indices.clear();
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
            //Delete VAO & buffers
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(buffers.size(), buffers.data());

            buffers.clear();

            buffer_count = 0;
            colFl = false;
            texFl = false;
            idxFl = false;
            indicesCount = 0;
#else
#error No Graphics Model Data Deletion
#endif
        }

        inline void bind() {
#if CURRENT_PLATFORM == PLATFORM_PSP
            //No binding needed
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
            //Bind VAO
            glBindVertexArray(vao);
#else
#error No Graphics Model Data Binding
#endif
        }
        inline void draw() {
#if CURRENT_PLATFORM == PLATFORM_PSP
            //Rendering Call
            sceGuShadeModel(GU_SMOOTH);
            sceGumDrawArray(GU_TRIANGLES, GU_INDEX_16BIT | GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, indices.size(), (uint16_t*)indices.data(), verts.data());
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
            //Setup Program
            glUseProgram(program);
            
            glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_FALSE, glm::value_ptr(_gfx_proj));
            glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(_gfx_view));

            glm::mat4 newModel = glm::mat4(1.0f);

            for (int i = 0; i < _matrixStack.size(); i++) {
                newModel *= _matrixStack[i];
            }
            newModel *= _gfx_model;


            glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(newModel));

            //Bind VAO
            bind();

            //Render
            glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
#else
#error No Graphics Model Data Rendering
#endif
        }
    private:
        bool colFl;
        bool texFl;
        bool idxFl;
        int indicesCount;

#if CURRENT_PLATFORM == PLATFORM_PSP
        //TODO: Create PSP Vertex types
        std::vector<Vertex> verts;
        std::vector<short> indices;
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        GLuint vao;
        int buffer_count;
        std::vector<GLuint> buffers;
#endif
    };


    /**
     * Texture object representation.
     */
#if CURRENT_PLATFORM == PLATFORM_PSP
    inline int powerOfTwo(int value) {
        int poweroftwo = 1;
        while (poweroftwo < value) {
            poweroftwo <<= 1;
        }
        return poweroftwo;
    }

    inline void swizzle_fast(u8* out, const u8* in, unsigned int width, unsigned int height)
    {
        unsigned int blockx, blocky;
        unsigned int j;

        unsigned int width_blocks = (width / 16);
        unsigned int height_blocks = (height / 8);

        unsigned int src_pitch = (width - 16) / 4;
        unsigned int src_row = width * 8;

        const u8* ysrc = in;
        u32* dst = (u32*)out;

        for (blocky = 0; blocky < height_blocks; ++blocky)
        {
            const u8* xsrc = ysrc;
            for (blockx = 0; blockx < width_blocks; ++blockx)
            {
                const u32* src = (u32*)xsrc;
                for (j = 0; j < 8; ++j)
                {
                    *(dst++) = *(src++);
                    *(dst++) = *(src++);
                    *(dst++) = *(src++);
                    *(dst++) = *(src++);
                    src += src_pitch;
                }
                xsrc += 16;
            }
            ysrc += src_row;
        }
        }

    struct Texture{
        int width, height, pWidth, pHeight, ramSpace, colorMode, swizzled;
        unsigned short* data; //32 bit pixel data in (likely) 8888 RGBA

        int minFilter, maxFilter;
        bool repeat;
    };
#else
    struct Texture {
        int width, height, pWidth, pHeight;
        GLuint id;
    };
#endif

    /**
     * Texture Filters.
     */
#if CURRENT_PLATFORM == PLATFORM_PSP

#define GFX_FILTER_NEAREST GU_NEAREST
#define GFX_FILTER_LINEAR GU_LINEAR

#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)

#define GFX_FILTER_NEAREST GL_NEAREST
#define GFX_FILTER_LINEAR GL_LINEAR

#else
#error No GFX Filter defines
#endif

    /**
     * A Texture Manager which handles all loading and deletion of textures through an ID system.
     */
    class TextureManager{
    public:
        TextureManager(){
            fullMap.clear();
            texCount = 0;
        }

        /**
         * Loads up a texture.
         * 
         * \param texture - Path to the texture
         * \param filterMag - GFX Filters
         * \param filterMin - GFX Filters
         * \param repeat - Whether or not to repeat
         * \return 
         */
        inline unsigned int loadTex(std::string texture, int filterMag, int filterMin, bool repeat){
#if CURRENT_PLATFORM == PLATFORM_PSP
            int OutBytesPerPixel = 4;
            int Power2Width = 0;
            int Power2Height = 0;

            //GET WIDTH / HEIGHT
            int width, height, channels;
            //stbi_set_flip_vertically_on_load(true);
            unsigned short* data = (unsigned short*)stbi_load(texture.c_str(), &width, &height, &channels, STBI_rgb_alpha);

            Power2Width = powerOfTwo(width);
            Power2Height = powerOfTwo(height);

            Texture* Image1 = new Texture();

            int Swizzle = 1;
            int Vram = 0;
            int ColorMode = GU_PSM_8888;

            Image1->width = width;
            Image1->height = height;
            Image1->pWidth = Power2Width;
            Image1->pHeight = Power2Height;
            Image1->ramSpace = Vram;
            Image1->colorMode = ColorMode;
            Image1->swizzled = Swizzle;
            Image1->data = data;

            unsigned short* swizzled_pixels = NULL;
            if (Vram)
            {
                swizzled_pixels = (unsigned short*)getStaticVramTexture(Power2Width, Power2Height, ColorMode);
            }
            else
            {
                swizzled_pixels = (unsigned short*)memalign(16, Image1->pHeight * Image1->pWidth * OutBytesPerPixel);
            }

            swizzle_fast((u8*)swizzled_pixels, (const u8*)data, Image1->pWidth * OutBytesPerPixel, Image1->pHeight);

            Image1->data = swizzled_pixels;

            stbi_image_free(data);

            //clear the cache or there will be some errors
            sceKernelDcacheWritebackInvalidateAll();

            Image1->repeat = repeat;
            Image1->maxFilter = filterMag;
            Image1->minFilter = filterMin;


            fullMap.emplace(texCount, Image1);

            return texCount++;
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
            int width, height, nrChannels;
            //stbi_set_flip_vertically_on_load(true);
            unsigned char* data = stbi_load(texture.c_str(), &width, &height, &nrChannels, 4);

            Texture* tex = new Texture(); //Default initialization to 0 apparently
            glGenTextures(1, &tex->id);

            tex->width = tex->pWidth = width;
            tex->height = tex->pHeight = height;

            glBindTexture(GL_TEXTURE_2D, tex->id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);

            if(repeat){
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            }else{
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);

            fullMap.emplace(texCount, tex);

            return texCount++;
#else
#error No Texture Loading Functionality!
#endif
        }

        inline void bindTex(unsigned int id) {
            if (fullMap.find(id) != fullMap.end()) {
#if CURRENT_PLATFORM == PLATFORM_PSP
                sceGuEnable(GU_TEXTURE_2D);
                Texture* tex = fullMap[id];

                sceGuTexMode(tex->colorMode, 0, 0, tex->swizzled);
                sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
                sceGuTexFilter(tex->minFilter, tex->maxFilter);
                sceGuTexOffset(0.0f, 0.0f);
                sceGuTexWrap(tex->repeat ? GU_REPEAT : GU_CLAMP, tex->repeat ? GU_REPEAT : GU_CLAMP);
                sceGuTexImage(0, tex->pWidth, tex->pHeight, tex->pWidth, tex->data);

#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, fullMap[id]->id);
#else
#error No Texture Binding Functionality!
#endif
            }
        }
        
        inline void deleteTex(unsigned int in){
            if (fullMap.find(in) != fullMap.end()) {
#if CURRENT_PLATFORM == PLATFORM_PSP
                delete fullMap[in];
                fullMap.erase(in);
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
                glDeleteTextures(1, &fullMap[in]->id);
                delete fullMap[in];
                fullMap.erase(in);
#else
#error No Texture Deletion Functionality!
#endif

            }
        }

        inline Texture* getTex(unsigned int id) {
            if (fullMap.find(id) != fullMap.end()) {
                return fullMap[id];
            }
            return NULL;
        }
    private:
        std::map<unsigned int, Texture*> fullMap;
        int texCount;
    };

    
    /**
     * Pushes a matrix back onto the stack.
     * 
     */
    inline void pushMatrix(){
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGumPushMatrix();
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        _matrixStack.push_back(_gfx_model);
        _gfx_model = glm::mat4(1.0f);
#else
#error No GFX Matrix Stack (Push)!
#endif
    }

    /**
     * Pops a matrix back off the stack.
     * 
     */
    inline void popMatrix(){
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGumPopMatrix();
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        _gfx_model = _matrixStack[_matrixStack.size() - 1];
        _matrixStack.pop_back();
#else
#error No GFX Matrix Stack (Pop)!
#endif
    }

    /**
     * Loads the identity matrix.
     * 
     */
    inline void clearModelMatrix(){
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGumMatrixMode(GU_MODEL);
        sceGumLoadIdentity();
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        _gfx_model = glm::mat4(1.0f);
#else
#error No GFX Matrix Clear.
#endif
    }

    /**
     * Translates a model by Vector V.
     * 
     * \param v - The translation
     */
    inline void translateModelMatrix(glm::vec3 v) {
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGumMatrixMode(GU_MODEL);
        ScePspFVector3 vv = { v.x, v.y, v.z };
        sceGumTranslate(&vv);
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        _gfx_model = glm::translate(_gfx_model, v);
#else
#error No GFX Matrix Translate.
#endif
    }

    /**
     * Scales a model by Vector V.
     *
     * \param v - The scale
     */
    inline void scaleModelMatrix(glm::vec3 v) {
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGumMatrixMode(GU_MODEL);
        ScePspFVector3 vv = { v.x, v.y, v.z };
        sceGumScale(&vv);
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        _gfx_model = glm::scale(_gfx_model, v);
#else
#error No GFX Matrix Scale.
#endif
    }

    inline void gfxSetProjView(glm::mat4 proj, glm::mat4 view){
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGumMatrixMode(GU_PROJECTION);
        ScePspFMatrix4 m = *((ScePspFMatrix4*)glm::value_ptr(proj));
        sceGumLoadMatrix(&m);

        sceGumMatrixMode(GU_VIEW);
        ScePspFMatrix4 m1 = *((ScePspFMatrix4*)glm::value_ptr(view));
        sceGumLoadMatrix(&m1);

#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        _gfx_proj = proj;
        _gfx_view = view;
#else
#error No GFX Matrix Scale.
#endif
    }

    //TODO: Add rotation to model matrix.

    /**
     * Basic texture atlas.
     */
    class TextureAtlas {
    public:
        TextureAtlas(short tileSide){
            sideCount = { tileSide, tileSide };
        }
        TextureAtlas(glm::vec2 sideXY){
            sideCount = sideXY;
        }

        inline std::array<float, 8> getTexture(int index){
            int row = index / (int)sideCount.x;
            int column = index % (int)sideCount.y;

            float sizeX = 1.f / ((float)sideCount.x);
            float sizeY = 1.f / ((float)sideCount.y);
            float y = (float)row * sizeY;
            float x = (float)column * sizeX;
            float h = y + sizeY;
            float w = x + sizeX;

            return {
                x, y,
                w, y,
                w, h,
                x, h,
            };
        }

    private:
        glm::vec2 sideCount;
    };

}

namespace Stardust::GFX {
    extern TextureManager* g_TextureManager;
}