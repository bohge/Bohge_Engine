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

#include <math.h>

#include "ActionManage.h"
#include "Camera.h"
#include "3DMath.h"
#include "Predefine.h"
#include "IDriver.h"
#ifdef _DEBUG
#include "Shapes.h"
#endif


//-------------------------------------------------------------------
ICamera::ICamera()
:m_isChange(false),
m_isDrtyFrustum(false),
m_Fov(0.0f),
m_Aspect(0.0f),
m_RotateSpeed(0),
m_ZoomSpeed( 0 ),
m_MoveSpeed(0),
m_isNearFarChange(false)
{
}
//-------------------------------------------------------------------
ICamera::ICamera( const ICamera& came )
{
	this->_DoCopy( came );
}
//-------------------------------------------------------------------
ICamera::~ICamera()
{
}
//-------------------------------------------------------------------
void ICamera::CreatePerspectiveProjection(float near, float farClip)
{
	vector2d size = IDriver::Instance().GetViewprot();
	float aspect = static_cast<float>(size.m_x) / static_cast<float>(size.m_y);
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
	//float SinFov = sin( 0.5f * radian );
    //float CosFov = cos( 0.5f * radian );
    //float h = CosFov / SinFov;
	float h = 1.0f / tan( radian * 0.5f );
    float w  = h / aspect;
	float p1 = -( farClip ) / ( farClip - nearClip );
	float p2 = -( farClip * nearClip ) / ( farClip - nearClip );

	farClip += 0.01f * ( farClip - nearClip ) * nearClip / farClip ;
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
	const vector2d& size = IDriver::Instance().GetViewprot();
	this->CreateOrthographiProjection( size.m_x, size.m_y, n, f );
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

	f += 0.01f * ( f - n ) * n / f ;
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
	m_isDrtyFrustum = true;
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
		m_isChange = false;
		//计算新的view矩阵
		vector3f ZAxis = m_Position - m_LookAt;
		vector3f XAxis = Math::Cross( m_Up, ZAxis );
		vector3f YAxis = Math::Cross( ZAxis, XAxis );
		XAxis.NormalizeSelf();
		YAxis.NormalizeSelf();
		ZAxis.NormalizeSelf();
		m_View = Matrix44f(	XAxis.m_x, YAxis.m_x, ZAxis.m_x, 0.0f,
							XAxis.m_y, YAxis.m_y, ZAxis.m_y, 0.0f,
							XAxis.m_z, YAxis.m_z, ZAxis.m_z, 0.0f,
							-Math::Dot( XAxis, m_Position ), -Math::Dot( YAxis, m_Position ), -Math::Dot( ZAxis, m_Position ), 1.0f);

		//计算其他矩阵
		m_ViewProject = (m_View) * (m_Project);
		m_ViewProjectBias = (m_View) * (m_ProjectBias);
		m_View.GetInverse(m_UnView, false);
		m_Project.GetInverse(m_UnProject);
		//m_ViewProject.GetInverse( m_UnViewProject, false );
		m_UnViewProject = m_UnProject * m_UnView;
	}
	if( m_isDrtyFrustum )
	{
		m_isDrtyFrustum = false;
		m_Frustum.FromMatrix( m_ViewProject );
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
	////camera /= camera.m_w;
	////ray -= camera;
	//ray -= vector4f( m_Position );

	//vector3f reslut(ray.m_x, ray.m_y, ray.m_z);
	//reslut.NormalizeSelf();
	//out.m_RayOrig = m_Position;
	//out.m_RayDir = reslut;

	vector4f rayb(secen.m_x, secen.m_y , 0.0 , 1.0 );//理论上z这个值应该是0，但是这样算出来的Y值偏小和aabb射线拾取不统一，故该做-1，只放大初始位置
	vector4f raye(secen.m_x, secen.m_y , 1.0 , 1.0 );
	vector4f ray2;
	rayb *= (m_UnViewProject);
	ASSERT( 0 != rayb.m_w );
	rayb /= rayb.m_w;
	raye *= (m_UnViewProject);
	ASSERT( 0 != raye.m_w );
	raye /= raye.m_w;

	ray2 = raye - rayb;
	vector3f reslut2(ray2.m_x, ray2.m_y, ray2.m_z);
	reslut2.NormalizeSelf();
	out.m_RayOrig = rayb;
	out.m_RayDir = reslut2;
}
//-------------------------------------------------------------------
void ICamera::OnActionInput( ActionManage& am )
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
	IDriver::Instance()._AttachCamera( this );
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
//--------------------------------------------------------------------------------------------------------------------------------------
void ICamera::operator=( const ICamera& came )
{
	this->_DoCopy( came );
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ICamera::_DoCopy( const ICamera& came )
{
	memcpy( this, &came, sizeof(ICamera) );
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
	m_VerticalAixs = Math::Cross( this->GetUp(), this->GetForward() );
	m_VerticalAixs.NormalizeSelf();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TrackballCamera::OnActionInput( ActionManage& am )
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

	q.RotateAxis( vector3f(0.0f, Math::Dot(this->GetUp(), vector3f(0, 1, 0)) < 0 ? -1.0f : 1.0f, 0.0f), xd * m_RotateSpeed );
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
	vector3f up = Math::Cross(dir, m_VerticalAixs);
	this->_LookAt(pos, pos + dir, up);	
}
//--------------------------------------------------------------------------------------------------------------------------------------
void TrackballCamera::ZoomInOut( float d )
{
	vector3f offset = this->GetForward() * ( d * m_ZoomSpeed * 2);
	vector3f pos = this->GetPosition() + offset;

	if (Math::Dot(m_Target - pos, this->GetForward() ) <= 0)
	{
		m_isReverse = true;
	}
	else
	{
		m_isReverse = false;
	}
	this->_LookAt(pos, pos + this->GetForward(), this->GetUp() );
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
	m_VerticalAixs = Math::Cross( this->GetUp(), this->GetForward() );
	m_VerticalAixs.NormalizeSelf();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void FPSCamera::OnActionInput( ActionManage& am )
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
		float movedis = m_MoveSpeed * IDriver::Instance().GetTimeSpan() / 1000.0f;
		if( am.isAction( ActionManage::ACTION_MOVEFORWARD ) )
		{
			isMove = true;
			move += vector3f( 0, 0, -movedis );
		}
		if( am.isAction( ActionManage::ACTION_MOVEBACK ) )
		{
			isMove = true;
			move += vector3f( 0, 0, movedis );
		}
		if( am.isAction( ActionManage::ACTION_MOVELRIGHT ) )
		{
			isMove = true;
			move += vector3f( movedis, 0, 0 );
		}
		if( am.isAction( ActionManage::ACTION_MOVELEFT ) )
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

	q.RotateAxis( vector3f(0.0f, Math::Dot( this->GetUp(), vector3f(0, 1, 0)) < 0 ? -1.0f : 1.0f, 0.0f) , xd * m_RotateSpeed );
	m_VerticalAixs = Math::Transform_Quat(m_VerticalAixs, q);
	dir = Math::Transform_Quat( dir , q);
	vector3f up = Math::Cross(dir, m_VerticalAixs);
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
	m_VerticalAixs = Math::Cross( this->GetUp(), this->GetForward() );
	m_VerticalAixs.NormalizeSelf();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void EditCamera::OnActionInput( ActionManage& am )
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
		float movedis = m_MoveSpeed * IDriver::Instance().GetTimeSpan() / 1000.0f;
		if( am.isAction( ActionManage::ACTION_MOVEFORWARD ) )
		{
			isMove = true;
			move += vector3f( 0, 0, -movedis );
		}
		if( am.isAction( ActionManage::ACTION_MOVEBACK ) )
		{
			isMove = true;
			move += vector3f( 0, 0, movedis );
		}
		if( am.isAction( ActionManage::ACTION_MOVELRIGHT ) )
		{
			isMove = true;
			move += vector3f( movedis, 0, 0 );
		}
		if( am.isAction( ActionManage::ACTION_MOVELEFT ) )
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

	q.RotateAxis( vector3f(0.0f, Math::Dot(this->GetUp(), vector3f(0, 1, 0)) < 0 ? -1.0f : 1.0f, 0.0f), xd * m_RotateSpeed );
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
	vector3f up = Math::Cross(dir, m_VerticalAixs);
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

	if (Math::Dot(m_Target - pos, this->GetForward() ) <= 0)
	{
		m_isReverse = true;
	}
	else
	{
		m_isReverse = false;
	}
	this->_LookAt(pos, pos + this->GetForward(), this->GetUp() );
}