LOCAL_PATH := $(call my-dir)


# png¿â

include $(CLEAR_VARS)

LOCAL_MODULE := png

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
	../../../../Librarys/Bohge/External/libpng-1.5.10/png.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngerror.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngget.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngmem.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngpread.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngread.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngrio.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngrtran.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngrutil.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngset.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngtrans.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngwio.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngwrite.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngwtran.c \
	../../../../Librarys/Bohge/External/libpng-1.5.10/pngwutil.c  \
  
LOCAL_LDLIBS := -lz  
  
include $(BUILD_STATIC_LIBRARY)
