/*
 *  A Branch Predictor Simulator by Blake Oakley
 *
 * to compile: gcc sim_gshare.c -o sim_gshare.exe
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
int main(int argc, char *argv[])
{
	int m = atoi(argv[1]); //# of PC bits used (M)
	int n = atoi(argv[2]); //# of Global BHR bits used (N)
	FILE *trace_file = fopen(argv[3], "r"); // <HexBranch><single_space><single_char>
	char buff[100]; // a buffer to hold a line of data from the trace file
    int table_size = pow(2,m); // gives table size
    int *the_table = (int *)calloc(table_size, sizeof(int));//the table
    unsigned long long addr; //the address
    char *branch; //string of branch taken or not taken
    char actual; //'t' or 'n' to be compared against prediction
    unsigned long long bhr = 0; //the global branch histroy register
    unsigned long long index; //location in the table
    int hits = 0; //accurate predictions
    int misses = 0; //inaccurate predictions
    unsigned mask; //used for bit shifting and placement
    //initialize table predictions to 2
    for(int i=0; i<table_size;i++)
    {
        the_table[i] = 2;
    }    
    while (!feof(trace_file)) 
	{
		fgets(buff, sizeof(buff), trace_file); //Reads line from trace
		addr = strtoull(&buff[0], &branch, 16); //Gets Address and Branch
        actual = branch[1];//assigns actual t or n        
        addr >>= 2;//removes offset
        mask = (1 << m) - 1;//gets M bits from addr
        addr &= mask;        
        mask = bhr << (m-n);//creates global BHR ++ M-N zeroes        
        //index determined based on n either equal to 0, or greater than 0
        if (n>0)
        {
            index = mask ^ addr;//XOR over M bits of the addr
        }
        if (n==0)
        {
            index = addr;
        }
        //The branch is actually TAKEN:
        if (actual == 't')
        {
            //increment prediction counter
            if(the_table[index] == 0 || the_table[index] == 1)
            {
               misses++;
            }
            else
            {
                hits++;
            }
            //increment predition value in table
            if (the_table[index] != 3)
            {
                the_table[index]++;
            }
            //shift 1 into BHR
            bhr >>= 1;
            mask = (1 << (n-1));
            bhr |= mask;
        }
        //The branch is actually NOT TAKEN:
        if (actual == 'n')
        {
            //increment prediction counter
            if(the_table[index] == 0 || the_table[index] == 1)
            {
               hits++;
            }
            else
            {
                misses++;
            }
            //decrement predition value in table
            if (the_table[index] != 0)
            {
                the_table[index]--;
            }            
            bhr >>= 1;//shift 0 into BHR
        }
    }
    printf("\nM: %d\nN: %d\n", m, n);
    printf("Misprediction Ratio: %1.6f\n", ((double) misses/(double)(misses+hits)));
    return 0;
}
/*
Validation Runs:

1- Configuration A: M=8, N=5

MCF: misprediction rate =7.48%

GoBMK: misprediction rate =0.51%

 

2- Configuration B: M=4, N=2

MCF: misprediction rate =26.86%

GoBMK: misprediction rate=0.87%

 

3- Configuration C: M=3, N=1

MCF: misprediction rate=29.07%

GoBMK: misprediction rate=0.88%
*/