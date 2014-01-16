

uniform mat4  matTransform;
uniform mat3  NormalTransform;
uniform vec3  vLightPos;
uniform vec3  vCamPos;



attribute vec4 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec4 tangent;


varying vec2 g_vTexCoord;
varying vec3 g_vLightTangent;
varying vec3 g_vLightWorld;
varying vec3 g_vNormal;


void main()
{
	gl_Position = vec4( texCoord * vec2( 2.0, 2.0 ) + vec2( - 1.0, -1.0 ), 0.5, 1.0 );	
	
	g_vTexCoord = texCoord;
	
	vec4 vPositionWS = matTransform * position;
    g_vLightWorld = vLightPos - vPositionWS.xyz;
	
	vec3 endNormal = NormalTransform * normal;
	vec3 endTangent = NormalTransform * tangent.xyz;	
	vec3 endBinormal = cross( endNormal, endTangent.xyz) * tangent.w;

	g_vLightTangent.x = dot( g_vLightWorld, endTangent);
    g_vLightTangent.y = dot( g_vLightWorld, endBinormal);
    g_vLightTangent.z = dot( g_vLightWorld, endNormal);
}
