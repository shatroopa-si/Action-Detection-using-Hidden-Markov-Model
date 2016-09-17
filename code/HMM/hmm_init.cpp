/* HMM Initialization */

#include "hmm.h"
double a[nS];
double b[nV];

void hmm_init(void)
{
    double init[nS];                            //initial probability distribution of states (pi)
    double A[nS][nS];                                                    //state transition matrix
    double B[nS][nV];
    double sum;
    
    randomGen1(0xFFFFFFFF);

    for (int i = 0; i < nS; i++ )
    {
        init[i] = a[i];
        
    }
    for(int i=0;i<nS;i++)
    {
        randomGen1(i);
        for (int j = 0; j < nS; j++ )
        {
            A[i][j] = a[j];
        }
    }
    
    for (int i=0;i < nS; i++)
    {
        randomGen2(i);
        for (int j=0;j < nV;j++)
        {
            B[i][j] = b[j];
        }
            
    }
    cv::FileStorage fs("./Models/Hmm_model_init.yml",cv::FileStorage::WRITE);
    fs << "Project - Action Detection using HMM" << "Model Parameters";
    
    fs << "init" << "[:";
    for (int i=0;i < nS; i++)
    {
        fs << init[i];
    }
    fs << "]";
    
    fs << "A" << "[:";
    for(int i = 0; i < nS; i++)
    {
        for(int j = 0; j < nS; j++)
            fs << A[i][j];
        
    }
    
    fs << "]";
    ////
    //model << "\n\n--------------B-----------------\n\n";
    fs << "B" << "[:";
    for(int j = 0; j < nS; j++)
    {
        for(int k = 0; k < nV; k++)
            fs << B[j][k];
        //model << "\n";
    }
    fs << "]";

    fs.release();
    
    
}

void randomGen1(uint64 seed_value)
{
    RNG rng;
    double sum = 0.0;
    for (int i = 0; i < nS; i++ )
    {
        a[i] = rng.uniform(0.,1.);
        //cout << "a = " << a[i] << endl;
        sum = sum + a[i];
    }
    
    for (int i = 0; i < nS; i++)
    {
        a[i] = a[i]/sum;
        //cout << a[i] << endl;
    }

    
}

void randomGen2(uint64 seed_value)
{
    RNG rng;
    double sum = 0.0;
    for (int i = 0; i < nV; i++ )
    {
        b[i] = rng.uniform(0.,1.);
        //cout << "a = " << a[i] << endl;
        sum = sum + b[i];
    }
    
    for (int i = 0; i < nV; i++)
    {
        b[i] = b[i]/sum;
        //cout << a[i] << endl;
    }
    
    
}

