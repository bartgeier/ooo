#include "OArq.h"

#define ARQ_IMPLEMENTATION
#include "arq.h"

OArq_t *ctx;

void fn_version(Arq_Queue *queue) { 
        (void)queue;
        ctx->flag.version = true;
}

void fn_print(Arq_Queue *queue) {
        ctx->action = OARQ_PRINT;
        uint32_t begin = arq_uint(queue);
        uint32_t end = arq_uint(queue);
        if (begin > end) {
                ctx->print.row_begin = end;
                ctx->print.row_end = begin;
                return;
        }
        ctx->print.row_begin = begin;
        ctx->print.row_end = end;
}

void fn_input(Arq_Queue *queue) {
        if (ctx->action == OARQ_NO_ACTION) {
                ctx->action = OARQ_STYLE;
        }
        ctx->input_path = arq_cstr_t(queue);
}

void fn_output(Arq_Queue *queue) { (void)queue;
        if (ctx->action == OARQ_NO_ACTION) {
                ctx->action = OARQ_STYLE;
        }
        ctx->output_path = arq_cstr_t(queue);
}

void fn_help(Arq_Queue *queue) {
        (void) queue;
        ctx->flag.help = true;
}

int OArq_init(OArq_t *m, int argc, char **argv) {
        ctx = m;
        ctx->flag.version = false;
        ctx->flag.help = false;
        ctx->action = OARQ_NO_ACTION;
        ctx->input_path = NULL;
        ctx->output_path = NULL;

        char arena_buffer[1000];
        Arq_Option options[] = {
                {'v', "version", fn_version, "()"},
                {'h', "help",    fn_help, "()"},
                {'p', "print",   fn_print, "(uint row_begin = 0, uint row_end = 0xFFFFFFFF)"},
                {'i', "input",   fn_input, "(cstr_t file_path)"},
                {'o', "output",  fn_output, "(cstr_t file_path)"},
        };
        if (0 < arq_verify(arena_buffer, sizeof(arena_buffer), options, sizeof(options)/sizeof(Arq_Option))) {
                printf("%s\n", arena_buffer);
                return 1;
        }
        if (0 < arq_fn(argc, argv, arena_buffer, sizeof(arena_buffer), options, sizeof(options)/sizeof(Arq_Option))) {
                printf("%s\n", arena_buffer);
                return 1;
        }

        if (ctx->flag.version) {
                printf("1\n");
        }
        if (ctx->flag.help) {
                printf("./ooo -i./example/hello.c -o./example/hello.c              Apply code style -i input to -o output\n");
                printf("./ooo -i./example/hello.c -o*                              Apply code style -i input to input => output file is the input file\n");
                printf("./ooo -i./example/hello.c -o-                              Print code style to terminal\n");
                printf("./ooo -p 42 56 -i./example/hello.c -o./tree_output         Print tree-sitter lines -p \"firstLine numOfLine\"\n");
                printf("./ooo -p=42=56 -i./example/hello.c -o-                     Print tree-sitter to terminal\n");
                printf("\n");
                for (size_t i = 0; i < sizeof(options)/sizeof(Arq_Option); i++) {
                        printf("-%c --%s%s\n", options[i].chr, options[i].name, options[i].arguments);
                }
                printf("\n");
                printf("https://github.com/bartgeier/ooo \n");
                printf("Command line parser => https://github.com/bartgeier/arq \n");
        }
        if (ctx->action == OARQ_NO_ACTION) {
                return 0;
        }

        if (ctx->action == OARQ_PRINT) {
                if (m->print.row_begin == m->print.row_end) {
                        uint32_t const i = 2;
                        assert(options[i].chr == 'p');
                        fprintf(stderr, "Input error:\n");
                        fprintf(stderr, "    row_begin == row_end\n");
                        fprintf(stderr, "    -%c --%s%s\n", options[i].chr, options[i].name, options[i].arguments);
                        return 1;
                }
        }

        if (ctx->input_path == NULL) {
                fprintf(stderr, "no input file path\n");
                return 1;
        }
        if (ctx->output_path == NULL) {
                fprintf(stderr, "no output file path\n");
                return 1;
        }
        if (m->output_path != 0 && strcmp(m->output_path, "*") == 0) {
                ctx->output_path = ctx->input_path;
        }
        return 0; // no error, successful
}
