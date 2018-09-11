#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "search.h"

//使用函数指针,美化函数风格
//有一个4个数的flag,4个flag对应4中搜索类型
//下面的search,make,clean分别是对应的搜索类型数据结构的对应函数
//search fucntions pointers array:functions with the same parameters
int (*search[4])(void*,int,int)=
{
    linear_array_search,
    linkedlist_search,
    binary_array_search,
    binary_tree_search,
};

//make functions
void* (*make[4])(int)=
{
    make_sequential_array,
    make_sequential_list,
    make_sequential_array,
    make_sequential_tree,
};

//clean functions
void (*clean[4])(void*)=
{
    free,
    list_free,
    free,
    bst_free,
};

int Str2Int(const char*);//convert string to interger
long Powerof2(const int);//get Powerof2
void SetFlag(char*,int*);//set flag
void PrintTitle(const int *);

int main(int argc, char *argv[])
{
    //参数小于3个就退出,并且报错
    if(argc<4)//number of parameters is less than 3
    {
        exit(1);
    }
    //申请四个flag,初始化为0
    int flag[4]={0,0,0,0};//flag of linear_array,linear_list,binary_array,binary_tree
    long minpow=-1;//接受minpow
    long maxpow=-1;

    //get minpow and maxpow
    /*minpow=Powerof2( Str2Int(argv[1]) );*/
    //argv[1] 是第一个参数,power得到2的次幂.同理
    minpow=Powerof2(Str2Int(argv[1]));
    maxpow=Powerof2( Str2Int(argv[2]) );
    long repeat=Str2Int(argv[3]);

    //set flag
    if(argc==4)//如果没有第四个参数,就输出所有的4种类型
    {
        flag[0]=flag[1]=flag[2]=flag[3]=1;
    }else
    {
        SetFlag(argv[4],flag);//用第四个参数设置flag
    }

    //output
    //输出第一行的文字
    printf("  LENGTH SEARCHES");
    PrintTitle(flag);//根据flag输出第一行
    printf("\n");

    for(long len=minpow;len<=maxpow;len=len*2)//len每次乘2
    {

        printf("%8ld %8ld ",len,2*len);//len是data structure长度,2*len是搜索次数(每次对应一个正确一个错误)

        //genearte test numbers
        long *arr_cor=(long*)malloc(len*sizeof(long));
        long *arr_err=(long*)malloc(len*sizeof(long));
        for(long i=0;i<len/2;i++)//按照老师给的格式生成数组
        {
            arr_cor[i*2]=i;
            arr_cor[i*2+1]=i+len/2;
            arr_err[i*2]=-1-i;
            arr_err[i*2+1]=-1-i-len/2;
        }

        double time[4]={0.0,0.0,0.0,0.0};//四个计时结果

        for(int i=0;i<4;i++)
        {
            if(flag[i]==1)//run
            {
                void* data= make[i](len);//生成数据

                clock_t begin,end;

                for(long j=0;j<len;j++)
                {
                    for(int k=0;k<repeat;k++)
                    {
                        begin=clock();//只对搜索函数进行计时
                        search[i](data,len,arr_cor[j]);//search for the successful queries
                        end=clock();
                        time[i]+=((double)(end-begin)/CLOCKS_PER_SEC);
                    }
                }

                for(long j=0;j<len;j++)
                {
                    for(int k=0;k<repeat;k++)
                    {
                        begin=clock();
                        search[i](data,len,arr_err[j]);//search for the failing queries
                        end=clock();
                        time[i]+=((double)(end-begin)/CLOCKS_PER_SEC);
                    }
                }

                clean[i](data);//free数据
                printf("%11.4e ",time[i]);
            }
        }

        printf("\n");

        //free测试是数据
        free(arr_cor);
        free(arr_err);
    }



    return 0;
}


int Str2Int(const char* p)//把str变成int
{
    int res=0;
    for(int i=0;p[i]!='\0';i++)
    {
        res=res*10+p[i]-'0';
    }
    return res;
}

long Powerof2(const int l)
{
    long res=1;
    for(int i=0;i<l;i++)
    {
        res=res*2;
    }
    return res;
}

void SetFlag(char* str,int * flag)
{
    for(int i=0;str[i]!='\0';i++)
    {
        switch(str[i])
        {
            case 'a':
                flag[0]=1;
                break;

            case 'l':
                flag[1]=1;
                break;

            case 'b':
                flag[2]=1;
                break;

            case 't':
                flag[3]=1;
                break;

        }

    }

}

void PrintTitle(const int * flag)
{
    if(flag[0]) printf("       array");
    if(flag[1]) printf("        list");
    if(flag[2]) printf("      binary");
    if(flag[3]) printf("        tree");
    return ;
}


