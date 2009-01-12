package com.netease.space.antispam.skindetect;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FilenameFilter;
import java.io.PrintWriter;

/**
 * @author tony
 */
public class SkinDetectorImpl implements SkinDetector {
    private String curDir;

    private String picNameFile;

    private String modelFile;

    private native int trainBayesModelNative(final String picNameFile,
        final String modelFile);

    private native int loadBayesModelNative(final String modelFile);

    private native double detectSkinPicNative(final String picFile,
        final double limit);

    private static final String DEFAULT_CURDIR = "./";

    static {
        //System.loadLibrary("PicMatcherService");
    }

    public SkinDetectorImpl() {
        this(DEFAULT_CURDIR);
    }

    /**
     * @param curDir
     *            current directory for temp files.
     */
    public SkinDetectorImpl(String curDir) {
        setCurDir(curDir);

        picNameFile = this.curDir + "pic_names.dat";
        modelFile = this.curDir + "skinBayes.model";
    }

    /**
     * {@inheritDoc}
     */
    public boolean trainBayesModel(final String path) {
        if (path == null || path.trim().length() == 0) {
            System.out.println("The argument path is empty string.");
            return false;
        }

        File dir = new File(path.trim());
        if (!dir.isDirectory()) {
            System.out.println("The path is not a directory: " + path);
            return false;
        }

        File[] files = dir.listFiles(new FilenameFilter() {
            /**
             * {@inheritDoc}
             */
            public boolean accept(File dir, String name) {
                final String fileName = name.toLowerCase();
                if (fileName.endsWith("jpg") || fileName.endsWith("jpeg")) {
                    return true;
                }
                return false;
            }

        });

        PrintWriter pw = null;
        File temp = new File(picNameFile);
        try {
            pw = new PrintWriter(temp);
            for (File file : files) {
                StringBuilder sb = new StringBuilder();
                sb.append(file.getAbsolutePath())
                    .append('\n');
                pw.write(sb.toString());
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return false;
        } finally {
            if (pw != null) {
                pw.close();
            }
        }

        return true;
    }

    /**
     * @return the modelFile.
     */
    public String getModelFile() {
        return modelFile;
    }

    /**
     * @param modelFile
     *            the modelFile to set.
     */
    public void setModelFile(String modelFile) {
        this.modelFile = modelFile;
    }

    /**
     * @return the curDir.
     */
    public String getCurDir() {
        return curDir;
    }

    /**
     * @param curDir
     *            the curDir to set.
     */
    public void setCurDir(final String curDir) {
        boolean goodDir = false;
        String dir = null;
        if (curDir != null) {
            dir = curDir.replaceAll("\\\\", "/");
            final File file = new File(dir);
            if (!file.isDirectory()) {
                if (!file.exists()) {
                    if (file.mkdirs()) {
                        goodDir = true;
                    } else {
                        System.out.println("ERROR: Create directory error: "
                            + dir);
                    }
                } else {
                    System.out
                        .println("ERROR: The path exists, but not a directory: "
                            + dir);
                }
            } else {
                goodDir = true;
            }
        }

        if (goodDir) {
            this.curDir = dir;
            if (!this.curDir.endsWith("/")) {
                this.curDir += '/';
            }
        } else {
            this.curDir = DEFAULT_CURDIR;
        }
    }
}
