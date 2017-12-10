#include <iostream>
#include <cstring>
#include<cstdio>

using namespace std;

#define MAX_FILE_SIZE 50000
#define NUMBER_OF_CHARS 256
#define MAX_CODE_LENGTH 260

int totalBits;
char byteStream[2+MAX_FILE_SIZE*MAX_CODE_LENGTH/8];

int numCodes;
char dicCodes[NUMBER_OF_CHARS][NUMBER_OF_CHARS];
char dicChars[NUMBER_OF_CHARS];

char message[MAX_FILE_SIZE];

char code2Ch(char *code){
  for(int i=0;i<numCodes;i++){
    if(strcmp(code,dicCodes[i])==0){
      return dicChars[i];
    }
  }
  return 0;
}

bool readEncodedFile(char *fileName){
  FILE* file;
  file = fopen(fileName, "rb");
  if(!file){
    printf("ERROR: Cannot open file:\"%s\"\n",fileName);
    return false;
  }
  fread(&totalBits,sizeof(totalBits),1,file);
  fread(byteStream,1,totalBits,file);
  byteStream[totalBits]='\0';
  fclose(file);
  return true;
}

bool readDictionaryFile(char *fileName){
  FILE* file;
  file = fopen(fileName, "r");
  if(!file){
    printf("ERROR: Cannot open file:\"%s\"\n",fileName);
    return false;
  }
  char ch;
  char code[MAX_CODE_LENGTH];
  numCodes = 0;
  while(fscanf(file,"%c %s ",&ch,code)!=EOF){
    //cout<<ch<<" : "<<code<<endl;
    strcpy(dicCodes[numCodes],code);
    dicChars[numCodes]=ch;
    numCodes++;
  }

  fclose(file);
  return true;
}

bool decodeMessage(){
  char code[MAX_CODE_LENGTH];
  strcpy(code,"");
  int messageLen = 0;
  for(int bitIndex=0;bitIndex<totalBits;bitIndex++){
    int byteIndex = bitIndex/8;
    int bitPos = bitIndex%8;
    int mask = 128>>bitPos;
    if(byteStream[byteIndex]&mask){
      strcat(code,"1");
    }else{
      strcat(code,"0");
    }
    char ch = code2Ch(code);
    if(ch){
      message[messageLen++]=ch;
      strcpy(code,"");
    }
  }
  message[messageLen]='\0';
  return true;
}

bool writeDecodedFile(char *fileName){
  FILE* file;
  file = fopen(fileName, "w");
  if(!file){
    printf("ERROR: Cannot open file:\"%s\"\n",fileName);
    return false;
  }
  fwrite(message,1,strlen(message),file);
  fclose(file);
  return true;
}


int decode(){
  char fileName[128];
  printf("Please enter a file name for decoding(code.bin):\n");
  scanf(" %[^\n]",fileName);
  if(!readEncodedFile(fileName)){
    return 0;
  }

  printf("Please enter a file name for dictionary(dict.txt):\n");
  scanf(" %[^\n]",fileName);
  if(!readDictionaryFile(fileName)){
    return 0;
  }

  if(!decodeMessage()){
    return 0;
  }

  printf("Please enter a file name for decoded file(output.txt):\n");
  scanf(" %[^\n]",fileName);
  if(!writeDecodedFile(fileName)){
    return 0;
  }

  return 0;
}
