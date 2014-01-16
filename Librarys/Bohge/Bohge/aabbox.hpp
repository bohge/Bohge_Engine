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
//			包围盒		   //
/////////////////////////////
#pragma once
#include "3DMath.h"
#include "Vertex.h"
#include "Ray.h"
#include "Vertex.h"
#include "Plane.hpp"
#include "aabsphere.hpp"
#include "Line.hpp"

#ifdef _DEBUG
//#define DRAWAABBOX
#endif // _DEBUG




namespace BohgeEngine
{

	//位置关系
	//	   ulb----urb
	//     /|     /|
	//    ulf--urf |
	//    | dlb--|-drb
	//    |/     |/
	//    dlf---drf
	class Rect3D;
	class Engine;
#ifdef DRAWAABBOX
	void NewRect3D( Rect3D** rect, const vector3f& ulb, const vector3f& drf, const Color& clr );
	void ResetRect3D( Rect3D& rect, const vector3f& ulb, const vector3f& drf );
	void SetTransform( Rect3D& rect, const Matrix44f& m );
	void RenderRect3D( Engine& engine, Rect3D& rect );
#endif
	template <typename T>
	class aabbox3d
	{
	private:
		enum CONSTANT
		{
			INCRES = 10,
		};
	public:
		enum Points
		{
			PS_ULB = 0,
			PS_ULF,
			PS_URF,
			PS_URB,

