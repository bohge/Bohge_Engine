#ifndef  SCRIPTBASE_INC
#define  SCRIPTBASE_INC

#include <string>
#include "lua_tinker.hpp"

#define Script_RegClass(klass)	            gScript.regClass<klass>(#klass)
#define Script_RegClassFunc(klass, member)  gScript.regClassFunc<klass>(#member, &klass::member)
#define Script_RegClassVar(klass, member)   gScript.regClassVar<klass>(#member, &klass::member)

typedef lua_tinker::table luaTable;

class ScriptBase
{
public:
  ScriptBase()
        :_L(NULL)
    {}
    virtual ~ScriptBase();

public:
    virtual bool init(const char *str) = 0;
    virtual bool reload() {return true;}

    //@brief: 默认的初始化，一般情况下调用这个就可以
    bool defaultInti(const char *str);

    void dofile(const char *filename)
    {
      lua_tinker::dofile( _L, filename);
    }

    //HACK 使用引用传递，
    template<typename T>
    void setglobal(const char* name, T object)
    {
      lua_tinker::setglobal(_L, name, object);
    }

    template<typename T>
    T getglobal(const char* name)
    {
      return lua_tinker::getglobal<T>(_L, name);
    }

    //导入全局函数 
    template<typename F>
    void regFunc(const char *funcName, F func);

    //导入class 
    template<typename Class>
    void regClass(const char *name) {lua_tinker::class_add<Class>(_L, name);} 

    template<typename Class, typename F>
    void regClassFunc(const char* name, F func) {lua_tinker::class_def<Class>(_L, name, func);}

    template<typename Class, typename BASE, typename VAR>
    void regClassVar(const char* name, VAR BASE::*val) {lua_tinker::class_mem<Class>(_L, name, val);}

public:

    // Call Lua Func {
    template <typename R>
    inline R callLua(const char *func)
    {
      return lua_tinker::call<R>(_L, func);
    }
  
    template <typename R, typename T1>
    inline R callLua(const char *func, const T1& t1)
    {
      return lua_tinker::call<R, T1>(_L, func, t1);
    }
  
    template <typename R, typename T1, typename T2>
    inline R callLua(const char *func, const T1& t1,  const T2& t2)
    {
      return lua_tinker::call<R, T1, T2>(_L, func, t1, t2);
    }
  
    template <typename R, typename T1, typename T2, typename T3>
    inline R callLua(const char *func, const T1& t1,  const T2& t2, const T3& t3)
    {
      return lua_tinker::call<R, T1, T2, T3>(_L, func, t1, t2, t3);
    }
  
    template <typename R, typename T1, typename T2, typename T3, typename T4>
    inline R callLua(const char *func, const T1& t1,  const T2& t2, const T3& t3, const T4& t4)
    {
      return lua_tinker::call<R, T1, T2, T3, T4>(_L, func, t1, t2, t3, t4);
    }
    // Call lua Func End}

protected:
    lua_State * _L;
    std::string _path;

};

//导入非lua_CFunction 类型的函数
template<typename F>
inline void ScriptBase::regFunc(const char *funcName, F func) 
{
  lua_tinker::def<F>(_L, funcName, func);
}

//导入全局函数 warp for lua_register
template<>
inline void ScriptBase::regFunc<lua_CFunction>(const char *funcName, lua_CFunction func)
{
   lua_register(_L, funcName, func);
}

#endif   /* ----- #ifndef funcbase_INC  ----- */
