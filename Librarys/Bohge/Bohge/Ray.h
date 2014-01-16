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



/////////////////////////////
//			…‰œﬂ		   //
/////////////////////////////
#pragma once
#include "3DMath.h"

namespace BohgeEngine
{

	template <class T>
	class Ray
	{
	private:
		vector3<T>		m_RayOrig;
		vector3<T>		m_RayDir;
	public:
		Ray(){}
		Ray(const vector3<T>& orig, const vector3<T>& dir)
			:m_RayOrig(orig),
			m_RayDir(dir)
		{
		}
	public:
		BOHGE_FORCEINLINE Ray<T> operator * ( const Quaternion<T>& rot ) const
		{
			return Ray( m_RayOrig * rot, m_RayDir * rot );
		}
		BOHGE_FORCEINLINE const Ray<T>& operator *= ( const Quaternion<T>& rot )
		{
			m_RayOrig *= rot;
			m_RayDir *= rot;
			return *this;
		}
		BOHGE_FORCEINLINE Ray<T> operator * ( const Matrix44f& m ) const
		{
			vector4<T> ro( m_RayOrig.m_x, m_RayOrig.m_y, m_RayOrig.m_z, 1.0 );
			vector4<T> rd( m_RayDir.m_x, m_RayDir.m_y, m_RayDir.m_z, 1.0 );
			return Ray( vector3<T>( ro * m ), vector3<T>( rd * m ) );
		}
		BOHGE_FORCEINLINE const Ray<T>& operator *= ( const Matrix44f& m )
		{
			m_RayOrig *= m;
			m_RayDir *= m;
			return *this;
		}
		BOHGE_FORCEINLINE void SetRayOrigin( const vector3<T>& ori )
		{
			m_RayOrig = ori;
		}
		BOHGE_FORCEINLINE void SetRayDirection( const vector3<T>& dir )
		{
			m_RayDir = dir;
		}
		BOHGE_FORCEINLINE const vector3<T>& GetRayOrigin() const
		{
			return m_RayOrig;
		}
		BOHGE_FORCEINLINE const vector3<T>& GetRayDirection() const
		{
			return m_RayDir;
		}
	};

	typedef Ray<float> Rayf;

}