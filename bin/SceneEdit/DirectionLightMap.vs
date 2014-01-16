#define saturate(x) clamp( x, 0.0, 1.0 )
#define lerp        mix


uniform mat3  NormalTransform;	//faxian de bian huan ju zhen
uniform vec3  vLightDir;
uniform mat4  matShadowWorldViewProjBais;
uniform mat4  matWorldTransform;

attribute vec4 position;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec2 g_vTexCoord;
varying vec3 g_vLightVec;
varying vec3 g_vNormal;
varying vec4 g_vOutShadowCoord;


void main()
{
	gl_Position = vec4( texCoord * vec2( 2.0, 2.0 ) + vec2( - 1.0, -1.0 ), 0.5, 1.0 );
	g_vTexCoord = texCoord;
	
    g_vNormal = NormalTransform * normal;
    g_vLightVec = vLightDir;
	
	g_vOutShadowCoord = matShadowWorldViewProjBais * position;
	vec4 worldPos = matWorldTransform * position;
}
