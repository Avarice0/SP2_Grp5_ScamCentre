#include "Deck.h"

Deck::Deck() {
	srand(time(NULL));
}
Deck::~Deck() {

}

void Deck::resetopen(void) {
	opendeck.clear();
	for (size_t i = 0; i < size(cardlist); i++) {
		opendeck.push_back(cardlist[i]);
	}
	random_shuffle(opendeck.begin(), opendeck.end());
}

void Deck::resethand(vector <Card>& hand) {
	hand.clear();
}

void Deck::addcard(vector <Card>& hand) {
	hand.push_back(opendeck.back());
	opendeck.pop_back();

}
int Deck::valuecount(vector <Card>& hand) {
	int totalvalue = 0;
	for (vector<Card>::iterator it = hand.begin(); it != hand.end(); ++it) {
		totalvalue += it->getvalue();
	}
	return totalvalue;
}

void Deck::printdeck(vector <Card>& hand) {
	for (size_t i = 0; i < size(hand);i++) {
		std::cout << hand[i].getsuit() << ", " << hand[i].getvalue() << std::endl;
	}
}

//#include <iostream>
//#include <string>
//#include <conio.h>
//
//#include "Deck.h"
//using namespace std;
//
//int main(void) {
//	int balance = 1000;
//	int bettingvalue = 0;
//	int playerinput; bool pass;
//	Deck OpenDeck;
//	OpenDeck.resetopen();
//	while (balance > 0) {
//		do {
//			system("CLS");
//			std::cout << "Welcome to BlackJack!" << std::endl;
//			std::cout << "Press buttons to change bets." << std::endl;
//			std::cout << "You have $" << balance << " in your account." << std::endl;
//			std::cout << "You are betting $" << bettingvalue << std::endl;
//			std::cout << "Q for -100, W for -50, E for -10, R to start the game, T for +10, Y for +50, U for +100" << std::endl;
//			pass = false;
//			playerinput = _getch();
//			if (playerinput == 114) {		//A to continue next phase
//				pass = true;
//			}
//			else {
//				//reduce bet value
//				if (playerinput == 113 && bettingvalue >= 100) {
//					bettingvalue -= 100;
//				}
//				if (playerinput == 119 && bettingvalue >= 50) {
//					bettingvalue -= 50;
//				}
//				if (playerinput == 101 && bettingvalue >= 10) {
//					bettingvalue -= 10;
//				}
//				if (playerinput == 116 && bettingvalue + 10 <= balance) {
//					bettingvalue += 10;
//				}
//				if (playerinput == 121 && bettingvalue + 50 <= balance) {
//					bettingvalue += 50;
//				}
//				if (playerinput == 117 && bettingvalue + 100 <= balance) {
//					bettingvalue += 100;
//				}
//			}
//		} while (pass == false);
//
//		OpenDeck.resethand(OpenDeck.dealerhand); OpenDeck.resethand(OpenDeck.playerhand);
//		int result = 0;			//0 is no result, 1 is lose, 2 is tie, 3 is win
//		OpenDeck.addcard(OpenDeck.dealerhand); OpenDeck.addcard(OpenDeck.dealerhand);
//		OpenDeck.addcard(OpenDeck.playerhand); OpenDeck.addcard(OpenDeck.playerhand);
//		bool stand = false;
//		do {
//			system("CLS");
//			std::cout << "You have $" << balance << " in your account." << std::endl;
//			std::cout << "You are betting $" << bettingvalue << std::endl;
//			std::cout << "dealer hand:" << OpenDeck.valuecount(OpenDeck.dealerhand) << std::endl; OpenDeck.printdeck(OpenDeck.dealerhand);
//			std::cout << "player hand:" << OpenDeck.valuecount(OpenDeck.playerhand) << std::endl; OpenDeck.printdeck(OpenDeck.playerhand);
//			if (OpenDeck.valuecount(OpenDeck.playerhand) == 21 && OpenDeck.valuecount(OpenDeck.dealerhand) == 21) {
//				result = 2;
//			}
//			else if (OpenDeck.valuecount(OpenDeck.dealerhand) > 21) {
//				result = 3;
//			}
//			else if (OpenDeck.valuecount(OpenDeck.playerhand) > 21) {
//				result = 1;
//			}
//			else if (stand == true) {
//				if (OpenDeck.valuecount(OpenDeck.dealerhand) > OpenDeck.valuecount(OpenDeck.playerhand)) {
//					result = 1;
//				}
//				else if (OpenDeck.valuecount(OpenDeck.dealerhand) == OpenDeck.valuecount(OpenDeck.playerhand)) {
//					result = 2;
//				}
//				else if (OpenDeck.valuecount(OpenDeck.dealerhand) < OpenDeck.valuecount(OpenDeck.playerhand)) {
//					result = 3;
//				}
//			}
//			else if (stand == false) {
//				if (OpenDeck.valuecount(OpenDeck.dealerhand) <= 21 && OpenDeck.valuecount(OpenDeck.playerhand) <= 21) {
//					std::cout << "Q to Hit, W to stand" << std::endl;
//					playerinput = _getch();
//					if (playerinput == 113) {
//						OpenDeck.addcard(OpenDeck.playerhand);
//					}
//					else if (playerinput == 119) {
//						while (OpenDeck.valuecount(OpenDeck.dealerhand) <= 17) {
//							OpenDeck.addcard(OpenDeck.dealerhand);
//						}
//						stand = true;
//					}
//				}
//			}
//
//		} while (result == 0);
//
//		if (result == 1) {
//			std::cout << "Player lost" << std::endl;
//			balance -= bettingvalue;
//		}
//		else if (result == 2) {
//			std::cout << "Tie/Push" << std::endl;
//		}
//		else if (result == 3) {
//			std::cout << "Player won" << std::endl;
//			balance += bettingvalue;
//		}
//
//		std::cout << "Do you want to play again?: Q for yes, W for no";
//		int playagain = 0;
//		do {
//			playerinput = _getch();
//			if (playerinput == 113) {
//				playagain = 1;
//			}
//			else if (playerinput == 119) {
//				playagain = 2;
//			}
//		} while (playagain == 0);
//		if (playagain == 2) {
//			std::cout << "You left with $" << balance << std::endl;
//			break;
//		}
//	}
//	if (balance <= 0) {
//		std::cout << "You ran out of money!" << std::endl;
//	}
//
//
//	return 0;
//}