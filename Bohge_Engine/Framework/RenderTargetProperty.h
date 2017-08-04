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
#pragma once
#include "3DMath.h"


#include <EASTL/map.h>

namespace BohgeEngine
{
	class TextureEntity;
	class RenderTargetProperty : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR( RenderTargetProperty )
	private:
		RenderTargetProperty(void){}
		~RenderTargetProperty(void){}
	public:
		enum SwapTarget
		{
			ST_SWAP_A = 0,
			ST_SWAP_B,
			ST_SWAP_C,
			ST_SWAP_D,
		};
		enum TargetType
		{
			RT_RENDER_TARGET_MAIN = 0, //主屏幕
			RT_RENDER_TARGET_2D = 1,
			RT_RENDER_TARGET_CUBE = 6,
		};
		enum TargetFace
		{
			TF_POSITIVE_X = 0,//x 正方向
			TF_NEGATIVE_X,
			TF_POSITIVE_Y,
			TF_NEGATIVE_Y,
			TF_POSITIVE_Z,
			TF_NEGATIVE_Z,
			TF_DEFAULT,//2d纹理的方向
		};
		enum RenderTargetAttachment
		{
			TA_COLOR_0 = 0,
			TA_COLOR_1,
			TA_COLOR_2,
			TA_COLOR_3,
			TA_COLOR_4,
			TA_COLOR_5,
			TA_COLOR_6,
			TA_COLOR_7,
			TA_COLOR_8,
			TA_COLOR_9,
			TA_DEPTH,
			TA_DEPTH_STENCIL,
			TA_COUNT,
		};
		enum ClearFlag
		{
			CF_COLOR				= 1 << 0,
			CF_DEPTH				= 1 << 1,
			CF_STENCIL				= 1 << 2,
			CF_COLOR_DEPTH			= CF_COLOR | CF_DEPTH,
			CF_COLOR_STENCIL		= CF_COLOR | CF_STENCIL,
			CF_DEPTH_STENCIL		= CF_DEPTH | CF_STENCIL,
			CF_COLOR_DEPTH_STENCIL	= CF_COLOR | CF_DEPTH | CF_STENCIL,
		};

		struct RenderTargetMetadata
		{
			Color									m_Color;
			vector4d								m_ViewPort;
			vector2d								m_Size;
			RenderTargetProperty::TargetType		m_eTargetType;
			RenderTargetProperty::SwapTarget		m_eSwapTarget;
		};
		typedef eastl::map< RenderTargetAttachment, TextureEntity* > AttachmentMap;
	};
}