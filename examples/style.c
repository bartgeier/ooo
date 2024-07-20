#define NOB_IMPLEMENTATION
#include "../nob.h"
#include <stdio.h>

#define I_PATH "examples/Double_Page_Flash/"
#define GENERATE_PATH "ooo_generate/Double_Page_Flash/"
#define O_PATH GENERATE_PATH

bool create_directorys_ooo_gererate(const bool clean) {
    if (clean) {
        return  nob_remove("./styled_source");
    }
    return nob_mkdir_if_not_exists("./styled_source");
}

bool ooo_style(const char *input_path, const char *output_path) {
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "../build/ooo", "-i", input_path, "-o", output_path);
    bool ok = nob_cmd_run_async(cmd);
    nob_cmd_free(cmd);
    return ok;
}

int main(int argc, char **argv) {
        nob_log(NOB_INFO, "version 3");
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

        uint64_t t_start = nob_millis();

        result &= ooo_style("source/arr.c", "styled_source/arr.c");
        result &= ooo_style("source/conway.c", "styled_source/conway.c");
        result &= ooo_style("source/donut.c", "styled_source/donut.c");

        nob_log(NOB_INFO, "ooo styling duration %llu ms", nob_millis() - t_start);

        if (!result) { 
                nob_log(NOB_ERROR, "style exit with an error");
                return 2;
        }
        nob_log(NOB_INFO, "style exit successful");
        return 0;
}
