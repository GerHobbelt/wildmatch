
#include "wildmatch/monolithic_examples.h"

// define a name for the monolith
#define USAGE_NAME   "wildmatch_examples"

// load the monolithic definitions which you need for the dispatch table:
#include "monolithic_main_internal_defs.h"

// declare your own monolith dispatch table:
MONOLITHIC_CMD_TABLE_START()
{ "test", { .fa = wildmatch_test_main} },
{ "cxx_test", { .fa = wildmatch_cxx_test_main } },
MONOLITHIC_CMD_TABLE_END();

// load the monolithic core dispatcher
#include "monolithic_main_tpl.h"
