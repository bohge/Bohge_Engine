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



///////////////////////
//	shader内部的参数	 //
///////////////////////
#pragma once
#include "Predefine.h"


#include <string>
#include <map>

namespace BohgeEngine
{
	class ShaderLibrary;
	class ShaderParameter//这个类不能有指针，有指针的话，需要实现拷贝构造和赋值函数
	{
	public:
		enum FloatPrecision
		{
			FP_ERRORCODE = 0,//Lua得不到数据的时候会设置数据为0
			FP_NONE,
			FP_LOW,
			FP_MEDIUM,
			FP_HIGH,
		};
		enum VariablesType
		{
			VT_ERRORCODE = 0,//Lua得不到数据的时候会设置数据为0
			VT_FLOAT,
			VT_VEC2,
			VT_VEC3,
			VT_VEC4,
			VT_VEC2_ARRAY,
			VT_VEC3_ARRAY,
			VT_VEC4_ARRAY,
			VT_MAT3,
			VT_MAT4,
			VT_TEXTURE1D,
			VT_TEXTURE2D,
			VT_TEXTURE3D,
			VT_TEXTURECUBE,
		};
		enum InputsType//输出的参数类型
		{
			//顶点属性
			IT_ERRORCODE = 0,//Lua得不到数据的时候会设置数据为0
			IT_LOCALSPACE_POSITION,//原始位置
			IT_LOCALSPACE_COORDNATE,//原始纹理坐标,一般使用这个采样，这个一般就是输入的数据
			IT_LOCALSPACE_NORMAL,//原始发现
			IT_LOCALSPACE_TANGENT,//原始切线
			IT_LOCALSPACE_BINORMAL,//原始副法线
			IT_LOCALSPACE_BONE_INEX,//原始骨骼索引
			IT_LOCALSPACE_BONE_WEIGHT,//原始骨骼权重

			IT_WORLDSPACE_POSITION,//世界坐标下的位置
			IT_WORLDSPACE_NORMAL,//世界坐标下的发现
			IT_WORLDSPACE_TANGENT,//世界坐标下的切线
			IT_WORLDSPACE_BINORMAL,//世界坐标下的副法线
			IT_WORLDSPACE_VIEWDIRECTION,//世界坐标下的视觉射线方向

			IT_SCREENSPACE_POSITION,//屏幕空间位置[-1,1]
			IT_SCREENSPACE_COORDNATE,//屏幕空间纹理坐标，铺满整个屏幕[0,1]
			IT_SCREENSPACE_NORMAL,//屏幕空间的法线，一般没用
			IT_SCREENSPACE_TANGENT,//屏幕空间的切线，一般没用
			IT_SCREENSPACE_BINORMAL,//屏幕空间的副法线，一般没用

			IT_WORLDSPACE_LIGHT_DISTANCE,//光源和顶点的距离
			IT_WORLDSPACE_DEPTH,//深度

			IT_USER_ATTRIBUTE_0,//自定义定点流
			IT_USER_ATTRIBUTE_1,
			IT_USER_ATTRIBUTE_2,
			IT_USER_ATTRIBUTE_3,
			IT_USER_ATTRIBUTE_4,
			IT_USER_ATTRIBUTE_5,
			IT_USER_ATTRIBUTE_6,
			IT_USER_ATTRIBUTE_7,

			//模型寄存器
			IT_LOCALSPACE_REAL_ARRAY,//骨骼数组
			IT_LOCALSPACE_DUAL_ARRAY,//骨骼数组
			IT_LOCALWORLD_TRANSFORM,//本地到世界的变换矩阵
			IT_LOCALWORLD_ROTATION,//本地的旋转矩阵
			IT_LOCALSCREEN_TRANSVIEWPROJ,//本地到屏幕的mvp

			//摄像机寄存器数据
			IT_CAMERA_WORLDPOSITION,//摄像机位置
			IT_CAMERA_WORLDDIRECTION,//摄像机朝向
			IT_CAMERA_VIEW,//摄像机view矩阵
			IT_CAMERA_PROJECTION,//摄像机投影
			IT_CAMERA_VIEWPROJ,//摄像机vp矩阵
			IT_CAMERA_LINERPARAM,//摄像机线性化参数

