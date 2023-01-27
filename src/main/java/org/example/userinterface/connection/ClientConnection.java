package org.example.userinterface.connection;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.math.BigInteger;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.List;

public class ClientConnection {
    static
    Socket socket;
    InputStream is;
    String address;
    int port;
    Boolean errorFlag;
    int messageSize;
    String nowReaded;

    static String scoreResult;

    public ClientConnection(String address, int port) throws IOException {
        this.address = address;
        this.port = port;
        this.errorFlag = false;
        this.messageSize = -1;
        try {
            socket = new Socket();
            socket.connect(new InetSocketAddress(address, port), 5000);

            this.nowReaded = "";
        } catch (IOException e) {
            throw new IOException();
        }
    }

    public void sendData(String data) throws IOException {
        socket.getOutputStream().write(ByteBuffer.allocate(4).putInt(data.length()).array());
        socket.getOutputStream().write(data.getBytes());
    }

    public String getData() throws IOException {
        String result;

        while (true) {
            if (messageSize < 0) {
                if (nowReaded.length() > 4) {
                    messageSize = new BigInteger(nowReaded.substring(0, 4).getBytes()).intValue();
                    nowReaded = nowReaded.substring(4);
                }
            }
            if (messageSize > 0 && messageSize <= nowReaded.length()) {
                result = nowReaded.substring(0, messageSize);
                nowReaded = nowReaded.substring(messageSize);
                messageSize = -1;
                break;
            }
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            this.is = socket.getInputStream();
            byte[] bytearr = new byte[512];
            int len = is.read(bytearr);
            try{
                baos.write(bytearr, 0, len);
                this.nowReaded += String.valueOf(baos);
            }catch(IndexOutOfBoundsException e){
                throw new IndexOutOfBoundsException();
            }

            if (len < 1)
                throw new IOException();
        }
        if(result.contains("score")){
            try {
                List<String> tmp= Arrays.asList(result.split("\\\\"));
                scoreResult=tmp.get(tmp.size()-1);
            }
            catch (Exception e){
                System.out.println(e);
            }
        }
        return result;
    }
}