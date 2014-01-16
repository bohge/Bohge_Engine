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

#include "UI.h"
#include "Vertex.h"
#include "RendBuffer.h"
#include "ScriptBase.h"
#include "Language.h"
#include "Shaders2D.h"
#include "ShaderManage.h"



using namespace std;

extern void HideKeyBorad();


namespace BohgeEngine
{
	const int UIBase::Invalid = -1;
	//-------------------------------------------------------------------------------------
	UIBase::UIBase(IUIFather* father, bool isResponse, const UIData& data)
		:m_Father(father),
		m_eType(data.m_Type),
		m_eLayer(data.m_Layer),
		m_Name(data.m_Name),
		m_isResponse(isResponse),
		m_isTouched(false),
		m_isActive(true),
		m_isShow(true),
		m_isUpdata(false),
		m_pString(NULL),
		m_pBox(NULL),
		m_vScale(1,1),
		m_isDonw(false),
		m_Identification( Invalid )
	{
		ASSERT( COUNT_OF_VERTEX == 4 );
		m_Center = data.m_Center;
		m_Size = data.m_Size * 2.0f;
		float hsx = data.m_Size.m_x;
		float hsy = data.m_Size.m_y;
		m_pVertexArray = NEW VertexTextureColor[COUNT_OF_VERTEX];
		m_pVertexArray[0] = VertexTextureColor( vector4f(m_Center.m_x-hsx,m_Center.m_y+hsy,0), vector2f(data.m_Tex1.first.m_u,data.m_Tex1.second.m_v), Color::White );
		m_pVertexArray[1] = VertexTextureColor( vector4f(m_Center.m_x-hsx,m_Center.m_y-hsy,0), vector2f(data.m_Tex1.first.m_u,data.m_Tex1.first.m_v), Color::White );
		m_pVertexArray[2] = VertexTextureColor( vector4f(m_Center.m_x+hsx,m_Center.m_y+hsy,0), vector2f(data.m_Tex1.second.m_u,data.m_Tex1.second.m_v), Color::White );
		m_pVertexArray[3] = VertexTextureColor( vector4f(m_Center.m_x+hsx,m_Center.m_y-hsy,0), vector2f(data.m_Tex1.second.m_u,data.m_Tex1.first.m_v), Color::White );
		if ( m_isResponse )
		{
			m_pBox = NEW aabrectf(m_Center, m_Size);
		}
	}
	//-------------------------------------------------------------------------------------
	UIBase::~UIBase(void)
	{
		if( NULL != m_pString )
		{
			Engine::Instance().GetStringManager()->DestroyString( m_pString );
		}
		SAFE_DELETE( m_pBox );
		SAFE_DELETE_ARRAY(m_pVertexArray);
	}
	//-------------------------------------------------------------------------------------
	void UIBase::MakeBindBox()
	{
		if( NULL == m_pBox)
		{
			m_pBox = NEW aabrectf(m_Center, m_Size);
		}
	}
	//-------------------------------------------------------------------------------------
	void UIBase::GetVertexBuffer(SharedBuffer& input) const 
	{	
		//DEBUGLOG("Renew %s\n" ,this->m_Name.c_str());
		static ushort Indies[COUNT_OF_INDIES]={0,1,2, 2,1,3};
		input.AddVertex( m_pVertexArray, Indies );
	}
	//-------------------------------------------------------------------------------------
	void UIBase::SetCenter(const vector2f& center)
	{
		m_isUpdata = true;
		m_Center = center;
		if( NULL != m_pBox )
		{
			m_pBox->SetCenter(m_Center );//aabrectf没有缩放机制，这里需要传入缩放后的点
		}
		if( NULL != m_pString )	//如果已经有文本了，就清除之前的
		{
			m_pString->SetPosition(m_Center, BString::Center);//String本身会有scale机制，所以这里只输入原始坐标
		}
		if( NULL != m_Father )
		{
			m_Father->Renew();
		}
	}
	//-------------------------------------------------------------------------------------
	void UIBase::SetCenterForce(const vector2f& center)
	{
		this->SetCenter(center);
		this->Update();
	}
	//-------------------------------------------------------------------------------------
	void UIBase::Move(const vector2f& dis)
	{
		this->SetCenter( m_Center + dis );
	}
	//-------------------------------------------------------------------------------------
	bool UIBase::isSelected(bool isDown, const vector2f& point)
	{
		bool isSelecte = this->GetBindBox().isInsert(point);
		if (isSelecte)
		{
			if (isDown)
			{
				m_isTouched = true;
				if( !m_isDonw )
				{
					m_isDonw = true;
					this->OnMouseDown(point);
					if( NULL != m_Father)
					{
						m_Father->Renew();
					}
				}
				else
				{
					this->OnMouseMove(point);
				}
			} 
			else
			{
				m_isDonw = false;
				m_isTouched = false;
				this->OnMouseUp(point);
				this->OnAction(point);
				if( NULL != m_Father)
				{
					m_Father->Renew();
				}
			}
		}
		else if( m_isTouched ) //曾经被选择但是现在不在范围内了
		{
			m_isDonw = false;
			m_isTouched = false;
			this->OnMouseUp(point);
			if( NULL != m_Father)
			{
				m_Father->Renew();
			}
		}
		return isSelecte;
	}
	//-------------------------------------------------------------------------------------
	bool UIBase::isUI(const string& name) const
	{
		return m_Name == name;
	}
	//-------------------------------------------------------------------------------------
	void UIBase::SetString( const vector2f& fontsize, const Color& clr, bool isCover, const char* file )
	{
		SetString( fontsize, clr, isCover, STRING.GetString( file ) );
	}
	//-------------------------------------------------------------------------------------
	void UIBase::SetString(const vector2f& fontsize, const Color& clr, bool isCover, const wstring& wstr)
	{
		if( NULL != m_pString )	//如果已经有文本了，就清除之前的
		{
			Engine::Instance().GetStringManager()->DestroyString( m_pString );
		}
		m_StringColor = clr;
		if( m_isActive )
		{
			m_pString = Engine::Instance().GetStringManager()->MakeString(fontsize, clr, wstr);
		}
		else
		{
			m_pString = Engine::Instance().GetStringManager()->MakeString(fontsize,  Color::Gray, wstr);
		}
		if( this->isShow() )//&& m_Father->isShow() )
		{
			m_pString->Show();
		}
		else
		{
			m_pString->Hide();
		}
		m_pString->SetPosition( m_Center, BString::Center );
		m_pString->SetScale( m_vScale );
		m_pString->SetManaged( !isCover );//UI自己绘制
	}
	//-------------------------------------------------------------------------------------
	void UIBase::SetShow(bool show)
	{
		if( m_isShow != show )
		{
			m_isShow = show;
			m_Father->Renew();
			this->SetStringShow(m_isShow);
		}
	}
	//-------------------------------------------------------------------------------------
	void UIBase::SetStringShow(bool isShow)
	{
		if ( NULL != m_pString)
		{
			//if( m_Father->isShow() )
			{
				if( isShow )
				{
					m_pString->Show();
				}
				else
				{
					m_pString->Hide();
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------
	void UIBase::SetScale(const vector2f& scale)
	{
		m_isUpdata = true;
		m_vScale = scale;
		if( NULL != m_pString )
		{
			m_pString->SetScale( scale );
		}
		if( NULL != m_Father )
		{
			m_Father->Renew();
		}
	}
	//-------------------------------------------------------------------------------------
	void UIBase::Update()
	{
		m_isUpdata = false;
		//vector2f centerscale = m_Center * m_vScale;//这个不缩放，只缩放尺寸
		vector2f centerscale = m_Center;
		float hsx = m_Size.m_x * m_vScale.m_x * 0.5f;
		float hsy = m_Size.m_y * m_vScale.m_y * 0.5f;

		m_pVertexArray[0].SetVertex( vector4f(centerscale.m_x-hsx,centerscale.m_y+hsy,0) );
		m_pVertexArray[1].SetVertex( vector4f(centerscale.m_x-hsx,centerscale.m_y-hsy,0) );
		m_pVertexArray[2].SetVertex( vector4f(centerscale.m_x+hsx,centerscale.m_y+hsy,0) );
		m_pVertexArray[3].SetVertex( vector4f(centerscale.m_x+hsx,centerscale.m_y-hsy,0) );

	}
	//-------------------------------------------------------------------------------------
	void UIBase::Reset()
	{
		m_isDonw = false;
		this->OnMouseUp(vector2f());
		m_Father->Renew();
	}
	//-------------------------------------------------------------------------------------
	uint UIBase::GetCountOfNode()//返回当前的node个数
	{
		return 1;
	}
	//-------------------------------------------------------------------------------------
	uint UIBase::GetElementCountPerVertex()
	{
		return VertexTextureColor::GetElementCount();
	}
	//-------------------------------------------------------------------------------------
	void UIBase::OnDraw(Engine& engine, const Color& clr, const Texture* tex)
	{
#ifdef DRAW_AABRECT
		if( NULL != m_pBox )
		{
			(*m_pBox * m_vScale).Render(engine);
		}
#endif // DAW
		if( NULL != m_pString
			&& !m_pString->isManaged() )
		{
			m_pString->Render( engine );
		}
	}
	//-------------------------------------------------------------------------------------
	void UIBase::SetActive(bool isActive)
	{
		m_isActive = isActive;
		if( isActive )
		{
			if ( NULL != m_pString)m_pString->SetColor( m_StringColor );
			m_pVertexArray[0].SetColor( Color::White );
			m_pVertexArray[1].SetColor( Color::White );
			m_pVertexArray[2].SetColor( Color::White );
			m_pVertexArray[3].SetColor( Color::White );
		}
		else
		{
			static Color disableClr(0.6f, 0.6f, 0.6f, 0.6f);
			if ( NULL != m_pString)m_pString->SetColor( Color::Gray );
			m_pVertexArray[0].SetColor( disableClr );
			m_pVertexArray[1].SetColor( disableClr );
			m_pVertexArray[2].SetColor( disableClr );
			m_pVertexArray[3].SetColor( disableClr );
		}
		if( NULL != m_Father )
		{
			m_Father->Renew();
		}
	}









	/////////////////////////
	//		UIStatic	   //
	/////////////////////////
	//-------------------------------------------------------------------------------------
	UIStatic::UIStatic(IUIFather* father, const UIData& data)
		:UIBase(father, false, data)
	{

	}
	//-------------------------------------------------------------------------------------
	UIStatic::~UIStatic()
	{

	}




	/////////////////////////
	//		UIButton	   //
	/////////////////////////
	//-------------------------------------------------------------------------------------
	UIButton::UIButton(IUIFather* father, const UIData& data)
		:UIBase(father, true, data),
		m_CoordNormal(data.m_Tex1),
		m_CoordHighLight(data.m_Tex2)
	{
	}
	//-------------------------------------------------------------------------------------
	UIButton::~UIButton()
	{
	}
	//-------------------------------------------------------------------------------------
	void UIButton::OnMouseDown(const vector2f& pos)
	{
		//DEBUGLOG("Mouse Donw\n");
		m_pVertexArray[0].Coordinate() = vector2f(m_CoordHighLight.first.m_u,m_CoordHighLight.second.m_v);
		m_pVertexArray[1].Coordinate() = vector2f(m_CoordHighLight.first.m_u,m_CoordHighLight.first.m_v);
		m_pVertexArray[2].Coordinate() = vector2f(m_CoordHighLight.second.m_u,m_CoordHighLight.second.m_v);
		m_pVertexArray[3].Coordinate() = vector2f(m_CoordHighLight.second.m_u,m_CoordHighLight.first.m_v);
	}
	//-------------------------------------------------------------------------------------
	void UIButton::OnMouseUp(const vector2f& pos)
	{
		//DEBUGLOG("Mouse up\n");
		m_pVertexArray[0].Coordinate() = vector2f(m_CoordNormal.first.m_u,m_CoordNormal.second.m_v);
		m_pVertexArray[1].Coordinate() = vector2f(m_CoordNormal.first.m_u,m_CoordNormal.first.m_v);
		m_pVertexArray[2].Coordinate() = vector2f(m_CoordNormal.second.m_u,m_CoordNormal.second.m_v);
		m_pVertexArray[3].Coordinate() = vector2f(m_CoordNormal.second.m_u,m_CoordNormal.first.m_v);
	}
	//-------------------------------------------------------------------------------------
	void UIButton::OnAction(const vector2f& pos)
	{
		m_Event.Multicast( *this );
		for ( LuaCallBackList::iterator it = m_LuaCallBack.begin();
			it != m_LuaCallBack.end();
			it++ )
		{
			it->first->CallLua<bool>( it->second.c_str(), this );
		}
	}
	//-------------------------------------------------------------------------------------
	void UIButton::AddResponse(IFunction<bool,UIBase&>* const func)
	{
		m_Event.Connect( func );
	}
	//-------------------------------------------------------------------------------------
	void UIButton::AddResponse( ScriptBase* scrip, const char* func )
	{
		m_LuaCallBack.push_back( std::make_pair( scrip, func ) );
	}

	//-------------------------------------------------------------------------------------
	void UIButton::SetActive(bool isActive)
	{
		m_isActive = isActive;
		if( isActive )
		{
			if ( NULL != m_pString)m_pString->SetColor( m_StringColor );
			m_pVertexArray[0].SetColor( Color::White );
			m_pVertexArray[1].SetColor( Color::White );
			m_pVertexArray[2].SetColor( Color::White );
			m_pVertexArray[3].SetColor( Color::White );
		}
		else
		{
			static Color disableClr(0.6f, 0.6f, 0.6f, 0.6f);
			if ( NULL != m_pString)m_pString->SetColor( Color::Gray );
			m_pVertexArray[0].SetColor( disableClr );
			m_pVertexArray[1].SetColor( disableClr );
			m_pVertexArray[2].SetColor( disableClr );
			m_pVertexArray[3].SetColor( disableClr );
		}
		if( NULL != m_Father )
		{
			m_Father->Renew();
		}
	}


	/////////////////////////
	//		UISlider	   //
	/////////////////////////
	//-------------------------------------------------------------------------------------
	UISlider::UISlider(IUIFather* father, const UIData& data)
		:UIBase(father, true, data),
		m_CoordElement(data.m_Tex2),
		m_SliderCount(0),
		m_VisuableSliderCount(0),
		m_fPercentage(1.0),
		m_fMinPercentage(0.0)
	{
		vector2f slidersize = data.m_Size * 2.0f;
		//计算滑动条的大小
		m_ElementSize = data.m_Tex2Size * 2.0f; //每个元素尺寸
		float hsx = m_ElementSize.m_x / 2.0;
		float hsy = m_ElementSize.m_y / 2.0;
		int includeSliderElement = slidersize.m_x / m_ElementSize.m_x - 1; //包含多少个元素 - 1意思是两段预留一个图像的宽度
		float c = static_cast<float>(includeSliderElement) / 2.0;
		float Beging = data.m_Center.m_x -  c * m_ElementSize.m_x + hsx;
		m_pSliderVertexArray = NEW VertexTextureColor[ includeSliderElement * COUNT_OF_VERTEX ];
		for( int i = 0; i < includeSliderElement; i ++ )
		{
			m_pSliderVertexArray[i*COUNT_OF_VERTEX+0] = VertexTextureColor( vector4f( Beging + m_ElementSize.m_x * i - hsx ,data.m_Center.m_y+hsy, 0), 	vector2f( data.m_Tex2.first.m_u, data.m_Tex2.second.m_v), Color::White );
			m_pSliderVertexArray[i*COUNT_OF_VERTEX+1] = VertexTextureColor( vector4f( Beging + m_ElementSize.m_x * i - hsx ,data.m_Center.m_y-hsy, 0), 	vector2f( data.m_Tex2.first.m_u, data.m_Tex2.first.m_v), Color::White );
			m_pSliderVertexArray[i*COUNT_OF_VERTEX+2] = VertexTextureColor( vector4f( Beging + m_ElementSize.m_x * i + hsx ,data.m_Center.m_y+hsy, 0), 	vector2f( data.m_Tex2.second.m_u, data.m_Tex2.second.m_v), Color::White );
			m_pSliderVertexArray[i*COUNT_OF_VERTEX+3] = VertexTextureColor( vector4f( Beging + m_ElementSize.m_x * i + hsx ,data.m_Center.m_y-hsy, 0), 	vector2f( data.m_Tex2.second.m_u, data.m_Tex2.first.m_v), Color::White );
			m_pBox->Contains( vector2f( Beging + m_ElementSize.m_x * i - hsx ,data.m_Center.m_y+hsy ) );
			m_pBox->Contains( vector2f( Beging + m_ElementSize.m_x * i - hsx ,data.m_Center.m_y-hsy ) );
			m_pBox->Contains( vector2f( Beging + m_ElementSize.m_x * i + hsx ,data.m_Center.m_y+hsy ) );
			m_pBox->Contains( vector2f( Beging + m_ElementSize.m_x * i + hsx ,data.m_Center.m_y-hsy ) );
		}
		m_SliderCount = includeSliderElement;
		m_VisuableSliderCount = m_SliderCount;
	}
	//-------------------------------------------------------------------------------------
	UISlider::~UISlider()
	{
		SAFE_DELETE_ARRAY(m_pSliderVertexArray);
	}
	//-------------------------------------------------------------------------------------
	uint UISlider::GetCountOfNode()
	{
		return UIBase::GetCountOfNode() + m_SliderCount;
	}
	//-------------------------------------------------------------------------------------
	void UISlider::OnMouseMove(const vector2f& pos)
	{
		float percen= Math::Clamp0to1( ( pos.m_x - ( m_Center.m_x - m_Size.m_x / 2.0 ) ) / ( m_ElementSize.m_x * m_SliderCount ) );
		if( percen >= m_fMinPercentage )
		{
			m_fPercentage = percen;
		}
		else
		{
			m_fPercentage = m_fMinPercentage;
		}
		uint visuable = m_SliderCount * m_fPercentage;
		if ( m_VisuableSliderCount != visuable && visuable <= m_SliderCount )
		{
			m_Father->Renew();
			m_VisuableSliderCount = visuable;
		}
		this->OnAction(pos);
	}
	//-------------------------------------------------------------------------------------
	void UISlider::SetPercentage(clamp percentage)
	{
		if( m_isActive )
		{
			m_fPercentage = Math::Clamp0to1( percentage );
			uint visuable = m_SliderCount * m_fPercentage;
			if ( m_VisuableSliderCount != visuable && visuable <= m_SliderCount )
			{
				m_Father->Renew();
				m_VisuableSliderCount = visuable;
			}	
		}
	}
	//-------------------------------------------------------------------------------------
	void UISlider::GetVertexBuffer(SharedBuffer& input) const 
	{
		UIBase::GetVertexBuffer(input);//先读取地板的buffer
		const static ushort Indies[COUNT_OF_INDIES]={0,1,2, 2,1,3};
		for ( int i = 0 ; i < m_VisuableSliderCount ; i ++ )
		{
			input.AddVertex( &m_pSliderVertexArray[i*COUNT_OF_VERTEX], Indies );
		}
	}
	//-------------------------------------------------------------------------------------
	void UISlider::Update()
	{
		UIBase::Update();
		float hsx = m_ElementSize.m_x * m_vScale.m_x / 2.0;
		float hsy = m_ElementSize.m_y * m_vScale.m_y / 2.0;
		int includeSliderElement = m_Size.m_x / m_ElementSize.m_x - 1; //包含多少个元素 - 1意思是两段预留一个图像的宽度
		float c = static_cast<float>(includeSliderElement) / 2.0;
		float Beging = m_Center.m_x * m_vScale.m_x -  c * m_ElementSize.m_x * m_vScale.m_x + hsx;
		float sizex = m_ElementSize.m_x * m_vScale.m_x;
		float ybegin = m_Center.m_y*m_vScale.m_y;
		for( int i = 0; i < m_SliderCount; i ++ )
		{
			m_pSliderVertexArray[i*COUNT_OF_VERTEX+0].SetVertex( vector4f( Beging + sizex * i - hsx ,ybegin + hsy, 0) );
			m_pSliderVertexArray[i*COUNT_OF_VERTEX+1].SetVertex( vector4f( Beging + sizex * i - hsx ,ybegin - hsy, 0) );
			m_pSliderVertexArray[i*COUNT_OF_VERTEX+2].SetVertex( vector4f( Beging + sizex * i + hsx ,ybegin + hsy, 0) );
			m_pSliderVertexArray[i*COUNT_OF_VERTEX+3].SetVertex( vector4f( Beging + sizex * i + hsx ,ybegin - hsy, 0) );
		}
	}
	//-------------------------------------------------------------------------------------
	void UISlider::AddResponse(IFunction<bool,UIBase&>* const func)
	{
		m_Event.Connect( func );
	}
	//-------------------------------------------------------------------------------------
	void UISlider::AddResponse( ScriptBase* scrip, const char* func )
	{
		m_LuaCallBack.push_back( std::make_pair( scrip, func ) );
	}
	//-------------------------------------------------------------------------------------
	void UISlider::OnAction(const vector2f& pos)
	{
		m_Event.Multicast( *this );
		for ( LuaCallBackList::iterator it = m_LuaCallBack.begin();
			it != m_LuaCallBack.end();
			it++ )
		{
			it->first->CallLua<bool>( it->second.c_str(), this );
		}
	}
	//-------------------------------------------------------------------------------------
	void UISlider::SetActive(bool isActive)
	{
		m_isActive = isActive;
		if( isActive )
		{
			if ( NULL != m_pString)m_pString->SetColor( m_StringColor );
			for( int i = 0; i < m_SliderCount; i ++ )
			{
				m_pSliderVertexArray[i*COUNT_OF_VERTEX+0].SetColor( Color::White );
				m_pSliderVertexArray[i*COUNT_OF_VERTEX+1].SetColor( Color::White );
				m_pSliderVertexArray[i*COUNT_OF_VERTEX+2].SetColor( Color::White );
				m_pSliderVertexArray[i*COUNT_OF_VERTEX+3].SetColor( Color::White );
			}
		}
		else
		{
			static Color disableClr(0.6f, 0.6f, 0.6f, 0.6f);
			if ( NULL != m_pString)m_pString->SetColor( Color::Gray );
			for( int i = 0; i < m_SliderCount; i ++ )
			{
				m_pSliderVertexArray[i*COUNT_OF_VERTEX+0].SetColor( disableClr );
				m_pSliderVertexArray[i*COUNT_OF_VERTEX+1].SetColor( disableClr );
				m_pSliderVertexArray[i*COUNT_OF_VERTEX+2].SetColor( disableClr );
				m_pSliderVertexArray[i*COUNT_OF_VERTEX+3].SetColor( disableClr );
			}
		}
		if( NULL != m_Father )
		{
			m_Father->Renew();
		}
	}




	/////////////////////////
	//		UIList		   //
	/////////////////////////
	//-------------------------------------------------------------------------------------
	UIList::UIList(IUIFather* father, const UIData& data)
		:UIBase(father, true, data),
		m_isUp(false),
		m_LastMouseY(0),
		m_CurrentIndex(0)
	{
		vector2f listsize = data.m_Size * 2.0f;
		//计算滑动条的大小
		m_ListItemSize = data.m_Tex2Size * 2.0f; //每个元素尺寸
		float hsx = m_ListItemSize.m_x / 2.0;
		float hsy = m_ListItemSize.m_y / 2.0;
		m_ContainerCount = Math::Ceil( listsize.m_y / m_ListItemSize.m_y ) + 1; //包含多少个元素 + 1意思是需要一个在外部循环


		//构造绘制Itme的buffer
		m_pBuffer = NEW SharedBuffer(
			VertexTextureColor::GetElementCount(),
			UIBase::GetCountOfVertex(),
			UIBase::GetCountOfIndes(),
			m_ContainerCount);
		//计算容器的用量
		float Beging = data.m_Center.m_y + data.m_Size.m_y + hsy;
		UIData item = data;
		item.m_Center.m_y = Beging;
		item.m_Tex1 = item.m_Tex2;
		item.m_Size = item.m_Tex2Size;
		for ( int i = 0 ; i < m_ContainerCount ; i ++ )
		{
			item.m_Center.m_y -= m_ListItemSize.m_y ;
			UIStatic* temp = NEW UIStatic(this, item);
			temp->MakeBindBox();//判断点击的Item
			temp->SetShow(false);
			//temp->GetVertexBuffer( *m_pBuffer );
			m_ContainerVector.push_back( make_pair(temp, m_ItemList.end() ) );
		}
		m_pRender = NEW RendBuffer( UIBase::GetRenderMode() );
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_DYNAMIC );
		m_pRender->SetVertexBuffer( m_pVertexBuffer );
		m_pRender->SetIndicesBuffer( m_pIndicesBuffer );
		m_pVertexBuffer->FlushVertexBuffer( m_pBuffer->CountOfAllVertex(), 
			static_cast<VertexTextureColor*>(m_pBuffer->VertexPtr()) );
		m_pIndicesBuffer->FlushIndiesBuffer( m_pBuffer->CountOfAllIndies(), m_pBuffer->IndiesPtr() );
		m_pRender->SetDrawCount( 0 );


		//设置stencil绘制项目
		{
			float hsx = m_Size.m_x / 2.0f;
			float hsy = m_Size.m_y / 2.0f;
			Vertex stencil[4];
			stencil[0] = Vertex( vector4f(m_Center.m_x-hsx,m_Center.m_y+hsy,0) );
			stencil[1] = Vertex( vector4f(m_Center.m_x-hsx,m_Center.m_y-hsy,0) );
			stencil[2] = Vertex( vector4f(m_Center.m_x+hsx,m_Center.m_y+hsy,0) );
			stencil[3] = Vertex( vector4f(m_Center.m_x+hsx,m_Center.m_y-hsy,0) );
			m_pStencilRender = NEW RendBuffer( Device::RM_TRIANGLE_STRIP );
			m_pStencilVertexBuffer = NEW VertexBuffer( Device::MU_STATIC );
			m_pStencilIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
			m_pStencilRender->SetVertexBuffer( m_pStencilVertexBuffer );
			m_pStencilRender->SetIndicesBuffer( m_pStencilIndicesBuffer );
			m_pStencilVertexBuffer->FlushVertexBuffer( 4, stencil );
			static ushort index[]={0,1,2,3};
			m_pStencilIndicesBuffer->FlushIndiesBuffer( 4, index );
		}	
	}
	//-------------------------------------------------------------------------------------
	UIList::~UIList()
	{
		for (ContainerVector::iterator it = m_ContainerVector.begin();
			it != m_ContainerVector.end();
			it ++ )
		{
			SAFE_DELETE(it->first);
		}
		for (ItemList::iterator it = m_ItemList.begin();
			it != m_ItemList.end();
			it ++ )
		{
			SAFE_DELETE( (*it) );
		}
		SAFE_DELETE( m_pBuffer );
		SAFE_DELETE( m_pRender );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
		SAFE_DELETE( m_pStencilRender );
		SAFE_DELETE( m_pStencilVertexBuffer );
		SAFE_DELETE( m_pStencilIndicesBuffer );
	}
	//-------------------------------------------------------------------------------------
	void UIList::OnMouseDown(const vector2f& pos)
	{
		m_LastMouseY = pos.m_y;
		m_DownMouseY = pos.m_y;
		m_isAction = true;
	}
	//-------------------------------------------------------------------------------------
	void UIList::OnMouseMove(const vector2f& pos)
	{
		if( 0 == m_ItemList.size() )
		{
			return;
		}
		if (m_isAction)
		{
			if( Math::Abs(m_DownMouseY - pos.m_y) >=0.05 )
			{
				m_isAction = false;
			}
		}
		//计算位置
		vector2f dir;
		dir.m_y = pos.m_y - m_LastMouseY;
		m_LastMouseY = pos.m_y;
		m_isUp = dir.m_y > 0.0f ? true : false;//当前鼠标的移动方向时候是向上的
		float lastitemCenterY = (m_ContainerVector.begin()->first)->GetCenter().m_y;//item的中心位置
		if( m_isUp )
		{
			//首先判断是否可以继续移动
			float listbottom = this->GetCenter().m_y - this->GetSize().m_y / 2;
			for (ContainerVector::iterator it = m_ContainerVector.begin();
				it != m_ContainerVector.end();
				it ++ )
			{
				float itemedge = (it->first)->GetCenter().m_y - (it->first)->GetSize().m_y / 2;//下边框
				if ( it->second == (++(m_ItemList.rbegin())).base() //如果最后一个已在显示队列中
					&& itemedge >= listbottom )//且它已经完全显示
				{
					return;
				}
			}
			for (ContainerVector::iterator it = m_ContainerVector.begin();//找到最下方Y位置
				it != m_ContainerVector.end();
				it ++ )
			{
				if( (it->first)->GetCenter().m_y < lastitemCenterY )
				{
					lastitemCenterY = (it->first)->GetCenter().m_y;
				}
			}
		}
		else
		{
			//首先判断是否可以继续移动
			float lisup = this->GetCenter().m_y + this->GetSize().m_y / 2;
			for (ContainerVector::iterator it = m_ContainerVector.begin();
				it != m_ContainerVector.end();
				it ++ )
			{
				float itemedge = (it->first)->GetCenter().m_y + (it->first)->GetSize().m_y / 2;//上边框
				if ( it->second == m_ItemList.begin() //如果第一个已在显示队列中
					&& itemedge <= lisup )//且它已经完全显示
				{
					return;
				}
			}
			for (ContainerVector::iterator it = m_ContainerVector.begin();//找到最上方的Y位置
				it != m_ContainerVector.end();
				it ++ )
			{
				if( (it->first)->GetCenter().m_y > lastitemCenterY )
				{
					lastitemCenterY = (it->first)->GetCenter().m_y;
				}
			}
		}
		//开始计算新的位置
		for (ContainerVector::iterator it = m_ContainerVector.begin();
			it != m_ContainerVector.end();
			it ++ )
		{
			(it->first)->SetCenterForce( (it->first)->GetCenter() + dir );
			if (m_ItemList.end() != it->second)
			{
				(*(it->second))->SetPostion( it->first->GetCenter() );
			}
		}
		//计算是否超出
		if(m_isUp)
		{
			float listedge = this->GetCenter().m_y + this->GetSize().m_y / 2;//list的上边框
			float itemedge = (m_ContainerVector[m_CurrentIndex].first)->GetCenter().m_y - (m_ContainerVector[m_CurrentIndex].first)->GetSize().m_y / 2; //第一个UI的下边框
			if( listedge <= itemedge ) //如果item下边框都大于list上边框说明出界了，放到最后一个位置
			{
				int last = m_CurrentIndex-1 < 0 ? m_ContainerCount - 1: m_CurrentIndex-1;//找到最后一个容器
				vector2f cen = m_ContainerVector[last].first->GetCenter();
				cen.m_y -= m_ContainerVector[m_CurrentIndex].first->GetSize().m_y;
				m_ContainerVector[m_CurrentIndex].first->SetCenterForce( cen );

				//操作Item
				if( m_ItemList.end() != m_ContainerVector[m_CurrentIndex].second )
				{
					(*(m_ContainerVector[m_CurrentIndex].second))->OnInvisable();//撤销旧的Item
				}
				if( m_ItemList.end() != m_ContainerVector[last].second )
				{
					m_ContainerVector[m_CurrentIndex].second = m_ContainerVector[last].second;
					m_ContainerVector[m_CurrentIndex].second++;
					if( m_ItemList.end() != m_ContainerVector[m_CurrentIndex].second )
					{
						(*(m_ContainerVector[m_CurrentIndex].second))->OnVisable();
						(*(m_ContainerVector[m_CurrentIndex].second))->SetPostion( m_ContainerVector[m_CurrentIndex].first->GetCenter() );
					}
				}
				else
				{
					m_ContainerVector[m_CurrentIndex].second = m_ItemList.end();
				}
				//改变当前志向的第一个Item
				m_CurrentIndex = m_CurrentIndex+1 >= m_ContainerCount ? 0 : m_CurrentIndex+1;
			}
		}
		else
		{
			float listedge = this->GetCenter().m_y + this->GetSize().m_y / 2;//list的上边框
			float itemedge = (m_ContainerVector[m_CurrentIndex].first)->GetCenter().m_y + (m_ContainerVector[m_CurrentIndex].first)->GetSize().m_y / 2; //第一个UI的上边框
			if( itemedge < listedge  //如果item上边框都校于list上边框说明不够了，从后面拉取一个
				&& m_ItemList.begin() != m_ContainerVector[m_CurrentIndex].second )//且头一个不能是第一个
			{
				int last = m_CurrentIndex-1 < 0 ? m_ContainerCount - 1: m_CurrentIndex-1;//找到最后一个容器
				vector2f cen = m_ContainerVector[m_CurrentIndex].first->GetCenter();
				cen.m_y += m_ContainerVector[m_CurrentIndex].first->GetSize().m_y;
				m_ContainerVector[last].first->SetCenterForce( cen );//拽上来

				//操作Item
				if( m_ItemList.end() != m_ContainerVector[last].second )
				{
					(*(m_ContainerVector[last].second))->OnInvisable();//撤销旧的Item
				}
				if( m_ItemList.begin() != m_ContainerVector[m_CurrentIndex].second )
				{
					m_ContainerVector[last].second = m_ContainerVector[m_CurrentIndex].second;
					m_ContainerVector[last].second--;
					(*(m_ContainerVector[last].second))->OnVisable();
					(*(m_ContainerVector[last].second))->SetPostion( m_ContainerVector[last].first->GetCenter() );
				}
				else
				{
					m_ContainerVector[last].second = m_ItemList.end();
				}
				//改变当前志向的第一个Item
				m_CurrentIndex = last;
			}
		}
		//开始计算新的位置
		//for (ContainerVector::iterator it = m_ContainerVector.begin();
		//	it != m_ContainerVector.end();
		//	it ++ )
		//{
		//	if(m_isUp)
		//	{
		//		float itemedge = (it->first)->GetCenter().m_y - (it->first)->GetSize().m_y / 2; //下边框
		//		if( listedge <= itemedge ) //如果item下边框都大于list上边框说明出界了，放到最后一个位置
		//		{
		//			(it->first)->SetCenter( vector2f( (it->first)->GetCenter().m_x, lastitemCenterY - (it->first)->GetSize().m_y  ));
		//			(it->first)->SetCenterForce( (it->first)->GetCenter() + dir );

		//			int last = m_CurrentIndex-1 < 0 ? m_ContainerCount - 1: m_CurrentIndex-1;//找到最后一个容器
		//			//改变m_CurrentIndex指向的第一个容器
		//			m_CurrentIndex = (m_CurrentIndex+1) >= m_ContainerCount ? 0 : m_CurrentIndex+1;
		//			//激活新的Item，撤销旧的Item
		//			if( m_ItemList.end() != it->second )
		//			{
		//				(*(it->second))->OnInvisable();//撤销旧的Item
		//			}
		//			if( m_ItemList.end() != m_ContainerVector[last].second )
		//			{
		//				it->second = m_ContainerVector[last].second;
		//				it->second++;
		//				if( m_ItemList.end() != it->second )
		//				{
		//					(*(it->second))->OnVisable();
		//					(*(it->second))->SetPostion( it->first->GetCenter() );
		//				}
		//			}
		//			else
		//			{
		//				it->second = m_ItemList.end();
		//			}
		//		}
		//		else
		//		{
		//			(it->first)->SetCenterForce( (it->first)->GetCenter() + dir );
		//			if (m_ItemList.end() != it->second)
		//			{
		//				(*(it->second))->SetPostion( it->first->GetCenter() );
		//			}
		//		}
		//	}
		//	else
		//	{
		//		float itemedge = (it->first)->GetCenter().m_y + (it->first)->GetSize().m_y / 2;//上边框
		//		if( listedge >= itemedge ) //如果item上边框都小于list下边框说明出界了，放到最上方位置
		//		{
		//			(it->first)->SetCenter( vector2f( (it->first)->GetCenter().m_x, lastitemCenterY + (it->first)->GetSize().m_y  ));
		//			(it->first)->SetCenterForce( (it->first)->GetCenter() + dir );

		//			int wasfirst = m_CurrentIndex;
		//			m_CurrentIndex = (m_CurrentIndex-1) < 0 ? m_ContainerCount - 1: m_CurrentIndex-1;//找到最后一个容器
		//			//激活新的Item，撤销旧的Item
		//			if( m_ItemList.end() != it->second )
		//			{
		//				(*(it->second))->OnInvisable();//撤销旧的Item
		//			}
		//			if( m_ItemList.end() != m_ContainerVector[wasfirst].second )
		//			{
		//				it->second = m_ContainerVector[wasfirst].second;
		//				if( m_ItemList.begin() != it->second )
		//				{
		//					it->second--;
		//					if( m_ItemList.end() != it->second )
		//					{
		//						(*(it->second))->OnVisable();
		//						(*(it->second))->SetPostion( it->first->GetCenter() );
		//					}
		//				}
		//				else
		//				{
		//					it->second = m_ItemList.end();
		//				}
		//			}
		//			else
		//			{
		//				it->second = m_ItemList.end();
		//			}
		//		}
		//		else
		//		{
		//			(it->first)->SetCenterForce( (it->first)->GetCenter() + dir );
		//			if (m_ItemList.end() != it->second)
		//			{
		//				(*(it->second))->SetPostion( it->first->GetCenter() );
		//			}
		//		}
		//	}
		//}
	}
	//-------------------------------------------------------------------------------------
	void UIList::OnAction(const vector2f& pos)
	{
		if (m_isAction)
		{
			for (ContainerVector::iterator container = m_ContainerVector.begin();//找到最下方Y位置
				container != m_ContainerVector.end();
				container ++ )
			{
				if( container->first->isSelected( true, pos ) )
				{
					if( m_ItemList.end() != container->second )
					{
						m_Event.Multicast( **(container->second) );
						for ( LuaCallBackList::iterator lua = m_LuaCallBack.begin();
							lua != m_LuaCallBack.end();
							lua++ )
						{
							lua->first->CallLua<bool>( lua->second.c_str(), *(container->second) );
						}
					}
					break;
				}
			}
		}
		//if (m_isAction)
		//{
		//	this->m_Event.Invoker();
		//}
	}
	//-------------------------------------------------------------------------------------
	void UIList::SetActive(bool isActive)
	{
		UIBase::SetActive(isActive);
	}
	//-------------------------------------------------------------------------------------
	void UIList::AddResponse(IFunction<bool,IListItem&>* const func)
	{
		m_Event.Connect( func );
	}
	//-------------------------------------------------------------------------------------
	void UIList::AddResponse( ScriptBase* scrip, const char* func )
	{
		m_LuaCallBack.push_back( std::make_pair( scrip, func ) );
	}
	//-------------------------------------------------------------------------------------
	void UIList::Renew()
	{
		m_isUpdata = true;
		if( NULL != m_Father )
		{
			m_Father->Renew();
		}
	}
	//-------------------------------------------------------------------------------------
	void UIList::OnDraw(Engine& engine, const Color& clr, const Texture* tex)
	{
		UIBase::OnDraw(engine, clr, tex);

		//设置stencil
		engine.GetDevice()->SetStencilMask( 0xFFFFFFFF );
		engine.GetDevice()->EnableStancil();
		engine.GetDevice()->Clear( Device::STENCIL_BUFFER );
		engine.GetDevice()->ColorMask(false, false, false, false);
		engine.GetDevice()->StencilFunction(Device::FN_ALWAYS, 0, 0 );
		engine.GetDevice()->StencilOp(Device::ON_INCR,Device::ON_INCR,Device::ON_INCR);

		//在模板上绘制
		UIShader& ui = engine.GetShaderManage()->GetShader<UIShader>(ShaderManage::UIShader);
		ui.SetParamColor(Color::White);
		engine.GetDevice()->Draw( *m_pStencilRender, ui, NULL );
		//绘制Item
		engine.GetDevice()->ColorMask(true, true, true, true);
		engine.GetDevice()->StencilFunction(Device::FN_NOTEQUAL, 0, 0xFFFFFFFF );
		engine.GetDevice()->StencilOp(Device::ON_KEEP,Device::ON_KEEP,Device::ON_KEEP);
		ui.SetParamColor(clr);
		engine.GetDevice()->Draw( *m_pRender, ui, tex );
		for (ContainerVector::iterator it = m_ContainerVector.begin();
			it != m_ContainerVector.end();
			it ++ )
		{
			if( m_ItemList.end() != it->second )
			{
				(*(it->second))->Draw(engine);
			}
		}
		engine.GetDevice()->DisableStancil();
		engine.GetDevice()->Clear( Device::STENCIL_BUFFER );
	}
	//-------------------------------------------------------------------------------------
	void UIList::Update()
	{
		UIBase::Update();
		this->FlushItem();
	}
	//-------------------------------------------------------------------------------------
	void UIList::FlushItem()
	{
		m_pBuffer->CleanUp();
		for (ContainerVector::iterator it = m_ContainerVector.begin();
			it != m_ContainerVector.end();
			it ++ )
		{
			if( (it->first)->isShow() )
			{
				(it->first)->GetVertexBuffer( *m_pBuffer );
			}
		}
		m_pVertexBuffer->FlushVertexBuffer( m_pBuffer->CountOfAllVertex(), 
			static_cast<VertexTextureColor*>(m_pBuffer->VertexPtr()) );
		m_pIndicesBuffer->FlushIndiesBuffer( m_pBuffer->IndiesSize(), m_pBuffer->IndiesPtr() );
		m_pRender->SetDrawCount( m_pBuffer->IndiesSize() );
	}
	//-------------------------------------------------------------------------------------
	void UIList::AddItem(IListItem* item)
	{
		m_ItemList.push_back(item);
		//bool isSkipedFirst = false;
		for (int i = 0 ; i < m_ContainerCount ; i ++ )
		{
			//if( (m_CurrentIndex == m_ContainerCount - 1) 
			//	&& !isSkipedFirst )
			//{
			//	isSkipedFirst = true;
			//	continue;
			//}
			int index = i + m_CurrentIndex;
			if( index >= m_ContainerCount )
			{
				index -= m_ContainerCount;
			}
			if( m_ItemList.end() == m_ContainerVector[index].second )
			{
				item->OnVisable();
				item->SetPostion( m_ContainerVector[index].first->GetCenter() );
				m_ContainerVector[index].second = (++(m_ItemList.rbegin())).base();//找到刚才放入Item的It
				m_ContainerVector[index].first->SetShow(true);
				this->FlushItem();
				break;
			}
		}
	}
	//-------------------------------------------------------------------------------------
	void UIList::Clear()
	{
		for (ContainerVector::iterator it = m_ContainerVector.begin();
			it != m_ContainerVector.end();
			it ++ )
		{
			it->first->SetShow(false);
			it->second = m_ItemList.end();
		}
		for (ItemList::iterator it = m_ItemList.begin();
			it != m_ItemList.end();
			it ++ )
		{
			SAFE_DELETE( (*it) );
		}
		m_ItemList.clear();
	}
	//-------------------------------------------------------------------------------------
	void UIList::ShowLastItem()
	{
		if ( m_ItemList.size() <= m_ContainerCount - 1 )//小于可见数量也就可以退出了
		{
			return;
		}
		if( !isPushDonw() )//只有不被按下的时候
		{
			//判断是否已经显示
			for (ContainerVector::iterator it = m_ContainerVector.begin();
				it != m_ContainerVector.end();
				it ++ )
			{
				if( (++(m_ItemList.rbegin())).base() == it->second ) //如果已经显示了，直接返回
				{
					float listedge = this->GetCenter().m_y - this->GetSize().m_y / 2;//list的下边框
					float itemedge = (it->first)->GetCenter().m_y - (it->first)->GetSize().m_y / 2;//最后一个的下边框
					if( itemedge > listedge )
					{
						return;
					}
					break;
				}
			}

			//第一个设置为0
			m_CurrentIndex = 0;
			//删除字串
			for (ContainerVector::iterator it = m_ContainerVector.begin();
				it != m_ContainerVector.end();
				it ++ )
			{
				if( m_ItemList.end() != it->second )
				{
					(*it->second)->OnInvisable();
				}
			}
			//显示最后一个
			//从新分配全部的位置
			vector2f BeginPos = m_ContainerVector[0].first->GetCenter();
			BeginPos.m_y = this->GetCenter().m_y - this->GetSize().m_y / 2 + m_ContainerVector[0].first->GetSize().m_y / 2.0f;

			//设置最后一个
			BeginPos.m_y -= m_ContainerVector[0].first->GetSize().m_y;
			m_ContainerVector[m_ContainerCount-1].first->SetCenterForce( BeginPos );
			m_ContainerVector[m_ContainerCount-1].second = m_ItemList.end();
			BeginPos.m_y += m_ContainerVector[0].first->GetSize().m_y;

			//设置其余的
			ItemList::iterator Item = (++(m_ItemList.rbegin())).base();
			for( int i = m_ContainerCount - 2 ; i >= 0  ; i -- )
			{
				m_ContainerVector[i].first->SetCenterForce( BeginPos );
				m_ContainerVector[i].second = Item;
				if( m_ItemList.end() != m_ContainerVector[i].second )
				{
					(*m_ContainerVector[i].second)->OnVisable();
					(*m_ContainerVector[i].second)->SetPostion(BeginPos);
				}
				if( m_ItemList.begin() != Item )
				{
					Item--;
				}
				BeginPos.m_y += m_ContainerVector[i].first->GetSize().m_y;
			}
		}
	}








	/////////////////////////
	//		UIEdit		   //
	/////////////////////////
	//-------------------------------------------------------------------------------------
	UIEdit::UIEdit(IUIFather* father, const UIData& data)
		:UIBase(father, true, data),
		m_FontSize(0.01,0.01),
		m_FontClr( Color::Black ),
		m_MaxInput(0),
		m_isNumOnly(false)
	{
		//设置stencil绘制项目
		{
			float hsx = m_Size.m_x / 2.0f;
			float hsy = m_Size.m_y / 2.0f;
			Vertex stencil[4];
			stencil[0] = Vertex( vector4f(m_Center.m_x-hsx,m_Center.m_y+hsy,0) );
			stencil[1] = Vertex( vector4f(m_Center.m_x-hsx,m_Center.m_y-hsy,0) );
			stencil[2] = Vertex( vector4f(m_Center.m_x+hsx,m_Center.m_y+hsy,0) );
			stencil[3] = Vertex( vector4f(m_Center.m_x+hsx,m_Center.m_y-hsy,0) );
			m_pStencilRender = NEW RendBuffer( Device::RM_TRIANGLE_STRIP );
			m_pStencilVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
			m_pStencilIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
			m_pStencilRender->SetVertexBuffer( m_pStencilVertexBuffer );
			m_pStencilRender->SetIndicesBuffer( m_pStencilIndicesBuffer );
			m_pStencilVertexBuffer->FlushVertexBuffer( 4, stencil );
			static ushort index[]={0,1,2,3};
			m_pStencilIndicesBuffer->FlushIndiesBuffer( 4, index );
		}
	}
	//-------------------------------------------------------------------------------------
	UIEdit::~UIEdit()
	{
		m_OnReciveText.ClearConnect();
		SAFE_DELETE( m_pStencilRender );
		SAFE_DELETE( m_pStencilVertexBuffer );
		SAFE_DELETE( m_pStencilIndicesBuffer );
		HideKeyBorad();
	}
	//-------------------------------------------------------------------------------------
	void UIEdit::OnAction(const vector2f& pos)
	{
		Engine::Instance().GetUIManager()->OnActiveKeyborad( this, m_pBox->GetRect(), m_MaxInput, m_Content , m_isNumOnly);
	}
	//-------------------------------------------------------------------------------------
	bool UIEdit::OnReciveText(const std::wstring& str)
	{
		m_Content = str;
		this->SetString( m_FontSize, m_FontClr, true, m_Content );
		vector2f pos = m_Center;
		pos.m_x -= m_Size.m_x / 2.0;
		this->m_pString->SetPosition( pos + m_FontOffset , BString::Left );
		m_OnReciveText.Multicast( str );
		return true;
	}
	//-------------------------------------------------------------------------------------
	void UIEdit::OnDraw(Engine& engine, const Color& clr, const Texture* tex)
	{
		//设置stencil
		engine.GetDevice()->SetStencilMask( 0xFFFFFFFF );
		engine.GetDevice()->EnableStancil();
		engine.GetDevice()->Clear( Device::STENCIL_BUFFER );
		engine.GetDevice()->ColorMask(false, false, false, false);
		engine.GetDevice()->StencilFunction(Device::FN_ALWAYS, 0, 0 );
		engine.GetDevice()->StencilOp(Device::ON_INCR,Device::ON_INCR,Device::ON_INCR);

		//在模板上绘制
		UIShader& ui = engine.GetShaderManage()->GetShader<UIShader>(ShaderManage::UIShader);
		ui.SetParamColor(Color::White);
		engine.GetDevice()->Draw( *m_pStencilRender, ui, NULL );
		//绘制文字等
		engine.GetDevice()->ColorMask(true, true, true, true);
		engine.GetDevice()->StencilFunction(Device::FN_NOTEQUAL, 0, 0xFFFFFFFF );
		engine.GetDevice()->StencilOp(Device::ON_KEEP,Device::ON_KEEP,Device::ON_KEEP);
		UIBase::OnDraw(engine, clr, tex);

		engine.GetDevice()->DisableStancil();
		engine.GetDevice()->Clear( Device::STENCIL_BUFFER );
	}
	//-------------------------------------------------------------------------------------
	void UIEdit::AddResponse( IFunction<bool,const std::wstring&>* const func )
	{
		m_OnReciveText.Connect( func );
	}

}