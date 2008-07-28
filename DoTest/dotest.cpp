#include <string>
#include <cstdio>
using namespace std;

int main(int argc, char* argv[])
{
/*	char* FileName = "TestData\\abcd.jpg";
	string re = string(FileName);
	int i = re.rfind('\\');
	re = "ResultData" + re.substr(i) + "_r.jpg";
	printf("The result of the file is %s.\n", re.c_str());

	getchar();
	getchar();*/

	double a = 0.0;
	int b = 0;
	sscanf("1.4 1.4", "%lf", &a);
	sscanf("12", "%d", &b);
	printf("%f %d\n", a, b);
	return 0;
}
