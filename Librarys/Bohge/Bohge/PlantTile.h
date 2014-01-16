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
//		植被区块	   //
/////////////////////////
#pragma once
#include "PlantBaseInfo.h"
#include "aabbox.hpp"
#include <string>
#include <vector>
#include <map>


namespace BohgeEngine
{
	class PlantChunk;
	class PlantNode;
	class PlantTile
	{
	private:
		bool								m_isInView;//是否可见
		aabbox3df							m_BindBox;
		vector2d							m_Offset;
		PlantChunk*							m_QTNode;//植被树
	public:
		PlantTile(void);
		~PlantTile(void);
	public:
		void Update( uint milisecon, std::multimap< float, PlantChunk* >& plantmap );
		void Load( const std::string& path, const std::string& TexPath );
	public:
		BOHGE_FORCEINLINE aabbox3df& BindBox()
		{
			return m_BindBox;
		}
		BOHGE_FORCEINLINE bool& isInView()
		{
			return m_isInView;
		}
	};

}