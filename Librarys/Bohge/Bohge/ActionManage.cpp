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



//////////////////////////////////////
//		游戏中动作管理的类			//
//////////////////////////////////////

#include "ActionManage.h"
#include "Engine.h"
#include "aabbox.hpp"
#include "UIManager.h"
#include "Camera.h"
#include "ShaderManage.h"



#define TOUCH_LOGIC_TIME 600
namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------
	ActionManage::ActionManage()
		:m_isTouch(false),
		m_isTouchObject(false),
		m_nActionState(0),
		m_pTouched(NULL),
		m_Switch(false),
		m_isResponse(true),
		m_is2Point(false),
		m_fDisTwoPoint(0),
		m_fLastDisTwoPoint(0),
		m_isMove(false),
		m_CounterLastInput(TOUCH_LOGIC_TIME+1),
		m_CounterLongTouch(0),
		m_isLongTouchCasted(false)
	{
		//需要从新设计这个类，解耦合
		m_InputCount[0] = 0;
		m_InputCount[1] = 0;
		m_sCurrentAction[0].resize( C_INPUT_ALLOC );
		m_sCurrentAction[1].resize( C_INPUT_ALLOC );
	}
	//--------------------------------------------------------------------------------------
	ActionManage::~ActionManage()
	{
		m_OnTouchEvent.ClearConnect();
		m_OnMoveEvent.ClearConnect();
		m_OnReleaseEvent.ClearConnect();
	}
	//--------------------------------------------------------------------------------------
	void ActionManage::_CheckFreeObject(ITouchObject* ptr)
	{
		if( ptr == m_pTouched )
		{
			m_pTouched = NULL;
			m_isTouchObject = false;
		}
	}
	//--------------------------------------------------------------------------------------
	void ActionManage::Update( uint milisecon )
	{
		m_CounterLastInput = m_CounterLastInput < TOUCH_LOGIC_TIME ? m_CounterLastInput + milisecon: m_CounterLastInput;
		m_CounterLongTouch = m_isTouch ? m_CounterLongTouch < TOUCH_LOGIC_TIME ? m_CounterLongTouch + milisecon: m_CounterLongTouch : 0;

		if( 0 != m_InputCount[static_cast<int>(m_Switch)] ) //如果当前队列中有动作，则交替
		{
			bool was = m_Switch;
			m_Switch = !m_Switch;//向第二个队列输入指令
			for ( int i = 0 ; i < m_InputCount[static_cast<int>(was)]; i++ )
			{
				switch ( m_sCurrentAction[static_cast<int>(was)][i].GetType() )
				{
				case INPUT_DOWN:	this->_TouchPoint(m_sCurrentAction[static_cast<int>(was)][i]);break;
				case INPUT_MOVE:	this->_TouchMove(m_sCurrentAction[static_cast<int>(was)][i]);break;
				case INPUT_UP:		this->_ReleasePoint(m_sCurrentAction[static_cast<int>(was)][i]);break;
				}
			}
			m_InputCount[static_cast<int>(was)] = 0;
		}
		if( 0 != m_nActionState ) //处理按键输入
		{
			Engine::Instance().GetCamera()->OnActionInput( *this );
		}
	}
	//--------------------------------------------------------------------------------------
	void ActionManage::_TouchPoint( const InputAction& input )
	{
		if( !m_isTouch )
		{
#ifdef DRAWACTIONLINE
			m_Points.clear();
#endif // DRAWACTIONLINE
			m_is2Point = false;
			m_isTouch = true;
			m_vLastPoint = m_vTouch;
			m_vTouch = input.GetPoint(0);
			m_pTouched = NULL;
			if( Engine::Instance().GetUIManager()->TouchResponse(true, input.GetPoint(0) ) )//如果按下的是任意UI
			{
				m_isTouchOnUI = true;
				return;
			}
			else
			{
				m_isLongTouchCasted = false;
				m_CounterLongTouch = 0;//初始化长点击计时
				//计算输入射线
				Engine::Instance().GetCamera()->PickRay( input.GetPoint(0), m_vTouchRay );
				m_vLastRay = m_vTouchRay;
				for( TouchMap::iterator it = m_vTouchMap.begin();
					it != m_vTouchMap.end() ;
					it ++ )
				{
					ITouchObject* temp = it->second;
					aabbox3df newbox = temp->GetBindBox() * temp->GetTransform();
					bool isInsert = newbox.Intersection( m_vTouchRay );
					if( isInsert )
					{
						m_isTouchObject = true;
						m_pTouched = it->second;
						m_pTouched->OnTouch();
						//DEBUGLOG("Touched!\n");
						break;//应为本游戏只有一个可触摸物体，所以暂时这样，实际上应该找离摄像机最近的物体
					}
				}
				m_OnTouchEvent.Multicast( *this );
			}
		}
	}
	//--------------------------------------------------------------------------------------
	void ActionManage::_TouchMove( const InputAction& input )
	{
		if( m_isTouch )
		{
			//android系统又是会输入进来错误的坐标，下面这个段是丢弃错误的坐标
			for ( int i = 0; i < input.GetCount() ; i ++)
			{
				if ( Math::Abs(input.GetPoint(i).m_x) >= 1.0
					|| Math::Abs(input.GetPoint(i).m_y) >= 1.0
					//|| Abs (m_vLastPoint.m_x - input.m_x) > 0.3
					//|| Abs (m_vLastPoint.m_y - input.m_y) > 0.3
					)
				{
#ifdef DRAWACTIONLINE
					DEBUGLOG("dispath wrong point X %f, Y %f\n", input.m_x, input.m_y)
#endif
						return;
				}
			}
			vector2f delta = m_vLastPoint - input.GetPoint(0);
#ifdef DRAWACTIONLINE
			DEBUGLOG("Touch on X %f, Y %f\n", input.m_x, input.m_y)
				m_Points.push_back( VertexColor( vector4f(input.m_x, input.m_y, 0.5, 1.0), Color(1,0,0,1)));
#endif
            //DEBUGLOG("Inputs points %f, %f and last point is %f, %f\n", input.GetPoint(0).m_x, input.GetPoint(0).m_y, m_vLastPoint.m_x, m_vLastPoint.m_y );
			//DEBUGLOG("Touch time X %d\n", m_CounterLongTouch);
			if( Math::Abs( delta.m_x ) < 0.02 && Math::Abs( delta.m_y ) < 0.02 )//长时间触摸一个地方
			{
				if ( m_CounterLongTouch > TOUCH_LOGIC_TIME && !m_isLongTouchCasted )
				{
					m_isLongTouchCasted = true;
					m_OnLongTouch.Multicast( *this );
				}
			}
			else
			{
				m_CounterLastInput = 0;
				m_CounterLongTouch = 0;
				if( m_isTouchOnUI ) // 如果只输入了一个点
				{
					if( Engine::Instance().GetUIManager()->TouchResponse(true, input.GetPoint(0)) )//如果按下的是任意UI
					{
						return;
					}
				}
				else
				{
					//计算射线
					bool isUpdataCamera = true;
					m_vLastRay = m_vTouchRay;
					m_vLastPoint = m_vTouch;
					m_vTouch = input.GetPoint(0);
					Engine::Instance().GetCamera()->PickRay( input.GetPoint(0), m_vTouchRay );
					if( input.GetCount() == 1 && !m_is2Point ) // 如果只输入了一个点，就更新游戏
					{
						m_OnMoveEvent.Multicast( *this );
					}
					else if( input.GetCount() == 2 )
					{
						if( !m_is2Point ) //第一次触发两点的信息
						{
							m_fDisTwoPoint = (input.GetPoint(0) - input.GetPoint(1)).Length();
							m_fLastDisTwoPoint = m_fDisTwoPoint;

							Engine::Instance().GetUIManager()->TouchResponse(true, vector2f( 100, 100 ) );//反转全部ui
							if( NULL != m_pTouched )
							{
								m_pTouched->OnRelease();
							}
							m_is2Point = true;
							m_pTouched = NULL;
							m_isTouchObject = false;
						}
						m_fLastDisTwoPoint = m_fDisTwoPoint;
						m_fDisTwoPoint = (input.GetPoint(0) - input.GetPoint(1)).Length();
					}
					else
					{
						isUpdataCamera = false;
						m_is2Point = false;
					}
					if( !m_isTouchObject && isUpdataCamera )//如果没有触摸到任何物体才进行摄像机更新
					{
						m_isMove = true;
						Engine::Instance().GetCamera()->OnActionInput( *this );
						m_isMove = false;
					}
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------
	void ActionManage::_ReleasePoint( const InputAction& input )
	{
		if( m_isTouch )
		{
			if( m_isTouchOnUI )
			{
				Engine::Instance().GetUIManager()->TouchResponse(false, input.GetPoint(0));
			}
			else
			{
				m_vLastRay = m_vTouchRay;
				m_vLastPoint = m_vTouch;
				m_vTouch = input.GetPoint(0);
				Engine::Instance().GetCamera()->PickRay( input.GetPoint(0), m_vTouchRay );
				m_OnReleaseEvent.Multicast( *this );
				//engine::Instance().GetCamera()->PickRay( input, m_vLastRay );//抬起的时候记录最后的移动点
				if( NULL != m_pTouched )
				{
					m_pTouched->OnRelease();
				}
				DEBUGLOG( "Double click time is %d\n", m_CounterLastInput );
				if ( m_CounterLastInput < TOUCH_LOGIC_TIME )
				{
					m_OnDoubleTouchEvent.Multicast( *this );
				}
				m_CounterLastInput = 0;
			}
			m_pTouched = NULL;
			m_isTouch = false;
			m_isTouchObject = false;
			m_is2Point = false;
			m_isTouchOnUI = false;//不需要对UI的判断了
		}
	}
	//--------------------------------------------------------------------------------------
	void ActionManage::EraseTouchObject(ITouchObject* touchObj)
	{
		TouchMap::iterator it = m_vTouchMap.find( (int)touchObj );
		if( it != m_vTouchMap.end() )
		{
			m_vTouchMap.erase( it );
			this->_CheckFreeObject(touchObj);
		}
	}
	//--------------------------------------------------------------------------------------
#ifdef DRAWACTIONLINE
	void ActionManage::Render(Engine& engine)
	{
		if( 1 < m_Points.size() )
		{
			m_pBuffer = NEW RendBuffer( &m_Points[0], m_Points.size(), sizeof(VertexColor), VertexColor::Layout(), Device::RM_LINE_STRIP);
			ShapesShader& ss = engine.GetShaderManage()->GetShader<ShapesShader>(ShaderManage::ShapesShader);
			ss.SetParamTransform( NULL );
			engine.GetDevice()->Draw( *m_pBuffer, ss, NULL );
			SAFE_DELETE(m_pBuffer);
		}
	}
#endif
	//--------------------------------------------------------------------------------------
	void ActionManage::_ResetCounterTime()
	{
		m_CounterLastInput = TOUCH_LOGIC_TIME + 1;
	}


	//--------------------------------------------------------------------------------------
	ITouchObject::~ITouchObject()
	{
		Engine::Instance().GetActionManage()->EraseTouchObject(this);
	}
	//--------------------------------------------------------------------------------------
}