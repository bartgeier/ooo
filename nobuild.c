#define NOBUILD_IMPLEMENTATION
#include "./nobuild.h"
#include <stdbool.h>

#define NOT_USED "-ggdb", "-std=c99"
#define _CFLAGS  "-Wall", "-Wextra", "-pedantic"
#define CFLAGS "-pedantic" 
#define OOO_INC "tree-sitter/lib/include/" 
#define OOO_SRC "./source/main.c", "./tree-sitter/libtree-sitter.a", "./tree-sitter-c/src/parser.c"
#define OOO_BUILD_DIR "build"

void create_build_dir(bool const clean) {
        if (clean) {
                if (PATH_EXISTS(OOO_BUILD_DIR)) CMD("rm", "-r", OOO_BUILD_DIR);
                return;
        }
        if (PATH_EXISTS(OOO_BUILD_DIR)) return;
        MKDIRS(OOO_BUILD_DIR);
}

void move_nobbuild_old(bool const clean) {
        if (clean) return;
        if (PATH_EXISTS("nobuild.old")) {
                RENAME("nobuild.old", PATH(OOO_BUILD_DIR, "nobuild.c"));                    \
                INFO("MOVE: nobuild.old -> build/nobuild.c");
        }
}

void download_treesitter(bool const clean) {
        #define TS_COMMIT "0ff28346be3d27f935d7cde8bbdf6b621c268e1a"
        if (clean) {
                if (PATH_EXISTS("tree-sitter.zip")) CMD("rm", "-r", "tree-sitter.zip");
                if (PATH_EXISTS("tree-sitter-"TS_COMMIT)) CMD("rm", "-r", "tree-sitter-"TS_COMMIT);
                if (PATH_EXISTS("tree-sitter")) CMD("rm", "-r", "tree-sitter");
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
        CMD("rm", "-r", "tree-sitter.zip");
        CMD("make", "-C", "tree-sitter-"TS_COMMIT);
        MKDIRS("tree-sitter");
        MKDIRS("tree-sitter/lib");
        MKDIRS("tree-sitter/lib/include");
        CMD("cp", "tree-sitter-"TS_COMMIT"/libtree-sitter.a", "tree-sitter/libtree-sitter.a");
        CMD("cp", "tree-sitter-"TS_COMMIT"/lib/include/tree_sitter", "-r", "tree-sitter/lib/include");
        CMD("rm", "-r", "tree-sitter-"TS_COMMIT);
}

void download_tree_sitter_c(bool const clean) {
        #define TS_C_COMMIT "212a80f86452bb1316324fa0db730cf52f29e05a"
        if (clean) {
                if (PATH_EXISTS("tree-sitter-c.zip")) CMD("rm", "-r", "tree-sitter-c.zip");
                if (PATH_EXISTS("tree-sitter-c-"TS_C_COMMIT)) CMD("rm", "-r", "tree-sitter-c-"TS_C_COMMIT);
                if (PATH_EXISTS("tree-sitter-c")) CMD("rm", "-r", "tree-sitter-c");
                return;
        }
        if (PATH_EXISTS("tree-sitter-c")) return;
        INFO("DOWNLOAD BUILD: tree-sitter-c -> parser.c");
        CMD(
            "curl", 
            "-L",
            "https://github.com/tree-sitter/tree-sitter-c/archive/"TS_C_COMMIT".zip",
            "--output", "tree-sitter-c.zip"
        );
        CMD("unzip", "tree-sitter-c.zip");
        CMD("rm", "-r", "tree-sitter-c.zip");
        MKDIRS("tree-sitter-c");
        MKDIRS("tree-sitter-c/src");
        CMD("cp", "tree-sitter-c-"TS_C_COMMIT"/src/parser.c", "tree-sitter-c/src");
        CMD("rm", "-r", "tree-sitter-c-"TS_C_COMMIT);
}

void ooo_build(bool const clean) {
        if (clean) return;
        INFO("BUILD: ooo code styler");
        #ifndef _WIN32
                CMD("gcc", CFLAGS, "-I", OOO_INC, "-o", "ooo", OOO_SRC);
        #else
                CMD("cl.exe", CFLAGS, "-I", OOO_INC, "-o", "main", OOO_SRC);
        #endif
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
        move_nobbuild_old(flag.clean);
        download_treesitter(flag.clean);
        download_tree_sitter_c(flag.clean);
        ooo_build(flag.clean);
        INFO("Successful done!");
        return 0;
}

/* https://docs.github.com/en/repositories/working-with-files/using-files/downloading-source-code-archives */