			//光源的属性
			IT_LIGHT_POSITION,
			IT_LIGHT_COLOR,
			IT_AMBIENT_COLOR,
			IT_LOCALSPACE_LIGHTDIRECTION,//局部坐标下光照的方向，这个一般就是世界的
			IT_WORLDSPACE_LIGHTDIRECTION,//世界坐标下的光源方向

			//材质接受光照强度
			IT_MATERIAL_AMBIENT,//环境光
			IT_MATERIAL_DIFFUSE,//满反射
			IT_MATERIAL_SPECULAR,//反射
			IT_MATERIAL_SPECULAR_LEVEL,//反射强度
			IT_MATERIAL_SHININESS,//反射面积

			//光照和材质的复合型参数，就是把数据都相乘，提供ps使用，一般使用这个东西
			IT_LIGHT_MATERIAL_AMBIENT,//环境光
			IT_LIGHT_MATERIAL_LIGHT,//满反射颜色
			IT_LIGHT_MATERIAL_SPECULAR,//反射

			//像素作色器中数据
			IT_SURFACE_COLOR,//表面颜色

			//纹理
			IT_TEXTURE_AMBIENT,//环境贴图
			IT_TEXTURE_DIFFUSE,//纹理属性
			IT_TEXTURE_SPECULAR,//反射颜色
			IT_TEXTURE_SPLEVEL,//发射等级
			IT_TEXTURE_GLOSSINESS,//发光
			IT_TEXTURE_ILLUMINATION,//自发光
			IT_TEXTURE_OPACITY,//透明贴图
			IT_TEXTURE_FITER,//过滤贴图
			IT_TEXTURE_BUMP,//法线贴图
			IT_TEXTURE_REFLECTION,//发射贴图
			IT_TEXTURE_REFRACTION,//折射贴图
			IT_TEXTURE_DISPLACEMENT,//视察贴图
			IT_TEXTURE_TERRAIN_WEIGHT,//地形的贴图
			IT_TEXTURE_TERRAIN_DETAL,
			IT_TEXTURE_TERRAIN_BASE,
			IT_TEXTURE_TERRAIN_R,
			IT_TEXTURE_TERRAIN_G,
			IT_TEXTURE_TERRAIN_B,
			IT_TEXTURE_USER0,//用户自定义的
			IT_TEXTURE_USER1,//用户自定义的
			IT_TEXTURE_USER2,//用户自定义的
			IT_TEXTURE_USER3,//用户自定义的
			IT_TEXTURE_USER4,//用户自定义的
			IT_TEXTURE_USER5,//用户自定义的
			IT_TEXTURE_USER6,//用户自定义的
			IT_TEXTURE_USER7,//用户自定义的
		};
	private:
		VariablesType	m_eType;
		FloatPrecision	m_ePrecision;
		InputsType		m_eInputType;
		std::string		m_ParamName;//shader中的名字
	public:
		ShaderParameter( const InputsType& it, const FloatPrecision& fp, const VariablesType& vt, const std::string& name );
	public:
		std::string AsParameterString() const;//生成一个shader的输入流或者寄存器参数字符串
		std::string AsVaryingString( const std::string& name ) const;//生成一个顶点输入属性
		std::string AsVariables() const;//输出一个变量
		bool isUniform() const;
	public:
		static void RegisterShaderParameter( ShaderLibrary& library );
	public:
		BOHGE_FORCEINLINE const std::string& GetName() const
		{
			return m_ParamName;
		}
		BOHGE_FORCEINLINE void SetName( const std::string& to )
		{
			m_ParamName = to;
		}
		BOHGE_FORCEINLINE const InputsType& GetInputsType() const
		{
			return m_eInputType;
		}
		BOHGE_FORCEINLINE const VariablesType& GetVariableType() const
		{
			return m_eType;
		}
	};


	typedef std::map< ShaderParameter::InputsType, ShaderParameter*> ParameterMap;
}