package org.example.userinterface;

import org.example.game.Game;
import org.example.question.Question;
import org.example.userinterface.connection.ClientConnection;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.List;

public class AppSettings {
    public static final int width = 800;
    public static final int height = 800;
    public static final String title = "Kahoot";
    public static JSONObject teacherJSON;
    public static Game gameJSON;
    public static int answerCreationQuantity;
    public static List<Question> questionList;
    public static ClientConnection cl;
    public static String gameId;
    public static String myName;
    public static List<String> userNames = null;
    public static Panel userPanel;
    public static int questionToEnd;
    public static int currentQuestion;
    public static HashMap<String, Integer> rankingMap;
    public static boolean isAdmin = false;
    public static boolean isGameStarted = false;

    public static Thread t1;
    public static String serverAddress;
}