/* https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html */
#include "OArg.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>

#define OARG_ASSERT(x, msg) assert((x && msg))

static void swap_size_t(size_t *x, size_t *y) {
        if (x != y) {
                *x = *x + *y;
                *y = *x - *y;
                *x = *x - *y;
        }
}

static int str_to_num(char *s, size_t *result) {
        errno = 0;
        char *endptr;
        size_t n = strtol(s, &endptr, 10); 
        if (endptr == s) {
                return 1; //failure
        }
        *result = n;
        return 0; // successful
}

typedef struct {
        char split;
        bool is_changed;
        char* at;
} OArgSplit;

static OArgSplit create_split(char *vstr, char const split) {
        OArgSplit m = {
                .split = split,
                .is_changed = false,
                .at = vstr,
                .is_changed = false,
        };
        return m;
}

/* repairs the last split */
static char *unsplit(OArgSplit *m) {
        if (m->is_changed) {
                m->at[-1] = m->split;
                m->is_changed = false;
        }
        return m->at; // returns tail
}

static char *next_split(OArgSplit *m) {
        unsplit(m);
        if (m->at[0] == 0) {
                return m->at;
        }
        char *result = m->at;
        size_t i = 0;
        while (m->at[i] != m->split & m->at[i] != 0) {
                i++;
        }
        if (m->at[i] == m->split) {
                m->is_changed = true;
                m->at[i] = 0;
                i++;
        }
        m->at = &m->at[i];
        return result;
}

static char* next_argument(OArgSplit *split, char *optarg) {
        char *result = NULL;
        do {
                result = next_split(split);
        } while ((result[0] == 0) & split->is_changed);
        return result;
}

static int set_print(OArg_t *m, char *optarg) {
        int error;
        OArgSplit split = create_split(optarg, ' ');
        {
                error = str_to_num(
                        next_argument(&split, optarg),
                        &m->print.row_begin
                ); 
                error |= str_to_num(
                        next_argument(&split, optarg),
                        &m->print.row_end
                );
        }
        unsplit(&split);

        if (error | (m->print.row_begin == m->print.row_end)) {
                m->print.failure = 1;
                return 1;
        }
        if (m->print.row_begin > m->print.row_end) {
                swap_size_t(&m->print.row_begin, &m->print.row_end);
        }
        return 0;
}

static int parse(OArg_t *m, int argc, char **argv) {
        while (1) {
                static struct option options[] = {
                        // {"verbose", required_argument, &verbose_flag, 1},
                        {"help", no_argument, 0, 'h'},
                        {"print", required_argument, NULL, 'p'},
                        {"input", required_argument, NULL, 'i'},
                        {"output", required_argument, NULL, 'o'},
                        {0, 0, 0, 0}
                };

                int index = 0;
                int const c = getopt_long (
                        argc, 
                        argv, 
                        "h?p:i:o:",
                        options,
                        &index
                );

                switch (c) { 
                case -1:
                        if (optind < argc) {
                                /* Print any remaining                   */ 
                                /* command line arguments (not options). */
                                printf("non-option argv-elements: ");
                                while (optind < argc) {
                                        printf ("%s ", argv[optind++]);
                                }
                                putchar ('\n');
                        }
                        return 0;
                case 0:
                        // printf("option=%s flag=%d", options[index].name, verbose_flag);
                        if (optarg) printf(" arg=%s", optarg);
                        printf ("\n");
                        break;
                 case 'h':
                        m->action = OARG_HELP;
                        puts("? todo implementation of help\n");
                        break;
                case '?':
                        /* if parse error this is called automatically */
                        puts("? todo implementation of help\n");
                        break;
                case 'p':
                        m->action = OARG_PRINT;
                        set_print(m, optarg);
                        break;
                case 'i':
                        m->input_path = (optarg[0] == 0) ? NULL : optarg;
                        break;
                case 'o':
                        m->output_path = (optarg[0] == 0) ? NULL : optarg;
                        break;
                default:
                        m = NULL;
                        printf("default");
                        return 1;
                }
        }
}

static int verify_style(OArg_t *m) {
        int error = 0;
        switch (m->action) {
        case OARG_HELP:
                return 0;
        case OARG_STYLE:
                if (m->input_path == NULL) {
                        printf("-i Missing input path.\n");
                        error = 1;
                }
                if (m->output_path == NULL) {
                        printf("-o Missing output path.\n");
                        error = 1;
                } 
                if (error) {
                        printf("./ooo -i./example/hello.c -o./example/hello.c\n");
                        return error; 
                }
                return 0;
        case OARG_PRINT:
                if (m->print.failure == 1) {
                        printf("-p Row range failure.\n");
                        error = 1;
                }
                if (m->input_path == NULL) {
                        printf("-i Input path failure.\n");
                        error = 1;
                }
                if (m->output_path == NULL) {
                        printf("-o Output path failure.\n");
                        error = 1;
                } 
                if (error) {
                        printf("./ooo -p \"42 56\" -i./example/hello.c -o./tree_output\n");
                        return error;
                }
                return 0;
        default:
                return 0;
        }
}

int OArg_init(OArg_t *m, int argc, char **argv) {
        int error = 0;
        error |= parse(m, argc, argv);
        error |= verify_style(m); 
        return error;
}

//#endif
