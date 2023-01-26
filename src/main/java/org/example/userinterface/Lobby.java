package org.example.userinterface;


import org.example.userinterface.connection.ReadThread;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.ArrayList;

public class Lobby extends Thread implements ActionListener {
    private GUI window;
    private Text gameNameText;
    private Text usersListTitleText;
    private Boolean isAdmin;
    private Text gameId;
    private Text questionTime;
    private Text questionQuantity;
    private Button startGame;

    public Lobby(GUI window, Boolean isAdmin){
        int width = Setup.width;
        int height = Setup.height;
        this.window = window;
        this.isAdmin = isAdmin;

        Setup.userPanel = new Panel(0, (Setup.height / 4), (Setup.width / 2), (Setup.height / 2) - 50);

        if(isAdmin || Setup.userNames == null)
            Setup.userNames = new ArrayList<>();

        window.frame.getContentPane().removeAll();
        window.frame.repaint();

        window.frame.add(Setup.userPanel);

        gameNameText = new Text(Setup.gameJSON.getGameName(), 0, (height / 15), width, (height / 18));

        usersListTitleText = new Text("Użytkownicy:", 0, (height / 6), (width / 2), (height / 18));

        gameId = new Text("Id gry: "+ Setup.gameId, (width / 2) + 20, (height / 6) + 10, (width / 2) - 40, 50, Color.BLACK);

        questionQuantity = new Text("Ilość pytań: " + Setup.gameJSON.getQuestionQuantity(), (width / 2) + 20, (height / 6) + 70, (width / 2) - 40, 50, Color.BLACK);

        questionTime = new Text("Czas na pytanie: " + Setup.gameJSON.getQuestionTime(), (width / 2) + 20, (height / 6) + 130, (width / 2) - 40, 50, Color.BLACK);

        window.frame.add(gameNameText);
        window.frame.add(usersListTitleText);
        window.frame.add(gameId);
        window.frame.add(questionQuantity);
        window.frame.add(questionTime);

        if(isAdmin){
            startGame = new Button("Rozpocznij grę", (width / 4), (height / 3) + 300, (width / 2), (height / 12));
            startGame.addActionListener(this);
            window.frame.add(startGame);

            try {
                Setup.teacherJSON = Setup.gameJSON.addJSON(Setup.questionList);
            }
            catch (Exception e) {
                System.out.println("Problem z obsługą JSON");
            }
        }

        window.reload();

        Setup.t1 = new ReadThread(window);

        Setup.t1.start();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getSource() == startGame){
            try {
                Setup.cl.sendData("\\start_game\\id\\"+ Setup.gameId);
                Setup.isGameStarted = true;
            } catch (IOException ex) {
                System.out.println("Problem z wysłaniem danych");
            }
        }
    }
}
