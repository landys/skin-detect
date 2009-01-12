package com.netease.space.antispam.skindetect;

/**
 * @author tony
 */
public interface SkinDetector {

    boolean trainBayesModel(final String path);

    
}
