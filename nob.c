#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdbool.h>

#define OOO_BUILD_DIR "build"

bool create_build_dir(bool const clean) {
        if (clean) {
                return nob_remove(OOO_BUILD_DIR);
        }
        return nob_mkdir_if_not_exists(OOO_BUILD_DIR);
}

bool treesitter_download_build(bool const clean) {
#if 1
        #define TS_URL "https://github.com/tree-sitter/tree-sitter/archive/refs/heads/"
        #define TS_COMMIT "master"
#else
        #define TS_URL "https://github.com/tree-sitter/tree-sitter/archive/"
        #define TS_COMMIT "0ff28346be3d27f935d7cde8bbdf6b621c268e1a"
#endif
        bool ok = true;
        if (clean) {
                ok &= nob_remove("tree-sitter.zip");
                ok &= nob_remove("tree-sitter-"TS_COMMIT);
                ok &= nob_remove("tree-sitter");
                return ok;
        }
        if (nob_file_exists("tree-sitter")) return true;
        nob_log(NOB_INFO,"DOWNLOAD BUILD: tree-sitter -> tree-sitter.a");

        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "curl");
        nob_cmd_append(&cmd, "-L");
        nob_cmd_append(&cmd, TS_URL TS_COMMIT".zip");
        nob_cmd_append(&cmd, "--output", "tree-sitter.zip");
        ok &= nob_cmd_run_sync(cmd);

        cmd.count = 0;
        nob_log(NOB_INFO,"unzip");
        nob_cmd_append(&cmd, "unzip", "tree-sitter.zip");
        ok &= nob_cmd_run_sync(cmd);
        cmd.count = 0;
        ok &= nob_remove("tree-sitter.zip");
        #ifdef _WIN32
                nob_cmd_append(&cmd, "gcc");
                nob_cmd_append(&cmd, "-I", "tree-sitter-"TS_COMMIT"/lib/src", "-I", "tree-sitter-"TS_COMMIT"/lib/include");
                nob_cmd_append(&cmd, "-c","-o","tree-sitter-"TS_COMMIT"/lib.o", "tree-sitter-"TS_COMMIT"/lib/src/lib.c");
                ok &= nob_cmd_run_sync(cmd);
                cmd.count = 0;

                nob_cmd_append(&cmd,"ar", "rcs", "tree-sitter-"TS_COMMIT"/libtree-sitter.a", "tree-sitter-"TS_COMMIT"/lib.o");
                ok &= nob_cmd_run_sync(cmd);
                cmd.count = 0;
        #else
                nob_cmd_append(&cmd, "make", "-C", "tree-sitter-"TS_COMMIT);
                ok &= nob_cmd_run_sync(cmd);
                cmd.count = 0;
        #endif
        ok &= nob_mkdir_if_not_exists("tree-sitter");
        ok &= nob_mkdir_if_not_exists("tree-sitter/lib");
        ok &= nob_mkdir_if_not_exists("tree-sitter/lib/include");
        ok &= nob_mkdir_if_not_exists("tree-sitter/lib/include/tree_sitter");
        ok &= nob_rename("tree-sitter-"TS_COMMIT"/LICENSE", "tree-sitter/LICENSE");
        ok &= nob_rename("tree-sitter-"TS_COMMIT"/libtree-sitter.a", "tree-sitter/libtree-sitter.a");
        ok &= nob_rename("tree-sitter-"TS_COMMIT"/lib/include/tree_sitter/api.h", "tree-sitter/lib/include/tree_sitter/api.h");
        ok &= nob_remove("tree-sitter-"TS_COMMIT);
        nob_cmd_free(cmd);
        return ok;
}

bool tree_sitter_c_download(bool const clean) {
#if 1
        #define TS_C_URL "https://github.com/tree-sitter/tree-sitter-c/archive/refs/heads/"
        #define TS_C_COMMIT "master"
#else
        #define TS_C_URL "https://github.com/tree-sitter/tree-sitter-c/archive/"
        #define TS_C_COMMIT "212a80f86452bb1316324fa0db730cf52f29e05a"
#endif
        bool ok = true;
        if (clean) {
                ok &= nob_remove("tree-sitter-c.zip");
                ok &= nob_remove("tree-sitter-c-"TS_C_COMMIT);
                ok &= nob_remove("tree-sitter-c"); 
                return ok;
        }
        if (nob_file_exists("tree-sitter-c")) return true;
        nob_log(NOB_INFO,"DOWNLOAD: tree-sitter-c -> parser.c");
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "curl"); 
        nob_cmd_append(&cmd, "-L");
        nob_cmd_append(&cmd, TS_C_URL TS_C_COMMIT".zip");
        nob_cmd_append(&cmd, "--output", "tree-sitter-c.zip");
        ok &= nob_cmd_run_sync(cmd);
        
        cmd.count = 0;
        nob_cmd_append(&cmd, "unzip", "tree-sitter-c.zip");
        ok &= nob_cmd_run_sync(cmd);
        nob_cmd_free(cmd);
        ok &= nob_remove("tree-sitter-c.zip");
        ok &= nob_mkdir_if_not_exists("tree-sitter-c");
        ok &= nob_mkdir_if_not_exists("tree-sitter-c/src");
        ok &= nob_mkdir_if_not_exists("tree-sitter-c/src/tree_sitter");
        ok &= nob_rename("tree-sitter-c-"TS_COMMIT"/LICENSE", "tree-sitter-c/LICENSE");
        ok &= nob_rename("tree-sitter-c-"TS_C_COMMIT"/src/tree_sitter/parser.h", "tree-sitter-c/src/tree_sitter/parser.h");
        ok &= nob_rename("tree-sitter-c-"TS_C_COMMIT"/src/parser.c", "tree-sitter-c/src/parser.c");
        ok &= nob_remove("tree-sitter-c-"TS_C_COMMIT);
        return ok;
}

