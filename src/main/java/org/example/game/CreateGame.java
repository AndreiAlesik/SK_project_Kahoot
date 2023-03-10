package org.example.game;

import org.example.question.AddQuestion;
import org.example.userinterface.*;
import org.example.userinterface.connection.ClientConnection;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.ArrayList;

public class CreateGame implements ActionListener {
    GUI window;
    Button nextStepButton;
    InputText gameNameInput;
    Text gameNameLabel;
    InputNumeric questionsQuantityNumeric;
    Text questionsQuantityLabel;
    InputNumeric timeNumeric;
    Text timeLabel;
    Text errorMessage;

    public CreateGame(GUI window){
        int width = Setup.width;
        int height = Setup.height;
        this.window = window;

        window.frame.getContentPane().removeAll();
        window.frame.repaint();

        Setup.questionList = new ArrayList<>();


        gameNameInput = new InputText((width / 4), (height / 7), (width / 2), 50);
        gameNameLabel = new Text("Nazwa gry", 0, (height / 12), width, (height / 18));


        questionsQuantityNumeric = new InputNumeric(1, (width / 2) - 25, (height / 7) + 160, 50, 50, 1, 10);
        questionsQuantityLabel = new Text("Ilość pytań", 0, (height/12) + 150, width, (height/18));

        timeNumeric = new InputNumeric(10, (width / 2) - 25, (height / 7) + 300, 50, 50, 10, 99);
        timeLabel = new Text("Czas na pytanie", 0, (height/12) + 300, width, (height/18));

        nextStepButton = new Button("Dalej", (width / 4), (height / 3) + 300, (width / 2), (height / 12));
        nextStepButton.addActionListener(this);

        errorMessage = new Text("Podałeś błędne dane", 0, (height / 3) + 320 + (height / 12), Setup.width, (height/18));
        errorMessage.setVisible(false);

        window.frame.add(gameNameInput);
        window.frame.add(gameNameLabel);
        window.frame.add(questionsQuantityNumeric);
        window.frame.add(questionsQuantityLabel);
        window.frame.add(timeNumeric);
        window.frame.add(timeLabel);
        window.frame.add(nextStepButton);
        window.frame.add(errorMessage);

        window.reload();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getSource() == nextStepButton){
            String name = gameNameInput.getText();
            int quantity = (int) questionsQuantityNumeric.getValue();
            int time = (int) timeNumeric.getValue();
            if(!name.isEmpty() && quantity >= 1 && quantity <= 10 && time >= 10 && time <= 99){
                Setup.gameJSON = new Game(name, quantity, time);
                Setup.answerCreationQuantity = quantity;
                Setup.isAdmin = true;
                try {
                    Setup.cl = new ClientConnection(Setup.serverAddress, 5050);
                    Setup.cl.sendData("\\create_game\\"+name+"\\quantity\\"+quantity+"\\time\\"+time);
                    Setup.gameId = Setup.cl.getData();
                    Setup.questionToEnd = quantity;
                    new AddQuestion(window, quantity);
                } catch (IOException ex) {
                    errorMessage.setText("Błąd połączenia");
                    errorMessage.setVisible(true);
                }
            }
            else{
                errorMessage.setText("Błędne dane");
                errorMessage.setVisible(true);
            }

        }
    }
}

