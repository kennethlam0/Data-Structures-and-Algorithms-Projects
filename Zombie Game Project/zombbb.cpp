// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#include <iostream>
#include <vector>
#include <queue>
#include "xgetopt.h"
#include "P2random.h"
using namespace std;


class Zombie
{
    public:
        string getName()
        {
            return name;
        }
        uint32_t getDistance()
        {
            return dist;
        }
        uint32_t getSpeed()
        {
            return speed;
        }
        uint32_t getHealth()
        {
            return health;
        }
        int getRounds()
        {
            return rounds_Active;
        }
        uint32_t getETA()
        {
            return dist/speed;
        }
        void setName(string nameIn)
        {
            name = nameIn;
        }
        void setDistance(uint32_t distanceIn)
        {
            dist = distanceIn;
        }
        void setSpeed(uint32_t speedIn)
        {
            speed = speedIn;
        }
        void setHealth(uint32_t healthIn)
        {
            health = healthIn;
        }
        void updateRoundsActive()
        {
            ++rounds_Active;


        }
        
        void updateDistance()
        {
            dist-=min(dist,speed);
        }

    private:
        string name;
        uint32_t dist;
        uint32_t speed;
        uint32_t health;
        int rounds_Active = 1;
};

class runningMedian
{
    priority_queue<int> lower;
    priority_queue<int> upper;
    public:
        void add(int num)
        {
            lower.push(num);
            upper.push(-lower.top());
            lower.pop();
            if(lower.size()<upper.size())
            {
                lower.push(-upper.top());
                upper.pop();
            }
        }
        double findMedian()
        {
            if(lower.size() > upper.size())
            {
                return lower.top();
            }
            else
            {
                int toplower = lower.top();
                int topupper = -upper.top();
                return (toplower  + topupper) / 2.0;
            }
        }
};

class zombieLess
{
    public:
        bool operator()(Zombie *a, Zombie *b)
        {
            if(a->getRounds()!=b->getRounds())
            {
                return a->getRounds() < b->getRounds();
            }
            return a->getName() > b->getName();
        }
};

class zombieGreater
{
    public:
        bool operator()(Zombie *a, Zombie *b)
        {
            if(a->getRounds()!=b->getRounds())
            {
                return a->getRounds() > b->getRounds();
            }
            return a->getName() > b->getName();
        }
};

class zombieCompare
{
    public:
        bool operator()(Zombie *a, Zombie *b)
        {
            if(a->getETA()==b->getETA() && (a->getHealth()==b->getHealth()))
            {
                return a->getName() > b->getName();
            }
            if(a->getETA()==b->getETA())
            {
                return a->getHealth() > b->getHealth();
            }
            return a->getETA() > b->getETA();
        }
};

bool alive = true;
bool verboseMode = false;
bool medianMode = false;
bool statisticsMode = false;
uint32_t numZombies;
int curr_round = 0;
int quiverCapacity;
uint32_t quiverSize;
int randomSeed;
int maxRandDistance;
int maxRandSpeed;
int maxRandHealth;
int nextRound;
vector<Zombie *> masterList;
vector<std::string> destroyed;
//change to range based for loop eventually
priority_queue<Zombie*, vector<Zombie*>, zombieCompare> zombieQueue;
string killerZombie;
runningMedian median;

void printHelp()
{
    cout << "Help message";
}

void getMode(int argc, char * argv[])
{
    int choice;
    int index = 0;
    option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"verbose", no_argument, nullptr, 'v'},
        {"statistics", required_argument, nullptr, 's'},
        {"median", no_argument, nullptr, 'm'}, 
        {nullptr, 0, nullptr, '\0'},
    };
    while((choice = getopt_long(argc,argv,"s:hvm",long_options, &index))!=-1)
    {
        switch(choice)
        {
            case 'h':
                printHelp();
                exit(0);
            
            case 'v':
                verboseMode = true;
                break;
            case 's':
                statisticsMode = true;  
                numZombies = stoi(optarg);
                break;
            case 'm':
            {
                medianMode = true;
                break;
            }
            default:
                break;
        }
    }
}


