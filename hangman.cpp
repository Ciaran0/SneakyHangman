#include "hangman.h"

/**
*Constructor
*@param numlives the number of wrong guesses the user can make
*@param debugMode to show debug info
*/
Hangman::Hangman(int numLives, bool debugMode){
	//number of wrong guesses the user can have
	maxGuesses = numLives;
	wrongGuesses=0;
	guessedCorrect=0;
	debug = debugMode;
}
/**
*Generates randomly the word the user will try to guess
*Adds all possible words to a vector
*@param n the minimun length of the word to be guessed
*/
void Hangman::generateWord(int n){
	//stream to read through dictionary file
	ifstream inStream;
	inStream.open("dictionary.txt");
	vector<string> dictionary;
	//Dealing with no file error
	if(inStream.fail())
	{
		cout << "Input file opening failed. "<< endl;
		exit(1);
	}

    string temp;
	//put words in dictioary file into vector
	//only of size n >=
	//put to lower case
    while(inStream >> temp){
    	if(temp.length()==n){
			transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
    		dictionary.push_back(temp);
    	}
	}
	//close stream
	inStream.close();

    //Word to use for hangman
    hangManWord = randomFromVector(dictionary);
    //setup vector for equivalent words
    //inital equivalances are words of the same length
    for (long i = 0; i < dictionary.size(); i++)
	{
		if(dictionary[i].length()==hangManWord.length())
			equiv.push_back(dictionary[i]);
	}
    answer.assign(hangManWord.length(),'_');
}
/**
*Gets input from user for letter they are guessing
*Displays already guessed letters. Adds these to vector
*Displays the game
*/
void Hangman::getInput(){
	//for debugging will display the hangman word
	//cout << hangManWord << endl;
	cout <<"Guess a letter" << endl;
	cout << "Guessed Letters: ";
	for (int i = 0; i < guessed.size(); ++i)
	{

		cout << guessed[i]<< " ";
	}
	cout << endl;

	for (int i = 0; i < hangManWord.length(); i++)
	{
		cout << answer[i] <<' ';
	}
	cout << endl;
	//get char for guess
	cin >> guessedLetter;
	//check if guess is alphabetical
	if(!isalpha(guessedLetter))
    {
		cout<<"You must enter an alphabetical character (a-z)."<< endl;
		getInput();
		return;
    }  

}
/**
*@return true if user guessed a letter correctly
*@return false if user guessed incorectly
*/
bool Hangman::guessedL(){

	guessed.push_back(guessedLetter);
	changeWord();
	if (hangManWord.find(guessedLetter) == string::npos) {
		wrongGuesses++;
		return false;
	}
	
	return true;
}
int Hangman::numLivesLeft(){
	return maxGuesses - wrongGuesses;
}
int Hangman::countDuplicateCharacters(string str)
{
    sort(str.begin(), str.end());
    auto i = unique(str.begin(), str.end());
    return str.length() - (i - str.begin());
}
/**
*@return hangManWord the word the user is trying to guess
*/
string Hangman::word(){
	return hangManWord;
}
/**
*@returns a random word from vector
*/
string Hangman::randomFromVector(vector<string> words){
	srand(time(NULL));
	//Generate a random index
	int index = rand() % words.size();
	return words[index];
}
/**
*picks best possible word from the vector
*/
string Hangman::pickWord(){
	if(equiv.size()==0){
		return hangManWord;
	}
	vector<string> possibleWords;
	int bestWord =10;
	for(long i=0;i<equiv.size();i++){
		if(countDuplicateCharacters(equiv[i])<bestWord){
			bestWord = countDuplicateCharacters(equiv[i]);
		}
	}
	for(long i=0;i<equiv.size();i++){
		if(countDuplicateCharacters(equiv[i])==bestWord)
			possibleWords.push_back(equiv[i]);
	}
	return randomFromVector(possibleWords);

}
/**
*Sneaky Part!
*Changes the word when the user guesses correctly
*updates the vector with the possible equivalances
*@returns true if the word is changed
*/
bool Hangman::changeWord(){
	int position = bestPosition();
	update(position);
	hangManWord = pickWord();
	if(position!=-1){
		for(int i=0;i<hangManWord.length();i++){
			if(hangManWord[i]==guessedLetter){
				answer[i] = guessedLetter;
				guessedCorrect++;
			}
		}
	}
	
	return position==-1;
}

/**
*Check is the user won
*@returns true if the user has won
*/
bool Hangman::won(){
	//note multiple correct entries leads to win
	return (guessedCorrect == hangManWord.length());

}
/**
*Checks is user has lost
*@returns true if the user has lost the game
*/
bool Hangman::lost(){
	return (wrongGuesses == maxGuesses);
}
/**
*Checks if the letter has already been guessed
*Does not punish user if it has just notifys them
*@returns true if letter has been used previously
*/
bool Hangman::alreadyGuessed(){
	for (int i = 0; i < guessed.size(); i++)
	{
		if(guessedLetter==guessed[i]){
			return true;
		}

	}
	return false;
}
bool Hangman::debugEnabled(){
	return debug;
}
/**
*@return the best position for the letter
*/
int Hangman::bestPosition(){
	vector<int> positions (hangManWord.length(),0);
	int notIn =0;
	//go through words list
	for(long i=0; i<equiv.size();i++){
		//bump count notIn if letter isnt in the word
		bool contains = false;
		if(equiv[i].find(guessedLetter) == string::npos){
			notIn++;
			contains = true;
		}
		if(!contains){
			//go through word[i] of wordlist
			for(int k=0;k<hangManWord.length();k++){
				if(equiv[i].at(k)==guessedLetter){
					positions[k]++;
				}
			}
		}
	}
	//which is the best position
	//to do this we need to know which will leave us with most words
	int biggestPosition = positions[0];
	int bestPosition =0;
	for(int i=1;i<hangManWord.length();i++){
		if(positions[i]>biggestPosition){
			bestPosition = i;
			biggestPosition = positions[i];
		}
	}
	if(notIn>biggestPosition){
		bestPosition =-1;
	}
	//if in debug mode print out the positions families
	if(debugEnabled()){
		cout << "Letter not in word: " << notIn <<endl;
		for(int i=0; i<hangManWord.length();i++){
			cout << positions[i] << " ";
		}
		cout << "best position: " <<bestPosition <<endl;
	}
	
	return bestPosition;
}
/**
*update the equivalances
*/
void Hangman::update(int position){
	if(position == -1) {
		for(long i=0;i<equiv.size();i++){
			if(equiv[i].find(guessedLetter) != string::npos){
					equiv.erase(equiv.begin()+i);
					i--;
			}
		}
	}
	else{
		for(long i=0;i<equiv.size();i++){
			for(int k=0;k<hangManWord.length();k++){
				
				if((equiv[i][position]!=guessedLetter) || (equiv[i][k]==guessedLetter && k != position)){
					equiv.erase(equiv.begin()+i);
					i--;
					break;
				}
			}
		}
	}
}
