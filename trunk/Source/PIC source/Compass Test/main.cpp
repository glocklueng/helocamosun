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
    signed short x_adj, y_adj;
    
    if (x & 0xfc00)
    {
          x_adj = -1 * (x & 0x03ff);
    }
    else
    {
          x_adj = x & 0x03ff;
    }
    
    if (y & 0xfc00)
    {
          y_adj = -1 * (y & 0x03ff);
    }
    else
    {
          y_adj = y & 0x03ff;
    }
    
    if (( x_adj >= 0) && (y_adj >= 0))
    {
       fudge = 360;     
    }
    else 
    if (( x_adj >= 0) && (y_adj < 0))
    {
       fudge = 0;    
    }
    else 
    if (( x_adj < 0) && (y_adj < 0))
    {
       fudge = 180;    
    }
    else 
    if (( x_adj < 0) && (y_adj >= 0))
    {
        fudge = 180;
    }
    //fudge = 0;
    cout << "X: " << x_adj << "\tY: " << y_adj << "\n" << "Result: " ;
    
    result = fudge + (int) (( 180 / pi ) * (atan( ((float)y_adj * -1) / (float)x_adj)));
    if (result == 360) result = 0;
    return result;
    
}

void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
