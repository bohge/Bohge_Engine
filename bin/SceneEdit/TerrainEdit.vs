#define saturate(x) clamp( x, 0.0, 1.0 )
#define lerp        mix
#include "Utility.glsl"
uniform mat4  matViewProj;
uniform vec3  vLightDir;
uniform vec3  vCamPos;
uniform vec3 g_FogParams;

attribute vec4 position;
attribute vec4 texCoord;
attribute vec3 normal;

varying vec4 g_vTexCoord;
varying vec3 g_vLightVec;
varying vec3 g_vViewVec;
varying vec3 g_vNormal;
varying float g_fogFactor;

void main()
{
	gl_Position = matViewProj * position;
	g_vTexCoord = texCoord;
    g_vNormal = normal;
    g_vLightVec = vLightDir;
    g_vViewVec  = vCamPos - position.xyz;
	//fog
	g_fogFactor = fogFactor( gl_Position.w, g_FogParams );  
}
