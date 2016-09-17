//CODEBOOK

#ifndef codeBook_hpp
#define codeBook_hpp

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>

#include <opencv2/video/background_segm.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


#define NO_OF_ATTEMPTS 2
#define CLUSTER_COUNT 9
#define SIZE_OF_MESH 32
#define NO_OF_IMAGES 40
#define NO_OF_VECTORS 2
#define MESH_VECTOR_SIZE 36


using namespace cv;
using namespace std;


extern double *piMeshVector;
extern int mesh_count;


Mat Kmeans(Mat);



#endif /* codeBook_hpp */
