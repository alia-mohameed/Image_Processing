import java.io.*;
import java.util.*;

import static java.util.Arrays.sort;

class Property{
    int label;
    int numPixels;
    int minR, minC, maxR, maxC;

    public Property(int label, int numPixels, int minR, int minC, int maxR, int maxC) {
        this.label = label;
        this.numPixels = numPixels;
        this.minR = minR;
        this.minC = minC;
        this.maxR = maxR;
        this.maxC = maxC;
    }

}

class ccLabel{
    int numRows, numCols, minVal, maxVal;
    int newLabel;
    int trueNumCC;
    int newMin;
    int newMax;
    int[][] zeroFramedAry;
    int[] NonZeroNeighborAry;
    int[] EQAry;
    char option;
    Property[] CCproperty;

    public ccLabel(int numRows, int numCols, int minVal, int maxVal, int trueNumCC, char option) {
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        this.newLabel = 0;
        this.trueNumCC = trueNumCC;
        this.newMin = 0;
        this.newMax = trueNumCC;
        this.zeroFramedAry = new int[numRows + 2][numCols + 2];
        this.NonZeroNeighborAry = new int[5];
        this.EQAry = new int[(numRows * numCols) / 4];
        this.option = option;
        this.CCproperty = new Property[trueNumCC+1];
    }

    public void zero2D(int[][] Ary){
        for(int i=0; i<Ary.length; i++){
            for(int j=0; j< Ary[i].length; j++){
                Ary[i][j] = 0;
            }
        }
    }

    public void negative1D(int[] Ary){
        Arrays.fill(Ary, -1);
    }

