package com.netease.space.antispam.skindetect;

import java.awt.image.BufferedImage;
import java.io.DataInputStream;
import java.io.EOFException;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteOrder;

import javax.imageio.ImageIO;

/**
 * Implementation of {@link SkinDetector}.
 * 
 * @author tony
 */
public class SkinDetectorImpl implements SkinDetector {
    /**
     * skin/non-skin limit used in Bayes detector.
     */
    private double limit;

    /**
     * Bayes model for skin detecting.
     */
    private double[][] bayesModel;

    /**
     * The file should be in the classpath.
     */
    private static final String MODEL_FILE = "bayesSkin.model";

    /**
     * Cb, Cr size in YCrCb color mode.
     */
    private static final int CBCR_SIZE = 255;

    /**
     * Default skin/non-skin limit used in Bayes detector.
     */
    private static final double DEFAULT_LIMIT = 1.0;

    /**
     * Constructor with default limit value. It will load the Bayes Model file
     * for skin detecting.
     * 
     * @throws RuntimeException
     *             Any error happened.
     */
    public SkinDetectorImpl() throws RuntimeException {
        this(DEFAULT_LIMIT);
    }

    /**
     * Constructor. It will load the Bayes Model file for skin detecting.
     * 
     * @param limit
     *            skin/non-skin limit used in Bayes detector.
     * @throws RuntimeException
     *             Any error happened.
     */
    public SkinDetectorImpl(final double limit) throws RuntimeException {
        this.limit = limit;

        // Load the Bayes Model file for skin detecting.
        loadBayesModel();
    }

    /**
     * {@inheritDoc}
     */
    public double detectSkinPic(final InputStream jpgIn) {
        int totalPoints = 0;
        int skinPoints = 0;

        try {
            BufferedImage image = ImageIO.read(jpgIn);
            int width = image.getWidth();
            int height = image.getHeight();
            for (int i = 0; i < width; ++i) {
                for (int j = 0; j < height; ++j) {
                    // get r, g, b
                    int rgb = image.getRGB(i, j);
                    int r = (rgb & 0xff0000) >> 16;
                    int g = (rgb & 0xff00) >> 8;
                    int b = rgb & 0xff;

                    // calculate y, cr, cb
                    int y = (int) (0.299 * r + 0.587 * g + 0.114 * b + 0.5);
                    int cr = (int) ((r - y) * 0.713 + 128 + 0.5);
                    int cb = (int) ((b - y) * 0.564 + 128 + 0.5);

                    ++totalPoints;
                    // check if skin pixel
                    if (bayesModel[cb][cr] >= limit) {
                        ++skinPoints;
                    }
                }
            }
        } catch (IOException e) {
            // do nothing
        }

        return totalPoints == 0 ? -1.0 : (double) skinPoints / totalPoints;
    }

    /**
     * Load the Bayes Model file for detect.
     * 
     * @throws RuntimeException
     *             Any error happened.
     */
    private void loadBayesModel() throws RuntimeException {
        bayesModel = new double[CBCR_SIZE][CBCR_SIZE];
        DataInputStream in = null;
        try {
            in = new DataInputStream(this.getClass().getClassLoader()
                .getResourceAsStream(MODEL_FILE));
            for (int i = 0; i < CBCR_SIZE; ++i) {
                for (int j = 0; j < CBCR_SIZE; ++j) {
                    bayesModel[i][j] = readDoubleNativeEndian(in);
                }
            }
        } catch (FileNotFoundException e) {
            throw new RuntimeException("File not found: " + MODEL_FILE, e);
        } catch (EOFException e) {
            throw new RuntimeException("Not valid bayes model file: "
                + MODEL_FILE, e);
        } catch (IOException e) {
            throw new RuntimeException("Not valid bayes model file: "
                + MODEL_FILE, e);
        } finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    throw new RuntimeException("Error close bayes model file: "
                        + MODEL_FILE, e);
                }
            }
        }
    }

    /**
     * Read the next eight bytes of this input stream, interpreted as a double
     * according to native byte endian. If the native endian is big endian, use
     * the java way; it it's little endian, interpret according to little endian
     * rule.
     * 
     * @param in
     *            the input stream.
     * @return the next eight bytes of this input stream, interpreted as a
     *         double.
     * @throws EOFException
     *             if this input stream reaches the end before reading eight
     *             bytes.
     * @throws IOException
     *             the stream has been closed and the contained input stream
     *             does not support reading after close, or another I/O error
     *             occurs.
     */
    private double readDoubleNativeEndian(final DataInputStream in)
        throws EOFException, IOException {
        if (ByteOrder.nativeOrder() == ByteOrder.BIG_ENDIAN) {
            return in.readDouble();
        }
        // 8 bytes
        long accum = 0;
        for (int shiftBy = 0; shiftBy < 64; shiftBy += 8) {
            // must cast to long or shift done modulo 32
            accum |= (long) (in.readByte() & 0xff) << shiftBy;
        }

        return Double.longBitsToDouble(accum);
    }
}
