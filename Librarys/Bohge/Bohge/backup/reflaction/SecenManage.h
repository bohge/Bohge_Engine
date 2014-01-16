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
//		绘制管理	   //
/////////////////////////

#pragma once

#include <list>
#include <vector>
#include "3DMath.h"
#include <string>
#include <map>

class SkyBox;
class IDriver;
class Color;
class Environment;
class IShadowable;
class IBeShadowable;
class IRenderNode;
class SceneMain;
class ITransparent;
class IUpdateNode;
class EmitterManager;
class SecenManage
{
	friend class Emitter;
	friend class IUpdateNode;
	friend class IRenderNode;
private:
	typedef std::list< IRenderNode* > ListRenderable;	//可绘制的节点
	typedef std::list< IUpdateNode* > ListUpdateable;	//需要更新的节点
	//typedef std::list<IShadowable*> ListShadowable;	//可产生阴影的节点
	//typedef std::list<IBeShadowable*> ListBeShadowable;	//可被阴影影响的节点
	typedef std::vector<IRenderNode*> NeedRenders;//需要绘制的
private:
	SkyBox*				m_pSky;
	//ListShadowable		m_lShadowable;
	//ListBeShadowable	m_lBeShadowable;
	ListRenderable		m_lRenderable;
	ListUpdateable		m_lUpdateable;

	//剔除相关的
	NeedRenders			m_NeedRenders;//需要绘制的，没有被剔除的
	NeedRenders			m_ShadowRenders;//需要绘制的产生阴影的
	//编辑器的场景
	SceneMain*			m_pWorld;
	//粒子
	EmitterManager*		m_pEmitters;
	//没有深度的
	std::map<float, ITransparent*, std::greater<float> >		m_TransparentMap;
public:
	SecenManage();
	~SecenManage();
	void RenderBackground(IDriver& driver);
	void RenderSkyBox(IDriver& driver);
	void OnBeginUpdate( IDriver& driver );
	void Update( uint milisecond );//更新场景，控制是否绘制啊，升级骨骼动画绑定啊
	void Render(IDriver& driver);
	void RenderDepth(IDriver& driver);
	void RenderReflection(IDriver& driver);
	//void RenderShadow(IDriver& driver);
	//void RemoveShadowObject(IShadowable* input);
	//void RemoveBeShadowObject(IBeShadowable* input);
	void LoadScene( const std::string& path );//读取一个场景
	float GetWaterMiddleHeight();
private:
	BOHGE_FORCEINLINE EmitterManager& GetEmitterManage()
	{
		return *m_pEmitters;
	}
private:
	BOHGE_FORCEINLINE void AttachObject(IRenderNode* rn)
	{
		m_lRenderable.push_back( rn );
	}
	BOHGE_FORCEINLINE void RemoveObject(IRenderNode* rn)
	{
		m_lRenderable.remove(rn);
	}
	BOHGE_FORCEINLINE void AttachObject(IUpdateNode* rn)
	{
		m_lUpdateable.push_back( rn );
	}
	BOHGE_FORCEINLINE void RemoveObject(IUpdateNode* rn)
	{
		m_lUpdateable.remove(rn);
	}
public:
	//BOHGE_FORCEINLINE void AddShadowObject(IShadowable* input)
	//{
	//	m_lShadowable.push_back(input);
	//}
	//BOHGE_FORCEINLINE void AddBeShadowObject(IBeShadowable* input)
	//{
	//	m_lBeShadowable.push_back(input);
	//}
	BOHGE_FORCEINLINE SkyBox& GetSkyBox()
	{
		return *m_pSky;
	}
	BOHGE_FORCEINLINE bool isCastShadow()
	{
		return !m_ShadowRenders.empty();
	}
	BOHGE_FORCEINLINE SceneMain& GetWorld()
	{
		return *m_pWorld;
	}
};
