#include "gtest/gtest.h"

#include "OJob.h"
#include "OStr.h"
#define REGEX_IMPLEMENTAION
#include "Regex.h"

#define MEM_SIZE 1000*1024

TEST(Regex, no_comment) {
        char *snk = (char*)malloc(MEM_SIZE);
        char *src = (char*)malloc(MEM_SIZE);
        OJob job = {
                .idx = 0,
                .sink = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = snk
                },
                .source = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = src
                },
        };
        char const *s = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        OStr_append_cstring(&job.source, s);
        EXPECT_EQ(job.source.size, strlen(s));

        Regex_CommentBlock_1Line_t blockLine = {.state = RCB1_IDLE, .begin = 0, .num_of_spaces = 0};
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                RegexTupel const tupel = Regex_commentBlock_1line(&blockLine, job.idx, job.source.at[job.idx]);
                if (!tupel.found) {
                        OStr_append_chr(&job.sink, tupel.chr);
                } else {
                        ASSERT_TRUE(false);
                }
        }

        EXPECT_EQ(job.source.size, strlen(s));
        EXPECT_EQ(job.sink.size, job.source.size);
        EXPECT_TRUE(strcmp(job.sink.at,job.source.at) == 0);
}

TEST(Regex, comment_0) {
        char *snk = (char*)malloc(MEM_SIZE);
        char *src = (char*)malloc(MEM_SIZE);
        OJob job = {
                .idx = 0,
                .sink = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = snk
                },
                .source = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = src
                },
        };
        char const *s = "The quick brown fox /* jumps over */ the lazy dog.\n/* Is there * something.  */\nHello Sailor! Where are you coming frome.\nOk!";
        //                                                                    ^                      ^^  ^
        //                                                                    52                     2   79
        OStr_append_cstring(&job.source, s);

        Regex_CommentBlock_1Line_t blockLine = {.state = RCB1_IDLE, .begin = 0, .num_of_spaces = 0};
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                RegexTupel const tupel = Regex_commentBlock_1line(&blockLine, job.idx, job.source.at[job.idx]);
                OStr_append_chr(&job.sink, tupel.chr);
                if (tupel.found) {
                        EXPECT_EQ(blockLine.begin, (size_t)52);
                        EXPECT_EQ(blockLine.num_of_spaces, (size_t)2);
                        EXPECT_EQ(job.idx, (size_t)79);
                }
        }
        EXPECT_EQ(job.source.size, strlen(s));
        EXPECT_EQ(job.sink.size, job.source.size);
        EXPECT_TRUE(strcmp(job.sink.at, job.source.at) == 0);
}

TEST(Regex, comment_1) {
        char *snk = (char*)malloc(MEM_SIZE);
        char *src = (char*)malloc(MEM_SIZE);
        OJob job = {
                .idx = 0,
                .sink = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = snk
                },
                .source = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = src
                },
        };
        char const *s = "The quick bro // fox /* jumps over the lazy dog.*/\n/* Is there * something.  */\nHello Sailor! Where are you coming frome.\nOk!";
        //                                                                    ^                      ^^  ^
        //                                                                    52                     2   79
        OStr_append_cstring(&job.source, s);

        Regex_CommentBlock_1Line_t blockLine = {.state = RCB1_IDLE, .begin = 0, .num_of_spaces = 0};
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                RegexTupel const tupel = Regex_commentBlock_1line(&blockLine, job.idx, job.source.at[job.idx]);
                OStr_append_chr(&job.sink, tupel.chr);
                if (tupel.found) {
                        EXPECT_EQ(blockLine.begin, (size_t)52);
                        EXPECT_EQ(blockLine.num_of_spaces, (size_t)2);
                        EXPECT_EQ(job.idx, (size_t)79);
                }
        }
        EXPECT_EQ(job.source.size, strlen(s));
        EXPECT_EQ(job.sink.size, job.source.size);
        EXPECT_TRUE(strcmp(job.sink.at, job.source.at) == 0);
}

TEST(Regex, comment_2) {
        char *snk = (char*)malloc(MEM_SIZE);
        char *src = (char*)malloc(MEM_SIZE);
        OJob job = {
                .idx = 0,
                .sink = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = snk
                },
                .source = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = src
                },
        };
        char const *s = "The quick brown fox /* jumps over\nthe lazy dog.*/\n/* Is there * something.  */\nHello Sailor! Where are you coming frome.\nOk!";
        //                                                                    ^                      ^^  ^
        //                                                                    51                     2   78
        OStr_append_cstring(&job.source, s);

        Regex_CommentBlock_1Line_t blockLine = {.state = RCB1_IDLE, .begin = 0, .num_of_spaces = 0};
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                RegexTupel const tupel = Regex_commentBlock_1line(&blockLine, job.idx, job.source.at[job.idx]);
                OStr_append_chr(&job.sink, tupel.chr);
                if (tupel.found) {
                        EXPECT_EQ(blockLine.begin, (size_t)51);
                        EXPECT_EQ(blockLine.num_of_spaces, (size_t)2);
                        EXPECT_EQ(job.idx, (size_t)78);
                }
        }
        EXPECT_EQ(job.source.size, strlen(s));
        EXPECT_EQ(job.sink.size, job.source.size);
        EXPECT_TRUE(strcmp(job.sink.at, job.source.at) == 0);
}

TEST(Regex, comment_3) {
        char *snk = (char*)malloc(MEM_SIZE);
        char *src = (char*)malloc(MEM_SIZE);
        OJob job = {
                .idx = 0,
                .sink = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = snk
                },
                .source = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = src
                },
        };
        char const *r = "The quick brown fox /* jumps over */ the lazy dog.\n// Is there * something.\nHello Sailor! Where are you coming frome.\nOk!";
        char const *s = "The quick brown fox /* jumps over */ the lazy dog.\n/* Is there * something.  */\nHello Sailor! Where are you coming frome.\nOk!";
        //                                                                    ^                      ^^  ^
        //                                                                    52                     2   79
        OStr_append_cstring(&job.source, s);

        Regex_CommentBlock_1Line_t blockLine = {.state = RCB1_IDLE, .begin = 0, .num_of_spaces = 0};
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                RegexTupel const tupel = Regex_commentBlock_1line(&blockLine, job.idx, job.source.at[job.idx]);
                if (tupel.found) {
                        job.sink.at[blockLine.begin] = '/';
                        job.sink.size = job.sink.size - 2 - blockLine.num_of_spaces;
                }
                OStr_append_chr(&job.sink, tupel.chr);
        }
        EXPECT_EQ(job.sink.size, strlen(r));
        EXPECT_TRUE(strcmp(job.sink.at, r) == 0);
        printf("%s\n", job.sink.at);
}
