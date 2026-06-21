#define GEN_COMPILE_COMMANDS_IMPLEMENTATION
#include "gen_compile_commands.h"

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

#if 0
bool download_build_treesitter(bool const clean) {
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
        #ifdef _WIN32
                nob_log(NOB_INFO,"xxxxx unzip with tar xxxxx");
                nob_cmd_append(&cmd, "tar", "-xvzf", "tree-sitter.zip");
        #else
                nob_cmd_append(&cmd, "unzip", "tree-sitter.zip");                
        #endif                
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
        // ok &= nob_rename("tree-sitter-"TS_COMMIT"/Cargo.toml", "tree-sitter/Cargo.toml");
        ok &= nob_rename("tree-sitter-"TS_COMMIT"/LICENSE", "tree-sitter/LICENSE");
        ok &= nob_rename("tree-sitter-"TS_COMMIT"/libtree-sitter.a", "tree-sitter/libtree-sitter.a");
        ok &= nob_rename("tree-sitter-"TS_COMMIT"/lib/include/tree_sitter/api.h", "tree-sitter/lib/include/tree_sitter/api.h");
        ok &= nob_remove("tree-sitter-"TS_COMMIT);
        nob_cmd_free(cmd);
        return ok;
}
#else
bool download_build_treesitter(bool const clean) {
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
        #ifdef _WIN32
                nob_log(NOB_INFO,"xxxxx unzip with tar xxxxx");
                nob_cmd_append(&cmd, "tar", "-xvzf", "tree-sitter.zip");
        #else
                nob_cmd_append(&cmd, "unzip", "tree-sitter.zip");                
        #endif                
        ok &= nob_cmd_run_sync(cmd);
        cmd.count = 0;
        ok &= nob_remove("tree-sitter.zip");
        ok &= nob_mkdir_if_not_exists("tree-sitter");
        ok &= nob_mkdir_if_not_exists("tree-sitter/lib");
        ok &= nob_mkdir_if_not_exists("tree-sitter/lib/include");
        ok &= nob_mkdir_if_not_exists("tree-sitter/lib/include/tree_sitter");
        ok &= nob_rename("tree-sitter-"TS_COMMIT"/LICENSE", "tree-sitter/LICENSE");
        ok &= nob_rename("tree-sitter-"TS_COMMIT"/lib/include/tree_sitter/api.h", "tree-sitter/lib/include/tree_sitter/api.h");
        ok &= nob_rename("tree-sitter-"TS_COMMIT"/lib/src", "tree-sitter/lib/src");
        // ok &= nob_remove("tree-sitter/lib/src/portable");
        ok &= nob_remove("tree-sitter/lib/src/unicode");
        ok &= nob_remove("tree-sitter/lib/src/wasm");
        ok &= nob_remove("tree-sitter-"TS_COMMIT);

        nob_cmd_append(&cmd, "gcc", "-O3");
        nob_cmd_append(&cmd, "-I", "tree-sitter/lib/src", "-I", "tree-sitter/lib/include");
        nob_cmd_append(&cmd, "-c","-o","tree-sitter/lib.o", "tree-sitter/lib/src/lib.c");
        ok &= nob_cmd_run_sync(cmd);
        cmd.count = 0;

        nob_cmd_append(&cmd,"ar", "rcs", "tree-sitter/libtree-sitter.a", "tree-sitter/lib.o");
        ok &= nob_cmd_run_sync(cmd);
        cmd.count = 0;

        nob_cmd_free(cmd);
        return ok;
}

#endif

