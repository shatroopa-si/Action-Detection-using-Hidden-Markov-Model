/* Vector Quantization */

#include "vectorQ.hpp"
double *piMeshVector;
double *codeBook;
int mesh_count = MESH_VECTOR_SIZE;
char *filename = new char [50];
char *sequencename = new char[50];
char *vectorname = new char[50];

int vectorcount = 1;

int main(void)
{
    
    if (TRAIN)
    {
        
        
        Mat KmeansInput,centers;
        Mat codeBookMat;
        
        //Prepare the codebook
        
        cv::FileStorage fscode("codeBook.yml",cv::FileStorage::READ);
        
        for (int codebook_count = 1; codebook_count <= CODEBOOK_SIZE ; codebook_count++)
        {
            sprintf(filename,"code%i",codebook_count);
            cv::FileNode fs = fscode[filename];
            cv::FileNodeIterator it = fs.begin(), it_end = fs.end();
            
            
            codeBook = (double *) malloc(sizeof(double)*(mesh_count));
            
            for (int i = 0; i < mesh_count; i++)
            {
                codeBook[i] = (double)*it++;
                
            }
            
            
            vector<float> codeVec(codeBook, codeBook + mesh_count);
            Mat codeVecMat(codeVec);
            codeBookMat.push_back(codeVecMat.reshape(1,1));
            
        }
        
        
        
        cout << "CodeBook = " << codeBookMat.rows << " x " << codeBookMat.cols << endl;
        while (vectorcount <= NO_OF_TRAINING_EXAMPLES)
        {
            //Read from mesh-vectors:
            
            sprintf(vectorname,"../Testing Dataset/Action-2/Mesh Vectors/meshVector%i.yml",vectorcount);
            cv::FileStorage fsmesh(vectorname,cv::FileStorage::READ);
            piMeshVector = NULL;
            
            for (int frame_count = 1; frame_count <= NO_OF_IMAGES ; frame_count++)
            {
                
                sprintf(filename,"frame%i",frame_count);
                cv::FileNode fs = fsmesh[filename];
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
            fsmesh.release();
            
            //cout << "kMeansInput" << KmeansInput << endl << endl;
            
            
            KmeansInput.convertTo(KmeansInput,CV_32F);
            //cout << KmeansInput.rows << " x " << KmeansInput.cols << endl;
            centers = Kmeans (KmeansInput);
            
            /* ---- Euclidian Distance w.r.t to codeBook.yml ------- */
            sprintf(sequencename,"../Testing Dataset/Action-2/Observation Sequences/ObservationSequence%i.yml",vectorcount);
            FileStorage fsseq(sequencename,cv::FileStorage::WRITE);
            
            fsseq << "Project - Action Detection using HMM" << "Observation Sequence";
            fsseq << "Observation Seq" << vectorcount;
            fsseq << "Seq" << "[:";
            
            for (int label_count = 0; label_count < NO_OF_IMAGES; label_count++)
            {
                Mat iCentersRow = centers.row(label_count);
                int label = find_label(iCentersRow,codeBookMat);
                //cout << label_count << " Label = " << label << endl;
                fsseq << label; 
                
            }
            fsseq << "]";
            
            
            vectorcount++;
        }
    }
    return 0;
    
}

Mat Kmeans(Mat data)
{
    
    int clusterCount = CLUSTER_COUNT;
    Mat labels;
    int attempts = NO_OF_ATTEMPTS;
    Mat centers;
    Mat clusteredOutput;
    
    kmeans(data,
           clusterCount,
           labels,
           TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,10,0.1),
           attempts,
           KMEANS_PP_CENTERS,
           centers
           );
    
    int rows = centers.rows;
    int cols = centers.cols;
    //cout << "Centers" << rows << " x " << cols << endl;
    
    for (int frame_count = 0; frame_count < NO_OF_IMAGES; frame_count++)
    {
        int clusterId = labels.at<int>(frame_count);
        Mat iCentersRow = centers.row(clusterId);
        clusteredOutput.push_back(iCentersRow.reshape(1,1));
        //cout << "label - " << clusterId << endl;
        
    }
        
        
    return clusteredOutput;
}

int find_label(Mat iCentersRow, Mat codeBookMat)
{
    double min_eDistance = -1;
    int min_label;
    for (int x = 0; x < CODEBOOK_SIZE; x++)
    {
        Mat icodeBookRow = codeBookMat.row(x);
        double eDistance;
        
        double sum = 0.0;
        
        for (int i = 0;i < mesh_count; i++)
        {
            sum = sum + pow((iCentersRow.at<float>(0,i) - icodeBookRow.at<float>(0,i)),2);
        }
        eDistance = sqrt(sum);
        //cout << "Edistance" << eDistance << endl;
        
        if (min_eDistance == -1 | eDistance < min_eDistance)
        {
            min_eDistance = eDistance;
            min_label = x;
        }
        
        
    }
    return min_label;
    
}
