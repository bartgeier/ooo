#include "OArg.h"

#define ARQ_IMPLEMENTATION
#include "arq.h"
#include "../build/license.c"

static OArg_t *global_m;

static void fn_version(Arq_Queue *queue) { 
        (void)queue;
        global_m->flag.version = true;
}

static void fn_help(Arq_Queue *queue) {
        (void) queue;
        global_m->flag.help = true;
}

static void fn_license(Arq_Queue *queue) {
        (void) queue;
        global_m->flag.license = true;
}

static void fn_print(Arq_Queue *queue) {
        global_m->flag.print = true;
        uint32_t begin = arq_uint(queue);
        uint32_t end = arq_uint(queue);
        if (begin > end) {
                global_m->print.row_begin = end;
                global_m->print.row_end = begin;
                return;
        }
        global_m->print.row_begin = begin;
        global_m->print.row_end = end;
}

static void fn_input(Arq_Queue *queue) {
        global_m->input_path = arq_cstr_t(queue);
}

static void fn_output(Arq_Queue *queue) { 
        global_m->output_path = arq_cstr_t(queue);
}

static void fn_indent(Arq_Queue *queue) {
        global_m->flag.indent = true;
        global_m->indent = arq_uint(queue);
}

static void fn_Kernighan_and_Ritchie(Arq_Queue *queue) {
        (void) queue;
        // If K&R style is enabled,
        // the opening brace of a function is placed
        // on a new line.
        global_m->flag.KR = true;
}

OArg_t OArg_init(int argc, char **argv) {
        OArg_t m = {0};
        m.input_path = NULL;
        m.output_path = NULL;
        m.indent = 4;
        global_m = &m;

        char arena_buffer[1000];
        Arq_Option options[] = {
                {'v', "version", fn_version, "()"},
                {'h', "help",    fn_help, "()"},
                {'l', "license", fn_license, "()"},
                {'p', "print",   fn_print, "(uint row_begin = 0, uint row_end = 0xFFFFFFFF)"},
                {'i', "input",   fn_input, "(cstr_t file_path)"},
                {'o', "output",  fn_output, "(cstr_t file_path)"},
                {'t', "indent",  fn_indent, "(uint indent)"},
                {'k', "kr",      fn_Kernighan_and_Ritchie, "()"},
        };
        if (0 < arq_verify(arena_buffer, sizeof(arena_buffer), options, sizeof(options)/sizeof(Arq_Option))) {
                printf("%s\n", arena_buffer);
                m.action = OARG_ERROR;
                return m;
        }
        if (0 < arq_fn(argc, argv, arena_buffer, sizeof(arena_buffer), options, sizeof(options)/sizeof(Arq_Option))) {
                printf("%s\n", arena_buffer);
                m.action = OARG_ERROR;
                return m;
        }

        if (m.flag.version) {
                printf("2\n");
                printf("\n");
        }
        if (m.flag.help) {
                printf("./ooo -i ./example/hello.c -o ./example/hello.c   Apply code style: input -> output file\n");
                printf("\n");
                printf("./ooo -i ./example/hello.c -o *                   Apply code style in-place (overwrite input file)\n");
                printf("\n");
                printf("./ooo -i ./example/hello.c -o -                   Print styled code to stdout\n");
                printf("\n");
                printf("./ooo --kr -t8 -i./example/hello.c -o*            -t8 8 spaces indentation,\n");
                printf("                                                  --kr places function opening braces on a new line (Kernighan & Ritchie style variant)\n");
                printf("\n");
                printf("./ooo -p=42=56 -i./example/hello.c                Print Tree-sitter output for a range of source lines to stdout\n");
                printf("                                                  (row_end is not included in the output)\n");
                printf("\n");
                for (size_t i = 0; i < sizeof(options)/sizeof(Arq_Option); i++) {
                        printf("-%c --%s%s\n", options[i].chr, options[i].name, options[i].arguments);
                }
                printf("\n");
                printf("https://github.com/bartgeier/ooo \n");
                printf("Command line parser => https://github.com/bartgeier/arq \n");
                printf("\n");
        }
        if (m.flag.license) {
                printf("%s\n", license);
        }
        if (m.input_path == NULL && m.output_path == NULL && !m.flag.print 
        && (m.flag.version || m.flag.help || m.flag.license)) {
                m.action = OARG_NO_ACTION;
                return m;
        }
        if (m.input_path == NULL) {
                fprintf(stderr, "no input file path\n");
                m.action = OARG_ERROR;
                return m;
        }
        if (m.flag.print) {
                if (m.print.row_begin == m.print.row_end) {
                        uint32_t const i = 2;
                        assert(options[i].chr == 'p');
                        fprintf(stderr, "Input error:\n");
                        fprintf(stderr, "    row_begin == row_end\n");
                        fprintf(stderr, "    -%c --%s%s\n", options[i].chr, options[i].name, options[i].arguments);
                        m.action = OARG_ERROR;
                        return m;
                }
                if (m.output_path != NULL) {
                        uint32_t const i = 2;
                        uint32_t const n = 4;
                        assert(options[i].chr == 'p');
                        assert(options[n].chr == 'o');
                        fprintf(stderr, "Input error:\n");
                        fprintf(stderr, "    The following options cannot be used together:\n");
                        fprintf(stderr, "    -%c --%s%s\n", options[i].chr, options[i].name, options[i].arguments);
                        fprintf(stderr, "    -%c --%s%s\n", options[n].chr, options[n].name, options[n].arguments);
                        m.action = OARG_ERROR;
                        return m;
                }
                if (m.flag.KR) {
                        uint32_t const i = 2;
                        uint32_t const n = 6;
                        assert(options[i].chr == 'p');
                        assert(options[n].chr == 'k');
                        fprintf(stderr, "Input error:\n");
                        fprintf(stderr, "    The following options cannot be used together:\n");
                        fprintf(stderr, "    -%c --%s%s\n", options[i].chr, options[i].name, options[i].arguments);
                        fprintf(stderr, "    -%c --%s%s\n", options[n].chr, options[n].name, options[n].arguments);
                        m.action = OARG_ERROR;
                        return m;
                }
                if (m.flag.indent) {
                        uint32_t const i = 2;
                        uint32_t const n = 5;
                        assert(options[i].chr == 'p');
                        assert(options[n].chr == 't');
                        fprintf(stderr, "Input error:\n");
                        fprintf(stderr, "    The following options cannot be used together:\n");
                        fprintf(stderr, "    -%c --%s%s\n", options[i].chr, options[i].name, options[i].arguments);
                        fprintf(stderr, "    -%c --%s%s\n", options[n].chr, options[n].name, options[n].arguments);
                        m.action = OARG_ERROR;
                        return m;
                }
                m.action = OARG_PRINT;
                return m;
        }
        if (m.output_path == NULL) {
                fprintf(stderr, "no output file path\n");
                m.action = OARG_ERROR;
                return m;
        }
        if (0 == strcmp(m.output_path, "*")) {
                m.output_path = m.input_path;
        }
        m.action = OARG_STYLE;
        return m;
}
