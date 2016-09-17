// API source file
#include "action_detection.hpp"

using namespace std;
using namespace cv;

int dilation_elem =0;
int max_elem= 2;
int max_kernel_size = 21;
int max_value = 255;


int dilation_size = 1;

//int countX;
char *filename = new char[20];

Mat element;


void init_action_detection(ActionDetectionContext *psActionDetectionCxt)
{
    psActionDetectionCxt->FrameCount = 0;

    psActionDetectionCxt->psBackgroundSubtractionCxt = 
                                    new BackgroundSubstractionContext();
    BackgroundSubstractionContext *psBackgroundSubtractionCxt = 
                            psActionDetectionCxt->psBackgroundSubtractionCxt;

    init_background_subtraction(psBackgroundSubtractionCxt);
}

void process_action_detection(ActionDetectionContext *psActionDetectionCxt,
                                    Mat oInputFrameMat
                              
                                    )
{
    /* File Storage */
    
    //FileStorage fsmesh("meshVector.yml",FileStorage::APPEND);

    psActionDetectionCxt->FrameCount = psActionDetectionCxt->FrameCount + 1;
    BackgroundSubstractionContext *psBackgroundSubstractionCxt = 
                                    psActionDetectionCxt->psBackgroundSubtractionCxt;
    //resize(oInputFrameMat,oInputFrameMat,Size(560,360));

    Mat *poBinaryMaskMat;
    process_background_subtraction(psBackgroundSubstractionCxt,
                                    oInputFrameMat
                                   );

    piMeshVector = NULL;

    bool bCalMeshVecFlag = 
        cal_mesh_vector(psBackgroundSubstractionCxt->poOutputFrameMat
                    
                                                );
    
    //printf("piMeshVector %lf\n",piMeshVector[1]);
        
    if (bCalMeshVecFlag == false) cout << "Error : Invalid Mesh " << endl;
   
}

bool cal_mesh_vector(Mat *poBinaryMaskMat)
{

    
    /* convert into binary image */
    threshold(*poBinaryMaskMat,*poBinaryMaskMat,150,255,THRESH_BINARY_INV);
    
    element = getStructuringElement( MORPH_RECT,
                                    Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                    Point( dilation_size, dilation_size ) );
    
    erode (*poBinaryMaskMat, *poBinaryMaskMat, element);
    
    
    
    
    namedWindow("Binary Output",0);
    imshow("Binary Output",*poBinaryMaskMat);
//    //Writing Images
//    sprintf(filename,"./Images/image%i.jpg",countX);
//    countX++;
//    imwrite(filename,*poBinaryMaskMat);
    
    int xBlockWidth = 0;
    int yBlockHeight = 0;
    xBlockWidth = yBlockHeight = SIZE_OF_MESH;
    
    /*
    Note :  xWidth, yHeight should be multiple of 8
    */

    int xWidth = poBinaryMaskMat->cols;
    int yHeight = poBinaryMaskMat->rows;
    
    //cout << "xWidth " << xWidth << "yHeight " << yHeight << endl;
    if( ((xWidth%8) != 0) || ((yHeight%8) != 0))
    {
        cout << "x = " << xWidth << endl;
        cout << "y = " << yHeight << endl;
        cout << " dimensions should be multiple of 8" << endl;
        return false;
    }

    int BlockCount =  (xWidth/xBlockWidth) * (yHeight/yBlockHeight);
    //cout << BlockCount << endl;
    mesh_count = BlockCount;
    //cout << mesh_count << endl;
    piMeshVector = (double *) malloc(sizeof(double)*(BlockCount));
    
    

    if (piMeshVector == NULL)
        cout <<"Fail " << endl;
    // -------------------------------------------------------
    /* Calculation of MESH VECTOR */
    
    int sizeofMesh = SIZE_OF_MESH;
    
    for(int i = 0; i < BlockCount; i++)
        piMeshVector[i] = 0.0;

    
    if (poBinaryMaskMat->type() == CV_32F)
    {
        //double iMainCount = 0.00;
        int k = 0;
        for(int i = 0; i < xWidth; i = i + sizeofMesh)
        {
            for(int j = 0; j < yHeight; j = j + sizeofMesh)
            {
                double fMeshVal = 0.000;          //changed from float.
                int iCount = 0;
                for(int x = i; x < (i+sizeofMesh); x++ )
                {
                    for(int y = j; y < (j+sizeofMesh); y++)
                    {
                         
                        if( poBinaryMaskMat->at<float>(y,x) == 0)
                        {
                            iCount++;
                        }
                        
                    }
                }
                fMeshVal = ((double)iCount)/((double)(sizeofMesh * sizeofMesh));
                //printf("%lf\t",fMeshVal);
                //cout << "mesh value" << fMeshVal << endl;
                piMeshVector[k] = fMeshVal;
                k++;

            }
        }
        
         
    }
    else
    {
        cout << "Binary Image is not of CV_32F type !" << endl;
        return false;
    }
    return true;
}

