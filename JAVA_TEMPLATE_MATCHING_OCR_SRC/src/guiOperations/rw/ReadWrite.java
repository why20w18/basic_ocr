/*
    BEKLENEN ISLEVLER:
    +ilerleyen surecte burada okuma modlari eklenebilir
    {enum icersinde,enum abstract class icinde gomulu sekilde}

 */
package guiOperations.rw;

import java.awt.Color;
import java.awt.image.BufferedImage;
import javax.swing.JPanel;
import guiOperations.IReadFace;
import guiOperations.IWriteFace;

/**
 *
 * @author bw
 */
enum IMAGE_MODE{
    MODE_RGB,
    MODE_GRAYSCALE,
}

public abstract class ReadWrite{
    private Reader imageReader;
    private Writer imageWriter;
    private JPanel panel;
    protected BufferedImage image;
    protected IMAGE_MODE imageMode;
    
    
    
    //zaten abstract instance uretilemez
    public ReadWrite(String imagePath) {
            imageReader = new Reader(imagePath,Reader.setReaderRGB());
            image = imageReader.getReadedImage();
        
            imageWriter = new Writer(image);
            panel = imageWriter.getWritedPanel();
            
            this.imageMode = IMAGE_MODE.MODE_RGB;
    }
    
    //MODLU OKUMA
    public ReadWrite(String imagePath,IMAGE_MODE imageMode){
            this.imageMode = imageMode;
            
            if(imageMode == IMAGE_MODE.MODE_RGB){
                imageReader = new Reader(imagePath,Reader.setReaderRGB());
                image = imageReader.getReadedImage();

                imageWriter = new Writer(image);
                panel = imageWriter.getWritedPanel();    
           }
            else if(imageMode == IMAGE_MODE.MODE_GRAYSCALE){
                imageReader = new Reader(imagePath,Reader.setReaderGrayscale());
                image = imageReader.getReadedImage();
        
                imageWriter = new Writer(image);
                panel = imageWriter.getWritedPanel();  
            }       
    }
    
    public static IMAGE_MODE setModeRGB(){
        return IMAGE_MODE.MODE_RGB;
    }
    
    public static IMAGE_MODE setModeGrayscale(){
        return IMAGE_MODE.MODE_GRAYSCALE;
    }
    
    public BufferedImage getReadedImage(){
        return this.image;
    }
  
    public JPanel getWritedPanel(){
        return this.panel;
    }
  
    
    public int setScaleRange(int currentPixelValue , int minRange , int maxRange){
        return Math.min(maxRange, Math.max(minRange, currentPixelValue));
    }
    
     protected void clearImage(){
        Color newColor = new Color(255,255,255);
        
        for(int i = 0 ; i < image.getWidth() ; i++){
            for(int j = 0 ; j < image.getHeight() ; j++){
                this.image.setRGB(i, j, newColor.getRGB());
                
            }
        }
    }
    
    //ilerleyen surecte burada okuma modlari eklenebilir
    public abstract void applyPointOperation(boolean isLogOpen);
    
    
    
}
