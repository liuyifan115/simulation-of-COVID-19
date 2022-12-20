#include "stdio.h"
#include "stdlib.h"

//���Ƕ���ķ��յ��������ͣ����˻��������7�죬
typedef struct shelter{
    int day[7]; //day���±�ͱ���������������˶�����
    int total; //������һ�����������������
}SHELTER;

//����һ������˸������ݵ���������
typedef struct {
    unsigned long health; //���ж�����δ��Ⱦ
    unsigned long infected; //�Ѿ��ж����˸�Ⱦ��
    unsigned long immune; //�Ѿ��ж�����������
    unsigned long inShelter; //�ڷ������������
    unsigned long atHome; //�ӼҸ��������
    unsigned long notInShelter; //δ�ڷ������浫���Ѿ���Ⱦ������
    unsigned long long cost; //���յĻ���
    unsigned long long lockdownCost; //��ǵĻ���
}DATA;

//�����������
struct {
    int days;//ģ�������

    double what_this; //����ʲô��
    double infectRatio; //��Ⱦ�ʣ�����˵һ����һ���ܴ��������ˣ���ô��Ⱦ�ʾ���3

    int ifLockdown; //�Ƿ����
    unsigned long lockdownLimit; //��Ⱦ�������˾ͻ���
    int lockedDown; //�Ƿ��Ѿ����

    int ifStayAtHome;//�Ƿ���ԾӼҸ���
    double ratioStayAtHome;//���ٱ������˿��ԾӼҸ���
    double infectRatioAtHome;//�ھӼҵ�ʱ��ĸ�Ⱦ��

    int ifInfectedAgain;//�Ƿ���ٴα���Ⱦ
    int infectedAgainLimit;//���������ٴα���Ⱦ

