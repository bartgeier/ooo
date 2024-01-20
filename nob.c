#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdio.h>

bool nob_rm(const char *path)
{
        bool result = true;
        Nob_File_Paths children = {0};
        Nob_String_Builder sb = {0};
        size_t temp_checkpoint = nob_temp_save();

        Nob_File_Type type = nob_get_file_type(path);
        if (type < 0) return false;

        switch (type) {
        
            case NOB_FILE_DIRECTORY: {
                if (!nob_read_entire_dir(path, &children)) nob_return_defer(false);

                for (size_t i = 0; i < children.count; ++i) {
                        if (strcmp(children.items[i], ".") == 0) continue;
                        if (strcmp(children.items[i], "..") == 0) continue;
                        sb.count = 0;
                        nob_sb_append_cstr(&sb, path);
                        nob_sb_append_cstr(&sb, "/");
                        nob_sb_append_cstr(&sb, children.items[i]);
                        nob_sb_append_null(&sb);
                        if (!nob_rm(sb.items)) {
                                nob_return_defer(false);
                        }
                }
                if (rmdir(path) < 0) {
                    if (errno == EEXIST) {
                        errno = 0;
                        nob_log(NOB_WARNING, "directory %s does not exist", path);
                        nob_return_defer(false);
                    } else {
                        nob_log(NOB_ERROR, "could not remove directory %s: %s", path, strerror(errno));
                    }
                }
            } break;

            case NOB_FILE_REGULAR: {
                if (unlink(path) < 0) {
                    if (errno == ENOENT) {
                        errno = 0;
                        nob_log(NOB_WARNING, "file %s does not exist",path);
                    } else {
                        nob_log(NOB_ERROR, "could not remove file %s: %s", path, strerror(errno));
                        nob_return_defer(false);
                    }
                }
            } break;

            case NOB_FILE_SYMLINK: {
                nob_log(NOB_WARNING, "TODO: Copying symlinks is not supported yet");
            } break;

            case NOB_FILE_OTHER: {
                nob_log(NOB_ERROR, "Unsupported type of file %s", path);
                nob_return_defer(false);
            } break;

            default: NOB_ASSERT(0 && "unreachable");
        }
defer:
    nob_temp_rewind(temp_checkpoint);
    nob_da_free(sb);
    nob_da_free(children);
    return result;
}

int ooo_style(const char *input_path, const char *output_path) {
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "./ooo");
        nob_cmd_append(&cmd, "-i", input_path, "-o", output_path);
        if (!nob_cmd_run_sync(cmd)) return 1;
        return 0;
}

bool create_test_dir(bool clean) {
        if (clean) {
                return  nob_rm("./ooo_generate");
        }
        return nob_mkdir_if_not_exists("./ooo_generate");
}


int main(int argc, char **argv) {
        NOB_GO_REBUILD_URSELF(argc, argv);

        if (argc > 1) nob_shift_args(&argc, &argv);
        bool clean = strcmp(nob_shift_args(&argc, &argv), "clean") == 0;

        if (!create_test_dir(clean)) return 1;
        int error = 0;
        if (!clean) {
            error |= ooo_style("examples/CipActionDispatcher.h",    "ooo_generate/CipActionDispatcher.out.h");
            error |= ooo_style("examples/CipActionDispatcher.c",    "ooo_generate/CipActionDispatcher.out.c");
            error |= ooo_style("examples/Eventdetection_Heating.h", "ooo_generate/Eventdetection_Heating.out.h");
            error |= ooo_style("examples/Eventdetection_Heating.c", "ooo_generate/Eventdetection_Heating.out.c");
        }
        return error;
}
