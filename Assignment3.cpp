/*
===============================
Name      : Assignment3.cpp
Author    : Ye Yuan
Student ID: 260921269
*/
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cstdlib>
using namespace std;

//Create enum for Ranks
enum Rank
{
    ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};
//Create enum for Types
enum Type
{
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
};

class Card
{
public:
    Card(Type pType, Rank pRank);
    int getValue() const;
    void displayCard();

private:
    Type aType;
    Rank aRank;
};

//Constructor initialize corresponding fields
Card::Card(Type pType, Rank pRank)
{
    this->aType = pType;
    this->aRank = pRank;
}

//This function should be constant since we would call it in a constant function
int Card ::getValue() const
{
    //No need to have default case
    switch (this->aRank)
    {
    case ACE:
        return 1;
    case TWO:
        return 2;
    case THREE:
        return 3;
    case FOUR:
        return 4;
    case FIVE:
        return 5;
    case SIX:
        return 6;
    case SEVEN:
        return 7;
    case EIGHT:
        return 8;
    case NINE:
        return 9;
    case TEN:
        return 10;
    case JACK:
        return 10;
    case QUEEN:
        return 10;
    case KING:
        return 10;
    }
}

void Card ::displayCard()
{
    //No need to have default case
    switch (this->aRank)
    {
    case ACE:
        cout << "1";
        break;
    case TWO:
        cout << "2";
        break;
    case THREE:
        cout << "3";
        break;
    case FOUR:
        cout << "4";
        break;
    case FIVE:
        cout << "5";
        break;
    case SIX:
        cout << "6";
        break;
    case SEVEN:
        cout << "7";
        break;
    case EIGHT:
        cout << "8";
        break;
    case NINE:
        cout << "9";
        break;
    case TEN:
        cout << "10";
        break;
    case JACK:
        cout << "J";
        break;
    case QUEEN:
        cout << "Q";
        break;
    case KING:
        cout << "K";
        break;
    }
    //No need to have default case
    switch (this->aType)
    {
    case CLUBS:
        cout << "C";
        break;
    case DIAMONDS:
        cout << "D";
        break;
    case HEARTS:
        cout << "H";
        break;
    case SPADES:
        cout << "S";
        break;
    }
}

class Hand
{
public:
    void add(Card pCard);
    void clear();
    int getTotal() const;
    //Add a method to display the cards in the hand, not required by the documentation of assignment
    void displayHand();

private:
    vector<Card> allCards;
};

void Hand::add(Card pCard)
{
    //add the new Card at end of the vector
    this->allCards.push_back(pCard);
}

void Hand::clear()
{
    this->allCards.clear();
}

//This method declared as const since it would be called in a constant function
int Hand::getTotal() const
{
    int sum = 0;
    //To check if the player have an ACE in hand
    bool ifAce = false;
    for (int i = 0; i < this->allCards.size(); i++)
    {
        if (this->allCards[i].getValue() == 1)
        {
            ifAce = true;
        }
        sum += this->allCards[i].getValue();
    }
    if (ifAce && sum + 10 <= 21)
    {
        return sum + 10;
    }
    else
    {
        return sum;
    }
}

//Helper function to display the cards in the hand
void Hand::displayHand()
{
    for (int i = 0; i < this->allCards.size(); i++)
    {
        this->allCards[i].displayCard();
        cout << " ";
    }
    //Also show the value in hand
    cout << "[" << this->getTotal() << "]" << endl;
}

class Deck
{
public:
    void Populate();
    void shuffle();
    void deal(Hand &pHand);

private:
    vector<Card> allCards;
};

//Create all Cards. (Actually we can implement flyweight design pattern for Card class)
void Deck::Populate()
{
    this->allCards.clear();
    for (int t = CLUBS; t <= SPADES; t++)
    {
        for (int r = ACE; r <= KING; r++)
        {
            Card card = Card((Type)t, (Rank)r);
            this->allCards.push_back(card);
        }
    }
}

