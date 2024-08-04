#include "gtest/gtest.h"

#include "OJob.h"
#include "OStr.h"
#define REGEX_IMPLEMENTATION
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

        Regex_CommentBlock_1Line_t regex = {
                .state = RCB1_IDLE, 
                .begin = 0, 
                .num_of_truncate = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_commentBlock_1line(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                if (!found) {
                        OStr_append_chr(&job.sink, job.source.at[job.idx]);
                } else {
                        ASSERT_TRUE(false);
                }
        }

        EXPECT_EQ(job.source.size, strlen(s));
        EXPECT_EQ(job.sink.size, job.source.size);
        OStr_append_chr(&job.source, 0);
        OStr_append_chr(&job.sink, 0);
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

        Regex_CommentBlock_1Line_t regex = {
                .state = RCB1_IDLE, 
                .begin = 0, 
                .num_of_truncate = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_commentBlock_1line(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                OStr_append_chr(&job.sink, job.source.at[job.idx]);
                if (found) {
                        EXPECT_EQ(regex.begin, (size_t)52);
                        EXPECT_EQ(regex.num_of_truncate, (size_t)4);
                        EXPECT_EQ(job.idx, (size_t)79);
                }
        }
        EXPECT_EQ(job.source.size, strlen(s));
        EXPECT_EQ(job.sink.size, job.source.size);
        OStr_append_chr(&job.source, 0);
        OStr_append_chr(&job.sink, 0);
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

        Regex_CommentBlock_1Line_t regex = {
                .state = RCB1_IDLE, 
                .begin = 0, 
                .num_of_truncate = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_commentBlock_1line(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                OStr_append_chr(&job.sink, job.source.at[job.idx]);
                if (found) {
                        EXPECT_EQ(regex.begin, (size_t)52);
                        EXPECT_EQ(regex.num_of_truncate, (size_t)4);
                        EXPECT_EQ(job.idx, (size_t)79);
                }
        }
        EXPECT_EQ(job.source.size, strlen(s));
        EXPECT_EQ(job.sink.size, job.source.size);
        OStr_append_chr(&job.source, 0);
        OStr_append_chr(&job.sink, 0);
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

        Regex_CommentBlock_1Line_t regex = {
                .state = RCB1_IDLE, 
                .begin = 0, 
                .num_of_truncate = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_commentBlock_1line(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                OStr_append_chr(&job.sink, job.source.at[job.idx]);
                if (found) {
                        EXPECT_EQ(regex.begin, (size_t)51);
                        EXPECT_EQ(regex.num_of_truncate, (size_t)4);
                        EXPECT_EQ(job.idx, (size_t)78);
                }
        }
        EXPECT_EQ(job.source.size, strlen(s));
        EXPECT_EQ(job.sink.size, job.source.size);
        OStr_append_chr(&job.source, 0);
        OStr_append_chr(&job.sink, 0);
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

        Regex_CommentBlock_1Line_t regex = {
                .state = RCB1_IDLE, 
                .begin = 0, 
                .num_of_truncate = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_commentBlock_1line(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                if (found) {
                        job.sink.at[job.sink.size - (i - regex.begin)] = '/';
                        job.sink.size = job.sink.size - regex.num_of_truncate;
                }
                OStr_append_chr(&job.sink, job.source.at[job.idx]);
        }
        EXPECT_EQ(job.sink.size, strlen(r));
        OStr_append_chr(&job.sink, 0);
        EXPECT_TRUE(strcmp(job.sink.at, r) == 0);
        // printf("%s\n", job.sink.at);
}

TEST(Regex, comment_4) {
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
        char const *r = "The quick brown fox // jumps over";
        char const *s = "The quick brown fox /* jumps over */";
        OStr_append_cstring(&job.source, s);

        Regex_CommentBlock_1Line_t regex = {
                .state = RCB1_IDLE, 
                .begin = 0, 
                .num_of_truncate = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_commentBlock_1line(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                if (found) {
                        job.sink.at[job.sink.size - (i - regex.begin)] = '/';
                        job.sink.size = job.sink.size - regex.num_of_truncate;
                }
                OStr_append_chr(&job.sink, job.source.at[job.idx]);
        }
        bool const found = Regex_commentBlock_1line(
                &regex, 
                job.source.size, 
                0 // end of OStr
        );
        if (found) {
                job.sink.at[job.sink.size - (job.source.size - regex.begin)] = '/';
                job.sink.size = job.sink.size - regex.num_of_truncate;
        }

        EXPECT_EQ(job.sink.size, strlen(r));
        OStr_append_chr(&job.sink, 0);
        EXPECT_TRUE(strcmp(job.sink.at, r) == 0);
}
