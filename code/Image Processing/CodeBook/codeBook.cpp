// CODEBOOK

#include "codeBook.hpp"

double *piMeshVector;
int mesh_count = MESH_VECTOR_SIZE;
char *filename = new char[20];
char *vectorname = new char[20];
char *codename = new char[20];
int vectorCount = 1;
int code = 0;

int main(void)
{
    
    
    Mat src;
    Mat src32f;
    Mat KmeansInput,KmeansInput2, centers;
    
    // Write the CodeBook -----
    cv::FileStorage fs("codeBook.yml",cv::FileStorage::WRITE);
    fs << "Project - Action Detection using HMM" << "codeBook";
    
    
    while (vectorCount <= NO_OF_VECTORS)
    {
        
    sprintf(vectorname,"meshVector%i.yml",vectorCount);
        
        
    FileStorage fsread(vectorname,cv::FileStorage::READ);
    
    piMeshVector = NULL;
    
    for (int frame_count = 4; frame_count <= NO_OF_IMAGES - 4; frame_count += 4)
        {
            
            sprintf(filename,"frame%i",frame_count);
            cv::FileNode fs = fsread[filename];
            cv::FileNodeIterator it = fs.begin(), it_end = fs.end();
            
            piMeshVector = (double *) malloc(sizeof(double)*(mesh_count));
            
            for (int i = 0; i < mesh_count; i++)
            {
                piMeshVector[i] = (double)*it++;
                
            }


        vector<float> meshVec(piMeshVector, piMeshVector + mesh_count);
        Mat meshVecMat(meshVec);
        KmeansInput.push_back(meshVecMat.reshape(1,1));
    
        }
    
    
    free(piMeshVector);
    fsread.release();
    vectorCount++;
    }
    KmeansInput.convertTo(KmeansInput,CV_32F);
    
    //int rows = KmeansInput.rows;
    //int cols = KmeansInput.cols;
    cout << "K-Means" << KmeansInput.rows << " x " << KmeansInput.cols << endl;

    centers = Kmeans(KmeansInput);
    //cout << "centers" << centers.rows << "x" << centers.cols << endl;
    
    
    
    
    for (int i=0; i < centers.rows;i++)
    {
        sprintf(codename,"code%i",code);
        Mat iCentersRow = centers.row(i);
        
        fs << codename << "[:";

        for (int j=0;j < centers.cols; j++)
        {
            fs << iCentersRow.at<float>(0,j);
        }
        fs << "]";
        
        code++;

    }
    
    cout << "------Action-2-------" << endl;
    vectorCount = 1;
    while (vectorCount <= NO_OF_VECTORS)
    {
        
        sprintf(vectorname,"meshVector%i.yml",vectorCount + 2);
        
        
        FileStorage fsread(vectorname,cv::FileStorage::READ);
        
        piMeshVector = NULL;
        
        for (int frame_count = 4; frame_count <= NO_OF_IMAGES - 4; frame_count += 4)
        {
            
            sprintf(filename,"frame%i",frame_count);
            cv::FileNode fs = fsread[filename];
            cv::FileNodeIterator it = fs.begin(), it_end = fs.end();
            
            piMeshVector = (double *) malloc(sizeof(double)*(mesh_count));
            
            for (int i = 0; i < mesh_count; i++)
            {
                piMeshVector[i] = (double)*it++;
                
            }
            
            
            vector<float> meshVec(piMeshVector, piMeshVector + mesh_count);
            Mat meshVecMat(meshVec);
            KmeansInput2.push_back(meshVecMat.reshape(1,1));
            
        }
        
        
        free(piMeshVector);
        fsread.release();
        vectorCount++;
    }
    KmeansInput2.convertTo(KmeansInput2,CV_32F);
    
    int rows = KmeansInput2.rows;
    int cols = KmeansInput2.cols;
    cout << "K-Means" << rows << " x " << cols << endl;
    
    centers = Kmeans(KmeansInput2);
    //cout << "centers" << centers.rows << "x" << centers.cols << endl;
    
    
    
    
    for (int i=0;i < centers.rows;i++)
    {
        sprintf(codename,"code%i",code);
        Mat iCentersRow = centers.row(i);
        
        fs << codename << "[:";
        
        for (int j=0;j < centers.cols; j++)
        {
            fs << iCentersRow.at<float>(0,j);
        }
        fs << "]";
        
        code++;
        
    }

    fs.release();
    return 0;
}

Mat Kmeans(Mat data)
{
    
    int clusterCount = CLUSTER_COUNT;
    Mat labels;
    int attempts = NO_OF_ATTEMPTS;
    Mat centers;
    
    kmeans(data,
           clusterCount,
           labels,
           TermCriteria(CV_TERMCRIT_ITER,10,0.1),
           attempts,
           KMEANS_PP_CENTERS,
           centers
           );
    
    int rows = centers.rows;
    int cols = centers.cols;
    cout << "Centers" << rows << " x " << cols << endl;
        //cout << "Labels" << labels.size() << endl;
    
    //for (int y = 0; y < 20; y++)
        for (int x = 0; x < 18; x++)
        {
            //float cluster_idx2 = centers.at<float>(y , x);
            int cluster_idx1 = labels.at<int>(x);
            std::cout << cluster_idx1 << endl;
            //std::cout << cluster_idx2 << endl;
        }
    
    
    return centers;
    
}

