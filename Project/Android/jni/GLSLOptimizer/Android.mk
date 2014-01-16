LOCAL_PATH := $(call my-dir)


#nedmalloc¿âÎÄ¼þ

include $(CLEAR_VARS)

LOCAL_MODULE    := glsloptimizer

LOCAL_ARM_MODE 	:= arm

LOCAL_CFLAGS    := -DNDEBUG

LOCAL_SRC_FILES :=\
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/glcpp/glcpp-lex.c \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/glcpp/glcpp-parse.c \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/glcpp/pp.c \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ast_array_index.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ast_expr.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ast_function.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ast_to_hir.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ast_type.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/builtin_functions.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/builtin_types.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/builtin_variables.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/glsl_lexer.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/glsl_optimizer.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/glsl_parser.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/glsl_parser_extras.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/glsl_symbol_table.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/glsl_types.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/hir_field_selection.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_basic_block.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_builder.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_clone.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_constant_expression.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_equals.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_expression_flattening.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_function.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_function_can_inline.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_function_detect_recursion.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_hierarchical_visitor.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_hv_accept.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_import_prototypes.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_print_glsl_visitor.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_print_visitor.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_rvalue_visitor.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_stats.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_unused_structs.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_validate.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ir_variable_refcount.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/link_atomics.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/link_functions.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/link_interface_blocks.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/link_uniform_block_active_visitor.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/link_uniform_blocks.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/link_uniform_initializers.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/link_uniforms.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/link_varyings.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/linker.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/loop_analysis.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/loop_controls.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/loop_unroll.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_clip_distance.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_discard.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_discard_flow.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_if_to_cond_assign.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_instructions.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_jumps.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_mat_op_to_vec.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_named_interface_blocks.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_noise.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_output_reads.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_packed_varyings.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_packing_builtins.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_ubo_reference.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_variable_index_to_cond_assign.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_vec_index_to_cond_assign.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_vec_index_to_swizzle.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_vector.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/lower_vector_insert.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_algebraic.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_array_splitting.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_constant_folding.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_constant_propagation.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_constant_variable.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_copy_propagation.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_copy_propagation_elements.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_cse.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_dead_builtin_varyings.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_dead_code.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_dead_code_local.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_dead_functions.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_flatten_nested_if_blocks.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_flip_matrices.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_function_inlining.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_if_simplification.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_noop_swizzle.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_redundant_jumps.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_structure_splitting.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_swizzle_swizzle.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/opt_tree_grafting.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/ralloc.c \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/s_expression.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/standalone_scaffolding.cpp \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl/strtod.c \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/mesa/main/hash_table.c \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/mesa/main/imports.c \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/mesa/program/prog_hash_table.c \
	../../../../Librarys/Bohge/External/glsl_optimizer/src/mesa/program/symbol_table.c

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/glsl_optimizer/src/getopt \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/glsl_optimizer/src/glsl \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/glsl_optimizer/src/mesa \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/glsl_optimizer/src/node \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/glsl_optimizer/include
	
LOCAL_LDLIBS    := -ld -lm -ldl -lc
  
include $(BUILD_STATIC_LIBRARY)