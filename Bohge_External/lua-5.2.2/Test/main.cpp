extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "string.h"

#include <string>



void print_error(lua_State *L, const char* fmt, ...);
static void call_stack(lua_State* L, int n);
int on_error(lua_State *L);
void dostring(lua_State *L, const char* buff);
void dobuffer(lua_State *L, const char* buff, size_t len);


static void call_stack(lua_State* L, int n)
{
	lua_Debug ar;
	if(lua_getstack(L, n, &ar) == 1)
	{
		lua_getinfo(L, "nSlu", &ar);

		const char* indent;
		if(n == 0)
		{
			indent = "->\t";
			print_error(L, "\t<call stack>");
		}
		else
		{
			indent = "\t";
		}

		if(ar.name)
			print_error(L, "%s%s() : line %d [%s : line %d]", indent, ar.name, ar.currentline, ar.source, ar.linedefined);
		else
			print_error(L, "%sunknown : line %d [%s : line %d]", indent, ar.currentline, ar.source, ar.linedefined);

		call_stack(L, n+1);
	}
}


void print_error(lua_State *L, const char* fmt, ...)
{
	char text[4096];

	va_list args;
	va_start(args, fmt);
	vsprintf(text, fmt, args);
	va_end(args);

	lua_getglobal(L, "_ALERT");
	if(lua_isfunction(L, -1))
	{
		lua_pushstring(L, text);
		lua_call(L, 1, 0);
	}
	else
	{
		printf("%s", text);
		lua_pop(L, 1);
	}
	printf("\n");
}

int on_error(lua_State *L)
{
	print_error(L, "%s", lua_tostring(L, -1));

	call_stack(L, 0);

	return 0;	
}

void dostring(lua_State *L, const char* buff)
{
	dobuffer(L, buff, strlen(buff));
}


