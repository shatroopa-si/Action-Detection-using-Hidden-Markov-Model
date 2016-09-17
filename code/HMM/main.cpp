/* This program implements HMM
	Ajinkya Bhavalkar
	    MT2015505
*/

//header files
#include "hmm.h"


//global variables
double init[nS];                                          //initial probability distribution of states (pi)
double A[nS][nS];                                                       //state transition matrix
double B[nS][nV];                                                   //output emission matrix

//inputs
int obsSequence[NO_OF_TRAINING_EXAMPLES][INPUT_LENGTH];
//intermediate variables
double scaleFactor[NO_OF_TRAINING_EXAMPLES][INPUT_LENGTH];
double alpha[NO_OF_TRAINING_EXAMPLES][INPUT_LENGTH][nS];
double beta[NO_OF_TRAINING_EXAMPLES][INPUT_LENGTH][nS];


char *sequencename = new char[50];
char *modelname = new char [50];

//Initial values are stored in HMM_model.yml file.

//main function
int main()
{
	//auxillary variables
	int t;				//time steps
	int c;				//loop counter for convergence of EM	
	int i, j;			//loop counters for states
	int k;				//loop counter for data values
    int trainCount;     //loop counter for NO_OF_TRAINING_EXAMPLES
    int o = 0;          //loop counter Observation Sequences
	double sum_n, sum_d;		//accumulators for numerator and denominator
    int actionCount = 1;        //loop Counter for NO_OF_ACTIONS
    double Prob_final[NO_OF_TRAINING_EXAMPLES][NO_OF_ACTIONS];   //P(O|model parameters) for all actions.
    double sum_result;
    double max;
    int maxi;
    int testCount = 0;
    
    if (INIT)
    {
        hmm_init();
        waitKey(0);
    }
    
    for (actionCount = 1; actionCount <= NO_OF_ACTIONS;actionCount++)
    {
        cout << "ACtion Dataset : " << actionCount << endl;
        trainCount = 1;
        while (trainCount <= NO_OF_TRAINING_EXAMPLES)
        {
            
            sprintf(sequencename,"../Image Processing/Testing Dataset/Action-%i/Observation Sequences/ObservationSequence%i.yml",actionCount,trainCount);
            cv::FileStorage fsobsv(sequencename,cv::FileStorage::READ);
            
            /* Read Input Observation Sequence : */
            cv::FileNode fs0 = fsobsv["Seq"];
            cv::FileNodeIterator it0 = fs0.begin(), it_end0 = fs0.end();
            
            obsSequence[trainCount-1][0] = -1;
            for (int i=1; i < INPUT_LENGTH; i++ )
            {
                obsSequence[trainCount-1][i] = (int)*it0++;
            }
            trainCount ++;
            fsobsv.release();
            
        }
        /*-------------------------------------------------------*/
        
        //Reading previous Values from HMM-MODEL-INITIAL.yml
        if(TRAIN){
            
            cv::FileStorage fsmodelread("./Models/Hmm_model_init.yml",cv::FileStorage::READ);
            
            cv::FileNode fsin = fsmodelread["init"];
            cv::FileNodeIterator itin = fsin.begin(), it_endin = fsin.end();
            
            for (int i = 0;i< nS;i++)
            {
                for(int j=0;j<nS;j++)
                {
                    init[i] = (double)*itin++;
                    
                }
            }
            
            
            
            cv::FileNode fs1 = fsmodelread["A"];
            cv::FileNodeIterator it1 = fs1.begin(), it_end1 = fs1.end();
            
            for (int i = 0;i< nS;i++)
            {
                for(int j=0;j<nS;j++)
                {
                    A[i][j] = (double)*it1++;
                    
                }
            }
            
            
            cv::FileNode fs2 = fsmodelread["B"];
            cv::FileNodeIterator it2 = fs2.begin(), it_end2 = fs2.end();
            
            for (int j = 0;j< nS;j++)
            {
                for(int k=0;k<nV;k++)
                {
                    B[j][k] = (double)*it2++;
                    
                }
            }
            //fsmodelread[A] >> A[0][0];
            
            fsmodelread.release();
            
        }
        

        
        /*-------------------------------------------------------*/
        //TRAIN => determine parameters: A, B
        if(TRAIN){
            for(c = 0; c < NUMBER_OF_ITERATIONS; c++)
            {
                evalAlpha();
                evalBeta();
                
                //You have alpha and beta for all K Observation Sequences.
                
                //calculation of state transition probabilities: A[i][j]
                /*-------------------------------------------------------*/
                for(i = 0; i < nS; i++)
                {
                    for(j = 0; j < nS; j++)
                    {
                        sum_n = sum_d = 0.0;
                        for (o = 0; o < NO_OF_TRAINING_EXAMPLES; o++)
                        {
                            for(t = 1; t < INPUT_LENGTH - 1; t++)
                            {
                                sum_n += (alpha[o][t][i] * A[i][j] * B[j][obsSequence[o][t + 1]] * beta[o][t + 1][j]);
                                sum_d += ((alpha[o][t][i] * beta[o][t][i])/scaleFactor[o][t]);
                            }
                        }
                        A[i][j] = sum_n / sum_d;
                    }
                }
                
                
                /*--------------------end of A-----------------------------------*/
                //calculation of output emission probabilities: B[j][k]
                
                for(j = 0; j < nS; j++)
                {
                    for(k = 0; k < nV; k++)
                    {
                        sum_n = sum_d = 0.0;
                        for (o = 0; o < NO_OF_TRAINING_EXAMPLES; o++)
                        {
                            
                            for(t = 1; t < INPUT_LENGTH; t++)
                            {
                                if(obsSequence[o][t] == k)
                                    sum_n += ((alpha[o][t][j] * beta[o][t][j]) / scaleFactor[o][t]);
                                sum_d += ((alpha[o][t][j] * beta[o][t][j]) / scaleFactor[o][t]);
                            }
                        }
                        B[j][k] = sum_n / sum_d;
                    }
                }
                
                /*----------------------end of B---------------------------------*/
                
            }
            
        }
        
        /*--------------------end of NO_OF_ITERATIONS-----------------------------------*/
        
        /* ------------------ Testing ---------------------------------*/
        
        /* ------------------------------------------------------------ */
        // Reading from HMM-MODEL for Testing.
        if (TEST){
            for (testCount = 0;testCount < NO_OF_ACTIONS; testCount++)
            {
            
                sprintf(modelname,"./Models/Hmm_model%i.yml",testCount + 1);
                cv::FileStorage fsmodelread(modelname,cv::FileStorage::READ);
                
                cv::FileNode fsin = fsmodelread["init"];
                cv::FileNodeIterator itin = fsin.begin(), it_endin = fsin.end();
                
                for (int i = 0;i< nS;i++)
                {
                    for(int j=0;j<nS;j++)
                    {
                        init[i] = (double)*itin++;
                        
                    }
                }
                
                
                
                cv::FileNode fs1 = fsmodelread["A"];
                cv::FileNodeIterator it1 = fs1.begin(), it_end1 = fs1.end();
                
                for (int i = 0;i< nS;i++)
                {
                    for(int j=0;j<nS;j++)
                    {
                        A[i][j] = (double)*it1++;
                        
                    }
                }
                
                
                cv::FileNode fs2 = fsmodelread["B"];
                cv::FileNodeIterator it2 = fs2.begin(), it_end2 = fs2.end();
                
                for (int j = 0;j< nS;j++)
                {
                    for(int k=0;k<nV;k++)
                    {
                        B[j][k] = (double)*it2++;
                        
                    }
                }
                //fsmodelread[A] >> A[0][0];
                
                fsmodelread.release();
                
                
                
                /* ------------------------------------------------------------ */
                
                evalAlpha();
                t = INPUT_LENGTH - 1;
                for (o = 0; o < NO_OF_TRAINING_EXAMPLES; o ++ )
                {
                    sum_result = 0.0;
                    for (i = 0; i < nS; i++ )
                    {
                        sum_result += alpha[o][t][i];
                    }
                    
                    Prob_final[o][testCount] = sum_result/scaleFactor[o][t];
                    
                    //cout << "Prob_final = " << Prob_final[o][testCount];
                }
            
            
            //TEST => determining underlying state sequence for the input
            //detStateSequence(obsSequence);
            
            }
            
            for (o = 0; o < NO_OF_TRAINING_EXAMPLES; o++)
            {
                max = 0.0;
                maxi = 0;
                for (int c = 0; c < NO_OF_ACTIONS; c++)
                {
                    if ( Prob_final[o][c] > max )
                    {
                        max = Prob_final[o][c];
                        maxi = c + 1;
                    }
                    
                }
                if (maxi == 1) cout << "Straight Drive played" << endl;
                if (maxi == 2) cout << "Pull shot played" << endl;
            }
        }
        
        /* -----------------------end of TESTING-------------------------------------- */
        
//        printf("\n\n--------------A-----------------\n\n");
//        for(i = 0; i < nS; i++)
//        {
//            for(j = 0; j < nS; j++)
//                printf("%lf \t", A[i][j]);
//            printf("\n");
//        }
//        
//        printf("\n\n--------------B-----------------\n\n");
//        for(j = 0; j < nS; j++)
//        {
//            for(k = 0; k < nV; k++)
//                printf("%lf \t", B[j][k]);
//            printf("\n");
//        }
        
        //----------------------------------------------------------------------------
        
        if(TRAIN){
            //writing the model onto file
            //fsmodel << "Transition Probabilty matrix";
            sprintf(modelname,"./Models/Hmm_model%i.yml",actionCount);
            cv::FileStorage fsmodel(modelname,cv::FileStorage::WRITE);
            fsmodel << "Project - Action Detection using HMM" << "Model Parameters";
            fsmodel << "A" << "[:";
            for(i = 0; i < nS; i++)
            {
                for(j = 0; j < nS; j++)
                    fsmodel << A[i][j];
                
            }
            
            fsmodel << "]";
            ////
            //model << "\n\n--------------B-----------------\n\n";
            fsmodel << "B" << "[:";
            for(j = 0; j < nS; j++)
            {
                for(k = 0; k < nV; k++)
                    fsmodel << B[j][k];
                //model << "\n";
            }
            fsmodel << "]";
            
            
            fsmodel.release();
            
        }
        
    }
	return 0;
}

