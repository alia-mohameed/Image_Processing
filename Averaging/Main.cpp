#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

class Enhancement{
public:
    int numRows, numCols, minVal, maxVal;
    int maskRows, maskCols, maskMin, maskMax;
    int **mirrorFramedAry;
    int **avgAry;
    int **GaussAry;
    int neighborAry[25];
    int maskAry[25];
    int maskWeight;
    int *histAvgAry;
    int *histGaussAry;

    void loadImage(ifstream &inFile, int **mirrorFramedAry){
        int i=2;
        do{
            int j=2;
            do{
                inFile>>mirrorFramedAry[i][j];
                j++;
            }while(j<=numCols);
            i++;
        }while(i<=numRows);
    }

    void mirrorFraming(int **mirrorFramedAry){
        for(int i =0; i <numCols+4; i++){
            mirrorFramedAry[0][i]= mirrorFramedAry[2][i];
            mirrorFramedAry[1][i]= mirrorFramedAry[2][i];
        }
        for(int i =0; i <numCols+4; i++){
            mirrorFramedAry[numRows][i]= mirrorFramedAry[numRows-1][i];
            mirrorFramedAry[numRows+1][i]= mirrorFramedAry[numRows-1][i];
        }
        for(int i =0; i <numRows+4; i++){
            mirrorFramedAry[0][i]= mirrorFramedAry[2][i];
            mirrorFramedAry[1][i]= mirrorFramedAry[2][i];
        }
        for(int i =0; i <numRows+4; i++){
            mirrorFramedAry[i][numCols]= mirrorFramedAry[i][numCols-1];
            mirrorFramedAry[i][numCols+1]= mirrorFramedAry[i][numCols-1];
        }
    }

    int loadMaskAry(ifstream &maskFile, int maskAry[]){
        int sum=0;
        for(int i=0; i<25; i++){
            maskFile>>maskAry[i];
            sum+=maskAry[i];
        }
        return sum;
    }

    void loadNeighborAry(int **mirrorFramedAry, int r, int c, int neighborAry[]){
        int index=0;
        for(int i=r-2; i<=r+2; i++){
            for(int j=c-2; j<=c+2; j++){
                neighborAry[index] = mirrorFramedAry[i][j];
                index++;
            }
        }
    }

    int avg5x5 (int **mirrorFramedAry, int i, int j){
        int sum=0, average=0;
        int index=0;
        for(int r=i-2; r<=i+2; r++){
            for(int c=j-2; c<=j+2; c++){
                sum += mirrorFramedAry[r][c];
            }
        }
        average=sum/25;
        return average;
    }

    void computeGauss5x5 (int **mirrorFramedAry, int **GaussAry, int maskAry[], int neighborAry[], int weight, ofstream &deBugFile){
        
        deBugFile<<"Entering computeGauss method\n";
        int i=2;
        do{
            int j=2;
            do{
                loadNeighborAry(mirrorFramedAry, i, j, neighborAry);
                GaussAry[i][j] = convolution(neighborAry, maskAry, weight, deBugFile);
                j++;
            }while(j<numCols+2);
            i++;
        }while(i<numRows+2);
        deBugFile<<"Leaving computeGauss method\n";
    }

    void printHist(int *histAry, ofstream &histFile, int maxVal, ofstream &deBugFile){
        deBugFile<<"Entering printHist method";
        histFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        int index=0;
        do{
            histFile<<index<<" "<<histAry[index]<<"\n";
            index++;
        }while(index<=maxVal);
        deBugFile<<"Leaving printHist method\n";
    }

    int convolution(int neighborAry[], int maskAry[], int weight, ofstream &deBugFile){
        deBugFile<<"Entering convolution method\n";
        int result=0;
        int i=0;
        do{
            result+=neighborAry[i]*maskAry[i];
            i++;
        }while(i<25);
        deBugFile<<"in convolution method\n";
        deBugFile<<"Leaving convolution method\n";
        return (result/weight);
    }

    void computeAvg5x5(int **mirrorFramedAry, int **avgAry, ofstream &deBugFile){
        deBugFile<<"entering computeAvg5x5 method\n";
        int i=2;
        do{
            int j=2;
            do{
                avgAry[i][j] = avg5x5(mirrorFramedAry, i, j);
                if(avgAry[i][j]>maxVal){
                    maxVal = avgAry[i][j];
                }
                j++;
            }while(j<numCols+2);
            i++;
        }while(i<numRows+2);
        deBugFile<<"Leaving computeAvg5x5 method\n";
    }

    void imageReformat(int **inAry, ofstream &outFile){
        outFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        string str = to_string(maxVal);
        int Width = str.length();
        int WW=0;
        int r = 2;
        do{
            int c = 2;
            do{
                outFile<<inAry[r][c];
                str = to_string(inAry[r][c]);
                WW = str.length();
                do{
                    outFile<<" ";
                    WW++;
                }while(WW<Width);
                c++;
            }while(c<numCols+2);
            outFile<<"\n";
            r++;
        }while(r<numRows+2);
    }

