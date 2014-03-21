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
#include "3DMath.h"
#include <string>
#include <map>



namespace BohgeEngine
{
	class SkyBox;
	class Engine;
	class Color;
	class Environment;
	class IShadowable;
	class IBeShadowable;
	class ISceneNode;
	class SceneMain;
	class ICamera;
    class IAttributeNode;
	class LocatableObject;
	class SceneManager
	{
		friend class Emitter;
		friend class ISceneNode;
		friend class IAttributeNode;
	public:
		typedef std::list< ISceneNode* > SceneNodeList;	//场景节点
	private:
		typedef std::map<int, IAttributeNode* > NeedReset;//需要重置的
		typedef std::multimap<float, ISceneNode* > NodeMultimap;//需要绘制的
		typedef std::multimap<ushort, ISceneNode* > NodeQueue;//需要绘制的
		typedef std::multimap<float, ISceneNode*, std::greater<float> > TransparentNodeMultimap;//需要绘制的透明物体
	private:
		SkyBox*						m_pSky;
		//ListShadowable			m_lShadowable;
		//ListBeShadowable			m_lBeShadowable;
		NeedReset					m_AttributeMap;
		SceneNodeList				m_SceneNodeList;
		//排序Updata
		NodeQueue					m_UpdataQueue;
		//剔除相关的
		NodeMultimap				m_NeedRenders;//需要绘制的，没有被剔除的
		TransparentNodeMultimap		m_Transparent;//需要绘制的透明的物体
		//编辑器的场景
		SceneMain*			m_pWorld;
	public:
		explicit SceneManager();
		~SceneManager();
	public:
		void BeginUpdate( Engine& engine );
		void Update( uint milisecond );//更新场景，控制是否绘制啊，升级骨骼动画绑定啊
		void EndUpdate( Engine& engine );
		void RenderSecen(Engine& engine);
		void RenderNormal(Engine& engine);
	public:
		void AssignMaterial( const bool& isCast, const ICamera& camera );//通过阴影摄像机确定渲染每一块的材质
		//void RenderShadow(engine& engine);
		//void RemoveShadowObject(IShadowable* input);
		//void RemoveBeShadowObject(IBeShadowable* input);
		void LoadScene( const std::string& path );//读取一个场景
		bool OnCameraUpdate( ICamera& camera );
		const LocatableObject* GetLoadedObject( const std::string& Tilename );
	private:
		BOHGE_FORCEINLINE void AttachObject(ISceneNode* rn)
		{
			m_SceneNodeList.push_back( rn );
		}
		BOHGE_FORCEINLINE void RemoveObject(ISceneNode* rn)
		{
			m_SceneNodeList.remove(rn);
		}
		BOHGE_FORCEINLINE void AttachNode(IAttributeNode* rn)
		{
			m_AttributeMap.insert( std::make_pair( (int)rn, rn ) );
		}
		BOHGE_FORCEINLINE void RemoveNode(IAttributeNode* rn)
		{
			m_AttributeMap.erase( (int)rn );
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
		BOHGE_FORCEINLINE SceneMain& GetWorld()
		{
			return *m_pWorld;
		}
		BOHGE_FORCEINLINE SceneNodeList& GetSceneList()
		{
			return m_SceneNodeList;
		}
	};
}