LOCAL_PATH := $(call my-dir)


#NeonMath库文件

include $(CLEAR_VARS)

LOCAL_MODULE    := NeonMath

LOCAL_ARM_MODE 	:= arm

#LOCAL_CFLAGS    := -DLUA_ANSI -DANDROID

LOCAL_SRC_FILES :=\
	../../../../Librarys/Bohge/External/neonmath/math_acosf.c \
	../../../../Librarys/Bohge/External/neonmath/math_asinf.c \
	../../../../Librarys/Bohge/External/neonmath/math_atan2f.c \
	../../../../Librarys/Bohge/External/neonmath/math_atanf.c \
	../../../../Librarys/Bohge/External/neonmath/math_ceilf.c \
	../../../../Librarys/Bohge/External/neonmath/math_cosf.c \
	../../../../Librarys/Bohge/External/neonmath/math_coshf.c \
	../../../../Librarys/Bohge/External/neonmath/math_expf.c \
	../../../../Librarys/Bohge/External/neonmath/math_fabsf.c \
	../../../../Librarys/Bohge/External/neonmath/math_floorf.c \
	../../../../Librarys/Bohge/External/neonmath/math_fmodf.c \
	../../../../Librarys/Bohge/External/neonmath/math_frexpf.c \
	../../../../Librarys/Bohge/External/neonmath/math_invsqrtf.c \
	../../../../Librarys/Bohge/External/neonmath/math_ldexpf.c \
	../../../../Librarys/Bohge/External/neonmath/math_log10f.c \
	../../../../Librarys/Bohge/External/neonmath/math_logf.c \
	../../../../Librarys/Bohge/External/neonmath/math_mat2.c \
	../../../../Librarys/Bohge/External/neonmath/math_mat3.c \
	../../../../Librarys/Bohge/External/neonmath/math_mat4.c \
	../../../../Librarys/Bohge/External/neonmath/math_modf.c \
	../../../../Librarys/Bohge/External/neonmath/math_powf.c \
	../../../../Librarys/Bohge/External/neonmath/math_runfast.c \
	../../../../Librarys/Bohge/External/neonmath/math_sincosf.c \
	../../../../Librarys/Bohge/External/neonmath/math_sinf.c \
	../../../../Librarys/Bohge/External/neonmath/math_sinfv.c \
	../../../../Librarys/Bohge/External/neonmath/math_sinhf.c \
	../../../../Librarys/Bohge/External/neonmath/math_sqrtf.c \
	../../../../Librarys/Bohge/External/neonmath/math_sqrtfv.c \
	../../../../Librarys/Bohge/External/neonmath/math_tanf.c \
	../../../../Librarys/Bohge/External/neonmath/math_tanhf.c \
	../../../../Librarys/Bohge/External/neonmath/math_vec2.c \
	../../../../Librarys/Bohge/External/neonmath/math_vec3.c \
	../../../../Librarys/Bohge/External/neonmath/math_vec4.c
LOCAL_LDLIBS    := -ld -lm
  
include $(BUILD_STATIC_LIBRARY)