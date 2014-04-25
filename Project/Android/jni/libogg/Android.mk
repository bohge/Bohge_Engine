LOCAL_PATH := $(call my-dir)

# JNI

include $(CLEAR_VARS)

LOCAL_MODULE := libogg

LOCAL_ARM_MODE := arm

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -D__NEON_MATH__
endif

LOCAL_SRC_FILES := \
	../../../../Librarys/Bohge/External/libogg-1.3.0/src/bitwise.c \
	../../../../Librarys/Bohge/External/libogg-1.3.0/src/framing.c

	
	
LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/libvorbis-1.3.3/include \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/libogg-1.3.0/include

LOCAL_LDLIBS := -ldl -lc
  
include $(BUILD_STATIC_LIBRARY)