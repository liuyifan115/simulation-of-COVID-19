#include "stdio.h"
#include "stdlib.h"

//这是定义的方舱的数据类型，病人会在里面待7天，
typedef struct shelter{
    int day[7]; //day的下标就表明了他在里面待了多少天
    int total; //这是这一个方舱里面的总人数
}SHELTER;

//这是一个存放了各种数据的数据类型
typedef struct {
    unsigned long health; //还有多少人未感染
    unsigned long infected; //已经有多少人感染了
    unsigned long immune; //已经有多少人免疫了
    unsigned long inShelter; //在方舱里面的人数
    unsigned long atHome; //居家隔离的人数
    unsigned long notInShelter; //未在方舱里面但是已经感染的人数
    unsigned long long cost; //方舱的花费
    unsigned long long lockdownCost; //封城的花费
}DATA;

//程序参数设置
struct {
    int days;//模拟多少天

    double what_this; //这是什么鬼？
    double infectRatio; //感染率，比如说一个人一天能传播三个人，那么感染率就是3

    int ifLockdown; //是否会封城
    unsigned long lockdownLimit; //感染到多少人就会封城
    int lockedDown; //是否已经封城

    int ifStayAtHome;//是否可以居家隔离
    double ratioStayAtHome;//多少比例的人可以居家隔离
    double infectRatioAtHome;//在居家的时候的感染率

    int ifInfectedAgain;//是否会再次被感染
    int infectedAgainLimit;//几天后可以再次被感染

    DATA *data;//数据存放的地方
} settings = {30,
              2.5,
              2.5,
              1,
              1000000,
              0,
              1,
              0.7,
              0.05,
              1,
              10,
              NULL
};


SHELTER shelter[3000] = {};//一共有3,000,000人，一个方舱能住1000人，所以3000个足矣
DATA day0 = {3000000, 0, 0, 0, 0,0,0};//这是初始的数据

//这边是各种函数
void readme();//说明
void setting();//程序设置
void consoleOutput(int );//控制台输出
void dataInit();//数据初始化函数
void infectionUpdate(int );//感染函数
void shelterUpdate(int );//和方舱数据有关的函数
unsigned long infectAgain(int );//让你再次被感染
unsigned long stayAtHome(int day);//计算出解除居家隔离的人数
void infectRatioUpdate(int );//更新感染率的函数
int getShelterTotal(int);//获取一个方舱内有多少人的函数
void writeData(int );//写文件函数
void whatIWant();//这又是什么鬼？

int main(){
    readme();

    //创建目录来存放输出文件
    system("mkdir output");

    settings.data = (DATA *)malloc(settings.days* sizeof(DATA));

    //初始化数据
    dataInit();


    //控制台输出，用于调试
    printf("%-13s%-13s%-13s%-13s%-13s%-13s%-13s%-13s%-13s\n"
           ,"Days"
           ,"Health"
           ,"Infected"
           ,"InShelter"
           ,"AtHome"
           ,"NotInShelter"
           ,"Immune"
           ,"Cost"
           ,"LockdownCost");
    consoleOutput(0);

    writeData(0);
    //用循环模拟30天的数据
    for (int day = 1; day < settings.days; ++day) {
        infectionUpdate(day);
        shelterUpdate(day);
        infectRatioUpdate(day);
        consoleOutput(day);
        writeData(day);
    }
    whatIWant();
    system("pause");
    system("python chart.py");
    return 0;
}

//说明文档
void readme(){
    printf("这是一个模拟新冠感染的软件\n"
           "如果你安装了python并正确配置了PATH， 请同时将chart.py与程序放在同一目录下，执行完后会自动生成名为\"chart.pdf\"的图表\n"
           "如果没有，你也可以手动执行chart.py生成\n"
           "你可以更改软件的参数，当然你也可以使用默认参数,默认参数如下：\n"
           "模拟30天，感染率为2.5，在感染人数超过1,000,000时会封城\n"
           "有70%%的感染者是居家隔离，剩下的送至方舱，居家隔离以及封城时的感染率降为0.05\n"
           "同时，感染者在痊愈后10天内不会再次被感染\n"
           "输入d来使用自定义参数，或者输入其他字符来使用默认参数：");
    char option;
    scanf("%c",&option);
    if (option == 'd' || option == 'D'){
        system("cls");
        setting();
    }
    else{
        system("cls");
    }
}

