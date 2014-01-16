LOCAL_PATH := $(call my-dir)


#lua¿âÎÄ¼þ

include $(CLEAR_VARS)

LOCAL_MODULE    := lua

LOCAL_ARM_MODE 	:= arm

LOCAL_CFLAGS    := -DLUA_ANSI -DANDROID

LOCAL_SRC_FILES :=\
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lapi.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lauxlib.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lbaselib.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lbitlib.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lcode.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lcorolib.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lctype.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/ldblib.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/ldebug.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/ldo.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/ldump.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lfunc.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lgc.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/linit.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/liolib.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/llex.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lmathlib.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lmem.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/loadlib.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lobject.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lopcodes.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/loslib.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lparser.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lstate.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lstring.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lstrlib.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/ltable.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/ltablib.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/ltm.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lundump.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lvm.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lzio.c \
	../../../../Librarys/Bohge/External/lua-5.2.2/Lua/lua_tinker.cpp
	
LOCAL_LDLIBS    := -ld -lm
  
include $(BUILD_STATIC_LIBRARY)