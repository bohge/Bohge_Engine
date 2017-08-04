#define saturate(x) clamp( x, 0.0, 1.0 )
#define lerp        mix
#include "LinerDepth.glsl"

uniform mat4 matWorldViewProj;
uniform mat4  matWorldTransform;

attribute vec4 position;
attribute vec2 texCoord;

varying vec2 g_vTexCoord;
varying highp float g_fDepth;


void main()
{
    g_vTexCoord = texCoord;
	vec4 worldPos = matWorldTransform * position;
	g_fDepth = GetLineDepth( worldPos.xyz );
	gl_Position = matWorldViewProj * position;
}
