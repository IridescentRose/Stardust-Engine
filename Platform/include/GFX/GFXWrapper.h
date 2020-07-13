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

#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
#include <Platform/PC/Window.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <sstream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

#endif

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
    inline void gfxSetOrtho(float bottom, float top, float left, float right, float znear, float zfar){
#if CURRENT_PLATFORM == PLATFORM_PSP
        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();
        sceGumOrtho(0, 480, 272, 0, -30, 30);

        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();

        sceGumMatrixMode(GU_MODEL);
        sceGumLoadIdentity();
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        _gfx_proj = glm::ortho(bottom, top, left, right, znear, zfar);
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
        Model(const Mesh* mesh){
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

        inline void addData(const Mesh* mesh){

#if CURRENT_PLATFORM == PLATFORM_PSP
            //Generate data into relevant structure
            
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
            //Generate VAO & buffers
            buffer_count = 0;

            if (vao != 0) {
                deleteData();
            }

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            //Default VBO.
            genBO(3, mesh->position);
            
            genBO(2, mesh->uv);
            
            
            genBO(3, mesh->color);
            
            genEBO(mesh->indices);
#else
#error No Graphics Model Data Handling
#endif
        }

        inline void deleteData(){
#if CURRENT_PLATFORM == PLATFORM_PSP
            //Delete structure

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

#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
            //Setup Program
            glUseProgram(program);
            
            glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_FALSE, glm::value_ptr(_gfx_proj));
            glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(_gfx_view));
            glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(_gfx_model));

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
#elif (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
        GLuint vao;
        int buffer_count;
        std::vector<GLuint> buffers;
#endif
    };
}