//#ifndef OARGV_H
//#define OARGV_H
/* https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html */
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
//#include <string.h>

static int verbose_flag;
#define OARG_ASSERT(x, msg) assert((x && msg))

typedef enum {
        OARG_STYLE,
        OARG_HELP,
        OARG_PRINT
} OArg_e;

typedef struct {
        OArg_e action;
        union {
                struct {
                        size_t row_begin;
                        size_t row_end;
                } print;
                struct {
                        char *input_path;
                        char *output_path;
                } style;
        };
} OArg_t;

#define QARG_STR_SIZE 100
typedef struct {
        char at[QARG_STR_SIZE];
} OArgStr;

void swap_size_t(size_t *x, size_t *y) {
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

static size_t split_length(char const *vstr, char const split) {
        size_t i = 0;
        while(vstr[i] != 0 & vstr[i] != split) {
                i++;
        }
        return i;
}

static OArgStr next_split(char **vstr, char split) {
        size_t len = split_length(*vstr, split);
        OARG_ASSERT(len < QARG_STR_SIZE, "split argument to long");
        OArgStr s = {0};
        size_t i = 0;
        for(; i < len; i++) {
                s.at[i] = (*vstr)[i];
        }
        s.at[i] = 0;
        *vstr = &(*vstr)[i + 1];
        return s;
}

static int set_print(OArg_t *m, char *optarg) {
        OArgStr begin = next_split(&optarg, ' ');
        OArgStr end = next_split(&optarg, ' ');
        int error = str_to_num(begin.at, &m->print.row_begin);
        error |= str_to_num(end.at, &m->print.row_end);
        if (error | (m->print.row_begin == m->print.row_end)) {
                printf("-p error => examples => -p \"43 65\" --print \"43 65\"\n");
                printf("print row begin %zu\n", m->print.row_begin);
                printf("print row end %zu\n", m->print.row_end);
                if (m->print.row_begin == m->print.row_end) {
                        printf("Must be => begin > end\n");
                }
                return 1;
        }
        if (m->print.row_begin > m->print.row_end) {
                swap_size_t(&m->print.row_begin, &m->print.row_end);
        }
        return 0;
}

int OArg_get(OArg_t *m, int argc, char **argv) {
        while (1) {
                static struct option options[] = {
                        {"verbose", required_argument, &verbose_flag, 1},
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
                        printf("return 0\n"); 
                        return 0;
                case 0:
                        printf("option=%s flag=%d", options[index].name, verbose_flag);
                        if (optarg) printf(" arg=%s", optarg);
                        printf ("\n");
                        break;
                 case 'h':
                        m->action = OARG_HELP;
                        puts("option h help\n");
                        break;
                case '?':
                        m->action = OARG_HELP;
                        puts("option ? help\n");
                        break;
                case 'p':
                        m->action = OARG_PRINT;
                        if (set_print(m, optarg)) {
                                return 1;
                        }
                        break;
                case 'i':
                        m->action = OARG_STYLE;
                        printf("option -d with value `%s'\n", optarg);
                        break;
                case 'o':
                        m->action = OARG_STYLE;
                        printf("option -f with value `%s'\n", optarg);
                        break;
                default:
                        m = NULL;
                        printf("default");
                        return 1;
                }
        }
}


int main(int argc, char **argv) {
        OArg_t oarg = {0};
        printf("Example for oarg!\n");
        return OArg_get(&oarg, argc, argv);
}

//#endif
