#ifndef OOO_TREESITTER_SYMBOLS_H
#define OOO_TREESITTER_SYMBOLS_H

#include "tree_sitter/api.h"
#include "Cursor.h"

/* copyed from treesitter-c/src/parser.c */
/* ts_symbol_identifiers */
enum ts_symbol_identifiers {
  sym_identifier = 1,
  aux_sym_preproc_include_token1 = 2,
  aux_sym_preproc_include_token2 = 3,
  aux_sym_preproc_def_token1 = 4,
  anon_sym_LPAREN = 5,
  anon_sym_DOT_DOT_DOT = 6,
  anon_sym_COMMA = 7,
  anon_sym_RPAREN = 8,
  aux_sym_preproc_if_token1 = 9,
  anon_sym_LF = 10,
  aux_sym_preproc_if_token2 = 11,
  aux_sym_preproc_ifdef_token1 = 12,
  aux_sym_preproc_ifdef_token2 = 13,
  aux_sym_preproc_else_token1 = 14,
  aux_sym_preproc_elif_token1 = 15,
  aux_sym_preproc_elifdef_token1 = 16,
  aux_sym_preproc_elifdef_token2 = 17,
  sym_preproc_arg = 18,
  sym_preproc_directive = 19,
  anon_sym_LPAREN2 = 20,
  anon_sym_defined = 21,
  anon_sym_BANG = 22,
  anon_sym_TILDE = 23,
  anon_sym_DASH = 24,
  anon_sym_PLUS = 25,
  anon_sym_STAR = 26,
  anon_sym_SLASH = 27,
  anon_sym_PERCENT = 28,
  anon_sym_PIPE_PIPE = 29,
  anon_sym_AMP_AMP = 30,
  anon_sym_PIPE = 31,
  anon_sym_CARET = 32,
  anon_sym_AMP = 33,
  anon_sym_EQ_EQ = 34,
  anon_sym_BANG_EQ = 35,
  anon_sym_GT = 36,
  anon_sym_GT_EQ = 37,
  anon_sym_LT_EQ = 38,
  anon_sym_LT = 39,
  anon_sym_LT_LT = 40,
  anon_sym_GT_GT = 41,
  anon_sym_SEMI = 42,
  anon_sym___extension__ = 43,
  anon_sym_typedef = 44,
  anon_sym_extern = 45,
  anon_sym___attribute__ = 46,
  anon_sym_COLON_COLON = 47,
  anon_sym_LBRACK_LBRACK = 48,
  anon_sym_RBRACK_RBRACK = 49,
  anon_sym___declspec = 50,
  anon_sym___based = 51,
  anon_sym___cdecl = 52,
  anon_sym___clrcall = 53,
  anon_sym___stdcall = 54,
  anon_sym___fastcall = 55,
  anon_sym___thiscall = 56,
  anon_sym___vectorcall = 57,
  sym_ms_restrict_modifier = 58,
  sym_ms_unsigned_ptr_modifier = 59,
  sym_ms_signed_ptr_modifier = 60,
  anon_sym__unaligned = 61,
  anon_sym___unaligned = 62,
  anon_sym_LBRACE = 63,
  anon_sym_RBRACE = 64,
  anon_sym_signed = 65,
  anon_sym_unsigned = 66,
  anon_sym_long = 67,
  anon_sym_short = 68,
  anon_sym_LBRACK = 69,
  anon_sym_RBRACK = 70,
  anon_sym_EQ = 71,
  anon_sym_static = 72,
  anon_sym_auto = 73,
  anon_sym_register = 74,
  anon_sym_inline = 75,
  anon_sym___inline = 76,
  anon_sym___inline__ = 77,
  anon_sym___forceinline = 78,
  anon_sym_thread_local = 79,
  anon_sym___thread = 80,
  anon_sym_const = 81,
  anon_sym_constexpr = 82,
  anon_sym_volatile = 83,
  anon_sym_restrict = 84,
  anon_sym___restrict__ = 85,
  anon_sym__Atomic = 86,
  anon_sym__Noreturn = 87,
  anon_sym_noreturn = 88,
  sym_primitive_type = 89,
  anon_sym_enum = 90,
  anon_sym_COLON = 91,
  anon_sym_struct = 92,
  anon_sym_union = 93,
  anon_sym_if = 94,
  anon_sym_else = 95,
  anon_sym_switch = 96,
  anon_sym_case = 97,
  anon_sym_default = 98,
  anon_sym_while = 99,
  anon_sym_do = 100,
  anon_sym_for = 101,
  anon_sym_return = 102,
  anon_sym_break = 103,
  anon_sym_continue = 104,
  anon_sym_goto = 105,
  anon_sym___try = 106,
  anon_sym___except = 107,
  anon_sym___finally = 108,
  anon_sym___leave = 109,
  anon_sym_QMARK = 110,
  anon_sym_STAR_EQ = 111,
  anon_sym_SLASH_EQ = 112,
  anon_sym_PERCENT_EQ = 113,
  anon_sym_PLUS_EQ = 114,
  anon_sym_DASH_EQ = 115,
  anon_sym_LT_LT_EQ = 116,
  anon_sym_GT_GT_EQ = 117,
  anon_sym_AMP_EQ = 118,
  anon_sym_CARET_EQ = 119,
  anon_sym_PIPE_EQ = 120,
  anon_sym_DASH_DASH = 121,
  anon_sym_PLUS_PLUS = 122,
  anon_sym_sizeof = 123,
  anon_sym___alignof__ = 124,
  anon_sym___alignof = 125,
  anon_sym__alignof = 126,
  anon_sym_alignof = 127,
  anon_sym__Alignof = 128,
  anon_sym_offsetof = 129,
  anon_sym__Generic = 130,
  anon_sym_asm = 131,
  anon_sym___asm__ = 132,
  anon_sym_DOT = 133,
  anon_sym_DASH_GT = 134,
  sym_number_literal = 135,
  anon_sym_L_SQUOTE = 136,
  anon_sym_u_SQUOTE = 137,
  anon_sym_U_SQUOTE = 138,
  anon_sym_u8_SQUOTE = 139,
  anon_sym_SQUOTE = 140,
  aux_sym_char_literal_token1 = 141,
  anon_sym_L_DQUOTE = 142,
  anon_sym_u_DQUOTE = 143,
  anon_sym_U_DQUOTE = 144,
  anon_sym_u8_DQUOTE = 145,
  anon_sym_DQUOTE = 146,
  aux_sym_string_literal_token1 = 147,
  sym_escape_sequence = 148,
  sym_system_lib_string = 149,
  sym_true = 150,
  sym_false = 151,
  anon_sym_NULL = 152,
  anon_sym_nullptr = 153,
  sym_comment = 154,
  sym_translation_unit = 155,
  sym_preproc_include = 156,
  sym_preproc_def = 157,
  sym_preproc_function_def = 158,
  sym_preproc_params = 159,
  sym_preproc_call = 160,
  sym_preproc_if = 161,
  sym_preproc_ifdef = 162,
  sym_preproc_else = 163,
  sym_preproc_elif = 164,
  sym_preproc_elifdef = 165,
  sym_preproc_if_in_field_declaration_list = 166,
  sym_preproc_ifdef_in_field_declaration_list = 167,
  sym_preproc_else_in_field_declaration_list = 168,
  sym_preproc_elif_in_field_declaration_list = 169,
  sym__preproc_expression = 170,
  sym_preproc_parenthesized_expression = 171,
  sym_preproc_defined = 172,
  sym_preproc_unary_expression = 173,
  sym_preproc_call_expression = 174,
  sym_preproc_argument_list = 175,
  sym_preproc_binary_expression = 176,
  sym_function_definition = 177,
  sym__old_style_function_definition = 178,
  sym_declaration = 179,
  sym__declaration_declarator = 180,
  sym_type_definition = 181,
  sym__type_definition_type = 182,
  sym__type_definition_declarators = 183,
  sym__declaration_modifiers = 184,
  sym__declaration_specifiers = 185,
  sym_linkage_specification = 186,
  sym_attribute_specifier = 187,
  sym_attribute = 188,
  sym_attribute_declaration = 189,
  sym_ms_declspec_modifier = 190,
  sym_ms_based_modifier = 191,
  sym_ms_call_modifier = 192,
  sym_ms_unaligned_ptr_modifier = 193,
  sym_ms_pointer_modifier = 194,
  sym_declaration_list = 195,
  sym__declarator = 196,
  sym__field_declarator = 197,
  sym__type_declarator = 198,
  sym__abstract_declarator = 199,
  sym_parenthesized_declarator = 200,
  sym_parenthesized_field_declarator = 201,
  sym_parenthesized_type_declarator = 202,
  sym_abstract_parenthesized_declarator = 203,
  sym_attributed_declarator = 204,
  sym_attributed_field_declarator = 205,
  sym_attributed_type_declarator = 206,
  sym_pointer_declarator = 207,
  sym_pointer_field_declarator = 208,
  sym_pointer_type_declarator = 209,
  sym_abstract_pointer_declarator = 210,
  sym_function_declarator = 211,
  sym_function_field_declarator = 212,
  sym_function_type_declarator = 213,
  sym_abstract_function_declarator = 214,
  sym__old_style_function_declarator = 215,
  sym_array_declarator = 216,
  sym_array_field_declarator = 217,
  sym_array_type_declarator = 218,
  sym_abstract_array_declarator = 219,
  sym_init_declarator = 220,
  sym_compound_statement = 221,
  sym_storage_class_specifier = 222,
  sym_type_qualifier = 223,
  sym__type_specifier = 224,
  sym_sized_type_specifier = 225,
  sym_enum_specifier = 226,
  sym_enumerator_list = 227,
  sym_struct_specifier = 228,
  sym_union_specifier = 229,
  sym_field_declaration_list = 230,
  sym__field_declaration_list_item = 231,
  sym_field_declaration = 232,
  sym__field_declaration_declarator = 233,
  sym_bitfield_clause = 234,
  sym_enumerator = 235,
  sym_variadic_parameter = 236,
  sym_parameter_list = 237,
  sym__old_style_parameter_list = 238,
  sym_parameter_declaration = 239,
  sym_attributed_statement = 240,
  sym_labeled_statement = 241,
  sym__top_level_expression_statement = 242,
  sym_expression_statement = 243,
  sym_if_statement = 244,
  sym_else_clause = 245,
  sym_switch_statement = 246,
  sym_case_statement = 247,
  sym_while_statement = 248,
  sym_do_statement = 249,
  sym_for_statement = 250,
  sym__for_statement_body = 251,
  sym_return_statement = 252,
  sym_break_statement = 253,
  sym_continue_statement = 254,
  sym_goto_statement = 255,
  sym_seh_try_statement = 256,
  sym_seh_except_clause = 257,
  sym_seh_finally_clause = 258,
  sym_seh_leave_statement = 259,
  sym__expression = 260,
  sym__expression_not_binary = 261,
  sym_comma_expression = 262,
  sym_conditional_expression = 263,
  sym_assignment_expression = 264,
  sym_pointer_expression = 265,
  sym_unary_expression = 266,
  sym_binary_expression = 267,
  sym_update_expression = 268,
  sym_cast_expression = 269,
  sym_type_descriptor = 270,
  sym_sizeof_expression = 271,
  sym_alignof_expression = 272,
  sym_offsetof_expression = 273,
  sym_generic_expression = 274,
  sym_subscript_expression = 275,
  sym_call_expression = 276,
  sym_gnu_asm_expression = 277,
  sym_gnu_asm_qualifier = 278,
  sym_gnu_asm_output_operand_list = 279,
  sym_gnu_asm_output_operand = 280,
  sym_gnu_asm_input_operand_list = 281,
  sym_gnu_asm_input_operand = 282,
  sym_gnu_asm_clobber_list = 283,
  sym_gnu_asm_goto_list = 284,
  sym_argument_list = 285,
  sym_field_expression = 286,
  sym_compound_literal_expression = 287,
  sym_parenthesized_expression = 288,
  sym_initializer_list = 289,
  sym_initializer_pair = 290,
  sym_subscript_designator = 291,
  sym_field_designator = 292,
  sym_char_literal = 293,
  sym_concatenated_string = 294,
  sym_string_literal = 295,
  sym_null = 296,
  sym__empty_declaration = 297,
  sym_macro_type_specifier = 298,
  aux_sym_translation_unit_repeat1 = 299,
  aux_sym_preproc_params_repeat1 = 300,
  aux_sym_preproc_if_repeat1 = 301,
  aux_sym_preproc_if_in_field_declaration_list_repeat1 = 302,
  aux_sym_preproc_argument_list_repeat1 = 303,
  aux_sym__old_style_function_definition_repeat1 = 304,
  aux_sym__declaration_declarator_repeat1 = 305,
  aux_sym_type_definition_repeat1 = 306,
  aux_sym__type_definition_type_repeat1 = 307,
  aux_sym__type_definition_declarators_repeat1 = 308,
  aux_sym__declaration_specifiers_repeat1 = 309,
  aux_sym_attribute_declaration_repeat1 = 310,
  aux_sym_attributed_declarator_repeat1 = 311,
  aux_sym_pointer_declarator_repeat1 = 312,
  aux_sym_sized_type_specifier_repeat1 = 313,
  aux_sym_enumerator_list_repeat1 = 314,
  aux_sym__field_declaration_declarator_repeat1 = 315,
  aux_sym_parameter_list_repeat1 = 316,
  aux_sym__old_style_parameter_list_repeat1 = 317,
  aux_sym_case_statement_repeat1 = 318,
  aux_sym_generic_expression_repeat1 = 319,
  aux_sym_gnu_asm_expression_repeat1 = 320,
  aux_sym_gnu_asm_output_operand_list_repeat1 = 321,
  aux_sym_gnu_asm_input_operand_list_repeat1 = 322,
  aux_sym_gnu_asm_clobber_list_repeat1 = 323,
  aux_sym_gnu_asm_goto_list_repeat1 = 324,
  aux_sym_argument_list_repeat1 = 325,
  aux_sym_initializer_list_repeat1 = 326,
  aux_sym_initializer_pair_repeat1 = 327,
  aux_sym_concatenated_string_repeat1 = 328,
  aux_sym_string_literal_repeat1 = 329,
  alias_sym_field_identifier = 330,
  alias_sym_statement_identifier = 331,
  alias_sym_type_identifier = 332,
};

