package org.example.userinterface;

import javax.swing.*;

public class Panel extends JPanel {

    public Panel(int x, int y, int width, int height){
        this.setBounds(x, y, width, height);
        this.setLayout(null);
    }

    public void showNames(){
        this.removeAll();
        if(Setup.userNames.size() != 0){
            int i = 0;
            for(String user : Setup.userNames){
                Text temp = new Text(user, 20, i * 30, Math.round(Setup.width / 2) - 40,  Math.round(Setup.height / 18));
                this.add(temp);
                i++;
            }
        }
        this.revalidate();
        this.repaint();
    }
}
