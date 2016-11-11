//// IEA 1996
//// Example program - runs distance function
//
//#include <iostream.h>
//#include <iomanip.h>
//#include <math.h>
//
//// Function prototypes
//float distance(float,float);
//
//void main()
//{
//  float x,y,dist;
//
//  // Test function distance(x,y)
//  std::cout << "Testing function distance(x,y)"
//       << std::endl;
//  std::cout << "Enter values for x and y: ";
//  std::cin >> x >> y;
//  dist = distance(x,y);
//  std::cout << "Distance of (" << x << ',' << y
//       << ") from origin is " << dist
//       << std::endl
//       << "Tested"
//       << std::endl;
//
//} // End of main
//
//// Function Definitions
//
//float distance(float x, float y)
//      // Returns the distance of (x, y) from origin
//  {
//    float dist;  //local variable
//    dist = sqrt(x * x + y * y);
//    return dist;
//  }
