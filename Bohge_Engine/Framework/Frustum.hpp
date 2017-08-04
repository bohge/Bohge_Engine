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
#ifndef BOHGE_FRUSTUN
#define BOHGE_FRUSTUM
#else
#error( "Do not include this file in other files" )
#endif // BASEMATHDECLAR
#include "Object.hpp"


namespace BohgeEngine
{

	template<typename T>
	class Frustum : public Object
	{
		RTTI_TYPE_DECLEAR( Frustum<T> );
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
		enum CornerName
		{
			FRUSTUM_CORNER_LBN = 0, // left bottom near
			FRUSTUM_CORNER_RBN = 1, // right bottom near
			FRUSTUM_CORNER_LTN = 2, // left top near
			FRUSTUM_CORNER_RTN = 3, // right top near
			FRUSTUM_CORNER_LBF = 4, // left bottom far
			FRUSTUM_CORNER_RBF = 5, // right bottom far
			FRUSTUM_CORNER_LTF = 6, // left top far
			FRUSTUM_CORNER_RTF = 7, // right top far

			FRUSTUM_CORNER_COUNT = 8,
		};
	private:
		Plane<T>		m_Planes[FACE_COUNT];
		vector3<T>		m_Corners[FRUSTUM_CORNER_COUNT];
		int				m_VertexLUT[FACE_COUNT];
	public:
		BOHGE_FORCEINLINE int GetPlaneLUT( PalneName pn ) const
		{
			return m_VertexLUT[static_cast<int>(pn)];
		}
		BOHGE_FORCEINLINE const Plane<T>& GetPlane( PalneName pn ) const
		{
			return m_Planes[static_cast<int>(pn)];
		}
		BOHGE_FORCEINLINE const vector3<T>& GetCorner( CornerName cn ) const
		{
			return m_Corners[static_cast<int>(cn)];
		}
		BOHGE_FORCEINLINE void FromMatrix( const Matrix44f& clip , const Matrix44f& inv_clip)
		{
			m_Corners[0] = vector3<T>(-1, -1, 0) * inv_clip; // left bottom near
			m_Corners[1] = vector3<T>(+1, -1, 0) * inv_clip; // right bottom near
			m_Corners[2] = vector3<T>(-1, +1, 0) * inv_clip; // left top near
			m_Corners[3] = vector3<T>(+1, +1, 0) * inv_clip; // right top near
			m_Corners[4] = vector3<T>(-1, -1, 1) * inv_clip; // left bottom far
			m_Corners[5] = vector3<T>(+1, -1, 1) * inv_clip; // right bottom far
			m_Corners[6] = vector3<T>(-1, +1, 1) * inv_clip; // left top far
			m_Corners[7] = vector3<T>(+1, +1, 1) * inv_clip; // right top far


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
		BOHGE_FORCEINLINE IntersectedType::Volume IntersectType(Aabbox3d<T> const & aabb) const
		{
			return Math::Frustum_Aabbox3d_IntersectType( *this, aabb );
		}
		//BOHGE_FORCEINLINE IntersectedType::Volume IntersectType(Frustum<T> const & frustum) const这个有问题暂时不用了
		//{
		//	return Math::Frustum_Frustum_IntersectType( *this, frustum );
		//}
		BOHGE_FORCEINLINE bool Intersect(vector3<T> const & point) const
		{
			return Math::Frustum_vector3_Intersect( *this, point );
		}
		BOHGE_FORCEINLINE bool Intersect(Aabbox3d<T> const & aabb) const
		{
			return Math::Frustum_Aabbox3d_Intersect( *this, aabb );
		}
	};
}