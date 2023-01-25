package org.example.game;


import org.example.userinterface.*;
import org.example.userinterface.Button;
import org.example.userinterface.Menu;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.*;
import java.util.List;

public class ScoreBoard implements ActionListener {

    GUI window;
    Text scoreBoardTitle;
    Button toMenu;

    public ScoreBoard(GUI window){
        int width = AppSettings.width;
        int height = AppSettings.height;
        this.window = window;

        window.frame.getContentPane().removeAll();
        window.frame.repaint();

        Map<String, Integer> sorted = sortByValue(AppSettings.rankingMap);

        scoreBoardTitle = new Text("Tablica wyników", (width / 2) - 250, 75, 500, 100, Color.BLACK, 45);

        int i = 0;
        for(Map.Entry<String, Integer> en: sorted.entrySet()){
            Text textUser = new Text(en.getKey(), (width / 2) - 250, 175 + i * 30, 450, 30, Color.BLACK);
            Text scoreUser = new Text(String.valueOf(en.getValue()), (width / 2) + 200, 175 + i * 30, 50, 30, Color.BLACK);
            window.frame.add(textUser);
            window.frame.add(scoreUser);
            i++;
        }

        toMenu = new Button("Wróć do menu", (width / 4), (height / 3) + 300, (width / 2), (height / 12));
        toMenu.addActionListener(this);

        window.frame.add(scoreBoardTitle);
        window.frame.add(toMenu);


        window.reload();
    }

    public static HashMap<String, Integer> sortByValue(HashMap<String, Integer> hm)
    {
        // Create a list from elements of HashMap
        List<Map.Entry<String, Integer> > list =
                new LinkedList<>(hm.entrySet());

        // Sort the list
        list.sort((o1, o2) -> (o2.getValue()).compareTo(o1.getValue()));

        // put data from sorted list to hashmap
        HashMap<String, Integer> temp = new LinkedHashMap<>();
        for (Map.Entry<String, Integer> aa : list) {
            temp.put(aa.getKey(), aa.getValue());
        }
        return temp;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getSource() == toMenu){
            new Menu(window);
        }
    }
}
