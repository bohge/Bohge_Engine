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
#include "Framework/JsonDefualtDataType.h"



namespace BohgeHub
{
	//服务器通信资源不需要版本号，服务器会根据不同的版本发送不同的信息
	struct JsonResourceInfo : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonResourceInfo, 3 );
		eastl::string		m_LoacalPath;//资源本地地址
		eastl::string		m_ResUrl;//资源网络位置，提供下载
		uint				m_Version;//资源版本号
	};
	JSON_ARRAY_DATA_STRUCT( JsonResourceInfo );
	struct JsonEntryInfo : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonEntryInfo, 4 );
		JsonResourceInfo_Array	m_ResinfoArray;//入口程序资源列表
		eastl::string			m_BaseUrl;//入口资源网址
		eastl::string			m_Bundle;//程序名称
		bool					m_isValid;//是否有效版本号
	};

	//本地文件信息
	struct JsonLocalResourceInfo : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonLocalResourceInfo, 2 );
		eastl::string	m_LoacalPath;//哈西吗
		uint	m_Version;//资源版本号
	};
	JSON_ARRAY_DATA_STRUCT( JsonLocalResourceInfo );
	struct JsonLocalResourceList : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonLocalResourceList, 1 );
		JsonLocalResourceInfo_Array	m_ResinfoArray;//入口程序资源列表
	};
}