bool download_build_tree_sitter_c(bool const clean) {
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
        #ifdef _WIN32
                nob_log(NOB_INFO,"xxxxx unzip with tar xxxxx");
                nob_cmd_append(&cmd, "tar", "-xvzf", "tree-sitter-c.zip");
        #else
                nob_cmd_append(&cmd, "unzip", "tree-sitter-c.zip");                
        #endif
        ok &= nob_cmd_run_sync(cmd);
        nob_cmd_free(cmd);
        ok &= nob_remove("tree-sitter-c.zip");
        ok &= nob_mkdir_if_not_exists("tree-sitter-c");
        ok &= nob_mkdir_if_not_exists("tree-sitter-c/src");
        ok &= nob_mkdir_if_not_exists("tree-sitter-c/src/tree_sitter");
        // ok &= nob_rename("tree-sitter-c-"TS_COMMIT"/Cargo.toml", "tree-sitter-c/Cargo.toml");
        ok &= nob_rename("tree-sitter-c-"TS_COMMIT"/LICENSE", "tree-sitter-c/LICENSE");
        ok &= nob_rename("tree-sitter-c-"TS_C_COMMIT"/src/tree_sitter/parser.h", "tree-sitter-c/src/tree_sitter/parser.h");
        ok &= nob_rename("tree-sitter-c-"TS_C_COMMIT"/src/parser.c", "tree-sitter-c/src/parser.c");
        ok &= nob_remove("tree-sitter-c-"TS_C_COMMIT);
        return ok;
}

#if 1
bool tree_sitter_amalgamate(void) {
        bool ok = true;
        // https://github.com/rindeal/Amalgamate/releases/tag/v0.99.0
        // whereis amalgamate /usr/local/bin
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "amalgamate", "amalgamate.c", 
                "tree-sitter/tree_sitter_api.h",
                "-i", "tree-sitter/lib/include",
                "-i", "tree-sitter/lib/include/tree_sitter",
                "-i", "tree-sitter/lib/src",
                "-i", "tree-sitter/lib/src/portable",
                "-i", "tree-sitter-c/src/parser.c",
        );
        ok &= nob_cmd_run_sync(cmd);
        return ok;
}
#endif

bool copy_treesitter_symbols(bool const clean) {
        if (clean) {
                return true;
        }
        if (nob_file_exists("tree-sitter-c/ooo_treesitter_symbol_ids.h")) return true;
        nob_log(NOB_INFO, "BUILD: copy_treesitter_symbols ----> copy enumerator");
        Nob_Cmd cmd = {0};
#if 0
        // uses precompiled archive libtree-sitter.a
        nob_cmd_append(&cmd, "gcc");
        nob_cmd_append(&cmd, "-O3", "-ggdb", "-pedantic");
        nob_cmd_append(&cmd, "-I", "tree-sitter/lib/include/");
        nob_cmd_append(&cmd, "-I", "source/");
        nob_cmd_append(&cmd, "source/copy_treesitter_symbols.c"); 
        nob_cmd_append(&cmd, "-o", "copy_treesitter_symbols");
        nob_cmd_append(&cmd, "tree-sitter/libtree-sitter.a",  "tree-sitter-c/src/parser.c");;
#else
        // uses amalgamate single header tree_sitter_api.h
        nob_cmd_append(&cmd, "gcc");
        nob_cmd_append(&cmd, "-O3", "-ggdb", "-pedantic");
        nob_cmd_append(&cmd, "-DAMALGAMATE_TREE_SITTER_API_H");
        nob_cmd_append(&cmd, "-I", "tree-sitter/");
        nob_cmd_append(&cmd, "-I", "source/");
        nob_cmd_append(&cmd, "source/copy_treesitter_symbols.c"); 
        nob_cmd_append(&cmd, "-o", "copy_treesitter_symbols");
#endif
        bool ok = nob_cmd_run_sync(cmd);
        cmd.count = 0;
        #ifdef _WIN32
                ok &= nob_rename("copy_treesitter_symbols.exe", "build/copy_treesitter_symbols.exe");
                nob_cmd_append(&cmd,"build/copy_treesitter_symbols.exe" );
                ok &= nob_cmd_run_sync(cmd);
        #else
                ok &= nob_rename("copy_treesitter_symbols", "build/copy_treesitter_symbols");
                nob_cmd_append(&cmd,"build/copy_treesitter_symbols" );
                ok &= nob_cmd_run_sync(cmd);
        #endif
        nob_cmd_free(cmd);
        return ok;
}

