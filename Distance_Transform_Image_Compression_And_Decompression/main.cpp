#include<iostream>
#include <fstream>
#include<cmath>
#include<string>

using namespace std;

class ImageCompression{
public:
    int numRows, numCols, minVal, maxVal;
    int newMinVal, newMaxVal;
    int ** ZFAry;
    int **skeletonAry;

    void setZero(int **Ary){
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                Ary[i][j] = 0;
            }
        }
    }

    void loadImage(int **Ary, ifstream &inFile){
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                inFile>>Ary[i][j];
            }
        }
    }

    void loadSkeleton(ifstream &skeletonFile, int **ZFAry, ofstream &deBugFile){
        deBugFile<<"Entering loadSkeleton method\n";
        skeletonFile>>numRows;
        skeletonFile>>numCols;
        skeletonFile>>minVal;
        skeletonFile>>maxVal;
        int i=0, j=0, val=0;
        while(!skeletonFile.eof()){
            skeletonFile>>i;
            skeletonFile>>j;
            skeletonFile>>val;
            ZFAry[i][j] = val;
        }
        deBugFile<<"Leaving loadSkeleton method\n";
    }

    void imgReformat(int **inAry, ofstream &outFile){
        outFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        string str = to_string(maxVal);
        int Width = str.length();
        int WW=0;
        int r = 1;
        do{
            int c = 1;
            do{
                outFile<<inAry[r][c];
                str = to_string(inAry[r][c]);
                WW = str.length();
                do{
                    outFile<<" ";
                    WW++;
                }while(WW<Width);
                c++;
            }while(c<numCols+1);
            outFile<<"\n";
            r++;
        }while(r<numRows+1);
    }



    void distance8Pass1(int **Ary, ofstream &deBugFile){
        deBugFile<<"Entering distance8Pass1 method\n";
        newMinVal = 9999;
        newMaxVal = 0;
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(Ary[i][j]>0){
                    int neighbors[4] = {Ary[i-1][j-1]+1, Ary[i-1][j]+1, Ary[i-1][j+1]+1, Ary[i][j-1]+1};
                    int min=neighbors[0];
                    for(int k: neighbors){
                        if(min>k){
                            min = k;
                        }
                    }

                    Ary[i][j] = min;
                }
                
                if(ZFAry[i][j] > 0){
                    newMinVal = ZFAry[i][j];
                }
                if(newMaxVal<ZFAry[i][j]){
                    newMaxVal = ZFAry[i][j];
                }
            }
        }
        deBugFile<<"Leaving distance8Pass1 method\n";
    }

    void distance8Pass2(int **Ary, ofstream &deBugFile){
        deBugFile<<"Entering distance8Pass2 method\n";
        newMinVal = 9999;
        newMaxVal = 0;
        for(int i=numRows; i>=1; i--){
            for(int j=numCols; j>=1; j--){
                if(Ary[i][j]>0){
                    int ary[5] = {Ary[i][j+1]+1, Ary[i+1][j-1]+1, Ary[i+1][j]+1, Ary[i+1][j+1]+1, Ary[i][j]};
                    int min=ary[0];
                    for(int k: ary){
                        if(min>k){
                            min = k;
                        }
                    }

                    Ary[i][j] = min;
                }

                if(ZFAry[i][j] > 0){
                    newMinVal = ZFAry[i][j];
                }
                if(newMaxVal<ZFAry[i][j]){
                    newMaxVal = ZFAry[i][j];
                }
            }
        }
        deBugFile<<"Leaving distance8Pass2 method\n";
    }

    bool isLocalMaxima(int pixel, int *neighbors, int n){
        for(int i=0; i< n; i++){
            if(pixel<neighbors[i]){
                return false;
            }
        }
        return true;
    }

    void computeLocalMaxima(int **ZFAry, int **skeletonAry, ofstream &deBugFile){
        deBugFile<<"Entering computeLocalMaxima method\n";
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                int neighbors[8] = {ZFAry[i-1][j-1], ZFAry[i-1][j], ZFAry[i-1][j+1], ZFAry[i][j-1], ZFAry[i][j+1], ZFAry[i+1][j-1], ZFAry[i+1][j], ZFAry[i+1][j+1]};
                if(ZFAry[i][j]!=0){
                    if(isLocalMaxima(ZFAry[i][j], neighbors, 8)){
                        skeletonAry[i][j] = ZFAry[i][j];
                    }else{
                        skeletonAry[i][j] = 0;
                    }
                }
            }
        }
        deBugFile<<"Leaving computeLocalMaxima method\n";
    }

    void extractSkeleton(int **skeletonAry, ofstream &skeletonFile, ofstream &deBugFile){
        deBugFile<<"Entering extractSkeleton method\n";
        skeletonFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                if(skeletonAry[i][j]>0){
                    skeletonFile<<i<<" "<<j<<" "<<skeletonAry[i][j]<<" \n";
                }
            }
        }
        deBugFile<<"Leaving extractSkeleton method\n";
    }

    void imageCompression (int **ZFAry, int **skeletonAry, ofstream &skeletonFile, ofstream &outFile1, ofstream &deBugFile){
        deBugFile<<"Entering imageCompression () method\n";
        computeLocalMaxima(ZFAry, skeletonAry, deBugFile);
        outFile1<<"\nLocal maxima operation\n";
        imgReformat (skeletonAry, outFile1);
        extractSkeleton(skeletonAry, skeletonFile, deBugFile);
        deBugFile<<"Leaving imageCompression () method\n";
    }

    void expensionPass1(int **Ary, ofstream &deBugFile){
        deBugFile<<"Entering expensionPass1 method\n";
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(Ary[i][j] == 0){
                    int neighbors[9] = {Ary[i-1][j-1]-1, Ary[i-1][j]-1, Ary[i-1][j+1]-1, Ary[i][j-1]-1, Ary[i][j+1]-1, Ary[i+1][j-1]-1, Ary[i+1][j]-1, Ary[i+1][j+1]-1, Ary[i][j]};
                    int max = neighbors[0];
                    for(int k: neighbors){
                        if(max<k){
                            max = k;
                        }
                    }
                    Ary[i][j] = max;
                }
            }
        }
        deBugFile<<"Leaving expensionPass1 method\n";
    }

    void expensionPass2(int **Ary, ofstream &deBugFile){
        deBugFile<<"Entering expensionPass2 method\n";
        for(int i=numRows; i>=1; i--){
            for(int j=numCols; j>=1; j--){
                int neighbors[9] = {Ary[i-1][j-1]-1, Ary[i-1][j]-1, Ary[i-1][j+1]-1, Ary[i][j-1]-1, Ary[i][j+1]-1, Ary[i+1][j-1]-1, Ary[i+1][j]-1, Ary[i+1][j+1]-1, Ary[i][j]};
                int max = neighbors[0];
                for(int k: neighbors){
                    if(max<k){
                        max = k;
                    }
                }
                if(Ary[i][j]<max){
                    Ary[i][j] = max;
                }
            }
        }
        deBugFile<<"Leaving expensionPass2 method\n";
    }

    void threshold(int **Ary, ofstream &deCompressFile, int thr){
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(Ary[i][j]>=thr){
                    deCompressFile<<1<<" ";
                }else{
                    deCompressFile<<0<<" ";
                }
            }
            deCompressFile<<"\n";
        }

    }

    void imageDeCompression(int **ZFAry, ofstream &outFile1, ofstream &deBugFile){
        deBugFile<<"Entering imageDeCompression () method\n";
        expensionPass1(ZFAry, deBugFile);
        outFile1<<"\nexpansion pass 1\n";
        imgReformat(ZFAry, outFile1);
        expensionPass2(ZFAry, deBugFile);
        outFile1<<"\nexpansion pass 2\n";
        imgReformat(ZFAry, outFile1);
        deBugFile<<"Leaving imageDeCompression () method\n";
    }

    void Distance8(int **ZFAry, ofstream &outFile1, ofstream &deBugFile){
        deBugFile<<"Entering Distance8 () method\n";
        distance8Pass1(ZFAry, deBugFile);
        outFile1<<"\ndistance transform pass 1\n";
        imgReformat(ZFAry, outFile1);
        distance8Pass2(ZFAry, deBugFile);
        outFile1<<"\ndistance transform pass 2\n";
        imgReformat(ZFAry, outFile1);
        deBugFile<<"Leaving Distance8 () method\n";
    }

};



