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



/////////////////////////////
//		2D包围盒		   //
/////////////////////////////

#include "aabrect.h"
#include<cfloat>

#ifdef DRAW_AABRECT
#include "Shapes.h"
#include "SecenManage.h"
#endif // CANDRAW

//-------------------------------------------------------------------------------------
aabrect::aabrect()
:m_Center( vector2f(0,0) ),
m_LeftBottom( vector2f(FLT_MAX,FLT_MAX) ),
m_RightTop( vector2f(-FLT_MAX,-FLT_MAX) )
{
#ifdef DRAW_AABRECT
	m_pRect = NEW Rect2D(m_LeftBottom, m_RightTop, Color(1,0,0,1));
#endif
}
//-------------------------------------------------------------------------------------
aabrect::aabrect(const vector2f& center, const vector2f& size)
:m_Center(center)
{
	vector2f hs= size / 2.0f;
	m_LeftBottom	= center - hs;
	m_RightTop		= center + hs;

#ifdef DRAW_AABRECT
	m_pRect = NEW Rect2D(m_LeftBottom, m_RightTop, Color(1,0,0,1));
#endif
}
//-------------------------------------------------------------------------------------
aabrect::aabrect(const aabrect& input)
{
	m_Center = input.m_Center;
	m_LeftBottom = input.m_LeftBottom;
	m_RightTop = input.m_RightTop;
#ifdef DRAW_AABRECT
	m_pRect = NEW Rect2D(*input.m_pRect);
#endif
}
//-------------------------------------------------------------------------------------
aabrect::~aabrect(void)
{
#ifdef DRAW_AABRECT
	SAFE_DELETE(m_pRect);
#endif
}
//-------------------------------------------------------------------------------------
void aabrect::SetCenter(const vector2f& center)
{
	vector2f dir = center - m_Center;
	m_Center = center;
	m_LeftBottom+=dir;
	m_RightTop+=dir;
}
//-------------------------------------------------------------------------------------
void aabrect::ContainsAabrect( const aabrect& aab )
{
	if( m_LeftBottom.m_x > aab.m_LeftBottom.m_x )
	{
		m_LeftBottom.m_x = aab.m_LeftBottom.m_x;
	}
	if( m_LeftBottom.m_y > aab.m_LeftBottom.m_y )
	{
		m_LeftBottom.m_y = aab.m_LeftBottom.m_y;
	}

	if( m_RightTop.m_x < aab.m_RightTop.m_x )
	{
		m_RightTop.m_x = aab.m_RightTop.m_x;
	}
	if( m_RightTop.m_y < aab.m_RightTop.m_y )
	{
		m_RightTop.m_y = aab.m_RightTop.m_y;
	}
#ifdef DRAW_AABRECT
	SAFE_DELETE( m_pRect );
	m_pRect = NEW Rect2D(m_LeftBottom, m_RightTop, Color(1,0,0,1));
#endif
}
//-------------------------------------------------------------------------------------
void aabrect::ContainsPoint( const vector2f& point )
{
	if( m_LeftBottom.m_x > point.m_x )
	{
		m_LeftBottom.m_x = point.m_x;
	}
	if( m_LeftBottom.m_y > point.m_y )
	{
		m_LeftBottom.m_y = point.m_y;
	}

	if( m_RightTop.m_x < point.m_x )
	{
		m_RightTop.m_x = point.m_x;
	}
	if( m_RightTop.m_y < point.m_y )
	{
		m_RightTop.m_y = point.m_y;
	}
	m_Center = ( m_LeftBottom + m_RightTop ) / 2;
#ifdef DRAW_AABRECT
	SAFE_DELETE( m_pRect );
	m_pRect = NEW Rect2D(m_LeftBottom, m_RightTop, Color(1,0,0,1));
#endif
}
//-------------------------------------------------------------------------------------
aabrect& aabrect::operator = ( const aabrect& r )
{
	m_Center = r.m_Center;
	m_LeftBottom = r.m_LeftBottom;
	m_RightTop = r.m_RightTop;
#ifdef DRAW_AABRECT
	m_pRect = NEW Rect2D(*r.m_pRect);
#endif
	return *this;
}
//-------------------------------------------------------------------------------------
#ifdef DRAW_AABRECT
void aabrect::Render(IDriver& driver) const
{
	m_pRect->Render(driver);
}
#endif
//-------------------------------------------------------------------------------------
bool aabrect::isInsert( const aabrect& rect ) const
{
	//你是否在我的范围内
	if ( isInsert(rect.m_LeftBottom) )
	{
		return true;
	}
	else if( isInsert(rect.m_RightTop) )
	{
		return true;
	}
	else if( isInsert( vector2f( rect.m_LeftBottom.m_x, rect.m_RightTop.m_y ) ) )
	{
		return true;
	}
	else if( isInsert( vector2f( rect.m_RightTop.m_x, rect.m_LeftBottom.m_y ) ) )
	{
		return true;
	}

	//我是否在你的范围内
	else if ( rect.isInsert(m_LeftBottom) )
	{
		return true;
	}
	else if( rect.isInsert(m_RightTop) )
	{
		return true;
	}
	else if( rect.isInsert( vector2f( m_LeftBottom.m_x, m_RightTop.m_y ) ) )
	{
		return true;
	}
	else if( rect.isInsert( vector2f( m_RightTop.m_x, m_LeftBottom.m_y ) ) )
	{
		return true;
	}
	return false;
}