void Deck::shuffle()
{
    //tried to use random_shuffle, but cannot achieve completely random
    //random_shuffle(this->allCards.begin(), this->allCards.end());
    srand(time(0));
    //called Fisher Yates shuffle algorithm.
    for (int i = this->allCards.size() - 1; i >= 0; i--)
    {
        int random = rand() % this->allCards.size();
        swap(this->allCards[i], this->allCards[random]);
    }
}

//Pass by reference since we need to add the card to the hand
void Deck::deal(Hand &pHand)
{
    Card toDeal = this->allCards.back();
    this->allCards.pop_back();
    pHand.add(toDeal);
}

//Inherited from the Hand class so that we can use the getTotal() method
class AbstractPlayer : public Hand
{
public:
    virtual bool isDrawing() const = 0;
    bool isBusted();
};

bool AbstractPlayer::isBusted()
{
    return this->getTotal() > 21;
}

class HumanPlayer : public AbstractPlayer
{
public:
    bool isDrawing() const;
    void announce(Hand computer);
};

//Wheter the HumanPlayer is drawing or not depend on player's choice
bool HumanPlayer ::isDrawing() const
{
    if (this->getTotal() > 21)
    {
        return false;
    }
    cout << "Do you want to draw? (y/n): ";
    char answer;
    cin >> answer;
    if (answer == 'y')
    {
        return true;
    }
    else if (answer == 'n')
    {
        return false;
    }
    else
    {
        //throw an exception if the user enter an invalid input
        throw "You answer is not y/n";
    }
}

void HumanPlayer ::announce(Hand computer)
{
    //The player who busted first will lose the game
    if (this->isBusted())
    {
        cout << "Player busts." << endl;
        cout << "Casino wins." << endl;
    }
    else if (computer.getTotal() > 21)
    {
        cout << "Casino busts." << endl;
        cout << "Player wins." << endl;
    }
    else if (this->getTotal() > computer.getTotal())
    {
        cout << "Player wins." << endl;
    }
    else if (computer.getTotal() > this->getTotal())
    {
        cout << "Casino wins." << endl;
    }
    else if (computer.getTotal() == this->getTotal())
    {
        cout << "Push: No one wins." << endl;
    }
}

class ComputerPlayer : public AbstractPlayer
{
public:
    bool isDrawing() const;
};

bool ComputerPlayer::isDrawing() const
{
    return this->getTotal() <= 16;
}

class BlackJackGame
{
public:
    void play();

private:
    Deck m_deck;
    ComputerPlayer m_casino;
};

void BlackJackGame::play()
{
    cout << "Now begin a new round!" << endl;
    //We need to clear the hand of casino every round since we only have one casino
    m_casino.clear();
    //Reset the deck, the deck will be cleared before populate
    m_deck.Populate();
    m_deck.shuffle();
    //Computer will have one open card
    m_deck.deal(m_casino);
    cout << "Casino: ";
    m_casino.displayHand();
    //Create a new human player
    HumanPlayer player = HumanPlayer();
    //Human player will have two open card
    m_deck.deal(player);
    m_deck.deal(player);
    cout << "Player: ";
    player.displayHand();
    //Player's turn
    while (player.isDrawing())
    {
        m_deck.deal(player);
        cout << "Player: ";
        player.displayHand();
        if (player.isBusted())
        {
            player.announce(m_casino);
            cout << endl;
            return;
        }
    }
    cout << "Player stop drawing. Casino's turn." << endl;
    //Casino's turn
    while (m_casino.isDrawing())
    {
        m_deck.deal(m_casino);
        cout << "Casino: ";
        m_casino.displayHand();
    }
    player.announce(m_casino);
    cout << endl;
}

int main()
{
    cout << "Welcome to the Comp322 Blackjack game!" << endl;
    BlackJackGame game;
    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while (playAgain)
    {
        game.play();
        // Check whether the player would like to play another round
        cout << "Would you like another round? (y/n): ";
        cin >> answer;
        playAgain = (answer == 'y' ? true : false);
    }
    cout << "Gave over!" << endl;
    return 0;
}