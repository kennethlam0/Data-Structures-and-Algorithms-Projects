// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "Card.h"
#include "unit_test_framework.h"
#include <sstream>
#include <iostream>
#include <string>

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

TEST(test_player_ctr) {
    Player * ann= Player_factory("Ann", "Simple");
    Player * bob = Player_factory("Bob", "Human");
    ASSERT_EQUAL("Ann", ann->get_name());
    ASSERT_EQUAL("Bob", bob->get_name());

    ostringstream oss;
    oss << *ann;
    ASSERT_EQUAL(oss.str(), "Ann");
    oss.str("");
    oss << *bob;
    ASSERT_EQUAL(oss.str(), "Bob");

    delete ann;
    delete bob;
}


static void add_cards(Player *player, const Card &card1, const Card &card2,
                    const Card &card3, const Card &card4, const Card &card5) {
    player->add_card(card1);
    player->add_card(card2);
    player->add_card(card3);
    player->add_card(card4);
    player->add_card(card5);
}

TEST(test__make_trump_round1){
    Player * ann = Player_factory("Ann", "Simple");
    Player * bob = Player_factory("Bob", "Simple");
    Player * zach = Player_factory("Zach", "Simple");
    Suit trump;
    add_cards(ann,queen_Spades,ten_Spades ,nine_Spades , queen_Hearts, queen_Diamonds);
    ASSERT_FALSE(ann->make_trump(ten_Spades, true, 1, trump));
    ASSERT_FALSE(ann->make_trump(nine_Hearts, false , 1, trump));
    ASSERT_FALSE(ann->make_trump(ace_Diamonds,true,1,trump));
    ASSERT_FALSE(ann->make_trump(nine_Clubs,false, 1, trump));
    add_cards(bob,queen_Hearts, king_Hearts, nine_Diamonds, nine_Hearts, king_Clubs);
    ASSERT_FALSE(bob->make_trump(ten_Clubs,true,1,trump));
    ASSERT_TRUE(bob->make_trump(ten_Hearts,false, 1, trump));
    ASSERT_EQUAL(trump,HEARTS);
    ASSERT_TRUE(bob->make_trump(queen_Hearts,true,1, trump));
    ASSERT_FALSE(bob->make_trump(king_Diamonds,false, 1, trump));
    add_cards(zach, queen_Spades, king_Spades, nine_Diamonds, ten_Diamonds, queen_Hearts);
    ASSERT_FALSE(zach->make_trump(ten_Hearts, true, 1, trump));
    ASSERT_TRUE(zach->make_trump(ten_Spades,true, 1, trump));
    ASSERT_EQUAL(trump,SPADES);
    ASSERT_TRUE(zach->make_trump(queen_Spades, false , 1, trump));
    ASSERT_FALSE(zach->make_trump(queen_Diamonds, false, 1, trump));
    delete ann;
    delete bob;
    delete zach;
}

TEST(test_make_trump_round1_edge){
    Player * ann = Player_factory("Ann", "Simple");
    Player * bob = Player_factory("Bob", "Simple");
    Player * zach = Player_factory("Zach", "Simple");
    add_cards(ann, queen_Clubs, jack_Spades, queen_Hearts, jack_Diamonds, nine_Clubs);
    add_cards(bob, jack_Clubs, queen_Spades, nine_Diamonds, ten_Diamonds, jack_Hearts);
    add_cards(zach, jack_Diamonds, queen_Diamonds, nine_Clubs, king_Diamonds, jack_Hearts);
    Suit trump;
    ASSERT_FALSE(ann->make_trump(ten_Diamonds, true, 1, trump));
    ASSERT_TRUE(ann->make_trump(queen_Clubs,false ,1 , trump));
    ASSERT_EQUAL(trump, CLUBS);
    ASSERT_TRUE(ann->make_trump(nine_Hearts, true, 1, trump));
    ASSERT_EQUAL(trump,HEARTS);
    ASSERT_FALSE(ann->make_trump(ten_Spades,true,1, trump));
    ASSERT_FALSE(bob->make_trump(queen_Hearts, false, 1 , trump));
    ASSERT_FALSE(bob->make_trump(nine_Diamonds, true, 1, trump))
    ASSERT_TRUE(bob->make_trump(ten_Spades,false,1, trump));
    ASSERT_TRUE(bob->make_trump(queen_Spades,true,1,trump));
    ASSERT_EQUAL(trump,SPADES);
    ASSERT_TRUE(zach->make_trump(ten_Diamonds, false, 1, trump));
    ASSERT_EQUAL(trump,DIAMONDS);
    delete ann;
    delete bob;
    delete zach;
}

