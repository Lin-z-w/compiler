#include "InterCode.h"

void insertInterCodes(InterCodes intercodes, InterCodes next) {
    assert(intercodes != NULL);
    while (intercodes->next != NULL)
    {
        intercodes = intercodes->next;
    }
    intercodes->next = next;
}

void displayInterCodes(InterCodes ics) {

}