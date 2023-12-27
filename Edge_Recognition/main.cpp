#include<iostream>
#include <fstream>
#include<cmath>
#include<string>
using namespace std;

class Edge{
public:
    int numRows, numCols, minVal, maxVal;
    int **mirrorFramedAry;
    int RobertVertical[2][2] = {{1,-1}, {1,-1}};
    int RobertHorizontal[2][2] = {{1,1}, {-1,-1}};
    int RobertRightDiag[2][2] = {{1, -1}, {-1, 1}};
    int RobertLeftDiag[2][2] = {{-1, 1}, {1, -1}};
    int **RobertEdgeAry;
    int SobelVertical[3][3] = {{1,0,-1}, {2,0,-2}, {1,0,-1}};
    int SobelHorizontal[3][3] = {{1,2,1}, {0,0,0}, {-1,-2,-1}};
    int SobelRightDiag[3][3] = {{2,1,0}, {1,0,-1}, {0,-1,-2}};
    int SobelLeftDiag[3][3] = {{0,1,2}, {-1,0,1}, {-2,-1,0}};
    int **SobelEdgeAry;
    int *histRobertAry;
    int *histSobelAry;


    Edge(int rows, int cols, int min, int max){
        numRows = rows;
        numCols = cols;
        minVal = min;
        maxVal = max;
    }

    void loadImage(int **Ary, ifstream &inFile){
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                inFile>>Ary[i][j];
            }
        }
    }

    void mirrorFraming(int **mirrorFramedAry){
        for(int i =0; i <numCols+2; i++){
            mirrorFramedAry[0][i]= mirrorFramedAry[2][i];
        }
        for(int i =0; i <numCols+2; i++){
            mirrorFramedAry[numRows+1][i]= mirrorFramedAry[numRows-1][i];
        }
        for(int i =0; i <numRows+2; i++){
            mirrorFramedAry[0][i]= mirrorFramedAry[2][i];
        }
        for(int i =0; i <numRows+2; i++){
            mirrorFramedAry[i][numCols+1]= mirrorFramedAry[i][numCols-1];
        }
    }

    int computeRobert(int i, int j, int mask[][2]){
        int result = 0;
        int Ary[2][2] = {{mirrorFramedAry[i][j], mirrorFramedAry[i][j+1]}, {mirrorFramedAry[i+1][j], mirrorFramedAry[i+1][j+1]}};
        for(int k=0; k<2; k++){
            for(int l=0; l<2; l++){
                result+= mask[k][l]*Ary[k][l];
            }
        }
        return result;
    }

    void RobertEdgeDetector (int **mirrorFramedAry, int **RobertEdgeAry, ofstream &deBugFile){
        deBugFile<<"entering RobertEdgeDetector method\n";
        int newMax = 0;
        int newMin = 99999;
        int i=1;
        do{
            int j=1;
            do{
                int tmpV = abs(computeRobert(i, j, RobertVertical));
                int tmpH = abs(computeRobert(i, j, RobertHorizontal));
                int tmpR = abs(computeRobert(i, j, RobertRightDiag));
                int tmpL = abs(computeRobert(i, j, RobertLeftDiag));

                RobertEdgeAry[i][j] = tmpV+tmpH+tmpR+tmpL;
                if(RobertEdgeAry[i][j]>newMax){
                    newMax = RobertEdgeAry[i][j];
                }if(RobertEdgeAry[i][j]<newMin){
                    newMin = RobertEdgeAry[i][j];
                }
                j++;

            }while(j<=numCols);

            i++;
        }while(i<=numRows);
        maxVal = newMax;
        minVal = newMin;

        deBugFile<<"Leaving RobertEdgeDetector method\n";
        
    }

    int computeSobel (int i, int j, int mask[][3]){
        int result = 0;
        int Ary[3][3] = {{mirrorFramedAry[i-1][j-1], mirrorFramedAry[i-1][j], mirrorFramedAry[i-1][j+1]}, {mirrorFramedAry[i][j-1], mirrorFramedAry[i][j], mirrorFramedAry[i][j+1]}, {mirrorFramedAry[i+1][j-1], mirrorFramedAry[i+1][j], mirrorFramedAry[i+1][j+1]}};
        for(int k=0; k<3; k++){
            for(int l=0; l<3; l++){
                result+= mask[k][l]*Ary[k][l];
            }
        }
        return result;
    }

    void SobelEdgeDetector (int **mirrorFramedAry, int **SobelEdgeAry, ofstream &deBugFile){
        deBugFile<<"entering SobelEdgeDetector method\n";
        int newMax = 0;
        int newMin = 99999;
        int i=1;
        do{
            int j=1;
            do{
                int tmpV = abs(computeSobel(i, j, SobelVertical));
                int tmpH = abs(computeSobel(i, j, SobelHorizontal));
                int tmpR = abs(computeSobel(i, j, SobelRightDiag));
                int tmpL = abs(computeSobel(i, j, SobelLeftDiag));
                SobelEdgeAry[i][j] = (tmpV + tmpH + tmpR + tmpL);
                if(SobelEdgeAry[i][j]>newMax){
                    newMax = SobelEdgeAry[i][j];
                }if(SobelEdgeAry[i][j]<newMin){
                    newMin = SobelEdgeAry[i][j];
                }

                j++;
            }while(j<=numCols);
            i++;
        }while(i<=numRows);

        maxVal = newMax;
        minVal = newMin;

        deBugFile<<"leaving SobelEdgeDetector method\n";

    }

    void computeHist (int **inAry, int *histAry, ofstream &deBugFile){
        deBugFile<<"entering computeHist method\n";
        int i=1;
        do{
            int j=1;
            do{
                if(inAry[i][j]>0)
                    histAry[inAry[i][j]]+=1;
                j++;
            }while(j<=numCols);
            i++;
        }while(i<=numRows);

        deBugFile<<"Leaving computeHist method\n";
    }

    void printHist (int *histAry, ofstream &histFile, ofstream &deBugFile){
        deBugFile<<"entering printHist method\n";
        histFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        int index=0;
        do{
            histFile<<index<<" "<<histAry[index]<<"\n";
            index++;
        }while(index<=maxVal);

        deBugFile<<"Leavinf printHist method\n";
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

};

