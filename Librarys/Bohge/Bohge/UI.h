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



/////////////////////////
//		UI基础类	   //
/////////////////////////

#pragma once

#include "3DMath.h"
#include "aabrect.hpp"
#include "Device.h"
#include "ISceneNode.h"
#include "Bfont.h"
#include "Event.hpp"
#include "UIManager.h"
#include <vector>
#include <list>
#include <string>



namespace BohgeEngine
{
	class ScriptBase;
	class UIGroup;
	class VertexBuffer;
	class IndicesBuffer;
	class UIBase : public IGetBuffer
	{
	public:
		const static int Invalid;
	protected:
		enum Const
		{
			COUNT_OF_VERTEX		= 4,
			COUNT_OF_INDIES		= 6,
		};
	protected:
		typedef std::list< std::pair< ScriptBase*, std::string> >	LuaCallBackList;
	private:
		int					m_Identification;
		bool				m_isShow;//是否显示
		bool				m_isResponse;//对按键有响应的UI
		bool				m_isTouched;//当前被按下
		bool				m_isDonw;//当按下后为true，降低OnMouseDown的调用此处，减少开销
		std::string			m_Name;//UI名称
		UIManager::Type		m_eType;//UI类型
		UIManager::Layer	m_eLayer;//层
	protected:
		bool				m_isUpdata;//是否需要更
		bool				m_isActive;//是否活动
		IUIFather*			m_Father;	//拥有它的Father可以响应更新指令
		vector2f			m_Center;//中心节点
		vector2f			m_Size;//大小
		BString*			m_pString;//文本
		Color				m_StringColor;
		VertexTextureColor*	m_pVertexArray;
		aabrectf*			m_pBox;//碰撞检测
		vector2f			m_vScale;
	protected:
		//begin----uve
		//|			|
		//|			|
		//|			|
		//uvb------end
		UIBase(IUIFather* father, bool isResponse, const UIData& data);
	public:
		virtual ~UIBase();
	public:
		void MakeBindBox();//UITool调用，生成绑定盒子
		void SetCenter(const vector2f& center);//这只UI在Group的中心，他和UIGroup的SetCenter不同，这个是设置UI自己的中心，而UIGroup的将UI整体偏移
		void SetCenterForce(const vector2f& center);//导致一个立即刷新的设置
		void SetScale(const vector2f& scale);
		void Move(const vector2f& dis);//移动单个UI一段距离
		bool isSelected(bool isDown, const vector2f& point);//是否被选中, isReflush表示是否需要刷星buffer, isDown表示是按下还是抬起鼠标
		bool isUI(const std::string& name) const;//是否是这个UI
		void SetString(const vector2f& fontsize, const Color& clr, bool isCover, const std::wstring& wstr);//isCover定义这个字体是否是由UI自己绘制，如果这个UI可能会被其他UI覆盖，这里则选择true
		void SetString(const vector2f& fontsize, const Color& clr, bool isCover, const char* file );//为Lua定义的
		void SetShow(bool show);
		void Reset();//重置，按下等信息重置
		void SetStringShow(bool isShow);
	public://UIGroup调用
		virtual void Update();//UIGroup调用，刷新内部变量
		virtual void OnMouseDown(const vector2f& pos){}//按下时改变外型
		virtual void OnMouseMove(const vector2f& pos){}//按下后在上面移动,传递的是鼠标位置[-1,1]
		virtual void OnMouseUp(const vector2f& pos){}//起来是改变外形
		virtual void OnAction(const vector2f& pos){}//动作
		virtual void OnDraw(Engine& engine, const Color& clr, const Texture* tex);
	public:
		BOHGE_FORCEINLINE bool isUpdata()
		{
			return m_isUpdata;
		}
	public://继承的接口
		virtual void SetActive(bool isActive);//是否活动
		virtual void GetVertexBuffer(SharedBuffer& input) const ;
		virtual uint GetCountOfNode();//返回当前的node个数
	public:
		BOHGE_FORCEINLINE const int& GetIdentification() const//读取识别符
		{
			return m_Identification;
		}
		BOHGE_FORCEINLINE void SetIdentification( const int& id )//设置识别符
		{
			m_Identification = id;
		}
		BOHGE_FORCEINLINE UIManager::Type UIType()
		{
			return m_eType;
		}
		BOHGE_FORCEINLINE bool isShow()
		{
			return m_isShow;
		}
		BOHGE_FORCEINLINE bool isResponse()
		{
			return m_isResponse;
		}
		BOHGE_FORCEINLINE bool isPushDonw()
		{
			return m_isDonw;
		}
		BOHGE_FORCEINLINE bool isActive()
		{
			return m_isActive;
		}
		BOHGE_FORCEINLINE UIManager::Layer GetLayer()
		{
			return m_eLayer;
		}
		BOHGE_FORCEINLINE aabrectf GetBindBox()
		{
			if ( NULL != m_pBox )
			{
				return m_vScale.m_x >0.99f && m_vScale.m_y >0.99f ? *m_pBox : *m_pBox * m_vScale;
			} 
			else
			{
				return aabrectf( );
			}
		}
		BOHGE_FORCEINLINE const vector2f& GetSize()
		{
			return m_Size;
		}
		BOHGE_FORCEINLINE const vector2f& GetCenter()
		{
			return m_Center;
		}
		BOHGE_FORCEINLINE void SetStringPostion( const vector2f& input)
		{
			if( NULL != m_pString )
			{
				m_pString->SetPosition( input, BString::Center );
			}
		}
		BOHGE_FORCEINLINE void SetStringPostion( const vector2f& input, BString::Alignment ali)
		{
			if( NULL != m_pString )
			{
				m_pString->SetPosition( input, ali );
			}
		}
		BOHGE_FORCEINLINE const BString& GetString()
		{
			return *m_pString;
		}
	public:
		static uint GetElementCountPerVertex();//得到每个顶点float元素个数
		static BOHGE_FORCEINLINE uint GetCountOfVertex()
		{
			return COUNT_OF_VERTEX;
		}
		static BOHGE_FORCEINLINE uint GetCountOfIndes()
		{
			return COUNT_OF_INDIES;
		}
		static BOHGE_FORCEINLINE Device::RenderMode GetRenderMode()
		{
			return Device::RM_TRIANGLES;
		}
	};



