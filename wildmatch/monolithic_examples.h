
#pragma once

#if defined(BUILD_MONOLITHIC)

#ifdef __cplusplus
extern "C" {
#endif

int wildmatch_test_main(int argc, const char **argv);
int wildmatch_cxx_test_main(int argc, const char** argv);

#ifdef __cplusplus
}
#endif

#endif
