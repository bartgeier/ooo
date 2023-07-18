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

TextFile::operator char const*() {
        return file.c_str();
}

std::vector<TextFile::Line>::iterator TextFile::begin()  {
        return lines.begin();
}

std::vector<TextFile::Line>::iterator TextFile::end()  {
        return lines.end();
}

char& TextFile::operator[](size_t chr_idx) {
        return file[chr_idx];
}

size_t TextFile::size() {
        return this->file.size();
}

size_t TextFile::numOfLines() {
        return this->lines.size();
}

TextFile::Line &TextFile::line(size_t line_idx) {
        return lines[line_idx];
}

/* return true if file[idx] is the first visible character in the line */
bool TextFile::first_character_in_line(size_t char_idx) const {
        assert(file[char_idx] != ' ' & file[char_idx] != '\t');
        for (size_t i = char_idx; i-- > 0;) {
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
bool TextFile::last_character_in_line(size_t char_idx) const {
        assert(file[char_idx] != ' ' & file[char_idx] != '\t');
        for (size_t i = char_idx; ++i < file.size();) {
                if (file[i] == '\n') {
                        return true;
                }
                if (file[i] != ' ' & file[i] != '\t') {
                        return false;
                }
        }
        return true;
}
