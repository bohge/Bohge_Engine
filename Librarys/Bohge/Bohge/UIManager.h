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
//		UI管理器	   //
/////////////////////////
#pragma once
#include "aabrect.hpp"
#include "3DMath.h"
#include "Event.hpp"
#include <string>
#include <vector>
#include <list>


namespace BohgeEngine
{
	typedef std::pair<vector2f, vector2f> CoordinatePair;
	typedef std::vector< CoordinatePair > CoordinateVector;

	class UIBase;
	class UIData;
	class Engine;
	class Texture2D;
	class SharedBuffer;
	class RendBuffer;
	class UIGroup;
	class UIAnimation;
	class VertexBuffer;
	class IndicesBuffer;
	class UIManager
	{
		friend class Engine;
		friend class ActionManage;
		friend class Pipeline;
		friend class UIGroup;
	public:
		enum Type
		{
			UI_STATIC,
			UI_BUTTON,
			UI_SLIDER,
			UI_LIST,
			UI_EDIT,
			Type_Count,
		};
		enum Layer //绘制的层
		{
			UL_TOP,
			UL_BOTTOM,
			Layer_Count,
		};
	private:
		typedef std::list<UIGroup*>	GroupList;
	private:
		bool                            m_isKeyboard;
		GroupList						m_GroupList;
		Event<bool,const std::wstring&>	m_eEditCallBakc;
	public:
		UIManager();
		~UIManager();
		std::string GetTypeName(int type);
		UIManager::Type GetType(int index);
		std::string GetLayerName(int type);
		UIManager::Layer GetLayer(int index);
	private:
		void Update(uint millisecond);//updata每个UI，刷新什么的
		void Render(Engine& engine, Layer layer);
		bool TouchResponse(bool isDown, const vector2f& point);//UI响应
		void TopMost(UIGroup* ui);//放在最上public://外部公共接口
	public:
		UIBase* Factory(UIGroup* group, const UIData& data);
		void PauseResponseAll();
		void ResumeResponseAll();
		UIGroup* CreateUIGroup( const char* file );//保存的在UIEdit中，我认为这种结构也不太好，应该放到一起，暂时不做了
		void DeleteUIGroup( UIGroup* ui );//释放
	public:
		void OnActiveKeyborad(UIBase* needer, const vector4f& rect, uint maxsize, const std::wstring& was, bool isNumberOnly);//被UI调用，需要键盘输入了
		void OnReceiveText(const std::wstring& text);
	};





	class IUIFather //可以做为父节点，响应Rennew的类,是否在绘制isShow
	{
	public:
		virtual void Renew() = 0;
		//virtual bool isShow() = 0;
	};

