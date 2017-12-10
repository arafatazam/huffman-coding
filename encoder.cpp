#include <iostream>
#include <cstring>
#include<cstdio>

using namespace std;

#define MAX_FILE_SIZE 50000
#define MAX_NODES 600
#define NUMBER_OF_CHARS 256
#define MAX_CODE_LENGTH 260

char input[MAX_FILE_SIZE];
char code[NUMBER_OF_CHARS][MAX_CODE_LENGTH]; //holds the code of a node

int map[NUMBER_OF_CHARS]; // map char to its node id. see char2Node()
char mapr[NUMBER_OF_CHARS]; // map node id to char. see node2Char()

int numNodes;
int nodeCount[MAX_NODES+1]; //holds the count of a node
char nodeRep[MAX_NODES+1][MAX_CODE_LENGTH];

/*
################################################
########Heap Data Structure Starts##############
################################################
*/

int heapSize;
int heap[MAX_NODES+1];

int cmp(int index1, int index2){
  //cout<<"comparing "<<nodeRep[heap[index1]]<<" and "<<nodeRep[heap[index2]]<<endl;
  int node1 = heap[index1];
  int node2 = heap[index2];
  return nodeCount[node1]<nodeCount[node2];
}

bool heapEmpty(){
  return heapSize==0;
}

void swap(int idx1, int idx2){
  //cout<<"swaping "<<nodeRep[heap[idx1]]<<" and "<<nodeRep[heap[idx2]]<<endl;
  int tmp = heap[idx1];
  heap[idx1] = heap[idx2];
  heap[idx2] = tmp;
}

int enqueue(int node){
  //cout<<"enqueuing "<<nodeRep[node]<<endl;
  int index = ++heapSize;
  heap[index]=node;
  while(index>1){
    int parnt = index>>1;
    if(cmp(parnt,index)){
      break;
    }
    //cout<<nodeRep[heap[index]]<<" is min than "<< nodeRep[heap[parnt]]<<endl;
    swap(parnt,index);
    index = parnt;
  }
  //cout<<"enqueued size="<<heapSize<<endl;
}

int dequeue(){
  if(heapEmpty()){
    cout<<"Q empty"<<endl;
    return -1;
  }
  int val = heap[1];
  //cout<<"dequeueing: "<<nodeRep[val]<<endl;
  int tmp = heapSize--;
  //cout<<"Searching place for "<<nodeRep[heap[tmp]]<<endl;
  int hole = 1;
  for(int child;((hole<<1)<heapSize);hole=child){
    //cout<<"There is a hole in = "<<hole<<endl;
    child = hole<<1;
    if(cmp(child+1,child)){
      child++;
    }
    if(cmp(child,tmp)){
      heap[hole] = heap[child];
      //cout<<nodeRep[heap[child]]<<" moved to "<<hole<<endl;
    }else{
      break;
    }
  }
  heap[hole]=heap[tmp];
  //cout<<hole<<" is the new seat for "<<nodeRep[heap[tmp]]<<endl;
  //cout<<"dequeued size="<<heapSize<<endl;
  return val;
}

/*
################################################
########Heap Data Structure Ends################
################################################
*/

int char2Node(char ch){
  return map[ch];
}

char node2Char(int node){
  return mapr[node];
}

int countChar(char ch){
  if(!map[ch]){
    map[ch]=++numNodes;
    mapr[map[ch]]=ch;
    if(ch=='\n'){
      strcpy(nodeRep[map[ch]],"\\n");
    }else if(ch=='\t'){
      strcpy(nodeRep[map[ch]],"\\t");
    }else{
      nodeRep[map[ch]][0] = ch;
      nodeRep[map[ch]][1] = '\0';
    }
    //cout<<"newnode "<<map[ch]<<" created with rep="<<nodeRep[map[ch]]<<endl;
  }
  nodeCount[map[ch]]++;
}

void printFreq(){
  int count;
  for(int i=0;i<NUMBER_OF_CHARS;i++){
    count = nodeCount[char2Node(i)];
    if(count){
      printf("%c = %d\n",i,nodeCount[char2Node(i)]);
    }
  }
}

void printCodes(){
  printf("------------------------------------\n");
  
  printf("Below are the generated codes:\n");
  
  printf("------------------------------------\n");
  int count;
  for(int i=0;i<NUMBER_OF_CHARS;i++){
    count = nodeCount[char2Node(i)];
    if(count){
      printf("%s(%d) = %s\n",nodeRep[char2Node(i)],nodeCount[char2Node(i)],code[char2Node(i)]);
    }
  }
  printf("------------------------------------\n");
}



