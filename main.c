#include "stdio.h"
#include "stdlib.h"

typedef struct shelter{
    int day[7];
    int ID;
    struct shelter* next;
    struct shelter* prev;
}SHELTER;

typedef struct {
    unsigned long health;
    unsigned long infected;
    unsigned long cured;
    unsigned long inShelter;
    unsigned long long cost;
    SHELTER * shelter;
}DATA;

SHELTER shelter = {{},1,NULL,NULL};
DATA initData = {3000000, 0, 0, 0, 0, &shelter};
DATA data[30] = {};

void dataUpdate(int );
void infection();
void treatment();
void cure();
void dataWrite();
void outPut(int );

int main(){

    printf("Days\tHealth\tinfected\tinShelter\tcured\tcost\n");
    for (int day = 0; day < 30; ++day) {
        dataUpdate(day);
        infection();
        treatment();
        cure();
        dataWrite();
        outPut(day);
    }
}

void outPut(int day){
    printf("%d\t%lu\t%lu\t%lu\t%lu\t%llu\n",day,data[day].health,data[day].infected,data[day].inShelter,data[day].cured,data[day].cost);
}

void dataUpdate(int day){
    if (day==0){
        data[day].health = initData.health-1;
        data[day].infected = 1;
    }
}