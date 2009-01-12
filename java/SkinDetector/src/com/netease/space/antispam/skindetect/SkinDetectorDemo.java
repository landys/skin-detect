package com.netease.space.antispam.skindetect;

import java.io.FileInputStream;
import java.io.FileNotFoundException;

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
     * @throws FileNotFoundException
     *             throws when file not found.
     */
    public static void main(String[] args) throws FileNotFoundException {
        SkinDetector sd = new SkinDetectorImpl();

        double re = sd.detectSkinPic(new FileInputStream(
            "E:\\pic_skindetect\\result\\data\\test.JPG"));
        System.out.println(re);
    }

}
