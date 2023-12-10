#define NOBUILD_IMPLEMENTATION
#include "./nobuild.h"
#include <stdbool.h>

#define OOO_BUILD_DIR "build"

void create_build_dir(bool const clean) {
        if (clean) {
                if (PATH_EXISTS(OOO_BUILD_DIR)) RM(OOO_BUILD_DIR);
                return;
        }
        if (PATH_EXISTS(OOO_BUILD_DIR)) return;
        MKDIRS(OOO_BUILD_DIR);
}

void treesitter_download_build(bool const clean) {
        #define TS_COMMIT "0ff28346be3d27f935d7cde8bbdf6b621c268e1a"
        if (clean) {
                if (PATH_EXISTS("tree-sitter.zip")) RM("tree-sitter.zip");
                if (PATH_EXISTS("tree-sitter-"TS_COMMIT)) RM("tree-sitter-"TS_COMMIT);
                if (PATH_EXISTS("tree-sitter")) RM("tree-sitter");
                return;
        }
        if (PATH_EXISTS("tree-sitter")) return;
        INFO("DOWNLOAD BUILD: tree-sitter -> tree-sitter.a");
        CMD(
            "curl", 
            "-L",
            "https://github.com/tree-sitter/tree-sitter/archive/"TS_COMMIT".zip",
            "--output", "tree-sitter.zip"
        );
        CMD("unzip", "tree-sitter.zip");
        RM("tree-sitter.zip");
        CMD("make", "-C", "tree-sitter-"TS_COMMIT);
        MKDIRS("tree-sitter");
        MKDIRS("tree-sitter/lib");
        MKDIRS("tree-sitter/lib/include");
        CMD("cp", "tree-sitter-"TS_COMMIT"/libtree-sitter.a", "tree-sitter/libtree-sitter.a");
        CMD("cp", "tree-sitter-"TS_COMMIT"/lib/include/tree_sitter", "-r", "tree-sitter/lib/include");
        RM("tree-sitter-"TS_COMMIT);
}

void tree_sitter_c_download(bool const clean) {
        #define TS_C_COMMIT "212a80f86452bb1316324fa0db730cf52f29e05a"
        if (clean) {
                if (PATH_EXISTS("tree-sitter-c.zip")) RM("tree-sitter-c.zip");
                if (PATH_EXISTS("tree-sitter-c-"TS_C_COMMIT)) RM("tree-sitter-c-"TS_C_COMMIT);
                if (PATH_EXISTS("tree-sitter-c")) RM("tree-sitter-c");
                return;
        }
        if (PATH_EXISTS("tree-sitter-c")) return;
        INFO("DOWNLOAD: tree-sitter-c -> parser.c");
        CMD(
            "curl", 
            "-L",
            "https://github.com/tree-sitter/tree-sitter-c/archive/"TS_C_COMMIT".zip",
            "--output", "tree-sitter-c.zip"
        );
        CMD("unzip", "tree-sitter-c.zip");
        RM("tree-sitter-c.zip");
        MKDIRS("tree-sitter-c");
        MKDIRS("tree-sitter-c/src");
        CMD("cp", "tree-sitter-c-"TS_C_COMMIT"/src/parser.c", "tree-sitter-c/src");
        RM("tree-sitter-c-"TS_C_COMMIT);
}

