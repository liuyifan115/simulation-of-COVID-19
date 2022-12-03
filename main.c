#include "stdio.h"
#include "stdlib.h"

typedef struct shelter{
    int day[7];
    int ID;
    struct shelter* next;
    struct shelter* prev;
}SHELTER;

int main(){
    SHELTER shelter = {{},1,NULL,NULL};
}