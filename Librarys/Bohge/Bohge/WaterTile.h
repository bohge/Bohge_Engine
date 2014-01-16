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
//		水体		   //
/////////////////////////
#pragma once
#include "aabbox.hpp"
#include "aabrect.hpp"
#include "parameterhandler.h"
#include <string>


namespace BohgeEngine
{
	class Engine;
	class Material;
	class parameterhandler;
	class WaterTile
	{
	private:
		int					m_Index;//索引，用来判断是否需要重复拷贝复制给grid
		bool				m_isInView;//是否可见
		aabrectf			m_WorldRange;//范围
		aabbox3df			m_BindBox;
		Material*			m_WaterMaterial;//水体反射等材质
		parameterhandler*	m_WaterParameter;//水体的一些参数
	public:
		WaterTile(void);
		~WaterTile(void);
	public:
		void Update( uint milisecon );
		void Render( Engine& engine );
		void Load( const std::string& path, const std::string& TexPath );
	public:
		BOHGE_FORCEINLINE float GetHeight( float x, float y, float& outDepth ) const
		{
			if ( m_WorldRange.isInsert( x, y ) )
			{
				outDepth = m_WaterParameter->params[p_fWaterBaseHeight].fData;
				return true;
			}
			return false;
		}
		BOHGE_FORCEINLINE aabbox3df& BindBox()
		{
			return m_BindBox;
		}
		BOHGE_FORCEINLINE bool& isInView()
		{
			return m_isInView;
		}
		BOHGE_FORCEINLINE Material& GetMaterial()
		{
			return *m_WaterMaterial;
		}
		BOHGE_FORCEINLINE parameterhandler& GetWaterParameter()
		{
			return *m_WaterParameter;
		}
		BOHGE_FORCEINLINE int GetIndex()
		{
			return m_Index;
		}
	};
}