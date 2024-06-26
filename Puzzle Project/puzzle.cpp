// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include "xgetopt.h"

using namespace std;

int num_Colors;
int height;
int width;
int start_Count = 0;
int target_Count = 0;
int start_Row;
int start_Col;
int end_Row;
int end_Col;
bool stack_Mode = false;
bool queue_Mode = false;
bool list_output = false;
bool map_output = false;
bool output_Argument = false;
int stack_Count = 0;
int queue_Count = 0;
char endColor;
vector<vector<char>> twoDmap;
vector<vector<vector<char>>> backTrace;
bool solution;

struct State    
{
    char color;
    int row;
    int col;
};
deque<State> path;
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
        {"queue", no_argument, nullptr, 'q'},
        {"stack", no_argument, nullptr, 's'},
        {"output", required_argument, nullptr, 'o'},
        {nullptr, 0, nullptr, '\0'},
    };
    while((choice = getopt_long(argc,argv,"o:hqs",long_options, &index))!=-1)
    {
        switch(choice)
        {
            case 'h':
                printHelp();
                exit(0);
            
            case 'q':
                    queue_Mode = true;
                    ++queue_Count;
                break;
            case 's':

                    stack_Mode = true;
                    ++stack_Count;
                break;
            case 'o':
            {
                output_Argument = true;
                string arg=optarg;
                if(arg!="map" && arg!="list")
                {
                    cerr << "Invalid output mode";
                    exit(1);
                }
                if(arg=="map" && arg=="list")
                {
                    cerr << "Enter only one output mode";
                    exit(1);
                }
                if(arg=="map")
                {
                    map_output = true;
                }
                else if(arg=="list")
                {
                    list_output = true;
                }
                break;
            }
            default:
                cerr << "Error";
                exit(1);
        }
    }
    if(stack_Count > 1)
    {
        cerr << "Too many stack arguments";
        exit(1);
    }
    if(queue_Count > 1)
    {
        cerr << "Too many queue arguments";
        exit(1);
    }
    if(stack_Count + queue_Count > 1)
    {
        cerr << "Too many stack or queue arguments";
        exit(1);
    }
    if(output_Argument == false)
    {
        map_output = true;
    }
    if(!queue_Mode && !stack_Mode)
    {
        cerr << "Enter stack or queue mode";
        exit(1);
    }
}

//use queue to store search container
//while search container not empty take the 
//next thing out of search container. becomes current state

bool is_valid(char character)
{
    if(!(character == '@' || character == '?' || character == '.' || character == '#' 
    || isalpha(character) || character == '^'))
    {
        cerr << "One of the inputs is not a valid character";
        exit(1);
    }
    if(isalpha(character))
    {
        if(!(character >= 'A' && character < 'A' + num_Colors) && !(character>='a' && character < 'a' + num_Colors))
        {
            cerr << "Invalid door or button";
            exit(1); 
        }
    }
    return true;
}

vector<vector<char>> InitializeMaps()
{
    cin >> num_Colors;
    cin >> height;
    cin >> width;
    if(num_Colors < 0 || num_Colors > 26){
        cerr << "There is an invalid input for the colors ";
        exit(1);
    }
    if(height < 1){
        cerr << "There is an invalid input for the height";
        exit(1);
    }
    if(width < 1){
        cerr << "There is an invalid input for the width";
        exit(1);
    }
    //get rid of newline
    string junk;
    getline(cin,junk);  
    backTrace.resize(num_Colors+1,vector<vector<char>>(height,vector<char>(width,' ')));
    twoDmap.resize(height,vector<char>(width));
    string line;
    int row = 0;
    while(getline(cin,line))
    {
        if(!(line.substr(0,2)=="//"))
        {   
            if(line.empty())
            {
                break;
            }
            int index = 0;
            for(int col = 0; col < width; ++col)
            {
                if(is_valid(line[index]))
                {
                    if(line[index] == '@')
                    {
                        ++start_Count;
                        twoDmap[row][col] = line[index];
                        start_Row = row;
                        start_Col = col;
                    }
                    else if(line[index] == '?')
                    {
                        ++target_Count;
                        twoDmap[row][col] = line[index];
                        end_Row = row;
                        end_Col = col;
                    }
                    else
                    {
                        twoDmap[row][col] = line[index];
                    }
                    ++index;
                }
            }
            ++row;
        }
    }   
    return twoDmap;
}

