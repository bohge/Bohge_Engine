LOCAL_PATH := $(call my-dir)

# JNI

include $(CLEAR_VARS)

LOCAL_MODULE := JNIgame

LOCAL_ARM_MODE := arm

LOCAL_CFLAGS += -DANDROID -DTERRAINMANAGER -DMEMORYPOOL

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -D__NEON_MATH__
endif

LOCAL_SRC_FILES := ../jni.cpp

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../Librarys/Game/Game \
$(LOCAL_PATH)/../../../../Librarys/Bohge/BohgeNet/BohgeNet \
$(LOCAL_PATH)/../../../../Librarys/Bohge/Bohge

LOCAL_LDLIBS := -lstdc++  -ldl -lc -lGLESv2 -lz  -llog -lOpenSLES
LOCAL_STATIC_LIBRARIES := game bohge zip freetype png BohgeNet lua bullet NeonMath cpufeatures glsloptimizer
  
include $(BUILD_SHARED_LIBRARY)