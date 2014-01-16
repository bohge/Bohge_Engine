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
//			enter point of game		   //
/////////////////////////////////////////

#pragma once
#include "Frustum.hpp"
#include "3DMath.h"
#include "Ray.h"


//由于投影水面的关系，本帧update中的摄像机更新全部统一在下一帧进行，否则会导致多线程水面计算的问题
//新的摄像机如果带有x轴上的旋转需要从新做watergrid
class LineNode;
class ActionManage;
class ICamera
{
	friend class Game;
private:
	float			m_Near;
	float			m_Far;
	float			m_Middle;
	float			m_Fov;
	float			m_Aspect;
	vector2f		m_NearQ_Q;//给shader的摄像机参数
	vector3f		m_LineDepthParam;//线性深度的参数
	bool			m_isChange;// 摄像机有变化
	Matrix44f		m_Project; //投影矩阵
	Matrix44f		m_View;//试图矩阵
	Matrix44f		m_ViewProject;//投影视图
	Matrix44f		m_ProjectBias;//投影的偏移矩阵
	Matrix44f		m_ViewProjectBias;//投影视图的便宜矩阵
	Matrix44f		m_UnView;//view的逆矩阵
	Matrix44f		m_UnProject;//project的逆矩阵
	Matrix44f		m_UnViewProject;//view,project的逆矩阵

	vector3f		m_Position;//位置
	vector3f		m_LookAt;//看向的点
	vector3f		m_Up;//上轴
	vector3f		m_ViewDirction;//视射线

	//视锥相关
	bool			m_isDrtyFrustum;
	Frustumf		m_Frustum;

	bool			m_isNearFarChange;
protected:
	float			m_MoveSpeed;
	float			m_RotateSpeed;
	float			m_ZoomSpeed;
private:
	void _DoCopy(  const ICamera& came  );
public:
	ICamera();
	ICamera( const ICamera& came );
	virtual ~ICamera();
protected:
	virtual void _OnlookAt( ){};//档外部设置lookat的时候调用_LookAt不会调用
	void _LookAt(const vector3f& position, const vector3f& lookAt, const vector3f& up);
public:
	/*产生透视投影矩阵radian是视角宽度，aspect是窗口宽高比，near是近剪切平面，far是远剪切平面*/
	void CreatePerspectiveProjection(float near, float farClip);
	void CreatePerspectiveProjection(float radian, float aspect, float near, float farClip);
	/*正交投影矩阵，size是正交盒的包围范围，aspect是窗口宽高比，near是近剪切平面，far是远剪切平面*/
	void CreateOrthographiProjection(float w, float h, float n, float f);
	void CreateOrthographiProjection(float n, float f);
	/*传递位置，lookat向量*/
	//void CreateView(vector3f& position, vector3f& dir, vector3f& up);
	void Update( );
	/*传入屏幕空间（opengl坐标系）的点，返回拾取射线*/
	void PickRay(const vector2f& secen, Rayf& out);
	void Move( const vector3f& disvector );//移动一段距离，整体移动
	void AttachCamera();//将摄像机挂在引擎上
	void SetPosition(const vector3f& position);
	void SetForward(const vector3f& forward);
	void SetMoveRotateZoom( float mov, float rot, float zoom );//设置运动速度(unit/s)一个屏幕空间（左滑至右面）内旋转的角度和缩放
	void operator = (  const ICamera& came  );
public:
	virtual void OnActionInput( ActionManage& am );
public:
	BOHGE_FORCEINLINE bool isNearFarChange()
	{
		bool r = m_isNearFarChange;
		m_isNearFarChange = false;
		return r;
	}
	BOHGE_FORCEINLINE float MoveSpeed()
	{
		return m_MoveSpeed;
	}
	BOHGE_FORCEINLINE float RotateSpeed()
	{
		return m_RotateSpeed;
	}
	BOHGE_FORCEINLINE float ZoomSpeed()
	{
		return m_ZoomSpeed;
	}
	BOHGE_FORCEINLINE float Fov() const
	{
		return m_Fov;
	}
	BOHGE_FORCEINLINE float Aspect() const
	{
		return m_Aspect;
	}
	BOHGE_FORCEINLINE void LookAt(const vector3f& position, const vector3f& lookAt, const vector3f& up)
	{
		this->_LookAt(position, lookAt, up);
		this->_OnlookAt( );
	}
	BOHGE_FORCEINLINE const vector3f& GetForward() const
	{
		return m_ViewDirction;
	}
	BOHGE_FORCEINLINE const Matrix44f& GetView() const
	{
		//if( m_isChange )
		//{
		//	Update();
		//}
		return m_View;
	}
	/*返回投影矩阵*/
	BOHGE_FORCEINLINE const Matrix44f& GetProject() const
	{
		//if( m_isChange )
		//{
		//	Update();
		//}
		return m_Project;
	}
	BOHGE_FORCEINLINE const Matrix44f& GetViewProj() const
	{
		//if( m_isChange )
		//{
		//	Update();
		//}
		return m_ViewProject;
	}
	/*当产生z-fighting的时候调用的偏移投影矩阵*/
	BOHGE_FORCEINLINE const Matrix44f& GetViewProjBias() const
	{
		//if( m_isChange )
		//{
		//	Update();
		//}
		return m_ViewProjectBias;
	}
	BOHGE_FORCEINLINE const vector3f& GetPosition() const
	{
		return m_Position;
	}
	BOHGE_FORCEINLINE const vector3f& GetLookAt() const
	{
		return m_LookAt;
	}
	BOHGE_FORCEINLINE const vector3f& GetUp() const
	{
		return m_Up;
	}
	BOHGE_FORCEINLINE float GetNear() const
	{
		return m_Near;
	}
	BOHGE_FORCEINLINE float GetFar() const
	{
		return m_Far;
	}
	BOHGE_FORCEINLINE float GetMiddle() const
	{
		return m_Middle;
	}
	BOHGE_FORCEINLINE const vector2f& GetNearQ_Q() const
	{
		return m_NearQ_Q;
	}
	BOHGE_FORCEINLINE const Matrix44f& GetUnViewProj() const
	{
		//if( m_isChange )
		//{
		//	Update();
		//}
		return m_UnViewProject;
	}
	BOHGE_FORCEINLINE const Matrix44f& GetUnView() const
	{
		//if( m_isChange )
		//{
		//	Update();
		//}
		return m_UnView;
	}
	BOHGE_FORCEINLINE const Matrix44f& GetUnProject() const
	{
		//if( m_isChange )
		//{
		//	Update();
		//}
		return m_UnProject;
	}
	BOHGE_FORCEINLINE const Frustumf& GetFrustum() const//得到视锥
	{
		//if( m_isDrtyFrustum )
		//{
		//	if( m_isChange )
		//	{
		//		Update();
		//	}
		//	m_isDrtyFrustum = false;
		//	m_Frustum.FromMatrix( m_ViewProject );
		//}
		return m_Frustum;
	}
	BOHGE_FORCEINLINE const vector3f& LineDepthParam()
	{
		//线性深度公式 d = ( z - m_LineDepthParam.m_x ) * m_LineDepthParam.m_y;
		//返回实际距离 z = d * m_LineDepthParam.m_z + m_LineDepthParam.m_x;
		return m_LineDepthParam;
	}
};



