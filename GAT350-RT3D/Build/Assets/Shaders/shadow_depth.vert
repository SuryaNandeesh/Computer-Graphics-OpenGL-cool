#version 430

layout (location = 0) in vec3 vposition;

uniform mat4 model;            // Model matrix (transforms vertices from model space to world space)
uniform mat4 shadowVP;  // Light's projection matrix

void main()
{
    // Transform vertex position to light's view space
    gl_Position = shadowVP * model * vec4(vposition, 1.0);
}
