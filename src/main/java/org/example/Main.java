package org.example;

import org.example.userinterface.AppSettings;
import org.example.userinterface.GUI;
import org.example.userinterface.Menu;

public class Main {
    public static void main(String[] args) {


//        if(args.length < 1){
//            System.out.println("Podaj adres serwera");
//        }
//        else{
//            AppSettings.serverAddress = args[0];
        AppSettings.serverAddress = "8080";
        GUI window = new GUI();
        new Menu(window);
//        }
    }
}
