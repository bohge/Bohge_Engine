LOCAL_PATH := $(call my-dir)

# JNI

include $(CLEAR_VARS)

LOCAL_MODULE := BohgeNet

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
	../../../Librarys/Bohge/BohgeNet/BohgeNet/Base64.cpp \
	../../../Librarys/Bohge/BohgeNet/BohgeNet/Correspondent.cpp \
	../../../Librarys/Bohge/BohgeNet/BohgeNet/NetBase.cpp \
	../../../Librarys/Bohge/BohgeNet/BohgeNet/NetHost.cpp \
	../../../Librarys/Bohge/BohgeNet/BohgeNet/Utility.cpp

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../Librarys/Bohge/BohgeNet/BohgeNet \
$(LOCAL_PATH)/../../../Librarys/Bohge/Bohge

LOCAL_LDLIBS := -lstdc++ -llog
  
include $(BUILD_SHARED_LIBRARY)