#ifndef TEXT_FILE_H 
#define TEXT_FILE_H

#include <string>
#include <fstream>
#include <vector>


class TextVector {
        size_t CAPACITY;
        size_t _size;
        char *at;
public:
        //TextVector(char *buffer, size_t size);
        TextVector &setMemory(char *buffer, size_t capacity);
        void clear(); 
        bool load(char const *path, bool error);
        size_t size() const;
        char *begin();
        char *end();
        char const *cbegin() const;
        char const *cend() const;
        char const &operator[](size_t idx) const;
        operator char const*() const;
        bool push_back(char const chr);
};


struct Slice {
        char const *at;
        size_t front;
        size_t size;
        Slice(char const *at, size_t start, size_t size);
        char const *cbegin() const;
        char const *cend() const;
        char const &operator[](size_t idx) const;
};


#if 1
class TextFile {
        // std::string file;
        TextVector file;
#if 0
public: struct Slice {
                size_t start;
                size_t end;
                char const *at;
                size_t size;
        };
#endif
private: std::vector<Slice> lines;
public:
        //TextFile(size_t capacity);
        TextFile &setMemory(char *buffer, size_t capacity);
        void clear();
        bool load(char const *path, bool error);
        void append(char const chr);
        void setLineSlice();

        char const &operator[](size_t chr_idx) const;
        std::vector<Slice>::const_iterator begin() const;
        std::vector<Slice>::const_iterator end() const;
        operator char const*() const; // used for std::cout << textFile

/* used for unittests */
        size_t size() const;
        size_t numOfLines() const;
        Slice const &line(size_t line_idx) const;

};
#else
class TextFile {
        // std::string file;
        std::vector<char> file0;
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
        bool load0(char const *path, bool error);
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

#endif 
