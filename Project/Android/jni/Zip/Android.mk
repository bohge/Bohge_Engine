LOCAL_PATH := $(call my-dir)


#libzip库 转为读取assets资源提供

include $(CLEAR_VARS)

LOCAL_MODULE    := zip

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES :=\
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_add.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_add_dir.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_close.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_delete.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_dirent.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_entry_free.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_entry_new.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_err_str.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_error.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_error_clear.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_error_get.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_error_get_sys_type.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_error_strerror.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_error_to_str.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_fclose.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_file_error_clear.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_file_error_get.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_file_get_offset.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_file_strerror.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_filerange_crc.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_fopen.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_fopen_index.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_fread.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_free.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_get_archive_comment.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_get_archive_flag.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_get_file_comment.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_get_num_files.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_get_name.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_memdup.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_name_locate.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_new.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_open.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_rename.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_replace.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_set_archive_comment.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_set_archive_flag.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_set_file_comment.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_source_buffer.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_source_file.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_source_filep.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_source_free.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_source_function.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_source_zip.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_set_name.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_stat.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_stat_index.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_stat_init.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_strerror.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_unchange.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_unchange_all.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_unchange_archive.c \
	../../../../Librarys/Bohge/External/libzip-0.08.1/zip_unchange_data.c

LOCAL_LDLIBS := -lz

include $(BUILD_STATIC_LIBRARY)