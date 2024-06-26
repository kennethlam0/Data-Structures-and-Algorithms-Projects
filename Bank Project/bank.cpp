// Project identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98
#include <iostream>
#include <unordered_map>
#include <fstream>
#include "xgetopt.h"
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <cmath>
#include <unordered_set>
#include <sstream>
using namespace std;

bool verboseMode = false;
bool violation = false;
string fileName;
ifstream regFile;
int transactionsIDGen = 0;
int execIndex = 0; 
struct User
{
    string id;
    uint64_t ts;
    int pin;
    int balance;
    unordered_set<string> ipAddresses;
};

struct Transaction 
{
    int transactionID;
    int amount;
    string sender;
    string recipient;
    uint64_t exec_date;
    char cover;
    int index;
};

class TransactionComp
{
    public:
        bool operator()(const Transaction &a, const Transaction &b)
        {
            if(a.exec_date == b.exec_date)
            {
                return a.transactionID > b.transactionID;
            }
            return a.exec_date > b.exec_date;
        }
};



unordered_map<string, User> user_Info;
priority_queue<Transaction,vector<Transaction>,TransactionComp> transaction_Queue;
multimap<uint64_t,int> transaction_execid;
multimap<string,int> incomingHistory;
multimap<string,int> outgoingHistory;
vector<Transaction> master_Transactions;

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
        {"file", required_argument, nullptr, 'f'}, 
        {nullptr, 0, nullptr, '\0'},
    };
    while((choice = getopt_long(argc,argv,"f:hv",long_options, &index))!=-1)
    {
        switch(choice)
        {
            case 'h':
                printHelp();
                exit(0);
            
            case 'v':
                verboseMode = true;
                break;
            case 'f':
            {
                fileName = optarg;
            }
            default:
                break;
        }
    }
}

void login(string id, int pin, string ip)
{
    if(user_Info.find(id)!=user_Info.end())
    {
        if(user_Info[id].pin == pin)
        {
            user_Info[id].ipAddresses.insert(ip); 
            if(verboseMode)
            {
            cout << "User " << id << " logged in.\n";
            }
        }
        else
        {
            if(verboseMode)
            {
                cout << "Failed to log in " << id << ".\n";
            }
        }
    }
    else
    {
        if(verboseMode)
        {
            cout << "Failed to log in " << id << ".\n";
        }
    }
}

bool place(uint64_t ts, string ip, string sender, string recipient, uint64_t exec_date)
{
    bool violation = false;
    if(exec_date - ts > 3000000)
    {
        if(verboseMode)
        {
            cout << "Select a time less than three days in the future.\n";
        }
        violation = true;
    }
    else if(user_Info.find(sender)==user_Info.end())
    {
        if(verboseMode)
        {
            cout << "Sender " << sender << " does not exist.\n";
        }
        violation = true;
    }
    else if(user_Info.find(recipient)==user_Info.end())
    {
        if(verboseMode)
        {
            cout << "Recipient " << recipient << " does not exist.\n"; 
        }
        violation = true;
    }
    else if(exec_date < user_Info[sender].ts || exec_date < user_Info[recipient].ts)
    {
        if(verboseMode)
        {
            cout << "At the time of execution, sender and/or recipient have not registered.\n";
        }
        violation = true;
    }
    else if(user_Info[sender].ipAddresses.empty())
    {
        if(verboseMode)
        {
             cout << "Sender " << sender << " is not logged in.\n";
        }
        violation = true;
    }
    //fraud check: ip address of originating transaction is not in valid of IP addresses for sender
    else if(user_Info[sender].ipAddresses.find(ip)==user_Info[sender].ipAddresses.end())
    {
        if(verboseMode)
        {
            cout << "Fraudulent transaction detected, aborting request.\n";
        }
        violation = true;
    }
    return violation;
}

