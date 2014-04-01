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
//			包装层		   //
/////////////////////////////
#include "Device.h"
#include "3DMath.h"
#include "IOSystem.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "Vertex.h"
#include "Log.h"
#include "ResLoader.h"
#include "RenderTarget.h"
#include "RendBuffer.h"
#include "Material.h"
#include "EngineResourceList.h"
#include "TextureData.h"
#include "IFile.h"
#include "EncryptFile.h"
#include "Utility.h"
#include "glsl/glsl_optimizer.h"



#include <set>
#include <string>
#include <algorithm>
#ifdef IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#ifdef OPENGL
#include <GLEW/glew.h>
#else
#include <GLES2/GL2.h>
#include <GLES2/gl2ext.h>
#endif
#endif


glslopt_ctx* g_pOptctx;//优化shader的库


using namespace std;

void chech_error_gles ()
{
	GLenum error = glGetError();
	switch(error)
	{
	case GL_NO_ERROR:break;
	case GL_INVALID_ENUM:DEBUGLOG("An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n",1);break;
	case GL_INVALID_VALUE:DEBUGLOG("A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n",1);break;
	case GL_INVALID_OPERATION:DEBUGLOG("The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n",1);break;
	case GL_OUT_OF_MEMORY:DEBUGLOG("There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n",1);break;
	}
}
#ifdef _DEBUG
#define CHECK_ERROR_GLES chech_error_gles()
#else
#define CHECK_ERROR_GLES
#endif
namespace BohgeEngine
{

#ifdef _DEBUG
	void Device::_DebugLog()
	{
		//DEBUGLOG("Alpha change %d, draw state change %d draw call %d and draw primitive %d\n",  m_AlphaChange, m_DrawStateChange, m_DrawCall, m_DrawPrimitive );
	}
#endif
	//------------------------------------------------------------------------------------------------------
	void Device::OnDeviceCreate()
	{
		DEBUGLOG("OpenGL version is %s", glGetString( GL_VERSION ) );
		m_LineWidth = 1.0;
		glEnable(GL_TEXTURE_2D);//默认打开2D纹理
		glDisable(GL_DITHER);
		if ( NULL != glDepthRangef )
		{
			glDepthRangef( 0.0, 1.0 );
		}

		glEnable(GL_DEPTH_TEST);
		this->SetState(DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		this->SetState(CULL_FACE_TEST);

		glDepthMask(true);

		glEnable(GL_BLEND);
		this->SetState(ALPHA_BLEND);

		glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );


#ifdef OPENGL
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable( GL_POINT_SPRITE );
		g_pOptctx = glslopt_initialize( kGlslTargetOpenGL );
#else
		g_pOptctx = glslopt_initialize( kGlslTargetOpenGLES20 );
#endif
	}
	//------------------------------------------------------------------------------------------------------
	void Device::OnDeviceDestroy()
	{
		glslopt_cleanup( g_pOptctx );
	}
	//------------------------------------------------------------------------------------------------------
	void Device::DeviceCaps()
	{
		GLint i1,i2,i3,i4,i5,i6,i7;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &i1);
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &i2);
		glGetIntegerv(GL_MAX_VARYING_VECTORS, &i3);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &i4);
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &i5);
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &i6);
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &i7);
		DEBUGLOG("GL_MAX_VERTEX_ATTRIBS %d\n",i1);
		DEBUGLOG("GL_MAX_VERTEX_UNIFORM_VECTORS %d\n",i2);
		DEBUGLOG("GL_MAX_VARYING_VECTORS %d\n",i3);
		DEBUGLOG("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS %d\n",i4);
		DEBUGLOG("GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS %d\n",i5);
		DEBUGLOG("GL_MAX_TEXTURE_IMAGE_UNITS %d\n",i6);
		DEBUGLOG("GL_MAX_FRAGMENT_UNIFORM_VECTORS %d\n",i7);
		//得到设备拓展
		string deviecCapStr= (char*)glGetString(GL_EXTENSIONS);
		do
		{
			int begin = deviecCapStr.find( ' ' );
			if ( -1 == begin )
			{
				break;
			}
			else
			{
				deviecCapStr.erase( begin, 1 );
				deviecCapStr.insert( begin, "\n" );
			}
		}while(true);
		DEBUGLOG( deviecCapStr.c_str(),1 );
		DEBUGLOG( "\n",1 );
		//查看是否支持各向异性过滤
#ifdef OPENGL
		m_isUsingAnisotropic = true;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_fMaxAnisotropic);
#else
		if ( -1 == deviecCapStr.find( "GL_EXT_texture_filter_anisotropic"	) )
		{
			m_isUsingAnisotropic = true;
			//get the value
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_fMaxAnisotropic);
		}
		else
		{
			m_isUsingAnisotropic = false;
		}
