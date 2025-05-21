/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package guiOperations.segment;

import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.HashMap;

/**
 *
 * @author archw
 */
public class Projection {
    private int[][] Mat2D;
    private int thresholdVal;
    
    public Projection(BufferedImage image){
        this.Mat2D = convertTo2D(image);
    }
    
    public int[][] getMat2D(){
        return this.Mat2D;
    }
    
    
    private int[][] convertTo2D(BufferedImage image){
        int width = image.getWidth();
        int height = image.getHeight();
          
        int[][] Mat2D = new int[height][width];
        
        //0.satirdaki tum pikseller -> 0'dan satirin hepsi
        for(int y = 0 ; y < height ; y++)
            for(int x = 0 ; x < width ; x++){
                int pixel = image.getRGB(x, y) & 0xFF;
                Mat2D[y][x] = (pixel < 128) ? 1 : 0;
            }
        
/*
        System.out.println("debug mat2d");
        for(int y = 0; y < 10; y++){
            for(int x = 0; x < 10; x++){
                System.out.print(Mat2D[y][x] + " ");
            }
            System.out.println();      
        }
 */
        
        
        return Mat2D;
    }
    
    public ArrayList<int[]>  horizontalProjection(int [][] Mat2D){    //satirda siyah piksellerin baslangic ve bitisleri
        ArrayList<int[]> startEndRow = new ArrayList<>();
        
        int imgWidth = Mat2D[0].length;
        int imgHeight = Mat2D.length;
        int startY = 0;
        boolean inText = false;
        
        for(int y = 0 ; y < imgHeight ; y++){
            int rowTotal = 0;
            //satirdaki tum siyahlarin toplami (satirda karakter olup olmadigini gosterecek)
            for(int x = 0 ; x < imgWidth ; x++){
                rowTotal += Mat2D[y][x];
            }
            
            if(rowTotal > 0 && !inText){ //karakter bulunmustur
               startY = y;
               inText = true;
            }
            else if(rowTotal == 0 && inText){
                inText = false;
                startEndRow.add(new int[]{startY,y-1}); //baslangic,bitis
            }
            else{
                //System.out.println("MEVCUT SATIR BOS");
            }
        }
        
        return startEndRow;
    }
   
    public ArrayList<int[]> verticalProjection(int [][] Mat2D , int startY , int endY){
        ArrayList<int[]> chars = new ArrayList<>();
        int width = Mat2D[0].length;

        boolean inChar = false;
        int startX = 0;

        for(int x = 0; x < width; x++){
            int colSum = 0;
            for(int y = startY; y <= endY; y++){
                colSum += Mat2D[y][x];
            }

            if(colSum > 0 && !inChar){
                inChar = true;
                startX = x;
            } 
            else if(colSum == 0 && inChar){
                inChar = false;
                chars.add(new int[]{startX,x-1});
            }
            
        }

        return chars;
    }
    
public BufferedImage cropImage(BufferedImage img, int startX, int endX, int startY, int endY){
    int w = endX -startX +1;
    int h = endY -startY +1;
    return img.getSubimage(startX,startY,w,h);
}

public BufferedImage resizeImage(BufferedImage originalImage, int width, int height) {
    Image scaled = originalImage.getScaledInstance(width, height, Image.SCALE_SMOOTH);
    BufferedImage resized = new BufferedImage(width, height, BufferedImage.TYPE_BYTE_GRAY);
    Graphics2D g2d = resized.createGraphics();
    g2d.drawImage(scaled, 0, 0, null);
    g2d.dispose();
    return resized;
}


public double compareImages(BufferedImage img1, BufferedImage img2){
    int width = img1.getWidth();
    int height = img1.getHeight();
    int totalPixel = width * height;
    int diffCount = 0;

    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            int pixel1 = img1.getRGB(x,y) & 0xFF;
            int pixel2 = img2.getRGB(x,y) & 0xFF;

            if(pixel1 != pixel2) diffCount++;
        }
    }
    double diff = (double)diffCount / totalPixel;
    //      2/5 -> 1-0.04 -> 0.96
    return 1.0 - diff;
}


public char findBestMatch(BufferedImage charImg, HashMap<Character, BufferedImage> templates){
    char bestChar = '_';
    double bestScore = -1;

    for(Character ch : templates.keySet()){
        double score = compareImages(charImg, templates.get(ch));
        if(score > bestScore){
            bestScore = score;
            bestChar = ch;
        }
    }

    return bestChar;
}

    

}
