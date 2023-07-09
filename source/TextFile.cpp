#include "TextFile.h"
#include <assert.h>
#include <fstream>


bool TextFile_load(TextFile &self, char const *path, bool error) {
        if (error) {
                return true;
        }
        std::ifstream file(path);
        if (!file.is_open()) {
                return true;
        }
        self.file = std::string((std::istreambuf_iterator<char>(file)),
                                (std::istreambuf_iterator<char>()));
        file.close();
        size_t start = 0;
        for (size_t end = 0; end < self.file.size(); end++) {
                if (self.file[end] == '\n') {
                        self.lines.push_back({start, end});
                        start = end + 1;
                }
        }
        return false;
}
