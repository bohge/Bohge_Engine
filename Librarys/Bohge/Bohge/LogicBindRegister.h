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



/////////////////////////
//		注册碰撞库	   //
/////////////////////////
#pragma once
#include "ScriptBase.h"
#include "Plane.hpp"
#include "Frustum.hpp"
#include "Ray.h"
#include "aabsphere.hpp"
#include "aabround.hpp"
#include "aabrect.hpp"
#include "aabbox.hpp"





namespace BohgeEngine
{
	template < typename T >
	void RegisterPlaneT( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<Plane<T> >( classname );
		//注册构造函数
		s->RegClassCons<Plane<T> >();
		//注册table成员函数
		//操作符
		s->RegClassFunc<  Plane<T>, Plane<T>& (Plane<T>::*)( vector4<T> const &)  >( "EqualVector4", &Plane<T>::operator= );
		s->RegClassFunc<  Plane<T>  >( "isEqual", &Plane<T>::operator== );
		//函数
		s->RegClassFunc< Plane<T>  >( "A", &Plane<T>::A );
		s->RegClassFunc< Plane<T>  >( "B", &Plane<T>::B );
		s->RegClassFunc< Plane<T>  >( "C", &Plane<T>::C );
		s->RegClassFunc< Plane<T>  >( "D", &Plane<T>::D );
		s->RegClassFunc< Plane<T>  >( "FromPointNormal", &Plane<T>::FromPointNormal );
		s->RegClassFunc< Plane<T>  >( "FromPoints", &Plane<T>::FromPoints );
		s->RegClassFunc< Plane<T>  >( "Normal", &Plane<T>::Normal );
		s->RegClassFunc< Plane<T>  >( "IntersectRay", &Plane<T>::IntersectRay );
		s->RegClassFunc< Plane<T>  >( "DistanceTo", &Plane<T>::DistanceTo );
		s->RegClassFunc< Plane<T>  >( "Dot", &Plane<T>::Dot );
		s->RegClassFunc< Plane<T>  >( "NormalizeSelf", &Plane<T>::NormalizeSelf );
		s->RegClassFunc< Plane<T>  >( "GetSide", &Plane<T>::GetSide );

		//注册GetSide的返回值
		s->RegTable<OnSide >("OnSide");
		s->SetTableVolume<OnSide >( "OS_BOTH_SIDE", OnSide::OS_NEGATIVE_SIDE );
		s->SetTableVolume<OnSide >( "OS_POSITIVE_SIDE", OnSide::OS_POSITIVE_SIDE );
		s->SetTableVolume<OnSide >( "OS_BOTH_SIDE", OnSide::OS_BOTH_SIDE );
	}



	template < typename T >
	void RegisterFrustumT( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<Frustum<T> >( classname );
		//注册构造函数
		s->RegClassCons<Frustum<T> >();
		//注册table成员函数
		//函数
		s->RegClassFunc< Frustum<T>  >( "GetPlane", &Frustum<T>::GetPlane );
		s->RegClassFunc< Frustum<T>  >( "FromMatrix", &Frustum<T>::FromMatrix );
		s->RegClassFunc< Frustum<T>, IntersectedType::Volume (Frustum<T>::*)(aabbox3d<T> const &) const  >( "Intersect", &Frustum<T>::Intersect );
		s->RegClassFunc< Frustum<T>, IntersectedType::Volume (Frustum<T>::*)(vector3<T> const &) const  >( "Intersect", &Frustum<T>::Intersect );

		s->RegTable<IntersectedType >("IntersectedType");
		s->SetTableVolume<IntersectedType >( "IT_NO", IntersectedType::IT_NO );
		s->SetTableVolume<IntersectedType >( "IT_PARTIAL", IntersectedType::IT_PARTIAL );
		s->SetTableVolume<IntersectedType >( "IT_FULL", IntersectedType::IT_FULL );
	}



