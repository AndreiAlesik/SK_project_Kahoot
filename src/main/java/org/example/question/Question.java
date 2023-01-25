package org.example.question;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.List;

public class Question {
    private String content;
    private List<Answer> answers;

    public String getContent() {
        return content;
    }

    public List<Answer> getAnswers() {
        return answers;
    }

    public Question(String content, List<Answer> answers){
        this.content = content;
        this.answers = answers;
    }

    public JSONObject addToJSON() {
        JSONObject joQuestion = new JSONObject();
        JSONArray jaAnswers = new JSONArray();
        joQuestion.put("question", getContent());
        for(int i = 0; i < 4; i++){
            jaAnswers.put(answers.get(i).addToJSON());
        }
        joQuestion.put("answers", jaAnswers);
        return joQuestion;
    }
}