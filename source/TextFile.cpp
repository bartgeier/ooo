#include "TextFile.h"
#include <assert.h>
#include <fstream>
#include <iostream>

static void pr(TextFile::Line const &line) {
        printf("%p", line.at);
        std::string s;
        for (size_t i = 0; i < line.size; i++) {
                s += line.at[i];
        }
        std::cout << s;
}

TextFile::TextFile(size_t capacity) {
        file.reserve(10000);
        assert(file.capacity() == 10000);
        clear();
}

void TextFile::clear() {
        lines.clear();
        file.clear();
        lines.push_back({0, 0, nullptr, 0});
}

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
        lines.clear();
        size_t start = 0;
        for (size_t end = 0; end < this->file.size(); end++) {
                if (this->file[end] == '\n') {
                        lines.push_back({start, end, &(this->file[start]), end - start});
                        start = end + 1;
                }
        }
        return false;
}

void TextFile::append(char const chr) {
        file += chr;
        if (chr == '\n') {
                size_t x = lines.back().end + 1;
                 pr(lines.back());
                 std::cout << "-->" <<'\n';
                //lines.push_back({x, x, &(this->file[x]), 0});
                lines.push_back({x, x, &(this->file.c_str()[x]), 0});
        } else {
                lines.back().end++;
                lines.back().size++;
                lines.back().at = &(this->file.c_str()[lines.back().start]);
        }
}

TextFile::operator char const*() const {
        return file.c_str();
}

std::vector<TextFile::Line>::const_iterator TextFile::begin() const {
        return lines.begin();
}

std::vector<TextFile::Line>::const_iterator TextFile::end() const {
        return lines.end();
}

char const &TextFile::operator[](size_t chr_idx) const {
        return file[chr_idx];
}

size_t TextFile::size() const {
        return this->file.size();
}

size_t TextFile::numOfLines() const {
        return this->lines.size();
}

TextFile::Line const &TextFile::line(size_t line_idx) const {
        return lines[line_idx];
}