typedef enum {
        OOO_ENTRY,
        OOO_NEXT,
        OOO_EXIT
} OOO_Transition;

typedef struct {
        Cursor cursor;
        StringView sink;
        StringView source;
        size_t indentation_level;
} OOO_Job;

typedef struct {
        size_t begin;
        size_t end;
} OOO_Slice;

void ooo_set_indentation(
        Cursor *cursor,
        StringView *sink,
        StringView *source,
        TSNode node,
        size_t indentation_level
);

void ooo_print_nodes(
        TSNode node,
        size_t row_begin,
        size_t row_end,
        size_t level
); 

TSNode ooo_visit_nodes(
        TSNode node,
        TSNode serial_node,
        OOO_Job *job
);

bool ooo_rule_dispatcher(
        TSNode node,
        TSNode serial_node,
        OOO_Slice slice,
        OOO_Job *job
);

size_t ooo_indentation(OOO_Transition const transition, TSNode const node, size_t level);

#endif

//#define OOO_IMPLEMENTATION 
#ifdef OOO_IMPLEMENTATION


TSNode ooo_visit_nodes(
        TSNode node,
        TSNode serial_node,
        OOO_Job *job
) {
        static size_t count = 0;

        TSPoint start_point = ts_node_start_point(node);
        OOO_Slice slice = {
                .begin = job->cursor.idx,
                .end = Cursor_move_to_point(&job->cursor, &job->source, start_point)
        };

        if (ooo_rule_dispatcher(node, serial_node, slice, job)) {
                for (size_t i = slice.begin; i < slice.end; i++) {
                        StringView_append_chr(&job->sink, job->source.at[i]);
                }
        }

        size_t num_of_childs = ts_node_child_count(node);
        TSNode serial = node;
        for (size_t it = 0; it < num_of_childs; it++) {
               TSNode child = ts_node_child(node, it);
                serial = ooo_visit_nodes(
                        child,
                        serial,
                        job
                );
        }

        TSPoint end_point = ts_node_end_point(node);
        slice.begin = job->cursor.idx;
        slice.end = Cursor_move_to_point(&job->cursor, &job->source, end_point);
        for (size_t i = slice.begin; i < slice.end; i++) {
                StringView_append_chr(&job->sink, job->source.at[i]);
        }
        return serial;
}


