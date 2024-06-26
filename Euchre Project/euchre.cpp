// Project UID 1d9f47bfc76643019cfbf037641defe1
#include <iostream> 
#include <cstdlib>
#include <fstream>
#include "Player.h"
#include "Pack.h"
#include <cstring>
using namespace std;

class Game
{
    public:
        Game(std::vector<Player*> player_list, 
        Pack pack_in, int points_to_win, std::string shuffle_in)
        {
            players = player_list;
            pack = pack_in;
            dealer_index = 0;
            team1_points = 0;
            team2_points = 0;
            points_win = points_to_win;
            shuffle_or_not = shuffle_in;
        }

        void shuffle_game()
        {
            if(shuffle_or_not == "shuffle")
            {
                pack.shuffle();
            }
            else
            {
                pack.reset();
            }
        }

        string get_player_name(int index)
        {
            return players[index]->get_name();
        }

        void deal_two_cards(Player * player)
        {
            player->add_card(pack.deal_one());
            player->add_card(pack.deal_one());
        }

        void deal_three_cards(Player * player)
        {
            player->add_card(pack.deal_one());
            player->add_card(pack.deal_one());
            player->add_card(pack.deal_one());
        }

        void deal()
        {   
            deal_three_cards(players[(dealer_index + 1) % 4]);
            deal_two_cards(players[(dealer_index + 2) % 4]);
            deal_three_cards(players[(dealer_index + 3) % 4]);
            deal_two_cards(players[dealer_index]);
            deal_two_cards(players[(dealer_index + 1) % 4]);
            deal_three_cards(players[(dealer_index + 2) % 4]);
            deal_two_cards(players[(dealer_index + 3) % 4]);
            deal_three_cards(players[dealer_index]);
            upcard = pack.deal_one();
        }

        void switch_eldest_hand()
        {
            eldest_hand_index = (dealer_index + 1) % 4;
        }

        void reset_team_tricks()
        {
            team1_tricks = 0;
            team2_tricks = 0;
        }

        void make_trump_game()
        {  
            Suit proposed_suit;
            int making_trumpindex = 0;
            dealer_index = dealer_index % 4; 
            round = 1;
            for(int i = eldest_hand_index; i < eldest_hand_index + 4; ++i)
            {
                making_trumpindex = i % 4;
                bool is_dealer = (making_trumpindex==dealer_index);
                bool order_made = 
                players[making_trumpindex]->
                make_trump(upcard,is_dealer,round,proposed_suit);
                if(order_made)
                {
                    cout << get_player_name(making_trumpindex)
                    << " orders up " << proposed_suit << endl << endl;
                    trump_suit = proposed_suit;
                    players[dealer_index]->add_and_discard(upcard);
                    player_ordered_up = making_trumpindex;
                    return;
                }
                else
                {
                    cout << get_player_name(making_trumpindex) << " passes" << endl;
                }
            }
            round = 2;
            for(int i = eldest_hand_index; i < eldest_hand_index + 4; ++i)
            {
                making_trumpindex = i % 4;
                bool is_dealer = (making_trumpindex==dealer_index);    
                bool order_made = 
                players[making_trumpindex]->
                make_trump(upcard,is_dealer,round,proposed_suit);
                if(order_made)
                {
                    cout << get_player_name(making_trumpindex)
                    << " orders up " << proposed_suit << endl << endl;
                    trump_suit = proposed_suit;
                    player_ordered_up = making_trumpindex;
                    return;
                }
                else
                {
                    cout << get_player_name(making_trumpindex) << " passes" << endl;
                }
            }
        }

        void scoring()
        {
            string team1_names = get_player_name(0) + " and " + get_player_name(2);
            string team2_names = get_player_name(1) + " and " + get_player_name(3);
            if(team1_tricks > team2_tricks)
            {
                if(player_ordered_up % 2==0)
                {
                    if(team1_tricks == 3 || team1_tricks==4)
                    {
                        team1_points+=1;
                        cout << team1_names << " win the hand" << endl;
                    }
                    else if(team1_tricks==5)
                    {
                        team1_points+=2;
                        cout << team1_names << " win the hand" << endl;
                        cout << "march!" << endl;
                    }
			    }
			    else
			    {
                    team1_points+=2;
                    cout << team1_names << " win the hand" << endl;
                    cout << "euchred!" << endl;
			    }
		    }
		    else if(team2_tricks > team1_tricks)
		    {
			    if(player_ordered_up % 2==1)
			    {
				    if(team2_tricks==3 || team2_tricks==4)
				    {
                        team2_points+=1;
                        cout << team2_names << " win the hand" << endl;
                    }

				    else if(team2_tricks==5)
                    {
                        team2_points+=2;
                        cout << team2_names << " win the hand" << endl;
                        cout << "march!" << endl;
                    }
			    }
			    else
                {
                    team2_points+=2;
                    cout << team2_names << " win the hand" << endl;
                    cout << "euchred!" << endl;
                }
		    }
            cout << team1_names << " have " << team1_points << " points" << endl;
            cout << team2_names << " have " << team2_points << " points" 
            << endl << endl;
        }

