//
// opencv0.c - creating and displaying an image using Intel OpenCV
//
#include <cv.h>         // includes OpenCV definitions
#include <highgui.h>    // includes highGUI definitions
#include <stdio.h>      // includes C standard input/output definitions
#include <string>
#include <string.h>
#include <Windows.h>

using std::string;

#define BIG_DOUBLE (1e10)

int g_SkinStaticTable[255][255] = {0}; // for train process
int g_totalSkinSize = 0;  // for train process
int g_NonSkinStaticTable[255][255] = {0};  // for train process
int g_totalNonSkinSize = 0;  // for train process
double g_SkinDetector[255][255];  // for train and test process
double g_tor = 2.0; // for test process
string resultFilePath = "..\\SkinDetect\\ResultData"; // for test process, default value
FILE* resultLog; // for test process

VOID
TrainTraverse(
    const char* FileName, 
    int para )
{
    // new IplImage structure img0
    IplImage* imgRgb = NULL;
    IplImage* imgYCbCr = NULL;
    CvSize imgSize;

    imgRgb = cvLoadImage( FileName, CV_LOAD_IMAGE_COLOR );
    imgSize.width = imgRgb->width;
    imgSize.height = imgRgb->height;

    imgYCbCr = cvCreateImage(  imgSize, imgRgb->depth, imgRgb->nChannels );

    cvCvtColor( imgRgb, imgYCbCr, CV_BGR2YCrCb );

    //cvNamedWindow( "1", 1 );
    //cvShowImage( "1", imgYCbCr );

    //cvWaitKey( 0 );
    //cvDestroyWindow( "1" );

    for( int h = 0; h < imgSize.height; ++h ) {
        for ( int w = 0; w < imgSize.width * 3; w += 3 ) {
            BYTE B  = ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+0];
            BYTE G = ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+1];
            BYTE R = ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+2];

            BYTE Y  = ((PUCHAR)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+0];
            BYTE Cb = ((PUCHAR)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+1];
            BYTE Cr = ((PUCHAR)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+2];

            if (B == 255 && G == 255 && R == 255) {
                // white pixel
                continue;
            }

            if ( para == 1) {
                g_SkinStaticTable[Cr][Cb]++;
				g_totalSkinSize++;
            } else {
                g_NonSkinStaticTable[Cr][Cb]++;
				g_totalNonSkinSize++;
            }
        }
    }

    cvReleaseImage( &imgYCbCr );
    cvReleaseImage( &imgRgb );
}

VOID
ProcessTraverse(
    const char* FileName,
    int para )
{
	static int count = 0;

	string file = string(FileName);
	int i = file.rfind('\\');
	string skinFile = resultFilePath + "\\S" + file.substr(i+1);
	string modifiedFile = resultFilePath + "\\A" + file.substr(i+1);
	//printf("The result of the file is %s.\n", re.c_str());

    // new IplImage structure img0
    IplImage* imgRgb = NULL;
    IplImage* imgYCbCr = NULL;
    CvSize imgSize;

    imgRgb = cvLoadImage( FileName, CV_LOAD_IMAGE_COLOR );
    imgSize.width = imgRgb->width;
    imgSize.height = imgRgb->height;

    imgYCbCr = cvCreateImage(  imgSize, imgRgb->depth, imgRgb->nChannels );

    cvCvtColor( imgRgb, imgYCbCr, CV_BGR2YCrCb );

	int totalPoints = 0;
	int skinPoints = 0;
    for( int h = 0; h < imgSize.height; ++h ) {
        for ( int w = 0; w < imgSize.width * 3; w += 3 ) {
            BYTE Y  = ((PUCHAR)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+0];
            BYTE Cb = ((PUCHAR)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+1];
            BYTE Cr = ((PUCHAR)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+2];
			totalPoints++;
			if (g_SkinDetector[Cr][Cb] >= g_tor)
			{
				// skin color
				skinPoints++;
			}
			else
			{
				// non skin color
				((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+0] = 255;
                ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+1] = 255;
                ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+2] = 255;

				((PUCHAR)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+0] = 0;
			}
        }
    }
	// save result
	fprintf(resultLog, "%-8d%-12d%-11d%-12.3f\n", ++count, totalPoints, skinPoints, (float)skinPoints / totalPoints);
	cvSaveImage(skinFile.c_str(), imgRgb);
	//cvSaveImage(modifiedFile.c_str(), imgYCbCr);
/*
    cvNamedWindow( "1", 1 );
    cvShowImage( "1", imgRgb );

    cvWaitKey( 0 );
    cvDestroyWindow( "1" );
*/
    cvReleaseImage( &imgYCbCr );
    cvReleaseImage( &imgRgb );
}

typedef void (*ProcessCallback)( const char* fileName, int para );

