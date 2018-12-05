/* Copyright (c) 2018 Baidu, Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include <map>
#include <list>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <sys/time.h>
#include <iostream>
#include "stdlib.h"
#include "ilac.h"
#include "lac_main.h"

void *g_lac_handle = NULL;
int g_line_count = 0;
long g_usec_used = 0;

pthread_mutex_t g_mutex;

class TimeUsing {
public:
    explicit TimeUsing() {
        start();
    }

    virtual ~TimeUsing() {
    }

    void start() {
        gettimeofday(&_start, NULL);
    }

    long using_time() {
        gettimeofday(&_end, NULL);
        long using_time = (long) (_end.tv_sec - _start.tv_sec) * (long) 1000000
                          + (long) (_end.tv_usec - _start.tv_usec);
        return using_time;
    }

private:
    struct timeval _start;
    struct timeval _end;
};

int init_dict(const char *conf_dir) {
    g_lac_handle = lac_create(conf_dir);
    std::cerr << "create lac handle successfully" << std::endl;
    return 0;
}

int destroy_dict() {
    lac_destroy(g_lac_handle);
    return 0;
}

/**
 * 切分语句，返回分词结果，每个词语的词性，以及每个词语在句子中的起始和截止位置
 * @param conf_dir
 * @param max_result_num
 * @param content
 * @return
 */
char *lexer(const char *conf_dir, int max_result_num, const char *content) {
    std::cerr << "content: " << content << std::endl;
    std::cerr << "configure dir: " << conf_dir << std::endl;
    init_dict(conf_dir);

    if (g_lac_handle == NULL) {
        std::cerr << "creat g_lac_handle error" << std::endl;
        return NULL;
    }

    void *lac_buff = lac_buff_create(g_lac_handle);
    if (lac_buff == NULL) {
        std::cerr << "creat lac_buff error" << std::endl;
        return NULL;
    }
    std::cerr << "create lac buff successfully" << std::endl;
    tag_t *results = new tag_t[max_result_num];

    int result_num = lac_tagging(g_lac_handle,
                                 lac_buff, content, results, max_result_num);
    std::cerr << "result_num: " << result_num << std::endl;
    if (result_num < 0) {
        std::cerr << "lac tagging failed : content = " << content
                  << std::endl;
        return NULL;
    }

    std::stringstream ss;
    std::string content_str = content;
    for (int i = 0; i < result_num; i++) {
        std::cerr << "offset: " << results[i].offset << ", length: " << results[i].length << std::endl;
        std::string name = content_str.substr(results[i].offset,
                                              results[i].length);
        if (i >= 1) {
            std::cerr << "\t";
        }
        std::cerr << name << " " << results[i].type << " "
                  << results[i].offset << " " << results[i].length;
        ss << name << " /" << results[i].type << " " << results[i].offset << " " << results[i].length;
    }
    std::cerr << std::endl;
    std::cerr << ss.str();

    lac_buff_destroy(g_lac_handle, lac_buff);
    delete[] results;
    destroy_dict();
    std::cerr << "return value: " << ss.str();
    char *buf = strdup(ss.str().c_str());
    printf("allocated address: %p\n", buf);
    return buf;
}

/**
 * 切分语句，返回分词结果及每个词语的词性
 * @param conf_dir
 * @param max_result_num
 * @param content
 * @return
 */
