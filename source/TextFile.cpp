#include "TextFile.h"
#include <assert.h>
#include <fstream>
#include <iostream>

bool TextFile::load(char const *path, bool error) {
        if (error) {
                return true;
        }
        std::ifstream file(path);
        if (!file.is_open()) {
                return true;
        }
        this->file = std::string(
                        (std::istreambuf_iterator<char>(file)),
                        (std::istreambuf_iterator<char>())
        );
        file.close();
        size_t start = 0;
        for (size_t end = 0; end < this->file.size(); end++) {
                if (this->file[end] == '\n') {
                        lines.push_back({start, end});
                        start = end + 1;
                }
        }
        return false;
}

/* return true if file[idx] is the first visible character in the line */
bool TextFile::first_character_in_line(size_t idx) const {
        assert(file[idx] != ' ' & file[idx] != '\t');
        for (size_t i = idx; i-- > 0;) {
                if (file[i] == '\n') {
                        return true;
                } 
                if (file[i] != ' ' & file[i] != '\t') {
                        return false;
                }
        }
        return true;
}

/* return true if file[idx] is the last visible character in the line */
bool TextFile::last_character_in_line(size_t idx) const {
        assert(file[idx] != ' ' & file[idx] != '\t');
        for (size_t i = idx; ++i < file.size();) {
                if (file[i] == '\n') {
                        return true;
                }
                if (file[i] != ' ' & file[i] != '\t') {
                        return false;
                }
        }
        return true;
}
