LOCAL_PATH := $(call my-dir)

# FreeType¿â


include $(CLEAR_VARS)

LOCAL_MODULE := freetype

# compile in ARM mode, since the glyph loader/renderer is a hotspot
# when loading complex pages in the browser
#
LOCAL_ARM_MODE := arm

LOCAL_CFLAGS += -W -Wall
LOCAL_CFLAGS += -fPIC -DPIC
LOCAL_CFLAGS += "-DDARWIN_NO_CARBON"
LOCAL_CFLAGS += "-DFT2_BUILD_LIBRARY"

# the following is for testing only, and should not be used in final builds
# of the product
#LOCAL_CFLAGS += "-DTT_CONFIG_OPTION_BYTECODE_INTERPRETER"

#LOCAL_CFLAGS += -O2



LOCAL_SRC_FILES := \
	../../../../Librarys/Bohge/External/freetype/src/base/ftbbox.c \
	../../../../Librarys/Bohge/External/freetype/src/base/ftbitmap.c \
	../../../../Librarys/Bohge/External/freetype/src/base/ftglyph.c \
	../../../../Librarys/Bohge/External/freetype/src/base/ftlcdfil.c \
	../../../../Librarys/Bohge/External/freetype/src/base/ftstroke.c \
	../../../../Librarys/Bohge/External/freetype/src/base/ftxf86.c \
	../../../../Librarys/Bohge/External/freetype/src/base/ftbase.c \
	../../../../Librarys/Bohge/External/freetype/src/base/ftsystem.c \
	../../../../Librarys/Bohge/External/freetype/src/base/ftinit.c \
	../../../../Librarys/Bohge/External/freetype/src/base/ftgasp.c \
	../../../../Librarys/Bohge/External/freetype/src/raster/raster.c \
	../../../../Librarys/Bohge/External/freetype/src/sfnt/sfnt.c \
	../../../../Librarys/Bohge/External/freetype/src/smooth/smooth.c \
	../../../../Librarys/Bohge/External/freetype/src/autofit/autofit.c \
	../../../../Librarys/Bohge/External/freetype/src/truetype/truetype.c \
	../../../../Librarys/Bohge/External/freetype/src/cff/cff.c \
	../../../../Librarys/Bohge/External/freetype/src/psnames/psnames.c \
	../../../../Librarys/Bohge/External/freetype/src/pshinter/pshinter.c
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../../../Librarys/Bohge/External/freetype/builds/unix \
	$(LOCAL_PATH)/../../../../Librarys/Bohge/External/freetype/include

# enable the FreeType internal memory debugger in the simulator
# you need to define the FT2_DEBUG_MEMORY environment variable
# when running the program to activate it. It will dump memory
# statistics when FT_Done_FreeType is called
#

#ifeq ($(TARGET_SIMULATOR),true)
#LOCAL_CFLAGS += "-DFT_DEBUG_MEMORY"
#endif
                    
LOCAL_LDLIBS    := -ldl -lstdc++ 

include $(BUILD_STATIC_LIBRARY)