void ooo_set_indentation(
        Cursor *cursor,
        StringView *sink,
        StringView *source,
        TSNode node,
        size_t indentation_level
) {
        char const *type_name = ts_node_type(node);
        TSSymbol str_symbol = ts_node_symbol(node);

        size_t cx = cursor->idx;
        size_t sx = Cursor_move_to_point(
                cursor, 
                source, 
                ts_node_start_point(node)
        );
        //printf("%zu %zu \n", cx, sx);
        indentation_level = ooo_indentation(OOO_ENTRY, node, indentation_level);
        for (size_t i = cx; i < sx; i++) {
                if (source->at[i] == '\n') {
                        StringView_append_chr(sink, source->at[i]);
                        printf("%c", source->at[i]);
                        StringView_append_spaces(sink, 4 * indentation_level);
                } else {
                        StringView_append_chr(sink, source->at[i]);
                        printf("%c", source->at[i]);
                }
        }

        indentation_level = ooo_indentation(OOO_NEXT, node, indentation_level);
        for (size_t it = 0; it < ts_node_child_count(node); it++) {
                TSNode child = ts_node_child(node, it);
                ooo_set_indentation(
                        cursor,
                        sink,
                        source,
                        child,
                        indentation_level
                );
        }
        size_t ax = cursor->idx;
        size_t ex = Cursor_move_to_point(cursor, source, ts_node_end_point(node));
#if 1
        if (str_symbol == sym_preproc_def) {
                /* preproc_def node includes the \n            */
                /* \n is then not used for indentation         */
                /* Cursor_decrement set the ex index before \n */
                ex = Cursor_decrement(cursor, source);
        } 
#endif
        for (size_t i = ax; i < ex; i++) {
                StringView_append_chr(sink, source->at[i]);
                printf("%c", source->at[i]);
        }
}

void ooo_print_nodes(
        TSNode node,
        size_t row_begin,
        size_t row_end,
        size_t level
) {
        TSPoint start = ts_node_start_point(node);
        TSPoint end = ts_node_end_point(node);
        size_t num_of_childs = ts_node_child_count(node);

        if (level == 0) {
                printf("NodeID| Start  | End    | Level-> NodeName  num_of_childs\n");
        }
        if (start.row >= row_begin && start.row < row_end) {
                printf("%4d  |%4d:%-3d|%4d:%-3d|%3zu",
                        ts_node_symbol(node),
                        start.row,
                        start.column,
                        end.row,
                        end.column,
                        level
                );
                for (size_t i = 0; i < level; i++) {
                        printf("-");
                }
                printf("> %s %zu\n",
                        ts_node_type(node),
                        num_of_childs
                );
        }

        level++;
        for (size_t it = 0; it < num_of_childs; it++) {
                TSNode child = ts_node_child(node, it);
                ooo_print_nodes(child, row_begin, row_end, level);
        }
}
#endif
#undef OOO_IMPLEMENTATION