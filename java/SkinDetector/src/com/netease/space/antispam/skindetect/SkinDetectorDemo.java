package com.netease.space.antispam.skindetect;

import java.io.FileInputStream;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;

/**
 * SkinDetector demo.
 * 
 * @author tony
 */
public class SkinDetectorDemo {

    /**
     * Demo main entry.
     * 
     * @param args
     *            arguments of main.
     * @throws Exception
     *             throws when an error happened.
     */
    public static void main(String[] args) throws Exception {
        // System.setProperty("proxySet", "true");
        // System.setProperty("proxyHost", "127.0.0.1");
        // System.setProperty("proxyPort", "8888");

        SkinDetector sd = new SkinDetectorImpl();

        InputStream in = null;
        double re = 0.0;

        in = new FileInputStream(
            "E:\\pic_skindetect\\result\\SkinTestData\\head\\807638_3724476891835759610.jpg");
        re = sd.detectSkinPic(in);
        System.out.println(re);
        in.close();

        URL url = new URL(
            "http://www.av199.net/softs/dvd/h00/h05/img200511251655413.jpg");

        URLConnection con = url.openConnection();

        in = con.getInputStream();
        re = sd.detectSkinPic(in);
        System.out.println(re);
        in.close();
    }

}
