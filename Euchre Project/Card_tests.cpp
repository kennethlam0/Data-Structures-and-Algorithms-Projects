// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;

const Card ace_Diamonds(ACE, DIAMONDS);
const Card king_Diamonds(KING, DIAMONDS);
const Card queen_Diamonds(QUEEN, DIAMONDS);
const Card jack_Diamonds(JACK, DIAMONDS);
const Card ten_Diamonds(TEN, DIAMONDS);
const Card nine_Diamonds(NINE, DIAMONDS);

const Card ace_Clubs(ACE, CLUBS);
const Card king_Clubs(KING, CLUBS);
const Card queen_Clubs(QUEEN, CLUBS);
const Card jack_Clubs(JACK, CLUBS);
const Card ten_Clubs(TEN, CLUBS);
const Card nine_Clubs(NINE, CLUBS);

const Card ace_Hearts(ACE, HEARTS);
const Card king_Hearts(KING, HEARTS);
const Card queen_Hearts(QUEEN, HEARTS);
const Card jack_Hearts(JACK, HEARTS);
const Card ten_Hearts(TEN, HEARTS);
const Card nine_Hearts(NINE, HEARTS);

const Card ace_Spades(ACE, SPADES);
const Card king_Spades(KING, SPADES);
const Card queen_Spades(QUEEN, SPADES);
const Card jack_Spades(JACK, SPADES);
const Card ten_Spades(TEN, SPADES);
const Card nine_Spades(NINE, SPADES);

TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());

    Card c2(TWO,SPADES);
    ASSERT_EQUAL(TWO, c2.get_rank());
    ASSERT_EQUAL(SPADES, c2.get_suit());

    Card c3(JACK, DIAMONDS);
    ASSERT_EQUAL(JACK, c3.get_rank());
    ASSERT_EQUAL(DIAMONDS, c3.get_suit()); 
}

TEST(test_card_default_ctor)
{
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}

TEST(test_get_rank)
{
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank())
}

TEST(test_get_suit)
{
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(HEARTS, c.get_suit())
}

TEST(test_get_suit_trump)
{
    Card c(JACK, CLUBS);
    ASSERT_EQUAL(c.get_suit(SPADES),SPADES);
    Card c1(JACK, HEARTS);
    ASSERT_EQUAL(c1.get_suit(DIAMONDS),DIAMONDS);
    Card c2(JACK,SPADES);
    ASSERT_EQUAL(c2.get_suit(CLUBS),CLUBS);
    Card c3(JACK,DIAMONDS);
    ASSERT_EQUAL(c3.get_suit(HEARTS),HEARTS);
    Card c4(JACK, SPADES);
    ASSERT_EQUAL(c4.get_suit(SPADES),SPADES);
    Card c5(JACK, SPADES);
    ASSERT_EQUAL(c5.get_suit(HEARTS),SPADES);
}

TEST(test_os)
{
    Card two_hearts = Card(TWO, HEARTS);
    ostringstream oss;
    oss << two_hearts;
    ASSERT_EQUAL(oss.str(), "Two of Hearts");
    
    Card jack_Clubs = Card(JACK,CLUBS);
    ostringstream oss1;
    oss1 << jack_Clubs;
    ASSERT_EQUAL(oss1.str(), "Jack of Clubs");
}

TEST(test_is)
{
    istringstream iss("Two of Hearts");
    Card c;
    iss >> c;
    ASSERT_EQUAL(c, Card(TWO, HEARTS));


    istringstream iss1("Ten of Spades");
    Card c2;
    iss1 >> c2;
    ASSERT_EQUAL(c2, Card(TEN, SPADES));
}

TEST(test_less_than)
{
    ASSERT_TRUE(ace_Hearts < ace_Clubs);
    ASSERT_TRUE(ace_Clubs < ace_Diamonds);
    ASSERT_TRUE(nine_Hearts < ace_Spades);
    ASSERT_TRUE(king_Hearts < ace_Hearts);
    ASSERT_TRUE(queen_Hearts < king_Hearts);
    ASSERT_FALSE(ace_Hearts < ace_Spades);
    ASSERT_FALSE(ace_Diamonds<ace_Clubs);
    ASSERT_FALSE(ace_Diamonds<ace_Hearts);
    ASSERT_FALSE(ace_Diamonds<ace_Spades);
}

TEST(test_less_than_equal)
{
    Card ace_Hearts_copy = Card(ACE,HEARTS);
    Card king_Hearts_copy = Card(KING,HEARTS);
    ASSERT_TRUE(ace_Hearts < ace_Clubs);
    ASSERT_TRUE(ace_Clubs < ace_Diamonds);
    ASSERT_TRUE(nine_Hearts < ace_Spades);
    ASSERT_TRUE(king_Hearts < ace_Hearts);
    ASSERT_TRUE(queen_Hearts < king_Hearts);
    ASSERT_FALSE(ace_Hearts < ace_Spades);
    ASSERT_FALSE(ace_Diamonds<ace_Clubs);
    ASSERT_FALSE(ace_Diamonds<ace_Hearts);
    ASSERT_FALSE(ace_Diamonds<ace_Diamonds);
    ASSERT_FALSE(ace_Diamonds<ace_Spades);
    ASSERT_TRUE(ace_Hearts<=ace_Hearts_copy);
    ASSERT_TRUE(king_Hearts<=king_Hearts_copy);
}

