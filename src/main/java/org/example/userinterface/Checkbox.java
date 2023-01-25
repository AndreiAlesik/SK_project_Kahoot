package org.example.userinterface;

import javax.swing.*;
import java.util.Objects;

public class Checkbox extends JCheckBox {


    ImageIcon checkIcon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/main/resources/check.png")));


    ImageIcon closeIcon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/main/resources/close.png")));

    public Checkbox(int x, int y){
        this.setFocusable(false);
        this.setBounds(x, y, 40, 32);
        this.setIcon(closeIcon);
        this.setSelectedIcon(checkIcon);
    }
}
