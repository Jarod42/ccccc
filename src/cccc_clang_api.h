#ifndef CCCC_CLANG_API_H
#define CCCC_CLANG_API_H

#include <vector>

class LocalStat;

void ComputeStats(const char* filename, int extraArgsCount, const char *extraArgs[], std::vector<LocalStat>& /* OUT */ localStats);


#endif // CCCC_CLANG_API_H
