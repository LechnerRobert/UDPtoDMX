#if DEBUG_CONSOLE == 1 
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

void DEBUG_BEGIN(uint8_t level){
  printf(" %d: ",level);
}

void DEBUG_PRINT(const int i) {
  printf("%d",i);
}

void DEBUG_PRINT(const char *c) {
  printf(c);
}

void DEBUG_END(){
  printf("\n");
}

void DEBUG_INIT(){
};

#endif