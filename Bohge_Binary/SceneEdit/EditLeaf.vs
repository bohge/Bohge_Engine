#define saturate(x) clamp( x, 0.0, 1.0 )
#define lerp        mix
#include "Utility.glsl"
uniform mat4 matWorldViewProj;
uniform vec3 vCameraPos;
uniform vec3 vLightDir;
uniform vec3 vAmbient;
uniform vec3 vLightColor;
uniform float Shininess;
uniform vec3 vSpecularLight;
uniform vec3 g_FogParams;

attribute vec4 position;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec2 g_vTexCoord;
varying vec3 g_vLightColor;
varying vec3 g_vSpecular;
varying float g_fogFactor;


void main()
{
    g_vTexCoord = texCoord;
	gl_Position = matWorldViewProj * position;
	//fog
	g_fogFactor = fogFactor( gl_Position.w, g_FogParams );
		
    vec3 vNormal= normalize( normal );
    vec3 vView   = normalize( vCameraPos - position.xyz );
    vec3 vHalf   = normalize( vLightDir + vView );
    
    float dotLightNormal= dot( vLightDir, vNormal );
	float fDiffuse = abs( dotLightNormal );
	float fSpecular = pow( abs( dot( vHalf, vNormal ) ) , Shininess );
	g_vSpecular = fSpecular * vSpecularLight;
	g_vLightColor = vAmbient + vLightColor * fDiffuse;
}
