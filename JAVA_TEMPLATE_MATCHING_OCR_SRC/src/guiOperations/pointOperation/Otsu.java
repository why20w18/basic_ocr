/*
    OTSU THRESHOLDING:
    1-otsu esik degeri kendisi cekerek background ve foreground'tu en optimal sekilde ayirir
    2-siniflar arasi varyansi maximize eden T degeri bizim optimal T degerimizdir
    3-CDF'in son degeri bize total pixel sayisini verir
    
    ALGORITMA:
    -1-grayscale olarak goruntuyu okuma
    -2-grayscale histogrami cikartma
    -3-grayscale icin CDF hesaplamasi yapilmali CDF icinde Prob==Olasik hesaplanmalidir
    -4-grayscale CDF hesaplamasi == cumulative distrubition function
    -6-background ve foregorund yogunluklarinin hesaplanmasi 
    -7-backgorund ve foreground arasi varyansin hesaplanmasi=> (7)
    -8-optimal esik degerini bulma                          => (8)
    -9-goruntuye uygulama islemi 
    
*/
package guiOperations.pointOperation;

import guiOperations.rw.ReadWrite;
import java.awt.Color;

/**
 *
 * @author bw
 */
public class Otsu extends ReadWrite{
    private int [] grayscaleHistogram;
    private int totalPixel;
    
    public Otsu(String imagePath) {
        super(imagePath,ReadWrite.setModeGrayscale()); // -1-
    
        grayscaleHistogram = new int[256];
        totalPixel = image.getWidth() * image.getHeight();
    }

    @Override
    public void applyPointOperation(boolean isLogOpen) {
        calcGrayscaleHistogram();
        double[] grayscaleCDF = calcGrayscaleCDF(grayscaleHistogram);
        int optimalThreshold = findOptimalThreshold(grayscaleCDF, grayscaleHistogram);
        applyThresholdOperation(optimalThreshold, 255);
    }
    
    
    private void calcGrayscaleHistogram(){ //   -2-
        for(int i = 0 ; i < image.getWidth() ; i++){
            for(int j = 0 ; j < image.getHeight() ; j++){
                Color currentPixel = new Color(image.getRGB(i, j));
                grayscaleHistogram[currentPixel.getRed()]++;
            }
        }
    }
    
    //Prob hesaplamasi => currentPixel/totalPixel
    private double [] calcGrayscaleProb(int [] grayscaleHist){ //   -3- =>grayscale histogramin problarini hesaplayip geri cevirecek
                                                                //       =>CDF 'de kullanilmak uzere
       double grayProb[] = new double[256];
       for(int i = 0 ; i < 256 ; i++){
           grayProb[i] = (double)grayscaleHist[i] / totalPixel;
       }
       
       return grayProb;
    }
    
    private double[] calcGrayscaleCDF(int[] grayscaleHist){ // -4-
        double[] grayCDF = new double[256];
        double[] grayProb = calcGrayscaleProb(grayscaleHist);
        
        grayCDF[0] = grayProb[0];
        for(int i = 1 ; i < 256 ; i++)
            grayCDF[i] = grayCDF[i-1] + grayProb[i];
        
        return grayCDF;
    }
    
    private double calcMean(int start , int stop , int [] grayHist){    // -6-
        double currentChannelSum = 0.0;
        double currentChannelTotal = 0.0;
        
        for(int i = start ; i <= stop ; i++){
            currentChannelSum += i * grayHist[i];
            currentChannelTotal += grayHist[i];
        }
        
        return (currentChannelSum == 0) ? 0 : (currentChannelSum/currentChannelTotal);
    }
    
    
    private double calcBackgrounForegroundVariance(int T, double[] grayscaleCDF, int[] grayscaleHistogram){
        double P0 = grayscaleCDF[T];    //background prob
        double P1 = 1 - P0;             //foreground prob

        //ortalamalari hesaplama
        double mean0 = calcMean(0, T, grayscaleHistogram);      
        double mean1 = calcMean(T + 1, 255, grayscaleHistogram); 

        //varyans
        double interClassVariance = P0 * P1 * Math.pow(mean0 - mean1, 2);
        return interClassVariance;
        
        //bunu baska fonksiyon uzerinden cagiracagiz surekli
    }
    
    //-9- => 6,7,8,9 buralarda kullanilmak uzere yazildi
    private int findOptimalThreshold(double[] grayscaleCDF, int[] grayscaleHistogram) {
    double maxVariance = -1;
    int optimalThreshold = 0;
    
    for (int T = 0; T < 256; T++) {
        double interClassVariance = calcBackgrounForegroundVariance(T, grayscaleCDF, grayscaleHistogram);
        if (interClassVariance > maxVariance) {
            maxVariance = interClassVariance;
            optimalThreshold = T;
           }
     }
    
    return optimalThreshold;
    }
    
    private void applyThresholdOperation(int threshold, int maxValue) {
        for (int i = 0; i < image.getWidth(); i++) {
            for (int j = 0; j < image.getHeight(); j++) {
                Color currentPixel = new Color(image.getRGB(i, j));
                int grayValue = currentPixel.getRed();
                int newGrayValue = (grayValue >= threshold) ? maxValue : 0;
                Color thresholdedPixel = new Color(newGrayValue, newGrayValue, newGrayValue);
                image.setRGB(i, j, thresholdedPixel.getRGB());
            }
        }
    }
    
}