    void computeHist(int **inAry, int *histAry, ofstream &deBugFile){
        deBugFile<<"entering computeHist method\n";
        int i=2;
        do{
            int j=2;
            do{
                histAry[inAry[i][j]]++;
                j++;
            }while(j<numCols+2);
            i++;
        }while(i<numRows+2);
        deBugFile<<"Leaving computeHist method\n";
    }




};

int main(int argc, char *argv[]){
    ifstream inFile, maskFile;
    ofstream outFile1, deBugFile;
    int choice;
    try{
        inFile.open(argv[1]);
        maskFile.open(argv[2]);
        outFile1.open(argv[3]);
        deBugFile.open(argv[4]);
        choice = stoi(argv[5]);

        Enhancement img;
        inFile>>img.numRows;
        inFile>>img.numCols;
        inFile>>img.minVal;
        inFile>>img.maxVal;

        maskFile>>img.maskRows;
        maskFile>>img.maskCols;
        maskFile>>img.maskMin;
        maskFile>>img.maskMax;

        img.mirrorFramedAry = new int *[img.numRows+4];
        img.avgAry = new int *[img.numRows+4];
        img.GaussAry = new int *[img.numRows+4];
        for(int i=0; i<img.numRows+4; i++){
            img.mirrorFramedAry[i] = new int[img.numCols+4];
            img.avgAry[i] = new int[img.numCols+4];
            img.GaussAry[i] = new int[img.numCols+4];
        }
        for(int i=0; i<img.numRows+4; i++){
            for(int j=0; j<img.numCols+4; j++){
                img.mirrorFramedAry[i][j] = 0;
                img.avgAry[i][j] = 0;
                img.GaussAry[i][j] = 0;

            }
        }

        img.histAvgAry = new int[img.maxVal+1];
        img.histGaussAry = new int[img.maxVal+1];
        for(int i=0; i<img.maxVal+1; i++){
            img.histAvgAry[i] = 0;
            img.histGaussAry[i] = 0;
        }

        img.loadImage(inFile, img.mirrorFramedAry);
        img.mirrorFraming(img.mirrorFramedAry);
        img.imageReformat(img.mirrorFramedAry, outFile1);
        if(choice == 1){
            img.computeAvg5x5(img.mirrorFramedAry, img.avgAry, deBugFile);
            img.computeHist(img.avgAry, img.histAvgAry, deBugFile);
            string nameAvg = (string) argv[1]+ "_Avg5x5.txt";
            ofstream avgFile;
            avgFile.open(nameAvg);
            img.imageReformat(img.avgAry, outFile1);
            avgFile<<img.numRows<<" "<<img.numCols<<" "<<img.minVal<<" "<<img.maxVal<<"\n";
            for(int i=0; i<img.numRows+4; i++){
                for(int j=0; j<img.numCols+4; j++){
                    avgFile<<img.avgAry[i][j]<<" ";
                }
                avgFile<<"\n";
            }
            string avgHist = (string) argv[1]+"_Avg5x5_hist.txt";
            ofstream histAvgFile;
            histAvgFile.open(avgHist);
            img.printHist(img.histAvgAry, histAvgFile, img.maxVal, deBugFile);
            
            avgFile.close();
            histAvgFile.close();

        }else if(choice == 2){
            img.maskWeight = img.loadMaskAry(maskFile, img.maskAry);
            img.computeGauss5x5(img.mirrorFramedAry, img.GaussAry, img.maskAry, img.neighborAry, img.maskWeight, deBugFile);
            img.computeHist(img.GaussAry, img.histGaussAry, deBugFile);
            string nameGauss = (string) argv[1]+"_Gaus5x5.txt";
            ofstream GaussFile;
            GaussFile.open(nameGauss);
            img.imageReformat(img.GaussAry, outFile1);
            GaussFile<<img.numRows<<" "<<img.numCols<<" "<<img.minVal<<" "<<img.maxVal<<"\n";
            for(int i=0; i<img.numRows+4; i++){
                for(int j=0; j<img.numCols+4; j++){
                    GaussFile<<img.GaussAry[i][j]<<" ";
                }
                GaussFile<<"\n";
            }
            string GauseHist = (string) argv[1]+"_Gauss5x5_hist.txt";
            ofstream histGaussFile;
            histGaussFile.open(GauseHist);
            img.printHist(img.histGaussAry, histGaussFile, img.maxVal, deBugFile);

            GaussFile.close();
            histGaussFile.close();

        }

        inFile.close();
        maskFile.close();
        outFile1.close();
        deBugFile.close();

    }catch(exception e){cout<<e.what();}

    return 0;
}