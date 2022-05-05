#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <math.h>

using namespace std;

int pc = 0;
string in = "";
int regfile[32];
int d_mem[32];
int jump = 0;
int regDst = 0;
int ALUSrc = 0;
int Memto_reg = 0;
int regWrite = 0;
int mem_read = 0;
int mem_Write = 0;
int branch = 0;
int instType1 = 0;
int instType2 = 0;

int next_pc = 0;
int jump_target = 0;
int branch_target = 0;

int total_clock_cycles = 0;

string alu_op = "0000";


void alu_control(string funct){

  if (funct == "100000" || funct == "101011" || funct == "100011"){
    alu_op = "0010";
  }
  else if (funct == "100010" || funct == "000100"){
    alu_op = "0110";
  }
  else if (funct == "100100"){
    alu_op = "0000";
  }
  else if (funct == "100101"){
    alu_op = "0001";
  }
  else if (funct == "101010"){
    alu_op = "0111";
  }
  else if (funct == "100111"){
    alu_op = "1100";
  }

}

void control_unit(string opcode){
  // SET ALL CONTROL VALUES TO ZERO
  jump = 0;
  regDst = 0;
  ALUSrc = 0;
  Memto_reg = 0;
  regWrite = 0;
  mem_read = 0;
  mem_Write = 0;
  branch = 0;
  instType1 = 0;
  instType2 = 0;

  alu_op = "0000";
  
  if (opcode == "000000"){
    regDst = 1;
    regWrite = 1;
    instType2 = 1;
  }
  else if (opcode == "100011"){
    ALUSrc = 1;
    Memto_reg = 1;
    regWrite = 1;
    mem_read = 1;
    alu_op = "0010";
  }
  else if (opcode == "101011"){
    ALUSrc = 1;
    mem_Write = 1;
    alu_op = "0010";
  }
  else if (opcode == "000100"){
    branch = 1;
    instType2 = 1;
    alu_op = "0110";
  }
  else if (opcode == "000011" || opcode == "000001"){
    jump = 1;
  }

}

void fetch();


void wb (){
  fetch();
}

void mem(){
  wb();
}

void exe(int read_data1, int read_data2){
    int result;
    if(alu_op == "1000"){//add
        result = read_data1 + read_data2;
    }
    else if(alu_op == "0110"){ //sub 
        result = read_data1 - read_data2;
    }
    else if(alu_op == "0000"){ //and
        result = read_data1 & read_data2;
    }
    else if(alu_op == "0001"){//or
        result = read_data1 | read_data2;
    }
    else if(alu_op == "1100"){//nor
        result = ~(read_data1 | read_data2);
    }

    mem();
  cout << result << endl;
}

void decode(string instr){

  int n = instr.length();

  int read_reg1 = 0;
  int read_data1 = 0;

  int read_reg2 = 0;
  int read_data2 = 0;

  int jump_address = 0;
  int branch_address = 0;

  string immediate;
  string funct;
  string opcode;

  int rs = 0;
  int rt = 0;
  int rd = 0;
  opcode = instr.substr(0,6);

    
    control_unit(opcode);
  

    if(jump = 1){ //Jump Instructions


    }
    else if(branch = 1){ //Branch Instructions

    }
    else{
      if(instType2 = 1){  //r-type

        funct = instr.substr(25,6);
        cout << "r-type " << funct << endl;
        alu_control(funct);

      }
      else{  //i-type

        cout<< " i-type" << endl;

      }

      //for(int t = 6; t < 11; t++){
      //    read_reg1 = read_reg1 * 2;
      //    read_reg1 = read_reg1 + (instruct[t] - '0');
      //}

      //for(int p = 11; p < 16; p++){
      //    read_reg2 = read_reg2 * 2;
      //    read_reg2 = read_reg2 + (instruct[p] - '0');
      //}

      //read_data1 = regfile[read_reg1];
      //read_data2 = regfile[read_reg2];

      exe(read_data1, read_data2);
    }
  }
    




void fetch(){

  int plc = pc / 4 * 34;
  if(plc < in.length()){
    string code = in.substr(plc, 32);
    decode(code);
  }
  pc = pc + 4;

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

