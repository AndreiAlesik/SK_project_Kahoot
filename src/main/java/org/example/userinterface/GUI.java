package org.example.userinterface;

import javax.swing.*;
import java.awt.*;

public class GUI {
    public JFrame frame;

    public GUI(){
        frame = new JFrame();
        frame.setPreferredSize(new Dimension(AppSettings.width, AppSettings.height));
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setTitle(AppSettings.title);
        frame.pack();
        frame.setLayout(null);
        frame.setVisible(true);
    }

    public void reload(){
        frame.invalidate();
        frame.validate();
        frame.repaint();
    }




}
