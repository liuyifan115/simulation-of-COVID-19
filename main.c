#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//这是定义的方舱的数据类型，病人会在里面待7天，
typedef struct shelter{
    int day[7]; //day的下标就表明了他在里面待了多少天
    int total; //这是这一个方舱里面的总人数
}SHELTER;

//这是一个存放了各种数据的数据类型
typedef struct {
    unsigned long health; //还有多少人未感染
    unsigned long infected; //已经有多少人感染了
    unsigned long cured; //已经有多少人治愈了
    unsigned long inShelter; //在方舱里面的人数
    unsigned long notInShelter; //未在方舱里面但是已经感染的人数
    unsigned long long cost; //方舱的花费
    unsigned long long lockdownCost; //封城的花费
}DATA;

//这边是各种全局变量
double what_this = 3; //这是什么鬼？
double infectRatio = 3; //感染率，比如说一个人一天能传播三个人，那么感染率就是3
int ifLockdown = 0; //是否会封城
unsigned long lockdownLimit = 0; //感染到多少人就会封城
int lockedDown = 0; //是否已经封城
SHELTER shelter[3000] = {};//一共有3,000,000人，一个方舱能住1000人，所以3000个足矣
DATA day0 = {3000000, 0, 0, 0, 0,0,0};//这是初始的数据
DATA data[30] = {};//这是30天，按每一天存放的数据

//这边是各种函数
void dataInit();//数据初始化函数
void infectionUpdate(int );//感染函数
void shelterUpdate(int );//和方舱数据有关的函数
void infectRatioUpdate(int );//更新感染率的函数
int getShelterTotal(int);//获取一个方舱内有多少人的函数
void writeData(int );//写文件函数
void whatIWant();//这又是什么鬼？

int main(){
    //创建目录来存放输出文件
    system("mkdir output");

    //自定义感染率
    double ration;
    printf("Set your infect ration:");
    scanf("%lf",&ration);

    //选择是否会封城
    printf("If lockdown (0-false 1-true):");
    scanf("%d",&ifLockdown);

    //如果会封城，那么感染到多少人会封城
    if (ifLockdown == 1){
        printf("When to lockdown:");
        scanf("%lu",&lockdownLimit);
    }
    //保存自定义数据
    what_this = ration;
    infectRatio = ration;

    //初始化数据
    dataInit();

    //控制台输出，用于调试
    printf("%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s\n","Days","Health","Infected","InShelter","NotInShelter","Cured","Cost","LockdownCost");
    printf("%-12d%-12lu%-12lu%-12lu%-12lu%-12lu%-12llu%-12llu\n",1,data[0].health,data[0].infected,data[0].inShelter,data[0].notInShelter,data[0].cured,data[0].cost,data[0].lockdownCost);

    writeData(0);
    //用循环模拟30天的数据
    for (int day = 1; day < 30; ++day) {
        infectionUpdate(day);
        shelterUpdate(day);
        infectRatioUpdate(day);
        printf("%-12d%-12lu%-12lu%-12lu%-12lu%-12lu%-12llu%-12llu\n",day+1,data[day].health,data[day].infected,data[day].inShelter,data[day].notInShelter,data[day].cured,data[day].cost,data[day].lockdownCost);
        writeData(day);
    }
    whatIWant();
    system("pause");
    return 0;
}

//初始化数据，其实就是创造一个感染者
void dataInit(){
    data[0].health = day0.health - 1;
    data[0].infected = 1;
    data[0].notInShelter = 1;
}

//进行感染的过程
void infectionUpdate(int day){

    //新感染的人数就等于没有进方舱的感染者乘以感染率
    unsigned long newInfected = (unsigned long)(data[day-1].notInShelter * infectRatio);

    //如果新感染的人数超过了剩余健康人数，那么新感染人数就是剩余的健康人数
    if (data[day-1].health < newInfected){
        newInfected = data[day-1].health;
    }

    //将相应数据写入到当天的元素中
    data[day].notInShelter = data[day-1].notInShelter + newInfected;
    data[day].infected = data[day-1].infected + newInfected;
    data[day].health = data[day-1].health - newInfected;
    data[day].cured = data[day-1].cured;
    data[day].inShelter = data[day-1].inShelter;
    data[day].cost = data[day-1].cost;
    data[day].lockdownCost = data[day-1].lockdownCost;

    //判断是否要封城
    if (data[day].infected >= lockdownLimit && ifLockdown){
        lockedDown = 1;
    }

    //判断是否要解除封城
    if (data[day].infected < lockdownLimit && ifLockdown){
        lockedDown = 0;
    }

    //如果封城，那么就在封城开支上加上6,000,000
    if (lockedDown){
        data[day].lockdownCost += 60000000;
    }
}

