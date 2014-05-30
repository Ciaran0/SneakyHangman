#include "hangman.h"

int main()
{

	//Settings for no. of lives and minimum word size
	const int lives =16;
	int minWordSize;
	//debug mode 
	bool debugMode = false;

	//play game
	cout << "Welcome to Hangman!" << endl;
	
	bool rightSize =false;
	while (!rightSize){
		cout << "What word size would you like? (4-8)"<<endl;
		cin >>minWordSize;
		if(minWordSize>3&&minWordSize<9)
			rightSize = true;
	}
	cout << "Enter debug mode? (Y/N)" <<endl;
	char debug;
	cin >> debug;
	if(debug=='y'||debug=='Y')
		debugMode=true;
	//create a game with no. of lives setting, debug setting
	Hangman game(lives,debugMode);

	//get the word the user will be guessing for
	//word will be minWordSize letters of size or greater
	game.generateWord(minWordSize);


	cout << "You have " <<lives <<" lives."<<endl;
	while(!game.lost()&&!game.won()){
		if(game.debugEnabled()){
			cout << "DEBUG MODE: "<< game.word()<<endl;
		}
		game.getInput();
		if(game.alreadyGuessed()){
			cout << "Letter already guessed!" << endl;
		}
		else
		{
			if(game.guessedL())
				cout << "Letter correct" << endl;
			else
				cout << "Letter Incorrect "<<endl<<"You have "<<game.numLivesLeft()<< " lives left\n";

		}

	}
	//game over user has won or lost
	cout << "GAME OVER!!" << endl;
	if(game.won()){
		cout << "You win" <<endl;
		cout << "The word was " << game.word()<<endl;
	}
	else {
		cout << "You lose!" <<endl;
		cout << "The word was " << game.word()<<endl;
	}
	return (0);
}
