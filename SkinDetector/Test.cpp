#include "SkinDetector.h"
#include <cstdio>

int main()
{
	//trainBayesModel("E:\\pic_skindetect\\result\\data\\pic_names.dat", "E:\\pic_skindetect\\result\\data\\bayestest.model");
	loadBayesModel("E:\\pic_skindetect\\result\\data\\bayestest.model");
	printf("%lf\n", detectSkinPic("E:\\pic_skindetect\\result\\SkinTestData\\skin\\1000132_3457920088890516805.jpg", 1.0));
	printf("%lf\n", detectSkinPic("E:\\pic_skindetect\\result\\SkinTestData\\skin\\1002231_1713901133191641591.jpg", 1.0));
	printf("%lf\n", detectSkinPic("E:\\pic_skindetect\\result\\SkinTestData\\skin\\1002231_1715589983051933333.jpg", 1.0));
	printf("%lf\n", detectSkinPic("E:\\pic_skindetect\\result\\SkinTestData\\skin\\1002231_1982146785996939136.jpg", 1.0));
	printf("%lf\n", detectSkinPic("E:\\pic_skindetect\\result\\SkinTestData\\skin\\1002231_2008323958830783228.jpg", 1.0));
	printf("%lf\n", detectSkinPic("E:\\pic_skindetect\\result\\SkinTestData\\skin\\1002231_2856971013613630646.jpg", 1.0));
}