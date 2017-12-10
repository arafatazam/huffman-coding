#include <iostream>
#include <cstdio>
#include "encoder.h"
#include "decoder.h"


using namespace std;

int main(){
  char ch;
  while(true){
    cout<<"Encode Decode, or Exit: (e|d|E)"<<endl;
    cin>>ch;
    switch(ch){
      case 'e':
        encode();
        break;
      case 'd':
        decode();
        break;
      case 'E':
        cout<<"Bye!!"<<endl;
        return 0;
        break;
      default:
        break;
    }
  }
  return 0;
}