//程序设置
void setting(){
    //自定义模拟天数
    printf("请输入你要模拟的天数：");
    scanf("%d",&settings.days);

    //自定义感染率
    double ration;
    printf("请设置你想要的感染率：");
    scanf("%lf",&ration);
    settings.what_this = ration;
    settings.infectRatio = ration;

    //选择是否会封城
    printf("请选择是否会封城(0-false 1-true)：");
    scanf("%d",&settings.ifLockdown);

    //如果会封城，那么感染到多少人会封城
    if (settings.ifLockdown){
        printf("感染人数为多少时封城：");
        scanf("%lu",&settings.lockdownLimit);
    }

    //选择是否居家隔离
    printf("请选择是否居家隔离(0-false 1-true)：");
    scanf("%d",&settings.ifLockdown);

    if (settings.ifStayAtHome){
        printf("居家隔离的人数比例：");
        scanf("%lf",&settings.ratioStayAtHome);
        printf("居家隔离时的感染率：");
        scanf("%lf",&settings.infectRatioAtHome);
    }

    //是否能再次被感染
    printf("能否被再次感染：");
    scanf("%d",&settings.ifInfectedAgain);
    if (settings.ifInfectedAgain){
        printf("几天后会再次被感染：");
        scanf("%d",&settings.infectedAgainLimit);
    }
}

//控制台输出
void consoleOutput(int day){
    printf("%-13d%-13lu%-13lu%-13lu%-13lu%-13lu%-13lu%-13llu%-13llu\n"
           ,day+1
           ,settings.data[day].health
           ,settings.data[day].infected
           ,settings.data[day].inShelter
           ,settings.data[day].atHome
           ,settings.data[day].notInShelter
           ,settings.data[day].immune
           ,settings.data[day].cost
           ,settings.data[day].lockdownCost
           );
}

//初始化数据
void dataInit(){
    for (int i = 0; i < settings.days; ++i) {
        settings.data[i].health = 0;
        settings.data[i].infected = 0;
        settings.data[i].notInShelter = 0;
        settings.data[i].inShelter = 0;
        settings.data[i].immune = 0;
        settings.data[i].cost = 0;
        settings.data[i].lockdownCost = 0;
        settings.data[i].atHome = 0;
    }
    settings.data[0].health = day0.health - 1;
    settings.data[0].infected = 1;
    settings.data[0].notInShelter = 1;
}

//进行感染的过程
void infectionUpdate(int day){

    //新感染的人数
    unsigned long newInfected = (unsigned long)(settings.data[day-1].notInShelter * settings.infectRatio
            + settings.infectRatioAtHome * settings.data[day-1].atHome);


    //如果新感染的人数超过了剩余健康人数，那么新感染人数就是剩余的健康人数
    if (settings.data[day-1].health < newInfected){
        newInfected = settings.data[day-1].health;
    }

    //将相应数据写入到当天的元素中
    settings.data[day].notInShelter = settings.data[day-1].notInShelter + newInfected;
    settings.data[day].infected = settings.data[day-1].infected + newInfected;
    settings.data[day].health = settings.data[day-1].health - newInfected;

    settings.data[day].immune = settings.data[day - 1].immune;
    settings.data[day].inShelter = settings.data[day-1].inShelter;
    settings.data[day].cost = settings.data[day-1].cost;
    settings.data[day].lockdownCost = settings.data[day-1].lockdownCost;
    settings.data[day].atHome = settings.data[day-1].atHome;

    //判断是否要封城
    if (settings.data[day].infected >= settings.lockdownLimit && settings.ifLockdown){
        settings.lockedDown = 1;
    }

    //判断是否要解除封城
    if (settings.data[day].infected < settings.lockdownLimit && settings.ifLockdown){
        settings.lockedDown = 0;
    }

    //如果封城，那么就在封城开支上加上6,000,000
    if (settings.lockedDown){
        settings.data[day].lockdownCost += 60000000;
    }
}

