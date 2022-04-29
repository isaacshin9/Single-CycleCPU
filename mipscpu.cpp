#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

int pc = 0;
string in = "";
int regfile[32];

void fetch();


void wb (){
  pc = pc+4;
  fetch();
}

void mem(){
  wb();
}

void exe(){
  mem();
}

void decode(string instr){
  int n = instr.length();
  int opcode = 0;
  int read_reg1 = 0;
  int read_data1 = 0;
  int read_reg2 = 0;
  int read_data2 = 0;
  int jump_address = 0;
  string instr_type = "";
  char instruct[n+1];
  strcpy(instruct, instr.c_str());
  for( int i = 0; i < n; i++){
    cout << instruct[i];
  }
  cout << endl;
  

  for(int y = 0; y < 6; y++){
        opcode = opcode * 2;
        opcode = opcode + (instruct[y] - '0');
    }
    
  if(opcode == 2 || opcode == 3){
      for(int yy = 6; yy < 32; yy++){
          jump_address = jump_address * 2;
          jump_address = jump_address + (instruct[yy] - '0');
          if(opcode == 2){
            instr_type = "j";
          }
          if(opcode == 3){
            instr_type = "jal";
          }
          
    }
  }
  else{
    for(int t = 6; t < 11; t++){
        read_reg1 = read_reg1 * 2;
        read_reg1 = read_reg1 + (instruct[t] - '0');
    }

    for(int p = 11; p < 16; p++){
        read_reg2 = read_reg2 * 2;
        read_reg2 = read_reg2 + (instruct[p] - '0');
    }

  }
    
  



  read_data1 = regfile[read_reg1];

  read_data2 = regfile[read_reg2];

  
  cout << "Opcode: " << opcode << endl;
  cout << "Instruction Type: " << instr_type << endl;
  cout << "ReadReg1: " << read_reg1 << endl;
  cout << "ReadData1: " << read_data1 << endl;
  cout << "ReadReg2: " << read_reg2 << endl;
  cout << "ReadData2: " << read_data2 << endl;
  


  exe();
}


void fetch(){

  int plc = pc / 4 * 34;

  if(plc < in.length()){
    string code = in.substr(plc, 32);
    decode(code);
  }
  

}




int main (){
    string line;
    ifstream myfile;
    myfile.open("sample_part1.txt");

    if(myfile.is_open()){
      while(getline(myfile, line)){
        in = in + line + '\n';
      }
        myfile.close();
    }
    else{
      cout << "Unable to open file";
    } 

    cout << "input: "<< endl << in << endl << endl;

    fetch();
}