TEST(test_greater_than)
{
    ASSERT_FALSE(ace_Hearts > ace_Clubs);
    ASSERT_FALSE(ace_Clubs > ace_Diamonds);
    ASSERT_FALSE(nine_Hearts > ace_Spades);
    ASSERT_FALSE(king_Hearts > ace_Hearts);
    ASSERT_FALSE(queen_Hearts > king_Hearts);
    ASSERT_TRUE(ace_Hearts > ace_Spades);
    ASSERT_TRUE(ace_Diamonds >ace_Clubs);
    ASSERT_TRUE(ace_Diamonds > ace_Hearts);
    ASSERT_TRUE(ace_Diamonds > ace_Spades);
}

TEST(test_greater_than_equal)
{
    Card ace_Hearts_copy = Card(ACE,HEARTS);
    Card king_Hearts_copy = Card(KING,HEARTS);
    ASSERT_FALSE(ace_Hearts>=ace_Clubs);
    ASSERT_FALSE(ace_Clubs>=ace_Diamonds);
    ASSERT_FALSE(nine_Hearts>=ace_Spades);
    ASSERT_FALSE(king_Hearts>=ace_Hearts);
    ASSERT_FALSE(queen_Hearts>=king_Hearts);
    ASSERT_TRUE(ace_Hearts>=ace_Spades);
    ASSERT_TRUE(ace_Diamonds>=ace_Clubs);
    ASSERT_TRUE(ace_Diamonds>=ace_Hearts);
    ASSERT_TRUE(ace_Diamonds>=ace_Spades);
    ASSERT_TRUE(ace_Hearts>=ace_Hearts_copy);
    ASSERT_TRUE(king_Hearts>=king_Hearts_copy);
}

TEST(test_equal)
{
    Card ace_Hearts = Card(ACE,HEARTS);
    Card ace_Hearts_copy = Card(ACE,HEARTS);
    Card ace_Spades = Card(ACE,SPADES);
    ASSERT_TRUE(ace_Hearts==ace_Hearts_copy);
    ASSERT_FALSE(ace_Hearts==ace_Spades);
}

TEST(test_not_equal)
{
    Card ace_Hearts = Card(ACE,HEARTS);
    Card ace_Hearts_copy = Card(ACE,HEARTS);
    Card ace_Spades = Card(ACE,SPADES);
    ASSERT_FALSE(ace_Hearts!=ace_Hearts_copy);
    ASSERT_TRUE(ace_Hearts!=ace_Spades);
}

TEST(test_suit_next)
{
    Suit diamonds = DIAMONDS;
    Suit clubs = CLUBS; 
    Suit hearts = HEARTS;
    Suit spades = SPADES; 
    ASSERT_EQUAL(Suit_next(diamonds),HEARTS);
    ASSERT_EQUAL(Suit_next(hearts),DIAMONDS);
    ASSERT_EQUAL(Suit_next(spades),CLUBS);
    ASSERT_EQUAL(Suit_next(clubs),SPADES);
}

TEST(test_card_less)
{
  ASSERT_FALSE(Card_less(jack_Spades,jack_Clubs,SPADES));
  ASSERT_FALSE(Card_less(nine_Hearts,jack_Spades,HEARTS))
  ASSERT_TRUE(Card_less(nine_Hearts,jack_Spades,SPADES))
  ASSERT_TRUE(Card_less(ten_Spades,king_Spades,SPADES));
  ASSERT_TRUE(Card_less(jack_Hearts,jack_Diamonds,SPADES));
  ASSERT_TRUE(Card_less(ace_Spades,jack_Clubs,SPADES));
  ASSERT_TRUE(Card_less(ace_Clubs,jack_Clubs,CLUBS));
  ASSERT_FALSE(Card_less(jack_Hearts,jack_Diamonds,HEARTS));
  ASSERT_TRUE(Card_less(jack_Hearts,jack_Diamonds,DIAMONDS));
  ASSERT_FALSE(Card_less(nine_Hearts,ace_Diamonds,HEARTS));
}

TEST(test_card_less_led)
{
    Card led_card = Card(QUEEN, HEARTS);
    ASSERT_TRUE(Card_less(jack_Clubs,jack_Spades,led_card,SPADES));
    ASSERT_FALSE(Card_less(jack_Spades,jack_Clubs,led_card,SPADES));
    ASSERT_FALSE(Card_less(nine_Hearts,ten_Clubs,led_card,SPADES));
    ASSERT_TRUE(Card_less(nine_Hearts,ten_Hearts,led_card,SPADES));
    ASSERT_TRUE(Card_less(jack_Hearts,jack_Diamonds,jack_Hearts,DIAMONDS));
    ASSERT_FALSE(Card_less(jack_Diamonds,jack_Hearts,ten_Hearts,DIAMONDS));
    ASSERT_TRUE(Card_less(jack_Diamonds,jack_Hearts,ten_Hearts,HEARTS));
    ASSERT_TRUE(Card_less(jack_Diamonds,jack_Hearts,ten_Hearts,SPADES));
    ASSERT_FALSE(Card_less(jack_Diamonds,jack_Hearts,ten_Diamonds,SPADES));
    ASSERT_FALSE(Card_less(jack_Diamonds,jack_Hearts,ten_Clubs,SPADES));
    ASSERT_FALSE(Card_less(jack_Diamonds,jack_Hearts,ten_Spades,SPADES));
    ASSERT_TRUE(Card_less(king_Diamonds, jack_Hearts, queen_Diamonds, HEARTS));
    ASSERT_TRUE(Card_less(king_Diamonds, jack_Hearts, queen_Hearts, HEARTS));
    ASSERT_TRUE(Card_less(king_Diamonds, jack_Hearts, queen_Hearts, DIAMONDS));

}
TEST_MAIN()