	template < typename T >
	void RegisterRayT( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<Ray<T> >( classname );
		//注册构造函数
		s->RegClassCons<Ray<T> >();
		//注册table成员函数
		//函数
		s->RegClassFunc< Ray<T>, Ray<T> (Ray<T>::*)(const Quaternion<T>&) const >( "MultiplyQuaternion", &Ray<T>::operator* );
		s->RegClassFunc< Ray<T>, const Ray<T>& (Ray<T>::*)(const Quaternion<T>&) >( "MultiplyEqualQuaternion", &Ray<T>::operator *= );
		s->RegClassFunc< Ray<T> >( "SetRayOrigin", &Ray<T>::SetRayOrigin );
		s->RegClassFunc< Ray<T> >( "SetRayDirection", &Ray<T>::SetRayDirection );
		s->RegClassFunc< Ray<T> >( "GetRayOrigin", &Ray<T>::GetRayOrigin );
		s->RegClassFunc< Ray<T> >( "GetRayDirection", &Ray<T>::GetRayDirection );
	}


	template < typename T >
	void RegisterAabboxT( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<aabbox3d<T> >( classname );
		//注册构造函数
		s->RegClassCons<aabbox3d<T> >();
		//注册table成员函数
		//函数
		s->RegClassFunc< aabbox3d<T> >( "Reset", &aabbox3d<T>::Reset );
		s->RegClassFunc< aabbox3d<T> >( "GetMin", &aabbox3d<T>::GetMin );
		s->RegClassFunc< aabbox3d<T> >( "GetMax", &aabbox3d<T>::GetMax );
		s->RegClassFunc< aabbox3d<T> >( "SetMin", &aabbox3d<T>::SetMin );
		s->RegClassFunc< aabbox3d<T> >( "SetMax", &aabbox3d<T>::SetMax );
		s->RegClassFunc< aabbox3d<T> >( "SetCenter", &aabbox3d<T>::SetCenter );
		s->RegClassFunc< aabbox3d<T> >( "GetCenter", &aabbox3d<T>::GetCenter );
		s->RegClassFunc< aabbox3d<T> >( "GetHalfSize", &aabbox3d<T>::GetHalfSize );
		s->RegClassFunc< aabbox3d<T> >( "MaxEdge", &aabbox3d<T>::MaxEdge );
		s->RegClassFunc< aabbox3d<T>, aabbox3d<T> (aabbox3d<T>::*)(const Matrix44<T>&) const >( "MultiplyMatrix44", &aabbox3d<T>::operator* );
		s->RegClassFunc< aabbox3d<T>, aabbox3d<T> (aabbox3d<T>::*)(const vector3<T>&) const >( "MultiplyScale", &aabbox3d<T>::operator* );
		s->RegClassFunc< aabbox3d<T>, const aabbox3d<T>& (aabbox3d<T>::*)(const T&) >( "MultiplyEqualNumber", &aabbox3d<T>::operator *= );
		s->RegClassFunc< aabbox3d<T>, const aabbox3d<T>& (aabbox3d<T>::*)(const Matrix44<T>&) >( "MultiplyEqualMatrix44", &aabbox3d<T>::operator *= );
		s->RegClassFunc< aabbox3d<T>, void (aabbox3d<T>::*)(const vector3<T>&) >( "ContainsVector3", &aabbox3d<T>::Contains );
		s->RegClassFunc< aabbox3d<T>, void (aabbox3d<T>::*)(const aabbox3d<T>&) >( "ContainsAabbox3d", &aabbox3d<T>::Contains );
		s->RegClassFunc< aabbox3d<T>, bool (aabbox3d<T>::*)(const vector3<T>&) const >( "IntersectionVector3", &aabbox3d<T>::Intersection );
		s->RegClassFunc< aabbox3d<T>, bool (aabbox3d<T>::*)(const Ray<T>&) const >( "IntersectionRay", &aabbox3d<T>::Intersection );
		s->RegClassFunc< aabbox3d<T>, bool (aabbox3d<T>::*)(const aabsphere<T>&) const >( "IntersectionAabsphere", &aabbox3d<T>::Intersection );
		s->RegClassFunc< aabbox3d<T>, bool (aabbox3d<T>::*)(const aabbox3d<T>&) const >( "IntersectionAabbox3d", &aabbox3d<T>::Intersection );
		s->RegClassFunc< aabbox3d<T> >( "GetPoint", &aabbox3d<T>::GetPoint );
		s->RegClassFunc< aabbox3d<T> >( "GetPlane", &aabbox3d<T>::GetPlane );
		s->RegClassFunc< aabbox3d<T> >( "IntersectPointWithRay", &aabbox3d<T>::IntersectPointWithRay );
		s->RegClassFunc< aabbox3d<T> >( "GetPlane", &aabbox3d<T>::GetPlane );
	}




