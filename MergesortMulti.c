/**********************************************
 *@file	    MergesortMulti.c
 *@brief   多线程归并排序，随机生成数字，进行排序（整型）
 *@author   刘琎 516020910128
 *@date       2019/5/3
***********************************************/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG 0
#define Length 1000    //数组长度
#define MaxThread  15//最大线程数

int arr[Length + 1];  //数组
int NumofThread = 0;    //线程数
int flag = 0;           //达到线程数限制标志位

/**
*@brief    归并函数
*@param start/end/mid: 要归并部分的开头/结尾/中间索引
*/
void merge(int start, int end, int mid);


/**
*@brief    归并排序函数
*@param arg: 要归并部分的开头/结尾索引组成的数组
*/
void *merge_sort(void *arg);


/**
*@brief    随机生成数组
*/
void createData() {
    srand((int) time(NULL));
    for (int i = 0; i < Length; ++i) {
        arr[i] = rand() % 100;
    }
}


/**
*@brief    主程序
*/
int main() {
    printf("待排序数组长度：%d\n", Length);

    //生成随机数
    createData();

    //获取时间
    int begintime=clock();	//计时开始

    //将start和end组合在数组里
    int arg[2];
    arg[0] = 0;
    arg[1] = Length;

    //创建线程，调用归并排序函数
    pthread_t tid;
    NumofThread = 1;
    pthread_create(&tid, NULL, merge_sort, arg);
    pthread_join(tid, NULL);  //等待进程结束


    //计算所用时间
    int endtime = clock();
    printf("运行时间为：%dms\n", (endtime-begintime)/1000 );

    //输出最大线程数
    if (flag == 1) {
        printf("最大线程数 : %d\n", MaxThread);
    }

#if DEBUG == 1
    //输出排序结果
    printf("归并排序结果： \n");
    for (i = 0; i < Length; ++i) {
        printf("%d\t", array[i]);
        if ((i + 1) % 20 == 0)printf("\n");
    }
#endif
    return 0;
}

//归并函数
void merge(int start, int end, int mid) {
    int length = end - start + 1;   // 数组长度
    int tmp[length];                // 保存归并好的数组
    int i = 0, idx1 = start, idx2 = mid + 1;  //两个数组的头
    //两个数组都没空
    while (idx1 <= mid && idx2 <= end) {
        if (arr[idx1] >= arr[idx2]) {
            tmp[i++] = arr[idx1++];
        } else {
            tmp[i++] = arr[idx2++];
        }
    }
    //其中一个数组空了
    if (idx1 <= mid) {
        for (; idx1 <= mid; ++idx1) {
            tmp[i++] = arr[idx1];
        }
    } else {
        for (; idx2 <= end; ++idx2) {
            tmp[i++] = arr[idx2];
        }
    }
    //将tmp复制到arr中

    memcpy(arr + start, tmp, sizeof(int) * length);

    return;
}

//归并排序
void *merge_sort(void *arg) {
    //处理传入参数
    int *argi = (int *) arg; //转换为int指针
    int start = argi[0];
    int end = argi[1]; //传过来的数组两个元素是start和end
    if (start >= end) return NULL;   //若只有一个元素则返回
    int mid = (start + end) / 2; //中间位置

    //拆分成两个数组的首尾坐标
    int arg1[2];
    int arg2[2];
    arg1[0] = start;
    arg1[1] = mid;
    arg2[0] = mid + 1;
    arg2[1] = end;

    //创建两个线程，分别归并排序一个数组
    pthread_t t2 = 0;
    pthread_t t1 = 0;

    if (NumofThread == MaxThread) { //如果线程数达到最大
        flag = 1;
    }
    //排序左半边
    if (NumofThread < MaxThread) { //如果没达到最大
        NumofThread += 1;  //加一个线程
        pthread_create(&t1, NULL, merge_sort, arg1);//创建进程

    } else {
        merge_sort(arg1);
    }
    //排序右半边
    if (NumofThread < MaxThread) {
        NumofThread += 1;
        pthread_create(&t2, NULL, merge_sort, arg2);
    } else {
        merge_sort(arg2);
    }
    //等待线程结束
    if (t1) {
        pthread_join(t1, NULL);
        --NumofThread;
        t1 = 0;
    }
    if (t2) {
        pthread_join(t2, NULL);
        --NumofThread;
        t1 = 0;
    }
    //归并
    merge(start, end, mid);

}
