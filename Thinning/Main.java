import java.util.*;
import java.io.*;

class Thinning{
    int numRows, numCols, minVal, maxVal;
    int changeCount;
    int cycleCount;
    int[][] aryOne;
    int[][] aryTwo;
    int[] neighborAry;

    Thinning(int row, int col, int min, int max){
         numRows = row;
         numCols = col;
         minVal = min;
         maxVal = max;
         changeCount = 0;
         cycleCount = 0;
         aryOne = new int[row+2][col+2];
         aryTwo = new int[row+2][col+2];
         neighborAry = new int[9];
         for(int i=0; i<row+2; i++) {
             Arrays.fill(aryOne[i], 0);
             Arrays.fill(aryTwo[i], 0);
         }
         Arrays.fill(neighborAry, 0);
    }

    void zeroFramed(int[][] Ary){
        for(int i=0; i<Ary.length; i++){
            for(int j=0; j<Ary[i].length; j++){
                Ary[i][0] = 0;
                Ary[i][numCols+1] = 0;
                Ary[0][j] = 0;
                Ary[numRows+1][j] = 0;
            }
        }
    }

    void loadImage(Scanner inFile, int[][] aryOne){
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                aryOne[i][j] = inFile.nextInt();
            }
        }
    }

    int loadNNeighborAry(int[] neighborAry, int[][] aryOne, int i, int j){
        neighborAry[0] = aryOne[i - 1][j - 1];
        neighborAry[1] = aryOne[i - 1][j];
        neighborAry[2] = aryOne[i - 1][j + 1];
        neighborAry[3] = aryOne[i][j-1];
        neighborAry[4] = aryOne[i][j];
        neighborAry[5] = aryOne[i][j + 1];
        neighborAry[6] = aryOne[i + 1][j - 1];
        neighborAry[7] = aryOne[i + 1][j];
        neighborAry[8] = aryOne[i + 1][j + 1];

        int count=0;
        for(int k=0; k<neighborAry.length; k++){
            if(k != 4 && neighborAry[k]>0){
                count++;
            }
        }
        return count-1;
    }

    void copyArys(int[][] aryTwo, int[][] aryOne){
        for(int i=0; i<numRows+2; i++){
            if (numCols + 2 >= 0) System.arraycopy(aryTwo[i], 0, aryOne[i], 0, numCols + 2);
        }
    }

    void northThinning(int[][] aryOne, int[][] aryTwo, FileWriter deBugFile)throws Exception{
        deBugFile.write("Entering northThinning(); cycleCount=; "+cycleCount+" changeCount= "+changeCount+"\n");
        int i=1;
        int nonZeroCount=0;
        boolean Flag = true;
        do{
            int j=1;
            do{
                if(aryOne[i][j]>0 && aryOne[i-1][j]==0){
                    nonZeroCount = loadNNeighborAry(neighborAry, aryOne, i, j);
                    Flag = checkConnector(neighborAry);
                    deBugFile.write("In northThinning, i= "+i+"; j= "+j+" nonZeroCount= "+nonZeroCount+"; Flag="+Flag+"\n");
                }
                if(nonZeroCount>=4 && !Flag){
                    aryTwo[i][j] = 0;
                    changeCount++;
                }else{
                    aryTwo[i][j] = aryOne[i][j];
                }
                j++;
            }while(j<numCols+1);
            i++;
        }while(i<numRows+1);
        deBugFile.write("Leaving northThinning(); cycleCount= "+cycleCount+"; changeCount= "+changeCount+"\n");
    }

    void southThinning(int[][] aryOne, int[][] aryTwo, FileWriter deBugFile)throws Exception{
        deBugFile.write("Entering southThinning(); cycleCount=; "+cycleCount+" changeCount= "+changeCount+"\n");
        int i=1;
        int nonZeroCount = 0;
        boolean Flag = true;
        do{
            int j=1;
            do{
                if(aryOne[i][j]>0 && aryOne[i+1][j]==0){
                    nonZeroCount = loadNNeighborAry(neighborAry, aryOne, i, j);
                    Flag = checkConnector(neighborAry);
                    deBugFile.write("In southThinning, i= "+i+"; j= "+j+" nonZeroCount= "+nonZeroCount+"; Flag="+Flag+"\n");
                }
                if(nonZeroCount>=4 && !Flag){
                    aryTwo[i][j] = 0;
                    changeCount++;
                }else{
                    aryTwo[i][j] = aryOne[i][j];
                }
                j++;
            }while(j<numCols+1);
            i++;
        }while(i<numRows+1);
        deBugFile.write("Leaving southThinning(); cycleCount= "+cycleCount+"; changeCount= "+changeCount+"\n");
    }

    void westThinning(int[][] aryOne, int[][] aryTwo, FileWriter deBugFile)throws Exception{
        deBugFile.write("Entering westThinning(); cycleCount=; "+cycleCount+" changeCount= "+changeCount+"\n");
        int i=1;
        int nonZeroCount = 0;
        boolean Flag = true;
        do{
            int j=1;
            do{
                if(aryOne[i][j]>0 && aryOne[i][j-1]==0){
                    nonZeroCount = loadNNeighborAry(neighborAry, aryOne, i, j);
                    Flag = checkConnector(neighborAry);
                    deBugFile.write("In westThinning, i= "+i+"; j= "+j+" nonZeroCount= "+nonZeroCount+"; Flag="+Flag+"\n");
                }
                if(nonZeroCount>=4 && !Flag){
                    aryTwo[i][j] = 0;
                    changeCount++;
                }else{
                    aryTwo[i][j] = aryOne[i][j];
                }
                j++;
            }while(j<numCols+1);
            i++;
        }while(i<numRows+1);
        deBugFile.write("Leaving westThinning(); cycleCount= "+cycleCount+"; changeCount= "+changeCount+"\n");
    }

    void eastThinning(int[][] aryOne, int[][] aryTwo, FileWriter deBugFile)throws Exception{
        deBugFile.write("Entering eastThinning(); cycleCount=; "+cycleCount+" changeCount= "+changeCount+"\n");
        int i=1;
        int nonZeroCount = 0;
        boolean Flag = true;
        do{
            int j=1;
            do{
                if(aryOne[i][j]>0 && aryOne[i][j+1]==0){
                    nonZeroCount = loadNNeighborAry(neighborAry, aryOne, i, j);
                    Flag = checkConnector(neighborAry);
                    deBugFile.write("In eastThinning, i= "+i+"; j= "+j+" nonZeroCount= "+nonZeroCount+"; Flag="+Flag+"\n");
                }
                if(nonZeroCount>=4 && !Flag){
                    aryTwo[i][j] = 0;
                    changeCount++;
                }else{
                    aryTwo[i][j] = aryOne[i][j];
                }
                j++;
            }while(j<numCols+1);
            i++;
        }while(i<numRows+1);
        deBugFile.write("Leaving eastThinning(); cycleCount= "+cycleCount+"; changeCount= "+changeCount+"\n");
    }

    void thinning(int[][] aryOne, int[][] aryTwo, FileWriter deBugFile)throws Exception{
        deBugFile.write("Entering thinning () before thinning 4 sides, aryOne is below:\n");
        reformatPrettyPrint(aryOne, deBugFile);
        changeCount = 0;
        northThinning(aryOne, aryTwo, deBugFile);
        deBugFile.write("“after northThinning; aryTwo is below:");
        reformatPrettyPrint(aryTwo, deBugFile);
        //copyArys(aryTwo, aryOne);
        southThinning(aryOne, aryTwo, deBugFile);
        deBugFile.write("“after southThinning; aryTwo is below:");
        reformatPrettyPrint(aryTwo, deBugFile);
        //copyArys(aryTwo, aryOne);
        westThinning(aryOne, aryTwo, deBugFile);
        deBugFile.write("“after southThinning; aryTwo is below:");
        reformatPrettyPrint(aryTwo, deBugFile);
        //copyArys(aryTwo, aryOne);
        eastThinning(aryOne, aryTwo, deBugFile);
        deBugFile.write("“after southThinning; aryTwo is below:");
        reformatPrettyPrint(aryTwo, deBugFile);
        //copyArys(aryTwo, aryOne);
        deBugFile.write("Leaving thinning (); cycleCount = "+cycleCount+"; changeCount ="+changeCount+"\n");

    }

    boolean checkConnector(int[] neighborAry){
        if(neighborAry[1] == 0 && neighborAry[7] == 0){
            return true;
        }else if(neighborAry[3]==0 && neighborAry[5] == 0) {
            return true;
        }else if(neighborAry[1] == 0 && neighborAry[3] == 0 && neighborAry[0] == 1) {
            return true;
        }else if(neighborAry[1] == 0 && neighborAry[5] == 0 && neighborAry[2] == 1) {
            return true;
        }else if(neighborAry[3] == 0 && neighborAry[7] == 0 && neighborAry[6] == 1) {
            return true;
        }else if( neighborAry[5] == 0 && neighborAry[7] == 0 && neighborAry[8] == 1){
            return true;
        }else{
            return false;
        }
    }

    void imgReformat(int[][] inAry, FileWriter outFile)throws Exception{
        outFile.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
        String str = String.valueOf(maxVal);
        int Width = str.length();
        int WW=0;
        int r = 1;
        do{
            int c = 1;
            do{
                outFile.write(String.valueOf(inAry[r][c]));
                str = String.valueOf(inAry[r][c]);
                WW = str.length();
                do{
                    outFile.write(" ");
                    WW++;
                }while(WW<Width);
                c++;
            }while(c<numCols+1);
            outFile.write("\n");
            r++;
        }while(r<numRows+1);
    }

    void reformatPrettyPrint(int[][] inAry, FileWriter outFile)throws Exception{
        outFile.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
        String str = String.valueOf(maxVal);
        int Width = str.length();
        int WW=0;
        int r = 1;
        do{
            int c = 1;
            do{
                if(inAry[r][c] == 0){
                    outFile.write(".");
                }else{
                    outFile.write(String.valueOf(inAry[r][c]));
                }
                str = String.valueOf(inAry[r][c]);
                WW = str.length();
                do{
                    outFile.write(" ");
                    WW++;
                }while(WW<Width);

                c++;
            }while(c<numCols+1);
            outFile.write("\n");
            r++;
        }while(r<numRows+1);
    }
}

