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



//////////////////////////////
//			视锥		   //
/////////////////////////////

#pragma once
#include "3DMath.h"
#include "Plane.hpp"
#include "aabbox.hpp"


namespace BohgeEngine
{

	template<typename T>
	class Frustum
	{
	public:
		enum PalneName
		{
			FRUSTUM_PLANE_LEFT   = 0,
			FRUSTUM_PLANE_RIGHT  = 1,
			FRUSTUM_PLANE_BOTTOM = 2,
			FRUSTUM_PLANE_TOP    = 3,
			FRUSTUM_PLANE_NEAR   = 4,
			FRUSTUM_PLANE_FAR    = 5,
			FACE_COUNT	= 6,
		};
	private:
		Plane<T>		m_Planes[FACE_COUNT];
		int				m_VertexLUT[FACE_COUNT];
	public:
		BOHGE_FORCEINLINE const Plane<T>& GetPlane( PalneName pn ) const
		{
			return m_Planes[static_cast<int>(pn)];
		}
		BOHGE_FORCEINLINE void FromMatrix( const Matrix44f& clip )
		{
			//http://web.archive.org/web/20120531231005/http://crazyjoke.free.fr/doc/3D/plane%20extraction.pdf

			vector4<T> column1( clip.a11, clip.a21, clip.a31, clip.a41 );
			vector4<T> column2( clip.a12, clip.a22, clip.a32, clip.a42 );
			vector4<T> column3( clip.a13, clip.a23, clip.a33, clip.a43 );
			vector4<T> column4( clip.a14, clip.a24, clip.a34, clip.a44 );

			m_Planes[FRUSTUM_PLANE_LEFT] = column4 + column1;  // left
			m_Planes[FRUSTUM_PLANE_RIGHT] = column4 - column1;  // right
			m_Planes[FRUSTUM_PLANE_BOTTOM] = column4 + column2;  // bottom
			m_Planes[FRUSTUM_PLANE_TOP] = column4 - column2;  // top
			m_Planes[FRUSTUM_PLANE_NEAR] = column3;  // near
			m_Planes[FRUSTUM_PLANE_FAR] = column4 - column3;  // far

			// Loop through each side of the frustum and normalize it.
			for ( int i = 0 ; i < FACE_COUNT ; i ++ )
			{
				m_Planes[i].NormalizeSelf();
			}

			//  build a bit-field that will tell us the indices for the nearest and farthest vertices from each plane...
			for (int i = 0; i < 6; ++ i)
			{
				m_VertexLUT[i] = ((m_Planes[i].A() < 0) ? 1 : 0) | ((m_Planes[i].B() < 0) ? 2 : 0) | ((m_Planes[i].C() < 0) ? 4 : 0);
			}
		}
		BOHGE_FORCEINLINE IntersectedType::Volume Intersect(aabbox3d<T> const & aabb) const
		{
			bool intersect = false;
			for (int i = 0; i < FACE_COUNT; ++ i)
			{
				int const n = m_VertexLUT[i];

				// v1 is diagonally opposed to v0
				vector3<T> v0((n & 1) ? aabb.GetMin().m_x : aabb.GetMax().m_x, (n & 2) ? aabb.GetMin().m_y : aabb.GetMax().m_y, (n & 4) ? aabb.GetMin().m_z : aabb.GetMax().m_z);
				vector3<T> v1((n & 1) ? aabb.GetMax().m_x : aabb.GetMin().m_x, (n & 2) ? aabb.GetMax().m_y : aabb.GetMin().m_y, (n & 4) ? aabb.GetMax().m_z : aabb.GetMin().m_z);

				if ( m_Planes[i].Dot(v0) < 0 )
				{
					return IntersectedType::IT_NO;
				}
				if ( m_Planes[i].Dot(v1) < 0 )
				{
					intersect = true;
				}
			}
			return intersect ? IntersectedType::IT_PARTIAL : IntersectedType::IT_FULL;


			//vector3f centre = aabb.GetCenter();
			//// 中心点指向一个边的向量
			//vector3f halfSize = aabb.GetHalfSize();

			//// 测试盒子在每一个平面的位置关系
			//bool isFullInster = true;
			//for (int plane = 0; plane < 6; ++plane)
			//{
			//	OnSide::Volume side = m_Planes[plane].GetSide(centre, halfSize);
			//	if (side == OnSide::OS_NEGATIVE_SIDE)
			//	{
			//		return IntersectedType::IT_NO;
			//	}
			//	if( side == OnSide::OS_BOTH_SIDE )
			//	{
			//		isFullInster = false;
			//	}
			//}
			//return isFullInster ? IntersectedType::IT_FULL : IntersectedType::IT_PARTIAL;
		}
		BOHGE_FORCEINLINE IntersectedType::Volume Intersect(vector3<T> const & point) const
		{
			for (int i = 0; i < FACE_COUNT; ++ i)
			{
				if ( m_Planes[i].Dot(point) < 0 )
				{
					return IntersectedType::IT_NO;
				}
			}
			return IntersectedType::IT_FULL;
		}
	};


	typedef Frustum<float> Frustumf;

}