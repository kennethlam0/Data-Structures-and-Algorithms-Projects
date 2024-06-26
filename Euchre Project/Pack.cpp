// Project UID 1d9f47bfc76643019cfbf037641defe1
#include "Pack.h"
#include <cassert>
#include <iostream>
#include <array>

Pack::Pack()
{
  int index = 0;
  for(int suit = SPADES; suit <= DIAMONDS; ++suit)
  {
    for(int rank = NINE; rank <= ACE; ++rank)
    {
    cards[index] = Card(static_cast<Rank>(rank),static_cast<Suit>(suit));
    ++index;
    }
  }   
 next = 0;
}


Pack::Pack(std::istream& pack_input)
{
  for(int i = 0; i < PACK_SIZE; ++i)
  {
    std::string str; 
    std::string str_rank;
    std::string str_suit;
    pack_input >> str_rank >> str >> str_suit;
    cards[i]=Card(static_cast<Rank>
    (string_to_rank(str_rank)),static_cast<Suit>(string_to_suit(str_suit)));
  }
  next = 0;
}

Card Pack::deal_one()
{
  return cards[next++];
}

void Pack::reset()
{
  next = 0; 
}

void Pack::shuffle()
{
  std::array<Card,PACK_SIZE> temp;
  for(int i = 0; i < 7; ++i)
  {
    int half = PACK_SIZE / 2;
    for(int j = 0; j < half; ++j)
    {
      temp[j * 2] = cards[j + half];
      temp[j * 2 + 1] = cards[j];
    }
    for(int j = 0; j < PACK_SIZE; ++j)
    {
    cards[j] = temp[j];
    }
  }
  reset();
}

bool Pack::empty() const
{
  if(next==PACK_SIZE)
  {
    return true;
  }
  else
  {
    return false;
  }
}
