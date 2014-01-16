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
//		实体团体			//
/////////////////////////

#pragma once
#include "ObjectDataIndex.h"
#include "LocatableObject.h"
#include <string>
#include <vector>


namespace BohgeEngine
{
	struct ObjectVersion
	{
		enum
		{
			ObjectVersion1 = 10001,//第一个版本
		};
	};

	class ObjectDataInfo
	{
	private:
		const ObjectGlobelDefined::Type			m_Type;
		IAttributeNode*							m_ObjectPtr;
	public:
		ObjectDataInfo( ObjectGlobelDefined::Type ot, IAttributeNode* ptr )
			:m_Type(ot),
			m_ObjectPtr(ptr)
		{
		}
		~ObjectDataInfo()
		{
			SAFE_DELETE( m_ObjectPtr );
		}
		BOHGE_FORCEINLINE const ObjectGlobelDefined::Type& GetType() const
		{
			return m_Type;
		}
		BOHGE_FORCEINLINE IAttributeNode* GetObjectPtr()
		{
			return m_ObjectPtr;
		}
	};

	//实体的包
	class Serializer;
	class IAttributeNode;
	class ObjectGroup : public ISceneNode, public LocatableObject
	{
	private:
		typedef std::vector<ObjectDataInfo*>		ObjectVector;
	private:
		std::string				m_Name;
		ObjectVector			m_ObjectVector;
		aabbox3df				m_TransedBinBox;//加速，防止每帧得矩阵乘向量
	public:
		ObjectGroup( );
		~ObjectGroup( );
	private:
		void _LoadObjectVersion_1( Serializer& loader, const std::string& path );
		IAttributeNode* _LoadModel( const std::string& path );
		IAttributeNode* _LoadEmitter( const std::string& path );
		IAttributeNode* _LoadLight( const std::string& path );
	public:
		void LoadObject( const std::string& path );
		void LoadLightMap( int modindex, int matindex, const std::string& path );
	public:
		virtual void Update( uint milisecon );
		virtual bool isInview( const Frustumf& cFru );
		virtual const void* Attribute(const Node_Attribute& name) const;
	public:
		BOHGE_FORCEINLINE uint GetObjectCount()
		{
			return m_ObjectVector.size();
		}
		BOHGE_FORCEINLINE ObjectDataInfo& GetObject( uint i )
		{
			return *m_ObjectVector[i];
		}
		BOHGE_FORCEINLINE const std::string& GetName()
		{
			return m_Name;
		}
		BOHGE_FORCEINLINE const aabbox3df& WorldBindBox()
		{
			return m_TransedBinBox;
		}
	};
}