int main(int argc, char *argv[]){
    ifstream inFile;
    ofstream deBugFile;
    ofstream outFile1;
    try{
        
        inFile.open(argv[1]);
        int choice = stoi(argv[2]);
        outFile1.open(argv[3]);
        deBugFile.open(argv[4]);
        int row, col, min, max;
        
        inFile>>row;
        inFile>>col;
        inFile>>min;
        inFile>>max;

        Edge edge(row, col, min, max);

        edge.mirrorFramedAry = new int *[edge.numRows+2];
        edge.RobertEdgeAry = new int *[edge.numRows+2];
        edge.SobelEdgeAry = new int *[edge.numRows+2];
        for(int i=0; i<edge.numRows+2; i++){
            edge.mirrorFramedAry[i] = new int[edge.numCols+2];
            edge.RobertEdgeAry[i] = new int[edge.numCols+2];
            edge.SobelEdgeAry[i] = new int[edge.numCols+2];
        }

        edge.histRobertAry = new int[edge.maxVal+1];
        edge.histSobelAry = new int[edge.maxVal+1];
        for(int i=0; i<edge.maxVal+1; i++){
            edge.histRobertAry[i] = 0;
            edge.histSobelAry[i] = 0;
        }
        edge.loadImage(edge.mirrorFramedAry, inFile);
        edge.mirrorFraming(edge.mirrorFramedAry);
        edge.imgReformat(edge.mirrorFramedAry, outFile1);
        if(choice == 1){
            edge.RobertEdgeDetector(edge.mirrorFramedAry, edge.RobertEdgeAry, deBugFile);
            edge.computeHist(edge.RobertEdgeAry, edge.histRobertAry, deBugFile);
            string nameRobertEdge = string(argv[1])+"_Robert_edge.txt";
            ofstream RobertEdgeFile;
            RobertEdgeFile.open(nameRobertEdge);
            edge.imgReformat(edge.RobertEdgeAry, outFile1);
            RobertEdgeFile<<edge.numRows<<" "<<edge.numCols<<" "<<edge.minVal<<" "<<edge.maxVal<<"\n";
            for(int i=0; i<edge.numRows+2; i++){
                for(int j=0; j<edge.numCols+2; j++){
                    RobertEdgeFile<<edge.RobertEdgeAry[i][j]<<" ";
                }
                RobertEdgeFile<<"\n";
            }
            string RobertHist = string(argv[1])+"_Robert_hist.txt";
            ofstream histRobertFile;
            histRobertFile.open(RobertHist);
            edge.printHist(edge.histRobertAry, histRobertFile, deBugFile);
            RobertEdgeFile.close();
            histRobertFile.close();
        }
        
        if(choice == 2){
            edge.SobelEdgeDetector(edge.mirrorFramedAry, edge.SobelEdgeAry, deBugFile);
            edge.computeHist(edge.SobelEdgeAry, edge.histSobelAry, deBugFile);
            string nameSobelEdge = string(argv[1])+"_Sobel_edge.txt";
            ofstream SobelEdgeFile;
            SobelEdgeFile.open(nameSobelEdge);
            edge.imgReformat(edge.SobelEdgeAry, outFile1);
            SobelEdgeFile<<edge.numRows<<" "<<edge.numCols<<" "<<edge.minVal<<" "<<edge.maxVal<<"\n";
            for(int i=0; i<edge.numRows+2; i++){
                for(int j=0; j<edge.numCols+2; j++){
                    SobelEdgeFile<<edge.SobelEdgeAry[i][j]<<" ";
                }
                SobelEdgeFile<<"\n";
            }
            string SobelHist = string(argv[1])+"_Sobel_hist.txt";
            ofstream histSobelFile;
            histSobelFile.open(SobelHist);
            edge.printHist(edge.histSobelAry, histSobelFile, deBugFile);

            SobelEdgeFile.close();
            histSobelFile.close();
        }

        inFile.close();
        outFile1.close();
        deBugFile.close();

    }catch(exception e){cout<<e.what();}

    



    return 0;
}