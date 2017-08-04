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

//////////////////////////////////////////////
//	3DMath的拆分实现，请不要单独引用该头文件	//
//////////////////////////////////////////////
#ifndef SPHEREMATHDECLAR
#define SPHEREMATHDECLAR
#else
#error( "Do not include this file in other files" )
#endif // SPHEREMATHIMPLEMENT

#include "Predefine.h"
#include "VariableType.h"

namespace BohgeEngine
{
	struct OnSide
	{
		enum Volume
		{
			OS_NEGATIVE_SIDE = 0,
			OS_POSITIVE_SIDE,
			OS_BOTH_SIDE,
		};
	};

	struct IntersectedType
	{
		enum Volume
		{
			IT_NO = 0,//不想交
			IT_PARTIAL,//相交一部分
			IT_FULL,//完全不想交
		};
	};

	template<typename T>class Ray;
	template<typename T>class Line;
	template<typename T>class Plane;
	template<typename T>class Frustum;
	template<typename T>class Aabbox3d;
	template<typename T>class Aabrect;
	template<typename T>class Aabround;
	template<typename T>class Aabsphere;


	typedef Ray<int> Rayd;
	typedef Ray<float> Rayf;
	typedef Ray<double> Rayb;

	typedef Line<int> Lined;
	typedef Line<float> Linef;
	typedef Line<double> Lineb;

	typedef Plane<int> Planed;
	typedef Plane<float> Planef;
	typedef Plane<double> Planeb;

	typedef Frustum<int> Frustumd;
	typedef Frustum<float> Frustumf;
	typedef Frustum<double> Frustumb;

	typedef Aabbox3d<int> Aabbox3dd;
	typedef Aabbox3d<float> Aabbox3df;
	typedef Aabbox3d<double> Aabbox3db;

	typedef Aabrect<int> Aabrectd;
	typedef Aabrect<float> Aabrectf;
	typedef Aabrect<double> Aabrectb;

	typedef Aabround<int> Aabroundd;
	typedef Aabround<float> Aabroundf;
	typedef Aabround<double> Aabroundb;

	typedef Aabsphere<int> Aabsphered;
	typedef Aabsphere<float> Aabspheref;
	typedef Aabsphere<double> Aabsphereb;

	namespace Math
	{
		//图形相关的
		template<typename T>
		BOHGE_FORCEINLINE T Dot(const Plane<T>& v1, const vector3<T>& v2);
		template<typename T>
		BOHGE_FORCEINLINE IntersectedType::Volume Aabbox3d_Aabbox3d_IntersectType(const Aabbox3d<T>& lhs, const Aabbox3d<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d_Aabbox3d_Intersect(const Aabbox3d<T>& lhs, const Aabbox3d<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d_Vector3_Intersect(const Aabbox3d<T>& lhs, const vector3<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d_Ray_Intersect(const Aabbox3d<T>& lhs, const Ray<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d_Line_Intersect(const Aabbox3d<T>& lhs, const Line<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d__Aabsphere_Intersect(const Aabbox3d<T>& lhs, const Aabsphere<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d_Ray_IntersectPoint(const Aabbox3d<T>& lhs, const Aabsphere<T>& rhs, vector3<T>& point);
		template<typename T>
		BOHGE_FORCEINLINE Aabrect<T> Aabrect_From_CenterSize(const vector2<T>& center, const vector2<T>& size);;
		template<typename T>
		BOHGE_FORCEINLINE bool Aabrect_Vector2_Intersect(const Aabrect<T>& lhs, const vector2<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Aabrect_Aabrect_Intersect(const Aabrect<T>& lhs, const Aabrect<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Aabround_Vector2_Intersect(const Aabround<T>& lhs, const vector2<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Aabsphere_Aabsphere_Intersect(const Aabsphere<T>& lhs, const Aabsphere<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Aabsphere_Vector3_Intersect(const Aabsphere<T>& lhs, const vector3<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE IntersectedType::Volume Frustum_Aabbox3d_IntersectType(const Frustum<T>& lhs, const Aabbox3d<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE IntersectedType::Volume Frustum_Frustum_IntersectType(const Frustum<T>& lhs, const Frustum<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Frustum_vector3_Intersect(const Frustum<T>& lhs, const vector3<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE bool Frustum_Aabbox3d_Intersect(const Frustum<T>& lhs, const Aabbox3d<T>& rhs);
		template<typename T>
		BOHGE_FORCEINLINE vector3<T> Plane_Ray_IntersectPoint(const Plane<T>& lhs, const Ray<T>& rhs);
	}
}