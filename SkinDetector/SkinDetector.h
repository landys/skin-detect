#ifdef WIN32
// picNameFile - contains picture file names, one in a line.
// modelFile - the Bayes model file name output.
// return the number of pictures successfully trained.
extern "C" __declspec(dllexport) int trainBayesModel(const char* picNameFile, const char* modelFile);

// modelFile - the Bayes model file name output.
// return 0 if no error happened, else error code.
extern "C" __declspec(dllexport) int loadBayesModel(const char* modelFile);

// picFile - the picture file name to be detected.
// limit - the limit used in the detect
// return the percent skin pixels of all pixels.
extern "C" __declspec(dllexport) double detectSkinPic(const char* picFile, const double limit);
#else
extern "C" int trainBayesModel(const char* picNameFile, const char* modelFile);
extern "C" int loadBayesModel(const char* modelFile);
extern "C" double detectSkinPic(const char* picFile, const double limit);
#endif