pair<int,int> calcTransactionFee(const Transaction &transaction)
{
    int fee = transaction.amount/100;
    //discount o case
    if(fee < 10)
    {
        fee = 10;
    }
    if(fee > 450)
    {
        fee = 450;
    }
    if(transaction.cover=='o')
    {
        if((transaction.exec_date-user_Info[transaction.sender].ts) > 50000000000)
        {
            fee = (fee * 3)/4;
        }
        return make_pair(fee,0);
    }
    else if(transaction.cover=='s')
    {
        if((transaction.exec_date-user_Info[transaction.sender].ts) > 50000000000)
        {
            fee = (fee*3)/4;
        }
        if(fee%2==0)
        {
            return make_pair(fee/2,fee/2);
        }
        if(fee%2==1)
        {
            return make_pair(fee/2+1,fee/2);
        }
    }   
    return {0,0};
}
//cover o sender covers, s shared equally between sender and recipient
void execute(const Transaction &transaction)
{
    // o case. Sender covers check if sender has enough money to cover amount + transaction fees
    bool violation = false;
    auto totalFee = calcTransactionFee(transaction);
    int transactionFee_sender = totalFee.first;
    int transactionFee_recipient = totalFee.second;
    if(transaction.cover == 'o')
    {
        if(user_Info[transaction.sender].balance-(transaction.amount + transactionFee_sender) < 0)
        {
            if(verboseMode)   
            {
                cout << "Insufficient funds to process transaction " << transaction.transactionID << ".\n";
            }
            transaction_Queue.pop();
            violation = true;
        }
    }
    //s case. transaction fee share equally
    if(transaction.cover == 's')
    {
        if(user_Info[transaction.recipient].balance-transactionFee_recipient < 0 ||
        user_Info[transaction.sender].balance-(transaction.amount + transactionFee_sender)<0)
        {
            if(verboseMode)
            {
                cout << "Insufficient funds to process transaction " << transaction.transactionID << ".\n";
            }
            transaction_Queue.pop();
            violation = true;
        }
    }
    if(violation == false)
    {
        Transaction curr_transaction = transaction_Queue.top();
        master_Transactions.emplace_back(curr_transaction);
        transaction_execid.insert({curr_transaction.exec_date,execIndex});
        incomingHistory.insert({curr_transaction.recipient,execIndex});
        outgoingHistory.insert({curr_transaction.sender,execIndex});
        ++execIndex;
        if(transaction.cover == 'o')
        {
            user_Info[transaction.sender].balance -= (transaction.amount + transactionFee_sender);
            user_Info[transaction.recipient].balance += transaction.amount;
        }
        if(transaction.cover == 's')
        {
            user_Info[transaction.sender].balance -= (transaction.amount + transactionFee_sender);
            user_Info[transaction.recipient].balance-=transactionFee_recipient; 
            user_Info[transaction.recipient].balance +=transaction.amount;
        }
        Transaction transaction_Top = transaction_Queue.top();
        if(verboseMode)
        {
            cout << "Transaction executed at " << transaction_Top.exec_date << ": $" << transaction_Top.amount 
            << " from " << transaction.sender << " to " << transaction.recipient << ".\n";
        }
        transaction_Queue.pop();
    }
}
void logout(string id, string ip)
{
    //there is an ipaddress associated with id
    if(user_Info.find(id)!=user_Info.end())
    {
        if(user_Info[id].ipAddresses.find(ip)!=user_Info[id].ipAddresses.end())
        {
            user_Info[id].ipAddresses.erase(ip);
            if(verboseMode)
            {
                cout << "User " << id << " logged out" << ".\n";
            }
        }   
         else
        {
            if(verboseMode)
            {
                cout << "Failed to log out " << id << ".\n";
            }
        }
    } 
}
void operations()
{
    uint64_t prev_ts = 0;
    string info;
    while(cin>>info)
    {
        if(info.substr(0,1)=="#")
        {
            getline(cin,info);
        }
        else if(info[0]=='l')
        {
            string id;
            int pin;
            string ip;
            cin >> id >> pin >> ip;
            login(id,pin,ip);
        }
        else if(info[0]=='o')
        {
            string id;
            string ip;
            cin >> id >> ip;
            logout(id,ip);
        }
        else if(info[0]=='p')
        {
            string ts;
            string ip;
            string sender;
            string recipient;
            int amount;
            string exec_date;
            char cover;
            cin >> ts >> ip >> sender >> recipient >> amount >> exec_date >> cover;
            ts.erase(remove(ts.begin(),ts.end(),':'),ts.end());
            if(prev_ts!=0)
            {
                if(stoul(ts) < prev_ts)
                {
                    cerr << "Invalid decreasing timestamp in place command";
                    exit(1);
                }
            }
            prev_ts = stoul(ts);
            exec_date.erase(remove(exec_date.begin(),exec_date.end(),':'),exec_date.end());
            if(stoul(exec_date) < stoul(ts))
            {
                cerr << "You cannot have an execution date before the current timestamp.";
                exit(1);
            }   
            bool violation = place(stoul(ts),ip,sender,recipient,stoul(exec_date));
            while(!transaction_Queue.empty() && (stoul(ts) >= transaction_Queue.top().exec_date) && violation==false)
            {
                Transaction transaction_Top = transaction_Queue.top();
                execute(transaction_Top);
            }
            if(violation == false)
            {
                 if(verboseMode)
                {
                    cout << "Transaction placed at " << stoul(ts) << ": $" << amount << " from " << sender << " to " << recipient << " at " << stoul(exec_date) << ".\n";  
                }
                transaction_Queue.push({transactionsIDGen, amount,sender,recipient,stoul(exec_date),cover,execIndex});
                ++transactionsIDGen;
            }
        }
        else if(info.substr(0,1)=="$")
        {
            while(!transaction_Queue.empty())
            {
                execute(transaction_Queue.top());
            }
            break;
        }
    }
}

