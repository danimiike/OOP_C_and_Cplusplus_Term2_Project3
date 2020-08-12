/*!	\file		bce.cpp
	\author		Danielle Miike
	\date		2020-03-16
				This program encodes a text message using a book cipher

*/

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//bok structure
struct book {
	unsigned int page;
	unsigned int line;
	unsigned int column;
	char character;
};

book makeBook(int p, int l, int c, char ch)
{
	book book;
	book.page = p;
	book.line = l;
	book.column = c;
	book.character = ch;
	return book;
}


int main(int argc, char* argv[]) {

	// declare vector 
	vector<book> bookVector; // book 
	vector<char> messageVector; //message file

	int countPage = 0, countLine = 0, countColumn = 0; // variables to identify pages, line and column os each character

	//Verify if too much parameters was included
	if (argc != 4) {
		cerr << "Incorrect number of argument." << endl;
		cerr << "\nUsage: BookCipherEncode bookfile originalfile codedfile." << endl;
		return EXIT_FAILURE;
	}
	else
		cout << "bcdecode (c) March 23, 2020. Danielle Miike.\nVersion: 1.0.0 " << endl;

	//read the book file
	ifstream bookFile(argv[1]);
	if (!bookFile) {
		cerr << "Could not open the file" << endl;
		return EXIT_FAILURE;
	}

	// load the book into the the vector
	char ch;
	while (!bookFile.eof()) {
		ch = bookFile.get();
		if (ch == '\f') {
			bookVector.push_back(makeBook(countPage, countLine, countColumn, ch));
			countPage++;
			countLine = 0;
			countColumn = 0;
		}
		else if (ch == '\n') {
			bookVector.push_back(makeBook(countPage, countLine, countColumn, ch));
			countLine++;
			countColumn = 0;
		}
		else {
			bookVector.push_back(makeBook(countPage, countLine, countColumn, ch));
			countColumn++;
		}
	}

	//read the message file
	ifstream messageFile(argv[2]);
	if (!messageFile) {
		cerr << "Could not open the file" << endl;
		return EXIT_FAILURE;
	}

	// load the message into the the vector
	while (!messageFile.eof()) {
		messageVector.push_back(messageFile.get());
	}

	//read or open the output code file
	ofstream codedFile(argv[3]);
	if (!codedFile) {
		cout << "Could not open the output file" << endl;
		return EXIT_FAILURE;

	}
	//declare variables to use in the decoder
	// This part of the code analyze if the message can be encrypted by the book choosed 
	vector<char>::size_type i, k, ctrlK;
	k = 0;
	ctrlK = 0;

	bool notFinded = false;


	for (i = 0; i < messageVector.size(); ++i) {

		ctrlK = k;
		bool stop = false;
		bool bookStart = false;
		int  ctrlRead = 0;

		if (notFinded == true) {
			i--;
			cerr << "Error <" << argv[1] << "> is not encodable by <" << argv[2] << "> missing character " << messageVector[i] << ":" << int(messageVector[i]) << endl;
			return EXIT_FAILURE;
		}

		while (stop == false && messageVector[i] != EOF) {

			if (bookStart == true) {
				stop = true;
				notFinded = true;
			}
			else if (ctrlRead >= 1 && ctrlK == bookVector.size()) {
				ctrlK = 0;
			}
			else {
				for (k = ctrlK; k < bookVector.size(); ++k) {
					if (ctrlRead > 1) {
						bookStart = true;
						break;
					}
					else {

						if (bookVector[k].character == messageVector[i]) {
							//load the decode file into the coded file
							codedFile << bookVector[k].page << "," << bookVector[k].line << "," << bookVector[k].column << " ";
							stop = true;
							break;
						}
					}
					ctrlK++;
				}
				ctrlRead++;
			}

		}

	}
	// Close the files objects

	bookFile.close();
	codedFile.close();
	messageFile.close();

}