TEST(test_player_make_trump_round2){
    Player * ann = Player_factory("Ann", "Simple");
    Player * bob = Player_factory("Bob", "Simple");
    Player * chelsea = Player_factory("Chelsea", "Simple");
    add_cards(ann,jack_Clubs, jack_Spades, queen_Hearts, ten_Hearts, nine_Spades);
    add_cards(bob, king_Diamonds, jack_Hearts, nine_Clubs, jack_Spades, ten_Diamonds);
    add_cards(chelsea, nine_Clubs, nine_Diamonds, nine_Hearts, ten_Spades, queen_Hearts);
    Suit trump;
    ASSERT_TRUE(ann->make_trump(ten_Clubs,true, 2, trump));
    ASSERT_EQUAL(trump,SPADES);
    ASSERT_TRUE(ann->make_trump(ten_Diamonds,true, 2, trump));
    ASSERT_EQUAL(trump,HEARTS);
    ASSERT_TRUE(ann->make_trump(ten_Spades,true, 2, trump));
    ASSERT_EQUAL(trump,CLUBS);
    ASSERT_TRUE(ann->make_trump(queen_Hearts,true, 2, trump));
    ASSERT_EQUAL(trump,DIAMONDS);
    ASSERT_TRUE(ann->make_trump(queen_Clubs,false,2,trump));
    ASSERT_EQUAL(trump,SPADES);
    ASSERT_FALSE(ann->make_trump(queen_Hearts, false, 2, trump));
    ASSERT_TRUE(bob->make_trump(king_Hearts, false ,2 , trump));
    ASSERT_EQUAL(trump, DIAMONDS);
    ASSERT_TRUE(bob->make_trump(king_Hearts, false ,2 ,trump));
    ASSERT_EQUAL(trump, DIAMONDS);
    ASSERT_TRUE(bob->make_trump(nine_Clubs,false,2,trump));
    ASSERT_EQUAL(trump, SPADES);
    ASSERT_TRUE(bob->make_trump(nine_Diamonds, false, 2, trump));
    ASSERT_EQUAL(trump, HEARTS);
    ASSERT_FALSE(chelsea->make_trump(nine_Clubs, false, 2, trump));
    ASSERT_FALSE(chelsea->make_trump(nine_Hearts,false,2, trump));
    ASSERT_FALSE(chelsea->make_trump(nine_Spades, false, 2, trump));
    ASSERT_TRUE(chelsea->make_trump(nine_Diamonds,false,2, trump));
    ASSERT_EQUAL(trump, HEARTS);
    delete ann;
    delete bob;
    delete chelsea; 
}

TEST(test_add_and_discard) {
    Player * ann = Player_factory("Ann", "Simple");
    Player * bob = Player_factory("Bob", "Simple");
    ann->add_card(jack_Hearts);
    ann->add_card(jack_Diamonds);
    ann->add_card(queen_Diamonds);
    ann->add_and_discard(nine_Diamonds);
    ASSERT_EQUAL(ann->play_card(jack_Clubs, DIAMONDS), queen_Diamonds);
    ASSERT_EQUAL(ann->play_card(jack_Clubs, DIAMONDS), jack_Hearts);
    ASSERT_EQUAL(ann->play_card(jack_Clubs, DIAMONDS), jack_Diamonds);
    bob->add_card(ace_Hearts);
    bob->add_card(nine_Spades);
    bob->add_and_discard(jack_Spades);
    ASSERT_EQUAL(bob->play_card(king_Diamonds,SPADES), nine_Spades);
    ASSERT_EQUAL(bob->play_card(king_Diamonds,SPADES),jack_Spades)
    delete ann;
    delete bob;
}

