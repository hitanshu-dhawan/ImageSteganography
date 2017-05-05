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
	./decode output_image.png
	argv[0] = ./decode
	argv[1] = output_image.png
	*/

	// Checks if proper number of arguments are passed
	if(argc != 2) {
		cout << "Arguments Error" << "\n";
		exit(-1);
	}

	// Stores original image
	Mat image = imread(argv[1]);
	if(image.empty()) {
		cout << "Image Error\n";
		exit(-1);
	}

	// char to work on
	char ch=0;
	// contains information about which bit of char to work on
	int bit_count = 0;

	/*
	To extract the message from the image, we will iterate through the pixels and extract the LSB of
	the pixel values (RGB) and this way we can get our message.
	*/
	for(int row=0; row < image.rows; row++) {
		for(int col=0; col < image.cols; col++) {
			for(int color=0; color < 3; color++) {

				// stores the pixel details
				Vec3b pixel = image.at<Vec3b>(Point(row,col));

				// manipulate char bits according to the LSB of pixel values
				if(isBitSet(pixel.val[color],0))
					ch |= 1;

				// increment bit_count to work on next bit
				bit_count++;

				// bit_count is 8, that means we got our char from the encoded image
				if(bit_count == 8) {

					// NULL char is encountered
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
