#include <cstdlib>
#include <iostream>
#include <math.h>
#include <time.h>

#define pi 3.142592654

using namespace std;

int GetHeading ( unsigned short x, unsigned short y );
void sleep(unsigned int mseconds);

int main(int argc, char *argv[])
{
    int x, y;
    while(1)
    {
        cout << "Enter X: ";
        cin >> x;
        cout << "Enter Y: ";
        cin >> y;
        cout << GetHeading(x, y) << "\n\n\n";
        
        system("PAUSE");
        system("CLS");
    }
    return EXIT_SUCCESS;
}

int GetHeading ( unsigned short x, unsigned short y )
{
    int fudge;
    int result;
    double angle;
    signed short x_adj, y_adj;
    double dx = (double) x;
    double dy = (double) y;
    if (( dx < 1024) && (dy < 1024))
    {
          angle = atan( -1 * dy / dx );
          angle *= 180 / pi;   
          angle += 360;
    } else
     
    if (( dx >= 1024) && (dy >= 1024))
    {
          dx = -1 * (2048.0 - dx);
          dy = -1 * (2048.0 - dy);
          
          angle = atan(-1*dy/dx);
          angle *= 180 / pi;   
          angle += 180;
    } else
    
    if (( dx >= 1024) && (dy < 1024))
    {
          dx = -1 * (2048.0 - dx);
      
          angle = atan(-1*dy/dx);
          angle *= 180 / pi;   
          angle += 180;
    } else
 
    if (( dx < 1024) && (dy >= 1024))
    {
          dy = -1 * (2048.0 - dy);
          
          angle = atan(-1*dy/dx);
          angle *= 180 / pi;   
    
    } 
    
    cout << "X: " << dx << "\tY: " << dy << "\n" << "Result: " ;
    
    result = angle;
    if (result == 360) result = 0;
    return result;
    
}

void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