char whatisit(char color, int row, int col, vector<vector<char>> &map)
{
    //if my color is the same as the button then it is open floor
    if(map[row][col]==color)
    {
        return '.';
    }
    //if my color is the same as a door it is open floor
    if(isupper(map[row][col]) && map[row][col]==toupper(color))
    {
        return '.';
    }
    //if my color is the not the same as a door it is a wall
    if(isupper(map[row][col]) && map[row][col]!=toupper(color))
    {
        return '#';
    }
    if(map[row][col]=='@')
    {
        return '.';
    }
    else
    {
        return map[row][col];
    }
}
//if standing on a button have to do button stuff, if not standing on a button can do north east south west
//North east south west check is it valid to go there, is it something I could walk on, Is it some place I have never discovered yet

bool validMove(char direction, State &temp)
{
    int newRow = temp.row;
    int newCol = temp.col;
    if(direction=='N')
    {
        --newRow;
    }
    else if(direction=='E')
    {
        ++newCol;
    }
    else if(direction=='S')
    {
        ++newRow;
    }
    else if(direction=='W')
    {
        --newCol;
    }
    //check valid coordinates
    if(newRow<0 || newRow>=height || newCol < 0 || newCol>=width)
    {
        return false;
    }
    char currentColor = temp.color;
    char currentContent = whatisit(currentColor,newRow,newCol, twoDmap);
    //check if it is a wall
    if(currentContent == '#')
    {
        return false;
    }
    //if it is a trap I can move to it
    if(currentContent == '^')
    {
        return true;
    }
    //If it is open floor 
    if(currentContent=='.')
    {
        return true;
    }
    if(currentContent=='@')
    {
        return true;
    }
    if(currentContent=='?')
    {
        return true;
    }
    if(currentContent >= 'a' && currentContent <= 'z')
    {
        return true;
    }
    else
    {
        return false;
    }
}

