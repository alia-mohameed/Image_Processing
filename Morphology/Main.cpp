#include <iostream>
#include <cmath>
#include<fstream>
#include<string>
using namespace std;

class Morphology{
public:
    int numImgRows, numImgCols, imgMin, imgMax;
    int numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin;
    int rowFrameSize, colFrameSize;
    int extraRows, extraCols;
    int rowSize, colSize;
    int **zeroFramedAry;
    int **morphAry;
    int **tempAry;
    int **structAry;

    void zero2DAry(int **Ary, int nRows, int nCols){
        int i=0;
        do{
            int j=0;
            do{

                Ary[i][j] = 0;
                j++;

            }while(j<nCols);
            i++;

        }while(i<nRows);

    }

    void loadImg(ifstream &imgFile, int **zeroFramedAry){
        int i=rowOrigin;
        do{
            int j= colOrigin;
            do{
                imgFile>>zeroFramedAry[i][j];
                j++;

            }while(j<colSize);
            i++;
            
        }while(i<rowSize);
    }

    void loadStruct(ifstream &structFile, int **structAry){
        int i=0;
        do{
            int j=0;
            do{
                structFile>>structAry[i][j];
                j++;

            }while(j<numStructCols);
            i++;

        }while(i<numStructRows);

    }

    void ComputeDilation(int **inAry, int **outAry){
        int i= rowFrameSize;
        do{
            int j=colFrameSize;
            do{
                if(inAry[i][j]>0){
                    onePixelDilation(i,j,inAry,outAry);
                
                }
                j++;
            }while(j<colSize);
            i++;
        }while(i<rowSize);
    }

    void ComputeErosion(int **inAry, int **outAry){
        int i=rowFrameSize;
        do{
            int j=colFrameSize;
            do{
                if(inAry[i][j]>0){
                    onePixelErosion(i,j,inAry,outAry);
                }
                j++;
            }while(j<colSize);
            i++;
        }while(i<rowSize);
    }
    void ComputeOpening(int **zeroFramedAry, int **morphAry, int **tempAry){
        ComputeErosion(zeroFramedAry, tempAry);
        ComputeDilation(tempAry, morphAry);
    }

    void ComputeClosing(int **zeroFramedAry, int **morphAry, int **tempAry){
        ComputeDilation(zeroFramedAry, tempAry);
        ComputeErosion(tempAry, morphAry);
    }

    void onePixelDilation(int i, int j, int **inAry, int **outAry){
        int iOffset = i-rowOrigin;
        int jOffset = j-colOrigin;
        int rIndex=0;
        do{
            int cIndex =0;
            do{
                if(structAry[rIndex][cIndex]>0){
                    outAry[iOffset+rIndex][jOffset+cIndex] = 1;
                }
                cIndex++;
            }while(cIndex<numStructCols);
            rIndex++;
        }while(rIndex<numStructRows);
    }

    void onePixelErosion(int i, int j, int **inAry, int **outAry){
        int iOffset=i-rowOrigin;
        int jOffset=j-rowOrigin;
        bool matchFlag=true;
        int rIndex = 0;
        do{
            int cIndex = 0;
            do{
                if((structAry[rIndex][cIndex]>0) && (inAry[iOffset+rIndex][jOffset+cIndex]<=0)){
                    matchFlag = false;
                }
                cIndex++;
            }while((matchFlag==true )&& (cIndex<numStructCols));
            rIndex++;
        }while((matchFlag==true) && (rIndex<numStructRows));

        if(matchFlag==true){
            outAry[i][j] = 1;
        }else{
            outAry[i][j]=0;
        }
    
    }

    void AryToFile(int **Ary, ofstream &outFile){
        outFile<<numImgRows<<numImgCols<<imgMin<<imgMax<<"\n";
        int i=rowOrigin;
        do{
            int j=colOrigin;
            do{
                outFile<<Ary[i][j];

            }while(j<numImgCols);

        }while(i<numImgRows);
    }

    void prettyPrint(int **Ary, int rowSize, int colSize, ofstream &outFile){
        int i=0;
        do{
            int j=0;
            do{
                if(Ary[i][j] == 0){
                    outFile<<". ";
                }
                else{
                    outFile<<"1 ";
                }

                // print the structure element
                j++;

            }while(j<colSize);
            outFile<<"\n";
            i++;
        }while(i<rowSize);
    }

    void objectExtraction(int **zeroFramedAry, int **morphAry, int **tempAry, ofstream &outFile){
        ComputeOpening(zeroFramedAry, morphAry, tempAry);
        outFile<<"the operation used to extract the object is Opening\n";
    }

    void fillHoles(int **zeroFramedAry, int **morphAry, int **tempAry, ofstream &outFile){
        ComputeClosing(morphAry, zeroFramedAry, tempAry);
        outFile<<"the operation used to fill the holes is Closing\n";
    }


};