//收治感染者的过程
void shelterUpdate(int day){
    //新治愈的人数
    unsigned long newCured = 0;
    for (int i = 0; i < 3000; ++i) {
        newCured += shelter[i].day[6];
    }
    newCured += stayAtHome(day);
    //将相应数据写入
    settings.data[day].immune += newCured;
    settings.data[day].infected -= newCured;
    settings.data[day].inShelter -= (newCured - stayAtHome(day));
    settings.data[day].atHome -= stayAtHome(day);

    //方舱中病人的累计天数增长一天
    for (int i = 0; i < 3000; ++i) {
        for (int j = 6; j > 0; --j) {
            shelter[i].day[j] = shelter[i].day[j-1];
        }
        shelter[i].day[0] = 0;
        shelter[i].total = getShelterTotal(i);
    }

    //当天送到方舱的人数
    unsigned long newAtHome = (unsigned long)((double)settings.data[day-1].notInShelter * settings.ratioStayAtHome);
    unsigned long newInShelter = settings.data[day-1].notInShelter - newAtHome;
    settings.data[day].inShelter += newInShelter;
    settings.data[day].notInShelter -= newInShelter;

    settings.data[day].atHome += newAtHome;
    settings.data[day].notInShelter -= newAtHome;


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
    settings.data[day].cost += newCost;

    //让人能再次被感染
    unsigned newInfect = infectAgain(day);

    settings.data[day].immune -= newInfect;
    settings.data[day].health += newInfect;
}

//计算出能再次被感染的人数
unsigned long infectAgain(int day){
    if (!settings.ifInfectedAgain || day < 3+settings.infectedAgainLimit){
        return 0;
    }
    return settings.data[day-settings.infectedAgainLimit].immune
            + infectAgain(day-settings.infectedAgainLimit)
            - settings.data[day-settings.infectedAgainLimit-1].immune;
}

//计算出解除居家隔离的人数
unsigned long stayAtHome(int day){
    if (!settings.ifStayAtHome || day < 8){
        return 0;
    }
    return settings.data[day-7].atHome + stayAtHome(day-7) - settings.data[day-8].atHome;
}

//更新感染率
void infectRatioUpdate(int day){
    //因为默认感染之后无法再次被感染，而已经被感染的也无法被感染，所以感染率会随着健康人数的下降有所下降
    settings.infectRatio = settings.what_this
            * ((double)settings.data[day].health
            / (double)(settings.data[day].health
            + settings.data[day].immune
            + settings.data[day].infected));

    //如果封城的话，就讲感染率下降到居家感染率
    if (settings.lockedDown && settings.ifLockdown){
        settings.infectRatio = settings.infectRatioAtHome;
    }
}

//获取方舱人数
int getShelterTotal(int index){
    //获取一个方舱内的总人数
    int total = 0;
    for (int i = 0; i < 7; ++i) {
        total += shelter[index].day[i];
    }
    return total;
}

//文件输出
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
    fprintf(fp,"%llu",settings.data[day].cost+settings.data[day].lockdownCost);
    fclose(fp);

    //写入infected_day_x.txt
    for (int j = 0; j < 100; ++j) { //清空filename字符串
        filename[j] = 0;
    }
    sprintf(filename,"./output/infected_day_%d.txt",day+1); //通过sprintf来切换日期
    fp = fopen(filename,"w");
    fprintf(fp,"%lu",settings.data[day].infected);
    fclose(fp);

    //写入cost_of_lock_down_population_day_x.txt
    for (int j = 0; j < 100; ++j) { //清空filename字符串
        filename[j] = 0;
    }
    sprintf(filename,"./output/cost_of_lock_down_population_day_%d.txt",day+1); //通过sprintf来切换日期
    fp = fopen(filename,"w");
    fprintf(fp,"%llu",settings.data[day].lockdownCost);
    fclose(fp);
}

void whatIWant(){
    FILE *fp = fopen("./output/I_want_this.csv","w");
    for (int i = 0; i < 29; ++i) {
        fprintf(fp,"%d,",i+1);
    }
    fprintf(fp,"%d\n",30);
    for (int i = 0; i < 29; ++i) {
        fprintf(fp,"%lu,",settings.data[i].infected);
    }
    fprintf(fp,"%lu\n",settings.data[29].infected);
    for (int i = 0; i < 29; ++i) {
        fprintf(fp,"%lu,",settings.data[i].inShelter);
    }
    fprintf(fp,"%lu\n",settings.data[29].inShelter);
    for (int i = 0; i < 29; ++i) {
        fprintf(fp,"%llu,",settings.data[i].cost+settings.data[i].lockdownCost);
    }
    fprintf(fp,"%llu\n",settings.data[29].cost+settings.data[29].lockdownCost);
    fclose(fp);
}