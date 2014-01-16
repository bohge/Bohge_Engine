LOCAL_PATH := $(call my-dir)


#nedmalloc¿âÎÄ¼þ

include $(CLEAR_VARS)

LOCAL_MODULE    := nedmalloc

LOCAL_ARM_MODE 	:= arm

LOCAL_CFLAGS    := -DNDEBUG -DSTRUCT_MALLINFO_DECLARED

LOCAL_SRC_FILES :=\
	../../../../Librarys/Bohge/External/nedmalloc_v1.06beta2/nedmalloc.c 

	
LOCAL_LDLIBS    := -ld -lm -lpthread -ldl -lc
  
include $(BUILD_STATIC_LIBRARY)