/* curl -L https://github.com/google/googletest/archive/76bb2afb8b522d24496ad1c757a49784fbfa2e42.zip --output googletest.zip */
void googleTest_download_build(bool const clean) {
        #define GTEST_COMMIT "76bb2afb8b522d24496ad1c757a49784fbfa2e42"
        if (clean) {
                if (PATH_EXISTS("googletest.zip")) RM("googletest.zip");
                if (PATH_EXISTS("googletest-"GTEST_COMMIT)) RM("googletest-"GTEST_COMMIT);
                if (PATH_EXISTS("googletest")) RM("googletest");
                return;
        }
        if (PATH_EXISTS("googletest")) return;
        INFO("DOWNLOAD BUILD: googletest");
        CMD(
            "curl", 
            "-L",
            "https://github.com/google/googletest/archive/"GTEST_COMMIT".zip",
            "--output", "googletest.zip"
        );
        CMD("unzip", "googletest.zip");
        RM("googletest.zip");
        CMD("mkdir", "googletest-"GTEST_COMMIT"/build");
        CMD(
                "cmake", 
                "-Hgoogletest-"GTEST_COMMIT, 
                "-DBUILD_SHARED_LIBS=OFF", "-DBUILD_GMOCK=OFF",
                "-Bgoogletest-"GTEST_COMMIT"/build"
        );
        CMD("make", "-C", "googletest-"GTEST_COMMIT"/build");
        MKDIRS("googletest");
        MKDIRS("googletest/build");
        CMD("cp", "googletest-"GTEST_COMMIT"/build/lib", "-r", "googletest/build/lib");
        CMD("mkdir", "googletest/include");
        CMD("cp", "googletest-"GTEST_COMMIT"/googletest/include/gtest", "-r", "googletest/include/gtest");
        RM("googletest-"GTEST_COMMIT);
}


void uinttests_build(bool const clean) {
        if (clean) {
                if (PATH_EXISTS("otest")) RM("otest");
                return;
        }
        INFO("BUILD: ooo uint tests");
        #ifndef _WIN32
                CMD(
                        "g++", "-Wall", "-Wextra", "-pedantic",
                        "-I", "googletest/include/", 
                        "-L", "googletest/build/lib/", 
                        "-L", "unittests/", 
                        "-o", "otest", "unittests/tst_hello.c",
                        "-lgtest", "-lgtest_main"
                );
        #else
                CMD("cl.exe", CFLAGS, "-I", OOO_INC, "-o", "main", OOO_SRC);
        #endif
}

void ooo_build(bool const clean) {
        #define NOT_USED "-ggdb", "-std=c99"
        #define _CFLAGS  "-Wall", "-Wextra", "-pedantic"
        #define CFLAGS "-ggdb", "-pedantic" 
        #define OOO_INC "tree-sitter/lib/include/" 
        #define OOO_SRC "./source/main.c", "./tree-sitter/libtree-sitter.a", "./tree-sitter-c/src/parser.c", "./source/ooo_runner.c"
        if (clean) {
                if (PATH_EXISTS("ooo")) RM("ooo");
                return;
        }
        INFO("BUILD: ooo code styler");
        #ifndef _WIN32
                CMD("gcc", CFLAGS, "-I", OOO_INC, "-o", "ooo", OOO_SRC);
        #else
                CMD("cl.exe", CFLAGS, "-I", OOO_INC, "-o", "main", OOO_SRC);
        #endif
}

void move_nobbuild_old(bool const clean) {
        if (clean) {
                if (PATH_EXISTS("nobuild.old")) RM("nobuild.old");
        }
        if (PATH_EXISTS("nobuild.old")) {
                RENAME("nobuild.old", PATH(OOO_BUILD_DIR, "nobuild.c"));                    \
                INFO("MOVE: nobuild.old -> build/nobuild.c");
        }
}

int main(int argc, char **argv) {
        GO_REBUILD_URSELF(argc, argv);
        struct {
                bool clean;
        } flag ={
                .clean = false
        };
        while (argc > 0) {
                char const *s = shift_args(&argc, &argv);
                if (strcmp(s, "clean") == 0) flag.clean = true;
        }
        create_build_dir(flag.clean);
        treesitter_download_build(flag.clean);
        tree_sitter_c_download(flag.clean);
        googleTest_download_build(flag.clean);
        uinttests_build(flag.clean);
        ooo_build(flag.clean);
        move_nobbuild_old(flag.clean);
        INFO("Successful done!");
        return 0;
}

/* https://docs.github.com/en/repositories/working-with-files/using-files/downloading-source-code-archives */
