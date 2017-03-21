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
	./encode image.png textfile.txt output_image.png
	argv[0] = ./encode
	argv[1] = image.png
	argv[2] = textfile.txt
	argv[3] = output_image.png
	*/

	// Checks if proper number of arguments are passed
	if(argc != 4) {
		cout << "Arguments Error" << "\n";
		exit(-1);
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

	// char to work on
	char ch;
	// reads the first char from the file
	file.get(ch);
	// contains information about which bit of char to work on
	int bit_count = 0;
	// to check whether file has ended
	bool last_null_char = false;
	// to check if the whole message is encoded or not
	bool encoded = false;

	/*
	To hide text into images. We are taking one char (8 bits) and each of the 8 bits are stored
	in the Least Significant Bits (LSB) of the pixel values (Red,Green,Blue).
	We are manipulating bits in such way that changing LSB of the pixel values will not make a huge difference.
	The image will still look similiar to the naked eye.
	*/

	for(int row=0; row < image.rows; row++) {
		for(int col=0; col < image.cols; col++) {
			for(int color=0; color < 3; color++) {

				// stores the pixel details
				Vec3b pixel = image.at<Vec3b>(Point(row,col));

				// if bit is 1 : change LSB of present color value to 1.
				// if bit is 0 : change LSB of present color value to 0.
				if(isBitSet(ch,7-bit_count))
					pixel.val[color] |= 1;
				else
					pixel.val[color] &= ~1;

				// update the image with the changed pixel values
				image.at<Vec3b>(Point(row,col)) = pixel;

				// increment bit_count to work on next bit
				bit_count++;

				// if last_null_char is true and bit_count is 8, then our message is successfully encode.
				if(last_null_char && bit_count == 8) {
					encoded  = true;
					goto OUT;
				}

				// if bit_count is 8 we pick the next char from the file and work on it
				if(bit_count == 8) {
					bit_count = 0;
					file.get(ch);

					// if EndOfFile(EOF) is encountered insert NULL char to the image
					if(file.eof()) {
						last_null_char = true;
						ch = '\0';
					}
				}

			}
		}
	}
	OUT:;

	// whole message was not encoded
	if(!encoded) {
		cout << "Message too big. Try with larger image.\n";
		exit(-1);
	}

	// Writes the stegnographic image
	imwrite(argv[3],image);

    return 0;
}
