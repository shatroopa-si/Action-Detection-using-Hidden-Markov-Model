// main.cpp


#include "action_detection.hpp"

using namespace cv;
using namespace std;

char *videoname = new char[50];
char *vectorname = new char[50];
char *InputPath = new char[50];
double *piMeshVector;
int mesh_count;
int frameNumber;
int countX;
int train_example = 1;
//char *filename = new [20];

int main()
{

    if (TRAIN)
    {
    
        for (train_example = 1; train_example <= NO_OF_TRAINING_EXAMPLES; train_example++)
        {
            sprintf(vectorname,"../Training Dataset/Action-2/Mesh Vectors/meshVector%i.yml",train_example);
            FileStorage fsmesh(vectorname,FileStorage::WRITE);
            fsmesh << "Project - Action Detection" << "Mesh Vector";
            fsmesh << "FrameCount" << "[:" << FRAME_COUNT << "]";
            
            
            
            sprintf(InputPath,"../Training/Action-2/action2-train (%i).mov",train_example);
            cv::VideoCapture cap;
            cap.open(InputPath);
            ActionDetectionContext sActionDetectionCtx;
                        fsmesh << "Training example " << train_example;
            
            char *Tag = new char [20];
            
            /* Initialisation */
            
            init_action_detection(&sActionDetectionCtx);
            
            Mat oOrigFrameMat,oFrameMat;
            Mat oOutputMat;
            
            frameNumber = 1;
            int count = 1;
            countX = 1;
            while(cap.read(oOrigFrameMat) && frameNumber <= FRAME_COUNT)
            {
                cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
                cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
                
                frameNumber++;
                

                //Resizing
                int iframeHeight = FRAME_HEIGHT;
                int iframeWidth = FRAME_WIDTH;
                resize(oOrigFrameMat, oOrigFrameMat, Size(iframeHeight,iframeWidth));
                namedWindow("Input",0);
                imshow("Input",oOrigFrameMat);
        
                
                                
                char q;
                q = (char)waitKey(5);
                if(q == 'q')
                    break;
                
                /*** Processing each frame ***/
                
                process_action_detection(&sActionDetectionCtx, oOrigFrameMat);
                sprintf(Tag,"frame%i",sActionDetectionCtx.FrameCount);
                
                fsmesh << Tag << "[:";
                for (int i=0;i<mesh_count;i++)
                {
                    
                    fsmesh << piMeshVector[i];
                }
                fsmesh << "]";
                
                free(piMeshVector);
            }
            fsmesh.release();

            }
        }
    
    return 0;
}
