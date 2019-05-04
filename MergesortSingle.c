/**********************************************
 *@file	    MergesortSingle.c
 *@brief     单线程归并排序，命令行传入要排序的数字（整型）
 *           输出从大到小排序结果，例如：
 *           “$ ./MergesortSingle 2 34 5 23”
 *@author   刘琎 516020910128
 *@date       2019/5/3
***********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Length 1000    //数组长度
#define DEBUG 0

/**
*@brief    归并函数
*@param arr: 原数组
*@param start/end/mid: 要归并部分的开头/结尾/中间索引
*/
void merge(int arr[], int start, int end, int mid);


/**
*@brief    归并排序函数
*@param arr: 原数组
*@param start/end: 要归并部分的开头/结尾索引
*/
void merge_sort(int arr[], int start, int end);


/**
*@brief    主程序
*/
int main(int argc, char **argv) {
/**从命令行读入待排序数组    
//    //检查输入
//    if(argc==1){
//        printf("未输入要排序的数字(整型，用空格隔开)，请重试。\n");
//        return 1;
//    }
//    int i=0;
//    int length = argc-1; //数字个数
//
//    //将字符参数转换为数字
//    int array[length];
//    for(i=0;i<length;++i)
//    {
//        array[i] = atoi(argv[i+1]);
//    }
***********************/
    printf("待排序数组长度：%d\n", Length);

    /**随机生成待排序数组*/
    int i = 0;
    int length = Length;
    int array[length];
    srand((int) time(NULL));
    for (i = 0; i < length; ++i) {
        array[i] = rand() % 100;
    }

    //获取时间
    int begintime=clock();	//计时开始

    //归并排序
    merge_sort(array, 0, length - 1);

    //计算所用时间
    int endtime = clock();
    printf("运行时间为：%dms\n", (endtime-begintime)/1000 );

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

void merge(int arr[], int start, int end, int mid) {
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

void merge_sort(int arr[], int start, int end) {
    int mid;
    if (start >= end) return;   //若只有一个元素则返回
    else {
        mid = (start + end) / 2;
        merge_sort(arr, start, mid);
        merge_sort(arr, mid + 1, end);
        merge(arr, start, end, mid);  //归并
    }
    return;
}
