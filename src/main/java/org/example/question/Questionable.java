package org.example.question;

import java.io.IOException;

public interface Questionable {
    Question addQuestion(String questionContent, String [] answersContent);
    void sendAnswers() throws IOException;
}

