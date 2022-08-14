/* This test is from Git's t/helper/test-wildmatch.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wildmatch/wildmatch.h>


static int die(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    return EXIT_FAILURE;
}


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      wildmatch_test_main(cnt, arr)
#endif

int main(int argc, const char **argv)
{
    int i;
    if (argc < 4) {
        return die("usage: test-wildmatch <mode> <string> <pattern>\n"
            "modes: wildmatch, iwildmatch, pathmatch");
    }

    for (i = 2; i < argc; i++) {
        if (argv[i][0] == '/')
            return die("Forward slash is not allowed at the beginning of the\n"
                "pattern because Windows does not like it. Use `XXX/' instead.");
        else if (!strncmp(argv[i], "XXX/", 4))
            argv[i] += 3;
    }
    if (!strcmp(argv[1], "wildmatch"))
        return !!wildmatch(argv[3], argv[2], WM_WILDSTAR);
    else if (!strcmp(argv[1], "iwildmatch"))
        return !!wildmatch(argv[3], argv[2], WM_WILDSTAR | WM_CASEFOLD);
    else if (!strcmp(argv[1], "pathmatch"))
        return !!wildmatch(argv[3], argv[2], 0);
    else
        return EXIT_FAILURE;
}
