#include <iostream>  
#include <bitset>  
using namespace std;  

int main() {  
  short short2 = 16384;      
  bitset<16> bitset1{short2};  
  cout << bitset1 << endl;  // 0100000000000000   

  short short3 = short2 << 1;  
  bitset<16> bitset3{short3};  // 16384 left-shifted by 1 = -32768  
  cout << bitset3 << endl;  // 100000000000000  

  short short4 = short2 << 14;  
  bitset<16> bitset4{short4};  // 4 left-shifted by 14 = 0  
  cout << bitset4 << endl;  // 000000000000000    
}