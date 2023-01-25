package org.example.userinterface;

import javax.swing.*;
import javax.swing.text.DefaultFormatterFactory;
import javax.swing.text.InternationalFormatter;
import java.text.NumberFormat;

public class InputNumeric extends JFormattedTextField {

    public InputNumeric(int defaultValue, int x, int y, int width, int height, int minValue, int maxValue){
        NumberFormat nf = NumberFormat.getNumberInstance();
        nf.setMinimumIntegerDigits(1);
        nf.setMaximumIntegerDigits(2);
        InternationalFormatter formatter = new InternationalFormatter(nf);
        formatter.setMinimum(minValue);
        formatter.setMaximum(maxValue);
        this.setValue(defaultValue);
        this.setEditable(true);
        this.setFormatterFactory(new DefaultFormatterFactory(formatter));
        this.setHorizontalAlignment(SwingConstants.CENTER);
        this.setBounds(x, y, width, height);
    }
}
