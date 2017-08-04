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
#include "VertexBufferResourceGL.h"
#include "Framework/IGraphicSystem.h"

#include <GLEW/glew.h>


using namespace BohgeEngine;
namespace OpenGLGraphicSystemPlugin
{
	//---------------------------------------------------------------------------------------------------------
	VertexBufferResourceGL::VertexBufferResourceGL(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	VertexBufferResourceGL::~VertexBufferResourceGL(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResourceGL::_DoDeleteBuffer( handle h )
	{
		glDeleteBuffers( 1, &h);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResourceGL::_DoBindBuffer( handle h )
	{
		glBindBuffer(GL_ARRAY_BUFFER, h);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------------------
	handle VertexBufferResourceGL::_DoCreateBuffer( uint vertexcount, uint typesize, const void* dataptr, BufferObjectProperty::MemoryUseage use )
	{
		handle h;
		glGenBuffers(1, &h);
		CHECK_GRAPHIC_SYSTEM_ERROR;
		_BindBuffer( h );
		switch( use )
		{
		case BufferObjectProperty::MU_STATIC:glBufferData(GL_ARRAY_BUFFER, vertexcount*typesize, dataptr, GL_STATIC_DRAW);break;
		case BufferObjectProperty::MU_DYNAMIC:glBufferData(GL_ARRAY_BUFFER, vertexcount*typesize, dataptr, GL_DYNAMIC_DRAW);break;
		}
		CHECK_GRAPHIC_SYSTEM_ERROR;
		return h;
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResourceGL::_DoChangeData( handle h, uint offset, uint vertexcount, uint typesize, const void* dataptr )
	{
		_BindBuffer( h );
		glBufferSubData(GL_ARRAY_BUFFER, offset*typesize, vertexcount*typesize, dataptr);
		CHECK_GRAPHIC_SYSTEM_ERROR;
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResourceGL::_DoBindLayout( const Attributes& attrs )
	{
		const Attributes::AttributeList& attlist = attrs.GetAttributeList();
		const VertexStreamLayout& vsl = _GetVertexStreamLayout();
		GLenum dataType = 0;
		FOR_EACH_CONST( Attributes::AttributeList, att, attlist )
		{
			const VertexStreamLayout::Layout& lay = vsl.GetVertexLayout( att->AttributeName );
			switch( lay.DataType() )
			{
			case BufferObjectProperty::DT_FLOAT: dataType = GL_FLOAT; break;
			case BufferObjectProperty::DT_HALF_FLOAT: dataType = GL_HALF_FLOAT; break;
			case BufferObjectProperty::DT_INT_8_8_8_8: dataType = GL_BYTE; break;
			case BufferObjectProperty::DT_UINT_8_8_8_8: dataType = GL_UNSIGNED_BYTE; break;
			case BufferObjectProperty::DT_INT_2_10_10_10: dataType = GL_INT_2_10_10_10_REV; break;
			case BufferObjectProperty::DT_UINT_10_10_10_2: dataType = GL_UNSIGNED_INT_10_10_10_2; break;
			default:ASSERT(false);
			}
			glVertexAttribPointer(
				att->nLocation,
				lay.DataCount(),
				dataType,
				static_cast<GLboolean>(lay.isNormalize()),
				GetVertexSize(),
				(void*)lay.Offset() );
			CHECK_GRAPHIC_SYSTEM_ERROR;
			glEnableVertexAttribArray( att->nLocation );
			CHECK_GRAPHIC_SYSTEM_ERROR;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void VertexBufferResourceGL::_DoUnbindLayout( const Attributes& attrs )
	{
		const Attributes::AttributeList& attlist = attrs.GetAttributeList();
		FOR_EACH_CONST( Attributes::AttributeList, att, attlist )
		{
			glDisableVertexAttribArray( att->nLocation );
		}
	}

}