TEST(test_lead_card){
    Player * ann = Player_factory("Ann", "Simple");
    Player * bob = Player_factory("Bob", "Simple");
    Player * zach = Player_factory("Zach", "Simple");
    add_cards(ann, ace_Clubs, ten_Clubs, queen_Clubs, king_Clubs, jack_Clubs);
    ASSERT_EQUAL(ann->lead_card(CLUBS),jack_Clubs);
    ann->add_card(jack_Clubs);
    ASSERT_EQUAL(ann->lead_card(SPADES),ace_Clubs);
    ann->add_card(ace_Clubs);
    ASSERT_EQUAL(ann->lead_card(DIAMONDS),ace_Clubs);
    add_cards(bob,nine_Hearts, ten_Hearts, queen_Hearts, ace_Hearts, jack_Diamonds);
    ASSERT_EQUAL(bob->lead_card(HEARTS),jack_Diamonds);
    bob->add_card(jack_Diamonds);
    ASSERT_EQUAL(bob->lead_card(DIAMONDS),ace_Hearts);
    bob->add_card(ace_Hearts);
    ASSERT_EQUAL(bob->lead_card(SPADES),ace_Hearts);
    add_cards(zach, nine_Hearts, nine_Clubs, nine_Spades, nine_Diamonds, ten_Diamonds);
    ASSERT_EQUAL(zach->lead_card(CLUBS), ten_Diamonds);
    zach->add_card(ten_Diamonds);
    ASSERT_EQUAL(zach->lead_card(DIAMONDS),nine_Clubs);
    zach->add_card(nine_Diamonds);
    ASSERT_EQUAL(zach->lead_card(SPADES), ten_Diamonds);
    zach->add_card(ten_Diamonds);
    ASSERT_EQUAL(zach->lead_card(HEARTS),ten_Diamonds);
    delete ann;
    delete bob;
    delete zach;
}

TEST(test_play_card){
    Player * ann = Player_factory("Ann", "Simple");
    Player * bob = Player_factory("Bob", "Simple");
    add_cards(ann,nine_Spades, ten_Spades, queen_Spades, king_Spades, jack_Clubs);
    ASSERT_EQUAL(ann->play_card(nine_Spades,SPADES),jack_Clubs);
    ann->add_card(jack_Clubs);
    ASSERT_EQUAL(ann->play_card(king_Spades,CLUBS),king_Spades);
    ann->add_card(king_Spades);
    ASSERT_EQUAL(ann->play_card(nine_Hearts,SPADES),nine_Spades);
    add_cards(bob,ace_Diamonds,jack_Hearts, nine_Diamonds, nine_Spades, queen_Spades);
    ASSERT_EQUAL(bob->play_card(nine_Clubs,SPADES),nine_Diamonds);
    bob->add_card(nine_Diamonds);
    ASSERT_EQUAL(bob->play_card(king_Spades, HEARTS),queen_Spades);
    bob->add_card(queen_Spades);
    ASSERT_EQUAL(bob->play_card(queen_Clubs,CLUBS), nine_Spades);
    bob->add_card(nine_Spades);
    ASSERT_EQUAL(bob->play_card(queen_Clubs,DIAMONDS),nine_Spades);
    bob->add_card(nine_Spades);
    ASSERT_EQUAL(bob->play_card(ten_Hearts,SPADES),jack_Hearts);
    bob->add_card(jack_Hearts);
    ASSERT_EQUAL(bob->play_card(ten_Diamonds,DIAMONDS),jack_Hearts);
    delete ann;
    delete bob;
}

TEST_MAIN()
