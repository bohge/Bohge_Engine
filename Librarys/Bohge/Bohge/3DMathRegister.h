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
//		注册数学库	   //
/////////////////////////
#pragma once
#include "3DMath.h"
#include "ScriptBase.h"


namespace BohgeEngine
{
	template < typename T >
	void RegisterVector2T( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass< vector2<T>  >( classname );
		//注册构造函数
		s->RegClassCons< vector2<T>, T, T >();
		//注册类成员变量
		s->RegClassVar< vector2<T>  >( "m_x", &vector2<T>::m_x );
		s->RegClassVar< vector2<T>  >( "m_y", &vector2<T>::m_y );
		//注册table成员函数
		//操作符
		s->RegClassFunc< vector2<T>, vector2<T>& (vector2<T>::*)(const vector2<T>&) >( "Equal", &vector2<T>::operator= );
		s->RegClassFunc< vector2<T>, vector2<T> (vector2<T>::*)(const vector2<T>&) const >( "AddVector", &vector2<T>::operator+ );
		s->RegClassFunc< vector2<T>, vector2<T> (vector2<T>::*)(const vector2<T>&) const >( "MinusVector", &vector2<T>::operator- );
		s->RegClassFunc< vector2<T>, vector2<T> (vector2<T>::*)(const vector2<T>&) const >( "MultiplyVector", &vector2<T>::operator* );
		s->RegClassFunc< vector2<T>, vector2<T> (vector2<T>::*)(const vector2<T>&) const >( "DivideVector", &vector2<T>::operator/ );
		s->RegClassFunc< vector2<T>, vector2<T>& (vector2<T>::*)(const vector2<T>&) >( "AddEqualVector", &vector2<T>::operator+= );
		s->RegClassFunc< vector2<T>, vector2<T>& (vector2<T>::*)(const vector2<T>&) >( "MinusEqualVector", &vector2<T>::operator-= );
		s->RegClassFunc< vector2<T>, vector2<T>& (vector2<T>::*)(const vector2<T>&) >( "MultiplyEqualVector", &vector2<T>::operator*= );
		s->RegClassFunc< vector2<T>, vector2<T>& (vector2<T>::*)(const vector2<T>&) >( "DivideEqualVector", &vector2<T>::operator/= );
		s->RegClassFunc< vector2<T>, vector2<T> (vector2<T>::*)(T) const >( "AddNumber", &vector2<T>::operator+ );
		s->RegClassFunc< vector2<T>, vector2<T> (vector2<T>::*)(T) const >( "MinusNumber", &vector2<T>::operator- );
		//s->RegClassFunc< vector2<T>, vector2<T> (vector2<T>::*)(T) const >( "MultiplyNumber", &vector2<T>::operator* );
		s->RegClassFunc< vector2<T>, vector2<T> (vector2<T>::*)(T) const >( "DivideNumber", &vector2<T>::operator/ );
		s->RegClassFunc< vector2<T>, vector2<T>& (vector2<T>::*)(T) >( "AddEqualNumber", &vector2<T>::operator+= );
		s->RegClassFunc< vector2<T>, vector2<T>& (vector2<T>::*)(T) >( "MinusEqualNumber", &vector2<T>::operator-= );
		s->RegClassFunc< vector2<T>, vector2<T>& (vector2<T>::*)(T) >( "MultiplyEqualNumber", &vector2<T>::operator*= );
		s->RegClassFunc< vector2<T>, vector2<T>& (vector2<T>::*)(T) >( "DivideEqualNumber", &vector2<T>::operator/= );
		s->RegClassFunc< vector2<T>, bool (vector2<T>::*)(const vector2<T>&) const >( "isEqualVector", &vector2<T>::operator== );
		s->RegClassFunc< vector2<T>, bool (vector2<T>::*)(const vector2<T>&) const >( "NotEqualVector", &vector2<T>::operator!= );
		//函数
		s->RegClassFunc< vector2<T> >( "Dot", &vector2<T>::Dot );
		s->RegClassFunc< vector2<T> >( "NormalizeSelf", &vector2<T>::NormalizeSelf );
		s->RegClassFunc< vector2<T> >( "Normalize", &vector2<T>::Normalize );
		s->RegClassFunc< vector2<T> >( "Length", &vector2<T>::Length );
		s->RegClassFunc< vector2<T> >( "LengthPow", &vector2<T>::LengthPow );
	}

