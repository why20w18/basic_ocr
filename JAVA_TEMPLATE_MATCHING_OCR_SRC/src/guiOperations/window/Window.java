/*
    1-pencere acmali
    2-pencere uzerinde JPanel baslangic olarak disaridan parametre olarak verilerek olusturulmali
    3-JPanel icersinde BufferImage islenmeli yine islemeyide Window yapmali
*/
package guiOperations.window;

import javax.swing.JFrame;
import javax.swing.JPanel;

/**
 *
 * @author archw
 */
public class Window {
    private JFrame window;
    private JPanel panel;
    
    private String title;
    private int width;
    private int height;
    
    public Window(int width , int height , String title , JPanel panel){
        window = new JFrame(title);
        window.setSize(width,height);
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        ////////////
        this.panel = panel;
        this.title = title;
        this.width = width;
        this.height = height;
        ////////////
        window.add(panel);
        window.revalidate();
        window.repaint();
        window.setVisible(true);
    }
}
