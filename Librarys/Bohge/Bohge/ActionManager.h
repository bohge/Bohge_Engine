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

#pragma once
#include <vector>
#include <map>
#include "3DMath.h"
#include "Ray.h"
#include "Log.h"
#include "Event.hpp"
//#define DRAWACTIONLINE


#ifdef DRAWACTIONLINE
#include "Vertex.h"
#endif // DRAWACTIONLINE

namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------
	class ITouchObject;

	class ActionManager
	{
	private:
		typedef std::map<int,ITouchObject*>		TouchMap;
#ifdef DRAWACTIONLINE
		typedef std::vector<VertexColor>		LineVector;
#endif // _DEBUG
	private:
		enum Constant
		{
			C_INPUT_ALLOC = 10,//出事分配的可输入的大小
		};
	public:
		enum Key_Down //案件动作,基本上是调试效果的时候使用
		{
			KD_KEY1 = 0,
			KD_KEY2,
			KD_KEY3,
			KD_KEY4,
			KD_KEY5,
			KD_KEY6,
			KD_KEY7,
			KD_KEY8,
			KD_KEY9,
			KD_KEY0,
			KD_KEYQ,
			KD_KEYE,
		};
		enum Input_Type//输入的动作
		{
			INPUT_NONE,//初始化用的
			INPUT_DOWN,
			INPUT_MOVE,
			INPUT_UP,
		};
		enum Action
		{
			ACTION_SWITH	= 1 << 0,
			ACTION_UP		= 1 << 1,
			ACTION_DOWN		= 1 << 2,
			ACTION_LEFT		= 1 << 3,
			ACTION_RIGHT	= 1 << 4,
			ACTION_FALL		= 1 << 5,
			ACTION_PAUSE	= 1 << 6,
			ACTION_EXIT		= 1 << 7,
			//摄像机操作
			ACTION_MOVELEFT		= 1 << 8,
			ACTION_MOVELRIGHT	= 1 << 9,
			ACTION_MOVEFORWARD	= 1 << 10,
			ACTION_MOVEBACK		= 1 << 11,
		};
	public:
		class InputAction
		{
		private:
			enum
			{
				IA_MAX_INPUT = 2
			};
		private:
			typedef std::vector<vector2f>		PointVector;
		private:
			Input_Type		m_eType;
			int				m_Count;
			PointVector		m_Points;
		public:
			InputAction()
				:m_eType(INPUT_NONE),
				m_Count(0)
			{
				m_Points.resize( IA_MAX_INPUT );
			}
			BOHGE_FORCEINLINE void Begin()
			{
				m_Count = 0;
			}
			BOHGE_FORCEINLINE void SetType( Input_Type t )
			{
				m_eType = t;
			}
			BOHGE_FORCEINLINE bool SetPoint( const vector2f& point )
			{
				if( m_Count < IA_MAX_INPUT )
				{
					m_Points[m_Count] = point;
					m_Count ++;
					return true;
				}
				return false;
			}
			BOHGE_FORCEINLINE Input_Type GetType() const
			{
				return m_eType;
			}
			BOHGE_FORCEINLINE int GetCount() const
			{
				return m_Count;
			}
			BOHGE_FORCEINLINE const vector2f& GetPoint( int index ) const
			{
				ASSERT( index < IA_MAX_INPUT );
				return m_Points[index];
			}
		};
	private:
		typedef std::vector<InputAction>	InputVector;
	private:
		uint			m_CounterLastInput;//判断是否双击
		uint			m_CounterLongTouch;//触摸这个点持续多久了
		bool			m_isLongTouchCasted;
		bool			m_isTouchOnUI;//是否第一次点在UI上
		bool			m_isTouch;//当触摸时
		bool			m_isTouchObject;//是否触摸了任意一个物体
		bool			m_is2Point;//是否输入了2个点
		bool			m_isMove;//是否移动了鼠标
		int				m_nActionState;
		float			m_fDisTwoPoint;//两点的点距0-1
		float			m_fLastDisTwoPoint;//前一个点距0-1
		vector2f		m_vTouch;//触摸的点,屏幕空间(-1,1)区间
		vector2f		m_vLastPoint;//前一个点,屏幕空间(-1,1)区间
		Rayf			m_vTouchRay;//触摸的射线
		Rayf			m_vLastRay;//上一个触摸的射线
		TouchMap		m_vTouchMap;//触摸列表
		ITouchObject*	m_pTouched;//被点击的物体
		Event<bool, ActionManager&>	m_OnTouchEvent;
		Event<bool, ActionManager&>	m_OnMoveEvent;
		Event<bool, ActionManager&>	m_OnReleaseEvent;
		Event<bool, ActionManager&>	m_OnLongTouch;//长时间触摸
		Event<bool, ActionManager&>	m_OnDoubleTouchEvent;
		//调试的按键响应
		Event<bool, Key_Down>		m_OnKeyDown;

		//输入动作的链表
		bool			m_isResponse;//是否有输入响应
		bool			m_Switch;//两个链表交替使用
		int				m_InputCount[2];//输入数量的计数
		InputVector		m_sCurrentAction[2];//当前的动作
#ifdef DRAWACTIONLINE
		RendBuffer*		m_pBuffer;
		LineVector		m_Points;
	public:
		void Render(Engine& engine);
#endif // DRAWACTIONLINE
	public:
		ActionManager();
		~ActionManager();
	private:
		void _CheckFreeObject(ITouchObject* ptr);//如果当前触摸的物体被删除，则需要停止触摸响应
		/*输入为已经转换为opengle坐标系的相对点
		例如vector2f(-1.0,1.0)*/
		void _TouchPoint( const InputAction& input );
		/*按住移动的时候输入为已经转换为opengle坐标系的相对点
		例如vector2f(-1.0,1.0)*/
		void _TouchMove( const InputAction& input );
		/*释放点击的时候*/
		void _ReleasePoint( const InputAction& input );
		void _ResetCounterTime();
	public:
		void Update( uint milisecon ); //engine 调用的，输入响应
		/*释放可触摸的物体*/
		void EraseTouchObject(ITouchObject* touchObj);
	public:
		BOHGE_FORCEINLINE void OnKeyDownEvent( IFunction<bool, Key_Down>* const func )
		{
			m_OnKeyDown.Connect( func );
		}
		BOHGE_FORCEINLINE void KeyDown( Key_Down key )
		{
			m_OnKeyDown.Multicast( key );
		}
		BOHGE_FORCEINLINE void SetResponse(bool isResponse)
		{
			m_isResponse = isResponse;

		}
		BOHGE_FORCEINLINE bool isAction(Action action)//此Action只有通过按键触发的action，touchObject的响应全部由ITouchObject->Response处理
		{
			return ( ( m_nActionState & action ) != 0 );
		}
		BOHGE_FORCEINLINE void TouchAction( const InputAction& input )
		{
			if( m_isResponse )
			{
				//if( m_isTouch || INPUT_MOVE != type)
				//{
				//	DEBUGLOG("save in buffer\n");
				//}
				if( m_InputCount[static_cast<int>(m_Switch)] >= m_sCurrentAction[static_cast<int>(m_Switch)].size() )
				{
					DEBUGLOG("More input buffer\n",1);
					m_sCurrentAction[static_cast<int>(m_Switch)].resize( m_sCurrentAction[static_cast<int>(m_Switch)].size() + C_INPUT_ALLOC );
				}
				m_sCurrentAction[static_cast<int>(m_Switch)][m_InputCount[static_cast<int>(m_Switch)]] = input;
				m_InputCount[static_cast<int>(m_Switch)] ++;
			}
		}
		BOHGE_FORCEINLINE void ClearEvents()//清理全部的响应事件
		{
			m_OnTouchEvent.ClearConnect();
			m_OnMoveEvent.ClearConnect();
			m_OnReleaseEvent.ClearConnect();
			m_OnDoubleTouchEvent.ClearConnect();
		}
		BOHGE_FORCEINLINE FunctionPtr OnTouchEvent( IFunction<bool, ActionManager&>* const func )
		{
			return m_OnTouchEvent.Connect( func );
		}
		BOHGE_FORCEINLINE FunctionPtr OnMoveEvent( IFunction<bool, ActionManager&>* const func )
		{
			return m_OnMoveEvent.Connect( func );
		}
		BOHGE_FORCEINLINE FunctionPtr OnReleaseEvent( IFunction<bool, ActionManager&>* const func )
		{
			return m_OnReleaseEvent.Connect( func );
		}
		BOHGE_FORCEINLINE FunctionPtr OnLongTouch( IFunction<bool, ActionManager&>* const func )
		{
			return m_OnLongTouch.Connect( func );
		}
		BOHGE_FORCEINLINE FunctionPtr OnDoubleTouchEvent( IFunction<bool, ActionManager&>* const func )
		{
			_ResetCounterTime();
			return m_OnDoubleTouchEvent.Connect( func );
		}
		BOHGE_FORCEINLINE void RemoveEvent( FunctionPtr func )
		{
			m_OnTouchEvent.Remove( func );
			m_OnMoveEvent.Remove( func );
			m_OnReleaseEvent.Remove( func );
			m_OnLongTouch.Remove( func );
			m_OnDoubleTouchEvent.Remove( func );
		}
	public:
		BOHGE_FORCEINLINE void Reset()
		{
			m_nActionState = 0;
		}
		BOHGE_FORCEINLINE void SetAction(Action action)
		{
			m_nActionState |= action;
		}
		BOHGE_FORCEINLINE bool isTouch()
		{
			return m_isTouch;
		}
		BOHGE_FORCEINLINE void AddTouchObject(ITouchObject* touchObj)
		{
			m_vTouchMap.insert( std::make_pair((int)touchObj, touchObj) );
		}
		ITouchObject* TouchObject()
		{
			return m_pTouched;
		}
		BOHGE_FORCEINLINE ITouchObject* FirstObject()
		{
			if( m_vTouchMap.empty() )
			{
				return NULL;
			}
			return m_vTouchMap.begin()->second;
		}
		BOHGE_FORCEINLINE const vector2f& TouchPoint()//屏幕空间(-1,1)区间
		{
			return m_vTouch;
		}
		BOHGE_FORCEINLINE const Rayf& TouchRay()
		{
			return m_vTouchRay;
		}
		BOHGE_FORCEINLINE const vector2f& LastTouchPoint()//屏幕空间(-1,1)区间
		{
			return m_vLastPoint;
		}
		BOHGE_FORCEINLINE const Rayf& LastTouchRay()
		{
			return m_vLastRay;
		}
		BOHGE_FORCEINLINE bool isTowPoint()//是否是按下了2个点
		{
			return m_is2Point;
		}
		BOHGE_FORCEINLINE float DistanceTowPoint()
		{
			return m_fDisTwoPoint;
		}
		BOHGE_FORCEINLINE float LastDistanceTowPoint()
		{
			return m_fLastDisTwoPoint;
		}
		BOHGE_FORCEINLINE bool isMove()
		{
			return m_isMove;
		}
	};

	template <class T> class aabbox3d;
	typedef aabbox3d<float> aabbox3df;
	class ITouchObject
	{
	public:
		virtual ~ITouchObject();	//析构的时候如果ActionManager中指向这个物体，一定要停止继续的响应
		virtual aabbox3df& GetBindBox() const = 0;
		virtual Matrix44f& GetTransform() const = 0;
		virtual void Response(ActionManager::Action action) = 0;
		virtual void OnTouch() = 0;//当备选中的时候
		virtual void OnRelease() = 0;//当选择离开的时候
	};
}