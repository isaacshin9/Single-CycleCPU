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

void regVal(int rg)
{
    if(rg == 0){cout << "zero";}
    if(rg == 1){cout << "at";}
    if(rg == 2){cout << "v0";}
    if(rg == 3){cout << "v1";}
    if(rg == 4){cout << "a0";}
    if(rg == 5){cout << "a1";}
    if(rg == 6){cout << "a2";}
    if(rg == 7){cout << "a3";}
    if(rg == 8){cout << "t0";}
    if(rg == 9){cout << "t1";}
    if(rg == 10){cout << "t2";}
    if(rg == 11){cout << "t3";}
    if(rg == 12){cout << "t4";}
    if(rg == 13){cout << "t5";}
    if(rg == 14){cout << "t6";}
    if(rg == 15){cout << "t7";}
    if(rg == 16){cout << "s0";}
    if(rg == 17){cout << "s1";}
    if(rg == 18){cout << "s2";}
    if(rg == 19){cout << "s3";}
    if(rg == 20){cout << "s4";}
    if(rg == 21){cout << "s5";}
    if(rg == 22){cout << "s6";}
    if(rg == 23){cout << "s7";}
    if(rg == 24){cout << "t8";}
    if(rg == 25){cout << "t9";}
    if(rg == 26){cout << "k0";}
    if(rg == 27){cout << "k1";}
    if(rg == 28){cout << "gp";}
    if(rg == 29){cout << "sp";}
    if(rg == 30){cout << "fp";}
    if(rg == 31){cout << "ra";}
}


void wb (int writereg, int result){
  if (regWrite == 0 && mem_Write == 1)
    { // SW
        d_mem[writereg] = result;
        cout << "memory 0x" << hex << writereg << " is modified to 0x" << hex << result << endl;
    }
    else if (regWrite == 1)
    { // R AND LW
        regfile[writereg] = result;
        cout << "$";
        regVal(writereg);
        cout << " is modified to 0x" << hex << result << endl;
    }
    total_clock_cycles = total_clock_cycles + 1;
}

void mem(int writereg, int result){
  // Each entry of d_mem array will be accessed w/ following addresses
	// refer to project description
    int val = 0;
    if (mem_Write == 1){ //SW
        val = regfile[writereg];
        wb(result, val);
    }else if (mem_read == 1){ //lw
        val = d_mem[result / 4]; // divide by 4 to place it in the memory by 4
        wb(writereg, val);
    }else{
        // R/I
        wb(writereg,result);
    }
}

void exe(int read_data1, int read_data2, int writereg){
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

    mem(writereg, result);
    cout << result << endl;
}

void decode(string instr){

  int n = instr.length();
  int read_reg1 = 0;
  int read_data1 = 0;
  int writereg = 0;
  int read_reg2 = 0;
  int read_data2 = 0;
  int jump_address = 0;
  int branch_address = 0;

  string funct;
  string opcode;

  int rs = 0;
  int rt = 0;
  int rd = 0;
  int immediate = 0;

  opcode = instr.substr(0,6);

  control_unit(opcode);

  
  if(instType2 == 1){  //r-type
    funct = instr.substr(25,6);
    cout << "r-type " << funct << endl;
    alu_control(funct);
  }

  char instruct[n+1];
  strcpy(instruct, instr.c_str());
  

  for(int t = 6; t < 11; t++){
      read_reg1 = read_reg1 * 2;
      read_reg1 = read_reg1 + (instruct[t] - '0');
  }

  for(int p = 11; p < 16; p++){
      read_reg2 = read_reg2 * 2;
      read_reg2 = read_reg2 + (instruct[p] - '0');
  }

  for(int p = 16; p < 21; p++){
      rd = rd * 2;
      rd = rd + (instruct[p] - '0');
  }

  for(int p = 6; p < 32; p++){
          jump_target = jump_target * 2;
          jump_target = jump_target + (instruct[p] - '0');
  }
  for(int p = 16; p < 32; p++){
          immediate = immediate * 2;
          immediate = immediate + (instruct[p] - '0');
  }

  read_data1 = regfile[read_reg1];
  read_data2 = regfile[read_reg2];
  
  writereg = read_reg2;
  if (instType2 == 1) {
    writereg = rd;
  }
  
  if (instType2 == 0) {
    read_data2 = immediate;
  }

  exe(read_data1, read_data2, writereg);
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