bool download_build_googleTest(bool const clean) {
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
        #ifdef _WIN32
                nob_log(NOB_INFO,"xxxxx unzip with tar xxxxx");
                nob_cmd_append(&cmd, "tar", "-xvzf", "googletest.zip");
        #else
                nob_cmd_append(&cmd, "unzip", "googletest.zip");                
        #endif
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

bool download_arq(bool const clean) {
        #define ARQ_URL "https://raw.githubusercontent.com/bartgeier/arq/"
        #if 0
                #define ARQ_COMMIT "master"
        #else
                #define ARQ_COMMIT "5ee50cdae38d5b5e673496a0827ed186a8346209"
        #endif
        bool ok = true;
        if (clean) {
                ok &= nob_remove("arq");
                return ok;
        }
        if (nob_file_exists("arq")) return true;
        nob_log(NOB_INFO, "DOWNLOAD: arq");
        ok &= nob_mkdir_if_not_exists("arq");
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "curl");
        nob_cmd_append(&cmd, "-L");
        nob_cmd_append(&cmd,ARQ_URL ARQ_COMMIT "/amalgamate/arq.h");
        nob_cmd_append(&cmd, "--output", "arq/arq.h");
        ok &= nob_cmd_run_sync(cmd);
        cmd.count = 0;
        return ok;
}

Nob_Cmd include_paths = {0};
void create_include_paths(void) {
#if 0
        nob_cmd_append(&include_paths, "-I", "source");
        nob_cmd_append(&include_paths, "-I", "tree-sitter/lib/include");
        nob_cmd_append(&include_paths, "-I", "tree-sitter-c");
        nob_cmd_append(&include_paths, "-I", "arq");
#else
        nob_cmd_append(&include_paths, "-I", "source");
        nob_cmd_append(&include_paths, "-DAMALGAMATE_TREE_SITTER_API_H");
        //nob_cmd_append(&include_paths, "-DOOO_AMALGAMATE");
        nob_cmd_append(&include_paths, "-I", "tree-sitter/");
        nob_cmd_append(&include_paths, "-I", "tree-sitter-c");
        nob_cmd_append(&include_paths, "-I", "arq");
#endif
}

Nob_Cmd source_paths = {0};
void create_source_paths(void) {
#if 0
        nob_cmd_append(&source_paths, "./source/main.c");
        nob_cmd_append(&source_paths, "./source/single_header_implementation.c");
        nob_cmd_append(&source_paths, "./tree-sitter-c/src/parser.c");
        nob_cmd_append(&source_paths, "./source/node_printer.c");
        nob_cmd_append(&source_paths, "./source/truncate.c");
        nob_cmd_append(&source_paths, "./source/OArg.c");
        nob_cmd_append(&source_paths, "./source/iteration.c");
        nob_cmd_append(&source_paths, "./source/ruler.c");
        nob_cmd_append(&source_paths, "./source/indentation.c");
        nob_cmd_append(&source_paths, "./source/tree_navigator.c");
        nob_cmd_append(&source_paths, "./source/Pars.c");
        nob_cmd_append(&source_paths, "./tree-sitter/libtree-sitter.a");
#else
        nob_cmd_append(&source_paths, "./source/main.c");
        nob_cmd_append(&source_paths, "./source/single_header_implementation.c");
        //nob_cmd_append(&source_paths, "./tree-sitter-c/src/parser.c");
        nob_cmd_append(&source_paths, "./source/node_printer.c");
        nob_cmd_append(&source_paths, "./source/truncate.c");
        nob_cmd_append(&source_paths, "./source/OArg.c");
        nob_cmd_append(&source_paths, "./source/iteration.c");
        nob_cmd_append(&source_paths, "./source/ruler.c");
        nob_cmd_append(&source_paths, "./source/indentation.c");
        nob_cmd_append(&source_paths, "./source/tree_navigator.c");
        nob_cmd_append(&source_paths, "./source/Pars.c");
#endif
}


#if 0
#if 0
        // uses precompiled archive libtree-sitter.a
        nob_cmd_append(&cmd, "gcc");
        nob_cmd_append(&cmd, "-O3", "-ggdb", "-pedantic");
        nob_cmd_append(&cmd, "-I", "tree-sitter/lib/include/");
        nob_cmd_append(&cmd, "-I", "source/");
        nob_cmd_append(&cmd, "source/copy_treesitter_symbols.c"); 
        nob_cmd_append(&cmd, "-o", "copy_treesitter_symbols");
        nob_cmd_append(&cmd, "tree-sitter/libtree-sitter.a",  "tree-sitter-c/src/parser.c");;
