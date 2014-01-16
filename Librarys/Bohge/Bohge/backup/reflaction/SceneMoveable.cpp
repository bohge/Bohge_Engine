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
#include "SceneMoveable.h"
#include "Map.h"
#include "Perdeclare.h"

//------------------------------------------------------------------------------------------------------
SceneObjectMoveable::SceneObjectMoveable(float3 const & forward, float3 const & up )
:SceneObjectHelper(SceneObject::SOA_Cullable | SceneObject::SOA_Moveable),
m_isNeedUpdate(true),
m_Forward(forward),
m_Back(-m_Forward),
m_Up(up),
m_MoveSpeed( 0.0f ),
m_RotationSpeed( 0.0f ),
m_isBeginBack(false),
m_BackTime(0.0f),
m_isBeginBack(false)
{
	m_Forward = MathLib::normalize(m_Forward);
	m_Up = MathLib::normalize(m_Up);
	m_Postion = float3(0,0,0);
	m_Scale = float3(1,1,1);
	m_Rotation = Quaternion::Identity();
	m_isNeedUpdate = true;
}
//------------------------------------------------------------------------------------------------------
void SceneObjectMoveable::Update(float app_time, float elapsed_time)
{
	if( m_isBeginBack )
	{
		m_BackTime += elapsed_time;
	}
	SceneObjectHelper::Update(app_time,elapsed_time);//首先调用基类的Update，klayGE4_0_0_0此函数是空函数
	if ( m_isNeedUpdate )
	{
		m_isNeedUpdate = false;
		checked_pointer_cast<Renderable>(renderable_)->ModelMatrix(
			MathLib::scaling(m_Scale)
			* MathLib::to_matrix(m_Rotation)
			* MathLib::translation(m_Postion) 
			);
	}
}
//------------------------------------------------------------------------------------------------------
void SceneObjectMoveable::SetPostion(float3 const & pos)
{
	m_Postion = pos;
	m_isNeedUpdate = true;
}
//------------------------------------------------------------------------------------------------------
void SceneObjectMoveable::Move(float3 const & direction, float distance)
{
	m_Postion += direction * distance;
	m_isNeedUpdate = true;
}
//------------------------------------------------------------------------------------------------------
void SceneObjectMoveable::MoveForward(float distance)
{
	m_Postion += m_Forward * distance;//m_Forwar normalized
	m_isNeedUpdate = true;
}
//------------------------------------------------------------------------------------------------------
void SceneObjectMoveable::MoveBack(float distance)
{
	m_Postion += m_Back * distance;//m_Forwar normalized
	m_isNeedUpdate = true;
}
//------------------------------------------------------------------------------------------------------
void SceneObjectMoveable::MoveOnModelWorld(float3 const & direction, float distance)
{
	float3 NewDirection = MathLib::normalize( MathLib::transform_quat(direction, m_Rotation) );
	m_Postion += NewDirection * distance;//m_Forwar normalized
	m_isNeedUpdate = true;
}
//------------------------------------------------------------------------------------------------------
//void SceneObjectMoveable::Rotaion(float3 const & axis, float radian)
//{
//	Quaternion temp = MathLib::rotation_axis( axis, radian );
//	m_Up = MathLib::normalize( MathLib::transform_quat(m_Up, temp) );//m_Rotation是累积的m_Up也是累积的，所以此处不能成m_Rotation，下同
//	m_Forward = MathLib::normalize( MathLib::transform_quat(m_Forward, temp) );
//	m_Back = MathLib::normalize( MathLib::transform_quat(m_Back, temp) );
//	m_Rotation *= temp;
//	m_isNeedUpdate = true;
//}
//------------------------------------------------------------------------------------------------------
void SceneObjectMoveable::Rotaion( float3 const & faceto, float3 const & moveDir, float timespan)
{
	float cos_theta = MathLib::dot(faceto, moveDir);
	float angle = acos(cos_theta);
	float3 rotationDir = moveDir - faceto;	//forward向量转向的方向
	if( MathLib::abs(angle) > m_RotationSpeed * timespan )
	{
		if( MathLib::abs(angle) <= PI / 1.5 )
		{
			rotationDir = MathLib::normalize(rotationDir) * m_RotationSpeed * timespan;
		}
		else
		{
			float3 baseDir = MathLib::normalize( MathLib::cross( faceto, float3(0,1,0) ) );
			float3 rotto = MathLib::normalize( baseDir - faceto );
			if( MathLib::dot(baseDir, moveDir) < 0.0f )
			{
				rotto = -rotto;
			}
			rotationDir =  rotto * m_RotationSpeed * timespan;
		}
	}

	float3 newForward = m_Forward + rotationDir;
	Quaternion rotaion = MathLib::axis_to_axis(m_Forward, newForward);
	m_Rotation *= rotaion;
	m_Forward = MathLib::normalize( MathLib::transform_quat(m_Forward, rotaion) );
	m_Up = MathLib::normalize( MathLib::transform_quat(m_Up, rotaion) );
	//m_Forward = MathLib::normalize( newForward );
	m_Back = - m_Forward;
	m_isNeedUpdate = true;
}
//------------------------------------------------------------------------------------------------------
void SceneObjectMoveable::RotaionByUp(float radian)
{
	//旋转up和朝向向量
	Quaternion temp = MathLib::rotation_axis( m_Up, radian );
	m_Forward = MathLib::normalize( MathLib::transform_quat(m_Forward, temp) );
	m_Back = MathLib::normalize( MathLib::transform_quat(m_Back, temp) );
	m_Rotation *= temp;
	m_isNeedUpdate = true;
}
//------------------------------------------------------------------------------------------------------
void SceneObjectMoveable::Scale(float3 const & scale)
{
	m_box = SceneObjectHelper::Bound();
	m_box.Min() *= scale;
	m_box.Max() *= scale;
	m_Scale = scale;
	m_isNeedUpdate = true;
}
//------------------------------------------------------------------------------------------------------
AABBox const & SceneObjectMoveable::BoundLocal() const
{
	return m_box;
}
//------------------------------------------------------------------------------------------------------
void SceneObjectMoveable::ProcessMove( float3 const & dir, float sec)
{
	float cos_theta = MathLib::dot(m_Forward, dir);
	float angle = acos(cos_theta);
	if( angle != angle || MathLib::abs(angle) < 0.1f )//移动
	{
		float distance = m_MoveSpeed * sec;
		this->Rotaion( m_Forward, dir, sec );
		this->MoveForward( distance );
	}
	if( MathLib::abs(angle) > 0.1f )
	{
		this->Rotaion( m_Forward, dir, sec );
	}
	return true;
}
//------------------------------------------------------------------------------------------------------
void SceneObjectMoveable::ProcessAvoid(float3 const & dir, float sec)
{
	//计算MoveDir与Back和Forward的夹角，来判断向前还是向后
	float mDirBack = MathLib::dot( dir , m_Back );
	float mDirForw = MathLib::dot( dir , m_Forward );

	//迎合蝈蝈，全部向前跳出去，不搞方向了
	if ( mDirBack > mDirForw ) //dot是值越大，夹角越小
	{//向后移动
		this->MoveBack( m_MoveSpeed * sec );
		float angle = acos(mDirBack);
		if( MathLib::abs(angle) > 0.2f )
		{
			//float3 rotationDir = m_Back - m_MoveDir;	//forward向量转向的方向
			//if( MathLib::length(rotationDir) >= m_RotationSpeed * sec )
			//{
			//	rotationDir = MathLib::normalize(rotationDir) * m_RotationSpeed * sec;
			//}
			//else
			//{
			//	rotationDir = rotationDir;
			//}
			this->Rotaion( m_Back, dir, sec );
		}
	}
	else
	{
		//this->MoveForward(m_MoveSpeed * sec);
		float angle = acos(mDirForw);
		if( MathLib::abs(angle) > 0.2f )
		{
			//float3 rotationDir = m_MoveDir - m_Forward;	//forward向量转向的方向
			//if( MathLib::length(rotationDir) >= m_RotationSpeed * sec )
			//{
			//	rotationDir = MathLib::normalize(rotationDir) * m_RotationSpeed * sec;
			//}
			//else
			//{
			//	rotationDir = rotationDir;
			//}
			this->Rotaion( m_Forward, dir, sec );
		}
		else
		{
			this->Rotaion( m_Forward, dir, sec );
			this->MoveForward(m_MoveSpeed * sec);
		}
	}

}