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
#include "RenderTargetWarp.hpp"
#include "RenderObjectWarp.hpp"
#include "StaticCameraWrap.hpp"
#include "PathMetadataWarp.hpp"
#include "PostEffectWarp.hpp"
#include "FrameworkWrap.hpp"
#include "ICameraWarp.hpp"
#include "NodeWarp.hpp"

//动作
#include "IActionSystemWarp.hpp"

//环境
#include "IEnvironmentSystemWarp.hpp"

//渲染API
#include "IGraphicSystemWarp.hpp"

//材质
#include "IMaterialSystemWarp.hpp"

//内存
#include "IMemorySystemWarp.hpp"

//文件
#include "IOSystemWrap.hpp"

//物理
#include "IPhysicSystemWarp.hpp"

//管线
#include "IPipelineSystemWarp.hpp"

//渲染队列
#include "IRenderQueueSystemWarp.hpp"

//资源系统
#include "IResourceSystemWarp.hpp"

//脚本
#include "IScriptSystemWrap.hpp"
#include "IFunctionInvokerWrap.hpp"

//声音
#include "ISoundSystemWrap.hpp"

//文本
#include "ITextSystemWarp.hpp"

//线程
#include "IThreadSystemWrap.hpp"

//时间
#include "ITimeSystemWarp.hpp"

//网络
#include "IDownloadHandleWarp.hpp"
#include "IPostHandleWarp.hpp"
#include "IBaseHandleWarp.hpp"
#include "IWebSystemWarp.hpp"


//组建类
#include "ComponentWarp.hpp"
#include "ScriptComponentWarp.hpp"

//基础库
#include "QuaternionWrap.hpp"
#include "Matrix33Wrap.hpp"
#include "Matrix44Wrap.hpp"
#include "Vector1Wrap.hpp"
#include "Vector2Wrap.hpp"
#include "Vector3Wrap.hpp"
#include "Vector4Wrap.hpp"


namespace BohgeEngine
{
	static void DoRegister( IScriptSystem& sys )
	{
		//框架
		Register_ICamera( sys );
		Register_Framework( sys );

		//摄像机
		Register_StaticCamera( sys );
		//动作
		Register_IActionSystem(sys);

		//环境
		Register_IEnvironmentSystem(sys);

		//渲染API
		Register_IGraphicSystem(sys);

		//材质
		Register_IMaterialSystem( sys );

		//内存
		Register_IMemorySystem( sys );

		//文件
		Register_IIOSystem( sys );

		//物理
		Register_IPhysicSystem( sys );

		//管线
		Register_IPipelineSystem(sys );

		//渲染队列
		Register_IRenderQueueSystem(sys);

		//资源系统
		Register_IResourceSystem(sys);

		//脚本
		Register_IScriptSystem(sys);
		Register_IFunctionInvoker(sys);

		//声音
		Register_ISoundSystem(sys);

		//文本
		Register_ITextSystem( sys );

		//线程
		Register_IThreadSystem( sys );

		//时间
		Register_ITimeSystem( sys );

		//web系统
		Register_IBaseHandle( sys );
		Register_IPostHandle( sys );
		Register_IDownloadHandle( sys );
		Register_IWebSystem( sys );

		//文件系统
		Register_IIOSystem( sys );

		//组建
		Register_Node( sys );
		Register_IPostEffect( sys );
		Register_Component( sys );
		Register_ScriptComponent( sys );

		//实体
		Register_PathMetadata( sys );

		//渲染
		Register_RenderObjectEntity( sys );
		Register_RenderTargetEntity( sys );

		//数学库
		Register_vector1( sys );
		Register_vector2( sys );
		Register_vector3( sys );
		Register_vector4( sys );
		Register_Quaternion( sys );
		Register_Matrix33(sys);
		Register_Matrix44(sys);

	}
}