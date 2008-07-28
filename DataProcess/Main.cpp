#include <iostream>
#include <fstream>
#include <deque>
#include <algorithm>

using namespace std;

int main()
{
	deque<double> de;

	ifstream in("E:\\projects\\SkinDetect\\codes\\trunk\\SkinDetect\\SkinBayes.model_nozero.txt");
	double d;
	while (in >> d)
	{
		de.push_back(d);
	}
	in.close();

	sort(de.begin(), de.end());

	ofstream out("E:\\projects\\SkinDetect\\codes\\trunk\\SkinDetect\\SkinBayes.model_nozero_sorted.txt");
	int count = 0;
	for (deque<double>::iterator it=de.begin(); it != de.end(); ++it)
	{
		out << *it << ' ';
		if (++count % 10 == 0)
		{
			out << endl;
		}
	}
	out.close();
	
	return 0;
}
