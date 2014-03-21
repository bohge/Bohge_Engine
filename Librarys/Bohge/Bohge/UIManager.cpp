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

#include "UI.h"
#include "UIManager.h"
#include "Predefine.h"
#include "Serializer.h"
#include "Device.h"
#include "Texture.h"
#include "Vertex.h"
#include "ShaderManager.h"
#include "aabrect.hpp"
#include "UIAnimation.h"
#include "RendBuffer.h"
#include "Shaders2D.h"
#include "FilePath.h"
#include "Utility.h"



using namespace std;
using namespace BohgeEngine;


extern void Keyborad(const vector4f& rect, uint maxsize, const std::wstring& was, bool isNumberOnly  );//需要外部实现的支持键盘的函数！
extern void HideKeyBorad();
namespace BohgeEngine
{
	///////////////////////
	//		UIGroup		 //
	///////////////////////
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UIGroup::RendStruct::~RendStruct()
	{
		SAFE_DELETE( m_pShared );
		SAFE_DELETE( m_pRender );
		SAFE_DELETE( m_pVertex );
		SAFE_DELETE( m_pIndices );
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UIGroup::UIGroup(const std::string& filename)
		:m_isAnimation(false),
		m_pAnimation(NULL),
		m_pFuncPtrTemp(NULL),
		m_isShow(true),
		m_isReflush(true),
		m_isResponse(true),
		m_Color(1,1,1,1),
		m_isSetAnimation(false),
		m_PassedFrame(0),
		m_vScale(1,1),
		m_isManaged(true)//默认是被托管绘制的
	{
		string fullpath = FILEPATH.isExist( filename ) ? filename : FILEPATH.UIFolder() + filename;
		Serializer file( fullpath );
		string texName;//读取UI纹理
		file>>texName;

		Utility::RemoveTargetString( texName, ".enc" );//临时

		int layersize[UIManager::Layer_Count];
		int size = 0;
		for (int i = 0 ; i < UIManager::Layer_Count ; i ++)
		{
			file>>layersize[i];//首先输入每层的UI包含个数
			size += layersize[i];
		}
		UIData data;
		m_pTexture = NEW Texture2D();
		m_pTexture->LoadTexture( Device::PF_R8G8B8A8, FILEPATH.TextureFolder() + texName);
		m_pBox = NEW aabrectf( );
		for (int i = 0 ; i < size ; i ++)
		{
			data.Load(file);
			this->AddUI( Engine::Instance().GetUIManager()->Factory(this, data) );
		}
		this->MakeBuffer();
		this->Renew();
#ifdef _DEBUG
		this->SetName( texName );
#endif // _DEBUG
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UIGroup::~UIGroup()
	{
		SAFE_DELETE( m_pBox );
		SAFE_DELETE( m_pAnimation );
		SAFE_DELETE( m_pTexture );
		for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
		{
			SAFE_DELETE( m_pUIRendStructVector[i] );
		}
		for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
		{
			for( UIVector::iterator it = m_UIVectorArray[i].begin() ;
				it != m_UIVectorArray[i].end() ;
				it++ )
			{
				SAFE_DELETE( *it );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::AddUI(UIBase* ui)
	{
		m_UIVectorArray[ui->GetLayer()].push_back(ui);
		if( ui->isResponse() )
		{
			m_pBox->Contains( ui->GetBindBox() );
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::MakeBuffer()
	{
		for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
		{
			uint count = 0;
			for( UIVector::iterator it =  m_UIVectorArray[i].begin();
				it != m_UIVectorArray[i].end();
				it++ )
			{
				count += (*it)->GetCountOfNode();
			}
			if( 0 != count )
			{
				RendStruct* rs = NEW RendStruct;
				rs->m_pShared = NEW SharedBuffer(
					VertexTextureColor::GetElementCount(),
					UIBase::GetCountOfVertex(),
					UIBase::GetCountOfIndes(),
					count);
				rs->m_pRender = NEW RendBuffer( UIBase::GetRenderMode() );
				rs->m_pVertex = NEW VertexBuffer( Device::MU_DYNAMIC );
				rs->m_pIndices = NEW IndicesBuffer( Device::MU_DYNAMIC );
				rs->m_pRender->SetVertexBuffer( rs->m_pVertex );
				rs->m_pRender->SetIndicesBuffer( rs->m_pIndices );

				m_pUIRendStructVector.push_back( rs );
			}
			else
			{
				m_pUIRendStructVector.push_back( NULL );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::SetUIAnimation(const UIAnimation& animation, IFunction<bool,UIGroup&>* const func )
	{
		m_isSetAnimation = true;
		if( NULL != m_pAnimation )
		{
			SAFE_DELETE( m_pAnimation );
		}
		m_pAnimation = NEW UIAnimation( animation );
		if ( NULL != func )//这里引入一个临时的指针，防止在动画完成的回掉函数中添加一个新的动画而改变函数指针导致的错误，具体的改变移动到updata中完成
		{
			m_pFuncPtrTemp = func;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::ResetAnimation()
	{
		if( NULL != m_pAnimation )
		{
			m_pAnimation->Reset();
			m_isAnimation = true;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::Update(uint millisecond)
	{
		if( m_isSetAnimation )
		{
			m_PassedFrame = 0;
			m_isSetAnimation = false;
			m_isAnimation = true;
			m_Event.ClearConnect();
			if ( NULL != m_pFuncPtrTemp )//改变动画，改变函数指针
			{
				m_Event.Connect( m_pFuncPtrTemp );
			}		
			m_pFuncPtrTemp = NULL;
		}
		if( m_isShow )
		{
			if( m_isAnimation )
			{
				m_PassedFrame ++;
				//DEBUGLOG("%d ",millisecond);
				if( m_PassedFrame < 2 )//前两帧可能是转换状态等，这样时间较长，停止更新动画
				{
					m_isAnimation = m_pAnimation->Update(16);
				}
				else
				{
					m_isAnimation = m_pAnimation->Update(millisecond);
				}
				if( m_pAnimation->isColorAnimation() || m_pAnimation->isFlashAnimation() )
				{
					m_Color = m_pAnimation->GetColor();
				}
				if( m_pAnimation->isPostionAnimation() )
				{
					this->SetCenter( m_pAnimation->GetPotstion() );
				}
				if( m_pAnimation->isScaleAnimation() )
				{
					this->SetScale( m_pAnimation->GetScale() );
				}
				if( false == m_isAnimation )
				{
					m_Event.Multicast( *this );
				}
			}
			if ( m_isReflush )
			{
				m_isReflush = false;
				//首先updata 一次，避免list的buffer嵌套使用
				for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
				{
					for( UIVector::iterator it = m_UIVectorArray[i].begin() ;
						it != m_UIVectorArray[i].end() ;
						it++ )
					{
						if( (*it)->isShow() )
						{
							if( (*it)->isUpdata() )
							{
								(*it)->Update();
							}
						}
					}
				}
				//然后再提取buffer
				for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
				{
					if( NULL != m_pUIRendStructVector[i] )
					{
						m_pUIRendStructVector[i]->m_pShared->CleanUp();
					}
					for( UIVector::iterator it = m_UIVectorArray[i].begin() ;
						it != m_UIVectorArray[i].end() ;
						it++ )
					{
						if( (*it)->isShow() )
						{
							//if( (*it)->isUpdata() )
							//{
							//	(*it)->Update();
							//}
							if( NULL != m_pUIRendStructVector[i] )
							{
								(*it)->GetVertexBuffer(*(m_pUIRendStructVector[i]->m_pShared));
							}
						}
					}
					if( NULL != m_pUIRendStructVector[i] )
					{
						m_pUIRendStructVector[i]->m_pVertex->FlushVertexBuffer( 
							m_pUIRendStructVector[i]->m_pShared->VertexSize(), 
							static_cast<VertexTextureColor*>( m_pUIRendStructVector[i]->m_pShared->VertexPtr() ) );
						m_pUIRendStructVector[i]->m_pIndices->FlushIndiesBuffer( m_pUIRendStructVector[i]->m_pShared->IndiesSize(),  m_pUIRendStructVector[i]->m_pShared->IndiesPtr() );
						m_pUIRendStructVector[i]->m_pRender->SetDrawCount(m_pUIRendStructVector[i]->m_pShared->IndiesSize());
					}
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::Render(Engine& engine, UIManager::Layer layer)
	{
		//if ( m_isShow )//manager中用BOHGE_FORCEINLINE函数检查，提高效率
		{
			if( NULL != m_pUIRendStructVector[static_cast<int>(layer)]->m_pRender )
			{
				UIShader& ui = engine.GetShaderManager()->GetShader<UIShader>(ShaderManager::UIShader);
				ui.SetParamColor(m_Color);
				engine.GetDevice()->Draw( *m_pUIRendStructVector[static_cast<int>(layer)]->m_pRender, ui, m_pTexture );
				for(UIVector::iterator it = m_UIVectorArray[static_cast<int>(layer)].begin();
					it != m_UIVectorArray[static_cast<int>(layer)].end();
					it++
					)
				{
					(*it)->OnDraw( engine, m_Color, m_pTexture );
				}
			}
#ifdef DRAW_AABRECT
			m_pBox->Render(engine);
#endif
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool UIGroup::TouchResponse(bool isDown, const vector2f& point)//是否有任意可响应UI被按下
	{
		//if( m_isShow )//挪到manager检测，降低一些函数调用开销
		//{
		bool isSelect = false;
		if( ( m_vScale.m_x >0.99f && m_vScale.m_y >0.99f ? *m_pBox : *m_pBox * m_vScale ).isInsert(point) )
		{
			for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
			{
				for(UIVector::iterator it = m_UIVectorArray[i].begin();
					it != m_UIVectorArray[i].end();
					it++
					)
				{
					if( (*it)->isResponse() && (*it)->isActive() && (*it)->isShow() )
					{
						if( (*it)->isSelected(isDown, point) )
						{
							isSelect = true;
						}
					}
				}
			}
		}
		else //如果在大的触碰框范围外了那就通知全部的按钮跳到没有按下的状态
		{
			for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
			{
				for(UIVector::iterator it = m_UIVectorArray[i].begin();
					it != m_UIVectorArray[i].end();
					it++
					)
				{
					if( (*it)->isResponse() && (*it)->isActive() && (*it)->isShow() )
					{
						if( (*it)->isPushDonw() )
						{
							(*it)->Reset();
						}
					}
				}
			}
		}
		return isSelect;
		//}
		//return false;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::SetCenter(const vector2f& center)
	{
		vector2f dir = center - m_Center;
		m_Center = center;
		m_pBox->MoveDir(dir);
		for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
		{
			for(UIVector::iterator it = m_UIVectorArray[i].begin();
				it != m_UIVectorArray[i].end();
				it++
				)
			{
				(*it)->Move( dir );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::SetScale(const vector2f& scale)
	{
		m_vScale = scale;
		for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
		{
			for(UIVector::iterator it = m_UIVectorArray[i].begin();
				it != m_UIVectorArray[i].end();
				it++
				)
			{
				(*it)->SetScale( scale );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::SetShow(bool b)
	{
		m_isShow = b;
		for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
		{
			for(UIVector::iterator it = m_UIVectorArray[i].begin();
				it != m_UIVectorArray[i].end();
				it++
				)
			{
				(*it)->SetShow( m_isShow );
				(*it)->SetStringShow( m_isShow );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::setResponse(bool response)
	{
		m_isResponse = response;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::Renew()
	{
		m_isReflush = true;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIGroup::TopMost()
	{
		Engine::Instance().GetUIManager()->TopMost(this);
	}






	/////////////////////////
	//		UIData		   //
	/////////////////////////
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UIData::UIData()
		//:m_Reservation1(0),
		//m_Reservation2(0),
		:m_Reservation3(0),
		m_Reservation4(0),
		m_Reservation5(0),
		m_Reservation6(0),
		m_Reservation7(0),
		m_Reservation8(0),
		m_Reservation9(0),
		m_Reservation10(0),
		m_Reservation11(0),
		m_Reservation12(0),
		m_Reservation13(0),
		m_Reservation14(0),
		m_Reservation15(0),
		m_Reservation16(0)
	{

	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIData::Save(Serializer& file)
	{
		file<<m_Name;
		file<<m_Type;
		file<<m_Layer;
		file<<m_Center.m_x
			<<m_Center.m_y;
		file<<m_Tex1.first.m_x
			<<m_Tex1.first.m_y
			<<m_Tex1.second.m_x
			<<m_Tex1.second.m_y;
		file<<m_Tex2.first.m_x
			<<m_Tex2.first.m_y
			<<m_Tex2.second.m_x
			<<m_Tex2.second.m_y;
		file<<m_Size.m_x
			<<m_Size.m_y;
		file<<m_Tex2Size.m_x
			<<m_Tex2Size.m_y;
		file/*<<m_Reservation1
			<<m_Reservation2*/
			<<m_Reservation3
			<<m_Reservation4
			<<m_Reservation5
			<<m_Reservation6
			<<m_Reservation7
			<<m_Reservation8
			<<m_Reservation9
			<<m_Reservation10
			<<m_Reservation11
			<<m_Reservation12
			<<m_Reservation13
			<<m_Reservation14
			<<m_Reservation15
			<<m_Reservation16;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIData::Load(Serializer& file)
	{
		m_Name.clear();
		file>>m_Name;
		int type,layer;
		file>>type;
		file>>layer;
		m_Type = Engine::Instance().GetUIManager()->GetType(type);
		m_Layer= Engine::Instance().GetUIManager()->GetLayer(layer);
		file>>m_Center.m_x
			>>m_Center.m_y;
		file>>m_Tex1.first.m_x
			>>m_Tex1.first.m_y
			>>m_Tex1.second.m_x
			>>m_Tex1.second.m_y;
		file>>m_Tex2.first.m_x
			>>m_Tex2.first.m_y
			>>m_Tex2.second.m_x
			>>m_Tex2.second.m_y;
		file>>m_Size.m_x
			>>m_Size.m_y;
		file>>m_Tex2Size.m_x
			>>m_Tex2Size.m_y;
		file/*>>m_Reservation1
			>>m_Reservation2*/
			>>m_Reservation3
			>>m_Reservation4
			>>m_Reservation5
			>>m_Reservation6
			>>m_Reservation7
			>>m_Reservation8
			>>m_Reservation9
			>>m_Reservation10
			>>m_Reservation11
			>>m_Reservation12
			>>m_Reservation13
			>>m_Reservation14
			>>m_Reservation15
			>>m_Reservation16;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------









	/////////////////////////
	//		UIManager	   //
	/////////////////////////
	static const string TypeString[] =
	{
		"Static",
		"Button",
		"Slider",
		"List",
		"Edit",
	};
	static const string LayerString[] =
	{
		"Top",
		"Bottom",
	};

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UIBase* UIManager::Factory(UIGroup* group, const UIData& data)
	{
		UIBase* temp;
		switch (data.m_Type)
		{
		case UIManager::UI_STATIC: temp = NEW UIStatic(group, data);break;
		case UIManager::UI_BUTTON: temp = NEW UIButton(group, data);break;
		case UIManager::UI_SLIDER: temp = NEW UISlider(group, data);break;
		case UIManager::UI_LIST: temp = NEW UIList(group, data);break;
		case UIManager::UI_EDIT: temp = NEW UIEdit(group, data);break;
		}
		return temp;
	}





	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UIManager::UIManager(void)
		:m_isKeyboard(false)
	{
		int size = sizeof(TypeString) / sizeof(TypeString[0]);
		ASSERT( size == Type_Count );
		size = sizeof(LayerString) / sizeof(LayerString[0]);
		ASSERT( size == Layer_Count );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UIManager::~UIManager(void)
	{
		ASSERT( m_GroupList.empty() );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	string UIManager::GetTypeName(int type)
	{
		return TypeString[type];
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UIManager::Type UIManager::GetType(int index)
	{
		return static_cast<Type>(index);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	string UIManager::GetLayerName(int type)
	{
		return LayerString[type];
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UIManager::Layer UIManager::GetLayer(int index)
	{
		return static_cast<Layer>(index);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UIGroup* UIManager::CreateUIGroup( const char* file )
	{
		UIGroup* ui = NEW UIGroup( file );
		m_GroupList.push_back( ui );
		return ui;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIManager::DeleteUIGroup( UIGroup* ui )
	{
		if( NULL != ui )
		{
			for(GroupList::iterator it = m_GroupList.begin();//处理UI
				it != m_GroupList.end();
				it++ )	
			{
				if ( *it == ui )
				{
					SAFE_DELETE( *it );
					m_GroupList.erase( it );
					break;
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIManager::Render(Engine& engine, Layer layer)
	{
		for(GroupList::iterator it = m_GroupList.begin();
			it != m_GroupList.end();
			it++ )	
		{
			if( (*it)->isShow()
				&& (*it)->isManaged() ) //不是被托管的
			{
				(*it)->Render(engine, layer);
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool UIManager::TouchResponse(bool isDown, const vector2f& point)
	{
		if( m_isKeyboard )
		{
			HideKeyBorad();
		}
		bool isTouch = false ;
		for(GroupList::iterator it = m_GroupList.begin();
			it != m_GroupList.end();
			it++ )	
		{
			if( (*it)->isResponse() && (*it)->isShow() && !(*it)->isAnimation() )
			{
				if( (*it)->TouchResponse(isDown, point) )
				{
					isTouch = true;
				}
			}
		}
		return isTouch;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIManager::Update(uint millisecond)
	{
		for(GroupList::iterator it = m_GroupList.begin();
			it != m_GroupList.end();
			it++ )	
		{
			(*it)->Update(millisecond);
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIManager::PauseResponseAll()
	{
		for(GroupList::iterator it = m_GroupList.begin();
			it != m_GroupList.end();
			it++ )	
		{
			(*it)->setResponse(false);
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIManager::ResumeResponseAll()
	{
		for(GroupList::iterator it = m_GroupList.begin();
			it != m_GroupList.end();
			it++ )	
		{
			(*it)->setResponse(true);
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIManager::TopMost(UIGroup* ui)
	{
		m_GroupList.remove( ui );
		m_GroupList.push_back( ui );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIManager::OnActiveKeyborad(UIBase* needer, const vector4f& rect, uint maxsize, const std::wstring& was, bool isNumberOnly )
	{
		ASSERT( UI_EDIT == needer->UIType() );
		m_isKeyboard = true;//stop response all ui message
		m_eEditCallBakc.ClearConnect();//防止不等输完，就点击其他地方
		m_eEditCallBakc.Connect( make_bind( &UIEdit::OnReciveText, static_cast<UIEdit*>(needer) ) );
		Keyborad(rect, maxsize, was, isNumberOnly);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UIManager::OnReceiveText(const std::wstring& text)
	{
		if( m_isKeyboard )
		{
			m_eEditCallBakc.Multicast(text);
			m_eEditCallBakc.ClearConnect();
			m_isKeyboard = false;//resume response
		}
	}


	bool CheckName(const UIBase& ui, const string& name)//解决android移植问题
	{
		return ui.isUI(name);
	}


}