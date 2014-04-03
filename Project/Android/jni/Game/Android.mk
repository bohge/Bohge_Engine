LOCAL_PATH := $(call my-dir)


# Game¿â

include $(CLEAR_VARS)

LOCAL_MODULE := game

LOCAL_ARM_MODE := arm

LOCAL_CFLAGS += -DANDROID -DTERRAINMANAGER -DMEMORYPOOL

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -D__NEON_MATH__
endif

LOCAL_SRC_FILES := \
	../../../../Librarys/Game/Game/AdHelper.cpp \
	../../../../Librarys/Game/Game/Game.cpp \
	../../../../Librarys/Game/Game/MessageBox.cpp \
	../../../../Librarys/Game/Game/NetHelper.cpp \
	../../../../Librarys/Game/Game/SaveHelper.cpp \
	../../../../Librarys/Game/Game/ShopHelper.cpp \
	../../../../Librarys/Game/Game/StateLogo.cpp \
	../../../../Librarys/Game/Game/StateMainMenu.cpp \
	../../../../Librarys/Game/Game/StateManager.cpp



LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../Librarys/Bohge/Bohge \
$(LOCAL_PATH)/../../../../Librarys/Bohge/BohgeNet/BohgeNet \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/lua-5.2.2/Lua

LOCAL_LDLIBS := -lstdc++  -ldl -lc -lGLESv2 -lz  -llog -lOpenSLES -lpthread
LOCAL_STATIC_LIBRARIES := bohge zip freetype png BohgeNet lua bullet nedmalloc NeonMath cpufeatures
  
include $(BUILD_STATIC_LIBRARY)