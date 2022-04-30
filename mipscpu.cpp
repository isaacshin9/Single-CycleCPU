#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio>
using namespace std;

int pc = 0;
string in = "";
int regfile[32];

int jump = 0;
int regDst = 0;
int ALUSrc = 0;
int Memto_reg = 0;
int regWrite = 0
int mem_read = 0;
int mem_write = 0;
int branch = 0;
int instType1 = 0;
int instType2 = 0;
sting alu_op = "0000";


void alu_control(char funct[]){


  if (funct[0] == '1' && funct[1] == '0' && funct[2] == '0' && funct[3] == '0' && funct[4] == '0' && funct[5] == '0' ){
    alu_op = "0010";
  }
  else if (funct[0] == '1' && funct[1] == '0' && funct[2] == '0' && funct[3] == '0' && funct[4] == '1' && funct[5] == '0' ){
    alu_op = "0110";
  }
  else if (funct[0] == '1' && funct[1] == '0' && funct[2] == '0' && funct[3] == '1' && funct[4] == '0' && funct[5] == '0' ){
    alu_op = "0000";
  }
  else if (funct[0] == '1' && funct[1] == '0' && funct[2] == '0' && funct[3] == '1' && funct[4] == '0' && funct[5] == '1' ){
    alu_op = "0001";
  }
  else if (funct[0] == '1' && funct[1] == '0' && funct[2] == '1' && funct[3] == '0' && funct[4] == '1' && funct[5] == '0' ){
    alu_op = "0111";
  }
  else if (funct[0] == '1' && funct[1] == '0' && funct[2] == '0' && funct[3] == '1' && funct[4] == '1' && funct[5] == '1' ){
    alu_op = "1100";
  }


  
}

void control_unit(char opcode[]){
  // SET ALL CONTROL VALUES TO ZERO
  jump = 0;
  regDst = 0;
  ALUSrc = 0;
  Memto_reg = 0;
  regWrite = 0;
  mem_read = 0;
  mem_write = 0;
  branch = 0;
  alu_op = "0000";
  


  if (opcode[0] == '0' && opcode[1] == '0' && opcode[2] == '0' && opcode[3] == '0' && opcode[4] == '0' && opcode[5] == '0' ){
    regDst = 1;
    regWrite = 1;
    instType2 = 1;
  }
  else if (opcode[0] == '1' && opcode[1] == '0' && opcode[2] == '0' && opcode[3] == '0' && opcode[4] == '1' && opcode[5] == '1' ){
    ALUSrc = 1;
    Memto_reg = 1;
    regWrite = 1;
    mem_read = 1;
    alu_op = "0010";
  }
  else if (opcode[0] == '1' && opcode[1] == '0' && opcode[2] == '1' && opcode[3] == '0' && opcode[4] == '1' && opcode[5] == '1' ){
    ALUSrc = 1;
    mem_write = 1;
    alu_op = "0010";
  }
  else if (opcode[0] == '0' && opcode[1] == '0' && opcode[2] == '0' && opcode[3] == '1' && opcode[4] == '0' && opcode[5] == '0' ){
    branch = 1;
    instType2 = 1;
    alu_op = "0110";
  }
  else if (opcode[0] == '0' && opcode[1] == '0' && opcode[2] == '0' && opcode[3] == '0' && opcode[4] == '1' && opcode[5] == '0' ){
    jump = 1;
  }

 

}

void fetch();


void wb (){
  pc = pc+4;
  fetch();
}

void mem(){
  wb();
}

void exe(string alu_op){
  mem();
}

void decode(string instr){
  int n = instr.length();
  char opcode[6];
  int read_reg1 = 0;
  int read_data1 = 0;
  int read_reg2 = 0;
  int read_data2 = 0;
  int jump_address = 0;
  string instr_type = "";
  char instruct[n+1];
  char funct[6];
  strcpy(instruct, instr.c_str());
  for( int i = 0; i < n; i++){
    cout << instruct[i];
  }
  cout << endl;
  

  for(int y = 0; y < 6; y++){
        opcode[y] = instruct[y];
    }
    
      
  
    
    control_unit(opcode);

    if(instType2 = 1){
      for(int a = 26; a < 32; a++){
        funct[a] = instruct[a];
      }
      alu_control(funct);
    }

    exe(alu_op);

    
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

