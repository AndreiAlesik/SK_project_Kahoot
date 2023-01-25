package org.example.question;

import org.json.JSONObject;

public class Answer {
    private String content;
    private Boolean isCorrect;

    public String getContent() {
        return content;
    }

    public Boolean getCorrect() {
        return isCorrect;
    }

    public Answer(String content, Boolean isCorrect){
        this.content = content;
        this.isCorrect = isCorrect;
    }

    public JSONObject addToJSON() {
        JSONObject joAnswer = new JSONObject();
        joAnswer.put("question", getContent());
        joAnswer.put("isCorrect", getCorrect());
        return joAnswer;
    }
}
