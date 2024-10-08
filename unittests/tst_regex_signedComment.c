#include "gtest/gtest.h"

#include "OJob.h"
#include "OStr.h"

#define REGEX_SIGNED_COMMENT_IMPLEMENTATION
#include "Regex_signedComment.h"


#define MEM_SIZE 1000*1024

TEST(Regex_signedComment, no_comment) {
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
        char const *s = "The quick brown fox jumps over the lazy dog.\nOk!";
        OStr_append_cstring(&job.source, s);
        EXPECT_EQ(job.source.size, strlen(s));

        Regex_signedComment_t regex = {
                .state = RSC_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_signedComment(
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

TEST(Regex_signedComment, collect_information_1) {
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
        
        char const *s = "The lazy dog. /* Is there * something.|VB5FNX7iQCFJBz2Ka0mUzYGYgCvtEQ1SNmXPZ54e|*/\nOk";
        //                              ^                      ^                                           ^
        //                              15                     38                                          82
        OStr_append_cstring(&job.source, s);

        Regex_signedComment_t regex = {
                .state = RSC_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        bool f = false;
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_signedComment(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                f |= found;
                OStr_append_chr(&job.sink, job.source.at[job.idx]);
                if (found) {
                        EXPECT_EQ(regex.begin, (size_t)15);
                        EXPECT_EQ(regex.id_size, (size_t)44);
                        EXPECT_EQ(job.idx, (size_t)82);
                        EXPECT_EQ(job.source.at[job.idx], (char)'\n');
                }
        }
        EXPECT_TRUE(f);

        EXPECT_EQ(job.source.size, strlen(s));
        EXPECT_EQ(job.sink.size, job.source.size);
        OStr_append_chr(&job.source, 0);
        OStr_append_chr(&job.sink, 0);
        EXPECT_TRUE(strcmp(job.sink.at, job.source.at) == 0);
}

TEST(Regex_signedComment, collect_information_2) {
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
        
        char const *s = "The lazy dog. /* Is there * something.|VB5FNX7iQCFJBz2Ka0mUzYGYgCvtEQ1SNmXPZ54e|*/ hello \nOk";
        //                              ^                      ^                                           ^
        //                              15                     38                                          82
        OStr_append_cstring(&job.source, s);

        Regex_signedComment_t regex = {
                .state = RSC_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        bool f = false;
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_signedComment(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                f |= found;
                OStr_append_chr(&job.sink, job.source.at[job.idx]);
                if (found) {
                        EXPECT_EQ(regex.begin, (size_t)15);
                        EXPECT_EQ(regex.id_size, (size_t)44);
                        EXPECT_EQ(job.idx, (size_t)82);
                        EXPECT_EQ(job.source.at[job.idx], (char)' ');
                }
        }
        EXPECT_TRUE(f);

        EXPECT_EQ(job.source.size, strlen(s));
        EXPECT_EQ(job.sink.size, job.source.size);
        OStr_append_chr(&job.source, 0);
        OStr_append_chr(&job.sink, 0);
        EXPECT_TRUE(strcmp(job.sink.at, job.source.at) == 0);
}


TEST(Regex_signedComment, test_1) {
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
        char const *s0 = "The lazy dog. /* Is there * something.|VB5FNX7iQCFJBz2Ka0mUzYGYgCvtEQ1SNmXPZ54e|*/\nOk";
        char const *s1 = "The lazy dog. // Is there * something.\nOk";
        OStr_append_cstring(&job.source, s0);
        Regex_signedComment_t regex = {
                .state = RSC_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_signedComment(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                if (found) {
                        if (job.source.at[i] == '\n') {
                                job.sink.at[regex.begin] = '/';
                                job.sink.size = i - regex.id_size;
                        } else {
                                size_t x = job.sink.size - regex.id_size;
                                job.sink.at[x++] = '*';
                                job.sink.at[x++] = '/';
                                job.sink.size = x;
                        }
                }
                OStr_append_chr(&job.sink, job.source.at[job.idx]);
        }
        OStr_append_chr(&job.sink, 0);
        EXPECT_TRUE(strcmp(s1, job.sink.at) == 0);
}

TEST(Regex_signedComment, test_2) {
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
        char const *s0 = "The lazy dog. /* Is there * something.|VB5FNX7iQCFJBz2Ka0mUzYGYgCvtEQ1SNmXPZ54e|*/ \nOk";
        char const *s1 = "The lazy dog. /* Is there * something.*/ \nOk";
        OStr_append_cstring(&job.source, s0);
        Regex_signedComment_t regex = {
                .state = RSC_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_signedComment(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                if (found) {
                        if (job.source.at[i] == '\n') {
                                job.sink.at[regex.begin] = '/';
                                job.sink.size = i - regex.id_size;
                        } else {
                                size_t x = job.sink.size - regex.id_size;
                                job.sink.at[x++] = '*';
                                job.sink.at[x++] = '/';
                                job.sink.size = x;
                        }
                }
                OStr_append_chr(&job.sink, job.source.at[job.idx]);
        }
        OStr_append_chr(&job.sink, 0);
        EXPECT_TRUE(strcmp(s1, job.sink.at) == 0);
}

TEST(Regex_signedComment, test_3) {
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
        char const *s0 = "The lazy dog. /* Is there * something.|VB5FNX7iQCFJBz2Ka0mUzYGYgCvtEQ1SNmXPZ54e|*/// hello\nOk";
        char const *s1 = "The lazy dog. /* Is there * something.*/// hello\nOk";
        OStr_append_cstring(&job.source, s0);
        Regex_signedComment_t regex = {
                .state = RSC_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_signedComment(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                if (found) {
                        if (job.source.at[i] == '\n') {
                                job.sink.at[regex.begin] = '/';
                                job.sink.size = i - regex.id_size;
                        } else {
                                size_t x = job.sink.size - regex.id_size;
                                job.sink.at[x++] = '*';
                                job.sink.at[x++] = '/';
                                job.sink.size = x;
                        }
                }
                OStr_append_chr(&job.sink, job.source.at[job.idx]);
        }
        OStr_append_chr(&job.sink, 0);
        EXPECT_TRUE(strcmp(s1, job.sink.at) == 0);
}

TEST(Regex_signedComment, test_4) {
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
        // Situation like s0 never happens
        char const *s0 = "The lazy dog. /* Is there 42\nsomething.|VB5FNX7iQCFJBz2Ka0mUzYGYgCvtEQ1SNmXPZ54e|*/\nOk";
        char const *s1 = "The lazy dog. /* Is there 42\nsomething.|VB5FNX7iQCFJBz2Ka0mUzYGYgCvtEQ1SNmXPZ54e|*/\nOk";
        OStr_append_cstring(&job.source, s0);
        Regex_signedComment_t regex = {
                .state = RSC_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_signedComment(
                        &regex, 
                        job.idx, 
                        job.source.at[job.idx]
                );
                if (found) {
                        if (job.source.at[i] == '\n') {
                                job.sink.at[regex.begin] = '/';
                                job.sink.size = i - regex.id_size;
                        } else {
                                size_t x = job.sink.size - regex.id_size;
                                job.sink.at[x++] = '*';
                                job.sink.at[x++] = '/';
                                job.sink.size = x;
                        }
                }
                OStr_append_chr(&job.sink, job.source.at[job.idx]);
        }
        OStr_append_chr(&job.sink, 0);
        EXPECT_TRUE(strcmp(s1, job.sink.at) == 0);
}