int main(int argc, char *argv[]){
    ifstream inFile;
    ofstream outFile1;
    ofstream deBugFile;
    ofstream skeletonFile;
    ofstream deCompressFile;
    
    try{
        inFile.open(argv[1]);
        outFile1.open(argv[2]);
        deBugFile.open(argv[3]);

        ImageCompression img;
        inFile>>img.numRows;
        inFile>>img.numCols;
        inFile>>img.minVal;
        inFile>>img.maxVal;

        img.ZFAry = new int *[img.numRows+2];
        for(int i=0; i< img.numRows+2; i++){
            img.ZFAry[i] = new int [img.numCols+2];
        }

        img.skeletonAry = new int *[img.numRows+2];
        for(int i=0; i< img.numRows+2; i++){
            img.skeletonAry[i] = new int [img.numCols+2];
        }

        string skeletonFileName = string(argv[1])+"_skeleton.txt";
        skeletonFile.open(skeletonFileName);
        string deCompressFileName = string(argv[1])+"_deCompressed.txt";
        deCompressFile.open(deCompressFileName);


        img.setZero(img.ZFAry);
        img.setZero(img.skeletonAry);
        img.loadImage(img.ZFAry, inFile);

        img.Distance8(img.ZFAry, outFile1, deBugFile);
        img.imageCompression(img.ZFAry, img.skeletonAry, skeletonFile, outFile1, deBugFile);
        skeletonFile.close();

        ifstream skeletonFile;
        skeletonFile.open(skeletonFileName);
        img.setZero(img.ZFAry);
        img.loadSkeleton(skeletonFile, img.ZFAry, deBugFile);

        img.imageDeCompression(img.ZFAry, outFile1, deBugFile);
        deCompressFile<<img.numRows<<" "<<img.numCols<<" "<<img.minVal<<" "<<img.maxVal<<"\n";
        img.threshold (img.ZFAry, deCompressFile,1);


        

        inFile.close();
        outFile1.close();
        deBugFile.close();
        skeletonFile.close();
        deCompressFile.close();


    }catch(exception e){cout<<e.what();}




    return 0;
}