	template < typename T >
	void RegisterVector3T( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass< vector3<T> >( classname );
		//注册构造函数
		s->RegClassCons< vector3<T>, T, T, T  >();
		//注册类成员变量
		s->RegClassVar< vector3<T> >( "m_x", &vector3<T>::m_x );
		s->RegClassVar< vector3<T> >( "m_y", &vector3<T>::m_y );
		s->RegClassVar< vector3<T> >( "m_z", &vector3<T>::m_z );
		//注册table成员函数
		//操作符
		s->RegClassFunc< vector3<T>, vector3<T>& (vector3<T>::*)(const vector3<T>&) >( "Equal", &vector3<T>::operator= );
		s->RegClassFunc< vector3<T>, vector3<T> (vector3<T>::*)(const vector3<T>&) const >( "AddVector", &vector3<T>::operator+ );
		s->RegClassFunc< vector3<T>, vector3<T> (vector3<T>::*)(const vector3<T>&) const >( "MinusVector", &vector3<T>::operator- );
		s->RegClassFunc< vector3<T>, vector3<T> (vector3<T>::*)(const vector3<T>&) const >( "MultiplyVector", &vector3<T>::operator* );
		s->RegClassFunc< vector3<T>, vector3<T> (vector3<T>::*)(const vector3<T>&) const >( "DivideVector", &vector3<T>::operator/ );
		s->RegClassFunc< vector3<T>, vector3<T>& (vector3<T>::*)(const vector3<T>&) >( "AddEqualVector", &vector3<T>::operator+= );
		s->RegClassFunc< vector3<T>, vector3<T>& (vector3<T>::*)(const vector3<T>&) >( "MinusEqualVector", &vector3<T>::operator-= );
		s->RegClassFunc< vector3<T>, vector3<T>& (vector3<T>::*)(const vector3<T>&) >( "MultiplyEqualVector", &vector3<T>::operator*= );
		s->RegClassFunc< vector3<T>, vector3<T>& (vector3<T>::*)(const vector3<T>&) >( "DivideEqualVector", &vector3<T>::operator/= );
		s->RegClassFunc< vector3<T>, vector3<T> (vector3<T>::*)(T) const >( "AddNumber", &vector3<T>::operator+ );
		s->RegClassFunc< vector3<T>, vector3<T> (vector3<T>::*)(T) const >( "MinusNumber", &vector3<T>::operator- );
		//s->RegClassFunc< vector3<T>, vector3<T> (vector3<T>::*)(T) const >( "MultiplyNumber", &vector3<T>::operator* );
		s->RegClassFunc< vector3<T>, vector3<T> (vector3<T>::*)(T) const >( "DivideNumber", &vector3<T>::operator/ );
		s->RegClassFunc< vector3<T>, vector3<T>& (vector3<T>::*)(T) >( "AddEqualNumber", &vector3<T>::operator+= );
		s->RegClassFunc< vector3<T>, vector3<T>& (vector3<T>::*)(T) >( "MinusEqualNumber", &vector3<T>::operator-= );
		s->RegClassFunc< vector3<T>, vector3<T>& (vector3<T>::*)(T) >( "MultiplyEqualNumber", &vector3<T>::operator*= );
		s->RegClassFunc< vector3<T>, vector3<T>& (vector3<T>::*)(T) >( "DivideEqualNumber", &vector3<T>::operator/= );
		s->RegClassFunc< vector3<T>, bool (vector3<T>::*)(const vector3<T>&) const >( "isEqualVector", &vector3<T>::operator== );
		s->RegClassFunc< vector3<T>, bool (vector3<T>::*)(const vector3<T>&) const >( "NotEqualVector", &vector3<T>::operator!= );
		s->RegClassFunc< vector3<T>, vector3<T> (vector3<T>::*)(const Matrix33<T>&) const >( "MultiplyMatrix33", &vector3<T>::operator* );
		s->RegClassFunc< vector3<T>, vector3<T>& (vector3<T>::*)(const Matrix33<T>&) >( "MultiplyEqualMatrix33", &vector3<T>::operator*= );
		s->RegClassFunc< vector3<T>, vector3<T> (vector3<T>::*)(const Quaternion<T>&) const >( "MultiplyQuaternion", &vector3<T>::operator* );
		s->RegClassFunc< vector3<T>, vector3<T>& (vector3<T>::*)(const Quaternion<T>&) >( "MultiplyEqualQuaternion", &vector3<T>::operator*= );
		//函数
		s->RegClassFunc< vector3<T> >( "Cross", &vector3<T>::Cross );
		s->RegClassFunc< vector3<T> >( "Dot", &vector3<T>::Dot );
		s->RegClassFunc< vector3<T> >( "NormalizeSelf", &vector3<T>::NormalizeSelf );
		s->RegClassFunc< vector3<T> >( "Length", &vector3<T>::Length );
		s->RegClassFunc< vector3<T> >( "LengthPow", &vector3<T>::LengthPow );
	}


