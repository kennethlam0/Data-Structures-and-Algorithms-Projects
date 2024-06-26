// Project UID 1d9f47bfc76643019cfbf037641defe1


#include "Player.h"
#include <cassert>
#include <iostream>
#include <array>
#include <algorithm>
using namespace std;

class SimplePlayer: public Player{
  public:
    SimplePlayer(const std::string &n)
    {
      name = n; 
    }

    const std::string & get_name() const override
    {
      return name; 
    }

    void add_card(const Card &c) override
    {
      hand.push_back(c);
    }

    int count_faceace_cards(const std::vector<Card>& hand, Suit suit) const 
    {
      int count = 0;
      for(const Card& card: hand)
      {
        if((card.get_suit()==suit || card.is_left_bower(suit)) && card.is_face_or_ace())
        {
          ++count;
        }
      }
      return count;
    }

    bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override
    {
      int num_face_cards = count_faceace_cards(hand,upcard.get_suit());
      if(round==1)
      {
        if(num_face_cards>=2)
        {
          order_up_suit = upcard.get_suit();
          return true;
        }
        else
        {
          return false; 
        }
      }
      else if(round==2)
      {
        if(is_dealer)
        {
          order_up_suit  = Suit_next(upcard.get_suit());
          return true; 
        }
        else
        {
          Suit same_color_suit = Suit_next(upcard.get_suit());
          int num_same_color_suit = count_faceace_cards(hand,same_color_suit);
          if(num_same_color_suit>=1) 
          {
            order_up_suit = same_color_suit;
            return true;
          }
          else
          {
            return false;
          }
        }
      }
      return false;
    }

    void add_and_discard(const Card &upcard)  override
    {
      add_card(upcard);
      int lowest_card_index = 0;
      for(int i = 0; i < hand.size(); ++i)
      {
        if(Card_less(hand[i],hand[lowest_card_index],upcard.get_suit()))
        {
          lowest_card_index = i;
        }
      }
      hand.erase(hand.begin() + lowest_card_index);
    }

    Card lead_card(Suit trump) override
    {
      Card highest_non_trump;
      bool has_non_trump = false;
      for(auto& card:hand)
      {
        if(!card.is_trump(trump))
        {
          if(!has_non_trump || card > highest_non_trump)
          {
            highest_non_trump = card;
            has_non_trump = true;   
          }
        }
      }
      if(has_non_trump)
      {
        for(int i = 0; i < hand.size(); ++i)
        {
          if(hand[i]==highest_non_trump)
          {
            hand.erase(hand.begin() + i);
            return highest_non_trump;
            }
          }
        }
        Card highest_trump;
        for(auto &card:hand)
        {
          if(card.is_trump(trump) && Card_less(highest_trump,card,trump))
          {
            highest_trump = card;
          }
        }
        for(int i = 0; i < hand.size(); ++i)
        {
          if(hand[i]==highest_trump)
          {
            hand.erase(hand.begin() + i);
            return highest_trump;
          }
        }
        hand.erase(hand.begin());
        return hand.front();
    }

    Card play_card(const Card &led_card, Suit trump) override
    {
      Card highest_follow_suit;
      bool can_follow_suit = false;
      for(auto& card: hand)
      {
        if(card.get_suit(trump) == led_card.get_suit(trump))
        {
          if(!can_follow_suit || Card_less(highest_follow_suit,card,led_card,trump))
          {
            can_follow_suit = true;
            highest_follow_suit = card;
          }
        }
      }
      if(can_follow_suit)
      {
        for(int i = 0; i < hand.size(); ++i)
        {   
          if(hand[i] == highest_follow_suit)
          {
            hand.erase(hand.begin() + i);
            return highest_follow_suit;
          }
        }
      }
      else
      {
        Card lowest_card = hand.front();
        for(auto& card: hand)
        {
          if(Card_less(card,lowest_card,led_card,trump))
          {
            lowest_card = card;
          }
        }
        for(int i = 0; i < hand.size(); ++i)
        {
          if(hand[i] == lowest_card)
          {
            hand.erase(hand.begin() + i);
            return lowest_card;
          }
        }
      }
      return Card();
    }


  private:
    std::string name;
    std::vector<Card> hand;
};

class HumanPlayer: public Player
{
  public:
    HumanPlayer(const std::string &n)
    {
      name = n;
    }
    const std::string & get_name() const override
    {
      return name;
    }
    void add_card(const Card &c) override
    {
      hand.push_back(c);
      std::sort(hand.begin(), hand.end());
    }
    
    bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override
    {
      print_hand();
      cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
      std::string decision;
      cin >> decision; 
      if(decision == "pass")
      {
        return false;
      }
      order_up_suit = string_to_suit(decision);
      return true;
    }

    void add_and_discard(const Card &upcard) override
    {
      print_hand();
      size_t choice;
      hand.push_back(upcard);
      cout << "Discard upcard: [-1]\n";
      cout << "Human player " << name << ", please select a card to discard:\n\n";
      cin >> choice;
      if(choice == -1)
      {
        for(size_t i = 0; i < hand.size(); ++i)
        {
          if(hand[i]==upcard)
          {
            hand.erase(hand.begin() + i);
          }
        }
      }
      else
      {
        hand.erase(hand.begin() + choice);
      }
      std::sort(hand.begin(),hand.end());
    }

    Card lead_card(Suit trump) override
    {
      print_hand();
      int choice; 
      cout << "Human player " << name << ", please select a card:\n";
      cin >> choice;
      Card played_card = hand[choice];
      hand.erase(hand.begin() + choice);
      return played_card;
    }

    Card play_card(const Card &led_card, Suit trump) override
    {
      print_hand();
      int choice;
      cout << "Human player " << name << ", please select a card:\n";
      cin >> choice;
      Card played_card = hand[choice];
      hand.erase(hand.begin() + choice);  
      return played_card;
    }


  private:
    std::string name;
    std::vector<Card> hand;
    void print_hand() const
    {
      for(int i=0; i < hand.size(); ++i)
      {
        cout << "Human player " << name << "'s hand: "
        << "[" << i << "] " << hand[i] <<"\n";
      }
    }
};

Player * Player_factory(const std::string &name, const std::string &strategy)
{
  if(strategy=="Simple")
  {
    return new SimplePlayer(name);
  }
  else if(strategy=="Human")
  {
    return new HumanPlayer(name);
  }
  return nullptr;
}

std::ostream & operator<<(std::ostream &os, const Player &p)
{
  os << p.get_name();
  return os;
}