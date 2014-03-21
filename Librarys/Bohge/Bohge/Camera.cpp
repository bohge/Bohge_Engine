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



/////////////////////////////////////////
//			State Manage class		   //
/////////////////////////////////////////
#include "ActionManager.h"
#include "Camera.h"
#include "3DMath.h"
#include "Predefine.h"
#include "Device.h"
#include "Engine.h"
#ifdef _DEBUG
#include "Shapes.h"
#endif


namespace BohgeEngine
{
#define BAIS 0.98

	//-------------------------------------------------------------------
	ICamera::ICamera()
		:m_isChange(false),
		m_isDirtyFrustum(false),
		m_Fov(0.0f),
		m_Aspect(0.0f),
		m_RotateSpeed(0),
		m_ZoomSpeed( 0 ),
		m_MoveSpeed(0),
		m_isNearFarChange(false),
		m_isDirtyRotation(false),
		m_isDirtyTransform(false),
		m_isDirtyViewProjectBias(false),
		m_isDirtyUnView(false),
		m_isDirtyUnProject(false),
		m_isDirtyUnViewProject(false)
	{
	}
	//-------------------------------------------------------------------
	ICamera::~ICamera()
	{
	}
	//-------------------------------------------------------------------
	void ICamera::_DoCopy( const ICamera& rhs )
	{
		//memcpy( this, &rhs, sizeof(ICamera) ); //不能用，由于event
		m_Rotation = rhs.m_Rotation;
		m_Near = rhs.m_Near;
		m_Far = rhs.m_Far;
		m_Middle = rhs.m_Middle;
		m_Fov = rhs.m_Fov;
		m_Aspect = rhs.m_Aspect;
		m_NearQ_Q = rhs.m_NearQ_Q;
		m_LineDepthParam = rhs.m_LineDepthParam;
		m_LineDepthParamBias = rhs.m_LineDepthParamBias;
		m_isChange = rhs.m_isChange;
		m_Project = rhs.m_Project;
		m_View = rhs.m_View;
		m_ViewProject = rhs.m_ViewProject;
		m_ProjectBias = rhs.m_ProjectBias;
		m_ViewProjectBias = rhs.m_ViewProjectBias;
		m_UnView = rhs.m_UnView;
		m_UnProject = rhs.m_UnProject;
		m_UnViewProject = rhs.m_UnViewProject;
		m_Position = rhs.m_Position;
		m_LookAt = rhs.m_LookAt;
		m_Up = rhs.m_Up;
		m_ViewDirction = rhs.m_ViewDirction;
		m_isDirtyFrustum = rhs.m_isDirtyFrustum;
		m_Frustum = rhs.m_Frustum;
		m_isNearFarChange = rhs.m_isNearFarChange;
		m_isDirtyRotation = rhs.m_isDirtyRotation;
		m_isDirtyTransform = rhs.m_isDirtyTransform;
		m_isDirtyViewProjectBias = rhs.m_isDirtyViewProjectBias;
		m_isDirtyUnView = rhs.m_isDirtyUnView;
		m_isDirtyUnProject = rhs.m_isDirtyUnProject;
		m_isDirtyUnViewProject = rhs.m_isDirtyUnViewProject;
	}
	//-------------------------------------------------------------------
	void ICamera::CreatePerspectiveProjection(float near, float farClip)
	{
		vector4d size = Engine::Instance().GetDevice()->GetViewprot();
		float aspect = static_cast<float>(size.m_z-size.m_x) / static_cast<float>(size.m_w-size.m_y);
		this->CreatePerspectiveProjection( PI/4, aspect, near, farClip);
	}
	//-------------------------------------------------------------------
	void ICamera::CreatePerspectiveProjection(float radian, float aspect, float nearClip, float farClip)
	{
		m_isNearFarChange = true;
		m_Fov = radian;
		m_Aspect = aspect;
		m_Near = nearClip;
		m_Far = farClip;
		m_Middle = m_Near + ( m_Far - m_Near ) * 0.5;
		m_NearQ_Q.m_y = m_Far / ( m_Far - m_Near );
		m_NearQ_Q.m_x = m_Near * m_NearQ_Q.m_y;
		m_LineDepthParam.m_x = m_Near;
		m_LineDepthParam.m_y = 1.0 / ( m_Far - m_Near );
		m_LineDepthParam.m_z = m_Far - m_Near;

		m_LineDepthParamBias.m_x = m_Near;
		m_LineDepthParamBias.m_y = 1.0 / ( m_Far*BAIS - m_Near );
		m_LineDepthParamBias.m_z = m_Far*BAIS - m_Near;
		//float SinFov = sin( 0.5f * radian );
		//float CosFov = cos( 0.5f * radian );
		//float h = CosFov / SinFov;
		float h = 1.0f / Math::Tan( radian * 0.5f );
		float w  = h / aspect;
		float p1 = -( farClip ) / ( farClip - nearClip );
		float p2 = -( farClip * nearClip ) / ( farClip - nearClip );

		//farClip += 0.01f * ( farClip - nearClip ) * nearClip / farClip ;
		farClip *= BAIS;
		float p1b = -( farClip ) / ( farClip - nearClip );
		float p2b = -( farClip * nearClip ) / ( farClip - nearClip );

		m_Project = Matrix44f(  w, 0.0, 0.0, 0.0,
			0.0,   h, 0.0, 0.0,
			0.0, 0.0,  p1, -1.0f,
			0.0, 0.0,  p2, 0.0);
		m_ProjectBias = Matrix44f(  w, 0.0, 0.0, 0.0,
			0.0,   h, 0.0, 0.0,
			0.0, 0.0, p1b, -1.0f,
			0.0, 0.0, p2b, 0.0);
		m_isChange = true;
	}
	//-------------------------------------------------------------------
	void ICamera::CreateOrthographiProjection( float n, float f )
	{
		vector4d size = Engine::Instance().GetDevice()->GetViewprot();
		this->CreateOrthographiProjection( size.m_z-size.m_x, size.m_w-size.m_y, n, f );
	}
	//-------------------------------------------------------------------
	void ICamera::CreateOrthographiProjection(float w, float h, float n, float f)
	{
		//详细请见
		//http://en.wikipedia.org/wiki/Orthographic_projection_(geometry)
		//http://www.songho.ca/opengl/gl_projectionmatrix.html
		//连接中的矩阵为列主序，我的引擎中使用的是行主徐
		m_isNearFarChange = true;
		m_Near = n;
		m_Far = f;
		m_Middle = m_Near + ( m_Far - m_Near ) * 0.5;
		m_NearQ_Q.m_y = m_Far / ( m_Far - m_Near );
		m_NearQ_Q.m_x = m_Near * m_NearQ_Q.m_y;

		m_LineDepthParam.m_x = m_Near;
		m_LineDepthParam.m_y = 1.0 / ( m_Far - m_Near );
		m_LineDepthParam.m_z = m_Far - m_Near;

		m_LineDepthParamBias.m_x = m_Near;
		m_LineDepthParamBias.m_y = 1.0 / ( m_Far*BAIS - m_Near );
		m_LineDepthParamBias.m_z = m_Far*BAIS - m_Near;

		//float r = 2.0f / w;
		//float t = 2.0f / h;
		//float p1 = -2 / ( f - n );
		//float p2 = - ( f + n ) / ( f - n );

		//f += 0.01f * ( f - n ) * n / f ;
		//float p1b = -2 / ( f - n );
		//float p2b = - ( f + n ) / ( f - n );

		//m_Project = Matrix44f(  r, 0.0, 0.0, 0.0,
		//	0.0,   t, 0.0, 0.0,
		//	0.0, 0.0,  p1, 0.0,
		//	0.0, 0.0,  p2, 1.0);
		//m_ProjectBias = Matrix44f(  r, 0.0, 0.0, 0.0,
		//	0.0,   t, 0.0, 0.0,
		//	0.0, 0.0, p1b, 0.0,
		//	0.0, 0.0, p2b, 1.0);


		float const left = -w / 2.0f;
		float const right = w / 2.0f;
		float const top = h / 2.0f;
		float const bottom = -h / 2.0f;

		float const q = 1.0f / (f - n);
		float const invWidth = 1.0f / (right - left);
		float const invHeight = 1.0f / (top - bottom);

		m_Project = Matrix44f(
			invWidth + invWidth,		0,								0,					0,
			0,							invHeight + invHeight,			0,					0,
			0,							0,								-q,					0,//这个q应该是-q,还是左右手坐标系的问题，如果是q的话会导致问题
			-(left + right) * invWidth,	-(top + bottom) * invHeight,	-n * q,				1);

		f *= BAIS;
		float const qb = 1.0f / (f - n);
		m_ProjectBias = Matrix44f(
			invWidth + invWidth,		0,								0,					0,
			0,							invHeight + invHeight,			0,					0,
			0,							0,								-qb,					0,
			-(left + right) * invWidth,	-(top + bottom) * invHeight,	-n * qb,				1);


		m_isChange = true;
	}
	//-------------------------------------------------------------------
	void ICamera::_LookAt( const vector3f& position, const vector3f& lookAt, const vector3f& up )
	{
		m_Position = position ;
		m_LookAt = lookAt;
		m_Up = up;
		m_ViewDirction = m_LookAt - m_Position;
		m_ViewDirction.NormalizeSelf();
		m_isChange = true;
		m_isDirtyFrustum = true;
	}
	////-------------------------------------------------------------------
	//void Camera::CreateView(vector3f& position, vector3f& dir, vector3f& up)
	//{
	//	vector3f ZAxis = dir;
	//    vector3f XAxis = Cross( up, ZAxis );
	//    vector3f YAxis = Cross( ZAxis, XAxis );
	//	XAxis.NormalizeSelf();
	//    YAxis.NormalizeSelf();
	//    ZAxis.NormalizeSelf();
	//
	//    m_View = Matrix44f(	-XAxis.m_x, +YAxis.m_x, -ZAxis.m_x, 0.0f,
	//							-XAxis.m_y, +YAxis.m_y, -ZAxis.m_y, 0.0f,
	//							-XAxis.m_z, +YAxis.m_z, -ZAxis.m_z, 0.0f,
	//							+Dot( XAxis, position ), -Dot( YAxis, position ), +Dot( ZAxis, position ), 1.0f);
	// 	m_isChange = true;
	//}
	//-------------------------------------------------------------------
	void ICamera::Update ()
	{
		//*m_pView = Matrix44f::CreateRotateMatrix(vector3f(0,1,0),0.01) * *m_pView;
		//m_isChange = true;
		if( m_isChange )
		{
			m_EventOnCameraUpdate.Multicast( *this );
			m_isChange = false;
			//计算新的view矩阵
			vector3f ZAxis = m_Position - m_LookAt;
			vector3f XAxis = m_Up.Cross( ZAxis );
			vector3f YAxis = ZAxis.Cross( XAxis );
			XAxis.NormalizeSelf();
			YAxis.NormalizeSelf();
			ZAxis.NormalizeSelf();
			m_View = Matrix44f(	XAxis.m_x, YAxis.m_x, ZAxis.m_x, 0.0f,
								XAxis.m_y, YAxis.m_y, ZAxis.m_y, 0.0f,
								XAxis.m_z, YAxis.m_z, ZAxis.m_z, 0.0f,
								-XAxis.Dot( m_Position ), -YAxis.Dot( m_Position ), -ZAxis.Dot( m_Position ), 1.0f);
			m_ViewProject = (m_View) * (m_Project);

			//计算其他矩阵
			m_isDirtyRotation = true;
			m_isDirtyTransform = true;
			m_isDirtyViewProjectBias = true;
			m_isDirtyUnView = true;
			m_isDirtyUnProject = true;
			m_isDirtyUnViewProject = true;
			//m_Transform = vector3f( m_View.a41, m_View.a42, m_View.a43 );
			//m_Rotation = Matrix33f(m_View);
			//m_ViewProjectBias = (m_View) * (m_ProjectBias);
			//m_View.GetInverse(m_UnView, false);
			//m_Project.GetInverse(m_UnProject);
			//m_UnViewProject = m_UnProject * m_UnView;
		}
	}
	//-------------------------------------------------------------------
	void ICamera::PickRay(const vector2f& secen, Rayf& out)
	{
		//if( m_isChange )
		//{
		//	Update();
		//}
		//http://stackoverflow.com/questions/2093096/implementing-ray-picking
		//First you need to determine where on the nearplane the mouse click happened:
		//
		//rescale the window coordinates (0..640,0..480) to [-1,1], with (-1,-1) at the bottom-left corner and (1,1) at the top-right.
		//'undo' the projection by multiplying the scaled coordinates by what I call the 'unview' matrix: unview = (P * M).inverse() = M.inverse() * P.inverse(), where M is the ModelView matrix and P is the projection matrix.
		//Then determine where the camera is in worldspace, and draw a ray starting at the camera and passing through the point you found on the nearplane.
		//
		//The camera is at M.inverse().col(4), i.e. the final column of the inverse ModelView matrix.
		//
		//Final pseudocode:
		//
		//normalised_x = 2 * mouse_x / win_width - 1
		//normalised_y = 1 - 2 * mouse_y / win_height
		//// note the y pos is inverted, so +y is at the top of the screen
		//
		//unviewMat = (projectionMat * modelViewMat).inverse()
		//
		//near_point = unviewMat * Vec(normalised_x, normalised_y, 0, 1)
		//camera_pos = ray_origin = modelViewMat.inverse().col(4)
		//ray_dir = near_point - camera_pos


		//vector4f ray(secen.m_x, secen.m_y , 1.0 , 1.0 );
		////ray *= (*m_pUnProject);//这两部等效于ray *= ( (*m_pUnProject) * (*m_pUnView) );
		////ray *= (*m_pUnView);
		//ray *= m_UnViewProject;
		//ray /= ray.m_w;
		////vector4f camera(m_pUnView->a41,m_pUnView->a42,m_pUnView->a43,m_pUnView->a44);
		////camera /= camera->m_w;
		////ray -= camera;
		//ray -= vector4f( m_Position );

		//vector3f reslut(ray.m_x, ray.m_y, ray.m_z);
		//reslut.NormalizeSelf();
		//out.m_RayOrig = m_Position;
		//out.m_RayDir = reslut;

		vector4f rayb(secen.m_x, secen.m_y , 0.0 , 1.0 );//理论上z这个值应该是0，但是这样算出来的Y值偏小和aabb射线拾取不统一，故该做-1，只放大初始位置
		vector4f raye(secen.m_x, secen.m_y , 1.0 , 1.0 );
		vector4f ray2;
		rayb *= GetUnViewProj();
		ASSERT( 0 != rayb.m_w );
		rayb /= rayb.m_w;
		raye *= GetUnViewProj();
		ASSERT( 0 != raye.m_w );
		raye /= raye.m_w;

		ray2 = raye - rayb;
		vector3f reslut2(ray2.m_x, ray2.m_y, ray2.m_z);
		reslut2.NormalizeSelf();
		out.SetRayOrigin( vector3f(rayb) );
		out.SetRayDirection( reslut2 );
	}
	//-------------------------------------------------------------------
	void ICamera::OnActionInput( ActionManager& am )
	{
	}
	//-------------------------------------------------------------------
	void ICamera::Move( const vector3f& disvector )
	{
		m_Position += disvector;
		m_LookAt += disvector;
		this->LookAt( m_Position, m_LookAt, m_Up );
	}
	//-------------------------------------------------------------------
	void ICamera::AttachCamera()
	{
		Engine::Instance()._AttachCamera( this );
	}
	//-------------------------------------------------------------------
	void ICamera::SetForward(const vector3f& forward)
	{
		LookAt( m_Position, m_Position + forward, m_Up );
	}
	//-------------------------------------------------------------------
	void ICamera::SetPosition( const vector3f& position )
	{
		LookAt( position, position+m_ViewDirction, m_Up );
	}