int main(int argc, char *argv[]){

    ifstream imgFile, structFile, img1, structFile1;
    ofstream outFile1, outFile2;
    try{
        imgFile.open(argv[1]);
        structFile.open(argv[2]);
        img1.open(argv[3]);
        structFile1.open(argv[4]);
        outFile1.open(argv[5]);
        outFile2.open(argv[6]);

        Morphology morph;
        imgFile>>morph.numImgRows;
        imgFile>>morph.numImgCols;
        imgFile>>morph.imgMin;
        imgFile>>morph.imgMax;
        structFile>>morph.numStructRows;
        structFile>>morph.numStructCols;
        structFile>>morph.structMin;
        structFile>>morph.structMax;
        structFile>>morph.rowOrigin;
        structFile>>morph.colOrigin;

        morph.rowFrameSize = morph.numStructRows/2;
        morph.colFrameSize = morph.numStructCols/2;
        morph.extraRows = morph.rowFrameSize*2;
        morph.extraCols = morph.colFrameSize*2;
        morph.rowSize = morph.numImgRows+morph.extraRows;
        morph.colSize = morph.numImgCols+morph.extraCols;

        morph.zeroFramedAry = new int *[morph.rowSize];
        morph.morphAry = new int *[morph.rowSize];
        morph.tempAry = new int *[morph.rowSize];
        for(int i=0; i<morph.rowSize; i++){
            morph.zeroFramedAry[i] = new int[morph.colSize];
            morph.morphAry[i] = new int[morph.colSize];
            morph.tempAry[i] = new int[morph.colSize];
        }
        morph.structAry = new int *[morph.numStructRows];
        for(int i=0; i<morph.numStructRows; i++){
            morph.structAry[i] = new int[morph.numStructCols];
        }

        morph.zero2DAry(morph.zeroFramedAry, morph.rowSize, morph.colSize);
        morph.loadImg (imgFile, morph.zeroFramedAry);
        outFile1<<"Printing the image to outFile1\n";
        morph.prettyPrint (morph.zeroFramedAry, morph.rowSize, morph.colSize, outFile1);

        morph.zero2DAry(morph.structAry, morph.numStructRows, morph.numStructCols);
        morph.loadStruct (structFile, morph.structAry);
        morph.prettyPrint (morph.structAry, morph.numStructRows, morph.numStructCols, outFile1);
        
        morph.zero2DAry(morph.morphAry, morph.rowSize, morph.colSize);
        morph.ComputeDilation(morph.zeroFramedAry, morph.morphAry);
        outFile1<<"Printing the result of Dialation to outFile1\n";
        morph.prettyPrint (morph.morphAry, morph.rowSize, morph.colSize, outFile1);

        morph.zero2DAry(morph.morphAry, morph.rowSize, morph.colSize);
        morph.ComputeErosion (morph.zeroFramedAry, morph.morphAry);
        outFile1<<"Printing the result of Erosion to outFile1\n";
        morph.prettyPrint (morph.morphAry, morph.rowSize, morph.colSize, outFile1);

        morph.zero2DAry(morph.morphAry, morph.rowSize, morph.colSize);
        morph.ComputeOpening(morph.zeroFramedAry, morph.morphAry, morph.tempAry);
        outFile1<<"Printing the result of Opening to outFile1\n";
        morph.prettyPrint (morph.morphAry, morph.rowSize, morph.colSize, outFile1);

        morph.zero2DAry(morph.morphAry, morph.rowSize, morph.colSize);
        morph.ComputeClosing (morph.zeroFramedAry, morph.morphAry, morph.tempAry);
        outFile1<<"Printing the result of Closing to outFile1\n";
        morph.prettyPrint (morph.morphAry, morph.rowSize, morph.colSize, outFile1);

        //PerformTask1(…)
        morph.zero2DAry(morph.zeroFramedAry, morph.rowSize, morph.colSize);
        morph.loadImg (img1, morph.zeroFramedAry);
        outFile2<<"Printing the image to outFile2\n";
    

        morph.zero2DAry(morph.structAry, morph.numStructRows, morph.numStructCols);
        morph.zero2DAry(morph.morphAry, morph.rowSize, morph.colSize);
        morph.loadStruct (structFile1, morph.structAry);
        morph.objectExtraction(morph.zeroFramedAry, morph.morphAry, morph.tempAry, outFile2);
        
        morph.zero2DAry(morph.zeroFramedAry, morph.rowSize, morph.colSize);
        morph.zero2DAry(morph.morphAry, morph.rowSize, morph.colSize);
        morph.fillHoles(morph.zeroFramedAry, morph.morphAry, morph.tempAry, outFile2);
        morph.prettyPrint (morph.zeroFramedAry, morph.rowSize, morph.colSize, outFile2);
        outFile2<<"printing the structure element\n";
        morph.prettyPrint (morph.structAry, morph.numStructRows, morph.numStructCols, outFile2);

        
        imgFile.close();
        structFile.close();
        img1.close();
        structFile1.close();
        outFile1.close();
        outFile2.close();
    }catch(exception e){cout<<e.what();}
    

    return 0;
}