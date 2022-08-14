#include <cstdlib>
#include <cstring>
#include <iostream>

#include <wildmatch/wildmatch.hpp>

static int die(const char *msg)
{
    std::cerr << "error: " << msg << std::endl;
	return EXIT_FAILURE;
}


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      wildmatch_cxx_test_main(cnt, arr)
#endif

int main(int argc, const char **argv)
{
    if (argc < 4) {
        return die("usage: wildmatch <mode> <string> <pattern>\n"
            "modes: wildmatch, iwildmatch, pathmatch, fnmatch");
    }

    bool match = false;

    if (!strcmp(argv[1], "wildmatch"))
        match = wild::match(argv[3], argv[2]);
    else if (!strcmp(argv[1], "iwildmatch"))
        match = wild::match(argv[3], argv[2], wild::WILDSTAR | wild::CASEFOLD);
    else if (!strcmp(argv[1], "pathmatch") || !strcmp(argv[1], "fnmatch"))
        match = wild::match(argv[3], argv[2], wild::FNMATCH);

    return (match) ? EXIT_SUCCESS : EXIT_FAILURE;
}
