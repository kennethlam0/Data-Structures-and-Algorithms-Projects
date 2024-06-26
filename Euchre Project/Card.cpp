// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <array>
#include "Card.h"

using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  is >> str;
  rank = string_to_rank(str);
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  std::string str;
  is >> str;
  suit = string_to_suit(str);
  return is;
}


/////////////// Write your implementation for Card below ///////////////
Card::Card()
{
  rank = TWO;
  suit = SPADES;
}

Card::Card(Rank rank_in, Suit suit_in)
{
  rank = rank_in;
  suit = suit_in;
}

 Rank Card::get_rank() const
 {
  return rank; 
 }

Suit Card::get_suit() const
{
  return suit;
}

Suit Card::get_suit(Suit trump) const
{
  if(is_left_bower(trump)==true)
  {
    return trump;
  }
  else
  {
    return suit;
  }
}

bool Card::is_face_or_ace() const
{
  if(rank==JACK || rank==QUEEN || rank==KING || rank==ACE)
  {
    return true;
  }
  else
  {
    return false; 
  }
}

bool Card::is_left_bower(Suit trump) const
{
  return rank==JACK && suit==Suit_next(trump);
}

bool Card::is_right_bower(Suit trump) const
{
  if(suit==trump && rank==JACK)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Card::is_trump(Suit trump) const
{
  if(suit==trump)
  {
    return true;
  }
  else if(is_left_bower(trump))
  {
    return true;
  }
  else
  {
    return false;
  }
}

std::ostream & operator<<(std::ostream &os, const Card &card)
{
  os << card.get_rank() << " of " << card.get_suit();
  return os;
}

std::istream & operator>>(std::istream &is, Card &card)
{
  std::string str;
  std::string str_rank;
  std::string str_suit; 
  is >> str_rank >> str >> str_suit;
  card.rank = string_to_rank(str_rank);
  card.suit = string_to_suit(str_suit);
  return is;
}


bool operator<(const Card &lhs, const Card &rhs)
{
  if(lhs.get_rank()!=rhs.get_rank())
  {
    return lhs.get_rank() < rhs.get_rank();
  }
  else
  {
    return lhs.get_suit() < rhs.get_suit();
  }
}

//EFFECTS Returns true if lhs is lower value than rhs or the same card as rhs.
//  Does not consider trump.
bool operator<=(const Card &lhs, const Card &rhs)
{
  return !(rhs<lhs);
}

//EFFECTS Returns true if lhs is higher value than rhs.
//  Does not consider trump.
bool operator>(const Card &lhs, const Card &rhs)
{
   return rhs < lhs;
}

//EFFECTS Returns true if lhs is higher value than rhs or the same card as rhs.
//  Does not consider trump.
bool operator>=(const Card &lhs, const Card &rhs)
{
  return !(lhs < rhs);
}

//EFFECTS Returns true if lhs is same card as rhs.
//  Does not consider trump.
bool operator==(const Card &lhs, const Card &rhs)
{
  if(lhs.get_rank()==rhs.get_rank() && lhs.get_suit()==rhs.get_suit())
  {
    return true;
  }
  else
  {
    return false;
  }
}

//EFFECTS Returns true if lhs is not the same card as rhs.
//  Does not consider trump.
bool operator!=(const Card &lhs, const Card &rhs)
{
  return !(lhs==rhs);
}

//EFFECTS returns the next suit, which is the suit of the same color
Suit Suit_next(Suit suit)
{
  if(suit==SPADES)
  {
    return CLUBS;
  }
  else if(suit==HEARTS)
  {
    return DIAMONDS;
  }
  else if(suit==CLUBS)
  {
    return SPADES;
  }
  else if(suit==DIAMONDS)
  {
    return HEARTS;
  }
  else
  {
    return SPADES;
  }
}

//EFFECTS Returns true if a is lower value than b.  Uses trump to determine
// order, as described in the spec.
bool Card_less(const Card &a, const Card &b, Suit trump)
{
  if(a.is_right_bower(trump))
  {
    return false; 
  }
  else if(b.is_right_bower(trump))
  {
    return true;
  }
  else if(a.is_left_bower(trump))
  {
    return false;
  }
  else if(b.is_left_bower(trump))
  {
    return true;
  }
  else if(a.is_trump(trump) && !b.is_trump(trump))
  {
    return false;
  }
  else if(!a.is_trump(trump) && b.is_trump(trump))
  {
    return true; 
  }
  return a < b; 
}

//EFFECTS Returns true if a is lower value than b.  Uses both the trump suit
//  and the suit led to determine order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump)
{
  //led suit is trump suit led suit spades trump suit spades a hearts b is a spades
  //a is jack of hearts b is ten of diamonds a is ten of diamonds b is jack of hearts 
  Suit led_suit = led_card.get_suit(trump);
  if(a.get_suit(trump)==b.get_suit(trump))
  {
    return Card_less(a,b,trump);
  }
  else if((a.get_suit(trump)==led_suit && b.get_suit(trump)==trump))
  {
    return true;
  }
  else if(a.get_suit(trump)==led_suit && !(b.get_suit(trump)==trump))
  {
    return false;
  }
  else if(b.get_suit(trump)==led_suit && a.get_suit(trump)==trump)
  {
    return false;
  }
  else if(b.get_suit(trump)==led_suit && !(a.get_suit(trump)==trump))
  {
    return true;
  }
  else
  {
    return Card_less(a,b,trump);
  }
  //a is led suit b is trump
  //a is led suit b is non trump
  //b is led suit a is trump
  //b is led suit a is non trump 
  //neither are led suit 

  return false;
}


