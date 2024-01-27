#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdio.h>

#define I_PATH "examples/Double_Page_Flash/"
#define GENERATE_PATH "ooo_generate/Double_Page_Flash/"
#define O_PATH GENERATE_PATH


bool create_directorys_ooo_gererate(bool clean) {
        if (clean) {
                return  nob_rm("./ooo_generate");
        }
        nob_mkdir_if_not_exists("./ooo_generate");
        nob_mkdir_if_not_exists("./ooo_generate/Double_Page_Flash");
        nob_mkdir_if_not_exists("./ooo_generate/Double_Page_Flash/example");
        nob_mkdir_if_not_exists("./ooo_generate/Double_Page_Flash/unitTests");
        return nob_mkdir_if_not_exists("./ooo_generate/Double_Page_Flash/source");

}

int ooo_style(const char *file_path) {
        int error;
        Nob_String_Builder sbi = {0};
        nob_sb_append_cstr(&sbi, I_PATH);
        nob_sb_append_cstr(&sbi, file_path);
        nob_sb_append_null(&sbi);
        Nob_String_Builder sbo = {0};
        nob_sb_append_cstr(&sbo, O_PATH);
        nob_sb_append_cstr(&sbo, file_path);
        nob_sb_append_null(&sbo);

        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "./ooo");
        nob_cmd_append(&cmd, "-i", sbi.items, "-o", sbo.items);
        if (!nob_cmd_run_sync(cmd)) error = 1;
        nob_cmd_free(cmd);
        nob_sb_free(sbi);
        nob_sb_free(sbo);
        return error;
}

int main(int argc, char **argv) {
        int error = 0;
        NOB_GO_REBUILD_URSELF(argc, argv);
        error |= !nob_rm("style.old");

        if (argc > 1) nob_shift_args(&argc, &argv);
        bool clean = strcmp(nob_shift_args(&argc, &argv), "clean") == 0;

        if (!create_directorys_ooo_gererate(clean)) return 1;
        if (!clean) {
                nob_log(NOB_INFO, "source/");
                error |= ooo_style("source/FlashState.h");
                error |= ooo_style("source/FlashState.c");
                error |= ooo_style("source/FlashStream.h");
                error |= ooo_style("source/FlashStream.c");
                error |= ooo_style("source/FlashStreamView.h");

                nob_log(NOB_INFO, "example/");
                error |= ooo_style("example/FlashConfig.h");
                error |= ooo_style("example/Flash_impl.h");
                error |= ooo_style("example/Flash_impl.c");
                error |= ooo_style("example/LogQueue.h");
                error |= ooo_style("example/LogQueue.c");
                error |= ooo_style("example/PERSISTENT.h");
                error |= ooo_style("example/PERSISTENT.c");
                error |= ooo_style("example/main.cpp");
                error |= ooo_style("example/spi_hardware.h");
                error |= ooo_style("example/spi_hardware.c");

                nob_log(NOB_INFO, "unitTests/");
                error |= ooo_style("unitTests/stub_TEST_FLASH.h");
                error |= ooo_style("unitTests/stub_TEST_FLASH.cpp");
                error |= ooo_style("unitTests/test_FlashState.cpp");
                error |= ooo_style("unitTests/test_FlashStream.cpp");
                error |= ooo_style("unitTests/test_FlashStream_read_record.cpp");
        }
        return error;
}
