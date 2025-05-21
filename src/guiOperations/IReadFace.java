/*
    BEKLENEN ISLEVSELLIK: Reader
    1-verilen imagePath uzerinden okuma yapacak
    2-BufferImage nesnesine bu resmi aktaracak
    3-goruntuyu geri cevirecek bir fonksiyona sahip olacak

*/

package guiOperations;

import java.awt.image.BufferedImage;
import javax.swing.JPanel;

/**
 *
 * @author bw
 */
public interface IReadFace {
    
    public abstract void readImage();
    public abstract BufferedImage getReadedImage();
    
}