void shelterUpdate(int day){
    //新治愈的人数等于方舱中出来的人数
    unsigned long newCured = 0;
    for (int i = 0; i < 3000; ++i) {
        newCured += shelter[i].day[6];
    }

    //将相应数据写入
    data[day].cured += newCured;
    data[day].infected -= newCured;
    data[day].inShelter -= newCured;

    //方舱中病人的累计天数增长一天
    for (int i = 0; i < 3000; ++i) {
        for (int j = 6; j > 0; --j) {
            shelter[i].day[j] = shelter[i].day[j-1];
        }
        shelter[i].day[0] = 0;
        shelter[i].total = getShelterTotal(i);
    }

    //因为设置为感染后第二天被送往方舱，所以当天送到方舱的人数应该是上一天未进方舱的感染人数
    unsigned long newInShelter = data[day-1].notInShelter;
    data[day].inShelter += newInShelter;
    data[day].notInShelter -= newInShelter;

    //从第一个方舱开始收治感染者
    for (int i = 0; i < 3000; ++i) {
        //如果没有需要入住的，则退出循环
        if (newInShelter <= 0){
            break;
        }
        //如果当前方舱剩余空位小于需要入住的总人数，则将方舱住满
        if (newInShelter > 1000 - shelter[i].total){
            shelter[i].day[0] = 1000 - shelter[i].total;
            newInShelter -= shelter[i].day[0];
            shelter[i].total = 1000;
        }
        //否则，直接全部住进去
        else{
            shelter[i].day[0] = (int)newInShelter;
            shelter[i].total += (int)newInShelter;
            newInShelter = 0;
        }
    }

    //计算方舱的花费
    unsigned long long newCost = 0;
    for (int i = 0; i < 3000; ++i) {
        newCost += shelter[i].total*200;
    }
    data[day].cost += newCost;
}

void infectRatioUpdate(int day){
    //因为默认感染之后无法再次被感染，而已经被感染的也无法被感染，所以感染率会随着健康人数的下降有所下降
    infectRatio = what_this * ((double)data[day].health / (double)(data[day].health + data[day].cured + data[day].infected));

    //如果封城的话，就讲感染率下降到0.1
    if (lockedDown && ifLockdown){
        infectRatio = 0.1;
    }
}

int getShelterTotal(int index){
    //获取一个方舱内的总人数
    int total = 0;
    for (int i = 0; i < 7; ++i) {
        total += shelter[index].day[i];
    }
    return total;
}

void writeData(int day){
    char filename[100] ={};//存放文件名的字符串

    //写入shelter_day_x.txt
    for (int j = 0; j < 100; ++j) { //清空filename字符串
        filename[j] = 0;
    }
    sprintf(filename,"./output/shelter_day_%d.txt",day+1); //通过sprintf来切换日期
    FILE *fp = fopen(filename,"w");
    fprintf(fp,"Day %d:\n",day+1);
    fprintf(fp,"ID\tDaysStayedIn(0..6)\n");
    if (day != 0){
        for (int j = 0; j < 3000; ++j) {
            if (shelter[j].total != 0){
                fprintf(fp,"%d",j+1);
                for (int k = 0; k < 7; ++k) {
                    fprintf(fp,"\t%d",shelter[j].day[k]);
                }
                fprintf(fp,"\n");
            }
        }
        fclose(fp);
    }


    //写入total_cost_day_x.txt
    for (int j = 0; j < 100; ++j) { //清空filename字符串
        filename[j] = 0;
    }
    sprintf(filename,"./output/total_cost_day_%d.txt",day+1); //通过sprintf来切换日期
    fp = fopen(filename,"w");
    fprintf(fp,"%llu",data[day].cost+data[day].lockdownCost);
    fclose(fp);

    //写入infected_day_x.txt
    for (int j = 0; j < 100; ++j) { //清空filename字符串
        filename[j] = 0;
    }
    sprintf(filename,"./output/infected_day_%d.txt",day+1); //通过sprintf来切换日期
    fp = fopen(filename,"w");
    fprintf(fp,"%lu",data[day].infected);
    fclose(fp);

    //写入cost_of_lock_down_population_day_x.txt
    for (int j = 0; j < 100; ++j) { //清空filename字符串
        filename[j] = 0;
    }
    sprintf(filename,"./output/cost_of_lock_down_population_day_%d.txt",day+1); //通过sprintf来切换日期
    fp = fopen(filename,"w");
    fprintf(fp,"%llu",data[day].lockdownCost);
    fclose(fp);
}

void whatIWant(){
    FILE *fp = fopen("./output/I_want_this.csv","w");
    for (int i = 0; i < 29; ++i) {
        fprintf(fp,"%d,",i+1);
    }
    fprintf(fp,"%d\n",30);
    for (int i = 0; i < 29; ++i) {
        fprintf(fp,"%lu,",data[i].infected);
    }
    fprintf(fp,"%lu\n",data[29].infected);
    for (int i = 0; i < 29; ++i) {
        fprintf(fp,"%lu,",data[i].inShelter);
    }
    fprintf(fp,"%lu\n",data[29].inShelter);
    for (int i = 0; i < 29; ++i) {
        fprintf(fp,"%llu,",data[i].cost+data[i].lockdownCost);
    }
    fprintf(fp,"%llu\n",data[29].cost+data[29].lockdownCost);
    fclose(fp);
}
