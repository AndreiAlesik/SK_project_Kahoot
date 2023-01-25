package org.example.userinterface;

import javax.swing.*;
import java.awt.*;
import java.util.Objects;

public class Button extends JButton {
    ImageIcon icon;
    public Button(String text, int x, int y, int width, int height){
        this.setText(text);
        this.setBounds(x, y, width, height);
    }


    public Button(String text, int x, int y, int width, int height, String shape, Boolean isAdmin){
        if(isAdmin){
            switch (shape) {
                case "star":
                    icon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/main/resources/star-32.png")));
                    this.setBackground(Color.CYAN);
                    break;
                case "triangle":
                    icon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/main/resources/triangle-32.png")));
                    this.setBackground(Color.RED);
                    break;
                case "circle":
                    icon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/main/resources/circle-32.png")));
                    this.setBackground(Color.YELLOW);
                    break;
                case "square":
                    icon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/main/resources/square-32.png")));
                    this.setBackground(Color.GREEN);
                    break;
            }
            this.setHorizontalAlignment(SwingConstants.LEFT);
        }
        else{
            switch (shape) {
                case "star":
                    icon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/main/resources/star-48.png")));
                    this.setBackground(Color.CYAN);
                    break;
                case "triangle":
                    icon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/main/resources/triangle-48.png")));
                    this.setBackground(Color.RED);
                    break;
                case "circle":
                    icon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/main/resources/circle-48.png")));
                    this.setBackground(Color.YELLOW);
                    break;
                case "square":
                    icon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/main/resources/square-48.png")));
                    this.setBackground(Color.GREEN);
                    break;
            }
        }
        this.setIcon(icon);
        this.setText(text);
        this.setHorizontalTextPosition(JButton.RIGHT);
        this.setVerticalTextPosition(JButton.CENTER);
        this.setIconTextGap(15);
        this.setBounds(x, y, width, height);
        this.setFocusable(false);
    }

    public void changeToGrey(){
        this.setBackground(Color.GRAY);
    }
}