	bool CheckName(const UIBase& ui, const std::string& name);//解决android移植问题
	class UIGroup : public IUIFather//UI体，manager每次load就产生一个UI群体
	{
		friend class UIManager;
	private:
		struct RendStruct
		{
			SharedBuffer*	m_pShared;
			RendBuffer*		m_pRender;
			VertexBuffer*	m_pVertex;
			IndicesBuffer*	m_pIndices;
			explicit RendStruct()
				:m_pShared(NULL),
				m_pRender(NULL),
				m_pVertex(NULL),
				m_pIndices(NULL)
			{}
			~RendStruct();
		};
	private:
		typedef std::vector<UIBase*> UIVector;
		typedef std::vector< RendStruct* > RendStructVector;
	private:
#ifdef _DEBUG
		std::string				m_Name;
#endif // _DEBUG
		bool						m_isManaged;//是否被托管绘制
		bool						m_isShow;//是否显示这个UIGroup,不显示就不更新动画
		bool						m_isResponse;//是否暂停该Group的响应
		uint						m_PassedFrame;//动画走过的帧数
		bool						m_isAnimation;//是否有动画
		UIAnimation*				m_pAnimation;
		bool						m_isSetAnimation;//是否被设置的新动画
		IFunction<bool,UIGroup&>*	m_pFuncPtrTemp;//临时的指针，防止在动画结束的回调函数中，制造一个新的Animation导致的正在使用的指针被改变等错误
		Event<bool,UIGroup&>		m_Event;//动画播放完毕的响应函数
		aabrectf*					m_pBox;//整个UI的碰撞检测
		RendStructVector			m_pUIRendStructVector;//绘制buffer的Vector
		UIVector					m_UIVectorArray[UIManager::Layer_Count];//UI链表层表
		Texture2D*					m_pTexture;//纹理
		vector2f					m_Center;//UI中心，默认为屏幕中心
		Color						m_Color;//附加颜色
		vector2f					m_vScale;//缩放
		bool						m_isReflush;
	private:
		//UI个数的计数，纹理指针
		UIGroup(const std::string& filename);
		~UIGroup();
	private:
		void Update(uint millisecond);//updata每个UI，刷新什么的
		void AddUI(UIBase* ui);
		void Render(Engine& engine, UIManager::Layer layer);
		bool TouchResponse(bool isDown, const vector2f& point);//是否有任意可响应UI被按下
		void MakeBuffer();//构造一个Buffer
#ifdef _DEBUG
		BOHGE_FORCEINLINE void SetName(const std::string& name)
		{
			m_Name = name;
		}
#endif // _DEBUG
	public://以下为外部可以访问的公用接口
		void SetCenter(const vector2f& center);//设置UIGroup的中心位置，与缩放有关，UIGroup的SetCenter是将UI群整体便宜
		void SetScale(const vector2f& scale);//设置缩放
		void SetShow(bool b);//当设置show为false的时候如果有动画，则停止更新动画
		void setResponse(bool response);//停止按键响应，停止显示文字
		void SetUIAnimation(const UIAnimation& animation, IFunction<bool,UIGroup&>* const func );
		void ResetAnimation();
		void TopMost();//置顶
	public:
		virtual void Renew();
		BOHGE_FORCEINLINE void SetCorlor(const Color& clr)
		{
			m_Color = clr;
		}
		BOHGE_FORCEINLINE bool isResponse()
		{
			return m_isResponse;
		}
		BOHGE_FORCEINLINE bool isAnimation()
		{
			return m_isAnimation;
		}
		BOHGE_FORCEINLINE bool isShow()
		{
			return m_isShow;
		}
		BOHGE_FORCEINLINE const vector2f& GetPostion()
		{
			return m_Center;
		}
		BOHGE_FORCEINLINE UIAnimation& GetUIAnimation()
		{
			return *m_pAnimation;
		}
		BOHGE_FORCEINLINE void PauseAnimation()
		{
			m_isAnimation = false;
		}
		BOHGE_FORCEINLINE bool isManaged()
		{
			return m_isManaged;
		}
		BOHGE_FORCEINLINE void SetManaged(bool b)
		{
			m_isManaged = b;
		}
		BOHGE_FORCEINLINE void Draw(Engine& engine)
		{
			this->Render(engine, UIManager::UL_TOP);
		}
	public:
		template <typename T> T* Control(const char* name)
		{
			for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
			{
				for( UIVector::iterator it = m_UIVectorArray[i].begin() ;
					it != m_UIVectorArray[i].end() ;
					it++ )
				{
					if ( CheckName(**it, name) )
					{
						ASSERT( NULL != dynamic_cast<T*>(*it) );//如果要求转换的指针不是目标指针，此处会报错
						return static_cast<T*>(*it);
					}
				}
			}
			ASSERT( false );
			return NULL;
		}
		template <typename T> T* Control(UIManager::Layer layer, int index)
		{
			ASSERT( NULL != dynamic_cast<T*>( m_UIVectorArray[static_cast<int>(layer)][index] )  );
			return static_cast<T*>(m_UIVectorArray[static_cast<int>(layer)][index]);
		}
	};


	class Serializer;
	class UIData	//储存构造一个UI所需要的数据
	{
	public:
		std::string						m_Name;
		UIManager::Type				m_Type;
		UIManager::Layer			m_Layer;
		vector2f					m_Center;
		CoordinatePair				m_Tex1;
		CoordinatePair				m_Tex2;
		vector2f					m_Size;//这个size是00-11的范围的
		//float						m_Reservation1;//预留的数据
		//float						m_Reservation2;
		vector2f					m_Tex2Size;//第二个纹理的大小是00-11的范围的
		float						m_Reservation3;
		float						m_Reservation4;
		float						m_Reservation5;
		float						m_Reservation6;
		float						m_Reservation7;
		float						m_Reservation8;
		float						m_Reservation9;
		float						m_Reservation10;
		float						m_Reservation11;
		float						m_Reservation12;
		float						m_Reservation13;
		float						m_Reservation14;
		float						m_Reservation15;
		float						m_Reservation16;
	public:
		UIData();
		void Save(Serializer& file);
		void Load(Serializer& file);
	};
}