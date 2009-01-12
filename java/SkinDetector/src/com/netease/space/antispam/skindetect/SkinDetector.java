package com.netease.space.antispam.skindetect;

import java.io.InputStream;

/**
 * Skin detector.
 * 
 * @author tony
 */
public interface SkinDetector {

    /**
     * Detect the skin pixels in the picture, and return the percent of the skin
     * pixels.
     * 
     * @param jpgIn
     *            the input stream of the jpg.
     * @return the percent of skin pixels in the picture; -1.0 if error exists.
     */
    double detectSkinPic(final InputStream jpgIn);
}
