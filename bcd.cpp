/*!	\file		bcd.cpp
	\author		Danielle Miike
	\date		2020-03-16
				This program decodes a text message using a book cipher

*/
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

//structure to store the book
struct book {
	unsigned int page;
	unsigned int line;
	unsigned int column;
	char character;
};
//structure to store the code file
struct coded {
	unsigned int page;
	unsigned int line;
	unsigned int column;
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

coded makeCodeFile(int p, int l, int c) {
	coded file;
	file.page = p;
	file.line = l;
	file.column = c;
	return file;
}


int main(int argc, char* argv[]) {

	// declare vectors 
	vector<book> bookVector;
	vector<coded> codedVector;

	//declare variables
	int countPage = 0, countLine = 0, countColumn = 0;
	int page = 0, line = 0, column = 0;
	vector<char>::size_type  i, k = 0;

	if (argc != 4)
	{
		cout << "Incorrect number of argument <" << argc - 1 << ">";
		cout << "\nUsage: BookCipherDecode bookfile codedfile decodefile" << endl;
		exit(1); // exit program
	}
	else
		cout << "bcdecode (c) March 23, 2020. Danielle Miike.\nVersion: 1.0.0 " << endl;



	//read the book
	ifstream bookFile(argv[1]);
	if (!bookFile) {
		cerr << "Could not open the file" << endl;
		return EXIT_FAILURE;
	}

	// load the book into the the vector
	int bookIndex = 0;
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

	//Read the message to be decode
	FILE* codedFile;
	fopen_s(&codedFile, argv[2], "r");
	if (!codedFile) {
		cerr << "Could not open the file" << endl;
		return EXIT_FAILURE;
	}
	// load the message into the the vector
	while (fscanf_s(codedFile, "%u,%u,%u ", &page, &line, &column) != EOF) {
		codedVector.push_back(makeCodeFile(page, line, column));
	}

	//open or create the message file
	ofstream messageFile(argv[3]);
	if (!messageFile) {
		cout << "Could not open the output file" << endl;
		return EXIT_FAILURE;
	}

	//Analyze if the in the book vector and the message vector codes are the same, do the decode message character by character
	int temp = 0;
	bool errFinded = false;

	for (i = 0; i < codedVector.size(); ++i) {

		if (errFinded == true) {
			cerr << "bce (c++) 2020, Danielle Miike " << "\n" << "\t\t version 1.0.0" << endl;
			cerr << "Error: column <" << codedVector[i].page << "> not found on line <" << codedVector[i].line << "> of page <" << codedVector[i].column << "> in the book." << endl;
			return EXIT_FAILURE;
		}

		for (k = 0; k < bookVector.size(); ++k) {
			if (bookVector[k].character == EOF) {
				break;
			}

			if (codedVector[i].page == bookVector[k].page) {
				if (codedVector[i].line == bookVector[k].line) {
					if (codedVector[i].column == bookVector[k].column) {
						messageFile << bookVector[k].character;
						temp++;
						break;
					}
					else if (k == bookVector.size() - 2) {
						errFinded = true;
						break;
					}
				}
			}
		}
	}


	// Close the files objects

	bookFile.close();
	fclose(codedFile);
	messageFile.close();

}