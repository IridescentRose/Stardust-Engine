#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aCol;


uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

//Optional
out vec2 uv;
out vec3 color;

void main()
{
    gl_Position = proj * view * model vec4(aPos, 1.0);
    uv = aTex;
    color = aCol;
}