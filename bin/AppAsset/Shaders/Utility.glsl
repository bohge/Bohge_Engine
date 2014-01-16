//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



#define saturate(x) clamp( x, 0.0, 1.0 )
#define lerp        mix

//雾相关
const highp float LOG2 = 1.442695;
highp float fogFactor( highp float len, highp vec3 FogParam )
{
	highp float d = len * FogParam.x;
	return clamp( exp2( - FogParam.y * FogParam.y * d * d * LOG2 ), FogParam.z, 1.0 );
}

lowp vec3 FogEndColor( lowp vec3 FogClr, lowp vec3 OgrClr, highp float factor )
{
	return mix( FogClr, OgrClr, factor );
}


//解包深度
#ifdef _WINDOWS_
highp float unpackFloatFromVec(const highp vec4 value)
{
	const highp vec4 bitShifts = vec4(1.0,
                    1.0 / 256.0,
                    1.0 / 65536.0,
                    1.0 / 16777216.0);
	return dot(value, bitShifts);
}
#else
highp float unpackFloatFromVec(const highp vec4 value)
{
	const highp vec3 bitShifts = vec3(1.0,
                    1.0 / 256.0,
                    1.0 / 65536.0);
	return dot(value.xyz, bitShifts);
}
#endif


//打包深读
#ifdef _WINDOWS_
lowp vec4 packFloatToVec(const highp float value)
{
    const highp vec4 bias = vec4(1.0 / 255.0,
                1.0 / 255.0,
                1.0 / 255.0,
                0.0);

    lowp float r = value;
    lowp float g = fract(value * 256.0);
    lowp float b = fract(value * 65536.0);
    lowp float a = fract(value * 16777216.0);
    highp vec4 colour = vec4(r, g, b, a);
    
    return colour - (colour.yzww * bias);
}
#else
lowp vec4 packFloatToVec(const highp float value)
{
    const highp float bias = 1.0 / 255.0;

    lowp float r = value;
    lowp float g = fract(value * 256.0);
    lowp float b = fract(value * 65536.0);
    lowp vec3 colour = vec3(r, g, b);
    
    return vec4( colour - (colour.yzz * bias), 0.0 );
}
#endif

//ESM阴影
const mediump float constant = 96.0;
lowp float ESMShadowDirectionLight( highp float org_depth, highp vec4 tex_coord, sampler2D ShadowMap )
{
	/*
	lowp vec2 isShadow = tex_coord.xy - 0.5;
	isShadow = abs(isShadow);
	isShadow = clamp((0.5-isShadow)*100.0, 0, 1);
	lowp float shadowcolor = isShadow.x * isShadow.y;
	highp float tex_depth = unpackFloatFromVec( texture2DProj( ShadowMap, tex_coord ) );
	return 1.0 - shadowcolor * clamp(exp(constant * ( org_depth - tex_depth ) ), 0.0, 1.0);

	if( tex_coord.x < 0.0 || tex_coord.y < 0.0
		|| tex_coord.x > 1.0 || tex_coord.y > 1.0 )
	{
		return 1.0;
	}*/
	//highp float tex_depth = unpackFloatFromVec( texture2D( ShadowMap, tex_coord.xy ) );
	highp float tex_depth = unpackFloatFromVec( texture2DProj( ShadowMap, tex_coord ) );
	//return clamp(exp(constant * ( tex_depth - org_depth ) ), 0.0, 1.0);
	return clamp((tex_depth - org_depth)*2.0, 0.0, 1.0);
}
const mediump float Pointconstant = 8.0;
lowp float ESMShadowPointLight( highp float org_depth, highp vec3 tex_coord, samplerCube ShadowMap )
{
	highp float tex_depth = unpackFloatFromVec( textureCube( ShadowMap, tex_coord ) );
	return clamp(exp(Pointconstant * ( tex_depth - org_depth ) ), 0.0, 1.0);
}
const mediump float Spotconstant = 8.0;
lowp float ESMShadowSoptLight( highp float org_depth, highp vec4 tex_coord, sampler2D ShadowMap )
{
	highp float tex_depth = unpackFloatFromVec( texture2DProj( ShadowMap, tex_coord ) );
	return clamp(exp(Spotconstant * ( tex_depth - org_depth ) ), 0.0, 1.0);
}
