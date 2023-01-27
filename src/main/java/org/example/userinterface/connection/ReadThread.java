package org.example.userinterface.connection;


import org.example.game.QuizStudent;
import org.example.game.QuizTeacher;
import org.example.userinterface.Setup;
import org.example.userinterface.Countdown;
import org.example.userinterface.GUI;
import org.example.userinterface.Menu;

import javax.swing.*;
import java.io.IOException;
import java.util.HashMap;

import static org.example.userinterface.connection.ClientConnection.scoreResult;

public class ReadThread extends Thread{

    GUI window;

    public ReadThread(GUI window){
        this.window = window;
    }

    @Override
    public void run(){
        String command;
        boolean flag = true;

        while(flag){
            try {
                command = Setup.cl.getData();
                if(command.indexOf("\\add_user\\") == 0){
                    String user = command.substring(10);
                    Setup.userNames.add(user);
                    Setup.userPanel.showNames();
                }
                if(command.indexOf("\\delete_user\\") == 0){
                    String user = command.substring(13);
                    if(Setup.userNames.remove(user)){
                        Setup.userPanel.showNames();
                    }
                    else{
                        System.out.println("Nie ma takiego użytkownika: " + user);
                    }
                }
                if(command.indexOf("\\unreachable_host\\") == 0){
                    new Menu(window);
                    JOptionPane.showMessageDialog(null, "Host nieosiągalny", "Kahoot", JOptionPane.PLAIN_MESSAGE);
//                    AppSettings.cl.closeConnection();
                    this.join();
                }
                if(command.indexOf("\\next_question\\") == 0){
                    new QuizStudent(window);
                }
                if(command.indexOf("\\ok\\start_game\\") == 0){
                    new QuizTeacher(window, Setup.questionToEnd);
                }
                if(command.indexOf("\\error\\no_users") == 0){
                    JOptionPane.showMessageDialog(null, "Nie ma żadnych graczy", "Kahoot", JOptionPane.PLAIN_MESSAGE);
                }
                if(command.indexOf("\\end_game\\") == 0){
                    new Menu(window);
                    JOptionPane.showMessageDialog(null, "Koniec gry\nOtrzymales "+ scoreResult+" punktow", "Kahoot", JOptionPane.PLAIN_MESSAGE);
//                    AppSettings.cl.closeConnection();
                    this.join();
                }
                if(command.indexOf("\\question_end\\admin") == 0){
                    Countdown.changeWrongToGrey();
                }
                if(command.indexOf("\\question_end\\user") == 0){
                    QuizStudent.endQuestion();
                }
                if(command.indexOf("\\game_results\\") == 0){
                    Setup.rankingMap = new HashMap<>();
                    while(command.contains("\\name\\")){
                        int nameIndex = command.indexOf("\\name\\");
                        int pointsIndex = command.indexOf("\\score\\");
                        String tempCommand = command.substring(pointsIndex + 7);
                        String user = command.substring(nameIndex + 6, pointsIndex);
                        String points;

                        if(!tempCommand.contains("\\"))
                            points = command.substring(pointsIndex + 7);
                        else{
                            int tempSlash = tempCommand.indexOf("\\");
                            points = command.substring(pointsIndex + 7, pointsIndex + 7 + tempSlash);
                        }

                        Setup.rankingMap.put(user, Integer.parseInt(points));
                        command = command.substring(pointsIndex + 8);
                    }
                }


            } catch (IOException e) {
                new Menu(window);
                JOptionPane.showMessageDialog(null, "Błąd połączenia z serwerem", "Kahoot", JOptionPane.PLAIN_MESSAGE);
                flag = false;
                try {
                    this.join();
                } catch (InterruptedException ex) {
                    System.out.println("Koniec połączenia");
                }
            }
            catch(InterruptedException e){
                flag = false;
                System.out.println("Koniec połączenia");
            }
            catch(IndexOutOfBoundsException e){
                if(!Setup.isAdmin){
                    new Menu(window);
                    JOptionPane.showMessageDialog(null, "Błąd połączenia z serwerem", "Kahoot", JOptionPane.PLAIN_MESSAGE);
                }
                else if(!Setup.isGameStarted){
                    new Menu(window);
                    JOptionPane.showMessageDialog(null, "Błąd połączenia z serwerem", "Kahoot", JOptionPane.PLAIN_MESSAGE);
                }
                try {
                    flag = false;
                    this.join();
                } catch (InterruptedException ignored) {

                }
            }
        }
    }
}
