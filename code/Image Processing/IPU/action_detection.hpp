// API header file 

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>

#include <opencv2/video/background_segm.hpp>
#include <iostream>
#include <stdlib.h>

/* Configuration */
#define TRAIN 1
#define TEST 0


#define SIZE_OF_MESH 32
#define FRAME_HEIGHT 192
#define FRAME_WIDTH 192
#define FRAME_COUNT 40
#define NO_OF_TRAINING_EXAMPLES 24

using namespace cv;

extern double *piMeshVector;
extern int mesh_count;
extern int countX;


typedef struct BackgroundSubstractionContext
{
    Mat *poInputFrameMat;
    Mat *poOutputFrameMat;
    Ptr<BackgroundSubtractor> pBG;
}BackgroundSubstractionContext;

void init_background_subtraction(BackgroundSubstractionContext *psBackgroundSubstractionCxt);

void process_background_subtraction(BackgroundSubstractionContext *psBackgroundSubstractionCxt,
                                    Mat oInputFrame
                                    );



typedef struct ActionDetectionContext
{
    int FrameCount;
    BackgroundSubstractionContext *psBackgroundSubtractionCxt;
    //FileStorage fsmesh;
}ActionDetectionContext;

void init_action_detection(ActionDetectionContext *psActionDetectionCxt);

void process_action_detection(ActionDetectionContext *psActionDetectionCxt, 
                                                            Mat oInputFrameMat
                                                        );
bool cal_mesh_vector(Mat *poBinaryMaskMat);


// Background Subtraction header file



