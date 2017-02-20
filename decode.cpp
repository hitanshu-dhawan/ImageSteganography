#include <iostream>

//#include <cv.h>
#include <highgui.h>
//#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

// Checks whether the bit is set or not at a particular position.
// Returns true if set
// Returns false if not set
bool isBitSet(char ch, int pos) {
	// 7 6 5 4 3 2 1 0
	ch = ch >> pos;
	if(ch & 1)
		return true;
	return false;
}

int main(int argc, char** argv) {
	
	/*
	./encode image.jpg textfile.txt output_image.jpg
	argv[0] = ./encode
	argv[1] = output_image.jpg
	*/

	if(argc != 2) {
		cout << "Arguments Error" << "\n";
		return -1;
	}

	// Stores original image
	Mat image = imread(argv[1]);


	char ch=0;
	int bit_count = 0;

	for(int row=0; row < image.rows; row++) {
		for(int col=0; col < image.cols; col++) {
			for(int color=0; color < 3; color++) {

				if(isBitSet(image.at<Vec3b>(row,col)[color],0))
					ch |= 1;

				bit_count++;

				if(bit_count == 8) {
					if(ch == '\0')
						goto OUT;
					bit_count = 0;
					cout << ch;
					ch = 0;
				}
				else {
					ch = ch << 1;
				}

			}
		}
	}
	OUT:;
	

    return 0;
}