        void play_trick()
        {
            int leader = eldest_hand_index % 4;
            for(int trick = 0; trick < 5; ++trick)
            {
                led = players[leader]->lead_card(trump_suit);
                highest_card = led;
                trick_Winner = leader;
                cout << led << " led by " << get_player_name(leader) << endl;
                for(int i = 1; i < 4; ++i)
                {
                    Card played = players[(leader+i)%4]->play_card(led,trump_suit);
                    cout << played << " played by " << get_player_name((leader+i)%4) 
                    << endl;
                    if(Card_less(highest_card,played,led,trump_suit))
                    {
                        highest_card=played;
                        trick_Winner = (leader+i)%4;
                    }
                }
                if(trick_Winner % 2==0)
                {
                    ++team1_tricks;
                }
                else if(trick_Winner % 2==1)
                {
                    ++team2_tricks;
                }
                cout << get_player_name(trick_Winner) << " takes the trick\n\n";
                leader = trick_Winner;              
            }   
        }
        void determine_winner()
        {
            if(team1_points > team2_points)
            {
                cout << get_player_name(0) + " and " + get_player_name(2) 
                << " win!" << endl;
            }
            else
            {
                cout << get_player_name(1) + " and " + get_player_name(3)
                 << " win!" << endl;
             }
        }

        void switch_dealer()
        {
            dealer_index = (dealer_index + 1) % 4; 
        }

        void play()
        {
            while(team1_points < points_win && team2_points < points_win)
            {
                cout << "Hand " << hand << endl;
                cout << get_player_name(dealer_index % 4) << " deals" << endl;
                shuffle_game();
                deal();
                switch_eldest_hand();
                cout << upcard << " turned up" << endl;
                make_trump_game();
                play_trick();  
                scoring();
                ++hand;
                reset_team_tricks();
                switch_dealer();
            }
            determine_winner();
        }

    private:
        std::vector<Player *> players;
        Pack pack;
        int dealer_index = 0;
        int team1_tricks = 0;
        int team2_tricks = 0;
        int round = 1;
        int team1_points = 0;
        int team2_points = 0;
        int eldest_hand_index = 0;
        int hand = 0;
        int points_win;
        std::string shuffle_or_not;
        Card upcard;
        Suit trump_suit;
        Card led;
        Card played;
        Card highest_card;
        int player_ordered_up;
        int trick_Winner;
};


int main(int argc, char *argv[])
{
    if(argc!=12)
    {
        std::cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }  
    int points_to_win = atoi(argv[3]);
    if(points_to_win < 1 || points_to_win > 100)
    {
        std::cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }
    string shuffle_or_not = argv[2];
    if(shuffle_or_not!= "shuffle" && shuffle_or_not!="noshuffle")
    {
        std::cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }
    for(int i = 5; i < 11; i+=2)
    {
        string human_or_simple = argv[i];
        if(human_or_simple!="Simple" && human_or_simple!="Human")
        {
            std::cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
            return 1;
        } 
    }
    string pack_filename = argv[1];
    ifstream fin;
    fin.open(pack_filename);
    if(!fin.is_open())
    {
        std::cout << "Error opening " << pack_filename << endl;
        return 1;
    }
    Pack pack(fin);
    Player * player0 = Player_factory(argv[4],argv[5]);
    Player * player1 = Player_factory(argv[6],argv[7]);
    Player * player2 = Player_factory(argv[8],argv[9]);
    Player * player3 = Player_factory(argv[10],argv[11]);
    std::vector<Player*> players_list;
    players_list = {player0,player1,player2,player3};
    for(int i = 0; i < 12; ++i)
    {
        cout << argv[i] << " ";
    }
    cout << endl;
    Game game(players_list, pack, points_to_win, shuffle_or_not);
    game.play();
    for(size_t i = 0; i < players_list.size();++i)
    {
        delete players_list[i];
    }
    fin.close();
    return 0;
}

