#ifndef TEXT_FILE_H 
#define TEXT_FILE_H

#include <string>
#include <fstream>
#include <vector>

struct TextFile {
        std::string file;
        char& operator[](size_t txt_idx) {
                return file[txt_idx];
        }
        struct Line {
                size_t start;
                size_t end;
        };
        std::vector<Line> lines;
        auto begin()  {
                return lines.begin();
        }
        auto end()  {
                return lines.end();
        }
};

bool TextFile_load(TextFile &self, char const *path, bool error);

#endif 
