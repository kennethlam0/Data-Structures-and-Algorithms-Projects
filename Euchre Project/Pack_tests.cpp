// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Pack.h"
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_pack_default_ctor) 
{
    Pack pack;
    for(int suit = SPADES; suit <= DIAMONDS; ++suit)
    {
        for(int rank = NINE; rank <= ACE; ++rank)
        {
            Card card = pack.deal_one();
            ASSERT_EQUAL(rank,card.get_rank());
            ASSERT_EQUAL(suit,card.get_suit());
        }
    }   
}

TEST(test_pack_reset) 
{
    istringstream iss("Queen of Diamonds\nNine of Diamonds\nQueen of Clubs\n"
    "Ten of Clubs\nKing of Hearts\nNine of Spades\n"
    "Ace of Diamonds\nKing of Diamonds\nKing of Spades\n"
    "Ten of Hearts\nJack of Clubs\nKing of Clubs\n"
    "Queen of Spades\nAce of Hearts\nAce of Spades\n"
    "Jack of Diamonds\nNine of Hearts\n Queen of Hearts\n"
    "Ace of Clubs\nJack of Spades\nTen of Diamonds\n"
    "Ten of Spades\nNine of Clubs\nJack of Hearts\n");
    Pack pack(iss);
    Card queen_of_diamonds= Card(QUEEN,DIAMONDS);
    for(int i = 0; i < 24; ++i)
    {
        pack.deal_one();
    }
    pack.reset();
    Card first_card = pack.deal_one();
    ASSERT_EQUAL(first_card, queen_of_diamonds);

}

TEST(test_pack_empty)
{
    istringstream iss("Queen of Diamonds\nNine of Diamonds\nQueen of Clubs\n"
    "Ten of Clubs\nKing of Hearts\nNine of Spades\n"
    "Ace of Diamonds\nKing of Diamonds\nKing of Spades\n"
    "Ten of Hearts\nJack of Clubs\nKing of Clubs\n"
    "Queen of Spades\nAce of Hearts\nAce of Spades\n"
    "Jack of Diamonds\nNine of Hearts\n Queen of Hearts\n"
    "Ace of Clubs\nJack of Spades\nTen of Diamonds\n"
    "Ten of Spades\nNine of Clubs\nJack of Hearts\n");
    Pack pack(iss);
    for(int i = 0; i < 23; ++i)
    {
        pack.deal_one();
    }
    ASSERT_FALSE(pack.empty());
    pack.deal_one();
    ASSERT_TRUE(pack.empty());
}

TEST(test_pack_shuffle)
{
    istringstream iss("Queen of Diamonds\nNine of Diamonds\nQueen of Clubs\n"
    "Ten of Clubs\nKing of Hearts\nNine of Spades\n"
    "Ace of Diamonds\nKing of Diamonds\nKing of Spades\n"
    "Ten of Hearts\nJack of Clubs\nKing of Clubs\n"
    "Queen of Spades\nAce of Hearts\nAce of Spades\n"
    "Jack of Diamonds\nNine of Hearts\n Queen of Hearts\n"
    "Ace of Clubs\nJack of Spades\nTen of Diamonds\n"
    "Ten of Spades\nNine of Clubs\nJack of Hearts\n");
    Pack pack(iss);
    pack.shuffle();
    istringstream expected("Nine of Hearts\nKing of Spades\nQueen of Diamonds\n"
    "Queen of Hearts\nTen of Hearts\nNine of Diamonds\n"
    "Ace of Clubs\nJack of Clubs\nQueen of Clubs\n"
    "Jack of Spades\nKing of Clubs\nTen of Clubs\n"
    "Ten of Diamonds\nQueen of Spades\nKing of Hearts\n"
    "Ten of Spades\nAce of Hearts\nNine of Spades\n" 
    "Nine of Clubs\nAce of Spades\nAce of Diamonds\n"
    "Jack of Hearts\nJack of Diamonds\nKing of Diamonds\n");
    Pack pack_expected(expected);
    for(int i = 0; i < 24; ++i)
    {
        ASSERT_EQUAL(pack.deal_one(),pack_expected.deal_one());
    }

}




// Add more tests here

TEST_MAIN()
