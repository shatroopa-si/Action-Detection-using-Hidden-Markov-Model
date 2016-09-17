/* This is a header file for hmm implementation
	Ajinkya Bhavalkar
        MT2015505

LOGIC: Expectation MAximization algorithm
*/

/* Sample problem in consideration

	V = set of observation symbols = {watching GOT, sleeping, crying, fb}
	S = set of states = {happy, sad}
	
	A = matrix of state transition probabilities
	B = matrix of observation emission probabilities

	alpha = forward algorithm
	beta = backward algorithm

	input = sequence of observation symbols from V = obsSequence[0: T - 1]
	output = model parameters = A, B
*/


//header files
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/core/persistence.hpp>

#include <opencv2/video/background_segm.hpp>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>

using namespace cv;
using namespace std;

//Configuration
#define NO_OF_ACTIONS 2
#define NO_OF_TRAINING_EXAMPLES 5
#define INIT 0
#define TRAIN 0
#define TEST 1

//macros
#define INPUT_LENGTH 41                 // number of observations + 1
#define NUMBER_OF_ITERATIONS 10			// number of iterations in EM loop
#define nS 5                            // number of states
#define nV 18                            // number of observation symbols
//#define TEST_LENGTH 11                  // number of observations in the test sequence + 1
#define SENTINEL 0.000091




//model parameters
extern double init[nS];
extern double A[nS][nS];
extern double B[nS][nV];


//intermediate values
extern double scaleFactor[NO_OF_TRAINING_EXAMPLES][INPUT_LENGTH];
extern double alpha[NO_OF_TRAINING_EXAMPLES][INPUT_LENGTH][nS];
extern double beta[NO_OF_TRAINING_EXAMPLES][INPUT_LENGTH][nS];


//input
extern int obsSequence[NO_OF_TRAINING_EXAMPLES][INPUT_LENGTH];  //observation sequence for training
//extern int testSequence[INPUT_LENGTH];         //observation sequence for testing


//user-defined function prototypes
//defined in intermediate.c
void evalAlpha();
void evalBeta();
void evalTestAlpha();

//defined in hmm_init
void hmm_init(void);
void randomGen1(uint64);
void randomGen2(uint64);

//defined in viterbi.c --> for testing
int findState(int);
void detStateSequence(int []);
