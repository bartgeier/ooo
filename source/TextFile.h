#ifndef TEXT_FILE_H 
#define TEXT_FILE_H

#include <string>
#include <fstream>
#include <vector>

class TextFile {
        std::string file;
public: struct Line {
                size_t start;
                size_t end;
                char const *at;
                size_t size;
        };
private: std::vector<Line> lines;
public:
        TextFile(size_t capacity);
        void clear();
        bool load(char const *path, bool error);
        void append(char const chr);

        char const &operator[](size_t chr_idx) const;
        std::vector<Line>::const_iterator begin() const;
        std::vector<Line>::const_iterator end() const;
        operator char const*() const; // used for std::cout << textFile

/* used for unittests */
        size_t size() const;
        size_t numOfLines() const;
        Line const &line(size_t line_idx) const;

};


#endif 
