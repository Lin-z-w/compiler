#include "test.h"

int testInterCodes() {
  InterCodes test1, test2;
  Operand op1 = constOperand(0), op2 = constOperand(1), op3 = varOperand("test");
  InterCode ic1 = assignCode(op3, op1), ic2 = assignCode(op3, op2), ic3 = binopCode(op3, op1, op2, ADDCODE);
  test1 = interCodes(ic1);
  test2 = interCodes(ic2);
  insertInterCodes(test1, test2);
  insertInterCode(test1, ic3);
  displayInterCodes(test1);
}