bool googleTest_download_build(bool const clean) {
#if 1
        #define GTEST_URL "https://github.com/google/googletest/archive/refs/heads/"
        #define GTEST_COMMIT "main"
#else
        #define GTEST_URL "https://github.com/google/googletest/archive/"
        #define GTEST_COMMIT "76bb2afb8b522d24496ad1c757a49784fbfa2e42"
#endif
        bool ok = true;
        if (clean) {
                ok &= nob_remove("googletest.zip");
                ok &= nob_remove("googletest-"GTEST_COMMIT);
                ok &= nob_remove("googletest");
                return ok;
        }
        if (nob_file_exists("googletest")) return true;
        nob_log(NOB_INFO, "DOWNLOAD BUILD: googletest");
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "curl");
        nob_cmd_append(&cmd, "-L");
        nob_cmd_append(&cmd,GTEST_URL GTEST_COMMIT".zip");
        nob_cmd_append(&cmd, "--output", "googletest.zip");
        ok &= nob_cmd_run_sync(cmd);
        cmd.count = 0;

        nob_cmd_append(&cmd, "unzip", "googletest.zip");
        ok &= nob_cmd_run_sync(cmd);
        cmd.count = 0;
        ok &= nob_remove("googletest.zip");
        ok &= nob_mkdir_if_not_exists("googletest-"GTEST_COMMIT"/build");
        #ifdef _WIN32
                nob_cmd_append(&cmd, "cmake");
                nob_cmd_append(&cmd, "-DCMAKE_CXX_COMPILER=g++");
                nob_cmd_append(&cmd, "-DCMAKE_CC_COMPILER=gcc");
                nob_cmd_append(&cmd, "-DCMAKE_MAKE_PROGRAM=mingw32-make");
                nob_cmd_append(&cmd, "-G", "MinGW Makefiles");
                nob_cmd_append(&cmd, "-Hgoogletest-"GTEST_COMMIT);
                nob_cmd_append(&cmd, "-DBUILD_SHARED_LIBS=OFF", "-DBUILD_GMOCK=OFF");
                nob_cmd_append(&cmd, "-Bgoogletest-"GTEST_COMMIT"/build");
                ok &= nob_cmd_run_sync(cmd);
                cmd.count = 0;

                nob_cmd_append(&cmd, "mingw32-make", "-C", "googletest-"GTEST_COMMIT"/build");
                ok &= nob_cmd_run_sync(cmd);
                cmd.count = 0;
        #else       
                nob_cmd_append(&cmd, "cmake");
                nob_cmd_append(&cmd, "-Hgoogletest-"GTEST_COMMIT);
                nob_cmd_append(&cmd, "-DBUILD_SHARED_LIBS=OFF", "-DBUILD_GMOCK=OFF", "-Dgtest_disable_pthreads=ON");
                nob_cmd_append(&cmd, "-Bgoogletest-"GTEST_COMMIT"/build");
                ok &= nob_cmd_run_sync(cmd);
                cmd.count = 0;

                nob_cmd_append(&cmd, "make", "-C", "googletest-"GTEST_COMMIT"/build");                
                ok &= nob_cmd_run_sync(cmd);
                cmd.count = 0;
        #endif  
        nob_cmd_free(cmd);
        ok &= nob_mkdir_if_not_exists("googletest");
        ok &= nob_mkdir_if_not_exists("googletest/build");
        ok &= nob_mkdir_if_not_exists("googletest/include");
        ok &= nob_rename("googletest-"GTEST_COMMIT"/LICENSE", "googletest/LICENSE");
        ok &= nob_rename("googletest-"GTEST_COMMIT"/build/lib", "googletest/build/lib/");
        ok &= nob_rename("googletest-"GTEST_COMMIT"/googletest/include/gtest", "googletest/include/gtest/");
        ok &= nob_remove("googletest-"GTEST_COMMIT);
        return ok;
}