bool takeInput(char* fileName){
  FILE* file;
  file = fopen(fileName, "r");
  if(!file){
    printf("ERROR: Cannot open file:\"%s\"\n",fileName);
    return false;
  }
  char x;
  int numChars = 0;
  while(fscanf(file,"%c",&x)!=EOF){
    //x=tolower(x); //Uncomment this line for making case insensitive
    input[numChars++]=x;
    countChar(x);
    if(numChars>MAX_FILE_SIZE){
      printf("WARNING: MAX_FILE_SIZE exceeded. Overflow characters will not be encoded.\n");
      break;
    }
  }
  fclose(file);
  return true;
}

int parent[MAX_NODES];
int leftChild[MAX_NODES];
int rightChild[MAX_NODES];

int mergeNode(int node1, int node2){
  int newNode = ++numNodes;
  strcpy(nodeRep[newNode],nodeRep[node1]);
  strcat(nodeRep[newNode],nodeRep[node2]);
  //cout<<"newnode "<<newNode<<" created with rep="<<nodeRep[newNode]<<endl;
  nodeCount[newNode]= nodeCount[node1]+nodeCount[node2];
  leftChild[newNode]=node1;
  //cout<<"l = "<<leftChild[newNode]<<endl;
  rightChild[newNode]=node2;
  //cout<<"r = "<<rightChild[newNode]<<endl;
  parent[node1] = newNode;
  parent[node2] = newNode;
  return newNode;
}

void travarse(int node, bool addZero){
  if(node==0){
    return;
  }
  strcpy(code[node],code[parent[node]]);
  strcat(code[node],(addZero)?"0":"1");
  //cout<<"Travarsing "<<nodeRep[node]<<" code is= "<<code[node]<<endl;
  travarse(leftChild[node],true);
  travarse(rightChild[node],false);
}

bool generateCode(){
  for(int i=1;i<=numNodes;i++){
    enqueue(i);
  }
  while(heapSize>=2){
    int n1 = dequeue();
    int n2 = dequeue();
    enqueue(mergeNode(n1,n2));
  }
  int root = dequeue();
  //cout<<"root= "<<root<<endl;
  strcpy(code[root],"");
  travarse(leftChild[root],true);
  travarse(rightChild[root],false);
  printCodes();
  return true;
}

bool writeDictionary(char* fileName){
  FILE* file;
  file = fopen(fileName, "w+");
  if(!file){
    printf("ERROR: Cannot open file:\"%s\"\n",fileName);
    return false;
  }
 for(int i=0;i<256;i++){
    if(nodeCount[char2Node(i)]){
      fprintf(file,"%c %s ",i,code[char2Node(i)]);
    }
  }
  fclose(file);
  return true;
}


bool writeEncodedFile(char* fileName){
  FILE* file;
  file = fopen(fileName, "wb");
  if(!file){
    printf("ERROR: Cannot open file:\"%s\"\n",fileName);
    return false;
  }
  
  char byteStream[2+MAX_FILE_SIZE*MAX_CODE_LENGTH/8];
  int bitsWritten=0;
  
  for(int i=0, bitIndex=0;i<strlen(input);i++){
    char* charCode = code[char2Node(input[i])];
    int len = strlen(charCode);
    for(int j=0;j<len;j++,bitIndex++){
      int byteIndex = bitIndex/8;
      int bitPos = bitIndex%8;
      int mask = 128>>bitPos;
      if(charCode[j]=='0'){
        byteStream[byteIndex] &= ~mask;
      }else{
        byteStream[byteIndex] |= mask;
      }
    }
    bitsWritten = bitIndex;
  }
  
  
  
  int count = (bitsWritten/8) + !!(bitsWritten%8);
  byteStream[count]='\0';
  fwrite(&bitsWritten,sizeof(bitsWritten),1,file);
  fwrite(&byteStream,1,count,file);
  fclose(file);
  return true;
}


int encode(){
  char fileName[128];
  printf("Please enter a file name for encoding(input.txt):\n");
  scanf(" %[^\n]",fileName);
  if(!takeInput(fileName)){
    return 0;
  }
  //printFreq();
  if(!generateCode()){
    return 0;
  }
  printf("Please enter a file name for storing the dictionary(dict.txt):\n");
  scanf(" %[^\n]",fileName);
  if(!writeDictionary(fileName)){
    return 0;
  }
  printf("Please enter a file name for the encoded file(code.bin):\n");
  scanf(" %[^\n]",fileName);
  if(!writeEncodedFile(fileName)){
    return 0;
  }
  return 0;
}