package com.netease.space.antispam.skindetect;

/**
 * @author tony
 */
public class SkinDetectorDemo {

    /**
     * Demo main entry.
     * 
     * @param args
     *            arguments of main.
     */
    public static void main(String[] args) {
        SkinDetector sd = new SkinDetectorImpl("E:/pic_skindetect/result/data");
        sd.trainBayesModel("E:/pic_skindetect/result/SkinTrainData");
    }

}
