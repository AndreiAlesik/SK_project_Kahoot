package org.example.game;

import org.example.question.Question;
import org.json.JSONArray;
import org.json.JSONObject;

import java.util.List;

public class Game {
    private String gameName;
    private int questionQuantity;
    private int questionTime;
    private List<Question> questions;

    public String getGameName() {
        return gameName;
    }

    public void setGameName(String gameName) {
        this.gameName = gameName;
    }

    public int getQuestionQuantity() {
        return questionQuantity;
    }

    public void setQuestionQuantity(int questionQuantity) {
        this.questionQuantity = questionQuantity;
    }

    public int getQuestionTime() {
        return questionTime;
    }

    public void setQuestionTime(int questionTime) {
        this.questionTime = questionTime;
    }

    public List<Question> getQuestions() {
        return questions;
    }
    public void setQuestions(List<Question> questions){
        this.questions = questions;
    }
    public Game(String gameName, int questionQuantity, int questionTime){
        this.gameName = gameName;
        this.questionQuantity = questionQuantity;
        this.questionTime = questionTime;
    }



    public JSONObject addJSON(List<Question> questions) throws Exception {
        JSONObject joGame = new JSONObject();
        JSONArray jaGame = new JSONArray();
        setQuestions(questions);
        joGame.put("name", this.gameName);
        joGame.put("questionQuantity", getQuestionQuantity());
        joGame.put("questionTime",getQuestionTime());
        for(int i = 0; i <getQuestionQuantity(); i++){
            jaGame.put(questions.get(i).addToJSON());
        }
        joGame.put("questions", jaGame);
        return joGame;
    }
}
