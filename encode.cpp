#include <iostream>
#include <fstream>

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
	argv[1] = image.jpg
	argv[2] = textfile.txt
	argv[3] = output_image.jpg
	*/

	if(argc != 4) {
		cout << "Arguments Error" << "\n";
		return -1;
	}

	// Stores original image
	Mat image = imread(argv[1]);
	if(image.empty()) {
		cout << "Image Error\n";
		exit(-1);
	}

	// Open file for text information
	ifstream file(argv[2]);
	if(!file.is_open()) {
		cout << "File Error\n";
		exit(-1);
	}

	char ch;
	file.get(ch);
	int bit_count = 0;
	bool last_null_char = false;

	for(int row=0; row < image.rows; row++) {
		for(int col=0; col < image.cols; col++) {
			for(int color=0; color < 3; color++) {

				if(isBitSet(ch,7-bit_count))
					image.at<Vec3b>(row,col)[color] |= 1;
				else
					image.at<Vec3b>(row,col)[color] &= ~1;

				bit_count++;

				if(last_null_char && bit_count == 8) {
					goto OUT;
				}
				if(bit_count == 8) {
					bit_count = 0;
					file.get(ch);

					if(file.eof()) {
						last_null_char = true;
						ch = '\0';
					}
				}

			}
		}
	}
	OUT:;


	// Writes the stegnographic image
	imwrite(argv[3],image);

    return 0;
}