    public void loadImage(int[][] zeroFramedAry, Scanner inFile){
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                zeroFramedAry[i][j] = inFile.nextInt();
            }
        }
    }

    void printImg(int[][] zeroFramedAry, FileWriter labelFile)throws Exception{
        for(int i=0; i<zeroFramedAry.length; i++){
            for(int j=0; j<zeroFramedAry[i].length; j++){
                labelFile.write(zeroFramedAry[i][j]+" ");
            }
            labelFile.write("\n");
        }
    }

    void reformatPrettyPrint(int[][]  zeroFramedAry, FileWriter REprettyPrintFile)throws Exception{
        REprettyPrintFile.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
        String str = Integer.toString(maxVal);
        int Width = str.length();
        int WW=0;
        int r = 2;
        do{
            int c = 2;
            do{
                REprettyPrintFile.write(String.valueOf(zeroFramedAry[r][c]));
                str = Integer.toString(zeroFramedAry[r][c]);
                WW = str.length();
                do{
                    REprettyPrintFile.write(".");
                    WW++;
                }while(WW<Width);
                c++;
            }while(c<numCols+2);
            REprettyPrintFile.write("\n");
            r++;
        }while(r<numRows+2);
    }

    void connect8Pass1(int[][] zeroFramedAry, int newLabel, int[] EQAry){
        int p =0;
        for(int i=2; i<numRows+2; i++){
            for(int j=2; j<numCols+2; j++){
                p = zeroFramedAry[i][j];

                if(p>0){
                    if(zeroFramedAry[i - 1][j - 1] == 0 && zeroFramedAry[i - 1][j] == 0 && zeroFramedAry[i - 1][j + 1] == 0 && zeroFramedAry[i][j - 1] == 0){
                        newLabel++;
                        p = newLabel;
                        //update EQTable
                        updateEQ(EQAry, zeroFramedAry, i, j, 4, 1);
                    }else if(zeroFramedAry[i - 1][j] == zeroFramedAry[i - 1][j - 1] && zeroFramedAry[i - 1][j + 1] == zeroFramedAry[i - 1][j - 1] && zeroFramedAry[i][j - 1] == zeroFramedAry[i - 1][j - 1]){
                        p = zeroFramedAry[i - 1][j - 1];
                    }else if(zeroFramedAry[i - 1][j] != zeroFramedAry[i - 1][j - 1] || zeroFramedAry[i - 1][j + 1] != zeroFramedAry[i - 1][j - 1] || zeroFramedAry[i][j - 1] != zeroFramedAry[i - 1][j - 1]){
                        int [] Ary = new int[]{zeroFramedAry[i - 1][j - 1], zeroFramedAry[i - 1][j], zeroFramedAry[i - 1][j + 1], zeroFramedAry[i][j - 1]};
                        Arrays.sort(Ary);
                        p = Ary[0];
                        //update EQTable
                        updateEQ(EQAry, zeroFramedAry, i, j, 8, 1);
                    }
                }
                zeroFramedAry[i][j] = p;
            }
        }
    }

    void connect4Pass1(int[][] zeroFramedAry, int newLabel, int[] EQAry){
        int p=0;
        for(int i=2; i<numRows+2; i++){
            for(int j=2; j<numCols+2; j++){
                p= zeroFramedAry[i][j];
                if(p>0){
                    if(zeroFramedAry[i - 1][j] == 0 && zeroFramedAry[i][j - 1] == 0){
                        newLabel++;
                        p = newLabel;
                        //update EQTable
                        updateEQ(EQAry, zeroFramedAry, i, j, 4, 1);
                    }else if(zeroFramedAry[i - 1][j] == zeroFramedAry[i][j - 1]){
                        p = zeroFramedAry[i - 1][j];
                    }else if(zeroFramedAry[i - 1][j] != zeroFramedAry[i][j - 1]){
                        p = Math.min(zeroFramedAry[i - 1][j], zeroFramedAry[i][j - 1]);
                        //update EQTable
                        updateEQ(EQAry, zeroFramedAry, i, j, 4, 1);
                    }
                }
                zeroFramedAry[i][j] = p;
            }
        }
    }

    void connect8Pass2(int[][] zeroFramedAry,int[] EQAry){
        int p =0;
        for(int i=numRows; i>0; i--){
            for(int j=numCols; j>0; j--){
                p = zeroFramedAry[i][j];

                if(p>0){
                    if(zeroFramedAry[i - 1][j - 1] == 0 && zeroFramedAry[i - 1][j] == 0 && zeroFramedAry[i - 1][j + 1] == 0 && zeroFramedAry[i][j - 1] == 0){
                        p = zeroFramedAry[i][j];
                    }else if(zeroFramedAry[i - 1][j - 1] == p && zeroFramedAry[i - 1][j] == p && zeroFramedAry[i - 1][j + 1] == p && zeroFramedAry[i][j - 1] == p){
                        p = zeroFramedAry[i][j];
                    }else if(zeroFramedAry[i - 1][j] != zeroFramedAry[i - 1][j - 1] || zeroFramedAry[i - 1][j + 1] != zeroFramedAry[i - 1][j - 1] || zeroFramedAry[i][j - 1] != zeroFramedAry[i - 1][j - 1]){
                        int [] Ary = new int[]{zeroFramedAry[i - 1][j - 1], zeroFramedAry[i - 1][j], zeroFramedAry[i - 1][j + 1], zeroFramedAry[i][j - 1]};
                        Arrays.sort(Ary);
                        p = Ary[0];
                        EQAry[p] = Math.min(zeroFramedAry[i - 1][j], zeroFramedAry[i][j - 1]);
                        updateEQ(EQAry, zeroFramedAry, i, j, 8, 2);
                    }
                }
                zeroFramedAry[i][j] = p;
            }
        }
    }

    void connect4Pass2(int[][] zeroFramedAry, int[] EQAry){
        int p=0;
        for(int i=numRows; i>0; i--){
            for(int j=2; j>0; j--){
                p= zeroFramedAry[i][j];
                if(p>0){
                    if(zeroFramedAry[i - 1][j] == 0 && zeroFramedAry[i][j - 1] == 0){
                        p = zeroFramedAry[i][j];
                    }else if(zeroFramedAry[i - 1][j] == p && zeroFramedAry[i][j - 1] == p){
                        p = zeroFramedAry[i][j];
                    }else if(zeroFramedAry[i - 1][j] != zeroFramedAry[i][j - 1]){
                        p = Math.min(zeroFramedAry[i - 1][j], zeroFramedAry[i][j - 1]);
                        EQAry[p] = Math.min(zeroFramedAry[i - 1][j], zeroFramedAry[i][j - 1]);
                        //update EQTable
                        updateEQ(EQAry, zeroFramedAry, i, j, 4, 2);
                    }
                }
                zeroFramedAry[i][j] = p;
            }
        }
    }

    void connectPass3 (int[][] zeroFramedAry, int[] EQAry, Property[] CCproperty, int trueNumCC, FileWriter deBugFile) throws Exception{
        //int[][] p = new int[numRows+2][numCols+2];
        int p=0;
        deBugFile.write("entering connectPas3 method");
        for(int i=1; i<=trueNumCC; i++){
            CCproperty[i] = new Property(i, 0, numRows, numCols, 0, 0);
            CCproperty[i].label = i;
            CCproperty[i].numPixels = 0;
            CCproperty[i].minR = numRows;
            CCproperty[i].maxR = 0;
            CCproperty[i].minC = numCols;
            CCproperty[i].maxC = 0;
        }
        for(int r=0; r<numRows+2; r++){
            for(int c=0; c<numCols+2; c++){
                p = zeroFramedAry[r][c];
                if(p>0){
                    zeroFramedAry[r][c] = EQAry[p];
                    int k= zeroFramedAry[r][c];
                    CCproperty[k].numPixels++;
                    if (r < CCproperty[k].minR)
                        CCproperty[k].minR = r;
                    if (r > CCproperty[k].maxR)
                        CCproperty[k].maxR = r;
                    if (c < CCproperty[k].minC)
                        CCproperty[k].minC = c;
                    if (c > CCproperty[k].maxC)
                        CCproperty[k].maxC = c;
                }
                zeroFramedAry[r][c] = p;
            }
        }
        deBugFile.write("leavnig connectPas3 method");
    }

    void updateEQ(int[] EQAry, int[][] zeroFramedAry, int i, int j, int connectednes, int pass){
        if(connectednes == 8)
            if(pass == 1) this.NonZeroNeighborAry = new int[]{zeroFramedAry[i - 1][j - 1], zeroFramedAry[i - 1][j], zeroFramedAry[i - 1][j + 1], zeroFramedAry[i][j - 1], zeroFramedAry[i][j]};
            else if(pass == 2) this.NonZeroNeighborAry = new int[]{zeroFramedAry[i][j], zeroFramedAry[i][j+1], zeroFramedAry[i+1][j-1], zeroFramedAry[i+1][j], zeroFramedAry[i+1][j+1]};
        if(connectednes == 4)
            if(pass == 1) this.NonZeroNeighborAry = new int[]{zeroFramedAry[i - 1][j], zeroFramedAry[i][j - 1], zeroFramedAry[i][j]};
            else if(pass == 2) this.NonZeroNeighborAry = new int[]{zeroFramedAry[i][j], zeroFramedAry[i][j+1], zeroFramedAry[i+1][j]};
        Arrays.sort(this.NonZeroNeighborAry);
        int minLable = this.NonZeroNeighborAry[0];
        Arrays.fill(EQAry, minLable);
    }

    int manageEQAry(int[] EQAry, int newLabel){
        int readLable = 0;
        int index = 1;
        do{
            if(index != EQAry[index])
                EQAry[index] = EQAry[EQAry[index]];
            else {
                readLable++;
                EQAry[index] = readLable;
            }
            index++;
        }while(index>newLabel);
        Arrays.sort(EQAry);

        return EQAry[EQAry.length-1];
    }

    void printCCproperty(FileWriter propertyFile, Property[] CCproperty)throws Exception{
        propertyFile.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
        propertyFile.write(trueNumCC+"\n");
        for (Property property : CCproperty) {
            if(property!=null) {
                propertyFile.write(property.label + "\n");
                propertyFile.write(property.numPixels + "\n");
                propertyFile.write("upperLeftR " + property.minR + ", upperLeftC" + property.minC + "\n");
                propertyFile.write("LowerRgtR " + property.maxR + ", LowerRgtC" + property.maxC + "\n");
            }
        }
    }

    void printEQAry(int newLabel, FileWriter RFprettyPrintFile) throws Exception{
        for(int i=0; i<=newLabel; i++) {
            RFprettyPrintFile.write(String.valueOf(EQAry[i])+" ");
        }
    }

    void  drawBoxes (int[][] zeroFramedAry, Property[] CCproperty, int trueNumCC, FileWriter deBugFile)throws Exception{
        deBugFile.write("entering drawBoxes method\n");
        int index=1;
        do{
            int minRow = CCproperty[index].minR + 1;
            int minCol = CCproperty[index].minC + 1;
            int maxRow = CCproperty[index].maxR + 1;
            int maxCol = CCproperty[index].maxC + 1;
            int label = CCproperty[index].label;
            for(int i: zeroFramedAry[minRow])
                i = label;
            for(int i: zeroFramedAry[maxRow])
                i = label;
            for(int i: zeroFramedAry[minCol])
                i = label;
            for(int i: zeroFramedAry[maxCol])
                i = label;
            index++;
        }while(index<=trueNumCC);
        deBugFile.write("leaving drawBoxes method");
    }

    void connected4 (int[][] zeroFramedAry, int newLabel, int[] EQAry, FileWriter RFprettyPrintFile, FileWriter deBugFile)throws Exception{
        deBugFile.write("entering connected4 method\n");
        connect4Pass1(zeroFramedAry, newLabel, EQAry);
        deBugFile.write("After connected4 pass1, newLabel = "+newLabel+"\n");
        reformatPrettyPrint(zeroFramedAry, RFprettyPrintFile);
        printEQAry(newLabel, RFprettyPrintFile);

        connect4Pass2(zeroFramedAry, EQAry);
        deBugFile.write("After connected4 pass2, newLabel= "+newLabel+"\n");
        reformatPrettyPrint(zeroFramedAry, RFprettyPrintFile);
        printEQAry(newLabel, RFprettyPrintFile);

        trueNumCC = manageEQAry(EQAry, newLabel);
        printEQAry(newLabel, RFprettyPrintFile);
        int newMin = 0;
        int newMax = trueNumCC;
        CCproperty = new Property[trueNumCC+1];
        deBugFile.write("In connected4, after manage EQAry, trueNumCC="+trueNumCC+"\n");

        connectPass3(zeroFramedAry, EQAry, CCproperty, trueNumCC, deBugFile);
        reformatPrettyPrint(zeroFramedAry, RFprettyPrintFile);
        printEQAry(newLabel, RFprettyPrintFile);
        deBugFile.write("leaving connected4 method\n");

    }

    void connected8(int[][] zeroFramedAry, int newLabel, int[] EQAry, FileWriter RFprettyPrintFile, FileWriter deBugFile)throws  Exception{
        deBugFile.write("entering connected8 method\n");
        connect8Pass1(zeroFramedAry, newLabel, EQAry);
        deBugFile.write("After connected8 pass1, newLabel = "+newLabel+"\n");
        reformatPrettyPrint(zeroFramedAry, RFprettyPrintFile);
        printEQAry(newLabel, RFprettyPrintFile);

        connect8Pass2(zeroFramedAry, EQAry);
        deBugFile.write("After connected8 pass2, newLabel= "+newLabel+"\n");
        reformatPrettyPrint(zeroFramedAry, RFprettyPrintFile);
        printEQAry(newLabel, RFprettyPrintFile);

        trueNumCC = manageEQAry(EQAry, newLabel);
        printEQAry(newLabel, RFprettyPrintFile);
        int newMin = 0;
        int newMax = trueNumCC;
        CCproperty = new Property[trueNumCC+1];
        deBugFile.write("In connected8, after manage EQAry, trueNumCC="+trueNumCC+"\n");

        connectPass3(zeroFramedAry, EQAry, CCproperty, trueNumCC, deBugFile);
        reformatPrettyPrint(zeroFramedAry, RFprettyPrintFile);
        printEQAry(newLabel, RFprettyPrintFile);
        deBugFile.write("leaving connected8 method\n");
    }


}


