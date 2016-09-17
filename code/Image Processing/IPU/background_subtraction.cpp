// Background Subtraction Source file 
#include "action_detection.hpp"

using namespace std;

void init_background_subtraction(BackgroundSubstractionContext 
                                                *psBackgroundSubstractionCxt)
{
    /*
    cout << " init background subtraction"  << endl;
    */
    psBackgroundSubstractionCxt->pBG = createBackgroundSubtractorMOG2();
}

void process_background_subtraction(BackgroundSubstractionContext 
                                                *psBackgroundSubstractionCxt,
                                                Mat oInputFrame
                                                )
{
    /*
    cout << " process background subtraction " << endl;
    */
    Mat oTempMat;
    psBackgroundSubstractionCxt->pBG->apply(oInputFrame, oTempMat);
    
    //namedWindow("Mask",0);
    //imshow("Mask", oTempMat);
    //waitKey(0);

    /*  cout << "channels : " << oTempMat.channels() << endl; */
    

    if(oTempMat.type() != CV_8UC1)
        cout << "type is not CV_8UC1 : ENUM-> " << oTempMat.type() << endl;

    oTempMat.convertTo(oTempMat,CV_32F);

    psBackgroundSubstractionCxt->poOutputFrameMat = new Mat(oTempMat.rows, oTempMat.cols, oTempMat.type());
    oTempMat.copyTo(*psBackgroundSubstractionCxt->poOutputFrameMat);
}   


