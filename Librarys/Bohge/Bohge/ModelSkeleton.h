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



///////////////////////////////
//		模型相关shader		 //
///////////////////////////////
#pragma once

#include "BaseShader.h"


namespace BohgeEngine
{
	//---------------------------------------------------------------------
	class SkeletonModelBase : public MaterialShader
	{
	private:
		handle			m_hViewProjMatrixLoc;
		handle			m_hNormalTransform;
		handle			m_hLightDirectionLoc;
		handle			m_hCameraPositionLoc;

		handle			m_hRealArray;//骨骼动画序列
		handle			m_hDualArray;

		handle			m_hTransform;
		handle			m_hTextureBase;

		handle			m_hAmbient_AmbientLight;
		handle			m_hSpecular_Light;
		handle			m_hLight;
		handle			m_hShininess;
	protected:
		virtual void _SetParameters();
	private:
		virtual bool _CreateShader() = 0;
	public:
		SkeletonModelBase();
		virtual ~SkeletonModelBase(){}
		virtual bool Initialization();
	};



	//---------------------------------------------------------------------
	class SkeletonModelBumpBase : public MaterialShader
	{
	private:
		handle			m_hViewProjMatrixLoc;
		handle			m_hNormalTransform;
		handle			m_hLightDirectionLoc;
		handle			m_hCameraPositionLoc;

		handle			m_hRealArray;//骨骼动画序列
		handle			m_hDualArray;

		handle			m_hTransform;
		handle			m_hTextureBase;
		handle			m_hTextureBump;

		handle			m_hAmbient_AmbientLight;
		handle			m_hSpecular_Light;
		handle			m_hLight;
		handle			m_hShininess;
	protected:
		virtual void _SetParameters();
	private:
		virtual bool _CreateShader() = 0;
	public:
		SkeletonModelBumpBase();
		virtual ~SkeletonModelBumpBase(){};
		virtual bool Initialization();
	};




	//---------------------------------------------------------------------
	class SkeletonModelDepthBase : public MaterialShader
	{
	private:
		handle			m_hWorldViewProjMatrixLoc;
		handle			m_hWorldTransform;
		handle			m_hLightPositionLoc;
		handle			m_hCameraLineDepthParam;
		handle			m_hRealArray;//骨骼动画序列
		handle			m_hDualArray;
	protected:
		virtual void _SetParameters();
	private:
		virtual bool _CreateShader() = 0;
	public:
		SkeletonModelDepthBase();
		virtual ~SkeletonModelDepthBase(){};
		virtual bool Initialization();
	};


	//---------------------------------------------------------------------
	class SkeletonModelNormalBase : public MaterialShader
	{
	private:
		handle			m_hNormalTransform;
		handle			m_hWorldViewProjMatrixLoc;
		handle			m_hRealArray;//骨骼动画序列
		handle			m_hDualArray;
	protected:
		virtual void _SetParameters();
	private:
		virtual bool _CreateShader() = 0;
	public:
		SkeletonModelNormalBase();
		virtual ~SkeletonModelNormalBase(){};
		virtual bool Initialization();
	};



	//实体的模型shader
	//---------------------------------------------------------------------
	class SkeletonModel1B : public SkeletonModelBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModel2B : public SkeletonModelBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModel3B : public SkeletonModelBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModel4B : public SkeletonModelBase
	{
	private:
		virtual bool _CreateShader();
	};



	//---------------------------------------------------------------------
	class SkeletonModelBump1B : public SkeletonModelBumpBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModelBump2B : public SkeletonModelBumpBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModelBump3B : public SkeletonModelBumpBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModelBump4B : public SkeletonModelBumpBase
	{
	private:
		virtual bool _CreateShader();
	};



	//---------------------------------------------------------------------
	class SkeletonModelDepth1B : public SkeletonModelDepthBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModelDepth2B : public SkeletonModelDepthBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModelDepth3B : public SkeletonModelDepthBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModelDepth4B : public SkeletonModelDepthBase
	{
	private:
		virtual bool _CreateShader();
	};





	//---------------------------------------------------------------------
	class SkeletonModelNormal1B : public SkeletonModelNormalBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModelNormal2B : public SkeletonModelNormalBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModelNormal3B : public SkeletonModelNormalBase
	{
	private:
		virtual bool _CreateShader();
	};

	//---------------------------------------------------------------------
	class SkeletonModelNormal4B : public SkeletonModelNormalBase
	{
	private:
		virtual bool _CreateShader();
	};
}