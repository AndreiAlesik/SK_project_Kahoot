package org.example.userinterface;

import javax.swing.*;

public class Panel extends JPanel {

    public Panel(int x, int y, int width, int height){
        this.setBounds(x, y, width, height);
        this.setLayout(null);
    }

    public void showNames(){
        this.removeAll();
        if(AppSettings.userNames.size() != 0){
            int i = 0;
            for(String user : AppSettings.userNames){
                Text temp = new Text(user, 20, i * 30, Math.round(AppSettings.width / 2) - 40,  Math.round(AppSettings.height / 18));
                this.add(temp);
                i++;
            }
        }
        this.revalidate();
        this.repaint();
    }
}