class StaticCamera : public ICamera
{
protected:
	virtual void _OnlookAt( );//档外部设置lookat的时候调用_LookAt不会调用
};




class TrackballCamera : public ICamera
{
private:
	bool		m_isReverse;
	vector3f	m_Target;
	vector3f	m_VerticalAixs;//竖直移动轴
public:
	TrackballCamera( );
private:
	virtual void _OnlookAt( );//档外部设置lookat的时候调用_LookAt不会调用
public:
	virtual void OnActionInput( ActionManage& am );
public:
	void Rotate( float xd, float yd );//输入范围[-n,n]分别是在围绕right和up轴转动rotSpeed*xd,yd角度
	void ZoomInOut( float d );//放大缩小，同上
};



class FPSCamera : public ICamera
{
private:
	vector3f	m_VerticalAixs;//竖直移动轴
public:
	FPSCamera( );
private:
	virtual void _OnlookAt( );//档外部设置lookat的时候调用_LookAt不会调用
public:
	virtual void OnActionInput( ActionManage& am );
public:
	void Rotate( float xd, float yd );//输入范围[-n,n]分别是在围绕right和up轴转动rotSpeed*xd,yd角度
	void Move( const vector3f& dis );//移动一定距离
};




class EditCamera : public ICamera // 专门为编辑器设计的相机
{
private:
	bool		m_isReverse;
	vector3f	m_Target;
	vector3f	m_VerticalAixs;//竖直移动轴
public:
	EditCamera( );
private:
	virtual void _OnlookAt( );//档外部设置lookat的时候调用_LookAt不会调用
public:
	virtual void OnActionInput( ActionManage& am );
public:
	void Rotate( float xd, float yd );//输入范围[-n,n]分别是在围绕right和up轴转动rotSpeed*xd,yd角度
	void Move( const vector3f& dis );//移动一定距离
	void ZoomInOut( float d );
};


//新的摄像机如果带有x轴上的旋转需要从新做watergrid