#else
        // uses amalgamate single header tree_sitter_api.h
        nob_cmd_append(&cmd, "gcc");
        nob_cmd_append(&cmd, "-O3", "-ggdb", "-pedantic");
        nob_cmd_append(&cmd, "-DAMALGAMATE_TREE_SITTER_API_H");
        nob_cmd_append(&cmd, "-I", "tree-sitter/");
        nob_cmd_append(&cmd, "-I", "source/");
        nob_cmd_append(&cmd, "source/copy_treesitter_symbols.c"); 
        nob_cmd_append(&cmd, "-o", "copy_treesitter_symbols");
#endif
#endif

bool ooo_build(bool const clean) {
        if (clean) {
                return true;
        }
        nob_log(NOB_INFO, "BUILD: ooo ----> code styler");
        Nob_Cmd c_ompiler = {0};
        //nob_cmd_append(&cmd, "gcc", "-O0", "-ggdb", "-pedantic");
        nob_cmd_append(&c_ompiler, "gcc", "-O3", "-Wall", "-Wextra", "-Wno-Wbitwise-instead-of-logical", "-pedantic", "-Wno-parentheses"); 

        Nob_Cmd cmd = {0};
        nob_cmd_append_cmd(&cmd, &c_ompiler);
        nob_cmd_append_cmd(&cmd, &include_paths);
        nob_cmd_append(&cmd, "-o", "build/ooo");
        nob_cmd_append_cmd(&cmd, &source_paths);
        bool ok = nob_cmd_run_sync(cmd);

        cmd.count = 0;
        nob_cmd_append_cmd(&cmd, &c_ompiler);
        nob_cmd_append(&cmd, "-o", "build/ooo");
        nob_cmd_append_cmd(&cmd, &include_paths);
        // Language server
        generate_compile_commands("/home/berni/projects/ooo", &cmd, &source_paths);

        nob_cmd_free(c_ompiler);
        nob_cmd_free(cmd);
        return ok;
}

Nob_Cmd source_paths2 = {0};
void create_source_paths_2(void) {
        nob_cmd_append(&source_paths2, "./source/unitbuild.c");
        //nob_cmd_append(&source_paths2, "./tree-sitter-c/src/parser.c");
        nob_cmd_append(&source_paths2, "./source/node_printer.c");
        nob_cmd_append(&source_paths2, "./source/truncate.c");
        nob_cmd_append(&source_paths2, "./source/OArg.c");
        nob_cmd_append(&source_paths2, "./source/iteration.c");
        nob_cmd_append(&source_paths2, "./source/ruler.c");
        nob_cmd_append(&source_paths2, "./source/indentation.c");
        nob_cmd_append(&source_paths2, "./source/tree_navigator.c");
        nob_cmd_append(&source_paths2, "./source/Pars.c");
        nob_cmd_append(&source_paths2, "./tree-sitter/lib/src/lib.c");
}

bool ooo_unit_build(bool const clean) {
        if (clean) {
                return true;
        }
        nob_log(NOB_INFO, "BUILD: ooo ----> code styler");
        Nob_Cmd c_ompiler = {0};
        //nob_cmd_append(&cmd, "gcc", "-O0", "-ggdb", "-pedantic");
        nob_cmd_append(&c_ompiler, "gcc", "-O3", "-Wall", "-Wextra", "-Wno-Wbitwise-instead-of-logical", "-pedantic", "-Wno-parentheses"); 

        Nob_Cmd cmd = {0};
        nob_cmd_append_cmd(&cmd, &c_ompiler);
        nob_cmd_append_cmd(&cmd, &include_paths);
        nob_cmd_append(&cmd, "-o", "build/ooo2");
        nob_cmd_append_cmd(&cmd, &source_paths2);
        bool ok = nob_cmd_run_sync(cmd);

#if 0
        cmd.count = 0;
        nob_cmd_append_cmd(&cmd, &c_ompiler);
        nob_cmd_append(&cmd, "-o", "build/ooo");
        nob_cmd_append_cmd(&cmd, &include_paths);
        // Language server
        generate_compile_commands("/home/berni/projects/ooo", &cmd, &source_paths);
#endif

        nob_cmd_free(c_ompiler);
        nob_cmd_free(cmd);
        return ok;
}

