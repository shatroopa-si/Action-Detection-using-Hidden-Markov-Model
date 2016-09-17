/* Header: Vector Quantization */


#ifndef vectorQ_hpp
#define vectorQ_hpp

#include <stdio.h>
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


/* Macros */
#define NO_OF_ATTEMPTS 2
#define CLUSTER_COUNT 9
#define SIZE_OF_MESH 32

#define MESH_VECTOR_SIZE 36
//#define NO_OF_VECTORS 1
#define NO_OF_IMAGES 40
#define CODEBOOK_SIZE 18
#define NO_OF_TRAINING_EXAMPLES 18


using namespace cv;
using namespace std;

extern double *piMeshVector;
extern int mesh_count;
extern double *codeBook;


Mat Kmeans(Mat data);
int find_label(Mat centers,Mat codeBookMat);

#endif /* vectorQ_hpp */
