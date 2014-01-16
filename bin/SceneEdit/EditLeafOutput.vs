#define saturate(x) clamp( x, 0.0, 1.0 )
#define lerp        mix
#include "Utility.glsl"
uniform mat4 matWorldViewProj;


attribute vec4 position;
attribute vec2 texCoord;

varying vec2 g_vTexCoord;


void main()
{
    g_vTexCoord = texCoord;
	gl_Position = matWorldViewProj * position;
}