	template < typename T >
	void RegisterVector4T( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<vector4<T> >( classname );
		//注册构造函数
		s->RegClassCons<vector4<T>, T, T, T, T >();
		//注册类成员变量
		s->RegClassVar<vector4<T> >( "m_x", &vector4<T>::m_x );
		s->RegClassVar<vector4<T> >( "m_y", &vector4<T>::m_y );
		s->RegClassVar<vector4<T> >( "m_z", &vector4<T>::m_z );
		s->RegClassVar<vector4<T> >( "m_w", &vector4<T>::m_w );
		//注册table成员函数
		//操作符
		s->RegClassFunc< vector4<T>, vector4<T>& (vector4<T>::*)(const vector4<T>&) >( "Equal", &vector4<T>::operator= );
		s->RegClassFunc< vector4<T>, vector4<T> (vector4<T>::*)(const vector4<T>&) const >( "AddVector", &vector4<T>::operator+ );
		s->RegClassFunc< vector4<T>, vector4<T> (vector4<T>::*)(const vector4<T>&) const >( "MinusVector", &vector4<T>::operator- );
		s->RegClassFunc< vector4<T>, vector4<T> (vector4<T>::*)(const vector4<T>&) const >( "MultiplyVector", &vector4<T>::operator* );
		s->RegClassFunc< vector4<T>, vector4<T> (vector4<T>::*)(const vector4<T>&) const >( "DivideVector", &vector4<T>::operator/ );
		s->RegClassFunc< vector4<T>, vector4<T>& (vector4<T>::*)(const vector4<T>&) >( "AddEqualVector", &vector4<T>::operator+= );
		s->RegClassFunc< vector4<T>, vector4<T>& (vector4<T>::*)(const vector4<T>&) >( "MinusEqualVector", &vector4<T>::operator-= );
		s->RegClassFunc< vector4<T>, vector4<T>& (vector4<T>::*)(const vector4<T>&) >( "MultiplyEqualVector", &vector4<T>::operator*= );
		s->RegClassFunc< vector4<T>, vector4<T>& (vector4<T>::*)(const vector4<T>&) >( "DivideEqualVector", &vector4<T>::operator/= );
		s->RegClassFunc< vector4<T>, vector4<T> (vector4<T>::*)(T) const >( "AddNumber", &vector4<T>::operator+ );
		s->RegClassFunc< vector4<T>, vector4<T> (vector4<T>::*)(T) const >( "MinusNumber", &vector4<T>::operator- );
		//s->RegClassFunc< vector4<T>, vector4<T> (vector4<T>::*)(T) const >( "MultiplyNumber", &vector4<T>::operator* );
		s->RegClassFunc< vector4<T>, vector4<T> (vector4<T>::*)(T) const >( "DivideNumber", &vector4<T>::operator/ );
		s->RegClassFunc< vector4<T>, vector4<T>& (vector4<T>::*)(T) >( "AddEqualNumber", &vector4<T>::operator+= );
		s->RegClassFunc< vector4<T>, vector4<T>& (vector4<T>::*)(T) >( "MinusEqualNumber", &vector4<T>::operator-= );
		s->RegClassFunc< vector4<T>, vector4<T>& (vector4<T>::*)(T) >( "MultiplyEqualNumber", &vector4<T>::operator*= );
		s->RegClassFunc< vector4<T>, vector4<T>& (vector4<T>::*)(T) >( "DivideEqualNumber", &vector4<T>::operator/= );
		s->RegClassFunc< vector4<T>, bool (vector4<T>::*)(const vector4<T>&) const >( "isEqualVector", &vector4<T>::operator== );
		s->RegClassFunc< vector4<T>, bool (vector4<T>::*)(const vector4<T>&) const >( "NotEqualVector", &vector4<T>::operator!= );
		s->RegClassFunc< vector4<T>, vector4<T> (vector4<T>::*)(const Matrix44<T>&) const >( "MultiplyMatrix44", &vector4<T>::operator* );
		s->RegClassFunc< vector4<T>, vector4<T>& (vector4<T>::*)(const Matrix44<T>&) >( "MultiplyEqualMatrix44", &vector4<T>::operator*= );
		s->RegClassFunc< vector4<T>, vector4<T> (vector4<T>::*)(const Quaternion<T>&) const >( "MultiplyQuaternion", &vector4<T>::operator* );
		s->RegClassFunc< vector4<T>, vector4<T>& (vector4<T>::*)(const Quaternion<T>&) >( "MultiplyEqualQuaternion", &vector4<T>::operator*= );
		//函数
		s->RegClassFunc< vector4<T> >( "NormalizeSelf", &vector4<T>::NormalizeSelf );
		s->RegClassFunc< vector4<T> >( "Length", &vector4<T>::Length );
		s->RegClassFunc< vector4<T> >( "LengthPow", &vector4<T>::LengthPow );
	}


