#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class image{
    public: int numRows, numCols, minVal, maxVal, thrValue;
    image(){
        numRows=0;
        numCols=0;
        minVal=0;
        maxVal=0;
        thrValue=0;
    }

    void processing(ifstream &inFile, ofstream &outFile1, ofstream &outFile2, int &thrValue){
        int pixleVal;
        int count=0;
        while(!inFile.eof()){
            inFile>>pixleVal;
            if(pixleVal>= thrValue){
                outFile1<<1<<" ";
                outFile2<<pixleVal<<" ";
            }else{
                outFile1<<0<<" ";
                outFile2<<0<<" ";
            }
            count++;
            if(count>=numCols){
                outFile1<<"\n";
                outFile2<<"\n";
                count=0;
            }
        }

    }
    

};

int main(int argc, char *argv[]){
    image img;
    ifstream inFile;
    ofstream outFile1;
    ofstream outFile2;
    try{
        inFile.open(argv[1]);
        cout<<"opening inFile"<<endl;
        inFile>>img.numRows;
        inFile>>img.numCols;
        inFile>>img.minVal;
        inFile>>img.maxVal;
        cout<<"enter a threshold value: ";
        cin>>img.thrValue;
        
        outFile1.open(argv[2]);
        outFile1<<img.numRows<<" "<<img.numCols<<" "<<img.minVal<<" "<<img.maxVal;
        outFile1<<"\n";
        
        outFile2.open(argv[3]);
        outFile2<<img.numRows<<" "<<img.numCols<<" "<<img.minVal<<" "<<img.maxVal;
        outFile2<<"\n";

        img.processing(inFile, outFile1, outFile2, img.thrValue);
        inFile.close();
        outFile1.close();
        outFile2.close();

    }catch(exception e) {cout<<e.what();}
    
    return 0;
}
