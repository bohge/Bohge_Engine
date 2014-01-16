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



///////////////////////////////////////////
//弥补klay中的SceneObject不支持变换的问题//
///////////////////////////////////////////
#pragma once
#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Font.hpp>
#include <KlayGE/RenderableHelper.hpp>
#include <KlayGE/SceneObjectHelper.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/RenderSettings.hpp>
#include <KlayGE/Mesh.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/Window.hpp>
#include <KlayGE/PostProcess.hpp>
#include <KlayGE/Camera.hpp>
#include <KlayGE/Light.hpp>
#include <KlayGE/DeferredRenderingLayer.hpp>
#include <KlayGE/XMLDom.hpp>


using namespace KlayGE;


class SceneObjectMoveable : public SceneObjectHelper
{
protected:
	AABBox			m_box;
	bool			m_isNeedUpdate;//是否更新变换矩阵
	float3			m_Forward;//朝向
	float3			m_Back;//后退方向
	float3			m_Up;//Up轴

	Quaternion		m_Rotation;//旋转
	float3			m_Postion;//位置
	float3			m_Scale;//缩放

	float			m_MoveSpeed;//移动速度 m/s
	float			m_RotationSpeed;//旋转速度 r/s

	bool			m_isBeginBack;//是否开始后退
	float			m_BackTime;//后退的时间
public:
	explicit SceneObjectMoveable(float3 const & forward, float3 const & up );
	virtual ~SceneObjectMoveable()
	{
	}
public:
	//重载的虚函数，随着引擎更新，可能会产生问题
	virtual void Update(float app_time, float elapsed_time);
protected:
	//自动移动到指定的目标，这个函数包括了旋转当前的forward向量
	void ProcessMove( float3 const & dir, float sec);
	//处理避让，考虑是向前还是后退，是否带有旋转
	void ProcessAvoid(float3 const & dir, float sec);
public:
	//设置一个位置
	virtual void SetPostion(float3 const & pos);
	//以任意方向平移，输入的向量是世界坐标系中的向量
	virtual void Move(float3 const & direction, float distance);
	//在当前的朝向上面移动一定距离
	virtual void MoveForward(float distance);
	//在当前的朝向反方向上面移动一定距离
	virtual void MoveBack(float distance);
	//按照模型的坐标世界中的轴旋转，既此处输入的direction会在函数中乘旋转，变到模型坐标系
	virtual void MoveOnModelWorld(float3 const & direction, float distance);
	//绕任意周旋转，输入的向量是世界坐标系中的向量
	//virtual void Rotaion(float3 const & axis, float radian);
	//输入一个当前移动方向，模型会按照这个方向旋转一定角度,返回旋转的角度
	virtual void Rotaion( float3 const & faceto, float3 const & moveDir, float timespan);
	//绕Up轴旋转
	virtual void RotaionByUp(float radian);
	//缩放模型
	virtual void Scale(float3 const & scale);
	AABBox const & BoundLocal() const;
public:
	inline const float3& GetPostion()
	{
		return m_Postion;
	}
	inline const float3& GetForward()
	{
		return m_Forward;
	}
	inline void SetMoveSpeed( float ms )
	{
		m_MoveSpeed = ms;
	}
	inline void SetRotateSpeed( float rs )
	{
		m_RotationSpeed = rs;
	}
};