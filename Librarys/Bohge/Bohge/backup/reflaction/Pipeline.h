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
//		游戏中渲染管线	   //
/////////////////////////////

#pragma once
#include "3DMath.h"
#include "VariableType.h"
#include <list>


class RenderTarget;
class IPostEffect;
class IDriver;
class Texture;
class RendBuffer;
class CopyCurrentScreen;
class Pipeline//渲染管线管理
{
public:
	enum PassType//为延迟渲染做准备
	{
		PT_LIGHTING,//普通的光照
		PT_DEPTH,//线性深度绘制
		PT_COUNT,
	};
private:
	enum Const
	{
		COUNT_NODE			= 1,
		COUNT_OF_VERTEX		= 4,
		COUNT_OF_INDIES		= 4,
	};
private:
	typedef std::list<IPostEffect*> PostEffectList;
private:
	bool				m_isPostEffectActive;				//是否使用PE
	PassType			m_CurrentPassType;//当前的渲染状态
	RendBuffer*			m_pRendBuffer;
	PostEffectList		m_PostList;
	RenderTarget*		m_pOriginalColorDepthStencil;		//原始的绘制图像
	//RenderTarget*		m_pBottomUISkyBoxColor;				//原始的绘制的背景渲染目标
	CopyCurrentScreen*	m_pCopyer;							//拷贝当前的屏幕数据
	RenderTarget*		m_pPreviousSceneColor[2];			//之前的处理后的结果图,需要交替
	RenderTarget*		m_pReflection;						//反射纹理
#ifdef IOS
	RenderTarget*		m_EndSecen;	//苹果ios需要最后将一张有效地rt绑定，然后绘制次rt
public:
	void SetRendTargetHandle( handle h, const vector2d& size );
    BOHGE_FORCEINLINE RenderTarget* IosEndSecen()
    {
        return m_EndSecen;
    }
#endif
public:
	Pipeline(void);
	~Pipeline(void);
	void Render(IDriver& driver); //开始渲染管线
	void RemovePostEffect( IPostEffect* ipe );
	void MakeCopyScreen();//拷贝当前主FBO
	Texture* GetCopyColorTexture();
	Texture* GetCopyDepthTexture();
	Texture* GetReflection();
	void OnSizeChange( const vector2d& size );
public:
	BOHGE_FORCEINLINE void AddPostEffcet( IPostEffect* ipe )
	{
		m_PostList.push_back(ipe);
	}
	BOHGE_FORCEINLINE void Clear()
	{
		m_PostList.clear();
	}
	BOHGE_FORCEINLINE void SetPostEffectActive(bool active)
	{
		m_isPostEffectActive = active;
	}
	BOHGE_FORCEINLINE const RenderTarget* OriginalRenderTarget()
	{
		return m_pOriginalColorDepthStencil;
	}
	BOHGE_FORCEINLINE const PassType& CurrentPassType()
	{
		return m_CurrentPassType;
	}

public:
	/*
	特殊的还是，在正常渲染过程中不需要调用，
	在地形编辑器中需要输出地形的影音，
	我们需要画出深度图，这个时候需要改变渲染状态
	如果需要调用请慎重，调用完后回复先前的pt
	*/
	BOHGE_FORCEINLINE void CostumPassType( const PassType& pt )
	{
		m_CurrentPassType = pt;
	}
};