			PS_DRF,
			PS_DRB,
			PS_DLB,
			PS_DLF,
		};
		enum BoxPlane //正方形的面
		{
			BP_FRONT = 0,
			BP_RIGHT,
			BP_BACK,
			BP_LEFT,
			BP_UP,
			BP_DOWN,
		};
	private:
		bool			m_isIdentitat;//是否是初始值，没有改变的
		vector3<T>		m_vMin;	//最远点
		vector3<T>		m_vMax;	//最近点
#ifdef DRAWAABBOX
		Rect3D*			m_Rect;
#endif
	public:
		aabbox3d()
			:m_vMin(vector3<T>::Zero()),
			m_vMax(vector3<T>::Zero()),
			m_isIdentitat(true)
		{
#ifdef DRAWAABBOX
			NewRect3D( &m_Rect, GetPoint(PS_ULB), GetPoint(PS_DRF), Color::Red );
#endif
		}
		aabbox3d( const aabbox3d<T>& input)
			:m_vMin(input.m_vMin),
			m_vMax(input.m_vMax),
			m_isIdentitat(input.m_isIdentitat)
		{
#ifdef DRAWAABBOX
			NewRect3D( &m_Rect, GetPoint(PS_ULB), GetPoint(PS_DRF), Color::Red );
#endif
		}
		aabbox3d& operator= (const aabbox3d& rhs )
		{
			m_vMin = rhs.m_vMin;
			m_vMax = rhs.m_vMax;
			m_isIdentitat = rhs.m_isIdentitat;
#ifdef DRAWAABBOX
			ResetRect3D( *m_Rect, GetPoint(PS_ULB), GetPoint(PS_DRF) );
#endif
			return *this;
		}
		aabbox3d( const vector3<T>& minP, const vector3<T>& maxP )
		{
			m_isIdentitat = false;
			m_vMin.m_x = Math::GetMin( minP.m_x, maxP.m_x );
			m_vMin.m_y = Math::GetMin( minP.m_y, maxP.m_y );
			m_vMin.m_z = Math::GetMin( minP.m_z, maxP.m_z );

			m_vMax.m_x = Math::GetMax( minP.m_x, maxP.m_x );
			m_vMax.m_y = Math::GetMax( minP.m_y, maxP.m_y );
			m_vMax.m_z = Math::GetMax( minP.m_z, maxP.m_z );
#ifdef DRAWAABBOX
			NewRect3D( &m_Rect, GetPoint(PS_ULB), GetPoint(PS_DRF), Color::Red );
#endif
		}
		~aabbox3d()
		{
#ifdef DRAWAABBOX
			SAFE_DELETE( m_Rect );
#endif
		}
		/*只为debug*/
#ifdef DRAWAABBOX
		void Render(Engine& engine) const
		{
			RenderRect3D( engine, *m_Rect );
		}
		void SetRenderTransform( const Matrix44f& trf )
		{
			SetTransform( *m_Rect, trf );
		}
#endif
		BOHGE_FORCEINLINE void Reset() //初始化
		{
			m_isIdentitat = true;
			m_vMin = vector3<T>::Zero();
			m_vMax = vector3<T>::Zero();
#ifdef DRAWAABBOX
			ResetRect3D( *m_Rect, GetPoint(PS_ULB), GetPoint(PS_DRF) );
#endif
		}
		BOHGE_FORCEINLINE const vector3<T>& GetMin() const
		{
			return m_vMin;
		}
		BOHGE_FORCEINLINE const vector3<T>& GetMax() const
		{
			return m_vMax;
		}
		BOHGE_FORCEINLINE void SetMin( const vector3<T>& v )
		{
			m_vMin = v;
#ifdef DRAWAABBOX
			ResetRect3D( *m_Rect, GetPoint(PS_ULB), GetPoint(PS_DRF) );
#endif
		}
		BOHGE_FORCEINLINE void SetMax( const vector3<T>& v )
		{
			m_vMax = v;
#ifdef DRAWAABBOX
			ResetRect3D( *m_Rect, GetPoint(PS_ULB), GetPoint(PS_DRF) );
#endif
		}
		BOHGE_FORCEINLINE void MoveDir(const vector3<T>& dir)
		{
			m_vMin += dir;
			m_vMax += dir;
#ifdef DRAWAABBOX
			ResetRect3D( *m_Rect, GetPoint(PS_ULB), GetPoint(PS_DRF) );
#endif
		}
		BOHGE_FORCEINLINE void SetCenter( const vector3<T>& c )
		{
			vector3<T> olc = ( m_vMax + m_vMin ) * 0.5f;
			vector3<T> dir = c - olc;
			m_vMin += dir;
			m_vMax += dir;
#ifdef DRAWAABBOX
			ResetRect3D( *m_Rect, GetPoint(PS_ULB), GetPoint(PS_DRF) );
#endif
		}
		BOHGE_FORCEINLINE vector3<T> GetCenter() const
		{
			return ( m_vMax + m_vMin ) * 0.5f;
		}
		BOHGE_FORCEINLINE vector3<T> GetHalfSize() const
		{
			return (m_vMax - m_vMin) * 0.5;
		}
		BOHGE_FORCEINLINE T MaxEdge() const //最长得一个边
		{
			vector3<T> edge = m_vMax - m_vMin;
			edge.m_x = Math::Abs( edge.m_x );
			edge.m_y = Math::Abs( edge.m_y );
			edge.m_z = Math::Abs( edge.m_z );
			return Math::GetMax( edge.m_x, Math::GetMax( edge.m_y, edge.m_z) );
		}
		BOHGE_FORCEINLINE aabbox3d operator* (const vector3<T>& scale ) const
		{
			vector3<T> center = GetCenter();
			vector3<T> size = GetHalfSize();
			size *= scale;
			//注意旋转以后的顶点位置的前后有所变化，需要更改构造方式，此处会影响相交的判断
			return aabbox3d<T>( center - size , center + size );
		}
		BOHGE_FORCEINLINE aabbox3d operator* (const Matrix44<T>& input ) const
		{
			aabbox3d<T> temp;
			for ( int i = PS_ULB; i <= PS_DLF ; i ++)
			{
				vector4< T > point = vector4<T>( GetPoint( static_cast<Points>(i) ) );
				temp.Contains( vector3<T>( point * input ) );
			}
			return temp;
			//vector4<T> vmin( m_vMin );
			//vector4<T> vmax( m_vMax );
			//vmin = vmin * input;
			//vmax = vmax * input;
			////注意旋转以后的顶点位置的前后有所变化，需要更改构造方式，此处会影响相交的判断
			//return aabbox3d<T>( vector3f( vmin ), vector3f( vmax ) );
		}
		BOHGE_FORCEINLINE const aabbox3d& operator*= (const Matrix44<T>& input )
		{
			Reset();
			for ( int i = PS_ULB; i <= PS_DLF ; i ++)
			{
				vector4<T>point = vector4<T>( GetPoint( static_cast<Points>(i) ) );
				Contains( vector3<T>( point * input ) );
			}
			return *this;
		}
		BOHGE_FORCEINLINE const aabbox3d& operator *= ( const T& r )
		{
			m_vMin *= r;
			m_vMax *= r;
			return *this;
		}
		//从vertex中计算绑定盒
	public:
		BOHGE_FORCEINLINE void Contains(const vector3<T>& pos)
		{
			if( m_isIdentitat )
			{
				m_isIdentitat = false;
				m_vMin = pos;
				m_vMax = pos;
			}
			else
			{
				if( pos.m_x < m_vMin.m_x)
				{
					m_vMin.m_x = pos.m_x;
				}
				if( pos.m_y < m_vMin.m_y)
				{
					m_vMin.m_y = pos.m_y;
				}
				if( pos.m_z < m_vMin.m_z)
				{
					m_vMin.m_z = pos.m_z;
				}

				if( pos.m_x > m_vMax.m_x)
				{
					m_vMax.m_x = pos.m_x;
				}
				if( pos.m_y > m_vMax.m_y)
				{
					m_vMax.m_y = pos.m_y;
				}
				if( pos.m_z > m_vMax.m_z)
				{
					m_vMax.m_z = pos.m_z;
				}
			}
#ifdef DRAWAABBOX
			ResetRect3D( *m_Rect, GetPoint(PS_ULB), GetPoint(PS_DRF) );
#endif
		}
		BOHGE_FORCEINLINE void Contains(const aabbox3d<T>& box)
		{
			this->Contains( box.m_vMax );
			this->Contains( box.m_vMin );
		}
		//与包围盒相交
		BOHGE_FORCEINLINE bool Intersection(const aabbox3d<T>& box) const
		{
			vector3<T> cen = GetCenter() - box.GetCenter();
			vector3<T> hs = GetHalfSize() + box.GetHalfSize();
			return ( Math::Abs(cen.m_x) <= hs.m_x) && (Math::Abs(cen.m_y) <= hs.m_y) && (Math::Abs(cen.m_z) <= hs.m_z );
		}
		//与点相交
		BOHGE_FORCEINLINE bool Intersection(const vector3<T>& pos) const
		{
			if( pos.m_x < m_vMin.m_x ) return false;
			if( pos.m_y < m_vMin.m_y ) return false;
			if( pos.m_z < m_vMin.m_z ) return false;

			if( pos.m_x > m_vMax.m_x ) return false;
			if( pos.m_y > m_vMax.m_y ) return false;
			if( pos.m_z > m_vMax.m_z ) return false;
			return true;
		}
		/*次检测方法检测优先性，既计算首先碰撞的面*/
		BOHGE_FORCEINLINE bool Intersection( const Ray<T>& ray ) const
		{
			if( Intersection( ray.GetRayOrigin() ) )//如果观察点在box里面
			{
				return true;
			}
			//依次检查各面的相交情况
			float t;
			vector3<T> hitpoint;
			//前面
			if( ray.GetRayOrigin().m_z > m_vMax.m_z && ray.GetRayDirection().m_z < 0 )
			{
				t = ( m_vMax.m_z - ray.GetRayOrigin().m_z ) / ray.GetRayDirection().m_z;//计算投射到该平面的距离
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					//如果这个hitpoint在这个平面内
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
					{
						return true;
					}
				}
			}
			//左面
			if( ray.GetRayOrigin().m_x < m_vMin.m_x && ray.GetRayDirection().m_x > 0 )
			{
				t = ( m_vMin.m_x - ray.GetRayOrigin().m_x ) / ray.GetRayDirection().m_x;
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					if(		hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
					{
						return true;
					}
				}
			}
			//后面
			if( ray.GetRayOrigin().m_z < m_vMin.m_z && ray.GetRayDirection().m_z > 0 )
			{
				t = ( m_vMin.m_z - ray.GetRayOrigin().m_z ) / ray.GetRayDirection().m_z;
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
					{
						return true;
					}
				}
			}
			//右面
			if( ray.GetRayOrigin().m_x > m_vMax.m_x && ray.GetRayDirection().m_x < 0 )
			{
				t = ( m_vMax.m_x - ray.GetRayOrigin().m_x ) / ray.GetRayDirection().m_x;
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					if(		hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
					{
						return true;
					}
				}
			}
			//上面
			if( ray.GetRayOrigin().m_y > m_vMax.m_y && ray.GetRayDirection().m_y < 0 )
			{
				t = ( m_vMax.m_y - ray.GetRayOrigin().m_y ) / ray.GetRayDirection().m_y;
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z )
					{
						return true;
					}
				}
			}
			//下面
			if( ray.GetRayOrigin().m_y < m_vMin.m_y && ray.GetRayDirection().m_y > 0 )
			{
				t = ( m_vMin.m_y - ray.GetRayOrigin().m_y ) / ray.GetRayDirection().m_y;
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z )
					{
						return true;
					}
				}
			}
			return false;
		}
		/*次检测方法检测优先性，既计算首先碰撞的面*/
		BOHGE_FORCEINLINE bool Intersection( const Line<T>& line ) const
		{
			if( Intersection( line.GetOrigin() ) )//如果观察点在box里面
			{
				return true;
			}
			//依次检查各面的相交情况
			float t;
			vector3<T> hitpoint;
			//前面
			if( line.GetOrigin().m_z > m_vMax.m_z && line.GetDirection().m_z < 0 )
			{
				t = ( m_vMax.m_z - line.GetOrigin().m_z ) / line.GetDirection().m_z;//计算投射到该平面的距离
				if( t > 0 && t < line.GetLength() )
				{
					hitpoint = line.GetOrigin() + line.GetDirection() * t;
					//如果这个hitpoint在这个平面内
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
					{
						return true;
					}
				}
			}
			//左面
			if( line.GetOrigin().m_x < m_vMin.m_x && line.GetDirection().m_x > 0 )
			{
				t = ( m_vMin.m_x - line.GetOrigin().m_x ) / line.GetDirection().m_x;
				if( t > 0 && t < line.GetLength() )
				{
					hitpoint = line.GetOrigin() + line.GetDirection() * t;
					if(		hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
					{
						return true;
					}
				}
			}
			//后面
			if( line.GetOrigin().m_z < m_vMin.m_z && line.GetDirection().m_z > 0 )
			{
				t = ( m_vMin.m_z - line.GetOrigin().m_z ) / line.GetDirection().m_z;
				if( t > 0 && t < line.GetLength() )
				{
					hitpoint = line.GetOrigin() + line.GetDirection() * t;
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
					{
						return true;
					}
				}
			}
			//右面
			if( line.GetOrigin().m_x > m_vMax.m_x && line.GetDirection().m_x < 0 )
			{
				t = ( m_vMax.m_x - line.GetOrigin().m_x ) / line.GetDirection().m_x;
				if( t > 0 && t < line.GetLength() )
				{
					hitpoint = line.GetOrigin() + line.GetDirection() * t;
					if(		hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
					{
						return true;
					}
				}
			}
			//上面
			if( line.GetOrigin().m_y > m_vMax.m_y && line.GetDirection().m_y < 0 )
			{
				t = ( m_vMax.m_y - line.GetOrigin().m_y ) / line.GetDirection().m_y;
				if( t > 0 && t < line.GetLength() )
				{
					hitpoint = line.GetOrigin() + line.GetDirection() * t;
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z )
					{
						return true;
					}
				}
			}
			//下面
			if( line.GetOrigin().m_y < m_vMin.m_y && line.GetDirection().m_y > 0 )
			{
				t = ( m_vMin.m_y - line.GetOrigin().m_y ) / line.GetDirection().m_y;
				if( t > 0 && t < line.GetLength() )
				{
					hitpoint = line.GetOrigin() + line.GetDirection() * t;
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z )
					{
						return true;
					}
				}
			}
			return false;
		}
		//与球体相交
		BOHGE_FORCEINLINE bool Intersection(aabsphere<T> const & sphere) const
		{
			if( Intersection( sphere.GetCenter() ) )
			{
				return true;
			}
			vector3<T> half_size = this->GetHalfSize();
			vector3<T> d = sphere.GetCenter() - this->GetCenter();
			vector3<T> closest_point_on_obb = this->GetCenter();
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

			vector3<T> v = closest_point_on_obb - sphere.GetCenter();
			return v.Dot(v) <= sphere.MaxRadiusSq();
		}
		//取得顶点
		BOHGE_FORCEINLINE vector3<T> GetPoint( Points p ) const
		{
			switch ( p )
			{
			case PS_ULB: return vector3<T>( m_vMin.m_x, m_vMax.m_y, m_vMin.m_z );
			case PS_ULF: return vector3<T>( m_vMin.m_x, m_vMax.m_y, m_vMax.m_z );
			case PS_URF: return m_vMax;
			case PS_URB: return vector3<T>( m_vMax.m_x, m_vMax.m_y, m_vMin.m_z );
			case PS_DRF: return vector3<T>( m_vMax.m_x, m_vMin.m_y, m_vMax.m_z );
			case PS_DRB: return vector3<T>( m_vMax.m_x, m_vMin.m_y, m_vMin.m_z );
			case PS_DLB: return m_vMin;
			case PS_DLF: return vector3<T>( m_vMin.m_x, m_vMin.m_y, m_vMax.m_z );
			}
			return vector3<T>();
		}
		//取得绑定盒子的面
		BOHGE_FORCEINLINE Plane<T> GetPlane( BoxPlane bp ) const
		{
			//switch ( bp )
			//{
			//case BP_FRONT: return Plane<T>( m_vMax,
			//								vector3<T>( m_vMin.m_x, m_vMin.m_y, m_vMax.m_z ),
			//								vector3<T>( m_vMax.m_x, m_vMax.m_y, m_vMax.m_z ) );
			//case BP_RIGHT: return Plane<T>( m_vMax,
			//								vector3<T>( m_vMax.m_x, m_vMax.m_y, m_vMax.m_z ),
			//								vector3<T>( m_vMax.m_x, m_vMin.m_y, m_vMin.m_z ) );
			//case BP_BACK: return Plane<T>( m_vMin,
			//								vector3<T>( m_vMax.m_x, m_vMax.m_y, m_vMin.m_z ),
			//								vector3<T>( m_vMin.m_x, m_vMin.m_y, m_vMin.m_z ) );
			//case BP_LEFT: return Plane<T>( m_vMin,
			//								vector3<T>( m_vMin.m_x, m_vMax.m_y, m_vMax.m_z ),
			//								vector3<T>( m_vMin.m_x, m_vMin.m_y, m_vMin.m_z ) );
			//case BP_UP: return Plane<T>( m_vMin,
			//								vector3<T>( m_vMin.m_x, m_vMax.m_y, m_vMax.m_z ),
			//								vector3<T>( m_vMax.m_x, m_vMax.m_y, m_vMin.m_z ) );
			//case BP_DOWN: return Plane<T>( m_vMax,
			//								vector3<T>( m_vMin.m_x, m_vMin.m_y, m_vMax.m_z ),
			//								vector3<T>( m_vMax.m_x, m_vMin.m_y, m_vMin.m_z ) );
			//}
			switch ( bp )
			{
			case BP_FRONT: return Plane<T>( GetPoint(PS_DRF), GetPoint(PS_URF), GetPoint(PS_DLF) );
			case BP_RIGHT: return Plane<T>( GetPoint(PS_DRF), GetPoint(PS_URF), GetPoint(PS_DRB) );
			case BP_BACK: return Plane<T>( GetPoint(PS_ULB), GetPoint(PS_URB), GetPoint(PS_DLB) );
			case BP_LEFT: return Plane<T>( GetPoint(PS_ULB), GetPoint(PS_ULF), GetPoint(PS_DLB) );
			case BP_UP: return Plane<T>( GetPoint(PS_ULB), GetPoint(PS_ULF), GetPoint(PS_URB) );
			case BP_DOWN: return Plane<T>( GetPoint(PS_DRF), GetPoint(PS_DLF), GetPoint(PS_DRB) );
			}
			return Plane<T>();
		}
		//计算相交点,注意，如果点在包围盒内部，他会直接返回原点，这是个体的概念，绑定盒是个体
		BOHGE_FORCEINLINE bool IntersectPointWithRay( const Ray<T>& ray, vector3<T>& out ) const
		{
			if( Intersection( ray.GetRayOrigin() ) )//如果观察点在box里面
			{
				out = ray.GetRayOrigin();
				return true;
			}
			//依次检查各面的相交情况
			float t;
			float len = 0.0f;
			bool isInster = false;
			vector3<T> hitpoint;
			//前面
			if( ray.GetRayOrigin().m_z > m_vMax.m_z && ray.GetRayDirection().m_z < 0 )
			{
				t = ( m_vMax.m_z - ray.GetRayOrigin().m_z ) / ray.GetRayDirection().m_z;//计算投射到该平面的距离
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					//如果这个hitpoint在这个平面内
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
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
			if( ray.GetRayOrigin().m_x < m_vMin.m_x && ray.GetRayDirection().m_x > 0 )
			{
				t = ( m_vMin.m_x - ray.GetRayOrigin().m_x ) / ray.GetRayDirection().m_x;
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					if(		hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
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
			if( ray.GetRayOrigin().m_z < m_vMin.m_z && ray.GetRayDirection().m_z > 0 )
			{
				t = ( m_vMin.m_z - ray.GetRayOrigin().m_z ) / ray.GetRayDirection().m_z;
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
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
			if( ray.GetRayOrigin().m_x > m_vMax.m_x && ray.GetRayDirection().m_x < 0 )
			{
				t = ( m_vMax.m_x - ray.GetRayOrigin().m_x ) / ray.GetRayDirection().m_x;
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					if(		hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z
						&&	hitpoint.m_y < m_vMax.m_y &&	hitpoint.m_y > m_vMin.m_y )
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
			if( ray.GetRayOrigin().m_y > m_vMax.m_y && ray.GetRayDirection().m_y < 0 )
			{
				t = ( m_vMax.m_y - ray.GetRayOrigin().m_y ) / ray.GetRayDirection().m_y;
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z )
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
			if( ray.GetRayOrigin().m_y < m_vMin.m_y && ray.GetRayDirection().m_y > 0 )
			{
				t = ( m_vMin.m_y - ray.GetRayOrigin().m_y ) / ray.GetRayDirection().m_y;
				if( t > 0 )
				{
					hitpoint = ray.GetRayOrigin() + ray.GetRayDirection() * t;
					if(		hitpoint.m_x > m_vMin.m_x &&	hitpoint.m_x < m_vMax.m_x
						&&	hitpoint.m_z > m_vMin.m_z &&	hitpoint.m_z < m_vMax.m_z )
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
	};


	typedef aabbox3d<float> aabbox3df;

}