/**********************************************
 *@file	    BurgerBuddies.c
 *@brief    信号量练习
 *@author   刘琎 516020910128
 *@date     2019/5/3
***********************************************/
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define COOKS 2   // 厨师数量
#define BUFFS 5 // 缓冲数目
#define CASHIERS 2 //收银员数量
#define CUSTOMERS 4 //顾客数量

int in = 0;            // 厨师放置产品的位置
int out = 0;           // 收银员取产品的位置
int buff[BUFFS] = {0}; // 缓冲初始化为0， 开始时没有产品

/** 信号量 */
sem_t empty_sem; // 同步信号量， 当满了时阻止厨师放汉堡
sem_t full_sem;   // 同步信号量， 当没产品时阻止收银员取汉堡
sem_t order_sem;  // 同步信号量，当没有订单时阻止收银员工作
sem_t cash_sem;  // 同步信号量，当没有收银员时阻止顾客下订单
pthread_mutex_t mutex; // 互斥信号量， 一次只有一个线程访问缓冲
pthread_mutex_t print_mutex; // 互斥信号量，一次只有一个线程在输出

/**@brief 打印货架情况 */
void print() {
    int i;
    for (i = 0; i < BUFFS; i++)
        printf("%d ", buff[i]);
    printf("\n");
}

/**@brief 厨师线程 */
void *cook(void *idptr) {
    int id = *(int *) idptr; // 线程编号

    while (1) {
        // 用sleep可以调节生产和消费的速度，便于观察
        sleep(2);

        sem_wait(&empty_sem);
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&print_mutex);
        // 打印信息
        in = in % BUFFS;
        printf("厨师%d 放汉堡到货架%d，货架情况: \t", id, in);
        buff[in] = 1;
        print();
        ++in;

        pthread_mutex_unlock(&print_mutex);
        pthread_mutex_unlock(&mutex);
        sem_post(&full_sem);
    }
}

/**@brief 收银员线程 */
void *cashier(void *idptr) {
    int id = *(int *) idptr; // 线程编号
    while (1) {
        // 用sleep的数量可以调节生产和消费的速度，便于观察
        sleep(2);

        sem_wait(&order_sem);
        sem_wait(&full_sem);
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&print_mutex);

        //打印信息
        out = out % BUFFS;
        printf("收银%d 取汉堡从货架%d，货架情况: \t", id, out);
        buff[out] = 0;
        print();
        ++out;

        pthread_mutex_unlock(&print_mutex);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty_sem);
        sem_post(&cash_sem);
    }
}

/**@brief 顾客线程方法 */
void *customer(void *idptr) {
    int id = *(int *) idptr; // 线程编号
    while (1.5) {
        // 用sleep的数量可以调节生产和消费的速度，便于观察
        sleep(1);
        sem_wait(&cash_sem);

        //打印信息
        pthread_mutex_lock(&print_mutex);
        printf("顾客%d 下单\n", id);
        pthread_mutex_unlock(&print_mutex);

        sem_post(&order_sem);
    }
}

/**@breif 主程序*/
int main() {
    // 线程id
    pthread_t cook_id[COOKS];
    pthread_t cash_id[CASHIERS];
    pthread_t cust_id[CUSTOMERS];
    int i;
    int tmp;

    // 初始化同步信号量
    int ini1 = sem_init(&empty_sem, 0, BUFFS);
    int ini2 = sem_init(&full_sem, 0, 0);
    int ini3 = sem_init(&order_sem, 0, 0);
    int ini4 = sem_init(&cash_sem, 0, CASHIERS);
    if (ini1 && ini2 && ini3 && ini4 != 0) {
        printf("sem init failed \n");
        exit(1);
    }
    //初始化互斥信号量
    int ini5 = pthread_mutex_init(&mutex, NULL);
    int ini6 = pthread_mutex_init(&print_mutex, NULL);
    if (ini5 && ini6 != 0) {
        printf("mutex init failed \n");
        exit(1);
    }
    // 创建厨师线程
    for (i = 0; i < COOKS; i++) {
        tmp = pthread_create(&cook_id[i], NULL, cook, (void *) (&i));
        if (tmp != 0) {
            printf("cook%d creation failed \n", i);
            exit(1);
        }
    }
    //创建收银员线程
    for (i = 0; i < CASHIERS; i++) {
        tmp = pthread_create(&cash_id[i], NULL, cashier, (void *) (&i));
        if (tmp != 0) {
            printf("cashier%d creation failed \n", i);
            exit(1);
        }
    }

    //创建顾客线程
    for (i = 0; i < CUSTOMERS; i++) {
        tmp = pthread_create(&cust_id[i], NULL, customer, (void *) (&i));
        if (tmp != 0) {
            printf("customer%d creation failed \n", i);
            exit(1);
        }
    }
    //销毁线程
    for (i = 0; i < COOKS; i++)
        pthread_join(cook_id[i], NULL);
    for (i = 0; i < CASHIERS; i++)
        pthread_join(cash_id[i], NULL);
    for (i = 0; i < CUSTOMERS; i++)
        pthread_join(cust_id[i], NULL);

    exit(0);
}