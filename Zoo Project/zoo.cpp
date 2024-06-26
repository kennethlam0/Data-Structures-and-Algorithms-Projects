// Project identifier: 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include <iostream>
#include "xgetopt.h"
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
using namespace std;

void printHelp()
{
    cout << "Help message";
}

struct Coordinate
{
    pair<double, double> coord;
    char category; //safe wild border;
    Coordinate(pair<double,double> coord_in, char category_in): coord{coord_in},category{category_in}
    {

    }
};


struct PrimData
{
    double d; //lowest distance to MST
    int32_t p; //index of vertex's parent in MST
    bool k;  // whether or not vertex already in MST 
    PrimData():d{numeric_limits<double>::infinity()},p{0},k{false}
    {    
    }
};
string output_Mode;
int num_vertices;
vector<Coordinate> masterCoordinates;
bool wild;
bool safe;
bool border;
//coordinates read in 
//current path:path building up 
// best path 
//length of best path ever seen
//length of current path.  running total  

double calcDistance(Coordinate &a, Coordinate &b)
{
    if((a.category=='w' && b.category=='s') || (a.category=='s' && b.category=='w'))
    {
        return numeric_limits<double>::infinity();
    }
    return sqrt(((a.coord.first-b.coord.first)*(a.coord.first-b.coord.first)) + ((a.coord.second-b.coord.second)*(a.coord.second-b.coord.second)));
}

double calcDistanceBC(Coordinate &a, Coordinate &b)
{
    return sqrt(((a.coord.first-b.coord.first)*(a.coord.first-b.coord.first)) + ((a.coord.second-b.coord.second)*(a.coord.second-b.coord.second)));
}


vector<PrimData> makeMST()
{
    vector<PrimData> prim_table(num_vertices);
    prim_table[0].d = 0;
    for(int i = 0; i < num_vertices; ++i)
    {
        int index = 0;
        double minDist = numeric_limits<double>::infinity();
        for(int j = 0; j < num_vertices; ++j)
        {
            if(prim_table[j].k==false && prim_table[j].d < minDist)
            {
                index = j;
                minDist = prim_table[j].d;
            }
        }
        prim_table[index].k = true;
        for(int j = 0; j < num_vertices; ++j)
        {
            if(prim_table[j].k==false && calcDistance(masterCoordinates[index],masterCoordinates[j]) < prim_table[j].d)
            {
                prim_table[j].d = calcDistance(masterCoordinates[index],masterCoordinates[j]);
                prim_table[j].p = index;
            }
        }
    }
    return prim_table;
}

int findInsertionIndex(int newLoc,vector<int>& partialTour)
{
    int index = 0;
    double minDist = numeric_limits<double>::infinity();
    double dist;
    for(int i = 0; i < newLoc-1; ++i)
    {
        dist = calcDistanceBC(masterCoordinates[partialTour[i]],masterCoordinates[newLoc]) 
        + calcDistanceBC(masterCoordinates[newLoc],masterCoordinates[partialTour[i+1]]) 
        - calcDistanceBC(masterCoordinates[partialTour[i]],masterCoordinates[partialTour[i+1]]);
        if(dist < minDist)
        {
            minDist = dist;
            index = i+1;
        }
    }
    return index;
}

vector<int> makeFASTTSP()
{
    vector<int> partialTour;
    partialTour.reserve(num_vertices);
    partialTour.push_back(0);
    partialTour.push_back(1);
    int index_newLoc;
    int numVisited = 2;
    while(numVisited!=num_vertices)
    {
        index_newLoc = findInsertionIndex(numVisited,partialTour);
        partialTour.insert(partialTour.begin() + index_newLoc, numVisited);
        ++numVisited;
    }
    return partialTour;
}

void printFASTTSP(vector<int> &Tour)
{
    double distance = 0;
    int loc1;
    int loc2;
    for(int i = 0; i < num_vertices-1; ++i)
    {
        loc1 = Tour[i];
        loc2 = Tour[i+1];
        distance += calcDistanceBC(masterCoordinates[loc1],masterCoordinates[loc2]);
    }
    distance += calcDistanceBC(masterCoordinates[0],masterCoordinates[Tour[num_vertices-1]]);
    cout << setprecision(2) << fixed << distance << '\n';
    for(int i = 0; i < num_vertices; ++i)
    {
        cout << Tour[i] << " ";
    }
}
void printMST(vector<PrimData> &prim_table)
{
    double distance = 0;
    for(int i = 1; i < num_vertices; ++i)
    {
        distance += calcDistance(masterCoordinates[i],masterCoordinates[prim_table[i].p]);
    }
    cout << setprecision(2) << fixed << distance << "\n";
    for(int i = 1; i < num_vertices; ++i)
    {
        if(prim_table[i].p < i)
        {
            cout << prim_table[i].p << " " << i << "\n";
        }
        else
        {
            cout << i << " " << prim_table[i].p << "\n";
        }
    }
}
void readMap()
{
    int x;
    int y;
    cin >> num_vertices;
    string coord;
    masterCoordinates.reserve(num_vertices);
    while(cin >> x >> y)
    {
        //categorization
        // Quadrant 1:safe Quadrant II:safe Quadrant III:Wild Quadrant IV:safe 

        //Quadrant  II 
        // Quadrant III: Wild
        if(x < 0 && y < 0)
        {
            masterCoordinates.emplace_back(make_pair(x,y),'w');
            wild = true;
        }
        //border
        else if((x==0 && y <= 0) || (x <= 0 && y==0))
        {
            masterCoordinates.emplace_back(make_pair(x,y),'b');
            border = true;
        }
        else
        {
            masterCoordinates.emplace_back(make_pair(x,y),'s');
            safe = true;
        }
    }
}

