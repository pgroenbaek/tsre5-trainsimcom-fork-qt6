#version 130

in vec2 vTextureCoord;
in float fogFactor;
in vec3 vNormal;

uniform float textureEnabled;
uniform vec4 shapeColor;
uniform float isAlpha;
uniform float alphaTest;
uniform vec4 skyColor;
uniform vec4 diffuseColor;
uniform vec4 ambientColor;
uniform vec4 specularColor;
uniform vec3 lightDirection;
uniform sampler2D uSampler;
uniform sampler2D uSampler2;
uniform float secondTexEnabled;
uniform mat4 uMVMatrix;
uniform mat4 uMSMatrix;
uniform float enableNormals;

// Output
out vec4 fragColor;

void main() {
    if (textureEnabled != 0.0) {
        vec4 color = texture(uSampler, vTextureCoord);
        if (color.a < alphaTest)
            discard;
        fragColor = color;
    } else {
        fragColor = shapeColor;
    }
}
