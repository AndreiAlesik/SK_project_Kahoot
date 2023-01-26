package org.example.userinterface;

import javax.swing.*;
import java.awt.*;

public class GUI {
    public JFrame frame;

    public GUI(){
        frame = new JFrame();
        frame.setPreferredSize(new Dimension(Setup.width, Setup.height));
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setTitle(Setup.title);
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