char *posseg(const char *conf_dir, int max_result_num, const char *content) {
    std::cerr << "content: " << content << std::endl;
    std::cerr << "configure dir: " << conf_dir << std::endl;
    init_dict(conf_dir);

    if (g_lac_handle == NULL) {
        std::cerr << "creat g_lac_handle error" << std::endl;
        return NULL;
    }

    void *lac_buff = lac_buff_create(g_lac_handle);
    if (lac_buff == NULL) {
        std::cerr << "creat lac_buff error" << std::endl;
        return NULL;
    }
    std::cerr << "create lac buff successfully" << std::endl;
    tag_t *results = new tag_t[max_result_num];

    int result_num = lac_tagging(g_lac_handle,
                                 lac_buff, content, results, max_result_num);
    std::cerr << "result_num: " << result_num << std::endl;
    if (result_num < 0) {
        std::cerr << "lac tagging failed : content = " << content
                  << std::endl;
        return NULL;
    }

    std::stringstream ss;
    std::string content_str = content;
    for (int i = 0; i < result_num; i++) {
        std::cerr << "offset: " << results[i].offset << ", length: " << results[i].length << std::endl;
        std::string name = content_str.substr(results[i].offset,
                                              results[i].length);
        if (i >= 1) {
            std::cerr << "\t";
        }
        std::cerr << name << " " << results[i].type << " "
                  << results[i].offset << " " << results[i].length;
        ss << name << " /" << results[i].type << " ";
    }
    std::cerr << std::endl;
    std::cerr << ss.str();

    lac_buff_destroy(g_lac_handle, lac_buff);
    delete[] results;
    destroy_dict();
    std::cerr << "return value: " << ss.str();
    char *buf = strdup(ss.str().c_str());
    printf("allocated address: %p\n", buf);
    return buf;
}

/**
 * 切分语句，返回分词结果
 * @param conf_dir
 * @param max_result_num
 * @param content
 * @return
 */
char *cut(const char *conf_dir, int max_result_num, const char *content) {
    std::cerr << "content: " << content << std::endl;
    std::cerr << "configure dir: " << conf_dir << std::endl;
    init_dict(conf_dir);

    if (g_lac_handle == NULL) {
        std::cerr << "creat g_lac_handle error" << std::endl;
        return NULL;
    }

    void *lac_buff = lac_buff_create(g_lac_handle);
    if (lac_buff == NULL) {
        std::cerr << "creat lac_buff error" << std::endl;
        return NULL;
    }
    std::cerr << "create lac buff successfully" << std::endl;
    tag_t *results = new tag_t[max_result_num];

    int result_num = lac_tagging(g_lac_handle,
                                 lac_buff, content, results, max_result_num);
    std::cerr << "result_num: " << result_num << std::endl;
    if (result_num < 0) {
        std::cerr << "lac tagging failed : content = " << content
                  << std::endl;
        return NULL;
    }

    std::stringstream ss;
    std::string content_str = content;
    for (int i = 0; i < result_num; i++) {
        std::cerr << "offset: " << results[i].offset << ", length: " << results[i].length << std::endl;
        std::string name = content_str.substr(results[i].offset,
                                              results[i].length);
        if (i >= 1) {
            std::cerr << "\t";
        }
        std::cerr << name << " " << results[i].type << " "
                  << results[i].offset << " " << results[i].length;
        ss << name << " ";
    }
    std::cerr << std::endl;
    std::cerr << ss.str();

    lac_buff_destroy(g_lac_handle, lac_buff);
    delete[] results;
    destroy_dict();
    std::cerr << "return value: " << ss.str();
    char *buf = strdup(ss.str().c_str());
    printf("allocated address: %p\n", buf);
    return buf;
}

void freeme(char *ptr)
{
    printf("freeing address: %p\n", ptr);
    free(ptr);
}

std::string test_main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0]
                  << " + conf_dir + max_tokens + content"
                  << std::endl;
        exit(-1);
    }
    const char *conf_dir = argv[1];
    int max_result_num = atoi(argv[2]);
    const char *content = argv[3];
    std::cout << "conf_dif" << conf_dir << std::endl;
    std::cout << "max_result_num" << max_result_num << std::endl;
    std::cout << "content" << content << std::endl;

    TimeUsing t;

    g_usec_used += t.using_time();
    const char *result = cut_sentence(conf_dir, max_result_num, content);

    double time_using = (double) g_usec_used / 1000000.0;
    std::cerr << "page num: " << g_line_count << std::endl;
    std::cerr << "using time: " << time_using << std::endl;
    std::cerr << "page/s : " << g_line_count / time_using << std::endl;

    destroy_dict();
    return result;
}

int sum(int a, int b) {
    return a + b;
}

int main(int argc, char *argv[]) {
    test_main(argc, argv);
    return 0;
}