bool unittests_build(bool const clean) {
        if (clean) {
                return true;
        }
        bool ok = true;
        nob_log(NOB_INFO, "BUILD: otest ----> unit tests");
        Nob_Cmd cmd = {0};
        nob_cmd_append(
                &cmd, 
                "g++", "-ggdb", "-O0", "-std=c++20",
                "-Wall", "-Wextra", "-pedantic",
                "-Wno-parentheses", "-Wno-missing-field-initializers"
        );
        nob_cmd_append(&cmd, "-I", "googletest/include/"); 
        nob_cmd_append(&cmd, "-I", "source/"); 
        nob_cmd_append(&cmd, "-I", "tree-sitter/lib/include/"); 
        nob_cmd_append(&cmd, "-I", "tree-sitter-c/");
        nob_cmd_append(&cmd, "-L", "googletest/build/lib/");
        nob_cmd_append(&cmd, "-o", "build/otest");
        nob_cmd_append(&cmd, "source/tree_navigator.c");
        nob_cmd_append(&cmd, "unittests/tst_OStr.c");
        nob_cmd_append(&cmd, "unittests/tst_tree_navigator.c");
        nob_cmd_append(&cmd, "unittests/tst_OArena.c");
        nob_cmd_append(&cmd, "unittests/tst_regex_lineFeed.c");
        nob_cmd_append(&cmd, "unittests/tst_regex_tabFilter.c");
        nob_cmd_append(&cmd, "unittests/tst_regex_signedComment.c");
        nob_cmd_append(&cmd, "unittests/tst_regex_truncNodeSpace.c");
        nob_cmd_append(&cmd, "unittests/tst_regex_truncCommentSpace.c");
        nob_cmd_append(&cmd, "unittests/tst_regex_lineCont.c");
        nob_cmd_append(&cmd, "unittests/tst_regex_lineUp.c");
        nob_cmd_append(&cmd, "tree-sitter/libtree-sitter.a");
        nob_cmd_append(&cmd, "-lgtest", "-lgtest_main");
        ok &= nob_cmd_run_sync(cmd);
        cmd.count = 0;
        nob_cmd_free(cmd);
        return ok;
}


int main(int argc, char **argv) {
        uint64_t t_start = nob_millis();
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

        ok &= download_build_treesitter(flag.clean);
        ok &= download_build_tree_sitter_c(flag.clean);
        ok &= tree_sitter_amalgamate();
        ok &= copy_treesitter_symbols(flag.clean);

        ok &= download_build_googleTest(flag.clean);
        ok &= download_arq(flag.clean);

        create_source_paths();
        //create_source_paths_2();
        create_include_paths();

        ok &= ooo_build(flag.clean);
        //ok &= ooo_unit_build(flag.clean);
        // ok &= unittests_build(flag.clean);
        if (!ok) {
                nob_log(NOB_ERROR, "Done  => One or more errors occurred! %llu ms", nob_millis() - t_start);
                return false;
        }
        nob_log(NOB_INFO ,"Successful done! %llu ms", nob_millis() - t_start);
        return  0;
}

/* https://docs.github.com/en/repositories/working-with-files/using-files/downloading-source-code-archives */

// ## Windows
// #### Compiler
// WinLibs standalone build of GCC and MinGW-w64 for Windows\
// https://winlibs.com/
//
// #### unzip
// https://gnuwin32.sourceforge.net/packages/unzip.htm \
// Binaries....Zip....457440....14 February 2005....925a5d4d9b4a63ff1a473cc4d47f2f05 \
//
// First I used unzip because I had issues on windows with unzipping tree-sitter using `tar -xvzf tree-sitter.zip'.
// These issues are gone. So I use tar again, which is already installed on Windows.
