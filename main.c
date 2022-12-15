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

double infectRatio = 3;
SHELTER shelter = {{0,0,0,0,0,0,0},1,NULL,NULL};
DATA day0 = {3000000, 0, 0, 0, 0, &shelter};
DATA data[30] = {};

void dataInit();
void infectionUpdate(int );
void shelterUpdate(int );
//void dataWrite();
void outPut(int );
unsigned long getInShelter(int ); //获取某个方舱的人数
unsigned long getTotal(); //获取全部方舱的人数

int main(){
    dataInit();
    printf("Days\tHealth\tinfected\tinShelter\tcured\tcost\n");
    for (int day = 0; day < 30; ++day) {
        infectionUpdate(day);
        shelterUpdate(day);
//        dataWrite();
        outPut(day);
    }
}

unsigned long getInShelter(int ID){
    SHELTER *p1 = day0.shelter;
    unsigned long total = 0;
    while (p1 != NULL){
        if (p1->ID == ID){
            for (int i = 0; i < 7; ++i) {
                total += p1->day[i];
            }
            return total;
        }
        p1 = p1->next;
    }
}

unsigned long getTotal(){
    SHELTER *p1 = day0.shelter;
    unsigned long total = 0;
    while (p1 != NULL){
        for (int i = 0; i < 7; ++i) {
            total += p1->day[i];
        }
        p1 = p1->next;
    }
    return total;
}

void outPut(int day){
    printf("%d\t%lu\t%lu\t%lu\t%lu\t%llu\n",day+1,data[day].health,data[day].infected,data[day].inShelter,data[day].cured,data[day].cost);
}

void dataInit(){
    data[0].health = day0.health - 1;
    data[0].infected = 1;
}

void infectionUpdate(int day){
    if(day==0){
        return;
    }
    infectRatio *= ((double)(data[day].health)/(data[day].health + data[day].cured));
    data[day].infected = (unsigned long)(infectRatio * data[day-1].infected);
    data[day].cured = data[day-1].cured;
    data[day].health = data[day-1].health - (unsigned long)infectRatio * data[day-1].infected;
    data[day].inShelter = data[day-1].inShelter;
    data[day].cost = data[day-1].cost;
}

void shelterUpdate(int day){
    unsigned long newInfected = (unsigned long)(data[day-1].infected * infectRatio);
    SHELTER *p1 = day0.shelter;
    int newCured = 0;
    while (p1 != NULL){
        newCured += p1->day[6];
        for (int i = 6; i > 0; --i) {
            p1->day[i] = p1->day[i-1];
        }
        data[day].cured += newCured;
        data[day].infected -= newCured;
        data[day].inShelter -= newCured;
        if (newInfected > 0){
            p1->day[0] = 1000 - (int)getInShelter(p1->ID);
            newInfected -= p1->day[0];
            data[day].inShelter += p1->day[0];
        }
        newCured = 0;
        p1 = p1->next;
    }
    if (newInfected <= 0){
        return;
    }
    p1 = day0.shelter;
    while (p1->next != NULL){
        p1 = p1->next;
    }
    while (newInfected > 0){
        if(newInfected >= 1000){
            p1->next = (SHELTER *) malloc(sizeof(SHELTER));
            p1->next->ID = p1->ID+1;
            p1->next->day[0] = 1000;
            p1->next->next = NULL;
            p1->next->prev = p1;
            p1 = p1->next;
            newInfected -= 1000;
            data[day].inShelter += 1000;
        }
        else{
            p1->next = (SHELTER *) malloc(sizeof(SHELTER));
            p1->next->ID = p1->ID+1;
            p1->next->day[0] = (int)newInfected;
            p1->next->next = NULL;
            p1->next->prev = p1;
            p1 = p1->next;
            data[day].inShelter += newInfected;
            newInfected = 0;
        }
    }
}