void statisticsOutput()
{
    priority_queue<Zombie*,vector<Zombie*>,zombieGreater> zombiesLeastActive(masterList.begin(),masterList.end());
    priority_queue<Zombie*, vector<Zombie*>, zombieLess> zombiesMostActive(masterList.begin(),masterList.end());
    cout << "Zombies still active: " << zombieQueue.size() << "\n";
    cout << "First zombies killed:\n";
    size_t n = min(destroyed.size(),(size_t)numZombies);
    for(auto i = size_t(0); i < n;++i)
    {
        cout << destroyed[i] << " " << i + 1 << "\n";
    }
    cout << "Last zombies killed:\n";
    size_t lastn = n;
    vector<string>::iterator it = destroyed.end();
    while(it!=destroyed.begin())
    {
        --it;
        cout << *it << " " << lastn-- << endl;
        if(lastn==0)
        {
            break;
        }
    }
    n = min(masterList.size(),(size_t)numZombies);
    cout << "Most active zombies:\n";
    for(auto i = size_t(0); i < n; ++i)
    {
        Zombie * mostActiveZombie = zombiesMostActive.top();
        cout << mostActiveZombie->getName() << " " << mostActiveZombie->getRounds() << "\n";
        zombiesMostActive.pop();
    }
    cout << "Least active zombies:\n";
    for(auto i = size_t(0); i < n; ++i)
    {
        Zombie * leastActiveZombie = zombiesLeastActive.top();
        cout << leastActiveZombie->getName() << " " << leastActiveZombie->getRounds() << "\n";
        zombiesLeastActive.pop();
    }
}


void readStarterinfo()
{
    string junk;
    getline(cin,junk);
    string info;
    getline(cin,info,':');
    cin >> quiverCapacity;
    getline(cin,info,':');
    cin >> randomSeed;
    getline(cin,info,':');
    cin >> maxRandDistance;
    getline(cin,info,':');
    cin >> maxRandSpeed;
    getline(cin,info,':');
    cin >> maxRandHealth;
    //read --- in the beginning 
    cin >> junk;
}

void shoot()
{
    while(quiverSize > 0 && !zombieQueue.empty())
    {
        Zombie *target = zombieQueue.top();
        if(target->getHealth() <= quiverSize)
        {
            quiverSize-=target->getHealth();
            target->setHealth(0);
            median.add(target->getRounds());
            destroyed.push_back(target->getName());
            zombieQueue.pop();
            if(verboseMode)
            {
                cout << "Destroyed: " << target->getName() << " (distance: " << target->getDistance() << ", speed: " << target->getSpeed() << ", health: " << target->getHealth() << ")\n";
            }
        }
        else
        {
            target->setHealth(target->getHealth()-quiverSize);
            break;
        }
    }
}

void readNextRound()
{
    string round;
    cin >> round;
    if(round == "round:")
    {
        cin >> round;
        nextRound = stoi(round);
    }
}

