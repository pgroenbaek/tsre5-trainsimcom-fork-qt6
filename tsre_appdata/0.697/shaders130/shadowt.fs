#version 130

varying vec2 vTextureCoord;

uniform sampler2D uSampler4;
uniform vec3 iMapShadow;

void main() {
    vec4 z = texture2D(uSampler4, vTextureCoord);
    z = z * 0.5 + 0.5;
    gl_FragDepth = iMapShadow.x * z.r + iMapShadow.y * z.g + iMapShadow.z * z.b;
}