	class UIStatic : public UIBase
	{
	public:
		UIStatic(IUIFather* father, const UIData& data);
		~UIStatic();
	};


	class UIButton : public UIBase
	{
	private:
		CoordinatePair			m_CoordNormal;
		CoordinatePair			m_CoordHighLight;
		Event<bool,UIBase&>		m_Event;//响应函数
		LuaCallBackList			m_LuaCallBack;//lua的callback函数
	public:
		UIButton(IUIFather* father, const UIData& data);
		~UIButton();
	protected:
		virtual void OnMouseDown(const vector2f& pos);
		virtual void OnMouseUp(const vector2f& pos);
		virtual void OnAction(const vector2f& pos);
	public:
		virtual void SetActive(bool isActive);
	public:
		void AddResponse(IFunction<bool,UIBase&>* const func);
		void AddResponse( ScriptBase* scrip, const char* func );//Lua使用的callback,c++禁止使用
	};


	class UISlider : public UIBase
	{
	private:
		float					m_fPercentage;//滑动条百分比
		float					m_fMinPercentage;
		uint					m_SliderCount;
		uint					m_VisuableSliderCount;
		CoordinatePair			m_CoordElement;
		vector2f				m_ElementSize;
		VertexTextureColor*		m_pSliderVertexArray;
		Event<bool,UIBase&>		m_Event;//响应函数
		LuaCallBackList			m_LuaCallBack;//lua的callback函数
	public:
		UISlider(IUIFather* father, const UIData& data);
		~UISlider();
	protected:
		virtual void OnMouseMove(const vector2f& pos);
		virtual void OnAction(const vector2f& pos);
		virtual void Update();//UIGroup调用，刷新内部变量
	public://继承的接口
		virtual void SetActive(bool isActive);
		virtual void GetVertexBuffer(SharedBuffer& input) const ;
		virtual uint GetCountOfNode();//返回当前的node个数
	public:
		void AddResponse(IFunction<bool,UIBase&>* const func);
		void AddResponse( ScriptBase* scrip, const char* func );//Lua使用的callback,c++禁止使用
		void SetPercentage(clamp percentage);
	public:
		BOHGE_FORCEINLINE float Percentage () //得到百分比
		{
			return m_fPercentage;
		}
		BOHGE_FORCEINLINE void SetMinPercentage( clamp percentage )//最小数值
		{
			m_fMinPercentage = Math::Clamp0to1( percentage );
		}
	};


