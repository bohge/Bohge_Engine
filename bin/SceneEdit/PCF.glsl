//PCF
#define SHADOW_OFFSET 0.0//0.00001
uniform float ShadowTextureSizeInv;


const highp int PCFStep = 9;
const highp float fShadowVolume = 1.0 / ( float(PCFStep) * float(PCFStep) );
const mediump float halfStep = float(PCFStep-1)*0.5;
lowp float PCFShadow( highp float org_depth, highp vec4 tex_coord, sampler2D ShadowMap )
{
	if( tex_coord.x < 0.0 || tex_coord.y < 0.0
		|| tex_coord.x > 1.0 || tex_coord.y > 1.0 )
	{
		return 1.0;
	}
	
	highp float shadow = 0.0;
	for( int x = 0; x < PCFStep ; x ++ )
	{
		for( int y = 0; y < PCFStep ; y ++ )
		{
			vec2 localCoord = tex_coord.xy;
			localCoord.x += ( -halfStep + float(x) ) * ShadowTextureSizeInv;
			localCoord.y += ( -halfStep + float(y) ) * ShadowTextureSizeInv;
			highp float depth = unpackFloatFromVec( texture2D( ShadowMap, localCoord.xy ) );
			if( org_depth <= depth )
			{
				shadow += fShadowVolume;
			}
		}
	}
	return clamp( shadow, 0.0, 1.0);
}


lowp float PCFShadowDepth( highp float org_depth, highp vec4 tex_coord, sampler2D ShadowMap )
{
	if( tex_coord.x < 0.0 || tex_coord.y < 0.0
		|| tex_coord.x > 1.0 || tex_coord.y > 1.0 )
	{
		return 1.0;
	}
	
	highp float shadow = 0.0;
	for( int x = 0; x < PCFStep ; x ++ )
	{
		for( int y = 0; y < PCFStep ; y ++ )
		{
			vec2 localCoord = tex_coord.xy;
			localCoord.x += ( -halfStep + float(x) ) * ShadowTextureSizeInv;
			localCoord.y += ( -halfStep + float(y) ) * ShadowTextureSizeInv;
			highp float depth = texture2D( ShadowMap, localCoord.xy ).r * 2.0 - 1.0;
			if( org_depth <= depth + SHADOW_OFFSET )
			{
				shadow += fShadowVolume;
			}
		}
	}
	return clamp( shadow, 0.0, 1.0);
}

lowp float PCFPointLightShadow( highp float org_depth, highp vec3 tex_coord, samplerCube ShadowMap )
{
	highp float shadow = 0.0;
	for( int x = 0; x < PCFStep ; x ++ )
	{
		for( int y = 0; y < PCFStep ; y ++ )
		{
			vec3 localCoord = tex_coord;
			localCoord.x += ( -halfStep + float(x) ) * ShadowTextureSizeInv;
			//localCoord.y += ( -halfStep + float(y) ) * ShadowTextureSizeInv;
			localCoord.z += ( -halfStep + float(y) ) * ShadowTextureSizeInv;
			highp float depth = unpackFloatFromVec( textureCube( ShadowMap, localCoord ) );
			if( org_depth < depth + SHADOW_OFFSET )
			{
				shadow += fShadowVolume;
			}
		}
	}
	return clamp( shadow, 0.0, 1.0);
}