void query()
{
    string info;
    while(cin>>info)
    {
        if(info[0]=='l')
        {
            string ts_before;
            string ts_after;   
            cin >> ts_before >> ts_after;
            ts_before.erase(remove(ts_before.begin(),ts_before.end(),':'),ts_before.end());
            ts_after.erase(remove(ts_after.begin(),ts_after.end(),':'),ts_after.end());
            auto it = transaction_execid.lower_bound(stoul(ts_before));
            int num_transactions = 0;
            while(it!=transaction_execid.end() && (*it).first < stoul(ts_after))
            {
                int index = (*it).second;
                Transaction transaction = master_Transactions[index];
                if(master_Transactions[index].amount!=1)
                {
                    cout << transaction.transactionID << ": " << transaction.sender 
                << " sent " << transaction.amount << " dollars to " << transaction.recipient << " at " << transaction.exec_date << ".\n";
                }
                else
                {
                    cout << transaction.transactionID << ": " << transaction.sender 
                << " sent " << transaction.amount << " dollar to " << transaction.recipient << " at " << transaction.exec_date << ".\n";
                }
                ++it; 
                ++num_transactions;
            }
            if(num_transactions==1)
            {
                cout << "There was 1 transaction that was placed between time " << stoul(ts_before) << " to " << stoul(ts_after) << ".\n";;

            }
            else
            {
                cout << "There were " << num_transactions << " transactions that were placed between time " << stoul(ts_before) << " to " << stoul(ts_after) << ".\n";;

            }
        }
        else if(info[0]=='r')
        {
            string ts_before;
            string ts_after;   
            cin >> ts_before >> ts_after;
            ts_before.erase(remove(ts_before.begin(),ts_before.end(),':'),ts_before.end());
            ts_after.erase(remove(ts_after.begin(),ts_after.end(),':'),ts_after.end());
            auto it = transaction_execid.lower_bound(stoul(ts_before));
            int bankRev = 0; 
            while(it!=transaction_execid.end() && (*it).first < stoul(ts_after))
            {
                int index = (*it).second;
                Transaction transaction = master_Transactions[index];
                auto totalfee = calcTransactionFee(transaction);
                bankRev += (totalfee.first + totalfee.second);
                ++it;
            }
            ostringstream os;
            os << "281Bank has collected " << bankRev << " dollars in fees over" ;
            uint64_t interval = stoul(ts_after) - stoul(ts_before);
            uint64_t years = interval / 10000000000;
            uint64_t months = (interval % 10000000000) / 100000000;
            uint64_t days = (interval % 100000000) / 1000000;
            uint64_t hours = (interval % 1000000) / 10000;
            uint64_t minutes = (interval % 10000)/100;
            uint64_t seconds = interval % 100;
            if(years!=0)
            {
                if(years != 1)
                {
                    os << " " << years << " years";
                }
                else
                {
                    os << " " << years << " year"; 
                }
            }
            if(months!=0)
            {
                if(months!=1)
                {
                    os << " " << months << " months";
                }
                else
                {
                    os << " " << months << " month";
                }
            }            
            if(days!=0)
            {
                if(days!=1)
                {
                    os << " " << days << " days";
                }
                else
                {
                    os << " " << days << " day";
                }
            }
            if(hours!=0)
            {
                if(hours!=1)
                {
                    os << " " << hours << " hours";
                }
                else
                {
                    os << " " << hours << " hour";
                }
            }
            if(minutes!=0)
            {
                if(minutes!=1)
                {
                    os << " " << minutes << " minutes";
                }
                else
                {
                    os << " " << minutes << " minute";
                }
            }
            if(seconds!=0)
            {
                if(seconds!=1)
                {
                    os << " " << seconds << " seconds";
                }
                else
                {
                    os << " " << seconds << " second";
                }
            }
            os << ".\n";
            cout << os.str();
        }
        else if(info[0] == 'h')
        {
            string name;
            cin >> name;
            if(user_Info.find(name)==user_Info.end())
            {
                cout << "User " << name << " does not exist.\n";
            }
            else
            {
                auto incomingCount = incomingHistory.count(name);
                auto outgoingCount = outgoingHistory.count(name);
                auto totalTransaction = (incomingCount + outgoingCount);
                if(incomingCount>10)
                {
                    auto incoming_reverse = incomingHistory.upper_bound(name);
                    cout << "Customer " << name << " account summary:\n";
                    cout << "Balance: $" << user_Info[name].balance << endl;
                    cout << "Total # of transactions: " << totalTransaction << endl;
                    cout << "Incoming " << incomingCount <<":\n";
                    for(int i = 0; i < 10; ++i)
                    {
                        --incoming_reverse;
                    }
                    while(incoming_reverse!=incomingHistory.upper_bound(name) && (*incoming_reverse).first==name)
                    {
                        int index = (*incoming_reverse).second;
                        Transaction transaction = master_Transactions[index];
                        if(transaction.amount==1)
                        {
                            cout << transaction.transactionID << ": " << transaction.sender << " sent 1 dollar to " << name << " at " <<
                        transaction.exec_date << ".\n";
                        }
                        else
                        {
                            cout << transaction.transactionID << ": " << transaction.sender << " sent " << transaction.amount << " dollars to " << name << " at " <<
                        transaction.exec_date << ".\n";
                        }
                        ++incoming_reverse;
                    }
                }
                else
                {
                    auto incoming = incomingHistory.lower_bound(name);
                    cout << "Customer " << name << " account summary:\n";
                    cout << "Balance: $" << user_Info[name].balance << endl; 
                    cout << "Total # of transactions: " << totalTransaction << endl;
                    cout << "Incoming " << incomingCount <<":\n";
                    while(incoming!=incomingHistory.end() && (*incoming).first == name)
                    {
                        int index = (*incoming).second;
                        Transaction transaction = master_Transactions[index];
                        if(transaction.amount==1)
                        {
                            cout << transaction.transactionID << ": " << transaction.sender << " sent 1 dollar to " << name << " at " <<
                        transaction.exec_date << ".\n";
                        }
                        else
                        {
                            cout << transaction.transactionID << ": " << transaction.sender << " sent " << transaction.amount << " dollars to " << name << " at " <<
                        transaction.exec_date << ".\n";
                        }
                        ++incoming;
                    }
                }
                if(outgoingCount>10)
                {
                    auto outgoing_reverse = outgoingHistory.upper_bound(name);
                    for(int i = 0; i < 10; ++i)
                    {
                        --outgoing_reverse;
                    }
                    cout << "Outgoing " << outgoingCount << ":\n";
                    while(outgoing_reverse!=outgoingHistory.upper_bound(name) && (*outgoing_reverse).first==name)
                    {
                        int index = (*outgoing_reverse).second;
                        Transaction transaction = master_Transactions[index];
                        if(transaction.amount!=1)
                        {
                            cout << transaction.transactionID << ": " <<  name << " sent " << transaction.amount 
                        << " dollars to "  << transaction.recipient << " at " << transaction.exec_date << ".\n";
                        }
                        else
                        {
                        cout << transaction.transactionID << ": " <<  name << " sent 1 dollar to " 
                        << transaction.recipient << " at " << transaction.exec_date << ".\n";
                        }
                        ++outgoing_reverse;
                    }
                }
                else
                {
                    auto outgoing = outgoingHistory.lower_bound(name);
                    cout << "Outgoing " << outgoingCount << ":\n";
                    while(outgoing!=outgoingHistory.end() && (*outgoing).first == name)
                    {
                        int index = (*outgoing).second;
                        Transaction transaction = master_Transactions[index];
                        if(transaction.amount!=1)
                        {
                            cout << transaction.transactionID << ": " <<  name << " sent " << transaction.amount 
                        << " dollars to "  << transaction.recipient << " at " << transaction.exec_date << ".\n";
                        }
                        else
                        {
                        cout << transaction.transactionID << ": " <<  name << " sent 1 dollar to " 
                        << transaction.recipient << " at " << transaction.exec_date << ".\n";
                        }
                        ++outgoing;
                    }
                }
            }
        }
        else if(info[0] == 's')
        {
            string ts_before;
            int num_transactions = 0;
            int bankRev = 0; 
            cin >> ts_before;
            ts_before.erase(remove(ts_before.begin(),ts_before.end(),':'),ts_before.end());
            ts_before.replace(6,6,"000000");
            auto it = transaction_execid.lower_bound(stoul(ts_before));
            uint64_t ts_after = stoul(ts_before) + 1000000;
            cout << "Summary of [" << stoul(ts_before) << ", " << ts_after << "):\n";
            while(it!=transaction_execid.end() && (*it).first < ts_after)
            {
                int index = (*it).second;
                Transaction transaction = master_Transactions[index];
                if(transaction.amount!=1)
                {
                    cout << transaction.transactionID << ": " << transaction.sender << " sent " << transaction.amount 
                    << " dollars to " << transaction.recipient << " at " << transaction.exec_date << ".\n";
                }
                else
                {
                    cout << transaction.transactionID << ": " << transaction.sender << " sent 1 dollar to " << transaction.recipient << " at " << transaction.exec_date << ".\n";
                }
                ++num_transactions;
                auto totalfee = calcTransactionFee(transaction);
                bankRev += (totalfee.first + totalfee.second);
                ++it;
            }
            if(num_transactions!=1)
            {
                cout << "There were a total of " << num_transactions << " transactions, 281Bank has collected " << bankRev << " dollars in fees.\n";
            }
            else
            {
                cout << "There was a total of 1 transaction, 281Bank has collected " << bankRev <<" dollars in fees.\n";
            }
        }
    }    
}
//read in reg file 
void readRegFile()
{
    regFile.open(fileName);
    string ts;
    string id;
    string pin;
    string balance;
    while(regFile.peek()!=EOF)
    {
        getline(regFile,ts,'|');
        ts.erase(remove(ts.begin(),ts.end(),':'),ts.end());
        getline(regFile, id, '|');
        getline(regFile, pin, '|');
        getline(regFile, balance); 
        unordered_set<string> addresses;
        user_Info[id] = User({id,stoul(ts),stoi(pin),stoi(balance),addresses});
    } 
}


int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    getMode(argc,argv);
    readRegFile();
    operations();
    query();
    return 0; 
}