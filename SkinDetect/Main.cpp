//
// opencv0.c - creating and displaying an image using Intel OpenCV
//
#include "cv.h"         // includes OpenCV definitions
#include "highgui.h"    // includes highGUI definitions
#include <stdio.h>      // includes C standard input/output definitions
#include <string>
#include <Windows.h>
#include "svm.h"

#include <string>

using namespace std;

#define MAX_TRAINNING_DATA 1000000

struct svm_problem g_SvmProblem = { 0, 0 ,0 };
struct svm_model*  g_SvmModel;

int g_SkinStaticTable[255][255];
int g_NonSkinStaticTable[255][255];

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
                g_SvmProblem.y[g_SvmProblem.l] = 1;
            } else {
                g_NonSkinStaticTable[Cr][Cb]++;
                g_SvmProblem.y[g_SvmProblem.l] = -1;
            }

            g_SvmProblem.x[g_SvmProblem.l] = new svm_node[3];

            g_SvmProblem.x[g_SvmProblem.l][0].index = 1;
            g_SvmProblem.x[g_SvmProblem.l][0].value = (double)Cb / 255;
            g_SvmProblem.x[g_SvmProblem.l][1].index = 2;
            g_SvmProblem.x[g_SvmProblem.l][1].value = (double)Cr / 255;
            g_SvmProblem.x[g_SvmProblem.l][2].index = -1;

            g_SvmProblem.l++;

            if (g_SvmProblem.l >= MAX_TRAINNING_DATA) {
                printf( "Max training data set\n" );
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
	string re = string(FileName);
	int i = re.rfind('\\');
	re = "ResultData" + re.substr(i) + "_r.jpg";
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

    for( int h = 0; h < imgSize.height; ++h ) {
        for ( int w = 0; w < imgSize.width * 3; w += 3 ) {
            BYTE Y  = ((PUCHAR)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+0];
            BYTE Cb = ((PUCHAR)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+1];
            BYTE Cr = ((PUCHAR)(imgYCbCr->imageData + imgYCbCr->widthStep * h))[w+2];
#if 0
            // bayers classification
            if ( g_SkinStaticTable[Cr][Cb] == 0 ||
                 g_NonSkinStaticTable[Cr][Cb] != 0 &&
                 (double)g_SkinStaticTable[Cr][Cb] / g_NonSkinStaticTable[Cr][Cb] < (double)5 )
            {
                // non-skin color
                ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+0] = 255;
                ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+1] = 0;
                ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+2] = 0;
            }
#endif
            svm_node sn[3];
            sn[0].index = 1;
            sn[0].value = (double)Cb/255;
            sn[1].index = 2;
            sn[1].value = (double)Cr/255;
            sn[2].index = -1;

			double predict = svm_predict( g_SvmModel, sn);
			//printf("%lf\n", predict);
            if( predict <= 0.001 ) {
                // not a skin pixel
                ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+0] = 255;
                ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+1] = 0;
                ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+2] = 0;
            } else {
                // skin pixel
				/*
				((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+0] = 0;
				((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+1] = 255;
				((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * h))[w+2] = 0;
				*/
            }
        }
    }

	cvSaveImage(re.c_str(), imgRgb);
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
    const string& path,
    ProcessCallback cb )
{
    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind;

    string DirPathPattern = path + "*.jpg";

    hFind = FindFirstFileA( DirPathPattern.c_str(), &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf ("Invalid File Handle. GetLastError reports %d\n", GetLastError ());
        return;
    } else {
        DWORD dwError;
        printf ("First file name is %s\n", FindFileData.cFileName);
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


int main()
{
    g_SvmProblem.l = 0;
    g_SvmProblem.y = new double[MAX_TRAINNING_DATA];
    g_SvmProblem.x = new svm_node*[MAX_TRAINNING_DATA];

    //// new IplImage structure img0
    //IplImage* imgRgb = NULL;
    //CvSize imgSize;

    //imgSize.width = 100;
    //imgSize.height = 100;

    //imgRgb = cvCreateImage( imgSize, 8, 3 );

    //for (int i = 0; i < 100; ++i) {
    //    for ( int j = 0; j < 100 * 3; j += 3 ) {
    //        ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * i))[j+0] = 255;
    //        ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * i))[j+1] = 0;
    //        ((PUCHAR)(imgRgb->imageData + imgRgb->widthStep * i))[j+2] = 0;
    //    }
    //}

    //cvNamedWindow( "1", 1 );
    //cvShowImage( "1", imgRgb );

    //cvWaitKey( 0 );
    //cvDestroyWindow( "1" );


    FileTraverse( "SkinTrainData\\", TrainTraverse);

    g_SvmProblem.x[g_SvmProblem.l] = new svm_node[3];

    g_SvmProblem.x[g_SvmProblem.l][0].index = 1;
    g_SvmProblem.x[g_SvmProblem.l][0].value = 0;
    g_SvmProblem.x[g_SvmProblem.l][1].index = 2;
    g_SvmProblem.x[g_SvmProblem.l][1].value = 0;
    g_SvmProblem.x[g_SvmProblem.l][2].index = -1;

    g_SvmProblem.l++;

    //for (int i = 0; i < 255; ++i) {
    //    for (int j = 0; j < 255; ++j) {
    //        printf( "%d ", g_StaticTable[i][j] );
    //    }
    //    printf("\n");
    //}

    // train
    svm_parameter sp;
    sp.svm_type = C_SVC;
    sp.kernel_type = RBF;
    sp.degree = 3;
    sp.gamma = 0;	// 1/k
    sp.coef0 = 0;
    sp.nu = 0.5;
    sp.cache_size = 100;
    sp.C = 1;
    sp.eps = 1e-3;
    sp.p = 0.1;
    sp.shrinking = 1;
    sp.probability = 0;
    sp.nr_weight = 0;
    sp.weight_label = NULL;
    sp.weight = NULL;
	//sp.svm_type = NU_SVC;

    printf( "%s\n", svm_check_parameter( &g_SvmProblem, &sp ) );
    g_SvmModel = svm_train( &g_SvmProblem, &sp );

    svm_node sn[3];
    sn[0].index = 1;
    sn[0].value = 0;
    sn[1].index = 2;
    sn[1].value = 0;
    sn[2].index = -1;

    double ret = svm_predict( g_SvmModel, sn);

    svm_save_model( "Skin.model", g_SvmModel );

    FileTraverse( "TestData\\", ProcessTraverse);

    svm_destroy_model( g_SvmModel );

    delete [] g_SvmProblem.y;

	printf("%s\n", "end");
}
