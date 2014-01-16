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



/////////////////////////////////////
//			游戏中资源的列表	   //
/////////////////////////////////////
#pragma once
#include <string>


namespace BohgeGame
{

	//全部的声音的索引必须是唯一的
	enum SoundName
	{
		sou_Button,
		sou_Slider,

		Sound_Count,
	};

	static const char* SoundFileName[] =
	{
		"Button.ogg",
		"Slider.ogg",
	};


	//全部的纹理的索引必须是唯一的
	enum TextureName
	{
		tex_Logo,
		tex_Loading,

		Texture_Count,
	};

	static const char* TextureFileName[] =
	{
		"logos.png",
		"Loading.png",
	};


	//全部的纹理的索引必须是唯一的
	enum UIName
	{
		ui_StudioLogo,
		ui_EngineLogo,
		ui_Loading,

		UI_Count,
	};

	static const char* UIFileName[] =
	{
		"StudioLogo.ui",
		"EngineLogo.ui",
		"Loading.ui",
	};


	enum ModelName
	{
		Model_Resouces_Count = 0,
	};

	static const char* ModelFileName[] = //模型是以文件夹的形式整体保存的
	{
		"None",
		//"Ice", //读取是的模型字段,之后跟随它包含的资源
		//	"Ice/Ice.BMODEL",//这种方式还是不好，以后要找一个方便的方法
		//	"Ice/IceDiffuse.png",
		//"Stone",// id = 3
		//	"Stone/Stone.BMODEL",
		//	"Stone/StoneDiffuse.png",
		//end id = 6
	};

}