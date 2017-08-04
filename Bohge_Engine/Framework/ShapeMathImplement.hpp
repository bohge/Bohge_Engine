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
#ifndef SPHEREMATHIMPLEMENT
#define SPHEREMATHIMPLEMENT
#else
#error( "Do not include this file in other files" )
#endif // SPHEREMATHIMPLEMENT

namespace BohgeEngine
{
	namespace Math
	{

		//图形的
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE T Dot(const Plane<T>& v1, const vector3<T>& v2)
		{
			return ( v1.A() * v2.m_x + v1.B() * v2.m_y + v1.C() * v2.m_z + v1.D());
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE IntersectedType::Volume Aabbox3d_Aabbox3d_IntersectType(const Aabbox3d<T>& lhs, const Aabbox3d<T>& rhs)
		{
			bool min = lhs.Intersect( rhs.GetMin() );
			bool max = lhs.Intersect( rhs.GetMax() );
			if ( min && max )
			{
				return IntersectedType::IT_FULL;
			}
			else if ( min || max )
			{
				return IntersectedType::IT_PARTIAL;
			}
			return IntersectedType::IT_NO;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d_Aabbox3d_Intersect(const Aabbox3d<T>& lhs, const Aabbox3d<T>& rhs)
		{
			vector3<T> cen = lhs.GetCenter() - rhs.GetCenter();
			vector3<T> hs = lhs.GetHalfSize() + rhs.GetHalfSize();
			return ( Math::Abs(cen.m_x) <= hs.m_x) && (Math::Abs(cen.m_y) <= hs.m_y) && (Math::Abs(cen.m_z) <= hs.m_z );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d_Vector3_Intersect(const Aabbox3d<T>& lhs, const vector3<T>& rhs)
		{
			if( rhs.m_x < lhs.GetMin().m_x ) return false;
			if( rhs.m_y < lhs.GetMin().m_y ) return false;
			if( rhs.m_z < lhs.GetMin().m_z ) return false;

			if( rhs.m_x > lhs.GetMax().m_x ) return false;
			if( rhs.m_y > lhs.GetMax().m_y ) return false;
			if( rhs.m_z > lhs.GetMax().m_z ) return false;
			return true;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d_Ray_Intersect(const Aabbox3d<T>& lhs, const Ray<T>& rhs)
		{
			if( lhs.Intersect( rhs.GetRayOrigin() ) )//如果观察点在box里面
			{
				return true;
			}
			//依次检查各面的相交情况
			T t;
			vector3<T> hitpoint;
			//前面
			if( rhs.GetRayOrigin().m_z > lhs.GetMax().m_z && rhs.GetRayDirection().m_z < 0 )
			{
				t = ( lhs.GetMax().m_z - rhs.GetRayOrigin().m_z ) / rhs.GetRayDirection().m_z;//计算投射到该平面的距离
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					//如果这个hitpoint在这个平面内
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						return true;
					}
				}
			}
			//左面
			if( rhs.GetRayOrigin().m_x < lhs.GetMin().m_x && rhs.GetRayDirection().m_x > 0 )
			{
				t = ( lhs.GetMin().m_x - rhs.GetRayOrigin().m_x ) / rhs.GetRayDirection().m_x;
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					if(		hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						return true;
					}
				}
			}
			//后面
			if( rhs.GetRayOrigin().m_z < lhs.GetMin().m_z && rhs.GetRayDirection().m_z > 0 )
			{
				t = ( lhs.GetMin().m_z - rhs.GetRayOrigin().m_z ) / rhs.GetRayDirection().m_z;
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						return true;
					}
				}
			}
			//右面
			if( rhs.GetRayOrigin().m_x > lhs.GetMax().m_x && rhs.GetRayDirection().m_x < 0 )
			{
				t = ( lhs.GetMax().m_x - rhs.GetRayOrigin().m_x ) / rhs.GetRayDirection().m_x;
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					if(		hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						return true;
					}
				}
			}
			//上面
			if( rhs.GetRayOrigin().m_y > lhs.GetMax().m_y && rhs.GetRayDirection().m_y < 0 )
			{
				t = ( lhs.GetMax().m_y - rhs.GetRayOrigin().m_y ) / rhs.GetRayDirection().m_y;
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z )
					{
						return true;
					}
				}
			}
			//下面
			if( rhs.GetRayOrigin().m_y < lhs.GetMin().m_y && rhs.GetRayDirection().m_y > 0 )
			{
				t = ( lhs.GetMin().m_y - rhs.GetRayOrigin().m_y ) / rhs.GetRayDirection().m_y;
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z )
					{
						return true;
					}
				}
			}
			return false;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d_Line_Intersect(const Aabbox3d<T>& lhs, const Line<T>& rhs)
		{
			if( lhs.Intersect( rhs.GetOrigin() ) )//如果观察点在box里面
			{
				return true;
			}
			//依次检查各面的相交情况
			float t;
			vector3<T> hitpoint;
			//前面
			if( rhs.GetOrigin().m_z > lhs.GetMax().m_z && rhs.GetDirection().m_z < 0 )
			{
				t = ( lhs.GetMax().m_z - rhs.GetOrigin().m_z ) / rhs.GetDirection().m_z;//计算投射到该平面的距离
				if( t > 0 && t < rhs.GetLength() )
				{
					hitpoint = rhs.GetOrigin() + rhs.GetDirection() * t;
					//如果这个hitpoint在这个平面内
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						return true;
					}
				}
			}
			//左面
			if( rhs.GetOrigin().m_x < lhs.GetMin().m_x && rhs.GetDirection().m_x > 0 )
			{
				t = ( lhs.GetMin().m_x - rhs.GetOrigin().m_x ) / rhs.GetDirection().m_x;
				if( t > 0 && t < rhs.GetLength() )
				{
					hitpoint = rhs.GetOrigin() + rhs.GetDirection() * t;
					if(		hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						return true;
					}
				}
			}
			//后面
			if( rhs.GetOrigin().m_z < lhs.GetMin().m_z && rhs.GetDirection().m_z > 0 )
			{
				t = ( lhs.GetMin().m_z - rhs.GetOrigin().m_z ) / rhs.GetDirection().m_z;
				if( t > 0 && t < rhs.GetLength() )
				{
					hitpoint = rhs.GetOrigin() + rhs.GetDirection() * t;
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						return true;
					}
				}
			}
			//右面
			if( rhs.GetOrigin().m_x > lhs.GetMax().m_x && rhs.GetDirection().m_x < 0 )
			{
				t = ( lhs.GetMax().m_x - rhs.GetOrigin().m_x ) / rhs.GetDirection().m_x;
				if( t > 0 && t < rhs.GetLength() )
				{
					hitpoint = rhs.GetOrigin() + rhs.GetDirection() * t;
					if(		hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						return true;
					}
				}
			}
			//上面
			if( rhs.GetOrigin().m_y > lhs.GetMax().m_y && rhs.GetDirection().m_y < 0 )
			{
				t = ( lhs.GetMax().m_y - rhs.GetOrigin().m_y ) / rhs.GetDirection().m_y;
				if( t > 0 && t < rhs.GetLength() )
				{
					hitpoint = rhs.GetOrigin() + rhs.GetDirection() * t;
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z )
					{
						return true;
					}
				}
			}
			//下面
			if( rhs.GetOrigin().m_y < lhs.GetMin().m_y && rhs.GetDirection().m_y > 0 )
			{
				t = ( lhs.GetMin().m_y - rhs.GetOrigin().m_y ) / rhs.GetDirection().m_y;
				if( t > 0 && t < rhs.GetLength() )
				{
					hitpoint = rhs.GetOrigin() + rhs.GetDirection() * t;
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z )
					{
						return true;
					}
				}
			}
			return false;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d_Aabsphere_Intersect(const Aabbox3d<T>& lhs, const Aabsphere<T>& rhs)
		{
			if( lhs.Intersect( rhs.GetCenter() ) )
			{
				return true;
			}
			vector3<T> half_size = lhs.GetHalfSize();
			vector3<T> d = rhs.GetCenter() - lhs.GetCenter();
			vector3<T> closest_point_on_obb = lhs.GetCenter();
			for (int i = 0; i < 3; ++ i)
			{
				vector3<T> axis(0, 0, 0);
				axis[i] = 1;
				T dist = d.Dot(axis);
				if (dist > half_size[i])
				{
					dist = half_size[i];
				}
				if (dist < -half_size[i])
				{
					dist = -half_size[i];
				}
				closest_point_on_obb += axis * dist;
			}

			vector3<T> v = closest_point_on_obb - rhs.GetCenter();
			return v.Dot(v) <= rhs.MaxRadiusSq();
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Aabbox3d_Ray_IntersectPoint(const Aabbox3d<T>& lhs, const Aabsphere<T>& rhs, vector3<T>& out)
		{
			if( lhs.Intersect( rhs.GetRayOrigin() ) )//如果观察点在box里面
			{
				out = rhs.GetRayOrigin();
				return true;
			}
			//依次检查各面的相交情况
			float t;
			float len = 0.0f;
			bool isInster = false;
			vector3<T> hitpoint;
			//前面
			if( rhs.GetRayOrigin().m_z > lhs.GetMax().m_z && rhs.GetRayDirection().m_z < 0 )
			{
				t = ( lhs.GetMax().m_z - rhs.GetRayOrigin().m_z ) / rhs.GetRayDirection().m_z;//计算投射到该平面的距离
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					//如果这个hitpoint在这个平面内
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						if( len > t || !isInster )
						{
							isInster = true;
							len = t;
							out = hitpoint;
						}
					}
				}
			}
			//左面
			if( rhs.GetRayOrigin().m_x < lhs.GetMin().m_x && rhs.GetRayDirection().m_x > 0 )
			{
				t = ( lhs.GetMin().m_x - rhs.GetRayOrigin().m_x ) / rhs.GetRayDirection().m_x;
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					if(		hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						if( len > t || !isInster )
						{
							isInster = true;
							len = t;
							out = hitpoint;
						}
					}
				}
			}
			//后面
			if( rhs.GetRayOrigin().m_z < lhs.GetMin().m_z && rhs.GetRayDirection().m_z > 0 )
			{
				t = ( lhs.GetMin().m_z - rhs.GetRayOrigin().m_z ) / rhs.GetRayDirection().m_z;
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						if( len > t || !isInster )
						{
							isInster = true;
							len = t;
							out = hitpoint;
						}
					}
				}
			}
			//右面
			if( rhs.GetRayOrigin().m_x > lhs.GetMax().m_x && rhs.GetRayDirection().m_x < 0 )
			{
				t = ( lhs.GetMax().m_x - rhs.GetRayOrigin().m_x ) / rhs.GetRayDirection().m_x;
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					if(		hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z
						&&	hitpoint.m_y < lhs.GetMax().m_y &&	hitpoint.m_y > lhs.GetMin().m_y )
					{
						if( len > t || !isInster )
						{
							isInster = true;
							len = t;
							out = hitpoint;
						}
					}
				}
			}
			//上面
			if( rhs.GetRayOrigin().m_y > lhs.GetMax().m_y && rhs.GetRayDirection().m_y < 0 )
			{
				t = ( lhs.GetMax().m_y - rhs.GetRayOrigin().m_y ) / rhs.GetRayDirection().m_y;
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z )
					{
						if( len > t || !isInster )
						{
							isInster = true;
							len = t;
							out = hitpoint;
						}
					}
				}
			}
			//下面
			if( rhs.GetRayOrigin().m_y < lhs.GetMin().m_y && rhs.GetRayDirection().m_y > 0 )
			{
				t = ( lhs.GetMin().m_y - rhs.GetRayOrigin().m_y ) / rhs.GetRayDirection().m_y;
				if( t > 0 )
				{
					hitpoint = rhs.GetRayOrigin() + rhs.GetRayDirection() * t;
					if(		hitpoint.m_x > lhs.GetMin().m_x &&	hitpoint.m_x < lhs.GetMax().m_x
						&&	hitpoint.m_z > lhs.GetMin().m_z &&	hitpoint.m_z < lhs.GetMax().m_z )
					{
						if( len > t || !isInster )
						{
							isInster = true;
							len = t;
							out = hitpoint;
						}
					}
				}
			}
			return isInster;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE Aabrect<T> Aabrect_From_CenterSize(const vector2<T>& center, const vector2<T>& size)
		{
			vector2<T> hs= size / T( 2 );
			return Aabrect<T>( center - hs, center + hs );
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Aabrect_Vector2_Intersect(const Aabrect<T>& lhs, const vector2<T>& rhs)
		{
			if(		( rhs.m_x >= lhs.LeftBottom().m_x && rhs.m_y >= lhs.LeftBottom().m_y )
				&&	( rhs.m_x <= lhs.RightTop().m_x && rhs.m_y <= lhs.RightTop().m_y )	)
			{
				return true;
			}
			return false;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Aabrect_Aabrect_Intersect(const Aabrect<T>& lhs, const Aabrect<T>& rhs)
		{
			//你是否在我的范围内
			if ( lhs.Intersect(rhs.LeftBottom()) )
			{
				return true;
			}
			else if( lhs.Intersect(rhs.RightTop()) )
			{
				return true;
			}
			else if( lhs.Intersect( vector2<T>( rhs.LeftBottom().m_x, rhs.RightTop().m_y ) ) )
			{
				return true;
			}
			else if( lhs.Intersect( vector2<T>( rhs.RightTop().m_x, rhs.LeftBottom().m_y ) ) )
			{
				return true;
			}

			//我是否在你的范围内
			else if ( rhs.Intersect(lhs.LeftBottom()) )
			{
				return true;
			}
			else if( rhs.Intersect(lhs.RightTop()) )
			{
				return true;
			}
			else if( rhs.Intersect( vector2<T>( lhs.LeftBottom().m_x, lhs.RightTop().m_y ) ) )
			{
				return true;
			}
			else if( rhs.Intersect( vector2<T>( lhs.RightTop().m_x, lhs.LeftBottom().m_y ) ) )
			{
				return true;
			}
			return false;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Aabround_Vector2_Intersect(const Aabround<T>& lhs, const vector2<T>& rhs)
		{
			Aabrect<T> rect( lhs.GetCenter(), vector2<T>( lhs.GetRadius()*2, lhs.GetRadius()*2 ) );
			if( rect.Intersect( rhs ) )
			{
				float dx = lhs.GetCenter().m_x - rhs.m_x;
				float dy = lhs.GetCenter().m_y - rhs.m_y;
				float powR= dx*dx+dy*dy;
				if( m_PowRadius >= powR )
				{
					//out_percentage = powR / m_PowRadius;
					return true;
				}
			}
			return false;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Aabsphere_Aabsphere_Intersect(const Aabsphere<T>& lhs, const Aabsphere<T>& rhs)
		{
			vector3<T> d = lhs.GetCenter() - rhs.GetCenter();
			T r = lhs.GetRadius() + rhs.GetRadius();
			return d.Dot(d) <= r * r;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Aabsphere_Vector3_Intersect(const Aabsphere<T>& lhs, const vector3<T>& rhs)
		{
			vector3<T> d = rhs - lhs.GetCenter();
			return d.Dot(d) <= lhs.MaxRadiusSq();
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE IntersectedType::Volume Frustum_Aabbox3d_IntersectType(const Frustum<T>& lhs, const Aabbox3d<T>& rhs)
		{
			bool intersect = false;
			for (int i = 0; i < Frustum<T>::FACE_COUNT; ++ i)
			{
				int const n = lhs.GetPlaneLUT(static_cast<Frustum<T>::PalneName>(i));

				// v1 is diagonally opposed to v0
				vector3<T> v0((n & 1) ? rhs.GetMin().m_x : rhs.GetMax().m_x, (n & 2) ? rhs.GetMin().m_y : rhs.GetMax().m_y, (n & 4) ? rhs.GetMin().m_z : rhs.GetMax().m_z);
				vector3<T> v1((n & 1) ? rhs.GetMax().m_x : rhs.GetMin().m_x, (n & 2) ? rhs.GetMax().m_y : rhs.GetMin().m_y, (n & 4) ? rhs.GetMax().m_z : rhs.GetMin().m_z);

				if ( lhs.GetPlane(static_cast<Frustum<T>::PalneName>(i)).Dot(v0) < 0 )
				{
					return IntersectedType::IT_NO;
				}
				if ( lhs.GetPlane(static_cast<Frustum<T>::PalneName>(i)).Dot(v1) < 0 )
				{
					intersect = true;
				}
			}
			return intersect ? IntersectedType::IT_PARTIAL : IntersectedType::IT_FULL;

			//vector3f centre = rhs.GetCenter();
			//// 中心点指向一个边的向量
			//vector3f halfSize = rhs.GetHalfSize();

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
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE IntersectedType::Volume Frustum_Frustum_IntersectType(const Frustum<T>& lhs, const Frustum<T>& rhs)
		{
			bool outside = false;
			bool inside_all = true;
			for (int i = 0; i < 6; ++ i)
			{
				Plane<T> const & p = lhs.GetPlane(static_cast<Frustum<T>::PalneName>(i));

				T min_p, max_p;
				min_p = max_p = p.Dot(rhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_LBN));
				for (int j = 1; j < 8; ++ j)
				{
					T tmp = p.Dot(rhs.GetCorner(static_cast<Frustum<T>::CornerName>(j)));
					min_p = Math::GetMin(min_p, tmp);
					max_p = Math::GetMax(max_p, tmp);
				}

				outside |= (min_p > 0);
				inside_all &= (max_p <= 0);
			}
			if (outside)
			{
				return IntersectedType::IT_NO;
			}
			if (inside_all)
			{
				return IntersectedType::IT_FULL;
			}

			for (int i = 0; i < 6; ++ i)
			{
				Plane<T> const & p = rhs.GetPlane(static_cast<Frustum<T>::PalneName>(i));

				T min_p = p.Dot(lhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_LBN));
				for (int j = 1; j < 8; ++ j)
				{
					T tmp = p.Dot(lhs.GetCorner(static_cast<Frustum<T>::CornerName>(j)));
					min_p = Math::GetMin(min_p, tmp);
				}

				outside |= (min_p > 0);
			}
			if (outside)
			{
				return IntersectedType::IT_NO;
			}

			vector3<T> edge_axis_l[6];
			edge_axis_l[0] = rhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_LTF);
			edge_axis_l[1] = rhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_LBF);
			edge_axis_l[2] = rhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_RBF);
			edge_axis_l[3] = rhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_RTF);
			edge_axis_l[4] = rhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_LTF) - rhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_RBF);
			edge_axis_l[5] = rhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_RTF) - rhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_RBF);

			vector3<T> edge_axis_r[6];
			edge_axis_r[0] = lhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_LTF);
			edge_axis_r[1] = lhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_LBF);
			edge_axis_r[2] = lhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_RBF);
			edge_axis_r[3] = lhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_RTF);
			edge_axis_r[4] = lhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_LTF) - lhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_RBF);
			edge_axis_r[5] = lhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_RTF) - lhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_RBF);

			for (int i = 0; i < 6; ++ i)
			{
				for (int j = 0; j < 6; ++ j)
				{
					vector3<T> Axis = edge_axis_l[i].Cross(edge_axis_r[j]);

					T min_l, max_l, min_r, max_r;
					min_l = max_l = Axis.Dot(rhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_LBN));
					min_r = max_r = Axis.Dot(lhs.GetCorner(Frustum<T>::FRUSTUM_CORNER_LBN));
					for (int k = 1; k < 8; ++ k)
					{
						T tmp = Axis.Dot(rhs.GetCorner(static_cast<Frustum<T>::CornerName>(k)));
						min_l = Math::GetMin(min_l, tmp);
						max_l = Math::GetMax(max_l, tmp);

						tmp = Axis.Dot(lhs.GetCorner(static_cast<Frustum<T>::CornerName>(k)));
						min_r = Math::GetMin(min_r, tmp);
						max_r = Math::GetMax(max_r, tmp);
					}

					outside |= min_l > max_r;
					outside |= min_r > max_l;
				}
			}
			if (outside)
			{
				return IntersectedType::IT_NO;
			}
			return IntersectedType::IT_PARTIAL;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Frustum_vector3_Intersect(const Frustum<T>& lhs, const vector3<T>& rhs)
		{
			for (int i = 0; i < Frustum<T>::FACE_COUNT; ++ i)
			{
				if ( lhs.GetPlane(static_cast<Frustum<T>::PalneName>(i)).Dot(rhs) < 0 )
				{
					return false;
				}
			}
			return true;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE bool Frustum_Aabbox3d_Intersect(const Frustum<T>& lhs, const Aabbox3d<T>& rhs)
		{
			bool intersect = false;
			for (int i = 0; i < Frustum<T>::FACE_COUNT; ++ i)
			{
				int const n = lhs.GetPlaneLUT(static_cast<Frustum<T>::PalneName>(i));

				// v1 is diagonally opposed to v0
				vector3<T> v0((n & 1) ? rhs.GetMin().m_x : rhs.GetMax().m_x, (n & 2) ? rhs.GetMin().m_y : rhs.GetMax().m_y, (n & 4) ? rhs.GetMin().m_z : rhs.GetMax().m_z);
				vector3<T> v1((n & 1) ? rhs.GetMax().m_x : rhs.GetMin().m_x, (n & 2) ? rhs.GetMax().m_y : rhs.GetMin().m_y, (n & 4) ? rhs.GetMax().m_z : rhs.GetMin().m_z);

				if ( lhs.GetPlane(static_cast<Frustum<T>::PalneName>(i)).Dot(v0) < 0 )
				{
					return false;
				}
			}
			return true;
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE vector3<T> Plane_Ray_IntersectPoint(const Plane<T>& lhs, const Ray<T>& rhs)
		{
			T deno( rhs.GetRayDirection().Dot( lhs.Normal() ) );
			if ( Math::isEqual( deno, T(0) ) )
			{
				deno = T(0.0001);
			}
			T Intersect = - ( lhs.A()*rhs.GetRayOrigin().m_x + lhs.B()*rhs.GetRayOrigin().m_y + lhs.C()*rhs.GetRayOrigin().m_z + lhs.D() ) / deno;
			vector3<T> res = rhs.GetRayOrigin() + rhs.GetRayDirection() * Intersect;
			return res;
		}
	}
}