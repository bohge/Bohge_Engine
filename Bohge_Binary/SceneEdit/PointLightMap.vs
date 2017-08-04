uniform mat4  matTransform;
uniform mat3  NormalTransform;
uniform vec3  vLightPos;
uniform vec3  vCamPos;



attribute vec4 position;
attribute vec2 texCoord;
attribute vec3 normal;


varying vec3 g_vLightVec;
varying vec3 g_vNormal;


void main()
{
	gl_Position = vec4( texCoord * vec2( 2.0, 2.0 ) + vec2( - 1.0, -1.0 ), 0.5, 1.0 );	
	
	g_vNormal = NormalTransform * normal;
	
	vec4 vPositionWS = matTransform * position;
    g_vLightVec = vLightPos - vPositionWS.xyz;
	
}