/*

	printf("\n\n----PASS : %d-------\n\n", c + 1);

 for (int p=0; p < NO_OF_TRAINING_EXAMPLES; p++)
 {
 printf("\n\n--------------alpha[%d]-----------------\n\n",p);
 for(int s = 1; s < INPUT_LENGTH; s++)
 {
 for(int d = 0; d < nS; d++)
 printf("%lf \t", alpha[p][s][d]);
 printf("\n");
 }
 printf ("\n \n");
 }
 
 for (int p = 0; p < NO_OF_TRAINING_EXAMPLES; p++)
 {
 printf("\n\n--------------beta[%d]-----------------\n\n",p);
 for(int s = 1; s < INPUT_LENGTH; s++)
 {
 for(int d = 0; d < nS; d++)
 printf("%lf \t", beta[p][s][d]);
 printf("\n");
 }
 printf("\n \n");
 }

	printf("\n\n--------------A-----------------\n\n");
	for(i = 0; i < nS; i++)
	{
		for(j = 0; j < nS; j++)
			printf("%lf \t", A[i][j]);
		printf("\n");
	}

	printf("\n\n--------------B-----------------\n\n");
	for(j = 0; j < nS; j++)
	{	
		for(k = 0; k < nV; k++)
			printf("%lf \t", B[j][k]);
		printf("\n");
	}

*/