bool ooo_copy_treesitter_symbols_build(bool clean) {
        if (clean) {
                return nob_remove("copy_treesitter_symbols");
        }
        if (nob_file_exists("tree-sitter-c/ooo_treesitter_symbol_ids.h")) return true;
        nob_log(NOB_INFO, "BUILD: copy_treesitter_symbols ----> copy enumerator");
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "gcc");
        nob_cmd_append(&cmd, "-ggdb", "-pedantic");
        nob_cmd_append(&cmd, "-I", "tree-sitter/lib/include/");
        nob_cmd_append(&cmd, "source/copy_treesitter_symbols.c"); 
        nob_cmd_append(&cmd, "-o", "ooo_copy_treesitter_symbols");
        nob_cmd_append(&cmd, "tree-sitter/libtree-sitter.a",  "tree-sitter-c/src/parser.c");;
        bool ok = nob_cmd_run_sync(cmd);
        cmd.count = 0;
        #ifdef _WIN32
                ok &= nob_rename("ooo_copy_treesitter_symbols.exe", "build/ooo_copy_treesitter_symbols.exe");
                nob_cmd_append(&cmd,"build/ooo_copy_treesitter_symbols.exe" );
                ok &= nob_cmd_run_sync(cmd);
        #else
                ok &= nob_rename("ooo_copy_treesitter_symbols", "build/ooo_copy_treesitter_symbols");
                nob_cmd_append(&cmd,"build/ooo_copy_treesitter_symbols" );
                ok &= nob_cmd_run_sync(cmd);
        #endif
        nob_cmd_free(cmd);
        return ok;

}

bool ooo_build(bool const clean) {
        #define NOT_USED "-ggdb", "-std=c99"
        #define _CFLAGS  "-Wall", "-Wextra", "-pedantic"
        if (clean) {
                #ifdef _WIN32
                        return nob_remove("ooo.exe");
                #else
                        return nob_remove("ooo");
                #endif
        }
        nob_log(NOB_INFO, "BUILD: ooo ----> code styler");
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "gcc", "-ggdb", "-pedantic");
        nob_cmd_append(&cmd, "-I", "tree-sitter/lib/include/");
        nob_cmd_append(&cmd, "-I", "tree-sitter-c/");
        nob_cmd_append(&cmd, "-o", "ooo");
        nob_cmd_append(&cmd, "./source/main.c");
        nob_cmd_append(&cmd, "./tree-sitter/libtree-sitter.a", "./tree-sitter-c/src/parser.c");;
        nob_cmd_append(&cmd, "./source/ooo_runner.c", "./source/indentation.c", "./source/OArg.c");
        const bool ok = nob_cmd_run_sync(cmd);
        nob_cmd_free(cmd);
        return ok;
}

bool unittests_build(bool const clean) {
        if (clean) {
                #ifdef _WIN32
                        return nob_remove("otest.exe");
                #else
                        return nob_remove("otest");
                #endif
        }
        bool ok = true;
        nob_log(NOB_INFO, "BUILD: otest ----> unit tests");
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "g++", "-ggdb", "-O0", "-std=c++20", "-Wall", "-Wextra", "-pedantic", "-Wno-parentheses");
        nob_cmd_append(&cmd, "-I", "googletest/include/"); 
        nob_cmd_append(&cmd, "-I", "source/"); 
        nob_cmd_append(&cmd, "-I", "tree-sitter/lib/include/"); 
        nob_cmd_append(&cmd, "-L", "googletest/build/lib/");
        nob_cmd_append(&cmd, "-o", "otest");
        nob_cmd_append(&cmd, "unittests/tst_OStr.c");
        nob_cmd_append(&cmd, "tree-sitter/libtree-sitter.a");
        nob_cmd_append(&cmd, "-lgtest", "-lgtest_main");
        ok &= nob_cmd_run_sync(cmd);
        cmd.count = 0;
        nob_cmd_free(cmd);
        return ok;
}

int main(int argc, char **argv) {
        bool ok = true;
        NOB_GO_REBUILD_URSELF(argc, argv);
        #ifdef _WIN32
                ok &= nob_remove("nob.exe.old");
                if (nob_file_exists("nob.exe.old")) {                               
                        nob_log(NOB_INFO, "RM: Don't worry `nob.exe.old` will be deleted next time!");  
                }
        #else
                ok &= nob_remove("nob.old");
        #endif 
        struct {
                bool clean;
        } flag = { .clean = false };
        while (argc > 0) {
                char const *s = nob_shift_args(&argc, &argv);
                if (strcmp(s, "clean") == 0) flag.clean = true;
        }
        ok &= create_build_dir(flag.clean);
        ok &= treesitter_download_build(flag.clean);       
        ok &= tree_sitter_c_download(flag.clean);        
        ok &= googleTest_download_build(flag.clean);        
        ok &= ooo_copy_treesitter_symbols_build(flag.clean);
        ok &= ooo_build(flag.clean);
        ok &= unittests_build(flag.clean);
        if (!ok) {
                nob_log(NOB_ERROR, "Done  => One or more errors occurred!");
                return false;
        }
        nob_log(NOB_INFO ,"Successful done!");
        return 0;
}

/* https://docs.github.com/en/repositories/working-with-files/using-files/downloading-source-code-archives */
