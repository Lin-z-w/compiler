#include "semantics.h"

typedef struct AliasTable_* AliasTable;
typedef struct Alias_ Alias;

struct Alias_ {
    char* name;
    char* alias;
};

struct AliasTable_
{
    Alias alias;
    AliasTable next;
};

void genInterCode(FILE* f);