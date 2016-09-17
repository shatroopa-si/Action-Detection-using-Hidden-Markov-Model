/* This program finds an underlying state sequence for a given input
	Ajinkya Bhavalkar
        MT2015505
 */

//header files
#include "hmm.h"



int findState(int timeStep)
{
    //Determines the state of the system for a given input at a given time

    //auxillary variables
    int i;                      //loop counter for states
    double max_alpha = -1;          //maximum value of alpha
    
    int state;                  //output state
    
    for(i = 0; i < nS; i++)
        if(alpha[timeStep][i] >= max_alpha)
        {
            max_alpha = alpha[timeStep][i];
            //printf("alpha[timeStep][i]: %lf", alpha[timeStep][i]);
            state = i;
        }
    //printf("%d ", state);
    return state;
}


void detStateSequence(int sequence[])
{
    //Determines the underlying state sequence for a ginven input
    //Logic: Viterbi algorithm
    
    //auxillary variables
    int t;                  //time step
    
    //output
    int state;              //corresponding state
    
    printf("\n\n");
    for(t = 1; t < INPUT_LENGTH; t++)
    {
        state = findState(t);
        printf("%d ", state);
    }
}
