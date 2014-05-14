#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h> 

#define TABLESIZE 5 
#define PAGESIZE 5 
#define INSTRUCTIONSIZE 100 


typedef struct _page {
  int num; 
  struct _page *next; 
} PageNode, *PageList, *PagePointer; 

int instructionArray[INSTRUCTIONSIZE]; 

PageList pageTable; 


void 
genInstruction(int *Array) {
  int i = 0; 
  int index; 
  srand( (unsigned int)time(0) );
  for (; i < INSTRUCTIONSIZE; i++) 
    Array[i] = INSTRUCTIONSIZE; 
  for (i = 0; i < INSTRUCTIONSIZE; i++) {
    do {
      /*srand( (unsigned int)time(0) );*/
      index = rand() % INSTRUCTIONSIZE; 
      /*printf("%d\t", index); */
    } while(Array[index] != INSTRUCTIONSIZE); 
    /*printf("%d\t", index); */
    Array[index] = i; 
  }
}

void 
printInstruction() {
  printf("指令流地址序列: \n"); 
  int i = 0; 
  for (; i < INSTRUCTIONSIZE; i++) {
    printf("%d\t", instructionArray[i]); 
    if (i % 5 == 4) 
      printf("\n"); 
  } 
  printf("\n"); 
  sleep(2); 
}

int 
getTableLength() {
  PagePointer cursor = pageTable->next; 
  int length = 0; 
  while (cursor != NULL) {
    cursor = cursor->next; 
    length++; 
  }
  return length; 
}

void 
printTable() {
  printf("[ "); 
  PagePointer cursor = pageTable->next; 
  while (cursor != NULL) {
    printf("%d ", cursor->num); 
    cursor = cursor->next; 
  } 
  printf("]\n"); 
}

int 
isInTable(int pageNum) {
  PagePointer cursor = pageTable->next; 
  while (cursor != NULL) {
    if (cursor->num == pageNum) 
      return 1; 
    cursor = cursor->next; 
  }
  return -1; 
}

void 
run() {
  int i = 0; 
  int pageNum; 
  PagePointer cursor; 
  PagePointer cursorPrev; 
  for (; i < INSTRUCTIONSIZE; i++) {
    usleep(256000); 
    pageNum = instructionArray[i] / PAGESIZE; 
    if (isInTable(pageNum) == 1) {
      printf("[命中: address#%d\tin page#%d]\t", instructionArray[i], pageNum); printTable(); 
      /*printf("Hit: address#%d\tin page#%d\t", instructionArray[i], pageNum); printTable(); */
    } else {
      if (getTableLength() < 5) {
        cursor = pageTable->next; 
        cursorPrev = pageTable; 
        while (cursor != NULL) {
          cursor = cursor->next; 
          cursorPrev = cursorPrev->next; 
        }
        cursor = (PagePointer)malloc(sizeof(PageNode)); 
        cursor->num = pageNum; 
        cursor->next = NULL; 
        cursorPrev->next = cursor; 
        printf("[调入: address#%d\tin page#%d]\t", instructionArray[i], pageNum); printTable(); 
        /*printf("In : address#%d\tin page#%d\t", instructionArray[i], pageNum); printTable(); */
      } else {
        PagePointer out = pageTable->next; 
        pageTable->next = out->next; 
        printf("[换出: page#%d]\t", out->num); 
        /*printf("Out: page#%d\t", out->num); */
        out->num = pageNum; 
        out->next = NULL; 
        cursor = pageTable->next; 
        cursorPrev = pageTable; 
        while (cursor != NULL) {
          cursor = cursor->next; 
          cursorPrev = cursorPrev->next; 
        }
        cursorPrev->next = out; 
        printf("[调入: address#%d\tin page#%d]\t", instructionArray[i], pageNum); printTable(); 
        /*printf("In : address#%d\tin page#%d\t", instructionArray[i], pageNum); printTable(); */
      }
    }
  }

}


int 
main(int argc, 
    char **argv) {
  genInstruction(instructionArray); 
  printInstruction(); 
  pageTable = (PageList)malloc(sizeof(PageNode)); 
  pageTable->next = NULL; 

  run(); 

  return 0; 
}