public class Main {
    public static void main(String[] args) {
        try {
            Scanner inFile = new Scanner(new FileReader(args[0]));
            FileWriter outFile1, deBugFile;
            outFile1 = new FileWriter(args[1]);
            deBugFile = new FileWriter(args[2]);

            int row = inFile.nextInt();
            int col = inFile.nextInt();
            int min = inFile.nextInt();
            int max = inFile.nextInt();
            Thinning thin = new Thinning(row, col, min, max);
            thin.changeCount = 0;
            thin.cycleCount = 0;

            thin.loadImage(inFile, thin.aryOne);
            outFile1.write("In main(), before thinning, changeCount ="+thin.changeCount+"; cycleCount ="+thin.cycleCount+"\n");
            thin.reformatPrettyPrint(thin.aryOne, outFile1);
            do {
                thin.thinning(thin.aryOne, thin.aryTwo, deBugFile);
                thin.cycleCount++;
                outFile1.write("In main(), inside iteration, changeCount ="+thin.changeCount+"; cycleCount ="+thin.cycleCount+"\n");
                thin.reformatPrettyPrint(thin.aryOne, outFile1);
            }while(!(thin.changeCount<=0));

            outFile1.write("In main(), after thinning, changeCount ="+thin.changeCount+"; cycleCount ="+thin.cycleCount+"\n");
            thin.imgReformat(thin.aryOne, outFile1);



            inFile.close();
            outFile1.close();
            deBugFile.close();


        }catch (Exception e){
            System.out.println(e.getMessage());
        }
    }
}