//#include <iostream>
//#include <math.h>
//#include <ctime>
//#include "aStar.h"
//
//#define N 20
//#define M 10
//
//using namespace std;
//
//static int my_map[N][M];
//
//int main (){
//
//	srand(time(NULL));
//
//	// create empty map
//    for(int y=0;y<M;y++)
//    {
//        for(int x=0;x<N;x++) my_map[x][y]=0;
//    }
//
//    // fillout the map matrix with a '+' pattern
//    for(int x=N/8;x<N*7/8;x++)
//    {
//        my_map[x][M/2]=1;
//    }
//    for(int y=M/8;y<M*7/8;y++)
//    {
//        my_map[N/2][y]=1;
//    }
//    int static_map[M][N]={
//    {0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0},
//    {0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0},
//    {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
//    {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
//    {0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0},
//    {0,0,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0},
//    {0,0,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0},
//    {0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0},
//    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
//    };
//    for(int y=0;y<M;y++)
//	{
//		for(int x=0;x<N;x++) my_map[x][y]=static_map[y][x];
//	}
//	// randomly select start and finish locations
//    int xA, yA, xB, yB;
//    switch(rand()%8)
//    {
//        case 0: xA=0;yA=0;xB=n-1;yB=m-1; break;
//        case 1: xA=0;yA=m-1;xB=n-1;yB=0; break;
//        case 2: xA=n/2-1;yA=m/2-1;xB=n/2+1;yB=m/2+1; break;
//        case 3: xA=n/2-1;yA=m/2+1;xB=n/2+1;yB=m/2-1; break;
//        case 4: xA=n/2-1;yA=0;xB=n/2+1;yB=m-1; break;
//        case 5: xA=n/2+1;yA=m-1;xB=n/2-1;yB=0; break;
//        case 6: xA=0;yA=m/2-1;xB=n-1;yB=m/2+1; break;
//        case 7: xA=n-1;yA=m/2+1;xB=0;yB=m/2-1; break;
//    }
//
//	aStar(my_map, xA, yA, xB, yB);
//
//
//
//	return 0;
//}
