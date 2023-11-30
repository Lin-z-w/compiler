#include "semantics.h"

typedef struct AliasTable_* AliasTable;
typedef struct Alias_ Alias;
typedef struct ParamTable_* ParamTable;

struct Alias_ {
    char* name;
    char* alias;
};

struct AliasTable_
{
    Alias alias;
    AliasTable next;
};

struct ParamTable_
{
    char* name;
    ParamTable next;
};


void genInterCode(FILE* f);