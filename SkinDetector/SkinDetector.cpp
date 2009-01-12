#include <cstdio>
#include <cstdlib>
#include <cv.h>         // includes OpenCV definitions
#include <highgui.h>    // includes highGUI definitions
#include "SkinDetector.h"

#define BIG_DOUBLE (1e10)
#define CBCR_SIZE (255)

// in fact CR and CB are exchanged in this file!!

int g_SkinStaticTable[CBCR_SIZE][CBCR_SIZE] = {0}; // for train process
int g_totalSkinSize = 0;  // for train process
int g_NonSkinStaticTable[CBCR_SIZE][CBCR_SIZE] = {0};  // for train process
int g_totalNonSkinSize = 0;  // for train process

double g_SkinDetector[CBCR_SIZE][CBCR_SIZE];  // for train and test process

// function declarement
void initTrainGlobal();
bool statPic(const char* FileName, bool isSkin);
bool trainDetector(const char* modelFile);

#ifdef WIN32
extern "C" __declspec(dllexport) int trainBayesModel(const char* picNameFile, const char* modelFile)
#else
extern "C" int trainBayesModel(const char* picNameFile, const char* modelFile)
#endif
{
	FILE* picSet = fopen(picNameFile, "rt");
	if (picSet == NULL)
	{
		return -1;
	}

	// initialize global data.
	initTrainGlobal();

	char buf[1024] = {'\0'};
	int cSuc = 0;
	while (fgets(buf, 1023, picSet) != NULL)
	{
		int i = 0;
		for (i=0; buf[i] != '\n' && buf[i] != '\r' && buf[i] != '\0'; ++i); // loop stop here
		for (--i; i>=0 && (buf[i] == ' ' || buf[i] == '\t'); --i); // loop stop here, eliminate the tail spaces/tabs.
		if (i < 0)
		{
			// not legal file name with only spaces/tabs.
			continue;
		}
		buf[i+1] = '\0';
		char* p = buf + i;
		char* picName = 0;
		for (picName=buf; *picName!='\0' && (*picName==' ' || *picName=='\t'); ++picName); // loop stop here, eliminate the head spaces/tabs.
		for (; p!=picName && *p!='/' && *p!='\\'; --p); // end loop
		if (*p == '/' || *p == '\\')
		{
			++p;
		}
		
		if (statPic(picName, (*p == 's' || *p == 'S')))
		{
			++cSuc;
		}
	}
	fclose(picSet);

	if (!trainDetector(modelFile))
	{
		return -2;
	}

	return cSuc;
}

#ifdef WIN32
extern "C" __declspec(dllexport) bool loadBayesModel(const char* modelFile)
#else
extern "C" int loadBayesModel(const char* modelFile)
#endif
{
	FILE* fp = fopen(modelFile, "rb");
	if (fp == 0)
	{
		printf("Cannot open file %s.\n", modelFile);
		return 1;
	}

	int re = 0;
	if (fread(g_SkinDetector, sizeof(double), CBCR_SIZE*CBCR_SIZE, fp) != CBCR_SIZE*CBCR_SIZE)
	{
		printf("Fail to load Bayesian model %s.\n", modelFile);
		re = 2;
	}

	fclose(fp);

	return re;
}

#ifdef WIN32
extern "C" __declspec(dllexport) double detectSkinPic(const char* picFile, const double limit)
#else
extern "C" double detectSkinPic(const char* picFile, const double limit)
#endif
{
	// new IplImage structure img0
	IplImage* imgRgb = NULL;
	IplImage* imgYCbCr = NULL;
	CvSize imgSize;

	imgRgb = cvLoadImage( picFile, CV_LOAD_IMAGE_COLOR );
	if(imgRgb == NULL)
	{
		fprintf(stderr, "unable to load image from %s\n", picFile);
		return 0;
	}
	imgSize.width = imgRgb->width;
	imgSize.height = imgRgb->height;

	imgYCbCr = cvCreateImage(  imgSize, imgRgb->depth, imgRgb->nChannels );

	cvCvtColor( imgRgb, imgYCbCr, CV_BGR2YCrCb );

	int totalPoints = 0;
	int skinPoints = 0;
	for( int h = 0; h < imgSize.height; ++h ) {
		for ( int w = 0; w < imgSize.width * 3; w += 3 ) {
			// in fact CR and CB are exchanged
			unsigned char Y  = ((unsigned char*)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+0];
			unsigned char Cb = ((unsigned char*)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+1];
			unsigned char Cr = ((unsigned char*)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+2];
			totalPoints++;
			if (g_SkinDetector[Cr][Cb] >= limit)
			{
				// skin color
				skinPoints++;
			}
		}
	}

	cvReleaseImage( &imgYCbCr );
	cvReleaseImage( &imgRgb );

	return (double)skinPoints / totalPoints;
}

// initialize global data for train.
void initTrainGlobal()
{
	memset(g_SkinStaticTable, 0, sizeof(int) * CBCR_SIZE * CBCR_SIZE);
	g_totalSkinSize = 0;
	memset(g_NonSkinStaticTable, 0, sizeof(int) * CBCR_SIZE * CBCR_SIZE);
	g_totalNonSkinSize = 0;
}

// do statistics for pixels in skin pic and non-skin pic.
bool statPic(const char* FileName, bool isSkin)
{
	// new IplImage structure img0
	IplImage* imgRgb = NULL;
	IplImage* imgYCbCr = NULL;
	CvSize imgSize;

	imgRgb = cvLoadImage( FileName, CV_LOAD_IMAGE_COLOR );
	if(imgRgb == NULL)
	{
		fprintf(stderr, "unable to load image from %s\n", FileName);
		return false;
	}

	imgSize.width = imgRgb->width;
	imgSize.height = imgRgb->height;

	imgYCbCr = cvCreateImage(  imgSize, imgRgb->depth, imgRgb->nChannels );

	cvCvtColor( imgRgb, imgYCbCr, CV_BGR2YCrCb );

	for( int h = 0; h < imgSize.height; ++h ) {
		for ( int w = 0; w < imgSize.width * 3; w += 3 ) {
			unsigned char B  = ((unsigned char*)(imgRgb->imageData + imgRgb->widthStep * h))[w+0];
			unsigned char G = ((unsigned char*)(imgRgb->imageData + imgRgb->widthStep * h))[w+1];
			unsigned char R = ((unsigned char*)(imgRgb->imageData + imgRgb->widthStep * h))[w+2];

			// in fact CR and CB are exchanged
			unsigned char Y  = ((unsigned char*)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+0];
			unsigned char Cb = ((unsigned char*)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+1];
			unsigned char Cr = ((unsigned char*)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+2];

			if (B == 255 && G == 255 && R == 255) {
				// white pixel
				continue;
			}

			if (isSkin) {
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

	return true;
}

// use Bayesian classifier to train the detector
bool trainDetector(const char* modelFile)
{
	FILE* fp = fopen(modelFile, "wb");
	if (fp == 0)
	{
		printf("Cannot open file %s.\n", modelFile);
		return false;
	}

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

	fwrite(g_SkinDetector, sizeof(double), CBCR_SIZE * CBCR_SIZE, fp);

	fclose(fp);
}
