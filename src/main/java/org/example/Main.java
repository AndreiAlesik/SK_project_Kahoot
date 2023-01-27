package org.example;

import org.example.userinterface.Setup;
import org.example.userinterface.GUI;
import org.example.userinterface.Menu;

public class Main {
    public static void main(String[] args) {


        if(args.length < 1){
            System.out.println("Podaj adres serwera");
        }
        else{
            Setup.serverAddress = args[0];
        GUI window = new GUI();
        new Menu(window);
        }
    }
}
