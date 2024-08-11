#include "gtest/gtest.h"

#include "OJob.h"
#include "OStr.h"
#include "Regex_commentOpen.h"

#define MEM_SIZE 1000*1024

TEST(Regex_commentOpen, no_comment) {
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

        Regex_commentOpen_t regex = {
                .state = REGEX1_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_commentOpen(
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

TEST(Regex, collect_information) {
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
        //                              15                     38                                          86
        OStr_append_cstring(&job.source, s);

        Regex_commentOpen_t regex = {
                .state = REGEX1_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        bool f = false;
        for (size_t i = 0; i < job.source.size; i++) {
                job.idx = i;
                bool const found = Regex_commentOpen(
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