	void ICamera::SetMoveRotateZoom( float mov, float rot, float zoom )
	{
		m_MoveSpeed = mov;
		m_RotateSpeed = rot;
		m_ZoomSpeed = zoom;
	}




	/////////////////////////////////
	//		TrackballCamera		   //
	/////////////////////////////////
	//--------------------------------------------------------------------------------------------------------------------------------------
	TrackballCamera::TrackballCamera( )
		:m_isReverse(false)
	{

	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void TrackballCamera::_OnlookAt( )
	{
		m_isReverse = false;
		m_Target = this->GetLookAt();
		m_VerticalAixs = this->GetUp().Cross( this->GetForward() );
		m_VerticalAixs.NormalizeSelf();
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void TrackballCamera::OnActionInput( ActionManager& am )
	{
		if( am.isTouch() && am.isMove() )
		{
			if( am.isTowPoint() )
			{
				float d = am.DistanceTowPoint() - am.LastDistanceTowPoint();
				this->ZoomInOut( d );
				//DEBUGLOG("Camera zoom\n",1);
			}
			else
			{
				float xd = -( am.TouchPoint().m_x - am.LastTouchPoint().m_x ) / 2.0;//TouchPoint是(-1,1)区间的
				float yd = -( am.TouchPoint().m_y - am.LastTouchPoint().m_y ) / 2.0;
				this->Rotate(xd,yd);
				//DEBUGLOG("Camera Rotate\n",1);
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void TrackballCamera::Rotate( float xd, float yd )
	{
		Quaternionf q;
		q.RotateAxis( m_VerticalAixs,yd * m_RotateSpeed );
		Matrix44f mat = Math::Transformation<float>(NULL, NULL, NULL, &m_Target, &q, NULL);
		vector3f pos = Math::Transform_coord(this->GetPosition(), mat);

		q.RotateAxis( vector3f(0.0f, this->GetUp().Dot( vector3f(0, 1, 0) ) < 0 ? -1.0f : 1.0f, 0.0f), xd * m_RotateSpeed );
		mat = Math::Transformation<float>(NULL, NULL, NULL, &m_Target, &q, NULL);
		pos =  Math::Transform_coord(pos, mat);
		m_VerticalAixs = Math::Transform_Quat(m_VerticalAixs, q);

		vector3f dir;
		if (m_isReverse)
		{
			dir = pos - m_Target;
		}
		else
		{
			dir = m_Target - pos;
		}
		dir.NormalizeSelf();
		vector3f up = dir.Cross(m_VerticalAixs);
		this->_LookAt(pos, m_Target, up);	
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void TrackballCamera::ZoomInOut( float d )
	{
		vector3f offset = this->GetForward() * ( d * m_ZoomSpeed * 2);
		vector3f pos = this->GetPosition() + offset;

		if ( (m_Target - pos).Dot( this->GetForward() ) <= 0)
		{
			m_isReverse = true;
		}
		else
		{
			m_isReverse = false;
		}
		float lengthpow = (pos-m_Target).LengthPow();
		if ( lengthpow > m_ZoomMaxMinPow.m_x && lengthpow < m_ZoomMaxMinPow.m_y )
		{
			this->_LookAt(pos, m_Target, this->GetUp() );
		}
	}



	/////////////////////////////
	//		StaticCamera	   //
	/////////////////////////////
	//--------------------------------------------------------------------------------------------------------------------------------------
	void StaticCamera::_OnlookAt()
	{

	}





	/////////////////////////
	//		FPSCamera	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------------------------------------------------------
	FPSCamera::FPSCamera()
	{

	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void FPSCamera::_OnlookAt()
	{
		m_VerticalAixs = this->GetUp().Cross( this->GetForward() );
		m_VerticalAixs.NormalizeSelf();
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void FPSCamera::OnActionInput( ActionManager& am )
	{

		if( am.isTouch() && am.isMove() )
		{

			if ( am.isTowPoint() )
			{//视射线方向的移动
				float d = am.DistanceTowPoint() - am.LastDistanceTowPoint();
				float movedis = m_MoveSpeed * d;
				this->Move( vector3f( 0, 0, -movedis ) );
			}
			else
			{//旋转
				float xd = -( am.TouchPoint().m_x - am.LastTouchPoint().m_x ) / 2.0;//TouchPoint是(-1,1)区间的
				float yd = -( am.TouchPoint().m_y - am.LastTouchPoint().m_y ) / 2.0;
				this->Rotate(xd,yd);
				//DEBUGLOG("Camera Rotate\n",1);
			}

		}
		{//平移
			vector3f move;
			bool isMove = false;
			float movedis = m_MoveSpeed * Engine::Instance().GetTimeSpan() / 1000.0f;
			if( am.isAction( ActionManager::ACTION_MOVEFORWARD ) )
			{
				isMove = true;
				move += vector3f( 0, 0, -movedis );
			}
			if( am.isAction( ActionManager::ACTION_MOVEBACK ) )
			{
				isMove = true;
				move += vector3f( 0, 0, movedis );
			}
			if( am.isAction( ActionManager::ACTION_MOVELRIGHT ) )
			{
				isMove = true;
				move += vector3f( movedis, 0, 0 );
			}
			if( am.isAction( ActionManager::ACTION_MOVELEFT ) )
			{
				isMove = true;
				move += vector3f( -movedis, 0, 0 );
			}
			if( isMove )
			{
				this->Move( move );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void FPSCamera::Rotate( float xd, float yd )
	{
		Quaternionf q;
		q.RotateAxis( m_VerticalAixs ,yd * m_RotateSpeed );
		vector3f dir = Math::Transform_Quat( this->GetForward() , q);

		q.RotateAxis( vector3f(0.0f, GetUp().Dot( vector3f(0, 1, 0)) < 0 ? -1.0f : 1.0f, 0.0f) , xd * m_RotateSpeed );
		m_VerticalAixs = Math::Transform_Quat(m_VerticalAixs, q);
		dir = Math::Transform_Quat( dir , q);
		vector3f up = dir.Cross( m_VerticalAixs );
		this->_LookAt(this->GetPosition(), this->GetPosition() + dir, up);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void FPSCamera::Move( const vector3f& dis )
	{
		Matrix33f rot = Matrix33f( this->GetUnView() );
		vector3f movedis = dis * rot;
		this->_LookAt(this->GetPosition() + movedis, this->GetLookAt() + movedis, this->GetUp());
	}





	//--------------------------------------------------------------------------------------------------------------------------------------
	EditCamera::EditCamera()
		:m_isReverse(false)
	{

	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void EditCamera::_OnlookAt()
	{
		m_isReverse = false;
		m_Target = this->GetLookAt();
		m_VerticalAixs = this->GetUp().Cross( this->GetForward() );
		m_VerticalAixs.NormalizeSelf();
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void EditCamera::OnActionInput( ActionManager& am )
	{
		//if( am.isTouch() && am.isMove() )
		//{
		//	if( am.isTowPoint() )
		//	{
		//		float d = am.DistanceTowPoint() - am.LastDistanceTowPoint();
		//		this->ZoomInOut( d );
		//		//DEBUGLOG("Camera zoom\n",1);
		//	}
		//	else
		//	{
		//		float xd = -( am.TouchPoint().m_x - am.LastTouchPoint().m_x ) / 2.0;//TouchPoint是(-1,1)区间的
		//		float yd = -( am.TouchPoint().m_y - am.LastTouchPoint().m_y ) / 2.0;
		//		this->Rotate(xd,yd);
		//		//DEBUGLOG("Camera Rotate\n",1);
		//	}
		//}
		{
			vector3f move;
			bool isMove = false;
			float movedis = m_MoveSpeed * Engine::Instance().GetTimeSpan() / 1000.0f;
			if( am.isAction( ActionManager::ACTION_MOVEFORWARD ) )
			{
				isMove = true;
				move += vector3f( 0, 0, -movedis );
			}
			if( am.isAction( ActionManager::ACTION_MOVEBACK ) )
			{
				isMove = true;
				move += vector3f( 0, 0, movedis );
			}
			if( am.isAction( ActionManager::ACTION_MOVELRIGHT ) )
			{
				isMove = true;
				move += vector3f( movedis, 0, 0 );
			}
			if( am.isAction( ActionManager::ACTION_MOVELEFT ) )
			{
				isMove = true;
				move += vector3f( -movedis, 0, 0 );
			}
			if( isMove )
			{
				this->Move( move );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void EditCamera::Rotate( float xd, float yd )
	{
		Quaternionf q;
		q.RotateAxis( m_VerticalAixs,yd * m_RotateSpeed );
		Matrix44f mat = Math::Transformation<float>(NULL, NULL, NULL, &m_Target, &q, NULL);
		vector3f pos = Math::Transform_coord(this->GetPosition(), mat);

		q.RotateAxis( vector3f(0.0f, GetUp().Dot( vector3f(0, 1, 0)) < 0 ? -1.0f : 1.0f, 0.0f), xd * m_RotateSpeed );
		mat = Math::Transformation<float>(NULL, NULL, NULL, &m_Target, &q, NULL);
		pos =  Math::Transform_coord(pos, mat);
		m_VerticalAixs = Math::Transform_Quat(m_VerticalAixs, q);

		vector3f dir;
		if (m_isReverse)
		{
			dir = pos - m_Target;
		}
		else
		{
			dir = m_Target - pos;
		}
		dir.NormalizeSelf();
		vector3f up = dir.Cross(m_VerticalAixs);
		this->_LookAt(pos, pos + dir, up);	
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void EditCamera::Move( const vector3f& dis )
	{
		Matrix33f rot = Matrix33f( this->GetUnView() );
		vector3f movedis = dis * rot;
		movedis.m_y = 0;
		m_Target += movedis;
		this->_LookAt(this->GetPosition() + movedis, this->GetLookAt() + movedis, this->GetUp());
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void EditCamera::ZoomInOut( float d )
	{
		vector3f offset = this->GetForward() * ( d * m_ZoomSpeed * 2);
		vector3f pos = this->GetPosition() + offset;

		if ( (m_Target - pos).Dot( this->GetForward() ) <= 0)
		{
			m_isReverse = true;
		}
		else
		{
			m_isReverse = false;
		}
		this->_LookAt(pos, pos + this->GetForward(), this->GetUp() );
	}

}