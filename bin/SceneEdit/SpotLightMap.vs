#define saturate(x) clamp( x, 0.0, 1.0 )
#define lerp        mix

// #include "LinerDepth.glsl"

uniform mat4  matTransform;	//moxing de bian huan ju zhen
uniform mat3  NormalTransform;	//faxian de bian huan ju zhen
uniform vec3  vLightPos;
uniform mat4  matShadowWorldViewProjBais;



attribute vec4 position;
attribute vec2 texCoord;
attribute vec3 normal;


varying vec3 g_vLightVec;
varying vec3 g_vNormal;
varying vec4 g_vOutShadowCoord;

void main()
{
	gl_Position = vec4( texCoord * vec2( 2.0, 2.0 ) + vec2( - 1.0, -1.0 ), 0.5, 1.0 );
	
	vec4 vPositionWS = matTransform * position;

    g_vNormal = NormalTransform * normal;

	float poswinv = 1.0 / vPositionWS.w;
    g_vLightVec = vLightPos - vPositionWS.xyz * poswinv;

	g_vOutShadowCoord = matShadowWorldViewProjBais * position;
}
