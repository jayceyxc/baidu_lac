//
// Created by yuxuecheng on 2018-11-30.
//

#ifndef LAC_LAC_MAIN_H
#define LAC_LAC_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

///
/// \brief tagging, tag the content
/// \param conf_dir, configure file directory
/// \param max_result_num, limit of tagged results, tagging failed when the number of results exceeds the limit
/// \param content, the content need to be tagged
/// \return tagged results, or ""
///
const char* cut_sentence(const char* conf_dir, int max_result_num, const char* content);

int sum(int a, int b);

#ifdef __cplusplus
}
#endif

#endif //LAC_LAC_MAIN_H