	template < typename T >
	void RegisterQuaternionT( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<Quaternion<T> >( classname );
		//注册构造函数
		s->RegClassCons<Quaternion<T>, T, T, T, T >();
		//注册类成员变量
		s->RegClassVar<Quaternion<T> >( "m_x", &Quaternion<T>::m_x );
		s->RegClassVar<Quaternion<T> >( "m_y", &Quaternion<T>::m_y );
		s->RegClassVar<Quaternion<T> >( "m_z", &Quaternion<T>::m_z );
		s->RegClassVar<Quaternion<T> >( "m_w", &Quaternion<T>::m_w );
		//注册table成员函数
		//操作符
		s->RegClassFunc< Quaternion<T>, Quaternion<T>& (Quaternion<T>::*)(const Quaternion<T>&) >( "Equal", &Quaternion<T>::operator= );
		s->RegClassFunc< Quaternion<T>, Quaternion<T>& (Quaternion<T>::*)(const vector4<T>&) >( "EqualVector4", &Quaternion<T>::operator= );
		s->RegClassFunc< Quaternion<T>, Quaternion<T> (Quaternion<T>::*)(const Quaternion<T>&) const >( "AddQuaternion", &Quaternion<T>::operator+ );
		s->RegClassFunc< Quaternion<T>, Quaternion<T> (Quaternion<T>::*)(const Quaternion<T>&) const >( "MinusQuaternion", &Quaternion<T>::operator- );
		s->RegClassFunc< Quaternion<T>, Quaternion<T> (Quaternion<T>::*)(const Quaternion<T>&) const >( "MultiplyQuaternion", &Quaternion<T>::operator* );
		s->RegClassFunc< Quaternion<T>, Quaternion<T>& (Quaternion<T>::*)(const Quaternion<T>&) >( "AddEqualQuaternion", &Quaternion<T>::operator+= );
		s->RegClassFunc< Quaternion<T>, Quaternion<T>& (Quaternion<T>::*)(const Quaternion<T>&) >( "MinusEqualQuaternion", &Quaternion<T>::operator-= );
		s->RegClassFunc< Quaternion<T>, Quaternion<T>& (Quaternion<T>::*)(const Quaternion<T>&) >( "MultiplyEqualQuaternion", &Quaternion<T>::operator*= );
		//s->RegClassFunc< Quaternion<T>, Quaternion<T> (Quaternion<T>::*)(T) const >( "MultiplyNumber", &Quaternion<T>::operator* );
		s->RegClassFunc< Quaternion<T>, Quaternion<T> (Quaternion<T>::*)(T) const >( "DivideNumber", &Quaternion<T>::operator/ );
		s->RegClassFunc< Quaternion<T>, Quaternion<T>& (Quaternion<T>::*)(T) >( "MultiplyEqualNumber", &Quaternion<T>::operator*= );
		s->RegClassFunc< Quaternion<T>, Quaternion<T>& (Quaternion<T>::*)(T) >( "DivideEqualNumber", &Quaternion<T>::operator/= );
		//函数
		s->RegClassFunc< Quaternion<T> >( "NormalizeSelf", &Quaternion<T>::NormalizeSelf );
		s->RegClassFunc< Quaternion<T> >( "Length", &Quaternion<T>::Length );
		s->RegClassFunc< Quaternion<T> >( "LengthPow", &Quaternion<T>::LengthPow );
		s->RegClassFunc< Quaternion<T> >( "YawPitchRoll", &Quaternion<T>::YawPitchRoll );
		s->RegClassFunc< Quaternion<T> >( "RotateXYZ", &Quaternion<T>::RotateXYZ );
		s->RegClassFunc< Quaternion<T> >( "AxisToAxis", &Quaternion<T>::AxisToAxis );
		s->RegClassFunc< Quaternion<T> >( "RotateAxis", &Quaternion<T>::RotateAxis );
		s->RegClassFunc< Quaternion<T> >( "GetInverse", &Quaternion<T>::GetInverse );
		s->RegClassFunc< Quaternion<T> >( "InverseSelf", &Quaternion<T>::InverseSelf );
		s->RegClassFunc< Quaternion<T> >( "ToMatrix44", &Quaternion<T>::ToMatrix44 );
	}




