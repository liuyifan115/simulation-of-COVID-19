#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct shelter{
    int day[7];
    int total;
}SHELTER;

typedef struct {
    unsigned long health;
    unsigned long infected;
    unsigned long cured;
    unsigned long inShelter;
    unsigned long notInShelter;
    unsigned long long cost;
    unsigned long long lockdownCost;
}DATA;

double what_this = 3;
double infectRatio = 3;
int ifLockdown = 0;
unsigned long lockdownLimit = 0;
int lockedDown = 0;

SHELTER shelter[3000] = {};
DATA day0 = {3000000, 0, 0, 0, 0,0,0};
DATA data[30] = {};

void dataInit();
void infectionUpdate(int );
void shelterUpdate(int );
void infectRatioUpdate(int );
int getShelterTotal(int);
void writeData();

int main(){
    double ration;
    printf("Set your infect ration:");
    scanf("%lf",&ration);
    printf("If lockdown (0-false 1-true):");
    scanf("%d",&ifLockdown);
    if (ifLockdown == 1){
        printf("When to lockdown:");
        scanf("%lu",&lockdownLimit);
    }
    what_this = ration;
    infectRatio = ration;
    dataInit();
    printf("%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s\n","Days","Health","Infected","InShelter","NotInShelter","Cured","Cost","LockdownCost");
    printf("%-12d%-12lu%-12lu%-12lu%-12lu%-12lu%-12llu%-12llu\n",1,data[0].health,data[0].infected,data[0].inShelter,data[0].notInShelter,data[0].cured,data[0].cost,data[0].lockdownCost);
    for (int day = 1; day < 30; ++day) {
        infectionUpdate(day);
        shelterUpdate(day);
        infectRatioUpdate(day);
        printf("%-12d%-12lu%-12lu%-12lu%-12lu%-12lu%-12llu%-12llu\n",day+1,data[day].health,data[day].infected,data[day].inShelter,data[day].notInShelter,data[day].cured,data[day].cost,data[day].lockdownCost);
    }
    writeData();
    system("pause");


}

void dataInit(){
    data[0].health = day0.health - 1;
    data[0].infected = 1;
    data[0].notInShelter = 1;
}

void infectionUpdate(int day){
    unsigned long newInfected = (unsigned long)(data[day-1].notInShelter * infectRatio);
    if (data[day-1].health < newInfected){
        newInfected = data[day-1].health;
    }
    data[day].notInShelter = data[day-1].notInShelter + newInfected;
    data[day].infected = data[day-1].infected + newInfected;
    data[day].health = data[day-1].health - newInfected;

    data[day].cured = data[day-1].cured;
    data[day].inShelter = data[day-1].inShelter;
    data[day].cost = data[day-1].cost;
    data[day].lockdownCost = data[day-1].lockdownCost;

    if (data[day].infected >= lockdownLimit && ifLockdown){
        lockedDown = 1;
    }
    if (data[day].infected < lockdownLimit && ifLockdown){
        lockedDown = 0;
    }
    if (lockedDown){
        data[day].lockdownCost += 60000000;
    }
}

void shelterUpdate(int day){
    unsigned long newCured = 0;
    for (int i = 0; i < 3000; ++i) {
        newCured += shelter[i].day[6];
    }
    data[day].cured += newCured;
    data[day].infected -= newCured;
    data[day].inShelter -= newCured;


    for (int i = 0; i < 3000; ++i) {
        for (int j = 6; j > 0; --j) {
            shelter[i].day[j] = shelter[i].day[j-1];
        }
        shelter[i].day[0] = 0;
        shelter[i].total = getShelterTotal(i);
    }

    unsigned long newInShelter = data[day-1].notInShelter;
    data[day].inShelter += newInShelter;
    data[day].notInShelter -= newInShelter;
    for (int i = 0; i < 3000; ++i) {
        if (newInShelter <= 0){
            break;
        }
        if (newInShelter > 1000 - shelter[i].total){
            shelter[i].day[0] = 1000 - shelter[i].total;
            newInShelter -= shelter[i].day[0];
            shelter[i].total = 1000;
        }
        else{
            shelter[i].day[0] = (int)newInShelter;
            shelter[i].total += (int)newInShelter;
            newInShelter = 0;
        }
    }

    unsigned long long newCost = 0;
    for (int i = 0; i < 3000; ++i) {
        newCost += shelter[i].total*200;
    }
    data[day].cost += newCost;


    //shelter_day_x.txt
    char filename[50] = {};
    char date[3] = {};
    for (int j = 0; j < 50; ++j) {
        filename[j] = 0;
    }
    for (int j = 0; j < 3; ++j) {
        date[j] = 0;
    }
    itoa(day+1,date,10);
    strcat(filename,"./output/shelter_day_");
    strcat(filename,date);
    strcat(filename,".txt");

    FILE *p1 = fopen(filename,"w");
    fprintf(p1,"Day %d:\n",day+1);
    fprintf(p1,"ID\tDaysStayedIn(0..6)\n");
    for (int j = 0; j < 3000; ++j) {
        if (shelter[j].total != 0){
            fprintf(p1,"%d",j+1);
            for (int k = 0; k < 7; ++k) {
                fprintf(p1,"\t%d",shelter[j].day[k]);
            }
            fprintf(p1,"\n");
        }
    }
    fclose(p1);


}

void infectRatioUpdate(int day){
    infectRatio = what_this * ((double)data[day].health / (data[day].health + data[day].cured + data[day].infected));
    if (lockedDown && ifLockdown){
        infectRatio = 0.1;
    }
}

int getShelterTotal(int index){
    int total = 0;
    for (int i = 0; i < 7; ++i) {
        total += shelter[index].day[i];
    }
    return total;
}

void writeData(){
    char filename[50] ={};
    char date[3] = {};


    //total_cost.txt
    FILE *fp1 = fopen("./output/total_cost.txt","w");
    fprintf(fp1,"%llu",data[29].cost+data[29].lockdownCost);
    fclose(fp1);

    //infected_day_x.txt
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 50; ++j) {
            filename[j] = 0;
        }
        for (int j = 0; j < 3; ++j) {
            date[j] = 0;
        }
        itoa(i+1,date,10);
        strcat(filename,"./output/infected_day_");
        strcat(filename,date);
        strcat(filename,".txt");
        FILE *p1 = fopen(filename,"w");
        fprintf(p1,"%lu\n",data[i].infected);
        fclose(p1);
    }

    //cost_of_lock_down_population_day_x.txt
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 50; ++j) {
            filename[j] = 0;
        }
        for (int j = 0; j < 3; ++j) {
            date[j] = 0;
        }
        itoa(i+1,date,10);
        strcat(filename,"./output/cost_of_lock_down_population_day_");
        strcat(filename,date);
        strcat(filename,".txt");
        FILE *p1 = fopen(filename,"w");
        fprintf(p1,"%llu\n",data[i].lockdownCost);
        fclose(p1);
    }

}