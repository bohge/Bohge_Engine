LOCAL_PATH := $(call my-dir)

# JNI

include $(CLEAR_VARS)

LOCAL_MODULE := libvorbis

LOCAL_ARM_MODE := arm

LOCAL_CFLAGS += -DANDROID -DTERRAINMANAGER -DMEMORYPOOL

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -D__NEON_MATH__
endif

LOCAL_SRC_FILES := \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/analysis.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/barkmel.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/bitrate.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/block.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/codebook.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/envelope.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/floor0.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/floor1.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/info.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/lookup.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/lpc.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/lsp.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/mapping0.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/mdct.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/psy.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/registry.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/res0.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/sharedbook.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/smallft.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/synthesis.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/tone.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/vorbisenc.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/vorbisfile.c \
	../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/window.c \

	
	
LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/libvorbis-1.3.3/include \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/libogg-1.3.0/include \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/libvorbis-1.3.3/lib/

LOCAL_LDLIBS := -ldl -lc
  
include $(BUILD_STATIC_LIBRARY)