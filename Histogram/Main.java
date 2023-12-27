import java.io.*;
import java.util.*;

class thresholdSelection{
    int numRows, numCols, minVal, maxVal;
    int[] histAry, GaussAry;
    int BigGaussThrVal;
    int maxHeight;

    thresholdSelection(){
        numRows=0;
        numCols=0;
        maxVal=0;
        minVal=0;
        histAry = new int[maxVal+1];
        GaussAry = new int[maxVal+1];
        BigGaussThrVal=0;
        maxHeight=0;
        Arrays.fill(histAry, 0);
        Arrays.fill(GaussAry, 0);
    }

    int loadHist(int[] Ary, Scanner inFile){
        int max=0, i=0;
        while(inFile.hasNextInt()){
            i = inFile.nextInt();
            Ary[i]= inFile.nextInt();
            if(Ary[i]>max)
                max=Ary[i];
        }

        return max;
    }

    void dispHist(int[] histAray, FileWriter outFile1) throws Exception{
        outFile1.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
        for(int i=0; i<histAray.length; i++){
            outFile1.write("("+i+"): ");
            for(int j=0; j<histAray[i]; j++){
                outFile1.write("+");
            }
            outFile1.write("\n");
        }
    }

    void setZero(int[] histAry){
        Arrays.fill(histAry, 0);
    }

    int bigGaussian(int[] histAry, int[] GaussAry, int maxHeight, int minVal, int maxVal, FileWriter deBugFile)throws Exception{
        deBugFile.write("Entering BigGaussian method\n");
        double sum1, sum2, total, minSumDiff;
        int offSet= (maxVal-minVal)/10;
        int dividePt=offSet;
        int bestThr = dividePt;
        minSumDiff = 99999.0;
        while(dividePt<(maxVal-offSet)) {
            setZero(GaussAry);
            sum1 = fitGauss(0, dividePt, histAry, GaussAry, deBugFile);
            sum2 = fitGauss(dividePt, maxVal, histAry, GaussAry, deBugFile);
            total = sum1 + sum2;
            if (total < minSumDiff) {
                minSumDiff = total;
                bestThr = dividePt;
            }

            deBugFile.write(sum1 + " " + sum2 + " " + total + " " + minSumDiff + " " + bestThr+"\n");
            dividePt++;
        }
        deBugFile.write("Leaving bigGuassian method, minSumDiff = "+minSumDiff+" bestThr is "+bestThr+"\n");

        return bestThr;
    }

    double fitGauss (int leftIndex, int rightIndex, int[] histAry, int[] GaussAry, FileWriter deBugFile)throws Exception{
        deBugFile.write("Entering fitGauss method\n");
        double mean, var, sum=0.0, Gval, maxGval;
        mean = computeMean(leftIndex, rightIndex, maxHeight, histAry, deBugFile);
        var = computeVar(leftIndex, rightIndex, mean, histAry, deBugFile);
        int index = leftIndex;
        do{
            Gval = modifiedGauss(index, mean, var, maxHeight);
            sum += Math.abs(Gval - (double) histAry[index]);
            GaussAry[index] = (int) Gval;
            index++;
        }while(index<=rightIndex);
            deBugFile.write("Leaving fitGauss method, sum is; "+sum+"\n");
        return sum;

    }

    double  computeMean (int leftIndex, int rightIndex, int maxHeight, int[] histAry, FileWriter deBugFile)throws Exception{
        deBugFile.write("Entering computeMean method\n");
        maxHeight = 0;
        int sum = 0;
        int numPixles = 0;
        int index = leftIndex;
        while(index<rightIndex) {
            sum += (histAry[index] * index);
            numPixles += histAry[index];
            if (histAry[index] > maxHeight)
                maxHeight = histAry[index];
            index++;
        }
        double result = (double) sum/(double) numPixles;
        deBugFile.write("Leaving computeMean method maxHeight is an result "+maxHeight+" "+result+"\n");

        return result;
    }

    double computeVar (int leftIndex, int rightIndex, double mean, int[] histAry, FileWriter deBugFile)throws Exception{
        deBugFile.write("Entering computeVar method\n");
        double sum = 0.0;
        int numPixles = 0;
        int index = leftIndex;
        while (index<rightIndex) {
            sum += (double) histAry[index] * (((double) index - mean) * ((double) index - mean));
            numPixles += histAry[index];
            index++;
        }
        double result = sum / (double) numPixles;
        deBugFile.write("Leaving computeVar method returning result "+result+"\n");
        return result;
    }

    double modifiedGauss (int x, double mean, double var, int maxHeight){
        return (double) ((double)maxHeight * Math.exp( - ( Math.pow(((double) x - mean),2)/ (2*var) )));
    }
}

public class Main {
    public static void main(String[] args) {
        try {
            Scanner inFile = new Scanner(new FileReader(args[0]));
            FileWriter outFile1 = new FileWriter(args[1]);
            FileWriter deBugFile = new FileWriter(args[2]);

            thresholdSelection threshold = new thresholdSelection();
            threshold.numRows = inFile.nextInt();
            threshold.numCols = inFile.nextInt();
            threshold.minVal = inFile.nextInt();
            threshold.maxVal = inFile.nextInt();

            threshold.histAry = new int[threshold.maxVal+1];
            threshold.setZero(threshold.histAry);

            threshold.maxHeight = threshold.loadHist(threshold.histAry, inFile);

            threshold.GaussAry = new int[threshold.maxVal+1];
            threshold.setZero(threshold.GaussAry);

            threshold.dispHist(threshold.histAry, outFile1);
            threshold.BigGaussThrVal = threshold.bigGaussian(threshold.histAry, threshold.GaussAry, threshold.maxHeight, threshold.minVal, threshold.maxVal, deBugFile);
            outFile1.write("The threshold value is: "+threshold.BigGaussThrVal+"\n");


            inFile.close();
            outFile1.close();
            deBugFile.close();

        }catch (Exception e){
            System.out.println(e.getMessage());
        }

    }
}