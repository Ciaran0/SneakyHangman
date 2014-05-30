#ifndef hangman
#define hangman

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <cctype>

using namespace std;

class Hangman
{
private:

	//hold guessed letters
	vector<char> guessed;
	//hold words that are equivalent to the hangmanword
	vector<string> equiv;
	//the word user is trying to guess
	string hangManWord;
	//the developing answer. Initialised to '_'s as common with hangman games
	vector<char> answer;
	//max number of wrong guesses the user can make
	int wrongGuesses;
	//number of correct guesses user has made
	int guessedCorrect;
	//max no. of guesses user can make
	int maxGuesses;
	//the letter
	char guessedLetter;
	//setting for debugging
	bool debug;
	//helper function to update equivalances
	vector<string> updateEquiv();
	//find the best position (word families)
	int bestPosition();
	//update the equivalances
	void update(int position);
	//pick a word to become the hangmanWord
	string pickWord();
	//counts the duplicate chars in string
	int countDuplicateCharacters(string str);
	//random word from vector
	string randomFromVector(vector<string> words);

public:
	Hangman(int n, bool debugMode);
	void generateWord(int n);
	void getInput();
	bool guessedL();
	bool won();
	bool lost();
	bool alreadyGuessed();
	string word();
	bool changeWord();
	int numLivesLeft();
	bool debugEnabled();

};

#endif
