/* This program calculates some intermediate values required in HMM
	Ajinkya Bhavalkar
        MT2015505
*/

//header files
#include "hmm.h"


void evalAlpha()
{
	//evaluates a_i(t) = total probability of all observations up through time t & we are in state s_i at time t
	
	//iterative logic: forward algorithm

	//auxillary variables
	int i, j;					//loop counter for states (transition: i --> j)
	int t;//time step
    int o = 0;
    double sum_c, sum;					//accumulator

	//base case: t = 1
	t = 1;

    for (o = 0; o < NO_OF_TRAINING_EXAMPLES; o++)
    {
        sum_c = 0.0;
        for(i = 0; i < nS; i++)
        {
            alpha[o][t][i] = init[i] * B[i][obsSequence[o][t]];
            if(alpha[o][t][i] <= 0.0000002)
                alpha[o][t][i] = SENTINEL;
            sum_c += alpha[o][t][i];
        }
       
        //need to scale
        scaleFactor[o][t] = 1.0 / sum_c;

    }
    
    
    for (o = 0; o < NO_OF_TRAINING_EXAMPLES;o++)
    {
        for(i = 0; i < nS; i++)
            alpha[o][t][i] *= scaleFactor[o][t];
    }

	//iterative step
    for (o = 0; o < NO_OF_TRAINING_EXAMPLES; o++)
    {
        for(t = 1; t < INPUT_LENGTH - 1; t++)
        {
            sum_c = 0.0;
            for(j = 0; j < nS; j++)
            {
                sum = 0.0;
                for(i = 0; i < nS; i++)
                    sum += (alpha[o][t][i] * A[i][j]);
                alpha[o][t + 1][j] = sum * B[j][obsSequence[o][t + 1]];
                if(alpha[o][t + 1][j] <= 0)
                    alpha[o][t + 1][j] = SENTINEL;
                
                sum_c += alpha[o][t + 1][j];
            }
            //need to scale
            scaleFactor[o][t + 1] = 1.0 / sum_c;
            for(j = 0; j < nS; j++)
                alpha[o][t + 1][j] *= scaleFactor[o][t + 1];
        }
    }
}

void evalBeta()
{
	//evaluates b_i(t) = total probability of all observations from time t & we are in state s_i at time t
	
	//iterative logic: backward algorithm


	//auxillary variables
	int i, j;			//loop counters for states (transition: i --> j)
	int t;				//time step
	double sum; 			//accumulator
    int o = 0;
    
	//base case: t = INPUT_LENGTH - 1
	t = INPUT_LENGTH - 1;
    for (o = 0; o < NO_OF_TRAINING_EXAMPLES; o++)
    {
        for(i = 0; i < nS; i++)
        {
            beta[o][t][i] = 1;
            beta[o][t][i] *= scaleFactor[o][t];
        }
    }
	//iterative step
    for (o = 0; o < NO_OF_TRAINING_EXAMPLES; o++)
    {
        for(t = INPUT_LENGTH - 2; t > 0; t--)
        {
            for(i = 0; i < nS; i++)
            {
                sum = 0.0;
                for(j = 0; j < nS; j++)
                    sum += (A[i][j] * B[j][obsSequence[o][t + 1]] * beta[o][t + 1][j]);
                beta[o][t][i] = sum;
                beta[o][t][i] *= scaleFactor[o][t];
            }
        }
    }
}

void evalTestAlpha()
{
    int i, j;					//loop counter for states (transition: i --> j)
    int t;//time step
    int o = 0;
    double sum_c, sum;					//accumulator
    
    //base case: t = 1
    t = 1;
    
    for (o = 0; o < NO_OF_TRAINING_EXAMPLES; o++)
    {
        sum_c = 0.0;
        for(i = 0; i < nS; i++)
        {
            alpha[o][t][i] = init[i] * B[i][obsSequence[o][t]];
            if(alpha[o][t][i] <= 0.0000002)
                alpha[o][t][i] = SENTINEL;
            sum_c += alpha[o][t][i];
        }
        
    }
    
    
    
    //iterative step
    for (o = 0; o < NO_OF_TRAINING_EXAMPLES; o++)
    {
        for(t = 1; t < INPUT_LENGTH - 1; t++)
        {
            sum_c = 0.0;
            for(j = 0; j < nS; j++)
            {
                sum = 0.0;
                for(i = 0; i < nS; i++)
                    sum += (alpha[o][t][i] * A[i][j]);
                alpha[o][t + 1][j] = sum * B[j][obsSequence[o][t + 1]];
                if(alpha[o][t + 1][j] <= 0)
                    alpha[o][t + 1][j] = SENTINEL;
                
                sum_c += alpha[o][t + 1][j];
            }
        }
    }

    //print
    for (int p=0; p < NO_OF_TRAINING_EXAMPLES; p++)
    {
        printf("\n\n--------------alpha[%d]-----------------\n\n",p);
        for(int s = 0; s < INPUT_LENGTH; s++)
        {
            for(int d = 0; d < nS; d++)
                printf("%lf \t", alpha[p][s][d]);
            printf("\n");
        }
        printf ("\n \n");
    }
 
    
    
}