	static void RegisterMatrix33( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<Matrix33f >( classname );
		//注册构造函数
		s->RegClassCons<Matrix33f >();
		//注册类成员变量
		s->RegClassVar<Matrix33f >( "a11", &Matrix33f::a11 );
		s->RegClassVar<Matrix33f >( "a12", &Matrix33f::a12 );
		s->RegClassVar<Matrix33f >( "a13", &Matrix33f::a13 );
		s->RegClassVar<Matrix33f >( "a21", &Matrix33f::a21 );
		s->RegClassVar<Matrix33f >( "a22", &Matrix33f::a22 );
		s->RegClassVar<Matrix33f >( "a23", &Matrix33f::a23 );
		s->RegClassVar<Matrix33f >( "a31", &Matrix33f::a31 );
		s->RegClassVar<Matrix33f >( "a32", &Matrix33f::a32 );
		s->RegClassVar<Matrix33f >( "a33", &Matrix33f::a33 );
		//注册table成员函数
		//操作符
		s->RegClassFunc< Matrix33f, Matrix33f (Matrix33f::*)(const Matrix33f&) const >( "MultiplyMatrix33", &Matrix33f::operator* );
		//函数
		s->RegClassFunc< Matrix33f >( "YawPitchRoll", &Matrix33f::YawPitchRoll );
	}


	static void RegisterMatrix44( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<Matrix44f >( classname );
		//注册构造函数
		s->RegClassCons<Matrix44f >();
		//注册类成员变量
		s->RegClassVar<Matrix44f >( "a11", &Matrix44f::a11 );
		s->RegClassVar<Matrix44f >( "a12", &Matrix44f::a12 );
		s->RegClassVar<Matrix44f >( "a13", &Matrix44f::a13 );
		s->RegClassVar<Matrix44f >( "a14", &Matrix44f::a14 );
		s->RegClassVar<Matrix44f >( "a21", &Matrix44f::a21 );
		s->RegClassVar<Matrix44f >( "a22", &Matrix44f::a22 );
		s->RegClassVar<Matrix44f >( "a23", &Matrix44f::a23 );
		s->RegClassVar<Matrix44f >( "a24", &Matrix44f::a24 );
		s->RegClassVar<Matrix44f >( "a31", &Matrix44f::a31 );
		s->RegClassVar<Matrix44f >( "a32", &Matrix44f::a32 );
		s->RegClassVar<Matrix44f >( "a33", &Matrix44f::a33 );
		s->RegClassVar<Matrix44f >( "a34", &Matrix44f::a34 );
		s->RegClassVar<Matrix44f >( "a41", &Matrix44f::a41 );
		s->RegClassVar<Matrix44f >( "a42", &Matrix44f::a42 );
		s->RegClassVar<Matrix44f >( "a43", &Matrix44f::a43 );
		s->RegClassVar<Matrix44f >( "a44", &Matrix44f::a44 );

		//注册table成员函数
		//操作符
		s->RegClassFunc< Matrix44f, Matrix44f (Matrix44f::*)(const Matrix44f&) const >( "MultiplyMatrix44", &Matrix44f::operator* );
		//s->RegClassFunc< Matrix44f, Matrix44f (Matrix44f::*)(const float&) const >( "MultiplyNumber", &Matrix44f::operator* );
		s->RegClassFunc< Matrix44f, Matrix44f (Matrix44f::*)(float) const >( "DivideNumber", &Matrix44f::operator/ );
		s->RegClassFunc< Matrix44f, Matrix44f& (Matrix44f::*)(const Matrix44f&) >( "MultiplyEqualMatrix44", &Matrix44f::operator*= );
		//函数
		s->RegClassFunc< Matrix44f >( "Identitat", &Matrix44f::Identitat );
		s->RegClassFunc< Matrix44f >( "ToQuaternion", &Matrix44f::ToQuaternion );
		s->RegClassFunc< Matrix44f >( "InverseSelf", &Matrix44f::InverseSelf );
		s->RegClassFunc< Matrix44f >( "GetInverse", &Matrix44f::GetInverse );
	}







	static void RegisterColor( ScriptBase* s, const char* classname )
	{
		s->LoadClasses( ScriptManager::CLASS_VECTOR4F );
		//注册一个类，实际是table
		s->RegClass<Color >( classname );
		//注册构造函数
		s->RegClassCons<Color, float, float, float, float >();
		//注册类成员变量
		s->RegClassInh<Color, vector4f >();
	}
}