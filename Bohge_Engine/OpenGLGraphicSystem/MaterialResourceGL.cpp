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
#include "Framework/IGraphicSystem.h"
#include "MaterialResourceGL.h"
#include <GLEW/glew.h>

using namespace BohgeEngine;
namespace OpenGLGraphicSystemPlugin
{
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetDepthTest( bool b )
	{
		b ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetDepthMask( bool b )
	{
		glDepthMask(b);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetAlphaBlend( bool b )
	{
		b ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetAlphaTest( bool b )
	{
		if ( b )
		{
			glEnable(GL_SAMPLE_COVERAGE);
			CHECK_GRAPHIC_SYSTEM_ERROR;
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			CHECK_GRAPHIC_SYSTEM_ERROR;
		}
		else
		{
			glDisable(GL_SAMPLE_COVERAGE);
			CHECK_GRAPHIC_SYSTEM_ERROR;
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			CHECK_GRAPHIC_SYSTEM_ERROR;
		}
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetDepthFunciton( MaterialProperty::Function func )
	{
		glDepthFunc( GL_NEVER + static_cast<uint>(func) );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetCullFace( MaterialProperty::CullFaceMode cull )
	{
		switch( cull )
		{
		case MaterialProperty::CFM_NONE: glDisable(GL_CULL_FACE);break;
		case MaterialProperty::CFM_BACK: glEnable(GL_CULL_FACE);glCullFace(GL_BACK); break;
		case MaterialProperty::CFM_FRONT:glEnable(GL_CULL_FACE);glCullFace(GL_FRONT); break;
		}
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------
	GLenum AlphaFunction(MaterialProperty::Blend input)
	{
		switch ( input )
		{
		case MaterialProperty::BL_ONE :					return GL_ONE;
		case MaterialProperty::BL_ZERO :				return GL_ZERO;
		case MaterialProperty::BL_SRC_COLOR :			return GL_SRC_COLOR;
		case MaterialProperty::BL_ONE_MINUS_SRC_COLOR :	return GL_ONE_MINUS_SRC_COLOR;
		case MaterialProperty::BL_SRC_ALPHA :			return GL_SRC_ALPHA;
		case MaterialProperty::BL_ONE_MINUS_SRC_ALPHA :	return GL_ONE_MINUS_SRC_ALPHA;
		case MaterialProperty::BL_DST_ALPHA :			return GL_DST_ALPHA;
		case MaterialProperty::BL_ONE_MINUS_DST_ALPHA :	return GL_ONE_MINUS_DST_ALPHA;
		case MaterialProperty::BL_DST_COLOR :			return GL_DST_COLOR;
		case MaterialProperty::BL_ONE_MINUS_DST_COLOR :	return GL_ONE_MINUS_DST_COLOR;
		}
		return GL_ONE;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetBlendFunction( MaterialProperty::Blend src, MaterialProperty::Blend des, MaterialProperty::Blend srcA, MaterialProperty::Blend desA )
	{
		glBlendFuncSeparate( AlphaFunction(src), AlphaFunction(des), AlphaFunction(srcA), AlphaFunction(desA)  );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetColorMask( bool r, bool g, bool b, bool a )
	{
		glColorMask( r, g, b, a );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetStencil( bool b )
	{
		b ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetStencilMask( bitmask mask )
	{
		glStencilMask( mask );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetStencilFunction( MaterialProperty::Function func, uint ref, bitmask mask )
	{
		glStencilFunc( GL_NEVER + static_cast<uint>(func), ref, mask );
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//------------------------------------------------------------------------------------------------------
	GLenum OperationMap(MaterialProperty::Operation op)
	{
		switch(op)
		{
		case MaterialProperty::ON_ZERO:return GL_ZERO;
		case MaterialProperty::ON_ONE:return GL_ONE;
		case MaterialProperty::ON_KEEP:return GL_KEEP;
		case MaterialProperty::ON_REPLACE:return GL_REPLACE;
		case MaterialProperty::ON_INCR:return GL_INCR;
		case MaterialProperty::ON_DECR:return GL_DECR;
		case MaterialProperty::ON_INVERT:return GL_INVERT;
		case MaterialProperty::ON_INCR_WRAP:return GL_INCR_WRAP;
		case MaterialProperty::ON_DECR_WRAP:return GL_DECR_WRAP;
		}
		return 0;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResourceGL::_DoSetStencilOperation( MaterialProperty::Operation fail, MaterialProperty::Operation zfail, MaterialProperty::Operation zpass )
	{
		glStencilOp(
			OperationMap(fail),
			OperationMap(zfail),
			OperationMap(zpass));
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}

}