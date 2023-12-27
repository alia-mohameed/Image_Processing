import java.io.*;
import java.util.*;


public class Main {
    public static int count=0;
    public Main(){}
    public static void processing(Scanner inFile, FileWriter outFile1, FileWriter outFile2, int thrValue){
        int numCols = 40;
        try {
            int pixelVal = inFile.nextInt();
            if (pixelVal >= thrValue) {
                outFile1.write(1+" ");
                outFile2.write(pixelVal+" ");
            }else{
                outFile1.write(0+" ");
                outFile2.write(0+" ");
            }
            count++;
            if(count ==numCols){
                outFile1.write("\n");
                outFile2.write("\n");
                count=0;
            }
        }catch (Exception e){System.out.println(e.getMessage());}
    }

    public static void main(String[] args)
    {
        try {
            Scanner inFile = new Scanner(new FileReader(args[0]));
            FileWriter outFile1 = new FileWriter(args[1]);
            FileWriter outFile2 = new FileWriter(args[2]);

            Scanner scan = new Scanner(System.in);
            System.out.print("Please enter a threshhold value (6):");
            int thrValue = scan.nextInt();

            String header = inFile.nextLine();
            outFile1.write(header+"\n");
            outFile2.write(header+"\n");

            int count=0;

            while(inFile.hasNextInt()){
                processing(inFile, outFile1, outFile2, thrValue);
            }

            inFile.close();
            outFile1.close();
            outFile2.close();

        }catch (Exception e){System.out.println(e.getMessage());}
    }
}