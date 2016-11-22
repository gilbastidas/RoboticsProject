#include <iostream>
#include <iomanip>
#include <queue>
#include <algorithm>
#include <string>
#include <math.h>
#include <ctime>
#include <cstdlib>

using namespace std;

const int n=20; // horizontal size of the map
const int m=10; // vertical size size of the map
static int thismap[n][m];
static int closednodesmap[n][m]; // map of closed (tried-out) nodes
static int opennodesmap[n][m]; // map of open (not-yet-tried) nodes
static int dir_map[n][m]; // map of directions
const int dir=4; // number of possible directions to go at any position

static int dx[dir]={1, 0, -1, 0};
static int dy[dir]={0, 1, 0, -1};

class node
{
    // current position
    int xPos;
    int yPos;
    // total distance already travelled to reach the node
    int level;
    // priority=level+remaining distance estimate
    int priority;  // smaller: higher priority

    public:
        node(int xp, int yp, int d, int p)
            {xPos=xp; yPos=yp; level=d; priority=p;}

        int getxPos() const {return xPos;}
        int getyPos() const {return yPos;}
        int getLevel() const {return level;}
        int getPriority() const {return priority;}

        void updatePriority(const int & xDest, const int & yDest)
        {
             priority=level+estimate(xDest, yDest)*10; //A*
        }

        // give better priority to going strait instead of diagonally
        void nextLevel(const int & i) // i: direction
        {
             level+=(dir==8?(i%2==0?10:14):10);
        }

        // Estimation function for the remaining distance to the goal.
        const int & estimate(const int & xDest, const int & yDest) const
        {
            static int xd, yd, d;
            xd=xDest-xPos;
            yd=yDest-yPos;

            // Euclidian Distance
            d=static_cast<int>(sqrt(xd*xd+yd*yd));

            return(d);
        }
};

// Determine priority (in the priority queue)
bool operator<(const node & a, const node & b)
{
  return a.getPriority() > b.getPriority();
}

// A-star algorithm.
// The route returned is a string of direction digits.
string pathFind( const int & xStart, const int & yStart,
                 const int & xFinish, const int & yFinish )
{
    static priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
    static int pqi; // pq index
    static node* n0;
    static node* m0;
    static int i, j, x, y, xdx, ydy;
    static char c;
    pqi=0;

    // reset the node maps
    for(y=0;y<m;y++)
    {
        for(x=0;x<n;x++)
        {
            closednodesmap[x][y]=0;
            opennodesmap[x][y]=0;
        }
    }

    // create the start node and push into list of open nodes
    n0=new node(xStart, yStart, 0, 0);
    n0->updatePriority(xFinish, yFinish);
    pq[pqi].push(*n0);
    //opennodesmap[x][y]=n0->getPriority(); // mark it on the open nodes map
    //gab
    delete n0;
    opennodesmap[xStart][yStart]=n0->getPriority(); // mark it on the open nodes map

    // A* search
    while(!pq[pqi].empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        n0 = new node( pq[pqi].top().getxPos(), pq[pqi].top().getyPos(), pq[pqi].top().getLevel(), pq[pqi].top().getPriority() );

        x = n0->getxPos(); 
        y = n0->getyPos();

        pq[pqi].pop(); // remove the node from the open list
        opennodesmap[x][y]=0;
        // mark it on the closed nodes map
        closednodesmap[x][y]=1;

        // quit searching when the goal state is reached
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if(x==xFinish && y==yFinish)
        {
            // generate the path from finish to start
            // by following the directions
            string path="";
            while(!(x==xStart && y==yStart))
            {
                j = dir_map[x][y];
                c = '0'+ (j+dir/2)%dir;
                path = c + path;
                x += dx[j];
                y += dy[j];
            }

            // garbage collection
            delete n0;
            // empty the leftover nodes
            while(!pq[pqi].empty()) pq[pqi].pop();
            return path;
        }

        // generate moves (child nodes) in all possible directions
        for(i=0;i<dir;i++)
        {
            xdx=x+dx[i]; ydy=y+dy[i];

            if( !( (xdx < 0) || (xdx > (n-1)) || (ydy < 0) || (ydy > (m-1)) || (thismap[xdx][ydy] == 1 ) || (closednodesmap[xdx][ydy] == 1) ) )
            {
                // generate a child node
                m0=new node( xdx, ydy, n0->getLevel(),
                             n0->getPriority());
                m0->nextLevel(i);
                m0->updatePriority(xFinish, yFinish);

                // if it is not in the open list then add into that
                if(opennodesmap[xdx][ydy]==0)

                {
                    opennodesmap[xdx][ydy]=m0->getPriority();
                    pq[pqi].push(*m0);
                    // mark its parent node direction
                    dir_map[xdx][ydy]=(i+dir/2)%dir;
                }
                else if(opennodesmap[xdx][ydy]>m0->getPriority())
                {
                    // update the priority info
                    opennodesmap[xdx][ydy]=m0->getPriority();
                    // update the parent direction info
                    dir_map[xdx][ydy]=(i+dir/2)%dir;

                    while(!(pq[pqi].top().getxPos()==xdx &&
                           pq[pqi].top().getyPos()==ydy))
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop(); // remove the wanted node

                    // empty the larger size pq to the smaller one
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty())
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0); // add the better node instead
                }
                else delete m0; // garbage collection
            }
        }
        delete n0; // garbage collection
    }
    return ""; // no route found
}

string aStar(int new_map[n][m],int xA,int yA,int xB,int yB)
{
    srand(time(NULL));
    
    for(int y=0;y<m;y++)
    {
        for(int x=0;x<n;x++) 
            {
                thismap[x][y]=new_map[x][y];
            }
    }

    cout<<"Map Size (X,Y): "<<n<<","<<m<<endl;
    cout<<"Start: "<<xA<<","<<yA<<endl;
    cout<<"Finish: "<<xB<<","<<yB<<endl;
    // get the route
    clock_t start = clock();
    std::string route=pathFind(xA, yA, xB, yB);
    if(route=="") cout<<"An empty route generated!"<<endl;
    clock_t end = clock();
    double time_elapsed = double(end - start);
    cout<<"Time to calculate the route (ms): "<<time_elapsed<<endl;

    // follow the route on the map and display it
    if(route.length()>0)
    {
        int j; char c;
        int x=xA;
        int y=yA;
        thismap[x][y]=2;
        for(int i=0;i<route.length();i++)
        {
            c =route.at(i);
            j=atoi(&c);
            x=x+dx[j];
            y=y+dy[j];
            thismap[x][y]=3;
        }
        thismap[x][y]=4;

        // display the map with the route
        for(int y=0;y<m;y++)
        {
            for(int x=0;x<n;x++)
                if(thismap[x][y]==0)
                    cout<<".";
                else if(thismap[x][y]==1)
                    cout<<"O"; //obstacle
                else if(thismap[x][y]==2)
                    cout<<"S"; //start
                else if(thismap[x][y]==3)
                    cout<<"R"; //route
                else if(thismap[x][y]==4)
                    cout<<"F"; //finish
            cout<<endl;
        }
    }
    //getchar(); // wait for a (Enter) keypress
    replace(route.begin(),route.end(),'2','W');
	replace(route.begin(),route.end(),'3','N');
	replace(route.begin(),route.end(),'0','E');
	replace(route.begin(),route.end(),'1','S');

	return(route);
	}