void readZombieinfo()
{
    string info;
    while(cin>>info)
    {
        if(info=="---")
        {
            break;
        }
        if(info=="random-zombies:")
        {
            cin >> info;
            size_t numRandomZombies = stoi(info);
            for(auto i = size_t(0); i < numRandomZombies; ++i)
            {
                Zombie *zombie = new Zombie();
                string name = P2random::getNextZombieName();
                uint32_t dist = P2random::getNextZombieDistance();
                uint32_t speed = P2random::getNextZombieSpeed();
                uint32_t health = P2random::getNextZombieHealth();
                zombie->setName(name);
                zombie->setDistance(dist);
                zombie->setSpeed(speed);
                zombie->setHealth(health);
                masterList.push_back(zombie);
                zombieQueue.push(zombie);
                if(verboseMode)
                {
                    cout << "Created: " << zombie->getName() << " (distance: " 
                    << zombie->getDistance() << ", speed: " << zombie->getSpeed() << ", health: " << zombie->getHealth() << ")\n"; 
                }
            }
        }
        else if(info=="named-zombies:")
        {
            cin >> info;
            size_t numNamedZombies = stoi(info);
            for(auto i=size_t(0); i < numNamedZombies; ++i)
            {
                Zombie *zombie = new Zombie();
                cin >> info;
                zombie->setName(info);
                cin >> info;
                cin >> info;
                zombie->setDistance((uint32_t)stoi((info)));
                cin >> info;
                cin >> info;
                zombie->setSpeed((uint32_t)(stoi(info)));
                cin >> info;
                cin >> info;
                zombie->setHealth((uint32_t)(stoi(info)));
                masterList.push_back(zombie);
                zombieQueue.push(zombie);
                if(verboseMode)
                {
                    cout << "Created: " << zombie->getName() << " (distance: " 
                    << zombie->getDistance() << ", speed: " << zombie->getSpeed() << ", health: " << zombie->getHealth() << ")\n"; 
                }
            }
        }
    }
}


void deleteZombies()
{
    for(auto i:masterList)
    {
        delete i;
    }
}

int main(int argc, char * argv[])
{
    //two ways to end game. If i am dead. Or if there are no zombies left and there are no more zombies left in the file. 
    //Check if priority queue is empty for if there are no zombies left. 
    std::ios_base::sync_with_stdio(false);
    getMode(argc,argv);
    readStarterinfo();
    P2random::initialize(randomSeed,maxRandDistance,maxRandSpeed,maxRandHealth);

    //read in ---
    //read in round 
    //read in random zombies nummber
    // read in named zombies number
    // read in named zombies according to the amount of named zombies have named distance speed health
    masterList.reserve(3000);    
    while(true)
    {
        if(curr_round==nextRound)
        {
            readNextRound();
        }
        if(!alive || (zombieQueue.empty() && nextRound <= curr_round))
        {
            break;
        }
        ++curr_round;
        if(verboseMode)
        {
            cout << "Round: " << curr_round << "\n";
        }
        //refill player arrows
        quiverSize = quiverCapacity;
        for(auto zombie: masterList)
        {
            //check that zombie is alive and update distance
            if(zombie->getHealth()!=0)
            {   
                zombie->updateDistance();
                if(verboseMode)
                {
                    cout << "Moved: " << zombie->getName() << " " << "(distance: " << zombie->getDistance() << ", speed: " 
                    << zombie->getSpeed() << ", health: " << zombie->getHealth() << ")\n";
                }
                if(zombie->getDistance()==0)
                {
                    alive = false;
                    if(killerZombie.empty())
                    {
                        killerZombie = zombie->getName();
                    }
                }
            }   
        }
        if(!alive)
        {
            break;
        }
        if(curr_round == nextRound)
        {
            readZombieinfo();
        }
        shoot();
        if(medianMode)
        {
            cout << "At the end of round " << curr_round << ", the median zombie lifetime is " << median.findMedian() << "\n";
        }
        if(!zombieQueue.empty())
        {
            for(Zombie * zombie: masterList)
            {
                if(zombie->getHealth()!=0)
                {
                    zombie->updateRoundsActive();
                }
            }
        }
        // no more zombies left and no more zombies left to read in the file 
    }

    if(!alive)
    {
        cout << "DEFEAT IN ROUND " << curr_round << "! " << killerZombie << " ate your brains!" << "\n";
    }
    if(alive)
    {
        cout << "VICTORY IN ROUND " << curr_round << "! " << destroyed.back() << " was the last zombie." << "\n";
    }
    if(statisticsMode)
    {
        statisticsOutput();
    }
    deleteZombies();
    return 0;
}