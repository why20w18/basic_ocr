package guiOperations.rw;

import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;
import java.awt.Color;
import org.w3c.dom.css.RGBColor;
import guiOperations.IReadFace;

/**
 *
 * @author bw
 */

enum READER_MODE{
    RGB,
    GRAYSCALE
}


public class Reader implements IReadFace{
    private BufferedImage image;
    private int imageWidth;
    private int imageHeigt;
    private String imagePath;
    private READER_MODE readerMode;
    
    
    public Reader(String imagePath,READER_MODE readerMode){
        this.imagePath = imagePath;
        this.readerMode = readerMode;
        
        if(readerMode == READER_MODE.RGB)
            readImage();
        else if(readerMode == READER_MODE.GRAYSCALE)
            readImageGrayscale();
        
    }
    
    public static READER_MODE setReaderRGB(){
        return READER_MODE.RGB;
    }
    
    public static READER_MODE setReaderGrayscale(){
        return READER_MODE.GRAYSCALE;
    }
    
    
    
    @Override
    public void readImage() {
         try{
            File input = new File(imagePath);
            image = ImageIO.read(input);
            imageWidth = image.getWidth();
            imageHeigt = image.getHeight();
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }
    
    public void readImageGrayscale() {
         try{
            File input = new File(imagePath);
            BufferedImage raw_image = ImageIO.read(input);
            imageWidth = raw_image.getWidth();
            imageHeigt = raw_image.getHeight();
            
            
            image = new BufferedImage(imageWidth, imageHeigt, BufferedImage.TYPE_BYTE_GRAY);
            
        for (int i = 0; i < imageWidth; i++) {
            for (int j = 0; j < imageHeigt; j++) {
                Color color = new Color(raw_image.getRGB(i, j));
                int red = color.getRed();
                int green = color.getGreen();
                int blue = color.getBlue();
                int gray = (int)(0.299*red + 0.587*green + 0.114*blue);
                
                Color grayColor = new Color(gray, gray, gray);
                image.setRGB(i, j, grayColor.getRGB()); 
            }
        }

            
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

    @Override
    public BufferedImage getReadedImage() {
        return this.image;
    }
    
    public int getImageWidth(){
        return this.imageWidth;
    }
    
    public int getImageHeight(){
        return this.imageHeigt;
    }
    
}