	//list 专用的
	class IListItem
	{
		friend class UIList;
	public:
		virtual ~IListItem(){}
	private:
		virtual void OnVisable() = 0;//进入显示范围
		virtual void OnInvisable() = 0;//移出显示范围
		virtual void SetPostion(const vector2f& ItemCenter) = 0;
		virtual void Draw(Engine& engine) = 0;
	};
	class UIList : public UIBase , public IUIFather//这个也是一个UIFather管理内部的static刷新
	{
	private:
		typedef std::list< IListItem* > ItemList;//包含数据链表
		typedef std::vector< std::pair<UIStatic*,ItemList::iterator> >	ContainerVector;
	private:
		uint					m_ContainerCount;//需要的Item容器，这个类并不是针对每一个Item产生一个可绘制的Item，而是找一个需要的最小数，然后循环利用
		vector2f				m_ListItemSize;//list中需要的Item的尺寸
		Event<bool,IListItem&>	m_Event;//响应函数
		LuaCallBackList			m_LuaCallBack;//lua的callback函数

		ItemList				m_ItemList;//数据数组
		ContainerVector			m_ContainerVector;//容器包，可绘制的
		int						m_CurrentIndex;//当前的第一个容器索引
		SharedBuffer*			m_pBuffer;
		RendBuffer*				m_pRender;
		VertexBuffer*			m_pVertexBuffer;
		IndicesBuffer*			m_pIndicesBuffer;
		RendBuffer*				m_pStencilRender;
		VertexBuffer*			m_pStencilVertexBuffer;
		IndicesBuffer*			m_pStencilIndicesBuffer;

		bool					m_isUp;//鼠标移动的方向是否向上
		float					m_LastMouseY;//鼠标最后的Y值
		bool					m_isAction;//时候是点击事件
		float					m_DownMouseY;//按下式的鼠标位置
	public:
		UIList(IUIFather* father, const UIData& data);
		~UIList();
	private:
		void FlushItem();
	protected:
		virtual void OnMouseDown(const vector2f& pos);
		virtual void OnMouseMove(const vector2f& pos);
		virtual void OnAction(const vector2f& pos);//动作
		virtual void OnDraw(Engine& engine, const Color& clr, const Texture* tex);
		virtual void Update();
	public:
		virtual void SetActive(bool isActive);
		virtual void Renew();
	public:
		void AddResponse(IFunction<bool,IListItem&>* const func);
		void AddResponse( ScriptBase* scrip, const char* func );//Lua使用的callback,c++禁止使用
		void AddItem(IListItem* item);
		void ShowLastItem();//直接显示最后一个Item
		void Clear();
	};


	class UIEdit : public UIBase //输入板
	{
	private:
		bool			m_isNumOnly;
		uint			m_MaxInput;	//最大输入的个数
		vector2f		m_FontSize;
		Color			m_FontClr;
		std::wstring	m_Content;//输入的内容
		vector2f		m_FontOffset;//文本位置的偏移
		RendBuffer*		m_pStencilRender;
		VertexBuffer*	m_pStencilVertexBuffer;
		IndicesBuffer*	m_pStencilIndicesBuffer;
		Event<bool,const std::wstring&>	m_OnReciveText;
	public:
		UIEdit(IUIFather* father, const UIData& data);
		~UIEdit();
	protected:
		virtual void OnAction(const vector2f& pos);//被点击后调用系统键盘，控制输入
		virtual void OnDraw(Engine& engine, const Color& clr, const Texture* tex);
	public:
		void AddResponse(IFunction<bool,const std::wstring&>* const func);
		bool OnReciveText(const std::wstring& str);
	public:
		BOHGE_FORCEINLINE std::wstring& GetContent()
		{
			return m_Content;
		}
		BOHGE_FORCEINLINE void Clear()
		{
			m_Content.clear();
			if( NULL != m_pString )
			{
				Engine::Instance().GetStringManager()->DestroyString( m_pString );
				m_pString = NULL;
			}
		}
		BOHGE_FORCEINLINE void SetFontSize( const vector2f& size )
		{
			m_FontSize = size;
		}
		BOHGE_FORCEINLINE void SetFontColor( const Color& clr )
		{
			m_FontClr = clr;
		}
		BOHGE_FORCEINLINE void SetMaxInput( uint m )//最大的可输入的字符数量,0是没有限制
		{
			m_MaxInput = m;
		}
		BOHGE_FORCEINLINE void NumOnly( bool b )
		{
			m_isNumOnly = b;
		}
		BOHGE_FORCEINLINE void FontOffset( const vector2f& off )
		{
			m_FontOffset = off;
		}
	};
}