//check if standing on button
bool standingonButton(char color, int row, int col)
{
    //ToDotrap is also button
    if(whatisit(color,row,col, twoDmap)=='.')
    {
        return false;
    }
    if((twoDmap[row][col]>='a' && twoDmap[row][col]<='z') || twoDmap[row][col]=='^')
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint32_t chartoint(char tile)
{
    //Todo if trap door different behavior
    if(tile=='^')
    {
        return 0;
    }
    else
    {
        return static_cast<uint32_t>(tile-'a') + 1;
    }
} 

char inttochar(int color)
{
    if(color==0)
    {
        return '^';
    }
    else
    {
        return static_cast<char>('a' + color - 1);
    }
}

bool discovered(vector<vector<vector<char>>> &backTrace, char color, int row, int col)
{
    if(backTrace[chartoint(color)][row][col]!=' ')
    {
        return true;
    }
    else
    {
        return false;
    }
}

void solve_Maze()
{
    //Create search container to hold discovered states that have not been investigated
    deque<State> search_Container;
    //discover initial state and add to search container and record state as having been discovered 
    State start = {'^',start_Row,start_Col};
    search_Container.push_back(start);
    backTrace[chartoint('^')][start_Row][start_Col] = '@';
    State currentState;
    while(!search_Container.empty())
    {
        if(queue_Mode)
        {
            currentState = search_Container.front();
            search_Container.pop_front();
        }
        else
        {
            currentState = search_Container.back();
            search_Container.pop_back();
        }
        if(standingonButton(currentState.color, currentState.row,currentState.col))
        {
            if(!discovered(backTrace,twoDmap[currentState.row][currentState.col],currentState.row,currentState.col))
            {
                char prevState = currentState.color;
                currentState.color = twoDmap[currentState.row][currentState.col];
                search_Container.push_back(currentState);
                backTrace[chartoint(currentState.color)][currentState.row][currentState.col] = prevState;
                continue;
            }
        }
        if(!standingonButton(currentState.color, currentState.row,currentState.col))
        {
            //discover four states. check if valid move and if previously discovered 
            if(validMove('N',currentState)  && !discovered(backTrace,currentState.color,currentState.row-1,currentState.col))
            {
                //add to search container update backtrace
                search_Container.push_back({currentState.color,currentState.row-1,currentState.col});
                backTrace[chartoint(currentState.color)][currentState.row-1][currentState.col]='S';
            }
            if(validMove('E',currentState) && !discovered(backTrace,currentState.color,currentState.row,currentState.col+1))
            {
                search_Container.push_back({currentState.color,currentState.row,currentState.col+1});
                backTrace[chartoint(currentState.color)][currentState.row][currentState.col+1]='W';
            }
            if(validMove('S',currentState) && !discovered(backTrace,currentState.color,currentState.row+1,currentState.col))
            {
                search_Container.push_back({currentState.color,currentState.row+1,currentState.col});
                backTrace[chartoint(currentState.color)][currentState.row+1][currentState.col]='N';
            }
            if(validMove('W',currentState) && !discovered(backTrace,currentState.color,currentState.row,currentState.col-1))
            {
                search_Container.push_back({currentState.color,currentState.row,currentState.col-1});
                backTrace[chartoint(currentState.color)][currentState.row][currentState.col-1]='E';
            }  
        }
        //check if target has been discovered
        if(backTrace[chartoint(currentState.color)][end_Row][end_Col]!=' ')
        {
            endColor = currentState.color;
            solution = true;
            break;
        }
    }
    if(endColor=='\0')
    {
        solution = false;
    }
}

void findPath(vector<vector<vector<char>>> &backTrace)
{
    State current = {endColor,end_Row,end_Col};
    char currContent = backTrace[chartoint(endColor)][end_Row][end_Col];
    while(currContent!='@')
    {

        path.push_back(current);
        char direction = backTrace[chartoint(current.color)][current.row][current.col];
        if((direction >='a' && direction <='z') || direction=='^')
        {
            current.color = currContent;
            current = {current.color,current.row,current.col};
            currContent = backTrace[chartoint(current.color)][current.row][current.col];
            direction = currContent;
            continue;
        }
        if(direction=='N')
        {
            current = {current.color,current.row-1,current.col};
            currContent = backTrace[chartoint(current.color)][current.row][current.col];
        }
        else if(direction=='E')
        {
            current = {current.color,current.row,current.col+1};
            currContent = backTrace[chartoint(current.color)][current.row][current.col];
        }
        else if(direction=='S')
        {
            current = {current.color,current.row+1,current.col};
            currContent = backTrace[chartoint(current.color)][current.row][current.col];
        }
        else if(direction=='W')
        {
            if(currContent>='a' && currContent<='z')
            {
                current.color = currContent;
            }
            current = {current.color,current.row,current.col-1};
            currContent = backTrace[chartoint(current.color)][current.row][current.col];
        }
    }
    path.push_back({current.color,start_Row,start_Col});
}


void editBackTrace()
{
    for(auto& back:path)
    {
        char currContent = whatisit(back.color,back.row,back.col, twoDmap);
        if(((twoDmap[back.row][back.col]>='a' && twoDmap[back.row][back.col]<='z') || twoDmap[back.row][back.col]=='^') 
        && twoDmap[back.row][back.col]!=back.color)
        {
            backTrace[chartoint(back.color)][back.row][back.col] = '%';
        }
        else if(((twoDmap[back.row][back.col]>='a' && twoDmap[back.row][back.col]<='z') || twoDmap[back.row][back.col]=='^') 
        && (backTrace[chartoint(back.color)][back.row][back.col]=='^' || islower(backTrace[chartoint(back.color)][back.row][back.col])))
        {
            backTrace[chartoint(back.color)][back.row][back.col] = '@';
        }
        else if(currContent == '.')
        {
            backTrace[chartoint(back.color)][back.row][back.col] = '+';
        }
    }
    backTrace[0][start_Row][start_Col] = '@';
}

vector<vector<char>> twoDmapbyColor(char color)
{
    vector<vector<char>> twoDmapCopy = twoDmap;
    if(color=='^')
    {
        for(int i = 0; i < height; ++i)
        {
            for(int j = 0; j < width; ++j)
            {
                if(twoDmap[i][j]=='^')
                {
                    twoDmapCopy[i][j]='.';
                }
            }
        }
    }
    if(color!='^')
    {
        for(int i = 0; i < height; ++i)
        {
            for(int j = 0; j < width; ++j)
            {
                if(whatisit(color,i,j,twoDmap)=='.')
                {
                    twoDmapCopy[i][j]='.';
                }
            }
        }
    }
    for(auto &back:path)
    {
        if(back.color==color)
        {
            twoDmapCopy[back.row][back.col] = backTrace[chartoint(color)][back.row][back.col];
        }
    }
   twoDmapCopy[end_Row][end_Col] = '?';
    return twoDmapCopy;
}
void mapOutput()
{
    editBackTrace();
    for(int i = 0; i < num_Colors + 1; ++i)
    {
        cout << "// " << "color " << inttochar(i) << endl;
        vector<vector<char>> mapCopy = twoDmapbyColor(inttochar(i));
        for(int j = 0; j < height; ++j)
        {
            for(int k = 0; k < width; ++k)
            {
                cout << mapCopy[j][k];
            }
            cout << endl;
        }
    }
}


void listOutput()
{
    while(!path.empty())
    {
        State back = path.back();
        cout << "(" << back.color << ", (" << back.row << ", " << back.col << "))\n";
        path.pop_back();
    }
}

void noSolution()
{
    cout << "No solution.\n" << "Discovered:\n";
    vector<vector<char>> noSolutionmap(height,vector<char>(width,' '));
    for(int i = 0; i < num_Colors + 1; ++i)
    {
        for(int j = 0; j < height; ++j)
        {
            for(int k = 0; k < width; ++k)
            {
                if(discovered(backTrace,inttochar(i),j,k))
                {
                    noSolutionmap[j][k]='*';
                }
                else
                {
                    if(noSolutionmap[j][k]!='*')
                    {
                        noSolutionmap[j][k]='#';
                    }
                }
            }
        }
    }
    for(int i =0; i < height; ++i)
    {
        for(int j=0; j < width; ++j)
        {
            if(noSolutionmap[i][j]=='*')
            {
                cout << twoDmap[i][j];
            }
            else
            {
                cout << noSolutionmap[i][j];
            }
        }
        cout << '\n';
    }
}

int main(int argc, char * argv[])
{
    ios_base::sync_with_stdio(false);
    getMode(argc, argv);
    InitializeMaps();
    if(start_Count > 1)
    {
        cerr << "There are too many starts";
        exit(1);
    }
    if(target_Count > 1)
    {
        cerr << "There are too many ends";
        exit(1);
    }
    if(start_Count==0)
    {
        cerr << "You need one start";
        exit(1);
    }
    if(target_Count==0)
    {
        cerr << "You need one end";
        exit(1);
    }
    solve_Maze();
    if(solution==true)
    {
        findPath(backTrace);
        if(list_output)
        {
            listOutput();
        }
        else
        {
            mapOutput();
        }
    }
    else
    {
        noSolution();
    }
}