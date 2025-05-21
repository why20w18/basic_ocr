/*


*/
package main;


import guiOperations.pointOperation.HistogramEqualization;
import guiOperations.pointOperation.Otsu;
import guiOperations.pointOperation.Threshold;

import guiOperations.rw.ReadWrite;
import guiOperations.rw.Reader;
import guiOperations.rw.Writer;
import guiOperations.segment.Projection;
import guiOperations.window.Window;
import java.awt.List;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import javax.imageio.ImageIO;
import javax.swing.JPanel;
import javax.swing.text.Segment;

/**
 *
 * @author archw
 */
public class Main {
    public static void main(String[] args) /*throws IOException*/ {    
        String currentImagePath = "src/images/metin.png";
        final String imagePath_dejavuMono1 = "src/images/text4.bmp";
        final String imagePath_dejavuMono2 = "src/images/text3.bmp";
        final String templateBMP = "src/images/template.bmp";

        
        currentImagePath = imagePath_dejavuMono2;

        final int thresholdVal = 200;

        Reader imgReader = new Reader(currentImagePath, Reader.setReaderGrayscale());
        BufferedImage image = imgReader.getReadedImage();
        
        Writer imgWriter = new Writer(image);
        JPanel imagePanel = imgWriter.getWritedPanel();
        //Window org_img_window = new Window(imgReader.getImageWidth(), imgReader.getImageHeight(), "org_img_window", imagePanel);
        
        Threshold threshold = new Threshold(currentImagePath,thresholdVal,255,Threshold.setThresholdModeGrayscale());
        threshold.applyPointOperation(false);
        BufferedImage thresholdImage = threshold.getReadedImage();
        JPanel thresholdPanel = threshold.getWritedPanel();
        Window threshold_img_window = new Window(imgReader.getImageWidth()+100, imgReader.getImageHeight()+100, "threshold_img_window", thresholdPanel);
        
        //segmentasyon
        Projection hv_projection = new Projection(thresholdImage);
        int[][] Mat2D = hv_projection.getMat2D();
        ArrayList<int[]> startEndRow_Y = hv_projection.horizontalProjection(Mat2D);
        System.out.println("TESPIT EDILEN SATIR SAYISI :  " + startEndRow_Y.size());
        
        HashMap<Character, BufferedImage> fontTemplates = new HashMap<>();
        String templateFolder = "/home/archw/NetBeansProjects/textRe/src/segmented";
        char[] letters ={
            'a','b','c','d','e','f','g','h','i','j',
            'k','l','m','n','o','p','q','r','s','t',
            'u','v','w','x','y','z','A', 'B', 'C' ,'D' ,'E' ,'F' ,'G' ,'H' ,'I' ,'J',
            'K', 'L' ,'M' ,'N','O' ,'P' ,'Q' ,'R' ,'S' ,'T' ,'U' ,'V' ,'W'  ,'Y' ,'Z' //X yok
        };
        
        for(char ch : letters){
            Threshold th = new Threshold(templateFolder+ "/"  +ch+".png", thresholdVal, 255, Threshold.setThresholdModeGrayscale());
            th.applyPointOperation(false);
            BufferedImage img  = th.getReadedImage();
            fontTemplates.put(ch, img);
            System.out.println(templateFolder + "/" +ch+".png");
        }
        
        int charCount = 0;
        StringBuilder text = new StringBuilder();
        
        for(int[] line : startEndRow_Y){
            int startY = line[0];
            int endY = line[1];
            ArrayList<int[]> characters = hv_projection.verticalProjection(Mat2D, startY, endY);
            //System.out.println("size -> " + characters.size());
            
            for(int[] ch : characters){
                int startX = ch[0];
                int endX = ch[1];
                BufferedImage croppedChar = hv_projection.cropImage(thresholdImage, startX, endX, startY, endY);
                BufferedImage resizedChar = hv_projection.resizeImage(croppedChar, 28, 28);
                
                char predictedChar = hv_projection.findBestMatch(resizedChar, fontTemplates);
                System.out.println("Char " + charCount + ": " + predictedChar);
                text.append(predictedChar);
                
//                 File outputfile = new File("segmented/char_" + charCount + ".png");
//                 ImageIO.write(resizedChar, "png", outputfile);

                Writer w = new Writer(resizedChar);
                JPanel p = w.getWritedPanel();
                Window win = new Window(200,200, "Char " + charCount, p);
                charCount++;
            }
        }
        System.out.println("TOPLAM KARAKTER SAYISI : " + charCount);
        System.out.println("TAHMIN : " + text.toString());
        



    }
}
