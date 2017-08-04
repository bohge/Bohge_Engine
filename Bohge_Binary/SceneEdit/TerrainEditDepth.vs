#define saturate(x) clamp( x, 0.0, 1.0 )
#define lerp        mix
#include "LinerDepth.glsl"

uniform mat4 matWorldViewProj;


attribute vec4 position;
varying highp float g_fDepth;

void main()
{
	g_fDepth = GetLineDepth( position.xyz );
	gl_Position = matWorldViewProj * position;
}
