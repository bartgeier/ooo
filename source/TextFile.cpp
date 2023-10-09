#include "TextFile.h"
#include <assert.h>
#include <fstream>
#include <iostream>

TextVector &TextVector::setMemory(char *buffer, size_t capacity) {
        assert(capacity > 0);
        _size = 0;
        CAPACITY = capacity;
        at = buffer;
        for (size_t i = 0; i < CAPACITY; i++) {
                at[i] = 0;
        }
        return *this;
}

void TextVector::clear() {
        for (size_t i = 0; i < _size; i++) {
                at[i] = 0;
        }
        _size = 0;
}

bool TextVector::load(char const *path, bool error) {
        if (error) {
                return true;
        }
        std::ifstream is(path, std::ifstream::binary);
        if (!is.is_open()) {
                return true;
        }
        is.seekg(0, is.end);
        size_t const isLen = is.tellg();
        is.seekg(0, is.beg);
        if (isLen >= (CAPACITY - 1)) {
                is.close();
                return true;
        }
        is.read(at, isLen);
        is.close();
        if (!is) {
                return true;
        }
        _size = isLen;
        return error;
}

size_t TextVector::size() const {
        return _size;
}

char *TextVector::begin() {
        return &at[0];
}

char *TextVector::end() {
        return  &at[_size];
}
char const *TextVector::cbegin() const {
        return &at[0];
}

char const *TextVector::cend() const {
        return  &at[_size];
}

char const &TextVector::operator[](size_t idx) const {
        return at[idx];
}

TextVector::operator char const*() const {
        return at;
}

bool TextVector::push_back(char const chr) {
        if (_size < (CAPACITY - 1)) {
                at[_size] = chr;
                _size++;
                return false;
        }
        /* error */
        return true;
}

///////////////////////////////////////////////////////////////////////////////

Slice::Slice(char const *at, size_t front, size_t size) {
        this->at = at;
        this->front = front;
        this->size = size;
}

char const *Slice::cbegin() const {
        return &at[0];
}

char const *Slice::cend() const {
        return  &at[size];
}

char const &Slice::operator[](size_t idx) const {
        return at[idx];
}

///////////////////////////////////////////////////////////////////////////////

TextFile &TextFile::setMemory(char *buffer, size_t capacity) {
        file.setMemory(buffer, capacity);
        clear();
        return *this;
}

void TextFile::clear() {
        lines.clear();
        file.clear();
}

bool TextFile::load(char const *path, bool error) {
        if (error) {
                return true;
        }
        error = file.load(path, error);
        if (error) {
                return true;
        }
        lines.clear();
        return false;
}

void TextFile::setLineSlice() {
        size_t front = 0;
        for (size_t idx = 0; idx < file.size(); idx++) {
                if ((file[idx] == '\n')
                ||  (idx == file.size() - 1)) {
                        lines.push_back({&file[front], front, (idx - front)});
                        front = idx + 1;
                }
        }
}

void TextFile::append(char const chr) {
         file.push_back(chr);
}

TextFile::operator char const*() const {
        return &file[0];
}

std::vector<Slice>::const_iterator TextFile::begin() const {
        return lines.begin();
}

std::vector<Slice>::const_iterator TextFile::end() const {
        return lines.end();
}

char const &TextFile::operator[](size_t idx) const {
        return file[idx];
}

size_t TextFile::size() const {
        return this->file.size();
}

size_t TextFile::numOfLines() const {
        return this->lines.size();
}

Slice const &TextFile::line(size_t line_idx) const {
        return lines[line_idx];
}
