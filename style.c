#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdio.h>

#define I_PATH "examples/Double_Page_Flash/"
#define GENERATE_PATH "ooo_generate/Double_Page_Flash/"
#define O_PATH GENERATE_PATH

bool create_directorys_ooo_gererate(const bool clean) {
    if (clean) {
        return  nob_remove("./ooo_generate");
    }
    nob_mkdir_if_not_exists("./ooo_generate");
    nob_mkdir_if_not_exists("./ooo_generate/Double_Page_Flash");
    nob_mkdir_if_not_exists("./ooo_generate/Double_Page_Flash/example");
    nob_mkdir_if_not_exists("./ooo_generate/Double_Page_Flash/unitTests");
    return nob_mkdir_if_not_exists("./ooo_generate/Double_Page_Flash/source");
}

bool ooo_style(const char *file_path) {
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
    bool ok = nob_cmd_run_sync(cmd);
    nob_cmd_free(cmd);
    nob_sb_free(sbi);
    nob_sb_free(sbo);
    return ok;
}

int main(int argc, char **argv) {
    nob_log(NOB_INFO, "version 3");
    bool result = true;
    NOB_GO_REBUILD_URSELF(argc, argv);      
    #ifdef _WIN32
        ok |= !nob_remove("style.exe.old");
        if (nob_file_exists("style.exe.old")) {                               
            nob_log(NOB_INFO, "RM: Don't worry `style.exe.old` will be deleted next time!");  
        }
    #else
        result |= nob_remove("style.old");
    #endif 
    if (argc > 1) nob_shift_args(&argc, &argv);
    const bool clean = strcmp(nob_shift_args(&argc, &argv), "clean") == 0;
    if (!create_directorys_ooo_gererate(clean)) nob_return_defer(false);
    if (!clean) {
        nob_log(NOB_INFO, "source/");
        result &= ooo_style("source/FlashState.h");
        result &= ooo_style("source/FlashState.c");
        result &= ooo_style("source/FlashStream.h");
        result &= ooo_style("source/FlashStream.c");
        result &= ooo_style("source/FlashStreamView.h");

        nob_log(NOB_INFO, "example/");
        result &= ooo_style("example/FlashConfig.h");
        result &= ooo_style("example/Flash_impl.h");
        result &= ooo_style("example/Flash_impl.c");
        result &= ooo_style("example/LogQueue.h");
        result &= ooo_style("example/LogQueue.c");
        result &= ooo_style("example/PERSISTENT.h");
        result &= ooo_style("example/PERSISTENT.c");
        result &= ooo_style("example/main.cpp");
        result &= ooo_style("example/spi_hardware.h");
        result &= ooo_style("example/spi_hardware.c");

        nob_log(NOB_INFO, "unitTests/");
        result &= ooo_style("unitTests/stub_TEST_FLASH.h");
        result &= ooo_style("unitTests/stub_TEST_FLASH.cpp");
        result &= ooo_style("unitTests/test_FlashState.cpp");
        result &= ooo_style("unitTests/test_FlashStream.cpp");
        result &= ooo_style("unitTests/test_FlashStream_read_record.cpp");
    }
defer:
    if (!result) { 
        nob_log(NOB_ERROR, "style exit with an error");
        return 2;
    }
    nob_log(NOB_INFO, "style exit successful");
    return 0;
}
