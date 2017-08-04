#define saturate(x) clamp( x, 0.0, 1.0 )
#define lerp        mix
#include "LinerDepth.glsl"

//uniform mat4 matViewProj;
uniform vec3 vLightDir;
uniform mat4 matShadowWorldViewProjBais;


attribute vec4 position;
attribute vec4 texCoord;
attribute vec3 normal;
attribute vec3 tangent;

varying vec4 g_vTexCoord;
varying vec3 g_vLightVec;
varying vec4 g_vOutShadowCoord;
varying float g_fDepth;



void main()
{
	//gl_Position = matViewProj * position;
	gl_Position = vec4( texCoord.zw * vec2( 2.0, -2.0 ) + vec2( - 1.0, 1.0 ), 0.5, 1.0 );
	
	g_vTexCoord = texCoord;
	
    vec3 endNormal = normal;
	vec3 endTangent = tangent;	
	vec3 endBinormal = cross( endNormal, endTangent);

	
    vec3 LightWorld = vLightDir;
	
	g_vLightVec.x = dot( LightWorld, endTangent);
    g_vLightVec.y = dot( LightWorld, endBinormal);
    g_vLightVec.z = dot( LightWorld, endNormal);
       
	g_vOutShadowCoord = matShadowWorldViewProjBais * position;
	g_fDepth = GetLineDepth( position.xyz );	
}
