/*
    THRESHOLD BEKLENTISI:
    1-tum fotografi gezmeli esik degerle 
    2-max degerle kiyasa girmeli buna gore bir sonuc cikartmali
    3-threshold modlari eklenebilir zeroto_inv , threshold_inv gibi

 */
package guiOperations.pointOperation;

import guiOperations.rw.ReadWrite;
import java.awt.Color;
import javax.swing.JPanel;

/**
 *
 * @author archw
 */

enum IMAGE_MODE{
    IMAGE_SRGB,
    IMAGE_GRAYSCALE
}

public class Threshold extends ReadWrite{
    private int threshold;
    private int maxVal;
    private IMAGE_MODE mode_gray_srgb;
 
    
    
    public Threshold(String imagePath,int threshold , int maxVal,IMAGE_MODE mode_gray_srgb) {
        super(imagePath);
        this.threshold = threshold;
        this.maxVal = maxVal;
        this.mode_gray_srgb = mode_gray_srgb;
        
    }
    
    public static IMAGE_MODE setThresholdModeSRGB(){
        return IMAGE_MODE.IMAGE_SRGB;
    }
    
    public static IMAGE_MODE setThresholdModeGrayscale(){
        return IMAGE_MODE.IMAGE_GRAYSCALE;
    }
    

    @Override
    public void applyPointOperation(boolean isLogOpen) {
        applyThresholdOperation(this.threshold, this.maxVal, isLogOpen,this.mode_gray_srgb);
    }
    
    
    private void applyThresholdOperation(int threshold , int maxValue , boolean isLogOpen,IMAGE_MODE mode_gray_srgb){
        for(int i = 0 ; i < image.getWidth() ; i++){
            for(int j = 0 ; j < image.getHeight() ; j++){
                Color currentPixel = new Color(image.getRGB(i, j));
                Color thresholdPixel = null;
                
                if(mode_gray_srgb == IMAGE_MODE.IMAGE_GRAYSCALE){
                    int currentGray = currentPixel.getRed();
                    currentGray = (threshold <= currentGray) ? maxValue : 0;
                    thresholdPixel = new Color(currentGray,currentGray,currentGray);
                }                                
                else if(mode_gray_srgb == IMAGE_MODE.IMAGE_SRGB){
                    int currentRed = currentPixel.getRed();
                    int currentBlue = currentPixel.getBlue();
                    int currentGreen = currentPixel.getGreen();
                                
                    currentRed = (threshold <= currentRed) ? maxValue : 0;
                    currentBlue = (threshold <= currentBlue) ? maxValue : 0;
                    currentGreen = (threshold <= currentGreen) ? maxValue : 0;

                
                    thresholdPixel = new Color(currentRed,currentGreen,currentBlue);
                }
                

                image.setRGB(i, j, thresholdPixel.getRGB());
            }
        }
        
        
        
    }
    
}
