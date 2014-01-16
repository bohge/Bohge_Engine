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
//			天空盒		   //
/////////////////////////////


#include "SkyBox.h"
#include "Engine.h"
#include "Texture.h"
#include "3DMath.h"
#include "Vertex.h"
#include "ShaderManage.h"
#include "Camera.h"
#include "Device.h"
#include "RendBuffer.h"
#include "EnvironmentShaders.h"
#include "FilePath.h"


namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	SkyBox::Box::Box( bool isAlpha, const std::string& f, const std::string& b, const std::string& u, const std::string& d, const std::string& l, const std::string& r )
		:m_Times(0)
	{
		ICamera* cv = Engine::Instance().GetCamera();//为了防止被错误的剪切掉，此处使用far和near的平均值作为大小
		float farplane = cv->GetFar();
		float nearplane = cv->GetNear();
		float p = nearplane + ( farplane - nearplane ) / 4;

		m_pVertexArray = NEW Vertex[VERTEX_COUNT];

		m_pVertexArray[0] = Vertex( vector4f(-p, p, p) );
		m_pVertexArray[1] = Vertex( vector4f(-p,-p, p) );
		m_pVertexArray[2] = Vertex( vector4f( p,-p, p) );
		m_pVertexArray[3] = Vertex( vector4f( p, p, p) );
		m_pVertexArray[4] = Vertex( vector4f(-p, p,-p) );
		m_pVertexArray[5] = Vertex( vector4f(-p,-p,-p) );
		m_pVertexArray[6] = Vertex( vector4f( p,-p,-p) );
		m_pVertexArray[7] = Vertex( vector4f( p, p,-p) );
		static ushort indies[] = { 4,5,7,7,5,6, 0,1,4,4,1,5, 7,6,3,3,6,2, 3,2,0,0,2,1, 0,4,3,3,4,7, 5,1,6,6,1,2 };
		m_pBuffer = NEW RendBuffer( Device::RM_TRIANGLES);
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_STATIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pVertexBuffer->FlushVertexBuffer( VERTEX_COUNT, m_pVertexArray );
		m_pIndicesBuffer->FlushIndiesBuffer( sizeof(indies)/sizeof(ushort), indies );

		m_Texture = NEW TextureCube();
		std::string basepath = FILEPATH.isExist( f ) ? "" : FILEPATH.TextureFolder();
		if( isAlpha )
		{
			m_Texture->LoadTexture( Device::PF_R8G8B8A8, basepath+f, basepath+b, basepath+u, basepath+d, basepath+l, basepath+r);
		}
		else
		{
			m_Texture->LoadTexture( Device::PF_R8G8B8, basepath+f, basepath+b, basepath+u, basepath+d, basepath+l, basepath+r);
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	SkyBox::Box::~Box()
	{
		SAFE_DELETE( m_pVertexArray );
		SAFE_DELETE( m_pBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
		SAFE_DELETE( m_Texture );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SkyBox::Box::Render(Engine& engine, const Matrix44f* trans)
	{
		//static float time = 0; //旋转
		//time += static_cast<float>( engine.GetTimeSpan() * 0.001 );
		//Quaternionf ro;
		//ro.RotateAxis( vector3f( 1,0,0) , time * 0.01 * PI );
		//Matrix44f trm = Math::Quaternion_to_Matrix( ro );
		//trm = *trans * trm;
		SkyBoxShader& sbs = engine.GetShaderManage()->GetShader<SkyBoxShader>(ShaderManage::SkyBoxShader);
		sbs.SetParamAlpha( 0.0 );
		sbs.SetParamTransform(trans);
		engine.GetDevice()->Draw( *m_pBuffer, sbs, m_Texture );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SkyBox::Box::RenderRotation( Engine& engine, const Matrix44f* trans )
	{
		engine.GetDevice()->DisableDepthTest();
		engine.GetDevice()->DepthMask( false );
		m_Times += static_cast<float>( engine.GetTimeSpan() * 0.001 );
		Quaternionf ro;
		ro.RotateAxis( vector3f( 1,0,0) , m_Times * 0.005 * PI );
		m_LocalRotate = ro.ToMatrix44();
		m_LocalRotate = *trans * m_LocalRotate;
		SkyBoxShader& sbs = engine.GetShaderManage()->GetShader<SkyBoxShader>(ShaderManage::SkyBoxShader);
		sbs.SetParamTransform(&m_LocalRotate);
		sbs.SetParamAlpha( 1.0 );
		engine.GetDevice()->Draw( *m_pBuffer, sbs, m_Texture );
	}










	//--------------------------------------------------------------------------------------------------------------------------------------
	SkyBox::SkyBox()
		:m_pBackground(NULL)
	{
		m_pTransform = NEW Matrix44f();
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	SkyBox::~SkyBox()
	{
		SAFE_DELETE(m_pTransform);
		SAFE_DELETE(m_pBackground);
		for( SkyBoxList::iterator it = m_Front.begin();
			it != m_Front.end();
			it ++)
		{
			SAFE_DELETE(*it);
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SkyBox::AndSkyBox(POS pos, const std::string& f, const std::string& b, const std::string& u, const std::string& d, const std::string& l, const std::string& r )
	{
		switch( pos )
		{
		case BACKGROUND: SAFE_DELETE(m_pBackground); m_pBackground = NEW Box( false, f, b, u, d, l, r ); break;
		case FRONT: m_Front.push_back( NEW Box( true, f, b, u, d, l, r ) ); break;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SkyBox::ClearAll()
	{
		SAFE_DELETE(m_pBackground);
		for( SkyBoxList::iterator it = m_Front.begin();
			it != m_Front.end();
			it ++)
		{
			SAFE_DELETE(*it);
		}
		m_Front.clear();
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SkyBox::RenderBackground(Engine& engine)
	{
		if( NULL != m_pBackground )
		{
			engine.GetDevice()->DepthMask( false );
			engine.GetDevice()->DisableAlpha();
			engine.GetDevice()->EnableDepthTest();
			m_pBackground->Render(engine, m_pTransform);
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SkyBox::RenderSkyBox(Engine& engine)
	{
		if ( !m_Front.empty() )
		{
			engine.GetDevice()->DepthMask( false );
			engine.GetDevice()->EnableDepthTest();
			engine.GetDevice()->EnableAlphaBlend(Device::BL_SRC_ALPHA, Device::BL_ONE_MINUS_SRC_ALPHA, Device::BL_ONE, Device::BL_ONE );
			for( SkyBoxList::iterator it = m_Front.begin();
				it != m_Front.end();
				it ++)
			{
				(*it)->RenderRotation(engine, m_pTransform);
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SkyBox::Rotate(const vector3f& vector , const float radian)
	{
		*m_pTransform = *m_pTransform * Matrix44f::CreateRotateMatrix(vector, radian);
	}
}