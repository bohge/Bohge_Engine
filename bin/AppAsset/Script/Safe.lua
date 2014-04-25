-- 仅支持Lua 5.2版
assert(_VERSION == "Lua 5.2")

-- 全局环境在注册表中的索引值（见lua.h）
local LUA_RIDX_GLOBALS = 2

-- 安全table的metatable标志
local SAFE_TABLE_FLAG = ".SAFETABLE"

-- 设置全局安全保护机制
local function SetupGlobal()

	-- 获取注册表
	local reg = debug.getregistry()

	local env = {}			-- 新环境table
	local proxy = {}		-- 代理table
	local mt = {}			-- metatable

	-- 操作重载
	mt.__index = proxy
	mt.__newindex = function() print("cannot modify global enviroment!") end
	mt.__len = function() return #proxy end
	mt.__pairs = function() return pairs(proxy) end
	mt.__ipairs = function() return ipairs(proxy) end

	-- 隐藏metatable
	mt.__metatable = 0

	-- 标记为安全table
	mt[SAFE_TABLE_FLAG] = true

	-- 获取旧环境
	local old_env = reg[LUA_RIDX_GLOBALS]

	-- 设置新环境的metatable
	setmetatable(env, mt)

	-- 启用新环境
	_ENV = env

	-- 将全局默认环境也改为新环境
	reg[LUA_RIDX_GLOBALS] = env

	-- 返回代理table和旧环境
	return proxy, old_env

end

-- 新建一个有安全保护的table
local function CreateSafeTable(base)

	local new = {}			-- 新table
	local mt = {}			-- metatable

	-- 如果没有指定base则新建一个空table
	local proxy = (type(base) == "table") and base or {}

	-- 操作重载
	mt.__index = proxy
	mt.__newindex = function() print("cannot modify safe table!") end
	mt.__len = function() return #proxy end
	mt.__pairs = function() return pairs(proxy) end
	mt.__ipairs = function() return ipairs(proxy) end

	-- 隐藏metatable
	mt.__metatable = 0

	-- 标记为安全table
	mt[SAFE_TABLE_FLAG] = true

	-- 设置新table的metatable
	setmetatable(new, mt)

	-- 返回新table和对应的代理table
	return new, proxy

end

-- 开启全局保护
local proxy, old_env = SetupGlobal()

-- 在这里复制需要导出给新环境使用的Lua原生全局变量和函数
-- 被屏蔽的原生全局变量和函数有：
--	_G			Lua 5.2推荐使用_ENV（你可以根据需要把它定义为_ENV）
--	dofile		我的工程需要屏遮文件系统，我没有评估过开放它对安全性有没有影响
--	loadfile	我的工程需要屏遮文件系统，我没有评估过开放它对安全性有没有影响
--	rawequal	需要覆盖，不应该直接操作安全table
--	rawget		需要覆盖，不应该直接操作安全table
--	rawlen		需要覆盖，不应该直接操作安全table
--	rawset		需要覆盖，不应该直接操作安全table
--	require		我的工程需要屏遮文件系统，我没有评估过开放它对安全性有没有影响
proxy._VERSION = old_env._VERSION
proxy.assert = old_env.assert
proxy.collectgarbage = old_env.collectgarbage
proxy.error = old_env.error
proxy.getmetatable = old_env.getmetatable
proxy.ipairs = old_env.ipairs
proxy.load = old_env.load
proxy.next = old_env.next
proxy.pairs = old_env.pairs
proxy.pcall = old_env.pcall
proxy.print = old_env.print
proxy.select = old_env.select
proxy.setmetatable = old_env.setmetatable
proxy.tostring = old_env.tostring
proxy.tonumber = old_env.tonumber
proxy.type = old_env.type
proxy.xpcall = old_env.xpcall

-- 在这里导出给新环境使用的Lua原生全局table（将被设为只读table）
-- 被屏蔽的原生全局table有：
--	coroutine	我的工程里不需要coroutine，我没有评估过开放它对安全性有没有影响
--	debug		会严重影响安全性，必须屏蔽
--	io			我的工程需要屏遮文件系统，我没有评估过开放它对安全性有没有影响
--	os			我的工程里不需要os，我没有评估过开放它对安全性有没有影响
--	package		我的工程需要屏遮文件系统，我没有评估过开放它对安全性有没有影响
proxy.bit32 = CreateSafeTable(old_env.bit32)
proxy.math = CreateSafeTable(old_env.math)
proxy.string = CreateSafeTable(old_env.string)
proxy.table = CreateSafeTable(old_env.table)

-- 实现安全版的rawequal
proxy.rawequal = function(v1, v2)

	-- 获得真实的metatable
	local mt1 = old_env.debug.getmetatable(v1)
	local mt2 = old_env.debug.getmetatable(v2)

	-- 如果是安全table则使用代理table
	if mt1 and mt1[SAFE_TABLE_FLAG] then
		v1 = mt1.__index
	end
	if mt2 and mt2[SAFE_TABLE_FLAG] then
		v2 = mt2.__index
	end

	-- 调用原始rawequal
	return old_env.rawequal(v1, v2)

end

-- 实现安全版的rawget
proxy.rawget = function(t, k)

	-- 获得真实的metatable
	local mt = old_env.debug.getmetatable(t)

	-- 如果是安全table则使用代理table
	if mt and mt[SAFE_TABLE_FLAG] then
		t = mt.__index
	end

	-- 调用原始rawget
	return old_env.rawget(t, k)

end

-- 实现安全版的rawlen
proxy.rawlen = function(v)

	-- 获得真实的metatable
	local mt = old_env.debug.getmetatable(v)

	-- 如果是安全table则使用代理table
	if mt and mt[SAFE_TABLE_FLAG] then
		v = mt.__index
	end

	-- 调用原始rawlen
	return old_env.rawlen(v)

end

-- 实现安全版的rawset
proxy.rawset = function(t, k, v)

	-- 获得真实的metatable
	local mt = old_env.debug.getmetatable(t)

	-- 如果是安全table则使用代理table
	if mt and mt[SAFE_TABLE_FLAG] then
		t = mt.__index
	end

	-- 调用原始rawset
	return old_env.rawset(t, k, v)

end

-- 这里可以自定义一些自己的内容

-- 脚本文件装载列表
local loaded_proxy
proxy.LOADED, loaded_proxy = CreateSafeTable()

-- 导入脚本文件
proxy.import = function(s)

	-- 如果已经被导入则返回true
	if LOADED[s] ~= nil then
		return true
	end

	-- 装载文件
	local f, msg = old_env.loadfile(s)

	-- 如果装载失败，输出错误
	if not f then
		old_env.io.stderr:write(msg)
		return false
	end

	-- 否则执行该脚本
	local r, msg = pcall(f)

	-- 如果执行过程中出错，输出错误
	if not r then
		old_env.io.stderr:write(msg)
		return false
	end

	-- 记录文件名到装载列表
	loaded_proxy[s] = f

	-- 成功
	return true

end

-- 由于外界（这里指的是main.lua）环境已经初始化过环境了，没办法在safe.lua里直接更改（我没找到办法）
-- 因此这里返回新环境给main.lua，main.lua需要在装载完该文件后把自己的环境设为该新环境
-- 对于C这一步是不需要的，本身main.lua做作的一切可以都在C里完成
do return _ENV end