#endif
		if ( -1 == deviecCapStr.find("GL_OES_packed_depth_stencil")	)
		{
			m_isPacked_Depth_Stencil = true;
		}
		else
		{
			m_isPacked_Depth_Stencil = false;
		}
		if ( -1 == deviecCapStr.find("GL_OES_depth_texture" )
			&& -1 == deviecCapStr.find("GL_ARB_depth_texture" ) )
		{
			ASSERT(false);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::EnableDepthTest()
	{
		if( !this->isState(DEPTH_TEST) )
		{
#ifdef _DEBUG
			m_DrawStateChange ++;
#endif
			m_DrawState.m_isDepthTest = true;
			glEnable(GL_DEPTH_TEST);
			this->SetState(DEPTH_TEST);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::DisableDepthTest()
	{
		if( this->isState(DEPTH_TEST) )
		{
#ifdef _DEBUG
			m_DrawStateChange ++;
#endif
			m_DrawState.m_isDepthTest = false;
			glDisable(GL_DEPTH_TEST);
			this->EraseState(DEPTH_TEST);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::SetCullFace( const CullFaceMode& mod )
	{
		if( CFM_NONE != mod && !this->isState(CULL_FACE_TEST) )
		{
#ifdef _DEBUG
			m_DrawStateChange ++;
#endif
			glEnable(GL_CULL_FACE);
			SetState(CULL_FACE_TEST);
		}
		if ( m_DrawState.m_isCullFace != mod )
		{
			m_DrawState.m_isCullFace = mod;
			switch( mod )
			{
			case CFM_NONE: glDisable(GL_CULL_FACE); EraseState(CULL_FACE_TEST); break;
			case CFM_BACK: glCullFace(GL_BACK); break;
			case CFM_FRONT:glCullFace(GL_FRONT); break;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	GLenum AlphaFunction(Device::Blend input)
	{
		switch ( input )
		{
		case Device::BL_ONE :					return GL_ONE;
		case Device::BL_ZERO :					return GL_ZERO;
		case Device::BL_SRC_COLOR :			return GL_SRC_COLOR;
		case Device::BL_ONE_MINUS_SRC_COLOR :	return GL_ONE_MINUS_SRC_COLOR;
		case Device::BL_SRC_ALPHA :			return GL_SRC_ALPHA;
		case Device::BL_ONE_MINUS_SRC_ALPHA :	return GL_ONE_MINUS_SRC_ALPHA;
		case Device::BL_DST_ALPHA :			return GL_DST_ALPHA;
		case Device::BL_ONE_MINUS_DST_ALPHA :	return GL_ONE_MINUS_DST_ALPHA;
		case Device::BL_DST_COLOR :			return GL_DST_COLOR;
		case Device::BL_ONE_MINUS_DST_COLOR :	return GL_ONE_MINUS_DST_COLOR;
		}
		return GL_ONE;
	}
	void Device::EnableAlphaBlend(Blend src, Blend des, Blend srcA, Blend desA)
	{
		if( isState(ALPHA_TEST) )
		{
			this->DisableAlpha();
		}
		if( !isState(ALPHA_BLEND) )
		{
			glEnable(GL_BLEND);
			this->SetState(ALPHA_BLEND);
		}
		//glBlendFunc( AlphaFunction(src), AlphaFunction(des) );
		if( m_AlphaState.m_isAlphaEnable == false || m_AlphaState.m_Src != src || m_AlphaState.m_Des != des || m_AlphaState.m_SrcA != srcA || m_AlphaState.m_DesA != desA )
		{
#ifdef _DEBUG
			m_AlphaChange ++;
#endif
			m_AlphaState.m_isAlphaEnable = true;
			m_AlphaState.m_isAlphaTest = false;
			m_AlphaState.m_Src = src;
			m_AlphaState.m_Des = des;
			m_AlphaState.m_SrcA = srcA;
			m_AlphaState.m_DesA = desA;
			glBlendFuncSeparate( AlphaFunction(src), AlphaFunction(des), AlphaFunction(srcA), AlphaFunction(desA)  );
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::EnableAlphaCoverage( )
	{
		if( isState(ALPHA_BLEND) )
		{
			this->DisableAlpha();
		}
		if( !isState(ALPHA_TEST) )
		{
#ifdef _DEBUG
			m_AlphaChange ++;
#endif
			m_AlphaState.m_isAlphaEnable = true;
			m_AlphaState.m_isAlphaTest = true;
			glEnable(GL_SAMPLE_COVERAGE);
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			//glSampleCoverage( 1.0, GL_FALSE);
			this->SetState(ALPHA_TEST);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::DisableAlpha()
	{
		if( m_AlphaState.m_isAlphaEnable )
		{
#ifdef _DEBUG
			m_AlphaChange ++;
#endif
			m_AlphaState.m_isAlphaEnable = false;
			if( m_AlphaState.m_isAlphaTest )
			{
				glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE );
				this->EraseState(ALPHA_TEST);
			}
			else
			{
				glDisable(GL_BLEND);
				this->EraseState(ALPHA_BLEND);
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::EnableStancil()
	{
		glEnable( GL_STENCIL_TEST );
		this->SetState(STANCIL_TEST);
	}
	//------------------------------------------------------------------------------------------------------
	void Device::DisableStancil()
	{
		glDisable( GL_STENCIL_TEST );
		this->EraseState(STANCIL_TEST);
	}
	//------------------------------------------------------------------------------------------------------
	void Device::StencilFunction(Function func, uint ref, bitmask mask)
	{
		glStencilFunc( GL_NEVER + static_cast<uint>(func), ref, mask );
	}
	//------------------------------------------------------------------------------------------------------
	GLenum OperationMap(Device::Operation op)
	{
		switch(op)
		{
		case Device::ON_ZERO:return GL_ZERO;
		case Device::ON_ONE:return GL_ONE;
		case Device::ON_KEEP:return GL_KEEP;
		case Device::ON_REPLACE:return GL_REPLACE;
		case Device::ON_INCR:return GL_INCR;
		case Device::ON_DECR:return GL_DECR;
		case Device::ON_INVERT:return GL_INVERT;
		case Device::ON_INCR_WRAP:return GL_INCR_WRAP;
		case Device::ON_DECR_WRAP:return GL_DECR_WRAP;
		}
		return 0;
	}
	void Device::StencilOp(Operation fail, Operation zfail, Operation zpass)
	{
		glStencilOp(
			OperationMap(fail),
			OperationMap(zfail),
			OperationMap(zpass));
	}
	//------------------------------------------------------------------------------------------------------
	void Device::StencilOpSeparate(CullFaceMode face, Operation fail, Operation zfail, Operation zpass)
	{
		glStencilOpSeparate(
			GL_FRONT + static_cast<uint>(face),
			OperationMap(fail),
			OperationMap(zfail),
			OperationMap(zpass));
	}
	//------------------------------------------------------------------------------------------------------
	void Device::DepthFunction(Function func)
	{
		if ( m_DrawState.m_DepthFunction != func )
		{
#ifdef _DEBUG
			m_DrawStateChange ++;
#endif
			m_DrawState.m_DepthFunction = func;
			glDepthFunc( GL_NEVER + static_cast<uint>(func) );
		}	
	}
	//------------------------------------------------------------------------------------------------------
	float Device::LineWidth(float s)
	{
		glLineWidth( s );
		float old = m_LineWidth;
		m_LineWidth = s;
		return old;
	}
	//------------------------------------------------------------------------------------------------------
	void Device::SetViewport(const vector4d& vp)
	{
		if( m_Viewport != vp )
		{
			m_Viewport = vp;
			float r = static_cast<float>(vp.m_z) / (vp.m_w);
			if( r > 1.6 )
			{
				m_eViewType = Device::R_16_9;
			}
			else if( r < 1.4 )
			{
				m_eViewType = Device::R_4_3;
			}
			else
			{
				m_eViewType = Device::R_3_2;
			}		
			glViewport(vp.m_x,vp.m_y,vp.m_z,vp.m_w);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::ClearScreenColor(clamp r, clamp g, clamp b, clamp a)
	{
		if ( m_vCurrentClearColor != Color(r,g,b,a) )
		{
			m_vCurrentClearColor = Color(r,g,b,a);
			glClearColor(r,g,b,a);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::Clear(bitmask mask)
	{
//#ifdef _DEBUG
		bool fixed = false;
		if ( !m_DrawState.m_isDepthMask && 0 != (mask&DEPTH_BUFFER) )//当depthmask关闭的时候，清除深度会有问题
		{
			fixed = true;
			glDepthMask( GL_TRUE );
			//DEBUGLOG( "fixed depth bug! better not going here!\n" );
		}
//#endif
		GLbitfield glmask = 0;
		if( 0 != (mask&COLOR_BUFFER))
		{
			glmask |= GL_COLOR_BUFFER_BIT;
		}
		if( 0 != (mask&DEPTH_BUFFER))
		{
			glmask |= GL_DEPTH_BUFFER_BIT;
		}
		if( 0 != (mask&STENCIL_BUFFER))
		{
			glmask |= GL_STENCIL_BUFFER_BIT;
		}
		if( 0 != glmask )
			glClear( glmask );
//#ifdef _DEBUG
		if ( fixed )
		{
			glDepthMask( GL_FALSE );
		}
//#endif
	}
	//------------------------------------------------------------------------------------------------------
	void Device::SetStencilMask(bitmask mask)
	{
		glStencilMask( mask );
	}
	//------------------------------------------------------------------------------------------------------
	void Device::DeleteProgram(handle h)
	{
		glDeleteProgram( h );
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	string LoadShaderSource( const std::string& strFileName )
	{
		string source;
		IFile* readfile;
		readfile = FILEFACTORY( strFileName );
		if( false == readfile->OpenFile( IFile::AT_READ ) )
		{
			DEBUGLOG( "ERROR: Could not load shader file %s !\n", strFileName.c_str() );
			return false;
		}
		char* datas = NEW char[readfile->GetSize()+1];
		readfile->ReadFile( datas, readfile->GetSize() );
		datas[readfile->GetSize()] = 0;
		source = datas;
		readfile->CloseFile();
		FILEDESTROY( readfile );
		SAFE_DELETE_ARRAY(datas);
		return source;
	}
	size_t FindMainFunction( std::string& str )//找到主函数段
	{
		size_t start_pos = 0;
		do 
		{
			start_pos = str.find("main", start_pos);
			if( start_pos != std::string::npos )
			{
				size_t i = -1;
				bool istype = false;
				do 
				{
					istype = ! Utility::NotVariablePart( str[ start_pos + i-- ] );
				} while ( !istype );
				if ( 'd' == str[start_pos + ++i] )
				{
					start_pos = str.find("{", start_pos);
					return start_pos + 1;
				}
			}
			start_pos++;
		} while ( start_pos != std::string::npos );
		return std::string::npos;
	}
	string GetVariableName( const string& str, size_t begin, size_t dir )//得到变量全名
	{
		size_t spb = begin;
		size_t spe;
		while(true)
		{
			spb += dir;
			if ( !Utility::NotVariablePart( str[spb] ) )
			{
				spe = spb + dir;
				break;
			}
		}
		while(true)
		{
			spe += dir;
			if ( Utility::NotVariablePart( str[spe] ) )
			{
				break;
			}
		}
		return str.substr( (spb < spe ? spb : spe)+1, spb < spe ? spe - spb : spb - spe );
	}
	bool VariableModifier_Type( string& str, const string& vname, const char* modifier, string& type )//是否为某种修饰类型的变量
	{
		size_t start_pos = 0;
		do 
		{
			start_pos = str.find( vname, start_pos );
			if ( start_pos != std::string::npos )
			{
				size_t trunk = str.find_last_of( ';', start_pos );
				trunk = trunk != std::string::npos ? trunk : 0;
				string substr = str.substr( trunk, start_pos - trunk );
				size_t istype = substr.find( modifier );
				if ( istype != std::string::npos )
				{
					type = substr.substr( substr.find_first_of(' ') );
					return true;
				}
				start_pos += vname.size();
			}
		} while ( start_pos != std::string::npos );
		return false;
	}
	void FixAttributeIndexingType( string& str, size_t begin )//修复索引Attribute的问题
	{
		set<uint> handled;
		size_t start_pos = begin;
		do 
		{
			start_pos = str.find("[", start_pos);
			if ( start_pos != std::string::npos )
			{
				string vbname = GetVariableName( str, start_pos, -1 );
				if ( handled.end() == handled.find(  Utility::HashCode( vbname ) ) )
				{
					handled.insert( Utility::HashCode( vbname ) );
					string type;
					if ( VariableModifier_Type(str, vbname, "attribute", type ) )
					{
						string fixtoname = vbname + "_fixed_bohge";
						handled.insert( Utility::HashCode( fixtoname ) );
						string eqution = type + fixtoname + " = " + vbname + ";\r\n";
						Utility::ReplaceAllParamterName( str, vbname, fixtoname, start_pos-vbname.size() );
						str.insert( begin, eqution );
					}
				}
				start_pos += 1;
			}
		} while ( start_pos != std::string::npos );
	}
	bool ComplieShaderCode( const std::string& code, handle hShaderHandle, bool isvs )
	{
		string source = code;
		{//处理自定义的include
			while ( source.find("#include") != -1 )
			{
				int begin = source.find("#include");
				int first = source.find_first_of('\"', begin );
				int second = source.find_first_of('\"', first+1 );
				string includefile = source.substr( first+1, second - first - 1 );
				string includetext;
				if ( IOINSTANCE.isExist( IOINSTANCE.ShaderFolder() + includefile ) )
				{
					includetext = LoadShaderSource( IOINSTANCE.ShaderFolder() + includefile );
				}
				else
				{
					includetext = LoadShaderSource( IOINSTANCE.AddtionFolder() + includefile );
				}
				source.erase( begin, second - begin + 1 );
				source.insert( begin, includetext );
			}
		}
#ifdef OPENGL //如果是opengl，需要删除精度语句
		Utility::RemoveTargetString( source, "precision highp float;" );
		Utility::RemoveTargetString( source, "precision mediump float;" );
		Utility::RemoveTargetString( source, "precision lowp float;" );
		Utility::RemoveTargetString( source, "highp " );
		Utility::RemoveTargetString( source, "mediump " );
		Utility::RemoveTargetString( source, "lowp " );
		source.insert(0, "#define	MAXJOINTS	128\n" );
		source.insert(0, "#define	MAXARRAYSIZE	128\n" );
		source.insert(0, "#define	_WINDOWS_\n" );
#else
		source.insert(0, "#define	MAXJOINTS	100\n" );
		source.insert(0, "#define	MAXARRAYSIZE	100\n" );
#endif
		glslopt_shader* shader = glslopt_optimize( g_pOptctx, isvs ? kGlslOptShaderVertex : kGlslOptShaderFragment, source.c_str(), 0 );
		if ( glslopt_get_status(shader) )
		{
			source = glslopt_get_output( shader );
		}
		else
		{
			const char* log = glslopt_get_log( shader );
			DEBUGLOG("Can't optimizer shader, caz %s\n", log );
		}
		//处理[]的问题,只有vs有这样的问题
		if ( isvs )
		{
			size_t mainbegin = FindMainFunction(source);
			FixAttributeIndexingType( source, mainbegin );
		}
		const char* ptr = source.c_str();
		glShaderSource( hShaderHandle, 1, &(ptr), NULL );
		glCompileShader( hShaderHandle );
		//Check for compile success
		GLint nCompileResult = 0;
		glGetShaderiv( hShaderHandle, GL_COMPILE_STATUS, &nCompileResult );
		glslopt_shader_delete( shader );
		if( 0 == nCompileResult )
		{
			char strInfoLog[1024];
			sint nLength;
			glGetShaderInfoLog( hShaderHandle, 1024, &nLength, strInfoLog );
			DEBUGLOG( "Error %s\n", strInfoLog );
			return false;
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Device::CreateShaderProgram(handle& hProgram, Attributes& attr, int shaderindex )
	{
		string vscode = LoadShaderSource( ShaderResource::Instance().GetShaderVS(shaderindex) );
		string pscode = LoadShaderSource( ShaderResource::Instance().GetShaderPS(shaderindex) );
		return CreateShaderProgram( hProgram, attr, vscode, pscode );
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Device::CreateShaderProgram( handle& hProgram, Attributes& attr, const string& vscode, const string& pscode )
	{		
		//创建shader句柄
		handle hVertexShader = glCreateShader( GL_VERTEX_SHADER );
		handle hFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		//读取文件
		if( !ComplieShaderCode( vscode, hVertexShader, true ) )
		{
			glDeleteShader( hVertexShader );
			glDeleteShader( hFragmentShader );
			ASSERT(false);
			return false;
		}
		if( !ComplieShaderCode( pscode, hFragmentShader, false ) )
		{
			glDeleteShader( hVertexShader );
			glDeleteShader( hFragmentShader );
			ASSERT(false);
			return false;
		}

		//绑定shader
		handle hShaderProgram = glCreateProgram();
		glAttachShader( hShaderProgram, hVertexShader );
		glAttachShader( hShaderProgram, hFragmentShader );

		//绑定shader的Attributes
		if( 0 != attr.Size() )
		{
			for( uint i=0; i<attr.Size(); i++ )
			{
				if ( -1 != attr.GetLocation(i) )
				{
					glBindAttribLocation(	hShaderProgram,	attr.GetLocation(i),
						attr.StringAttribute(i).c_str() );
				}
			}
		}
		//连接shader
		glLinkProgram( hShaderProgram );
		//Check for link success
		GLint LinkStatus;
		glGetProgramiv( hShaderProgram, GL_LINK_STATUS, &LinkStatus );
		if( 0 == LinkStatus )
		{
			char  strInfoLog[1024];
			int nLength;
			glGetProgramInfoLog( hShaderProgram, 1024, &nLength, strInfoLog );
			DEBUGLOG( "Error %s\n", strInfoLog );
			glDeleteProgram( hShaderProgram );
			ASSERT(false);
			return false;
		}

		//释放
		glDeleteShader( hVertexShader );
		glDeleteShader( hFragmentShader );
		//赋值
		hProgram = hShaderProgram;
		return true;
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	handle Device::GetShaderParameterHandle(handle hPragma, const char* name)
	{
		handle h = glGetUniformLocation( hPragma,  name );
		if( -1 == h )
		{
			GLenum e = glGetError();
			DEBUGLOG("Error %d\n", e);
		}
		return h;
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::BindShader(handle h)
	{
		if( h != m_hCurrentShader )
		{
			m_hCurrentShader = h;
			glUseProgram( h );
			CHECK_ERROR_GLES;
		}
		//else
		//{
		//	DEBUGLOG("Save Time!!\n");
		//}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::CleanTextrueReference()
	{
		//清除引用计数
		m_uTextrueReferenceCount = 0;
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::SetShaderParameter(handle h, const std::vector<Quaternionf>& input)
	{
		if ( -1 != h)
		{
			glUniform4fv(h, input.size(), &(input[0].data[0]));
			CHECK_ERROR_GLES;
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::SetShaderParameter(handle h, const int input)
	{
		if ( -1 != h)
		{
			glUniform1i(h, input);
			CHECK_ERROR_GLES;
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::SetShaderParameter( handle h, const std::vector<float>& input )
	{
		if ( -1 != h)
		{
			glUniform1fv(h, input.size(), &input[0]);
			CHECK_ERROR_GLES;
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::SetShaderParameter(handle h, const float input)
	{
		if ( -1 != h)
		{
			glUniform1f(h, input);
			CHECK_ERROR_GLES;
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::SetShaderParameter(handle h, const vector2f& input)
	{
		if ( -1 != h)
		{
			glUniform2f(h, input.m_x, input.m_y);
			CHECK_ERROR_GLES;
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::SetShaderParameter(handle h, const vector3f& input)
	{
		if ( -1 != h)
		{
			glUniform3f(h, input.m_x, input.m_y, input.m_z);
			CHECK_ERROR_GLES;
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::SetShaderParameter(handle h, const vector4f& input)
	{
		if ( -1 != h)
		{
			glUniform4f(h, input.m_x, input.m_y, input.m_z, input.m_w);
			CHECK_ERROR_GLES;
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::SetShaderParameter(handle h, const Matrix33f& input)
	{
		if ( -1 != h)
		{
			glUniformMatrix3fv(h, 1, false, input.GetMatrixArray() );
			CHECK_ERROR_GLES;
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::SetShaderParameter(handle h, const Matrix44f& input)
	{
		if ( -1 != h)
		{
			glUniformMatrix4fv(h, 1, false, input.GetMatrixArray() );
			CHECK_ERROR_GLES;
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::SetShaderParameter(handle h, const Texture& input)
	{
		if ( -1 != h)
		{
			ASSERT( m_uTextrueReferenceCount < 32 );
			//glActiveTexture( GL_TEXTURE0 + m_uTextrueReferenceCount );
			this->SetTexture(input);
			glUniform1i( h, m_uTextrueReferenceCount );
			CHECK_ERROR_GLES;
			m_uTextrueReferenceCount ++;
		}
	}









	//////////////////////////
	//			绘制		//
	//////////////////////////
	void Device::UsingVertexAttribPointer(handle h, uint size, bool normalize, uint stribie, const FloatVector* vector)
	{
		glVertexAttribPointer( h,
			size,
			GL_FLOAT,//暂时只允许float类型的数据点
			static_cast<GLboolean>(normalize),
			stribie,
			vector == NULL ? NULL : (const GLvoid*)( &(*vector)[0] ) );
		CHECK_ERROR_GLES;
		glEnableVertexAttribArray( h );
		CHECK_ERROR_GLES;
	}
	//------------------------------------------------------------------------------------------------------
	void Device::DropVertexAttribPointer(handle h)
	{
		glDisableVertexAttribArray( h );
		CHECK_ERROR_GLES;
	}
	//------------------------------------------------------------------------------------------------------
	//void Device::Draw(VertexBuffer& buff, Shader& shader, Texture* tex)
	//{
	//	if( false == buff.isEmpty() )
	//	{
	//		shader.UsingShader();
	//		if( NULL != tex )
	//		{
	//			this->SetTexture(*tex);
	//		}
	//		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//		buff.UsingBuffer();
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff.IndiesBuffer() );
	//		glDrawElements(GL_POINTS + static_cast<uint>(buff.GetType()),
	//			buff.GetIndiseCount(),GL_UNSIGNED_INT, 
	//			NULL);
	//			//buff.GetIndies());
	//		buff.DropBuffer();
	//	}
	//}
	//------------------------------------------------------------------------------------------------------
	void Device::Draw(const RendBuffer& buff, Shader& shader, const Texture* tex)
	{
		if( 0 != buff.DrawCount() )
		{
#ifdef _DEBUG
			m_DrawCall ++;
#endif
			shader.UsingShader();
			if( NULL != tex )
			{
				this->SetTexture(*tex);
			}
			const VertexLayoutList* list = buff.GetVertexLayoutList();
			uint ptr = 
				NULL;	//这个是用VBO的方法
			//buff.VertexPtr();
			glBindBuffer(GL_ARRAY_BUFFER, buff.VertexHanle());
			for (VertexLayoutList::const_iterator it = list->begin();
				it !=list->end();
				it ++
				)
			{
				GLint Attrib = static_cast<GLint>(shader.GetAttributesMap().GetLocation( (*it)->GetAttributes() ) );
				if( -1 != Attrib )
				{
					glVertexAttribPointer( Attrib,
						(*it)->Size(),
						GL_BYTE + static_cast<GLint>((*it)->DataType()),
						static_cast<GLboolean>((*it)->IsNormalize()),
						buff.VertexSize(),
						(void*)(ptr) );
					CHECK_ERROR_GLES;
					glEnableVertexAttribArray( Attrib );
					CHECK_ERROR_GLES;
				}
				ptr += (*it)->Stride();
			}
			//if( buff.hasInstance() )
			//{
			//	glBindBuffer(GL_ARRAY_BUFFER, buff.GeometryHanlde() );		
			//	GLint Attrib = static_cast<GLint>(shader.GetAttributesMap().GetLocation( Attributes::ATTRIBUTE_MATIRX_COLUMN1 ) );
			//	if( -1 != Attrib )
			//	{
			//		glEnableVertexAttribArray(Attrib + 0);
			//		glEnableVertexAttribArray(Attrib + 1);
			//		glEnableVertexAttribArray(Attrib + 2);
			//		glEnableVertexAttribArray(Attrib + 3);
			//		glVertexAttribPointer(Attrib + 0, 4, GL_FLOAT, 0, sizeof(Matrix44f), (GLvoid*)0);
			//		glVertexAttribPointer(Attrib + 1, 4, GL_FLOAT, 0, sizeof(Matrix44f), (GLvoid*)16);
			//		glVertexAttribPointer(Attrib + 2, 4, GL_FLOAT, 0, sizeof(Matrix44f), (GLvoid*)32);
			//		glVertexAttribPointer(Attrib + 3, 4, GL_FLOAT, 0, sizeof(Matrix44f), (GLvoid*)48);
			//	}
			//	else
			//	{
			//		//ASSERT(false);//shader不支持instance
			//	}
			//}
#ifdef _DEBUG
			switch( buff.RenderMode() )
			{
			case Device::RM_TRIANGLES: m_DrawPrimitive += buff.DrawCount() / 3;break;
			case Device::RM_TRIANGLE_STRIP: m_DrawPrimitive += buff.DrawCount() - 2;break;
			case Device::RM_TRIANGLE_FAN: m_DrawPrimitive += buff.DrawCount() - 2;break;
			case Device::RM_LINES: m_DrawPrimitive += buff.DrawCount() / 2;break;
			case Device::RM_LINE_LOOP: m_DrawPrimitive += buff.DrawCount() - 1;break;
			case Device::RM_LINE_STRIP: m_DrawPrimitive += buff.DrawCount() - 1;break;
			case Device::RM_POINTS: m_DrawPrimitive += buff.DrawCount();break;
			}
#endif
			if( buff.hasIndies() ) //如果有索引，用
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff.IndexHandle());
				glDrawElements(GL_POINTS + static_cast<uint>(buff.RenderMode()),
					buff.DrawCount(), GL_UNSIGNED_SHORT, //标准的gles2只能用short或者byte
					NULL); //这个是用VBO的方法
				//buff.GetIndies());
				CHECK_ERROR_GLES;
			}
			else
			{
				glDrawArrays(GL_POINTS + static_cast<uint>(buff.RenderMode()), 0, buff.DrawCount() );
				CHECK_ERROR_GLES;
			}
			for (VertexLayoutList::const_iterator it = list->begin();
				it !=list->end();
				it ++
				)
			{
				GLint Attrib = static_cast<GLint>(shader.GetAttributesMap().GetLocation( (*it)->GetAttributes() ) );
				if( -1 != Attrib )
				{
					glDisableVertexAttribArray( Attrib );
					CHECK_ERROR_GLES;
				}
			}
			//if( buff.hasInstance() )
			//{
			//	GLint Attrib = static_cast<GLint>(shader.GetAttributesMap().GetLocation( Attributes::ATTRIBUTE_MATIRX_COLUMN1 ) );
			//	if( -1 != Attrib )
			//	{
			//		glDisableVertexAttribArray(Attrib + 0);
			//		glDisableVertexAttribArray(Attrib + 1);
			//		glDisableVertexAttribArray(Attrib + 2);
			//		glDisableVertexAttribArray(Attrib + 3);
			//	}
			//}
		}
		//else
		//{
		//	DEBUGLOG("Nothing to draw \n");
		//}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::Draw(const RendBuffer& buff, const Material& material)
	{
		if( 0 != buff.DrawCount() )
		{
#ifdef _DEBUG
			m_DrawCall ++;
#endif
			material.Using( Engine::Instance().GetPipeline()->CurrentPassType() );
			const VertexLayoutList* list = buff.GetVertexLayoutList();
			uint ptr = 
				NULL;	//这个是用VBO的方法
			//buff.VertexPtr();
			glBindBuffer(GL_ARRAY_BUFFER, buff.VertexHanle());
			for (VertexLayoutList::const_iterator it = list->begin();
				it !=list->end();
				it ++
				)
			{
				GLint Attrib = static_cast<GLint>(material.GetShader( Engine::Instance().GetPipeline()->CurrentPassType() ).GetAttributesMap().GetLocation( (*it)->GetAttributes() ) );
				if( -1 != Attrib )
				{
					glVertexAttribPointer( Attrib,
						(*it)->Size(),
						GL_BYTE + static_cast<GLint>((*it)->DataType()),
						static_cast<GLboolean>((*it)->IsNormalize()),
						buff.VertexSize(),
						(void*)(ptr) );
					CHECK_ERROR_GLES;
					glEnableVertexAttribArray( Attrib );
					CHECK_ERROR_GLES;
				}
				ptr += (*it)->Stride();
			}
			//if( buff.hasInstance() )
			//{
			//	glBindBuffer(GL_ARRAY_BUFFER, buff.GeometryHanlde() );		
			//	GLint Attrib = static_cast<GLint>(material.GetShader(Engine::Instance().GetPipeline()->CurrentPassType()).GetAttributesMap().GetLocation( Attributes::ATTRIBUTE_MATIRX_COLUMN1 ) );
			//	glEnableVertexAttribArray(Attrib + 0);
			//	glEnableVertexAttribArray(Attrib + 1);
			//	glEnableVertexAttribArray(Attrib + 2);
			//	glEnableVertexAttribArray(Attrib + 3);
			//	glVertexAttribPointer(Attrib + 0, 4, GL_FLOAT, 0, sizeof(Matrix44f), (GLvoid*)0);
			//	glVertexAttribPointer(Attrib + 1, 4, GL_FLOAT, 0, sizeof(Matrix44f), (GLvoid*)16);
			//	glVertexAttribPointer(Attrib + 2, 4, GL_FLOAT, 0, sizeof(Matrix44f), (GLvoid*)32);
			//	glVertexAttribPointer(Attrib + 3, 4, GL_FLOAT, 0, sizeof(Matrix44f), (GLvoid*)48);

			//}
#ifdef _DEBUG
			switch( buff.RenderMode() )
			{
			case Device::RM_TRIANGLES: m_DrawPrimitive += buff.DrawCount() / 3;break;
			case Device::RM_TRIANGLE_STRIP: m_DrawPrimitive += buff.DrawCount() - 2;break;
			case Device::RM_TRIANGLE_FAN: m_DrawPrimitive += buff.DrawCount() - 2;break;
			case Device::RM_LINES: m_DrawPrimitive += buff.DrawCount() / 2;break;
			case Device::RM_LINE_LOOP: m_DrawPrimitive += buff.DrawCount() - 1;break;
			case Device::RM_LINE_STRIP: m_DrawPrimitive += buff.DrawCount() - 1;break;
			case Device::RM_POINTS: m_DrawPrimitive += buff.DrawCount();break;
			}
#endif
			if( buff.hasIndies() ) //如果有索引，用
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff.IndexHandle());
				glDrawElements(GL_POINTS + static_cast<uint>(buff.RenderMode()),
					buff.DrawCount(), GL_UNSIGNED_SHORT, //标准的gles2只能用short或者byte
					NULL); //这个是用VBO的方法
				//buff.GetIndies());
				CHECK_ERROR_GLES;
			}
			else
			{
				glDrawArrays(GL_POINTS + static_cast<uint>(buff.RenderMode()), 0, buff.DrawCount() );
				CHECK_ERROR_GLES;
			}
			for (VertexLayoutList::const_iterator it = list->begin();
				it !=list->end();
				it ++
				)
			{
				GLint Attrib = static_cast<GLint>(material.GetShader(Engine::Instance().GetPipeline()->CurrentPassType()).GetAttributesMap().GetLocation( (*it)->GetAttributes() ) );
				if( -1 != Attrib )
				{
					glDisableVertexAttribArray( Attrib );
					CHECK_ERROR_GLES;
				}
			}
			//if( buff.hasInstance() )
			//{
			//	GLint Attrib = static_cast<GLint>(material.GetShader(Engine::Instance().GetPipeline()->CurrentPassType()).GetAttributesMap().GetLocation( Attributes::ATTRIBUTE_MATIRX_COLUMN1 ) );
			//	glDisableVertexAttribArray(Attrib + 0);
			//	glDisableVertexAttribArray(Attrib + 1);
			//	glDisableVertexAttribArray(Attrib + 2);
			//	glDisableVertexAttribArray(Attrib + 3);
			//}
		}
		//else
		//{
		//	DEBUGLOG("Nothing to draw \n");
		//}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::Finish()
	{
		glFinish();
	}
	//------------------------------------------------------------------------------------------------------
	void Device::DepthMask(bool d)
	{
		if ( m_DrawState.m_isDepthMask != d)
		{
#ifdef _DEBUG
			m_DrawStateChange ++;
#endif
			m_DrawState.m_isDepthMask = d;
			glDepthMask( d );
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::ColorMask(bool r, bool g, bool b, bool a )
	{
		glColorMask( r, g, b, a );
	}

	//------------------------------------------------------------------------------------------------------
	void Device::DeleteTexture(handle& h)
	{
		glDeleteTextures(1, &h);
	}
	//------------------------------------------------------------------------------------------------------
	void Device::SetTexture(const Texture& tex)
	{
		if( tex.GetTextureHandle() != m_hCurrentTexture )
		{
			m_hCurrentTexture = tex.GetTextureHandle();
			glActiveTexture( GL_TEXTURE0 + m_uTextrueReferenceCount );
			switch( tex.TextureType() )
			{
			case TT_TEXTURE1D: ASSERT( false ); break;
			case TT_TEXTURE2D: glBindTexture( GL_TEXTURE_2D, tex.GetTextureHandle() ); break;
			case TT_TEXTURE3D: ASSERT( false ); break;
			case TT_TEXTURECUBE: glBindTexture( GL_TEXTURE_CUBE_MAP, tex.GetTextureHandle() ); break;
			}
			CHECK_ERROR_GLES;
		}
		//else
		//{
		//	DEBUGLOG("Save Texture Time %s!!\n", tex.GetName().c_str());
		//}
	}
	//------------------------------------------------------------------------------------------------------
	GLuint GetTextureType( Device::TextureType type )
	{
		switch( type )
		{
		case Device::TT_TEXTURE1D: ASSERT( false ); break;
		case Device::TT_TEXTURE2D: return GL_TEXTURE_2D; break;
		case Device::TT_TEXTURE3D: ASSERT( false ); break;
		case Device::TT_TEXTURECUBE: return GL_TEXTURE_CUBE_MAP; break;
		}
		ASSERT( false );
		return GL_TEXTURE_2D;
	}
	//------------------------------------------------------------------------------------------------------
	int GLWrap(Device::TextureWarp w)
	{
		switch( w )
		{
		case Device::TW_CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		case Device::TW_MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
		case Device::TW_REPEAT: return GL_REPEAT;
		default: ASSERT(false); break;
		}
		return 0;
	}
	void Device::SetTextureWarp(const handle& h, TextureType type, TextureWarp s, TextureWarp t)
	{
		int TexType = GetTextureType( type );
		if( m_hCurrentTexture != h )
		{
			glBindTexture( TexType, h );
			m_hCurrentTexture = h;
		}
		glTexParameteri( TexType, GL_TEXTURE_WRAP_S, GLWrap(s) );
		CHECK_ERROR_GLES;
		glTexParameteri( TexType, GL_TEXTURE_WRAP_T,  GLWrap(t) );
		CHECK_ERROR_GLES;

	}
	//------------------------------------------------------------------------------------------------------
	int GLFilter(Device::TextureFilter f)
	{
		switch( f )
		{
		case Device::TF_NEAREST: return GL_NEAREST;
		case Device::TF_LINEAR: return GL_LINEAR;
		case Device::TF_NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
		case Device::TF_LINEAR_MIPMAP_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
		case Device::TF_NEAREST_MIPMAP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Device::TF_LINEAR_MIPMAP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		default: ASSERT(false); break;
		}
		return 0;
	}
	void Device::SetTextureFilter(const handle& h, TextureType type, TextureFilter mag, TextureFilter min)
	{
		int TexType = GetTextureType( type );
		if( m_hCurrentTexture != h )
		{
			glBindTexture( TexType, h );
			m_hCurrentTexture = h;
		}
		glTexParameteri(TexType, GL_TEXTURE_MAG_FILTER, GLFilter(mag) );
		CHECK_ERROR_GLES;
		glTexParameteri(TexType, GL_TEXTURE_MIN_FILTER, GLFilter(min) ); 
		CHECK_ERROR_GLES;
	}
	//------------------------------------------------------------------------------------------------------
	uint Device::SetTextureAnisotropic( const handle& h, TextureType type, uint x )
	{
		if( true == m_isUsingAnisotropic && 0 != x )	//如果支持各向异性
		{
			int TexType = GetTextureType( type );
			if( m_hCurrentTexture != h )
			{
				glBindTexture( TexType, h );
				m_hCurrentTexture = h;
			}
			uint to = x <= m_fMaxAnisotropic ? x : m_fMaxAnisotropic;
			glTexParameterf(TexType, GL_TEXTURE_MAX_ANISOTROPY_EXT, to );
			CHECK_ERROR_GLES;
			return to;
		}
		return 0;
	}
	//------------------------------------------------------------------------------------------------------
	void Device::GenerateMipmap( const handle& h, TextureType type )
	{
		int TexType = GetTextureType( type );
		if( m_hCurrentTexture != h )
		{
			glBindTexture( TexType, h );
			m_hCurrentTexture = h;
		}
		glBindTexture( TexType, h );
		glGenerateMipmap(TexType);//生成mipmap
		CHECK_ERROR_GLES;
	}
	//------------------------------------------------------------------------------------------------------
	//输入的pixels每一个色彩数据点必须是8bit的，应为在此要对应不同的格式转换数据
	Device::PixelFormat glTexImage2DHelper(GLenum target, GLint level, int pixelsize, Device::PixelFormat pf, GLsizei width, GLsizei height, GLint border, const byte* pixels)
	{
		switch( pf )
		{
		case Device::PF_A8 :
			{
				switch( pixelsize )
				{
				case 1:
					{
						glTexImage2D( target, level, GL_ALPHA, width, height, border, GL_ALPHA, GL_UNSIGNED_BYTE, pixels );
						CHECK_ERROR_GLES;
						return Device::PF_A8;
					}
				case 3:
					{
						byte* newData = NEW byte[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								newData[j + i * width] = pixels[ ( j + i * width ) * pixelsize + 0 ] * 0.3
									+ pixels[ ( j + i * width ) * pixelsize + 1 ] * 0.59
									+ pixels[ ( j + i * width ) * pixelsize + 2 ] * 0.11;//转换到灰度
							}
						}
						glTexImage2D( target, level, GL_ALPHA, width, height, border, GL_ALPHA, GL_UNSIGNED_BYTE, newData );
						CHECK_ERROR_GLES;
						SAFE_DELETE_ARRAY(newData);
						return Device::PF_A8;
					}
				case 4:
					{
						byte* newData = NEW byte[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{

								newData[j + i * width] = pixels[ ( j + i * width ) * pixelsize + 3 ]; //只要alpha通道
							}
						}
						glTexImage2D( target, level, GL_ALPHA, width, height, border, GL_ALPHA, GL_UNSIGNED_BYTE, newData );
						CHECK_ERROR_GLES;
						SAFE_DELETE_ARRAY(newData);
						return Device::PF_A8;
					}
				}
			}
		case Device::PF_L8 :
			{
				switch( pixelsize )
				{
				case 1:
					{
						glTexImage2D( target, level, GL_LUMINANCE, width, height, border, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels );
						CHECK_ERROR_GLES;
						return Device::PF_L8;
					}
				default:
					{
						byte* newData = NEW byte[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								newData[j + i * width] = pixels[ ( j + i * width ) * pixelsize + 0 ] * 0.3
									+ pixels[ ( j + i * width ) * pixelsize + 1 ] * 0.59
									+ pixels[ ( j + i * width ) * pixelsize + 2 ] * 0.11;//转换到灰度
							}
						}
						glTexImage2D( target, level, GL_LUMINANCE, width, height, border, GL_LUMINANCE, GL_UNSIGNED_BYTE, newData );
						CHECK_ERROR_GLES;
						SAFE_DELETE_ARRAY(newData);
						return Device::PF_L8;
					}
				}
			}
			//case Device::PF_LA8 :
			//	{
			//		switch( pixelsize )
			//		{
			//		case 1:
			//			{
			//				glTexImage2D( target, level, GL_LUMINANCE_ALPHA, width, height, border, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, pixels );
			//				return Device::PF_L8;
			//			}
			//		default:
			//			{
			//				ASSERT(false);//暂时不处理其他情况
			//			}
			//		}
			//	}
		case Device::PF_R5G6B5 :	//需要数据转换
			{
				ASSERT( 1 != pixelsize );
				ushort* newData = NEW ushort[width*height];
				for ( int i = 0 ; i < height ; i ++  )
				{
					for ( int j = 0 ; j < width ; j ++ )
					{
						ushort data = 0;
						data |= ( pixels[ ( j + i * width ) * pixelsize + 0 ] >> 3 ) << 11;
						data |= ( pixels[ ( j + i * width ) * pixelsize + 1 ] >> 2 ) << 5;
						data |= pixels[ ( j + i * width ) * pixelsize + 2 ] >> 3;
						newData[j + i * width] = data;
					}
				}
				glTexImage2D( target, level, GL_RGB, width, height, border, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, newData );
				CHECK_ERROR_GLES;
				SAFE_DELETE_ARRAY(newData);
				return Device::PF_R5G6B5;
			}
		case Device::PF_R8G8B8 :
			{
				switch( pixelsize )
				{
				case 3:
					{
						glTexImage2D( target, level, GL_RGB, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, pixels );
						CHECK_ERROR_GLES;
						return Device::PF_R8G8B8;
					}
				case 4://去掉alpha
					{
						byte* newData = NEW byte[width*height*3];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								int index = ( j + i * width ) * 3;
								int indexSor = ( j + i * width ) * 4;
								newData[ index + 0 ] = pixels[ indexSor + 0 ];
								newData[ index + 1 ] = pixels[ indexSor + 1 ];
								newData[ index + 2 ] = pixels[ indexSor + 2 ];
							}
						}
						glTexImage2D( target, level, GL_RGB, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, newData );
						CHECK_ERROR_GLES;
						SAFE_DELETE_ARRAY(newData);
						return Device::PF_R8G8B8;
					}
				default:
					{
						ASSERT(false);
					}break;
				}
			}
		case Device::PF_R4G4B4A4 :
			{
				switch( pixelsize )  //需要数据转换
				{
				case 3: // 这里就直接转换成 RGB565了，应为多alpha通道也是浪费
					{
						ushort* newData = NEW ushort[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								ushort data = 0;
								data |= ( pixels[ ( j + i * width ) * pixelsize + 0 ] >> 3 ) << 11;
								data |= ( pixels[ ( j + i * width ) * pixelsize + 1 ] >> 2 ) << 6;
								data |= pixels[ ( j + i * width ) * pixelsize + 2 ] >> 3;
								newData[j + i * width] = data;
							}
						}
						glTexImage2D( target, level, GL_RGB, width, height, border, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, newData );
						CHECK_ERROR_GLES;
						SAFE_DELETE_ARRAY(newData);
						return Device::PF_R5G6B5;
					}
				case 4:
					{
						ushort* newData = NEW ushort[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								ushort data = 0;
								data |= ( pixels[ ( j + i * width ) * pixelsize + 0 ] >> 4 ) << 12;
								data |= ( pixels[ ( j + i * width ) * pixelsize + 1 ] >> 4 ) << 8;
								data |= ( pixels[ ( j + i * width ) * pixelsize + 2 ] >> 4 ) << 4;
								data |=   pixels[ ( j + i * width ) * pixelsize + 3 ] >> 4;
								newData[j + i * width] = data;
							}
						}
						glTexImage2D( target, level, GL_RGBA, width, height, border, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, newData );
						CHECK_ERROR_GLES;
						SAFE_DELETE_ARRAY(newData);
						return Device::PF_R4G4B4A4;
					}
				default:
					{
						ASSERT(false);
					}break;
				}
			}
		case Device::PF_R8G8B8A8 :
			{
				switch( pixelsize )
				{
				case 3: // 这里就直接转换成 RGB8了，应为多alpha通道也是浪费
					{
						glTexImage2D( target, level, GL_RGB, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, pixels );
						CHECK_ERROR_GLES;
						return Device::PF_R8G8B8;
					}
				case 4:
					{
						glTexImage2D( target, level, GL_RGBA, width, height, border, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
						CHECK_ERROR_GLES;
						return Device::PF_R8G8B8A8;
					}
				default:
					{
						ASSERT(false);
					}break;
				}
			}
		case Device::PF_DEPTH16 :
		case Device::PF_DEPTH24_STENCIL8 :
			{
				glTexImage2D( target, level, GL_DEPTH_COMPONENT, width, height, border, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, pixels );
				CHECK_ERROR_GLES;
				return  pf;
			}
		default:
			{
				ASSERT(false);
			}
		}
		ASSERT(false);
		return Device::PF_A8;
	}

	//------------------------------------------------------------------------------------------------------
	void CheckSize( const vector2d& size )
	{

	}
	bool Device::LoadTextureFromFile( Device::PixelFormat pf, TextureData& out, const std::string& strFileName)
	{
		handle	hTex;
		ResLoader::ResTextrue* tex = ResLoader::Instance().LoadTexture(strFileName);
		//GLenum glFormat = tex->m_PixelSize == 3 ? GL_RGB : GL_RGBA;
#ifdef _DEBUG
		CheckSize( tex->m_Size );
#endif
		glGenTextures( 1, &hTex );
		glBindTexture( GL_TEXTURE_2D, hTex );
		m_hCurrentTexture = hTex;
		//if( true == m_isUsingAnisotropic )	//如果支持各向异性
		//{
		//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_fMaxAnisotropic);
		//}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); 
		out.m_eFormat = glTexImage2DHelper(GL_TEXTURE_2D, 0, tex->m_PixelSize, pf, tex->m_Size.m_x, tex->m_Size.m_y, 0, tex->m_pData );//读取出来的data都是8 bit的，如果需要使用非8色彩格式，hlper中会转换
		glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
		out.m_eType = Device::TT_TEXTURE2D;
		out.m_hasMipMap = true;
		out.m_hTextureHandle = hTex;
		out.m_vSize = tex->m_Size;
		out.m_PixelSize = tex->m_PixelSize;
		out.m_SWarp = Device::TW_CLAMP_TO_EDGE;
		out.m_TWarp = Device::TW_CLAMP_TO_EDGE;
		out.m_MagFilter = Device::TF_LINEAR;
		out.m_MinFilter = Device::TF_LINEAR_MIPMAP_LINEAR;
		out.m_Anisotropic = 0;
		//out.m_eFormat = tex->m_PixelSize == 3 ? Device::RGB : Device::RGBA;
		out.m_Name = strFileName;
		SAFE_DELETE(tex);
		return true;
	}
	//------------------------------------------------------------------------------------------------------
	bool Device::LoadCubeTextureFromFile( Device::PixelFormat pf, TextureData& out, const std::string& f, const std::string& b, const std::string& u, const std::string& d, const std::string& l, const std::string& r )
	{
		handle	hTex;
		ResLoader::ResTextrue* ff = ResLoader::Instance().LoadTexture(f);
		ResLoader::ResTextrue* fb = ResLoader::Instance().LoadTexture(b);
		ResLoader::ResTextrue* fu = ResLoader::Instance().LoadTexture(u);
		ResLoader::ResTextrue* fd = ResLoader::Instance().LoadTexture(d);
		ResLoader::ResTextrue* fl = ResLoader::Instance().LoadTexture(l);
		ResLoader::ResTextrue* fr = ResLoader::Instance().LoadTexture(r);

#ifdef _DEBUG
		CheckSize( ff->m_Size );
		CheckSize( fb->m_Size );
		CheckSize( fu->m_Size );
		CheckSize( fd->m_Size );
		CheckSize( fl->m_Size );
		CheckSize( fr->m_Size );
#endif

		glGenTextures( 1, &hTex );
		glBindTexture( GL_TEXTURE_CUBE_MAP, hTex );
		m_hCurrentTexture = hTex;
		//if( true == m_isUsingAnisotropic )	//如果支持各向异性
		//{
		//	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_fMaxAnisotropic);
		//}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		Device::PixelFormat pff, pfb, pfu, pfd, pfl, pfr;
		pff = glTexImage2DHelper(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, ff->m_PixelSize, pf, ff->m_Size.m_x, ff->m_Size.m_y, 0, ff->m_pData );
		pfb = glTexImage2DHelper(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, fb->m_PixelSize, pf, fb->m_Size.m_x, fb->m_Size.m_y, 0, fb->m_pData );
		pfu = glTexImage2DHelper(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, fu->m_PixelSize, pf, fu->m_Size.m_x, fu->m_Size.m_y, 0, fu->m_pData );
		pfd = glTexImage2DHelper(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, fd->m_PixelSize, pf, fd->m_Size.m_x, fd->m_Size.m_y, 0, fd->m_pData );
		pfl = glTexImage2DHelper(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, fl->m_PixelSize, pf, fl->m_Size.m_x, fl->m_Size.m_y, 0, fl->m_pData );
		pfr = glTexImage2DHelper(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, fr->m_PixelSize, pf, fr->m_Size.m_x, fr->m_Size.m_y, 0, fr->m_pData );
		ASSERT( pff == pfb && pff == pfu && pff == pfd && pff == pfl && pff == pfr );
		//glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, ff->m_PixelSize == 3 ? GL_RGB : GL_RGBA, ff->m_Size.m_x, ff->m_Size.m_y, 0, ff->m_PixelSize == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, ff->m_pData );
		//glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, fb->m_PixelSize == 3 ? GL_RGB : GL_RGBA, fb->m_Size.m_x, fb->m_Size.m_y, 0, fb->m_PixelSize == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, fb->m_pData );
		//glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, fu->m_PixelSize == 3 ? GL_RGB : GL_RGBA, fu->m_Size.m_x, fu->m_Size.m_y, 0, fu->m_PixelSize == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, fu->m_pData );
		//glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, fd->m_PixelSize == 3 ? GL_RGB : GL_RGBA, fd->m_Size.m_x, fd->m_Size.m_y, 0, fd->m_PixelSize == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, fd->m_pData );
		//glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, fl->m_PixelSize == 3 ? GL_RGB : GL_RGBA, fl->m_Size.m_x, fl->m_Size.m_y, 0, fl->m_PixelSize == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, fl->m_pData );
		//glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, fr->m_PixelSize == 3 ? GL_RGB : GL_RGBA, fr->m_Size.m_x, fr->m_Size.m_y, 0, fr->m_PixelSize == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, fr->m_pData );
		//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);//生成mipmap
		out.m_eFormat = pff;
		out.m_eType = Device::TT_TEXTURECUBE;
		//out.m_eFormat = ff->m_PixelSize == 3 ? Device::RGB : Device::RGBA;
		out.m_hasMipMap = false;
		out.m_hTextureHandle = hTex;
		out.m_vSize = ff->m_Size;
		out.m_Name = f+b+u+d+l+r;
		out.m_SWarp = Device::TW_CLAMP_TO_EDGE;
		out.m_TWarp = Device::TW_CLAMP_TO_EDGE;
		out.m_MagFilter = Device::TF_LINEAR;
		out.m_MinFilter = Device::TF_LINEAR;
		out.m_Anisotropic = 0;
		SAFE_DELETE( ff );
		SAFE_DELETE( fb );
		SAFE_DELETE( fu );
		SAFE_DELETE( fd );
		SAFE_DELETE( fl );
		SAFE_DELETE( fr );
		return true;
	}
	//------------------------------------------------------------------------------------------------------
	bool Device::MakeTextureFromStream( handle& h, const TextureData& input, const byte* buffer )
	{
		//ASSERT( format != Device::DEPTH );
		ASSERT( input.m_eType == Device::TT_TEXTURE2D );
		handle	hTex;
		//GLint glFormat = Device::RGB == format ? GL_RGB : GL_RGBA;
		glGenTextures( 1, &hTex );
		glBindTexture( GL_TEXTURE_2D, hTex );
		m_hCurrentTexture = hTex;

		SetTextureAnisotropic( hTex, input.m_eType, input.m_Anisotropic );
		SetTextureWarp( hTex, input.m_eType, input.m_SWarp, input.m_TWarp );
		SetTextureFilter( hTex, input.m_eType, input.m_MagFilter, input.m_MinFilter );

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
		//glTexImage2D( GL_TEXTURE_2D, 0, glFormat, size.m_x, size.m_y, 0, glFormat, GL_UNSIGNED_BYTE, buffer );
		glTexImage2DHelper(GL_TEXTURE_2D, 0, input.m_PixelSize, input.m_eFormat, input.m_vSize.m_x, input.m_vSize.m_y, 0, buffer );

		if ( input.m_hasMipMap )
		{
			glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
		}
		h = hTex;

		return true;
	}
	//------------------------------------------------------------------------------------------------------
	bool Device::SubTextureFromStream( handle& h, const TextureData& input, const byte* buffer )
	{
		ASSERT( input.m_eType == Device::TT_TEXTURE2D );
		glBindTexture( GL_TEXTURE_2D, h );
		m_hCurrentTexture = -1;
		GLenum glformat,gltype;
		switch( input.m_eFormat )
		{
		case Device::PF_A8 : glformat = GL_ALPHA; gltype = GL_UNSIGNED_BYTE; break;
		case Device::PF_L8 : glformat = GL_LUMINANCE; gltype = GL_UNSIGNED_BYTE; break;
			//case Device::PF_LA8 : glformat = GL_LUMINANCE_ALPHA; gltype = GL_UNSIGNED_BYTE; break;		
		case Device::PF_R5G6B5 : glformat = GL_RGB; gltype = GL_UNSIGNED_SHORT_5_6_5; break;
		case Device::PF_R8G8B8 : glformat = GL_RGB; gltype = GL_UNSIGNED_BYTE; break;
		case Device::PF_R4G4B4A4 : glformat = GL_RGBA; gltype = GL_UNSIGNED_SHORT_4_4_4_4; break;
		case Device::PF_R8G8B8A8 : glformat = GL_RGBA; gltype = GL_UNSIGNED_BYTE; break;
		}
		glTexSubImage2D ( GL_TEXTURE_2D, 0, 0, 0, input.m_vSize.m_x, input.m_vSize.m_y, glformat, gltype, buffer);
		if( input.m_hasMipMap )
		{
			glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
		}
		return true;
	}
	//------------------------------------------------------------------------------------------------------
	void Device::CreateBuffer(handle& h, BufferType type, uint totleSize, const void* dataptr, MemoryUseage use)
	{
		glGenBuffers(1, &h);
		CHECK_ERROR_GLES;
		glBindBuffer(GL_ARRAY_BUFFER + static_cast<int>(type), h);
		CHECK_ERROR_GLES;
		m_hCurrentBuffer = h;
		switch( use )
		{
		case Device::MU_STATIC:glBufferData(GL_ARRAY_BUFFER + static_cast<int>(type), totleSize, dataptr, GL_STATIC_DRAW);break;
		case Device::MU_DYNAMIC:glBufferData(GL_ARRAY_BUFFER + static_cast<int>(type), totleSize, dataptr, GL_DYNAMIC_DRAW);break;
		}
		CHECK_ERROR_GLES;
		//DEBUGLOG("Make VBO %d\n",h);
	}
	//------------------------------------------------------------------------------------------------------
	void Device::ReallocBuffer(handle& h, BufferType type, uint totleSize, const void* dataptr, MemoryUseage use)
	{
		if ( m_hCurrentBuffer != h )
		{
			m_hCurrentBuffer = h;
			glBindBuffer(GL_ARRAY_BUFFER + static_cast<int>(type), h);
		}
		switch( use )
		{
		case Device::MU_STATIC:glBufferData(GL_ARRAY_BUFFER + static_cast<int>(type), totleSize, dataptr, GL_STATIC_DRAW);break;
		case Device::MU_DYNAMIC:glBufferData(GL_ARRAY_BUFFER + static_cast<int>(type), totleSize, dataptr, GL_DYNAMIC_DRAW);break;
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::ChangeData(handle h, BufferType type, uint offset, uint biteSize, const void* dataptr )
	{
		if ( m_hCurrentBuffer != h )
		{
			m_hCurrentBuffer = h;
			glBindBuffer(GL_ARRAY_BUFFER + static_cast<int>(type), h);
		}
		glBufferSubData(GL_ARRAY_BUFFER + static_cast<int>(type), offset, biteSize, dataptr);
	}
	//------------------------------------------------------------------------------------------------------
	void Device::BindBuffer(handle h, BufferType type)
	{
		if ( m_hCurrentBuffer != h )
		{
			m_hCurrentBuffer = h;
			glBindBuffer(GL_ARRAY_BUFFER + static_cast<int>(type), h);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::DeleteBuffer(handle& h)
	{
		//DEBUGLOG("Delete VBO %d\n",h);
		glDeleteBuffers( 1, &h);
		h = -1;
	}
	//------------------------------------------------------------------------------------------------------
	/*void Device::CreateBuffer(handle& h, BufferType type, uint bitSize, Useage use)
	{
	glGenBuffers(1, &h);
	glBindBuffer(GL_ARRAY_BUFFER + static_cast<int>(type), h);
	switch( use )
	{
	case Device::STATIC:glBufferData(GL_ARRAY_BUFFER + static_cast<int>(type), bitSize, NULL, GL_STATIC_DRAW);break;
	case Device::DYNAMIC:glBufferData(GL_ARRAY_BUFFER + static_cast<int>(type), bitSize, NULL, GL_DYNAMIC_DRAW);break;
	}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::BindBuffer(BufferType type, handle& h)
	{
	glBindBuffer(GL_ARRAY_BUFFER + static_cast<int>(type), h);
	}
	//------------------------------------------------------------------------------------------------------
	void Device::FillBuffer(BufferType type, handle& h, FloatVector& vec)
	{
	glBindBuffer(GL_ARRAY_BUFFER + static_cast<int>(type), h);
	glBufferSubData(GL_ARRAY_BUFFER + static_cast<int>(type), 0, vec.BitSize(), vec.GetDataArray());
	}
	//------------------------------------------------------------------------------------------------------
	void Device::FillBuffer(BufferType type, handle& h, IntVector& vec)
	{
	glBindBuffer(GL_ARRAY_BUFFER + static_cast<int>(type), h);
	glBufferSubData(GL_ARRAY_BUFFER + static_cast<int>(type), 0, vec.BitSize(), vec.GetDataArray());
	}
	//------------------------------------------------------------------------------------------------------
	void Device::DeleteBuffer(handle& h)
	{
	glDeleteBuffers(1,&h);
	h = 0;
	}*/
	//------------------------------------------------------------------------------------------------------
	bool Device::CreateRenderTarget(  RenderTarget& rt )
	{
		vector2d size = rt.GetSize();
		std::vector<uint> clr(size.m_x*size.m_y,1);
		switch(rt.m_eType)
		{
		case FBO_TCOLOR:
			{
				GLuint hcrt;
				GLuint htexColor;

				//生成纹理
				glGenTextures(1, &htexColor);
				glBindTexture(GL_TEXTURE_2D, htexColor);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
					static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
					0, GL_RGBA, GL_UNSIGNED_BYTE, &clr[0] );
				CHECK_ERROR_GLES;
				glBindTexture(GL_TEXTURE_2D, 0);
				m_hCurrentTexture = -1;

				//生成绑定纹理
				glGenFramebuffers(1, &hcrt);
				glBindFramebuffer(GL_FRAMEBUFFER, hcrt);
				CHECK_ERROR_GLES;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, htexColor, 0);
				CHECK_ERROR_GLES;
				glBindFramebuffer( GL_FRAMEBUFFER, 0 );
				CHECK_ERROR_GLES;

				rt.m_hRenderTarget = hcrt;
				rt.m_pColorTexture->m_hasMipMap = false;
				rt.m_pColorTexture->m_hTextureHandle = htexColor;
				rt.m_pColorTexture->m_eFormat = PF_R8G8B8A8;
				rt.m_pColorTexture->m_isLoaded = true;
				rt.m_pColorTexture->m_Name = "Color RT";
				rt.m_pColorTexture->m_vSize = size;
				rt.m_pColorTexture->m_Warp.m_SWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Warp.m_TWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Filter.m_MagFilter = Device::TF_LINEAR;
				rt.m_pColorTexture->m_Filter.m_MinFilter = Device::TF_LINEAR;
			}break;
		case FBO_TDEPTH:
			{
				GLuint hdrt;
				GLuint htexDepth;

				glGenTextures(1, &htexDepth);
				glBindTexture(GL_TEXTURE_2D, htexDepth);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
#ifdef OPENGL
				glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 
#else
				glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 
#endif
					static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
					0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, &clr[0] );
				CHECK_ERROR_GLES;
				glBindTexture(GL_TEXTURE_2D, 0);
				m_hCurrentTexture = -1;

				glGenFramebuffers(1, &hdrt);
				glBindFramebuffer(GL_FRAMEBUFFER, hdrt);
				CHECK_ERROR_GLES;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, htexDepth, 0);
				CHECK_ERROR_GLES;
				glBindFramebuffer( GL_FRAMEBUFFER, 0 );

				rt.m_hRenderTarget = hdrt;
				rt.m_pDepthStencilTexture->m_hasMipMap = false;
				rt.m_pDepthStencilTexture->m_hTextureHandle = htexDepth;
				rt.m_pDepthStencilTexture->m_eFormat = PF_DEPTH16;
				rt.m_pDepthStencilTexture->m_isLoaded = true;
				rt.m_pDepthStencilTexture->m_Name = "Depth RT";
				rt.m_pDepthStencilTexture->m_vSize = size;
				rt.m_pDepthStencilTexture->m_Warp.m_SWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pDepthStencilTexture->m_Warp.m_TWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pDepthStencilTexture->m_Filter.m_MagFilter = Device::TF_LINEAR;
				rt.m_pDepthStencilTexture->m_Filter.m_MinFilter = Device::TF_LINEAR;
			}break;
		case FBO_TCOLOR_TDEPTH:
			{
				GLuint hcrt;
				GLuint htexColor;
				GLuint htexDepth;

				glGenTextures(1, &htexColor);
				glBindTexture(GL_TEXTURE_2D, htexColor);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
					static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
					0, GL_RGBA, GL_UNSIGNED_BYTE, &clr[0] );
				CHECK_ERROR_GLES;
				glGenTextures(1, &htexDepth);
				glBindTexture(GL_TEXTURE_2D, htexDepth);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
#ifdef OPENGL
				glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 
#else
				glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 
#endif
					static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
					0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, &clr[0] );
				CHECK_ERROR_GLES;
				glBindTexture(GL_TEXTURE_2D, 0);
				m_hCurrentTexture = -1;

				glGenFramebuffers(1, &hcrt);
				glBindFramebuffer(GL_FRAMEBUFFER, hcrt);
				CHECK_ERROR_GLES;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, htexColor, 0);
				CHECK_ERROR_GLES;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, htexDepth, 0);
				CHECK_ERROR_GLES;
				glBindFramebuffer( GL_FRAMEBUFFER, 0 );

				rt.m_hRenderTarget = hcrt;
				rt.m_pColorTexture->m_hasMipMap = false;
				rt.m_pColorTexture->m_hTextureHandle = htexColor;
				rt.m_pColorTexture->m_eFormat = PF_R8G8B8A8;
				rt.m_pColorTexture->m_isLoaded = true;
				rt.m_pColorTexture->m_Name = "Color RT";
				rt.m_pColorTexture->m_vSize = size;
				rt.m_pColorTexture->m_Warp.m_SWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Warp.m_TWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Filter.m_MagFilter = Device::TF_LINEAR;
				rt.m_pColorTexture->m_Filter.m_MinFilter = Device::TF_LINEAR;

				rt.m_pDepthStencilTexture->m_hasMipMap = false;
				rt.m_pDepthStencilTexture->m_hTextureHandle = htexDepth;
				rt.m_pDepthStencilTexture->m_eFormat = PF_DEPTH16;
				rt.m_pDepthStencilTexture->m_isLoaded = true;
				rt.m_pDepthStencilTexture->m_Name = "Depth RT";
				rt.m_pDepthStencilTexture->m_vSize = size;
				rt.m_pDepthStencilTexture->m_Warp.m_SWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pDepthStencilTexture->m_Warp.m_TWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pDepthStencilTexture->m_Filter.m_MagFilter = Device::TF_LINEAR;
				rt.m_pDepthStencilTexture->m_Filter.m_MinFilter = Device::TF_LINEAR;
			}break;
		case FBO_TDEPTHSTENCIL:
			{
				GLuint hdrt;
				GLuint htexDepthStencil;

				glGenTextures(1, &htexDepthStencil);
				glBindTexture(GL_TEXTURE_2D, htexDepthStencil);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
#ifdef OPENGL
				glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, 
					static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
					0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, &clr[0] );
				CHECK_ERROR_GLES;

#else
				glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL_OES, 
					static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
					0, GL_DEPTH_STENCIL_OES, GL_UNSIGNED_INT_24_8_OES, &clr[0] );
				CHECK_ERROR_GLES;
#endif
				glBindTexture(GL_TEXTURE_2D, 0);
				m_hCurrentTexture = -1;

				glGenFramebuffers(1, &hdrt);
				glBindFramebuffer(GL_FRAMEBUFFER, hdrt);
				CHECK_ERROR_GLES;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, htexDepthStencil, 0);
				CHECK_ERROR_GLES;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, htexDepthStencil, 0);
				CHECK_ERROR_GLES;
				glBindFramebuffer( GL_FRAMEBUFFER, 0 );

				rt.m_hRenderTarget = hdrt;
				rt.m_pDepthStencilTexture->m_hasMipMap = false;
				rt.m_pDepthStencilTexture->m_hTextureHandle = htexDepthStencil;
				rt.m_pDepthStencilTexture->m_eFormat = PF_DEPTH24_STENCIL8;
				rt.m_pDepthStencilTexture->m_isLoaded = true;
				rt.m_pDepthStencilTexture->m_Name = "Depth Stencil RT";
				rt.m_pDepthStencilTexture->m_vSize = size;
				rt.m_pDepthStencilTexture->m_Warp.m_SWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pDepthStencilTexture->m_Warp.m_TWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pDepthStencilTexture->m_Filter.m_MagFilter = Device::TF_LINEAR;
				rt.m_pDepthStencilTexture->m_Filter.m_MinFilter = Device::TF_LINEAR;
			}break;
		case FBO_TCOLOR_BDEPTH:
			{
				GLuint hcrt;
				GLuint hdrt;
				GLuint htexColor;
				// Create the color buffer

				glGenTextures( 1, &htexColor );
				glBindTexture( GL_TEXTURE_2D, htexColor );
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
					static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
					0, GL_RGBA, GL_UNSIGNED_BYTE, &clr[0] );
				CHECK_ERROR_GLES;
				glBindTexture(GL_TEXTURE_2D, 0);
				m_hCurrentTexture = -1;

				// Create the depth buffer
				glGenRenderbuffers( 1, &hdrt );
				glBindRenderbuffer( GL_RENDERBUFFER, hdrt );
				CHECK_ERROR_GLES;
				glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y) );
				CHECK_ERROR_GLES;
				glBindRenderbuffer( GL_RENDERBUFFER, 0 );

				glGenFramebuffers( 1, &hcrt );
				glBindFramebuffer( GL_FRAMEBUFFER, hcrt );
				CHECK_ERROR_GLES;
				glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, htexColor, 0 );
				CHECK_ERROR_GLES;
				glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrt );
				CHECK_ERROR_GLES;
				glBindFramebuffer( GL_FRAMEBUFFER, 0 );


				rt.m_hRenderTarget = hcrt;
				rt.m_hDepthBuffer = hdrt;
				rt.m_pColorTexture->m_hasMipMap = false;
				rt.m_pColorTexture->m_hTextureHandle = htexColor;
				rt.m_pColorTexture->m_eFormat = PF_R8G8B8A8;
				rt.m_pColorTexture->m_isLoaded = true;
				rt.m_pColorTexture->m_Name = "Color RT";
				rt.m_pColorTexture->m_vSize = size;
				rt.m_pColorTexture->m_Warp.m_SWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Warp.m_TWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Filter.m_MagFilter = Device::TF_LINEAR;
				rt.m_pColorTexture->m_Filter.m_MinFilter = Device::TF_LINEAR;
			}break;
		case FBO_TCOLOR_BDEPTHSTENCIL:
			{
				GLuint hcrt;
				GLuint hdrt;
				GLuint htexColor;
				// Create the color buffer

				glGenTextures( 1, &htexColor );
				glBindTexture( GL_TEXTURE_2D, htexColor );
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
					static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
					0, GL_RGBA, GL_UNSIGNED_BYTE, &clr[0] );
				glBindTexture(GL_TEXTURE_2D, 0);
				m_hCurrentTexture = -1;

				// Create the depth buffer
				glGenRenderbuffers( 1, &hdrt );
				glBindRenderbuffer( GL_RENDERBUFFER, hdrt );
#ifdef OPENGL
				glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y) );
				CHECK_ERROR_GLES;
#else
				glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y) );
				CHECK_ERROR_GLES;
#endif
				glBindRenderbuffer( GL_RENDERBUFFER, 0 );
				CHECK_ERROR_GLES;

				glGenFramebuffers( 1, &hcrt );
				glBindFramebuffer( GL_FRAMEBUFFER, hcrt );
				glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, htexColor, 0 );
				CHECK_ERROR_GLES;
				glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrt );
				CHECK_ERROR_GLES;
				glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, hdrt );
				CHECK_ERROR_GLES;
				glBindFramebuffer( GL_FRAMEBUFFER, 0 );


				rt.m_hRenderTarget = hcrt;
				rt.m_hDepthBuffer = hdrt;
				rt.m_pColorTexture->m_hasMipMap = false;
				rt.m_pColorTexture->m_hTextureHandle = htexColor;
				rt.m_pColorTexture->m_eFormat = PF_R8G8B8A8;
				rt.m_pColorTexture->m_isLoaded = true;
				rt.m_pColorTexture->m_Name = "Color RT";
				rt.m_pColorTexture->m_vSize = size;
				rt.m_pColorTexture->m_Warp.m_SWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Warp.m_TWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Filter.m_MagFilter = Device::TF_LINEAR;
				rt.m_pColorTexture->m_Filter.m_MinFilter = Device::TF_LINEAR;
			}break;
		case FBO_TCOLOR_TDEPTHSTENCIL:
			{
				GLuint hcrt;
				GLuint htexColor;
				GLuint htexDepthStencil;

				glGenTextures(1, &htexColor);
				glBindTexture(GL_TEXTURE_2D, htexColor);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
					static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
					0, GL_RGBA, GL_UNSIGNED_BYTE, &clr[0] );
				CHECK_ERROR_GLES;
				glGenTextures(1, &htexDepthStencil);
				glBindTexture(GL_TEXTURE_2D, htexDepthStencil);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
#ifdef OPENGL
				glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, 
					static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
					0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, &clr[0] );
				CHECK_ERROR_GLES;

#else
				glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL_OES, 
					static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
					0, GL_DEPTH_STENCIL_OES, GL_UNSIGNED_INT_24_8_OES, &clr[0] );
				CHECK_ERROR_GLES;
#endif
				glBindTexture(GL_TEXTURE_2D, 0);
				m_hCurrentTexture = -1;

				glGenFramebuffers(1, &hcrt);
				glBindFramebuffer(GL_FRAMEBUFFER, hcrt);
				CHECK_ERROR_GLES;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, htexColor, 0);
				CHECK_ERROR_GLES;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, htexDepthStencil, 0);
				CHECK_ERROR_GLES;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, htexDepthStencil, 0);
				CHECK_ERROR_GLES;
				glBindFramebuffer( GL_FRAMEBUFFER, 0 );

				rt.m_hRenderTarget = hcrt;
				rt.m_pColorTexture->m_hasMipMap = false;
				rt.m_pColorTexture->m_hTextureHandle = htexColor;
				rt.m_pColorTexture->m_eFormat = PF_R8G8B8A8;
				rt.m_pColorTexture->m_isLoaded = true;
				rt.m_pColorTexture->m_Name = "Color RT";
				rt.m_pColorTexture->m_vSize = size;
				rt.m_pColorTexture->m_Warp.m_SWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Warp.m_TWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Filter.m_MagFilter = Device::TF_LINEAR;
				rt.m_pColorTexture->m_Filter.m_MinFilter = Device::TF_LINEAR;
				rt.m_pDepthStencilTexture->m_hasMipMap = false;
				rt.m_pDepthStencilTexture->m_hTextureHandle = htexDepthStencil;
				rt.m_pDepthStencilTexture->m_eFormat = PF_DEPTH24_STENCIL8;
				rt.m_pDepthStencilTexture->m_isLoaded = true;
				rt.m_pDepthStencilTexture->m_Name = "Depth Stencil RT";
				rt.m_pDepthStencilTexture->m_vSize = size;
				rt.m_pDepthStencilTexture->m_Warp.m_SWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pDepthStencilTexture->m_Warp.m_TWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pDepthStencilTexture->m_Filter.m_MagFilter = Device::TF_LINEAR;
				rt.m_pDepthStencilTexture->m_Filter.m_MinFilter = Device::TF_LINEAR;
			}break;
		case Device::FBO_CCOLOR_BDEPTH:
			{
				GLuint hcrt;
				GLuint hdrt;
				GLuint htexColor;
				// Create the color buffer
				glGenTextures( 1, &htexColor );
				glBindTexture( GL_TEXTURE_CUBE_MAP, htexColor );
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
				for ( int i = 0; i < 6; i ++ )
				{
					glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA,
						static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y), 
						0, GL_RGBA, GL_UNSIGNED_BYTE, &clr[0] );
					CHECK_ERROR_GLES;
				}

				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				m_hCurrentTexture = -1;

				// Create the depth buffer
				glGenRenderbuffers( 1, &hdrt );
				glBindRenderbuffer( GL_RENDERBUFFER, hdrt );
				CHECK_ERROR_GLES;
				glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, static_cast<GLsizei>(rt.GetSize().m_x), static_cast<GLsizei>(rt.GetSize().m_y) );
				CHECK_ERROR_GLES;
				glBindRenderbuffer( GL_RENDERBUFFER, 0 );

				glGenFramebuffers( 1, &hcrt );
				glBindFramebuffer( GL_FRAMEBUFFER, hcrt );
				CHECK_ERROR_GLES;
				for ( int i = 0; i < 6; i ++ )
				{
					glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, htexColor, 0 );
					CHECK_ERROR_GLES;
				}
				glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrt );
				CHECK_ERROR_GLES;
				glBindFramebuffer( GL_FRAMEBUFFER, 0 );


				rt.m_hRenderTarget = hcrt;
				rt.m_hDepthBuffer = hdrt;
				rt.m_pColorTexture->m_hasMipMap = false;
				rt.m_pColorTexture->m_hTextureHandle = htexColor;
				rt.m_pColorTexture->m_eFormat = PF_R8G8B8A8;
				rt.m_pColorTexture->m_isLoaded = true;
				rt.m_pColorTexture->m_Name = "Cube Color RT";
				rt.m_pColorTexture->m_vSize = size;
				rt.m_pColorTexture->m_Warp.m_SWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Warp.m_TWarp = Device::TW_CLAMP_TO_EDGE;
				rt.m_pColorTexture->m_Filter.m_MagFilter = Device::TF_LINEAR;
				rt.m_pColorTexture->m_Filter.m_MinFilter = Device::TF_LINEAR;
			}break;
		}
		return true;
	}
	////------------------------------------------------------------------------------------------------------
	//RenderTarget Device::GetBindRenderTarget( )
	//{
	//	RenderTarget rt;	
	//	//glGetIntegerv(GL_FRAMEBUFFER_BINDING, & framebufferhandle );//这个新能很低的。。。这里就不用了，ios想其他办法把
	//	//ASSERT( -1 != m_hCurrentFBO );//如果是-1说明没有RT，到时候就不压了
	//	rt.m_isGet = true;
	//	rt.m_hRenderTarget = m_hCurrentFBO;
	//	//GLint v[4];
	//	//glGetIntegerv(GL_VIEWPORT, v);
	//	rt.GetSize() = Viewport;
	//	return rt;
	//}
	//------------------------------------------------------------------------------------------------------
	//void Device::PushRenderTarget( const handle& rt, const handle& texColor, const handle& texDepthStencil ) //旧的，这里纹理是不需要传递进来的
	void Device::PushRenderTarget( RenderTarget* const rt )
	{
		if ( NULL != rt 
			&& -1 != rt->m_hRenderTarget )
			//&& rt->m_eType != FBO_Empty ) // becouse ios
		{
			ASSERT( NULL == m_pCurrentFBO );
			m_pCurrentFBO = rt;//这里保存指针，当记录的rt被西沟这里会有问题，但是一般不会产生
			//glBindTexture(GL_TEXTURE_2D, 0); //这个在主页的例子中有，但我觉得没用 http://www.khronos.org/registry/gles/extensions/OES/OES_packed_depth_stencil.txt
			m_vOldViewPort = m_Viewport;
			this->SetViewport(rt->m_vViewPort);
			glBindFramebuffer( GL_FRAMEBUFFER, rt->m_hRenderTarget );
			CHECK_ERROR_GLES;
			if( rt->m_hasTexColor )
			{
				if ( Device::FBO_CCOLOR_BDEPTH != rt->GetType() )
				{
					glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt->GetColorBuffer()->GetTextureHandle(), 0 );
				}
				else
				{
					RenderTargetCube* crt = static_cast<RenderTargetCube*>(rt);
					glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+crt->GetCurrentFace(), rt->GetColorBuffer()->GetTextureHandle(), 0 );
				}
				CHECK_ERROR_GLES;
			}
			switch( rt->m_eType )
			{
			case Device::FBO_EMPTY:
			case Device::FBO_TCOLOR:
				{
				}break;
			case Device::FBO_TDEPTH:
			case Device::FBO_TCOLOR_TDEPTH:
				{
					glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rt->GetDepthBuffer()->GetTextureHandle(), 0 );
					CHECK_ERROR_GLES;
				}break;
			case Device::FBO_TDEPTHSTENCIL:
			case Device::FBO_TCOLOR_TDEPTHSTENCIL:
				{
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rt->GetDepthBuffer()->GetTextureHandle(), 0);
					CHECK_ERROR_GLES;
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, rt->GetDepthBuffer()->GetTextureHandle(), 0);
					CHECK_ERROR_GLES;
				}break;
			case Device::FBO_CCOLOR_BDEPTH:
			case Device::FBO_TCOLOR_BDEPTH:
				{
					glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rt->m_hDepthBuffer );
					CHECK_ERROR_GLES;
				}break;
			case Device::FBO_TCOLOR_BDEPTHSTENCIL:
				{
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rt->m_hDepthBuffer );
					CHECK_ERROR_GLES;
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rt->m_hDepthBuffer );
					CHECK_ERROR_GLES;
				}break;
			default:
				{
					ASSERT(false);
				}break;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	//void Device::PopRenderTarget( RenderTarget* const rt )
	RenderTarget* Device::PopRenderTarget( )
	{
		if( NULL != m_pCurrentFBO )
		{
			this->SetViewport(m_vOldViewPort);
			glBindFramebuffer( GL_FRAMEBUFFER, 0 );
			glBindRenderbuffer( GL_RENDERBUFFER, 0 );
			RenderTarget* temp = m_pCurrentFBO;
			m_pCurrentFBO = NULL;
			return temp;
		}
		return NULL;
		//if ( NULL != rt 
		//	&& -1 != rt->m_hRenderTarget )
		//{
		//	this->SetViewport(m_vOldSize);
		//	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		//	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
		//	m_hCurrentFBO = -1;
		//}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::DestroyRenderTarget( RenderTarget& rt )
	{
		switch( rt.m_eType )
		{
		case Device::FBO_TCOLOR:
		case Device::FBO_TDEPTH:
		case Device::FBO_TCOLOR_TDEPTH:
		case Device::FBO_TDEPTHSTENCIL:
		case Device::FBO_TCOLOR_TDEPTHSTENCIL:
			{
				glDeleteFramebuffers( 1, &rt.m_hRenderTarget );
			}break;
		case Device::FBO_CCOLOR_BDEPTH:
		case Device::FBO_TCOLOR_BDEPTH:
		case Device::FBO_TCOLOR_BDEPTHSTENCIL:
			{
				glDeleteFramebuffers( 1, &rt.m_hRenderTarget );
				glDeleteRenderbuffers( 1, &rt.m_hDepthBuffer );
			}break;
		default:
			{
				ASSERT(false);
			}break;
		}
	}
	//------------------------------------------------------------------------------------------------------
	void Device::CopyFragment( PixelFormat outformat, std::vector<byte>& out )
	{
		GLenum innerFormat;
		uint PixelSize;
		switch( outformat )
		{
		case PF_R8G8B8:
			{
				PixelSize = 3;
				innerFormat = GL_RGB;
			}break;
		case PF_R8G8B8A8:
			{
				PixelSize = 4;
				innerFormat = GL_RGBA;
			}break;
		}
		ASSERT( Math::isPowerOfTwo( m_Viewport.m_z) ); //必须是2的冥次
		ASSERT( Math::isPowerOfTwo( m_Viewport.m_w) );
		ASSERT( out.size() == m_Viewport.m_z*m_Viewport.m_w*PixelSize );//必须传递进来已经初始化分配过内存的数据组
		glPixelStorei( GL_PACK_ALIGNMENT, 1 );
		byte* data = NEW byte[ m_Viewport.m_z*m_Viewport.m_w*PixelSize ];
		glReadPixels( 0, 0, m_Viewport.m_z, m_Viewport.m_w, innerFormat, GL_UNSIGNED_BYTE, data);//读出的纹理是Y对调着的
		//反向
		for ( int y = 0; y < m_Viewport.m_w; y++ )
		{
			memcpy( &out[(m_Viewport.m_w-1-y)*m_Viewport.m_z*PixelSize], &data[y*m_Viewport.m_z*PixelSize], m_Viewport.m_z*PixelSize );
		}
		SAFE_DELETE( data );
	}
	////------------------------------------------------------------------------------------------------------
	//void Device::CopyFragment( Texture2D& tex, const RenderTarget* from )
	//{
	//	ASSERT( m_pCurrentFBO == from );
	//	m_hCurrentTexture = tex.m_hTextureHandle;
	//	glBindTexture( GL_TEXTURE_2D, tex.m_hTextureHandle );
	//	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, tex.m_vSize.m_x, tex.m_vSize.m_y);
	//}
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------

}