	template < typename T >
	void RegisterAabrectT( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<aabrect<T> >( classname );
		//注册构造函数
		s->RegClassCons<aabrect<T> >();
		//注册table成员函数
		//函数
		s->RegClassFunc< aabrect<T>  >( "LeftBottom", &aabrect<T>::LeftBottom );
		s->RegClassFunc< aabrect<T>  >( "RightTop", &aabrect<T>::RightTop );
		s->RegClassFunc< aabrect<T>  >( "SetCenter", &aabrect<T>::SetCenter );
		s->RegClassFunc< aabrect<T>, void (aabrect<T>::*)(const aabrect<T>&)  >( "Contains", &aabrect<T>::Contains );
		s->RegClassFunc< aabrect<T>, void (aabrect<T>::*)(const vector2<T>&)   >( "Contains", &aabrect<T>::Contains );
		s->RegClassFunc< aabrect<T>, aabrect<T> (aabrect<T>::*)(T)  >( "MultiplyNumber", &aabrect<T>::operator * );
		s->RegClassFunc< aabrect<T>, aabrect<T> (aabrect<T>::*)(const vector2<T>&)  >( "MultiplyVector2", &aabrect<T>::operator * );
		s->RegClassFunc< aabrect<T>, bool (aabrect<T>::*)(const aabrect<T>&) const  >( "isInsertAabrect", &aabrect<T>::isInsert );
		s->RegClassFunc< aabrect<T>, bool (aabrect<T>::*)(const vector2<T>&) const  >( "isInsertVector2", &aabrect<T>::isInsert );
		s->RegClassFunc< aabrect<T>  >( "GetCenter", &aabrect<T>::GetCenter );
		s->RegClassFunc< aabrect<T>  >( "GetSize", &aabrect<T>::GetSize );
		s->RegClassFunc< aabrect<T>  >( "GetRect", &aabrect<T>::GetRect );
	}




	template < typename T >
	void RegisterAabsphereT( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<aabsphere<T> >( classname );
		//注册构造函数
		s->RegClassCons<aabsphere<T> >();
		//注册table成员函数
		//函数
		s->RegClassFunc< aabsphere<T>  >( "MoveOn", &aabsphere<T>::MoveOn );
		s->RegClassFunc< aabsphere<T>  >( "Scale", &aabsphere<T>::Scale );
		s->RegClassFunc< aabsphere<T>  >( "SetCenter", &aabsphere<T>::SetCenter );
		s->RegClassFunc< aabsphere<T>  >( "GetCenter", &aabsphere<T>::GetCenter );
		s->RegClassFunc< aabsphere<T>  >( "SetRadius", &aabsphere<T>::SetRadius );
		s->RegClassFunc< aabsphere<T>  >( "GetRadius", &aabsphere<T>::GetRadius );
		s->RegClassFunc< aabsphere<T>  >( "IsEmpty", &aabsphere<T>::IsEmpty );
		s->RegClassFunc< aabsphere<T>  >( "VecInBound", &aabsphere<T>::VecInBound );
		s->RegClassFunc< aabsphere<T>  >( "MaxRadiusSq", &aabsphere<T>::MaxRadiusSq );
		s->RegClassFunc< aabsphere<T>  >( "Intersect", &aabsphere<T>::Intersect );
	}

}