public class Main {
    public static void main(String[] args) {
        try {
            Scanner inFile = new Scanner(new FileReader(args[0]));
            char Connectness = args[1].charAt(0);
            FileWriter RFprettyPrintFile = new FileWriter(args[2]);
            FileWriter labelFile = new FileWriter(args[3]);
            FileWriter propertyFile = new FileWriter(args[4]);
            FileWriter deBugFile = new FileWriter(args[5]);

            ccLabel CC = new ccLabel(inFile.nextInt(), inFile.nextInt(), inFile.nextInt(), inFile.nextInt(), 0, Connectness);
            CC.zero2D(CC.zeroFramedAry);
            CC.loadImage(CC.zeroFramedAry, inFile);
            if(Connectness == 4){
                CC.connected4(CC.zeroFramedAry, CC.newLabel, CC.EQAry, RFprettyPrintFile, deBugFile);
            }
            if(Connectness == 8){
                CC.connected8(CC.zeroFramedAry, CC.newLabel, CC.EQAry, RFprettyPrintFile, deBugFile);
            }

            labelFile.write(CC.numRows+" "+CC.numCols+" "+CC.minVal+" "+CC.maxVal+"\n");
            CC.printImg(CC.zeroFramedAry, labelFile);
            CC.printCCproperty(propertyFile, CC.CCproperty);
            //CC.drawBoxes(CC.zeroFramedAry, CC.CCproperty, CC.trueNumCC, deBugFile);
            CC.reformatPrettyPrint(CC.zeroFramedAry, RFprettyPrintFile);
            RFprettyPrintFile.write("The number of connected components is: "+CC.trueNumCC+"\n");


            inFile.close();
            RFprettyPrintFile.close();
            deBugFile.close();
            labelFile.close();
            propertyFile.close();

        }catch (Exception e){System.out.println(e.getMessage());}
    }
}