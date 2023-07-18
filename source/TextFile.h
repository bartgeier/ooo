#ifndef TEXT_FILE_H 
#define TEXT_FILE_H

#include <string>
#include <fstream>
#include <vector>

class TextFile {
        std::string file;
        struct Line {
                size_t start;
                size_t end;
        };
        std::vector<Line> lines;
public:
        bool load(char const *path, bool error);
        std::vector<Line>::iterator begin();
        std::vector<Line>::iterator end();
        bool first_character_in_line(size_t chr_idx) const;
        bool last_character_in_line(size_t chr_idx) const;
        operator char const*(); //is used for std::cout << textFile
/* for unittests */
        char& operator[](size_t chr_idx);
        size_t size();
        size_t numOfLines();
        Line &line(size_t line_idx);

};


#endif 
