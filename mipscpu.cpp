#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <math.h>

using namespace std;
//testing git poosh

int pc = 0;
string in = "";
int registerfile[31];
int d_mem[31];
int jump = 0;
int regDst = 0;
int ALUSrc = 0;
int Memto_reg = 0;
int regWrite = 0;
int mem_read = 0;
int mem_Write = 0;
int branch = 0;
int inst_type = 0;
int instType1 = 0;
int instType2 = 0;
int jumpReg;

int next_pc = 0;
int jump_target = 0;
int branch_target = 0;

int total_clock_cycles = 0;

string alu_op;


void alu_control(string funct){
  //SW, LW, ADD
  if (funct == "100000" || funct == "101011" || funct == "100011"){
    alu_op = "0010";
  }
  //SUB, BEQ
  else if (funct == "100010" || funct == "000100"){
    alu_op = "0110";
  }
  //AND
  else if (funct == "100100"){
    alu_op = "0000";
  }
  //OR
  else if (funct == "100101"){
    alu_op = "0001";
  }
  //SLT
  else if (funct == "101010"){
    alu_op = "0111";
  }
  //NOR
  else if (funct == "100111"){
    alu_op = "1100";
  }
  return funct
   
  
}

void control_unit(string op){
//All R type
  if(op = "000000"){
    regDst = 1;
    jump = 0;
    branch = 0;
    mem_read = 0;
    Memto_reg = 0;
    alu_op = 1;
    mem_Write = 0;
    ALUSrc = 0;
    regWrite = 1;
    inst_type = 1;

  }
  //LW
  if(op = "100011"){
    regDst = 0;
    jump = 0;
    branch = 0;
    mem_read = 1;
    Memto_reg = 1;
    alu_op = 0;
    mem_Write = 0;
    ALUSrc = 1;
    regWrite = 1;
    inst_type = 0;

  }
  //SW
  if(op = "101011"){
    regDst = 0;
    jump = 0;
    branch = 0;
    mem_read = 1;
    Memto_reg = 1;
    alu_op = 0;
    mem_Write = 0;
    ALUSrc = 1;
    regWrite = 1;
    inst_type = 0;

  }
  //BEQ
  if(op = "00100"){
    regDst = 0;
    jump = 0;
    branch = 1;
    mem_read = 0;
    Memto_reg = 0;
    alu_op = 1;
    mem_Write = 0;
    ALUSrc = 0;
    regWrite = 0;
    inst_type = 0;
  }
  //Jal
  if(op = "000011"){
    regDst = 1;
    jump = 1;
    branch = 0;
    mem_read = 0;
    Memto_reg = 1;
    alu_op = 0;
    mem_Write = 0;
    ALUSrc = 0;
    regWrite = 1;
    inst_type = 0;
  }
  //Jr
  if(op = "001000"){
    regDst = 0;
    jump = 1;
    branch = 0;
    mem_read = 0;
    Memto_reg = 0;
    alu_op = 0;
    mem_Write = 0;
    ALUSrc = 0;
    regWrite = 0;
    inst_type = 0;
    int jumpReg;
  }

}


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

int binary_to_decimal(string binary){
  int value = 0;
  int index = 0;
  for(int i = binary.length() - 1;i >= 0;i--){
    if(binary[i] == '1'){
      value += pow(2, index);
      }
      index++;
}
  return(value);
}

void decode(string code){
  string opcode = code.substr(0,6);
  int rs;
  int rt;
  int rd;
  int shamt;
  int funct;
  string immediate;
  string address;

  int n = code.length();

  int read_reg1 = 0;
  int read_data1 = 0;

  int read_reg2 = 0;
  int read_data2 = 0;

  int jump_address = 0;
  int branch_address = 0;
  strcpy(instruct, instr.c_str());

  control_unit(opcode);
  //execute jump data path
  if(jump == 1){
    immediate = str.substring(6, 32);

  }

  //execute beq data path
  else if(branch == 1){
      

  }
  else{

  }
  //executes LW
  else(){

  }
  //executes SW
  else(){

  }

  //execute R type
  else if(ALUSrc == 0){

  }

  read_data1 = registerfile[read_reg1];
  read_data2 = registerfile[read_reg2];

  exe(read_data1, read_data2);
      
    }
  }
    




void fetch(vector<string> inst){
  while(pc / 4 < inst.size()){
    total_clock_cycles = total_clock_cycles + 1;
    cout << "Total clock cycles: " << total_clock_cycles;
    decode(inst[pc / 4]);
    new_pc = pc + 4;

  }

}




int main() {
    //creating a dynamic array which will hold 32 bit instruction
    vector<string> instruction;
    string line;
    ifstream myfile;
    myfile.open("sample_part1.txt");

    if(myfile.is_open()){
      while(getline(myfile, line)){
        instruction.push_back(line);
      }
        myfile.close();
    }
    else{
      cout << "Unable to open file";
    } 
    cout << "input: "<< endl << in << endl << endl;

    fetch(instruction);

}