void
FileTraverse(
    const string& pathPattern,
    ProcessCallback cb )
{
    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind;

    hFind = FindFirstFileA( pathPattern.c_str(), &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf ("Invalid File Handle. GetLastError reports %d\n", GetLastError ());
        return;
    } else {
        DWORD dwError;
        printf ("First file name is %s\n", FindFileData.cFileName);

		string path = "";
		int i = pathPattern.rfind('\\');
		if (i != -1)
		{
			path = pathPattern.substr(0, i+1);
		}

        do {
            printf ("processing file name is %s\n", FindFileData.cFileName);
            string filePath = FindFileData.cFileName;
            filePath = path + filePath;
            if ( FindFileData.cFileName[0] == 'S' ) {
                cb( filePath.c_str(), 1 );
            } else {
                cb( filePath.c_str(), 0 );
            }
        } while (FindNextFileA(hFind, &FindFileData) != 0);

        dwError = GetLastError();
        FindClose(hFind);
        if (dwError != ERROR_NO_MORE_FILES) 
        {
            printf ("FindNextFile error. Error is %u\n", dwError);
            return;
        }
    }

}

// use bayesian classifier to train the detector
void trainDetector(const char* resultFile)
{
	FILE* fp = fopen(resultFile, "wb");
	if (fp == 0)
	{
		printf("Cannot open file %s.\n", resultFile);
		return;
	}
	string ts = string(resultFile);
	string sFpTxt = ts + ".txt";
	string sFpTxtNoZero = ts + "_nozero.txt";
	FILE* fpTxt = fopen(sFpTxt.c_str(), "w");
	FILE* fpTxtNoZero = fopen(sFpTxtNoZero.c_str(), "w");

	double nonSkinVsSkin = double(g_totalNonSkinSize) / g_totalSkinSize;

	int i = 0;
	int j = 0;
	for (i=0; i<255; ++i)
	{
		for (j=0; j<255; ++j)
		{
			if (g_SkinStaticTable[i][j] == 0)
			{
				g_SkinDetector[i][j] = 0.0;
			}
			else if (g_NonSkinStaticTable[i][j] == 0)
			{
				g_SkinDetector[i][j] = BIG_DOUBLE;
			}
			else
			{
				g_SkinDetector[i][j] = nonSkinVsSkin * g_SkinStaticTable[i][j] / g_NonSkinStaticTable[i][j];
			}
		}
	}
	
	fwrite(g_SkinDetector, sizeof(double), 255*255, fp);
	if (fpTxt != 0 || fpTxtNoZero != 0)
	{
		for (i=0; i<255; ++i)
		{
			int f = 0;
			for (j=0; j<255; ++j)
			{
				fprintf(fpTxt, "%.3f ", g_SkinDetector[i][j]);
				if (fabs(g_SkinDetector[i][j]) > 1e-10)
				{
					f = 1;
					fprintf(fpTxtNoZero, "%.3f ", g_SkinDetector[i][j]);
				}
			}
			fprintf(fpTxt, "\n");
			if (f == 1)
			{
				fprintf(fpTxtNoZero, "\n");
			}
		}
	}

	fclose(fp);
}

int loadDetector(const char* resultFile)
{
	FILE* fp = fopen(resultFile, "rb");
	if (fp == 0)
	{
		printf("Cannot open file %s.\n", resultFile);
		return 0;
	}

	int re = 0;
	if (fread(g_SkinDetector, sizeof(double), 255*255, fp) == 255*255)
	{
		printf("Successfully load Bayesian model %s.\n", resultFile);
		re = 1;
	}
	else
	{
		printf("Fail to load Bayesian model %s.\n", resultFile);
	}

	fclose(fp);
	return re;
}

// the parameters are:
// 1-path pattern, i.e. SkinTrainData\\*.jpg
// 2-train/test selection, "1" means train, others means test
// 3-the limit value (tor) used as the bayesian limitation, it will be between 2.0 and 4.0 by experience
int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		printf("USAGE: SkinDetectBayes train pathPattern train_result_file\n      SkinDetectBayes test pathPattern train_result_file test_result_path tor_value");
		return 1;
	}

	if (strcmp("train", argv[1]) == 0)
	{
		printf("Begin train.\n");
		// train
		FileTraverse(argv[2], TrainTraverse);
		trainDetector(argv[3]);
	}
	else
	{
		// test
		printf("Begin test.\n");
		double tor = 2.0;
		if (argc >= 5)
		{
			resultFilePath = string(argv[4]);
		}

		if (argc >= 6 && sscanf(argv[5], "%lf", &tor) != 0)
		{
			g_tor = tor;
		}

		if (loadDetector(argv[3]) == 1)
		{
			string resultFileName = resultFilePath + "\\result.txt";
			resultLog = fopen(resultFileName.c_str(), "w");
			if (resultLog == 0)
			{
				printf("Cannot open file %s for write.\n", resultFileName.c_str());
				return 1;
			}
			fprintf(resultLog, "sn      totalPoints skinPoints skinPercent\n");
			FileTraverse(argv[2], ProcessTraverse);

			fclose(resultLog);
		}
	}

	printf("End.\n");
	return 0;
}
