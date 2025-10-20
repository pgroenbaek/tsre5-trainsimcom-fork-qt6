#version 130

attribute vec4 vertex;
varying vec2 vTextureCoord;

void main() {
    gl_Position = vertex;
    vTextureCoord = 0.5 * vertex.xy + 0.5;
}
