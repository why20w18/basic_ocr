/*
    1-Histogram hesaplama           +
    2-Pi degerlerini hesaplama      +
    3-CDF fonksiyonu                +
    4-yeni pixel degeri             +



*/
package guiOperations.pointOperation;

import guiOperations.rw.ReadWrite;
import java.awt.Color;

/**
 *
 * @author archw
 */

enum HIST_EQUAL_MODE{
    NORMALIZED,
    BASIC,
}


public class HistogramEqualization extends ReadWrite{
    private int redHistogram[];
    private int greenHistogram[];
    private int blueHistogram[];
    
    private int redEqualization[];
    private int greenEqualization[];
    private int blueEqualization[];
    
    private int totalPixel;
    private HIST_EQUAL_MODE mode_normalized_basic;
    private static int basicR;
    private static int basicG;
    private static int basicB;
    
    public HistogramEqualization(String imagePath,HIST_EQUAL_MODE mode_normalized_basic) {
        super(imagePath);
        this.redHistogram = new int[256];
        this.greenHistogram = new int[256];
        this.blueHistogram = new int[256];
        
        this.redEqualization = new int[256];
        this.greenEqualization = new int[256];
        this.blueEqualization = new int[256];
        
        this.totalPixel = image.getWidth() * image.getHeight();
        this.mode_normalized_basic = mode_normalized_basic;
        
        
    }
    
    public static HIST_EQUAL_MODE setModeNormalized(){
        return HIST_EQUAL_MODE.NORMALIZED;
    }
    
    public static HIST_EQUAL_MODE setModeBasic(int R , int G , int B){
        if(R < 0) R = Math.abs(R);
        if(G < 0) G = Math.abs(G);
        if(B < 0) B = Math.abs(B);
        
        basicR = R;
        basicG = G;
        basicB = B;
        return HIST_EQUAL_MODE.BASIC;
    }
    

    @Override
    public void applyPointOperation(boolean isLogOpen) {
        calculateHistogram();
        calculateEqualization(this.mode_normalized_basic);
        applyHistogramEqualization(isLogOpen);     
    }
    
    
    
    
    
    private void calculateHistogram(){
        for(int i = 0 ; i < image.getWidth() ; i++){
            for(int j = 0 ; j < image.getHeight() ; j++){
                Color currentPixel = new Color(image.getRGB(i, j));
               
                this.redHistogram[currentPixel.getRed()]++;
                this.greenHistogram[currentPixel.getGreen()]++;
                this.blueHistogram[currentPixel.getBlue()]++;
            }
        }
    }
    
    
    
    
    
    //burada CDF hesaplamak icin her kanali tek tek aliyoruz
    private double [] getCDF(int histogram[]){
        double cdf[] = new double[256];
        double prob[] = new double[256];
        
        for(int i = 0 ; i < 256 ; i++){
            prob[i] = (double)histogram[i] / totalPixel;
          //  if(i % 30 == 0)
          //        System.out.println(prob[i]);
        }
                
        
        cdf[0] = prob[0];
        for(int i = 1 ; i < 256 ; i++){
            cdf[i] = cdf[i-1] + prob[i];
        }
        
        return cdf;
    }
    
    private double getMin(double arr[]){
        double result = arr[0];
        for(int i = 0 ; i < arr.length ; i++){
            if(result > arr[i])
                result = arr[i];
        }
        return result;
    }
    
    private void calculateEqualization(HIST_EQUAL_MODE mode_normalized_basic){
        //tum kanallarin cdfleri hesaplandi
        double redCDF[] = getCDF(redHistogram);
        double greenCDF[] = getCDF(greenHistogram);
        double blueCDF[] = getCDF(blueHistogram);
        
        //NORMALIZASYON
        if(mode_normalized_basic == HIST_EQUAL_MODE.NORMALIZED){
            double redMin = getMin(redCDF);
            double greenMin = getMin(greenCDF);
            double blueMin = getMin(blueCDF);
            
            for(int i = 0 ; i < 256 ; i++){
                redEqualization[i] = setScaleRange((int)(((redCDF[i] - redMin))/(totalPixel-redMin) *255), 0, 255);
                greenEqualization[i] = setScaleRange( (int)(((greenCDF[i] - greenMin))/(totalPixel-greenMin) *255), 0, 255);
                blueEqualization[i] = setScaleRange((int)(((blueCDF[i] - blueMin))/(totalPixel-blueMin) *255), 0, 255);
            }
        }
        else if(mode_normalized_basic == HIST_EQUAL_MODE.BASIC){
            for(int i = 0 ; i < 256 ; i++){
                redEqualization[i] = setScaleRange((int) (((redCDF[i] * 255))/redCDF[255-basicR]), 0, 255);
                greenEqualization[i] = setScaleRange((int) (((greenCDF[i] * 255))/greenCDF[255-basicG]), 0, 255);
                blueEqualization[i] = setScaleRange((int) (((blueCDF[i] * 255))/blueCDF[255-basicB]), 0, 255);
            }
        }
    }
    
    
    
    private void applyHistogramEqualization(boolean  isLogOpen){
        for(int i = 0 ; i < image.getWidth() ; i++){
            for(int j = 0 ; j < image.getHeight() ; j++){
                Color currentPixel = new Color(image.getRGB(i, j));
                
                int eqRed = redEqualization[currentPixel.getRed()];
                int eqGreen = greenEqualization[currentPixel.getGreen()];
                int eqBlue = blueEqualization[currentPixel.getBlue()];
                
                Color eqPixel = new Color(eqRed,eqGreen,eqBlue);
                image.setRGB(i, j, eqPixel.getRGB());
            }
        }
    }
    
    
    
 
    
    
 
    
}
