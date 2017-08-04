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
#include "Framework/FunctionRegister.h"
#include "Framework/ScriptUtility.h"
#include "Framework/IScriptSystem.h"
#include "Framework/ClassRegister.h"
#include "Framework/Framework.h"
#include "Framework/3DMath.h"
#include <EASTL/map.h>



#define _PRINT_LUA_CLASS_INFO

#ifdef _PRINT_LUA_CLASS_INFO
typedef eastl::map< uint, BohgeEngine::ClassRegister* > ScriptClassMap;
static bool isNotOperater( const eastl::string& name )
{
	if ( name == BohgeEngine::ScriptProperty::__ADD
		|| name == BohgeEngine::ScriptProperty::__SUB
		|| name == BohgeEngine::ScriptProperty::__MUL
		|| name == BohgeEngine::ScriptProperty::__DIV
		|| name == BohgeEngine::ScriptProperty::__CON
		|| name == BohgeEngine::ScriptProperty::__EQU
		|| name == BohgeEngine::ScriptProperty::__LES
		|| name == BohgeEngine::ScriptProperty::__LEQ )
	{
		return false;
	}
	return true;
}
static void PrintScriptClassInfo( ScriptClassMap& classes )
{
	static bool call = true;
	if ( call )
	{
		call = false;
		eastl::string res;
		res += "return {\r\n";
		FOR_EACH( ScriptClassMap, it, classes )
		{
			res += it->second->GetClassTypeName();
			res += " = {\r\n";
			if ( it->second->isDriverType() )
			{
				res += "\tinherits = \"";
				res += it->second->GetBaseClassTypeName();
				res += "\",\r\n";
			}
			res += "\tdescription = \"\",\r\n\ttype = \"class\",\r\n\tchilds = {\r\n";
			if (	it->second->GetClassTypeID() == BohgeEngine::Bool::RTTI().GetTypeID()
				||  it->second->GetClassTypeID() == BohgeEngine::Number::RTTI().GetTypeID()
				||  it->second->GetClassTypeID() == BohgeEngine::String::RTTI().GetTypeID() )
			{
				res += "\t\tData = {\r\n\t\t\ttype = \"function\",\r\n\t\t\tdescription = \"\",\r\n";
				if ( it->second->GetClassTypeID() == BohgeEngine::Number::RTTI().GetTypeID() )
				{
					res += "\t\t\treturns = \"(:number)\"\r\n\t\t}\r\n";
				}
				else if( it->second->GetClassTypeID() == BohgeEngine::Bool::RTTI().GetTypeID() )
				{
					res += "\t\t\treturns = \"(:boolean)\"\r\n\t\t}\r\n";
				}
				else if( it->second->GetClassTypeID() == BohgeEngine::String::RTTI().GetTypeID() )
				{
					res += "\t\t\treturns = \"(:string)\"\r\n\t\t}\r\n";
				}
			}
			else
			{
				{//处理定值
					uint constCount = it->second->GetClassConstantCount();
					for ( int i = 0 ; i < constCount ; ++ i )
					{
						res += "\t\t";
						res += it->second->GetClassConstant( i ).first;
						res += " = {\r\n\t\t\t\description = \"\",\r\n\t\t\ttype = \"value\",\r\n\t\t},\r\n";
					}
				}
				{//处理成员函数
					uint funcCount = it->second->GetFunctionListCount();
					for ( int i = 0 ; i < funcCount ; ++ i )
					{
						if( isNotOperater( it->second->GetFunctionListName(i) ) )
						{
							res += "\t\t";
							res += it->second->GetFunctionListName(i) + " = {\r\n\t\t\ttype = \"function\",\r\n\t\t\tdescription = \"\",\r\n";
							{//处理函数参数
								res += "\t\t\targs = \"";
								const BohgeEngine::OverlappedMap& functions = it->second->GetFunctions( i );
								FOR_EACH_CONST( BohgeEngine::OverlappedMap::iterator, funcArray, functions )
								{
									FOR_EACH_CONST( BohgeEngine::FunctionArray, single, funcArray->second )
									{
										res += "(";
										uint argCount = (*single)->GetArgumentCount();
										for ( int j = 0 ; j < argCount ; ++ j )
										{
											res += ":";
											res += (*single)->GetArguments(j)->GetTypeName();
											if ( j != argCount - 1 )
											{
												res += ",";
											}
										}
										res += ")";
									}
								}
								res += "\",\r\n";
							}//处理函数参数
							{//处理函数返回值
								bool isNoReturn = true;
								const BohgeEngine::OverlappedMap& functions = it->second->GetFunctions( i );
								res += "\t\t\treturns = \"";
								FOR_EACH_CONST( BohgeEngine::OverlappedMap::iterator, funcArray, functions )
								{
									FOR_EACH_CONST( BohgeEngine::FunctionArray, single, funcArray->second )
									{
										if ( (*single)->hasReturn() )
										{
											isNoReturn = false;
											res += "(:";
											res += (*single)->GetReturnType()->GetTypeName();
											res += ")";
										}
									}
								}
								if ( isNoReturn )
								{
									res += "()\",\r\n";
								}
								else
								{
									res += "\",\r\n";
								}
							}
							res += "\t\t},\r\n";
						}
					}
				}
			}
			res += "\t}\r\n},\r\n";
		}
		res += "}";
	}
}
#endif