#include<iostream>
#include <fstream>
#include<cmath>
#include<string>
using namespace std;

class point{
public:
    int row;
    int col;
    point(){
        row=0;
        col=0;
    }
    point(int r, int c){
        row = r;
        col = c;
    }

};

class chainCode{
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int lable;
    int **ZFAry;
    int **reConstructAry;
    point coordOffset[8]  = {point(0, +1), point(-1, +1), point(-1, 0), point(-1, -1), point(0, -1), point(+1, -1), point(+1, 0), point(+1, +1)};
    int zeroTable[8] = {6, 0, 0, 2, 2, 4, 4, 6};
    point startP;
    point currentP;
    point nextP;
    int lastQ;
    int chainDir;
     
    chainCode(int row, int col, int min, int max){
        numRows = row;
        numCols = col;
        minVal = min;
        maxVal = max;

    }

    void loadImage(int **ZFAry, ifstream &inFile){
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                inFile>>ZFAry[i][j];
            }
        }
    }

    void getChainCode(int **ZFAry, ofstream &chainCodeFile, ofstream &deBugFile){
        deBugFile<<"entering getChainCode method\n";
        chainCodeFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        int p = 0;
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(ZFAry[i][j]>0){
                    lable = ZFAry[i][j];
                    if(p==0){
                        chainCodeFile<<lable<<" "<<i<<" "<<j<<" ";
                        p=1;
                    }
                    startP.row = i;
                    startP.col = j;
                    p=1;
                    currentP.row = i;
                    currentP.col = j;
                    lastQ = 4;
                
                    do{
                        lastQ = (lastQ+1 % 8);
                        chainDir = findNextP(currentP, lastQ, lable, deBugFile);
                        chainCodeFile<<chainDir<<" ";
                        nextP.row = currentP.row+coordOffset[chainDir].row;
                        nextP.col = currentP.col+coordOffset[chainDir].col;
                        ZFAry[nextP.row][nextP.col] = lable+4;
                        currentP = nextP;
                        if(chainDir == 0){
                            lastQ = zeroTable[7];
                        }else{
                            lastQ = zeroTable[chainDir-1];
                        }
                        deBugFile<<"lastQ = "<<lastQ<<"; currentP.row = "<<currentP.row<<"; currentP.col = "<<currentP.col<<" : nextP.row = "<<nextP.row<<": nextP.col = "<<nextP.col<<"\n";
                    }while(currentP.row != startP.row || currentP.col != startP.col);
                }
            }
        }
        deBugFile<<"leaving getchainCode method\n";        
        
    }



    int findNextP(point currentP, int lastQ, int label, ofstream &deBugFile){
        deBugFile<<"entering findNextP method\n";
        int index = lastQ;
        bool found = false;
        do{
            int iRow = currentP.row + coordOffset[index].row;
            int jCol = currentP.col + coordOffset[index].col;
            if(ZFAry[iRow][jCol] != label || ZFAry[iRow][jCol] != (label+4)){
                chainDir = index;
                found = true;
            }else{
                index = index+1 % 8;
            }
        }while(found == false);
        deBugFile<<"leaving findNextP method\n";
        deBugFile<<"chainDir= "<<chainDir<<"\n";
        return chainDir;
    }

    void constructBoundary(int **reConstructAry, ifstream &chainCodeFile){
        int row, col, min, max;
        chainCodeFile>>row>>col>>min>>max;
        chainCodeFile>>lable>>row>>col>>chainDir;
        startP.row = row;
        startP.col = col;
        currentP.row = row;
        currentP.col = col;

        do {
            reConstructAry[currentP.row][currentP.col] = lable;

            currentP.row += coordOffset[chainDir].row;
            currentP.col += coordOffset[chainDir].col;
            
            chainCodeFile >> chainDir;

        } while (currentP.row != startP.row && currentP.col != startP.col);
        
    
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

    void reformatPrettyPrint(int **inAry, ofstream &outFile){
        outFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        string str = to_string(maxVal);
        int Width = str.length();
        int WW=0;
        int r = 1;
        do{
            int c = 1;
             do{
                if(inAry[r][c] == 0){
                    outFile<<".";
                }else{
                outFile<<inAry[r][c];}
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
    ofstream outFile1, deBugFile;
    try{
        inFile.open(argv[1]);
        outFile1.open(argv[2]);
        deBugFile.open(argv[3]);
        int row, col, min, max;
        inFile>>row;
        inFile>>col;
        inFile>>min;
        inFile>>max;
        chainCode chain(row, col, min, max);

        chain.ZFAry = new int *[chain.numRows+2];
        chain.reConstructAry = new int *[chain.numRows+2];
        for(int i=0; i<chain.numRows+2; i++){
            chain.ZFAry[i] = new int[chain.numCols+2];
        }
        for(int i=0; i<chain.numRows+2; i++){
            chain.reConstructAry[i] = new int[chain.numCols+2];
        }


        for(int i=0; i<chain.numRows+2; i++){
            for(int j=0; j<chain.numCols+2; j++){
                chain.ZFAry[i][j] = 0;
                chain.reConstructAry[i][j] = 0;
            }
        }


        string chainCodeFileNamme = string(argv[1])+"_chainCode.txt";
        string boudaryFileName = string(argv[1])+"_boundary.txt";

        ofstream chainCodeFile, boundaryFile;
        chainCodeFile.open(chainCodeFileNamme);
        boundaryFile.open(boudaryFileName);


        chain.loadImage(chain.ZFAry, inFile);
        deBugFile<<"After loadImage; ZFAry as below\n";
        chain.reformatPrettyPrint(chain.ZFAry, outFile1);


        chain.getChainCode(chain.ZFAry, chainCodeFile, deBugFile);
        deBugFile<<"After getChainCode; ZFAry as below\n";
        chain.reformatPrettyPrint(chain.ZFAry, outFile1);
        chainCodeFile.close();

        ifstream chainCodeFile2;
        chainCodeFile2.open(chainCodeFileNamme);
        chain.constructBoundary(chain.reConstructAry, chainCodeFile2);
        chain.reformatPrettyPrint(chain.reConstructAry, outFile1);
        chain.imgReformat(chain.reConstructAry, boundaryFile);






        inFile.close();
        outFile1.close();
        deBugFile.close();
        boundaryFile.close();
        chainCodeFile2.close();



    }catch(exception e){cout<<e.what();}


    return 0;
}