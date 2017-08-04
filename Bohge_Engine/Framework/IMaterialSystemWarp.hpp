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
#include "IMaterialSystem.h"
#include "FunctionRegister.h"
#include "ClassRegister.h"
#include "ScriptValue.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_IMaterialSystem( IScriptSystem& sys )
	{

		{
			ClassRegister sc;
			sc.ClassType( &IMaterialSystem::RTTI() );

			sys.RegisterClass(sc);
		}

		{
			ClassRegister sc;
			sc.ClassType( &MaterialProperty::RTTI() );

			sc.ClassConstant( MaterialProperty::UPT_VEC1, "UPT_VEC1" );
			sc.ClassConstant( MaterialProperty::UPT_VEC2, "UPT_VEC2" );
			sc.ClassConstant( MaterialProperty::UPT_VEC3, "UPT_VEC3" );
			sc.ClassConstant( MaterialProperty::UPT_VEC4, "UPT_VEC4" );
			sc.ClassConstant( MaterialProperty::UPT_MAT3, "UPT_MAT3" );
			sc.ClassConstant( MaterialProperty::UPT_MAT4, "UPT_MAT4" );


			sc.ClassConstant( MaterialProperty::ATT_AMBIENT, "ATT_AMBIENT" );
			sc.ClassConstant( MaterialProperty::ATT_DIFFUSE, "ATT_DIFFUSE" );
			sc.ClassConstant( MaterialProperty::ATT_SPECULAR, "ATT_SPECULAR" );
			sc.ClassConstant( MaterialProperty::ATT_EMIT, "ATT_EMIT" );
			sc.ClassConstant( MaterialProperty::ATT_OPCITY, "ATT_OPCITY" );
			sc.ClassConstant( MaterialProperty::ATT_SPECULAR_LEVEL, "ATT_SPECULAR_LEVEL" );
			sc.ClassConstant( MaterialProperty::ATT_SHININESS, "ATT_SHININESS" );

			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_AMBIENT, "ATT_TEXTURE_AMBIENT" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_DIFFUSE, "ATT_TEXTURE_DIFFUSE" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_SPECULAR, "ATT_TEXTURE_SPECULAR" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_SPLEVEL, "ATT_TEXTURE_SPLEVEL" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_GLOSSINESS, "ATT_TEXTURE_GLOSSINESS" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_ILLUMINATION, "ATT_TEXTURE_ILLUMINATION" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_OPACITY, "ATT_TEXTURE_OPACITY" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_FITER, "ATT_TEXTURE_FITER" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_BUMP, "ATT_TEXTURE_BUMP" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_REFLECTION, "ATT_TEXTURE_REFLECTION" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_REFRACTION, "ATT_TEXTURE_REFRACTION" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_DISPLACEMENT, "ATT_TEXTURE_DISPLACEMENT" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_TERRAIN_WEIGHT, "ATT_TEXTURE_TERRAIN_WEIGHT" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_TERRAIN_DETAL, "ATT_TEXTURE_TERRAIN_DETAL" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_TERRAIN_BASE, "ATT_TEXTURE_TERRAIN_BASE" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_TERRAIN_R, "ATT_TEXTURE_TERRAIN_R" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_TERRAIN_G, "ATT_TEXTURE_TERRAIN_G" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_TERRAIN_B, "ATT_TEXTURE_TERRAIN_B" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_USER0, "ATT_TEXTURE_USER0" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_USER1, "ATT_TEXTURE_USER1" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_USER2, "ATT_TEXTURE_USER2" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_USER3, "ATT_TEXTURE_USER3" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_USER4, "ATT_TEXTURE_USER4" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_USER5, "ATT_TEXTURE_USER5" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_USER6, "ATT_TEXTURE_USER6" );
			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_USER7, "ATT_TEXTURE_USER7" );

			sc.ClassConstant( MaterialProperty::ATT_TEXTURE_LIGHT_MAP, "ATT_TEXTURE_LIGHT_MAP" );

			sc.ClassConstant( MaterialProperty::ATT_MATERIAL_USE_0, "ATT_MATERIAL_USE_0" );
			sc.ClassConstant( MaterialProperty::ATT_MATERIAL_USE_1, "ATT_MATERIAL_USE_1" );
			sc.ClassConstant( MaterialProperty::ATT_MATERIAL_USE_2, "ATT_MATERIAL_USE_2" );
			sc.ClassConstant( MaterialProperty::ATT_MATERIAL_USE_3, "ATT_MATERIAL_USE_3" );
			sc.ClassConstant( MaterialProperty::ATT_MATERIAL_USE_4, "ATT_MATERIAL_USE_4" );
			sc.ClassConstant( MaterialProperty::ATT_MATERIAL_USE_5, "ATT_MATERIAL_USE_5" );
			sc.ClassConstant( MaterialProperty::ATT_MATERIAL_USE_6, "ATT_MATERIAL_USE_6" );
			sc.ClassConstant( MaterialProperty::ATT_MATERIAL_USE_7, "ATT_MATERIAL_USE_7" );
			sc.ClassConstant( MaterialProperty::ATT_MATERIAL_USE_8, "ATT_MATERIAL_USE_8" );
			sc.ClassConstant( MaterialProperty::ATT_MATERIAL_USE_9, "ATT_MATERIAL_USE_9" );

			sys.RegisterClass(sc);
		}
	}
}