void getMode(int argc, char * argv[])
{
    int choice;
    int index = 0;
    option long_options[] = {
        {"mode", required_argument, nullptr, 'm'}, 
        {"help", no_argument,nullptr,'h'},
        {nullptr, 0, nullptr, '\0'}
    };
    while((choice = getopt_long(argc,argv,"m:h",long_options, &index))!=-1)
    {
        switch(choice)
        {
            case 'h':
                printHelp();
                exit(0);
            case 'm':
                output_Mode = optarg;
                if(output_Mode!="MST" && output_Mode!="FASTTSP" && output_Mode!="OPTTSP")
                {
                    cerr << "Invalid mode";
                    exit(1);
                }
            default:
                break;
        }
    }
}
//coordinates read in 
//current path:path building up 
// best bath 
//length of best path ever seen
//length of current path.  running total  

class OPTTSP
{
    public:
        bool promising(int permLength)
        {
            if((num_vertices - permLength) < 5)
            {
                return true;
            }
            vector<double> d(num_vertices,numeric_limits<double>::infinity());
            vector<bool> k(num_vertices,false);
            size_t currIndex = permLength;
            double mstTotal = 0;
            for(int i = permLength; i < num_vertices;++i)
            {
                int index = -1;
                for(int j = permLength; j < num_vertices; ++j)
                {
                    if(!k[j])
                    {
                        if(index==-1)
                        {
                            index = (int)j;
                        }
                        double dist = adjMatrix[currentPath[currIndex]][currentPath[j]];
                        if(dist < d[j])
                        {
                            d[j]=dist;
                        }
                        if(d[j] < d[index])
                        {
                            index=(int)j;
                        }
                    }
                }
                k[index]=true;
                currIndex = index;
                mstTotal += d[currIndex];
            }            
            double arm1 = numeric_limits<double>::infinity();
            double arm2 = numeric_limits<double>::infinity();
            for(int i = permLength; i < num_vertices; ++i)
            {
                if(adjMatrix[0][currentPath[i]] < arm1)
                {
                    arm1 = adjMatrix[0][currentPath[i]];
                }
                if(adjMatrix[currentPath[permLength-1]][currentPath[i]] < arm2)
                {
                    arm2 = adjMatrix[currentPath[permLength-1]][currentPath[i]];
                }
            }
            double lowerBound = arm1 + arm2 + currTotal + mstTotal;
            if(lowerBound < bestTotal)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        void genPerms(int permLength)
        {
            if(permLength==num_vertices)
            {
                currTotal += adjMatrix[0][currentPath[permLength-1]];
                if(currTotal < bestTotal)
                {
                    bestTotal = currTotal;
                    bestPath = currentPath;
                }
                currTotal -= adjMatrix[0][currentPath[permLength-1]];
                return;
            }
            if(!promising(permLength))
            {
                return;
            }
            for(int i = permLength; i < num_vertices; ++i)
            {
                swap(currentPath[permLength],currentPath[i]);
                currTotal += adjMatrix[currentPath[permLength-1]][currentPath[permLength]];
                genPerms(permLength+1);
                currTotal -= adjMatrix[currentPath[permLength-1]][currentPath[permLength]];
                swap(currentPath[permLength],currentPath[i]);
            }
        }
        void printOPTTSP()
        {
            cout << setprecision(2) << fixed << bestTotal << endl;
            for(int i =0; i < num_vertices;++i)
            {
                cout << bestPath[i] << " ";
            }
        }
        void makeOPTTSP()
        {
            for(int i = 0; i < num_vertices; ++i)
            {
                vector<double> temp;
                temp.reserve(num_vertices);
                adjMatrix.reserve(num_vertices);
                for(int j = 0; j < num_vertices; ++j)
                {
                    temp.push_back(calcDistanceBC(masterCoordinates[i],masterCoordinates[j]));
                }
                adjMatrix.push_back(temp);
            }
            vector<int> fastPath = makeFASTTSP();
            double distance = 0;
            int loc1;
            int loc2;
            for(int i = 0; i < num_vertices-1;++i)
            {
                loc1= fastPath[i];
                loc2=fastPath[i+1];
                distance+=calcDistanceBC(masterCoordinates[loc1],masterCoordinates[loc2]);
            }
            distance+=calcDistanceBC(masterCoordinates[0],masterCoordinates[fastPath[num_vertices-1]]);
            bestPath =fastPath;
            currentPath=fastPath;
            bestTotal = distance;
            genPerms(1);
        }
    private:
        vector<vector<double>> adjMatrix;
        vector<int> currentPath;
        vector<int> bestPath;
        double bestTotal = 0;
        double currTotal = 0;
};

int main(int argc, char * argv[])
{
    std::ios_base::sync_with_stdio(false);
    getMode(argc,argv);
    readMap();
    if(output_Mode=="MST")
    {
        if(wild && safe && !border)
        {
            cerr << "Cannot construct MST";
            exit(1);
        }
        auto mst = makeMST();
        printMST(mst);
    }
    else if(output_Mode=="FASTTSP")
    {
        auto FASTTSP = makeFASTTSP();
        printFASTTSP(FASTTSP);
    }
    else if(output_Mode=="OPTTSP")
    {
        OPTTSP OPT; 
        OPT.makeOPTTSP();
        OPT.printOPTTSP();
    }
    return 0;
}