    DATA *data;//���ݴ�ŵĵط�
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


SHELTER shelter[3000] = {};//һ����3,000,000�ˣ�һ��������ס1000�ˣ�����3000������
DATA day0 = {3000000, 0, 0, 0, 0,0,0};//���ǳ�ʼ������

//����Ǹ��ֺ���
void readme();//˵��
void setting();//��������
void consoleOutput(int );//����̨���
void dataInit();//���ݳ�ʼ������
void infectionUpdate(int );//��Ⱦ����
void shelterUpdate(int );//�ͷ��������йصĺ���
unsigned long infectAgain(int );//�����ٴα���Ⱦ
unsigned long stayAtHome(int day);//���������ӼҸ��������
void infectRatioUpdate(int );//���¸�Ⱦ�ʵĺ���
int getShelterTotal(int);//��ȡһ���������ж����˵ĺ���
void writeData(int );//д�ļ�����
void whatIWant();//������ʲô��

int main(){
    readme();

    //����Ŀ¼���������ļ�
    system("mkdir output");

    settings.data = (DATA *)malloc(settings.days* sizeof(DATA));

    //��ʼ������
    dataInit();


    //����̨��������ڵ���
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
    //��ѭ��ģ��30�������
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

//˵���ĵ�
void readme(){
    printf("����һ��ģ���¹ڸ�Ⱦ�����\n"
           "����㰲װ��python����ȷ������PATH�� ��ͬʱ��chart.py��������ͬһĿ¼�£�ִ�������Զ�������Ϊ\"chart.pdf\"��ͼ��\n"
           "���û�У���Ҳ�����ֶ�ִ��chart.py����\n"
           "����Ը�������Ĳ�������Ȼ��Ҳ����ʹ��Ĭ�ϲ���,Ĭ�ϲ������£�\n"
           "ģ��30�죬��Ⱦ��Ϊ2.5���ڸ�Ⱦ��������1,000,000ʱ����\n"
           "��70%%�ĸ�Ⱦ���ǾӼҸ��룬ʣ�µ��������գ��ӼҸ����Լ����ʱ�ĸ�Ⱦ�ʽ�Ϊ0.05\n"
           "ͬʱ����Ⱦ����Ȭ����10���ڲ����ٴα���Ⱦ\n"
           "����d��ʹ���Զ���������������������ַ���ʹ��Ĭ�ϲ�����");
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

//��������
void setting(){
    //�Զ���ģ������
    printf("��������Ҫģ���������");
    scanf("%d",&settings.days);

    //�Զ����Ⱦ��
    double ration;
    printf("����������Ҫ�ĸ�Ⱦ�ʣ�");
    scanf("%lf",&ration);
    settings.what_this = ration;
    settings.infectRatio = ration;

    //ѡ���Ƿ����
    printf("��ѡ���Ƿ����(0-false 1-true)��");
    scanf("%d",&settings.ifLockdown);

    //������ǣ���ô��Ⱦ�������˻���
    if (settings.ifLockdown){
        printf("��Ⱦ����Ϊ����ʱ��ǣ�");
        scanf("%lu",&settings.lockdownLimit);
    }

    //ѡ���Ƿ�ӼҸ���
    printf("��ѡ���Ƿ�ӼҸ���(0-false 1-true)��");
    scanf("%d",&settings.ifLockdown);

    if (settings.ifStayAtHome){
        printf("�ӼҸ��������������");
        scanf("%lf",&settings.ratioStayAtHome);
        printf("�ӼҸ���ʱ�ĸ�Ⱦ�ʣ�");
        scanf("%lf",&settings.infectRatioAtHome);
    }

    //�Ƿ����ٴα���Ⱦ
    printf("�ܷ��ٴθ�Ⱦ��");
    scanf("%d",&settings.ifInfectedAgain);
    if (settings.ifInfectedAgain){
        printf("�������ٴα���Ⱦ��");
        scanf("%d",&settings.infectedAgainLimit);
    }
}

//����̨���
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

//��ʼ������
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

//���и�Ⱦ�Ĺ���
void infectionUpdate(int day){

    //�¸�Ⱦ������
    unsigned long newInfected = (unsigned long)(settings.data[day-1].notInShelter * settings.infectRatio
            + settings.infectRatioAtHome * settings.data[day-1].atHome);


    //����¸�Ⱦ������������ʣ�ཡ����������ô�¸�Ⱦ��������ʣ��Ľ�������
    if (settings.data[day-1].health < newInfected){
        newInfected = settings.data[day-1].health;
    }

    //����Ӧ����д�뵽�����Ԫ����
    settings.data[day].notInShelter = settings.data[day-1].notInShelter + newInfected;
    settings.data[day].infected = settings.data[day-1].infected + newInfected;
    settings.data[day].health = settings.data[day-1].health - newInfected;

    settings.data[day].immune = settings.data[day - 1].immune;
    settings.data[day].inShelter = settings.data[day-1].inShelter;
    settings.data[day].cost = settings.data[day-1].cost;
    settings.data[day].lockdownCost = settings.data[day-1].lockdownCost;
    settings.data[day].atHome = settings.data[day-1].atHome;

    //�ж��Ƿ�Ҫ���
    if (settings.data[day].infected >= settings.lockdownLimit && settings.ifLockdown){
        settings.lockedDown = 1;
    }

    //�ж��Ƿ�Ҫ������
    if (settings.data[day].infected < settings.lockdownLimit && settings.ifLockdown){
        settings.lockedDown = 0;
    }

    //�����ǣ���ô���ڷ�ǿ�֧�ϼ���6,000,000
    if (settings.lockedDown){
        settings.data[day].lockdownCost += 60000000;
    }
}

//���θ�Ⱦ�ߵĹ���
void shelterUpdate(int day){
    //������������
    unsigned long newCured = 0;
    for (int i = 0; i < 3000; ++i) {
        newCured += shelter[i].day[6];
    }
    newCured += stayAtHome(day);
    //����Ӧ����д��
    settings.data[day].immune += newCured;
    settings.data[day].infected -= newCured;
    settings.data[day].inShelter -= (newCured - stayAtHome(day));
    settings.data[day].atHome -= stayAtHome(day);

    //�����в��˵��ۼ���������һ��
    for (int i = 0; i < 3000; ++i) {
        for (int j = 6; j > 0; --j) {
            shelter[i].day[j] = shelter[i].day[j-1];
        }
        shelter[i].day[0] = 0;
        shelter[i].total = getShelterTotal(i);
    }

    //�����͵����յ�����
    unsigned long newAtHome = (unsigned long)((double)settings.data[day-1].notInShelter * settings.ratioStayAtHome);
    unsigned long newInShelter = settings.data[day-1].notInShelter - newAtHome;
    settings.data[day].inShelter += newInShelter;
    settings.data[day].notInShelter -= newInShelter;

    settings.data[day].atHome += newAtHome;
    settings.data[day].notInShelter -= newAtHome;


    //�ӵ�һ�����տ�ʼ���θ�Ⱦ��
    for (int i = 0; i < 3000; ++i) {
        //���û����Ҫ��ס�ģ����˳�ѭ��
        if (newInShelter <= 0){
            break;
        }
        //�����ǰ����ʣ���λС����Ҫ��ס�����������򽫷���ס��
        if (newInShelter > 1000 - shelter[i].total){
            shelter[i].day[0] = 1000 - shelter[i].total;
            newInShelter -= shelter[i].day[0];
            shelter[i].total = 1000;
        }
        //����ֱ��ȫ��ס��ȥ
        else{
            shelter[i].day[0] = (int)newInShelter;
            shelter[i].total += (int)newInShelter;
            newInShelter = 0;
        }
    }

    //���㷽�յĻ���
    unsigned long long newCost = 0;
    for (int i = 0; i < 3000; ++i) {
        newCost += shelter[i].total*200;
    }
    settings.data[day].cost += newCost;

    //�������ٴα���Ⱦ
    unsigned newInfect = infectAgain(day);

    settings.data[day].immune -= newInfect;
    settings.data[day].health += newInfect;
}

//��������ٴα���Ⱦ������
unsigned long infectAgain(int day){
    if (!settings.ifInfectedAgain || day < 3+settings.infectedAgainLimit){
        return 0;
    }
    return settings.data[day-settings.infectedAgainLimit].immune
            + infectAgain(day-settings.infectedAgainLimit)
            - settings.data[day-settings.infectedAgainLimit-1].immune;
}

//���������ӼҸ��������
unsigned long stayAtHome(int day){
    if (!settings.ifStayAtHome || day < 8){
        return 0;
    }
    return settings.data[day-7].atHome + stayAtHome(day-7) - settings.data[day-8].atHome;
}

//���¸�Ⱦ��
void infectRatioUpdate(int day){
    //��ΪĬ�ϸ�Ⱦ֮���޷��ٴα���Ⱦ�����Ѿ�����Ⱦ��Ҳ�޷�����Ⱦ�����Ը�Ⱦ�ʻ����Ž����������½������½�
    settings.infectRatio = settings.what_this
            * ((double)settings.data[day].health
            / (double)(settings.data[day].health
            + settings.data[day].immune
            + settings.data[day].infected));

    //�����ǵĻ����ͽ���Ⱦ���½����ӼҸ�Ⱦ��
    if (settings.lockedDown && settings.ifLockdown){
        settings.infectRatio = settings.infectRatioAtHome;
    }
}

//��ȡ��������
int getShelterTotal(int index){
    //��ȡһ�������ڵ�������
    int total = 0;
    for (int i = 0; i < 7; ++i) {
        total += shelter[index].day[i];
    }
    return total;
}

//�ļ����
void writeData(int day){
    char filename[100] ={};//����ļ������ַ���

    //д��shelter_day_x.txt
    for (int j = 0; j < 100; ++j) { //���filename�ַ���
        filename[j] = 0;
    }
    sprintf(filename,"./output/shelter_day_%d.txt",day+1); //ͨ��sprintf���л�����
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


    //д��total_cost_day_x.txt
    for (int j = 0; j < 100; ++j) { //���filename�ַ���
        filename[j] = 0;
    }
    sprintf(filename,"./output/total_cost_day_%d.txt",day+1); //ͨ��sprintf���л�����
    fp = fopen(filename,"w");
    fprintf(fp,"%llu",settings.data[day].cost+settings.data[day].lockdownCost);
    fclose(fp);

    //д��infected_day_x.txt
    for (int j = 0; j < 100; ++j) { //���filename�ַ���
        filename[j] = 0;
    }
    sprintf(filename,"./output/infected_day_%d.txt",day+1); //ͨ��sprintf���л�����
    fp = fopen(filename,"w");
    fprintf(fp,"%lu",settings.data[day].infected);
    fclose(fp);

    //д��cost_of_lock_down_population_day_x.txt
    for (int j = 0; j < 100; ++j) { //���filename�ַ���
        filename[j] = 0;
    }
    sprintf(filename,"./output/cost_of_lock_down_population_day_%d.txt",day+1); //ͨ��sprintf���л�����
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