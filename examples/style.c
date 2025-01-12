#define NOB_IMPLEMENTATION
#define NOB_TIME_CONTEXT CLOCK_MONOTONIC // CLOCK_PROCESS_CPUTIME_ID
#include "../nob.h"
#include <stdio.h>

bool create_directorys_ooo_gererate(const bool clean) {
        if (clean) {
                return  nob_remove("./styled_source");
        }
        return nob_mkdir_if_not_exists("./styled_source");
}

Nob_File_Paths list = {0};
void create_list_A(void) {
    nob_cmd_append(&list, "arr.c");
    nob_cmd_append(&list, "conway.c");
    nob_cmd_append(&list, "donut.c");
    nob_cmd_append(&list, "show.c");
}

#if 0
void print_line_count(Nob_File_Paths *file_list, char *input_dir) {
    Nob_Cmd cmd = {0};
    size_t total = 0;
    for (size_t i = 0; i < file_list->count; i++) {
        Nob_String_Builder sbi = {0};
        nob_sb_append_cstr(&sbi, input_dir);
        nob_sb_append_cstr(&sbi, file_list->items[i]);
        nob_sb_append_null(&sbi);
        char * file = nob_temp_strdup(sbi.items);
        nob_sb_free(sbi);

        size_t line_count = 0;
        {
            FILE *x = fopen(file, "r");
            assert(x != NULL && "Error opening file");
            char ch;
            while ((ch = fgetc(x)) != EOF) {
                if (ch == '\n') { line_count++; }
            }
            fclose(x);
        }
        printf("%10zu %s\n", line_count, file);

        total += line_count;
        nob_temp_reset(); 
    }

    printf("%10zu total lines\n", total);
    nob_cmd_free(cmd);
}
#else
void print_line_count(Nob_File_Paths *file_list, char *input_dir, uint64_t const duration) {
    Nob_Cmd cmd = {0};
    size_t total = 0;
    for (size_t i = 0; i < file_list->count; i++) {
        Nob_String_Builder sbi = {0};
        nob_sb_append_cstr(&sbi, input_dir);
        nob_sb_append_cstr(&sbi, file_list->items[i]);
        nob_sb_append_null(&sbi);
        char * file = nob_temp_strdup(sbi.items);
        nob_sb_free(sbi);

        size_t line_count = 0;
        {
            FILE *x = fopen(file, "r");
            assert(x != NULL && "Error opening file");
            char ch;
            while ((ch = fgetc(x)) != EOF) {
                if (ch == '\n') { line_count++; }
            }
            fclose(x);
        }
        printf("%10zu %s\n", line_count, file);

        total += line_count;
        nob_temp_reset(); 
    }

    printf("%10zu lines formated in %lu ms\n", total, duration);
    nob_cmd_free(cmd);
}        
#endif

bool style_list(Nob_File_Paths *file_list, char *output_dir, char *input_dir) {
    bool ok = true;
    for (size_t i = 0; i < file_list->count; i++) {
        Nob_String_Builder sbi = {0};
        nob_sb_append_cstr(&sbi, input_dir);
        nob_sb_append_cstr(&sbi, file_list->items[i]);
        nob_sb_append_null(&sbi);
        Nob_String_Builder sbo = {0};
        nob_sb_append_cstr(&sbo, output_dir);
        nob_sb_append_cstr(&sbo, file_list->items[i]);
        nob_sb_append_null(&sbo);

        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "../../ooo/build/ooo");
        nob_cmd_append(&cmd, "-i", sbi.items, "-o", sbo.items);
        ok &= nob_cmd_run_sync(cmd);
        nob_cmd_free(cmd);
        nob_sb_free(sbi);
        nob_sb_free(sbo);
    }
    return ok;
}

int main(int argc, char **argv) {
        bool result = true;
        NOB_GO_REBUILD_URSELF(argc, argv);      
        #ifdef _WIN32
                result |= !nob_remove("style.exe.old");
                if (nob_file_exists("style.exe.old")) {                               
                        nob_log(NOB_INFO, "RM: Don't worry `style.exe.old` will be deleted next time!");  
                }
        #else
                result |= nob_remove("style.old");
        #endif 

        if (argc > 1) nob_shift_args(&argc, &argv);
        const bool clean = strcmp(nob_shift_args(&argc, &argv), "clean") == 0;
        if (!create_directorys_ooo_gererate(clean)) {
                nob_log(NOB_ERROR, "style exit with an error!");
                return 1;
        }
        if (clean) {
                nob_log(NOB_INFO, "style clean exit successful!");
                return 0;
        }

        create_list_A();

        uint64_t t_start = nob_millis();
        result &= style_list(&list, "styled_source/", "source/");
        uint64_t duration = nob_millis() - t_start;
        printf("\n");
        
        nob_log(NOB_INFO, "ooo benchmark:");
        print_line_count(&list, "source/", duration);
        printf("\n");
      
        if (!result) { 
                nob_log(NOB_ERROR, "style exit with an error");
                return 2;
        }
        nob_log(NOB_INFO, "style exit successful");
        return 0;
}
