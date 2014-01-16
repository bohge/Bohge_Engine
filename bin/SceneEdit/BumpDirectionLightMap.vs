#define saturate(x) clamp( x, 0.0, 1.0 )
#define lerp        mix


uniform mat3  NormalTransform;	//faxian de bian huan ju zhen
uniform vec3  vLightDir;
uniform mat4  matShadowWorldViewProjBais;
uniform mat4  matWorldTransform;

attribute vec4 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec4 tangent;


varying vec2 g_vTexCoord;
varying vec3 g_vLightVec;
varying vec4 g_vOutShadowCoord;



void main()
{
	gl_Position = vec4( texCoord * vec2( 2.0, 2.0 ) + vec2( - 1.0, -1.0 ), 0.5, 1.0 );
	g_vTexCoord = texCoord;
	
	
	vec3 endNormal = NormalTransform * normal;
	vec3 endTangent = NormalTransform * tangent.xyz;	
	vec3 endBinormal = cross( endNormal, endTangent.xyz) * tangent.w;

	g_vLightVec.x = dot( vLightDir, endTangent);
    g_vLightVec.y = dot( vLightDir, endBinormal);
    g_vLightVec.z = dot( vLightDir, endNormal);
	
	g_vOutShadowCoord = matShadowWorldViewProjBais * position;
	vec4 worldPos = matWorldTransform * position;
}
