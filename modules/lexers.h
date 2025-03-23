#ifndef cels_lexers_h 
#define cels_lexers_h

#include "../packages/cels/strings.h"
#include "../packages/cels/mems.h"

cels_warn_unused
estring_vec cels_lexers_scan(string source, const allocator *mem);

#endif