void dobuffer(lua_State *L, const char* buff, size_t len)
{
	lua_pushcclosure(L, on_error, 0);
	int errfunc = lua_gettop(L);

	if(luaL_loadbuffer(L, buff, len, "lua_tinker::dobuffer()") == 0)
	{
		if(lua_pcall(L, 0, 0, errfunc) != 0)
		{
			lua_pop(L, 1);
		}
	}
	else
	{
		print_error(L, "%s", lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	lua_pop(L, 1);
}

void call(lua_State* L, const char* name)
{
	lua_pushcclosure(L, on_error, 0);
	int errfunc = lua_gettop(L);

	lua_getglobal(L,name);
	if(lua_isfunction(L,-1))
	{
		if(lua_pcall(L, 0, 1, errfunc) != 0)
		{
			lua_pop(L, 1);
		}
	}
	else
	{
		print_error(L, "lua_tinker::call() attempt to call global `%s' (not a function)", name);
	}

	lua_remove(L, -2);
}

void dofile(lua_State *L, const char *filename)
{
	lua_pushcclosure(L, on_error, 0);
	int errfunc = lua_gettop(L);

	if(luaL_loadfile(L, filename) == 0)
	{
		if(lua_pcall(L, 0, 0, errfunc) != 0)
		{
			lua_pop(L, 1);
		}
	}
	else
	{
		print_error(L, "%s", lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	lua_pop(L, 1);
}


#include "lua_tinker.hpp"
#include "vector2.hpp"
using namespace BohgeEngine;
int Destory(lua_State *L)
{

	return 0;
}
//-------------------------------------------------------------------------------------------------------
void RegisteClass( lua_State* L, const std::string& name, int id )
{
	lua_newtable(L);

	int count;

	//lua_pushstring(L, "__TEST_DATA");
	//count = lua_gettop(L);
	////void* test = lua_newuserdata( L, sizeof(vector2<float>) );
	////count = lua_gettop(L);
	////new(test) vector2<float>(5,1);
	//vector2<float>* test = new vector2<float>(5,1);
	//lua_pushlightuserdata(L,test);
	//lua_rawset(L, -3);

	//lua_pushstring(L, "__ID");
	//lua_pushnumber( L, id );
	//lua_rawset(L, -3);

	lua_pushstring(L, "__name");
	lua_pushstring(L, name.c_str());
	lua_rawset(L, -3);

	lua_pushstring(L, "__index");
	lua_pushcclosure(L, lua_tinker::meta_get, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcclosure(L, lua_tinker::meta_set, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__gc");
	lua_pushcclosure(L, &Destory, 0);
	lua_rawset(L, -3);

	lua_setglobal(L, name.c_str());
}
//-------------------------------------------------------------------------------------------------------
void RegisteClassFunc( lua_State* L, const std::string& cn, const std::string& fn, lua_CFunction func )
{
	lua_getglobal(L, cn.c_str());
	if(lua_istable(L, -1))
	{
		lua_pushstring(L, fn.c_str() );
		vector2<float>* test = new vector2<float>(15,15);
		//lua_pushstring(L, fn.c_str() );
		lua_pushlightuserdata(L,test);
		int count = lua_gettop(L);
		//lua_pushcfunction(L,func);
		lua_pushcclosure(L,func,1);
		count = lua_gettop(L);
		lua_rawset(L,-3);
	}
	lua_pop(L, 1);
}
//-------------------------------------------------------------------------------------------------------
void RegisteClassAdd( lua_State* L, const std::string& cn, lua_CFunction func )
{
	lua_getglobal(L, cn.c_str());
	if(lua_istable(L, -1))
	{
		lua_pushstring(L, "__add");
		lua_pushcclosure(L, func, 0);
		lua_rawset(L, -3);
	}
	lua_pop(L, 1);
}
//-------------------------------------------------------------------------------------------------------
void RegisteClassMul( lua_State* L, const std::string& cn, lua_CFunction func )
{
	lua_getglobal(L, cn.c_str());
	if(lua_istable(L, -1))
	{
		lua_pushstring(L, "__mul");
		lua_pushcclosure(L, func, 0);
		lua_rawset(L, -3);
	}
	lua_pop(L, 1);
}
//-------------------------------------------------------------------------------------------------------
void RegisteClassCon( lua_State* L, const std::string& cn, lua_CFunction confunc )
{
	lua_getglobal(L, cn.c_str());
	if(lua_istable(L, -1))
	{
		lua_newtable(L);
		lua_pushstring(L, "__call");
		//void* test = lua_newuserdata( L, sizeof(vector2<float>) );
		//count = lua_gettop(L);
		//new(test) vector2<float>(5,1);
		vector2<float>* test = new vector2<float>(5,1);
		lua_pushlightuserdata(L,test);
		lua_pushnumber( L, 1001 );
		lua_pushcclosure(L, confunc, 2);
		lua_rawset(L, -3);


		lua_setmetatable(L, -2);
	}
	lua_pop(L, 1);
}
//-------------------------------------------------------------------------------------------------------
template<typename T> int Vector2Con(lua_State *L)
{
	int count = lua_gettop(L);
	lua_getfield(L, 1,"__ID");
	int id = lua_tonumber(L, -1); // n should now be equal to 3
	lua_getfield(L, 1,"__TEST_DATA");
	void* testdata = lua_touserdata(L,-1);
	vector2<float>* vf = (vector2<float>*)testdata;
	//new(lua_newuserdata(L, sizeof(lua_tinker::val2user<vector2<T>>))) lua_tinker::val2user<vector2<T>>(lua_tinker::read<T>(L,2),lua_tinker::read<T>(L,3));
	void* ptr = lua_newuserdata(L, sizeof( vector2<T> ) );
	new( ptr ) vector2<T>( lua_tinker::read<T>(L,2),lua_tinker::read<T>(L,3) );
	vector2<T>* vp = (vector2<T>*)ptr;
	lua_getglobal(L, "vector2f");
	lua_setmetatable(L, -2);
	return 1; 
}
//-------------------------------------------------------------------------------------------------------
template<typename T> int Matrix22Con(lua_State *L)
{
	int count = lua_gettop(L);
	//new(lua_newuserdata(L, sizeof(lua_tinker::val2user<Matrix22<T>>))) lua_tinker::val2user<Matrix22<T>>(lua_tinker::read<T>(L,2),lua_tinker::read<T>(L,3),lua_tinker::read<T>(L,4),lua_tinker::read<T>(L,5));
	void* ptr = lua_newuserdata(L, sizeof( Matrix22<T> ) );
	Matrix22<T> m22(lua_tinker::read<T>(L,2),lua_tinker::read<T>(L,3),lua_tinker::read<T>(L,4),lua_tinker::read<T>(L,5));
	memcpy( ptr, &m22, sizeof( Matrix22<T> ) );
	Matrix22<T>* temp = (Matrix22<T>*)ptr;
	lua_getglobal(L, "matrix22f");
	lua_setmetatable(L, -2);
	return 1; 
}
//-------------------------------------------------------------------------------------------------------
template<typename T> int Vector2Add(lua_State *L)
{
	int count = lua_gettop(L);
	//vector2<T>* v1 = lua_tinker::read<vector2<T>*>(L,1);
	//vector2<T>* v2 = lua_tinker::read<vector2<T>*>(L,2);
	vector2<T>* v1 = (vector2<T>*)lua_touserdata(L,1);
	vector2<T>* v2 = (vector2<T>*)lua_touserdata(L,2);
	void* ptr = lua_newuserdata(L, sizeof( vector2<T> ) );
	new( ptr ) vector2<T>( *v1+*v2 );
	//设置数据类型
	lua_getglobal(L, "vector2f");
	lua_setmetatable(L, -2);
	//lua_tinker::push( L, *v1+*v2 );
	return 1; 
}
//-------------------------------------------------------------------------------------------------------
template<typename T> int Vector2Mul(lua_State *L)
{
	int count = lua_gettop(L);
	if(lua_isuserdata(L, 1)&&lua_isuserdata(L, 2))
	{
		vector2<T>* v1 = (vector2<T>*)lua_touserdata(L,1);
		RttiObject* obj = (RttiObject*)lua_touserdata(L,2);
		if ( obj->isObject( vector2<T>::RTTI() ) )
		{
			vector2<T>* v2 = static_cast<vector2<T>*>(obj);
			//lua_tinker::push( L, *v1**v2 );
			void* ptr = lua_newuserdata(L, sizeof( vector2<T> ) );
			new( ptr ) vector2<T>( *v1**v2 );
			lua_getglobal(L, "vector2f");
			lua_setmetatable(L, -2);
		}
		else
		{
			Matrix22<T>* v2 = static_cast<Matrix22<T>*>(obj);
			//lua_tinker::push( L, *v1**v2 );
			void* ptr = lua_newuserdata(L, sizeof( vector2<T> ) );
			new( ptr ) vector2<T>( *v1**v2 );
			lua_getglobal(L, "vector2f");
			lua_setmetatable(L, -2);
		}		
	}
	else
	{
		//vector2<T>* v1 = lua_tinker::read<vector2<T>*>(L,1);
		T v2 = lua_tinker::read<T>(L,2);
		vector2<T>* v1 = (vector2<T>*)lua_touserdata(L,1);
		void* ptr = lua_newuserdata(L, sizeof( vector2<T> ) );
		new( ptr ) vector2<T>( *v1*v2 );
		lua_getglobal(L, "vector2f");
		lua_setmetatable(L, -2);
	}
	return 1; 
}
//-------------------------------------------------------------------------------------------------------
template<typename T> int Vector2Dot(lua_State *L)
{
	//lua_Debug ar;
	//if (lua_getstack(L, 0, &ar))
	//{
	//	lua_getinfo(L, "n", &ar);
	//}
	vector2<float>* test = (vector2<float>*)lua_touserdata(L, lua_upvalueindex(1));
	int count = lua_gettop(L);
	vector2<T>* v1 = (vector2<T>*)lua_touserdata(L,1);
	vector2<T>* v2 = (vector2<T>*)lua_touserdata(L,2);
	lua_tinker::push( L, v1->Dot(*v2) );
	return 1; 
}
//-------------------------------------------------------------------------------------------------------
int TestFunc(lua_State *L)
{
	int count = lua_gettop(L);
	return 0; 
}
//-------------------------------------------------------------------------------------------------------
template<typename Class, typename F>//注册类成员函数
void RegClassFunc(lua_State* l, const char* name, F func)//这里不能加inline，应为会导致inline的递归，gcc不允许
{
	lua_tinker::class_def<Class>(l, name, func);
}



#include "RttiLibrary.h"
#include "MathRegister.h"
#include "ScriptFunction.h"
#include "ScriptClass.h"
#include "LuaScriptSystem.h"

class B
{
	virtual ~B(){};
	virtual void get(){};
};

using namespace BohgeEngine;
class T1
{
	float a;
};

class T2 : public B
{
	static BohgeEngine::Rtti rt;
	float a;
};


class Base : public RttiObject
{
	RTTI_TYPE_DECLEAR( Base )
public:
	virtual void Func()
	{
		printf("Base\n");
	}
};

class Driver : public Base
{
	RTTI_TYPE_DECLEAR( Driver )
public:
	virtual void Func()
	{
		printf("Driver\n");
	}
};
RTTI_TYPE_IMPLEMENT( Base );
RTTI_TYPE_IMPLEMENT( Driver );

void BaseCon( Base* ptr )
{
	new(ptr) Base();
}

void DriverCon( Driver* ptr )
{
	new(ptr) Driver();
}

void BaseFunc( Base* self )
{
	self->Func();
}

void DriverFunc( Driver* self )
{
	self->Func();
}


//测试返回值为继承类
class IPlugin : public RttiObject
{
	RTTI_ABSTRACT_TYPE_DECLEAR( IPlugin )
public:
	virtual ~IPlugin()
	{

	}
	virtual void Func() = 0;
};

class IOSystem : public IPlugin
{
	RTTI_TYPE_DECLEAR( IOSystem )
private:
	int			m_Test1;
public:
	IOSystem()
		:m_Test1(1)
	{

	}
public:
	virtual void Func()
	{
		printf( "IOSystem! %d\n", m_Test1 );
	}
};

class ScriptSystem : public IPlugin
{
	RTTI_TYPE_DECLEAR( ScriptSystem )
private:
	int			m_Test1;
	int			m_Test2;
public:
	ScriptSystem()
		:m_Test1( 1 ),
		m_Test2( 2 )
	{
	}
public:
	virtual void Func()
	{
		printf( "ScriptSystem! %d %d\n", m_Test1, m_Test2 );
	}
};

class Framework : public RttiObject
{
public:
	enum Constant
	{
		FC_IO = 0,
		FC_SCRIPT,
	};
private:
	IOSystem			m_IO;
	ScriptSystem		m_Script;
	RTTI_TYPE_DECLEAR( Framework )
public:
	IPlugin* GetPlugin( Constant i )
	{
		switch( i )
		{
		case FC_IO: return &m_IO;
		case FC_SCRIPT: return &m_Script;
		}
		return NULL;
	}
};
#include "ScriptValue.h"

void FrameworkGetPlugin( ScriptValue* res, Framework* self, Int* num )
{
	res->SetValue( self->GetPlugin( static_cast<Framework::Constant>( num->operator int() ) ), true );
}

void ScriptSystemFunc( ScriptSystem* self )
{
	self->Func();
}

void IOSystemFunc( IOSystem* self )
{
	self->Func();
}

RTTI_TYPE_IMPLEMENT( Framework );
RTTI_TYPE_IMPLEMENT( IPlugin );
RTTI_TYPE_IMPLEMENT( ScriptSystem );
RTTI_TYPE_IMPLEMENT( IOSystem );


void main(void)
{
	//void* func = (void*)&BohgeEngine::Vector2Add;
	//void* res = new vector2<float>(0,0);
	//void* v1 = new vector2<float>(1,2);
	//void* v2 = new vector2<float>(3,4);
	//void* f = new float(2.0);
	////(( void(*)( void*, void*, void*) )func)( res, v1, v2 );

	//ScriptFunction sf;
	//sf.Result( &vector2<float>::RTTI() );
	//sf.Arguments( &vector2<float>::RTTI(), &vector2<float>::RTTI() );
	//sf.Function( &BohgeEngine::Vector2MulVector2, __MUL );

	//ScriptFunction sf2;
	//sf2.Result( &vector2<float>::RTTI() );
	//sf2.Arguments( &vector2<float>::RTTI(), &Float::RTTI() );	
	//sf2.Function( &BohgeEngine::Vector2MulNum, __MUL );

	//ScriptFunction con1;
	//con1.Result( &vector2<float>::RTTI() );
	//con1.Function( &BohgeEngine::Vector2Con, __CON );

	//ScriptFunction con2;
	//con2.Result( &vector2<float>::RTTI() );
	//con2.Arguments( &Float::RTTI(), &Float::RTTI() );
	//con2.Function( &BohgeEngine::Vector2ConArg, __CON );

	//ScriptClass sc;
	//sc.ClassType( &vector2<float>::RTTI() );
	//sc.MemberFunction( sf );
	//sc.MemberFunction( sf2 );
	//sc.MemberFunction( con1 );
	//sc.MemberFunction( con2 );


	//ScriptFunction sfb;
	//sfb.Arguments( &Base::RTTI() );
	//sfb.Function( &BaseFunc, "Func" );
	//ScriptFunction conb;
	//conb.Result( &Base::RTTI() );
	//conb.Function( &BaseCon, __CON );
	//ScriptClass base;
	//base.ClassType( &Base::RTTI() );
	//base.MemberFunction( sfb );
	//base.MemberFunction( conb );

	//ScriptFunction sfd;
	//sfd.Arguments( &Driver::RTTI() );
	//sfd.Function( &DriverFunc, "Func" );
	//ScriptFunction cond;
	//cond.Result( &Driver::RTTI() );
	//cond.Function( &DriverCon, __CON );
	//ScriptClass driver;
	//driver.ClassType( &Driver::RTTI(), &Base::RTTI() );
	//driver.MemberFunction( sfd );
	//driver.MemberFunction( cond );


	LuaScriptSystem luasystem;

	luasystem.Initialize();
	//luasystem.RegisterClass( sc );
	//luasystem.RegisterClass( base );
	//luasystem.RegisterClass( driver );


	{//测试返回值为继承类
		{
			ScriptFunction fwg;
			fwg.Result( &IPlugin::RTTI() );
			fwg.Arguments( &Framework::RTTI(), &Int::RTTI() );
			fwg.Function( &FrameworkGetPlugin, "GetPlugin" );
			ScriptClass fw;
			fw.ClassType( &Framework::RTTI() );
			fw.SetConstant( Framework::FC_IO, "FC_IO" );
			fw.SetConstant( Framework::FC_SCRIPT, "FC_SCRIPT" );
			fw.MemberFunction( fwg );
			luasystem.RegisterClass( fw );
		}
		{
			ScriptClass fw;
			fw.ClassType( &IPlugin::RTTI() );
			luasystem.RegisterClass( fw );
		}
		{
			ScriptFunction fwg;
			fwg.Arguments( &IOSystem::RTTI() );
			fwg.Function( &IOSystemFunc, "Func" );
			ScriptClass fw;
			fw.ClassType( &IOSystem::RTTI(), &IPlugin::RTTI() );
			fw.MemberFunction( fwg );
			luasystem.RegisterClass( fw );
		}
		{
			ScriptFunction fwg;
			fwg.Arguments( &ScriptSystem::RTTI() );
			fwg.Function( &ScriptSystemFunc, "Func" );
			ScriptClass fw;
			fw.ClassType( &ScriptSystem::RTTI(), &IPlugin::RTTI() );
			fw.MemberFunction( fwg );
			luasystem.RegisterClass( fw );
		}
	}
	Framework* framework = new Framework;
	//vector2f* testG = new vector2<float>(3,4);
	//luasystem.SetGlobal( "vecTest", testG );
	luasystem.SetGlobal( "framework", framework );

	luasystem.LoadScript("E:\\Bohge_Engine_New_Architecture\\Bohge_External\\lua-5.2.2\\lua_tinker_0.5c\\test.lua");
	//call(luasystem.m_LuaState, "meta.main");
	lua_pushcclosure(luasystem.m_LuaState, on_error, 0);
	int errfunc = lua_gettop(luasystem.m_LuaState);

	lua_getglobal(luasystem.m_LuaState,"meta");
	if(lua_istable(luasystem.m_LuaState,-1))
	{
		lua_getfield( luasystem.m_LuaState, -1, "main" );
		if ( lua_isfunction( luasystem.m_LuaState, -1 ) );
		{
			if(lua_pcall(luasystem.m_LuaState, 0, 1, errfunc) != 0)
			{
				lua_pop(luasystem.m_LuaState, 1);
			}
		}
	}
	else
	{
		print_error(luasystem.m_LuaState, "lua_tinker::call() attempt to call global `%s' (not a function)", "main");
	}

	lua_remove(luasystem.m_LuaState, -2);

	//vector2<float>* ptr = (vector2<float>*)res;
	//ptr = (vector2<float>*)v1;
	//ptr = (vector2<float>*)v2;

	//int t1 = sizeof( T1 );
	//int t2 = sizeof( T2 );


	//lua_State* L = luaL_newstate();
	//luaL_openlibs(L);

	//RegisteClass(L, "vector2f", 10001 );
	//RegisteClassCon(L, "vector2f", &Vector2Con<float> );
	//RegisteClassAdd(L, "vector2f", &Vector2Add<float> );
	//RegisteClassMul(L, "vector2f", &Vector2Mul<float> );
	//RegisteClassFunc(L, "vector2f", "TestFunc", &TestFunc );
	//RegisteClassFunc(L, "vector2f", "Dot", &Vector2Dot<float> );

	//RegisteClass(L, "matrix22f", 10002 );
	//RegisteClassCon(L, "matrix22f", &Matrix22Con<float> );

	//lua_tinker::class_add<vector2<float>>(L, "vector2f");
	//lua_tinker::class_con<vector2<float>>(L, lua_tinker::constructor<vector2<float>>);
	//RegClassFunc< vector2<float>, vector2<float> (vector2<float>::*)(const vector2<float>&) const >( L, "AddVector", &vector2<float>::operator+ );

	//dofile(L, "E:\\Bohge_Engine_New_Architecture\\Bohge_External\\lua-5.2.2\\lua_tinker_0.5c\\meta.lua");
	//call(L, "main");
	lua_close(luasystem.m_LuaState);
}