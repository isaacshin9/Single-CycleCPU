#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;
int pc = 0;
int alu_zero = 0;
int total_clock_cycles = 0;
int branch_target = 0;
int jump_target = 0;
int branch =0;
int jump = 0;
int regfile[32] = {0}; // initialize registerfile entries with 0
int d_mem [32] = {0}; // initialize d_mem entries with 0
int regDST = 0;
int regWrite = 0;
int aluSRC = 0;
int memRead = 0;
int memWrite = 0;
int memToReg = 0;
int jumpREG = 0;

string alu(string code){
    // Create seperate function for ALU control called by ControlUnit
    // To generate ALU OP ocde used by execute()
    // or put in ControlUnit()
	string funct;
    if (code == "100100"){ //AND
        funct = "0000";
    }
    if (code == "100101"){ //OR
        funct = "0001";
    }
    if (code == "100000" || code == "101011" || code == "100011"){ // SW and LW ADD                                                              
        funct = "0010";
    }
    if (code == "100010" || code == "000100"){ //SUB and BEQ
        funct = "0110";
    }
    if (code == "101010"){ //SLT
        funct = "0111";
    }
    if (code == "100111"){ //NOR
        funct = "1100";
    }
    return funct;
}

string negCheck(string immediate){
    for (int i = 0; i < immediate.size(); i++){
        if (immediate[i] == '1'){
            immediate[i] = '0';
        }
        else{
            immediate[i] = '1';
        }
    }
    for (int i = immediate.size() - 1; i > 0; i--){
        if (immediate[i] == '1'){
            immediate[i] = '0';
        }
        else{
            immediate[i] = '1';
            break;
        }
    }
    return immediate;
}

int immCheck(string immediate){
	if (immediate[0] == '1'){
        immediate = negCheck(immediate);
    }
    return stoi(immediate, nullptr, 2);
}

string rv(int rg){
    string val = "";
    if(rg == 0){
         val = "zero";
        }
    if(rg == 1){
        val = "at";
        }
    if(rg == 2){
        val = "v0";
        }
    if(rg == 3){
        val = "v1";
        }
    if(rg == 4){
        val = "a0";
        }
    if(rg == 5){
        val = "a1";
        }
    if(rg == 6){
        val = "a2";
        }
    if(rg == 7){
        val = "a3";
        }
    if(rg == 8){
        val = "t0";
    }
    if(rg == 9){
        val = "t1";
        }
    if(rg == 10){
        val = "t2";
        }
    if(rg == 11){
        val = "t3";
        }
    if(rg == 12){
        val = "t4";
        }
    if(rg == 13){
        val = "t5";
        }
    if(rg == 14){
        val = "t6";
        }
    if(rg == 15){
        val = "t7";
        }
    if(rg == 16){
        val = "s0";
        }
    if(rg == 17){
        val = "s1";
        }
    if(rg == 18){
        val = "s2";
        }
    if(rg == 19){
        val = "s3";
        }
    if(rg == 20){
        val = "s4";
        }
    if(rg == 21){
        val = "s5";
        }
    if(rg == 22){
        val = "s6";
        }
    if(rg == 23){
        val = "s7";
        }
    if(rg == 24){
        val = "t8";
        }
    if(rg == 25){
        val = "t9";
        }
    if(rg == 26){
        val = "k0";
        }
    if(rg == 27){
        val = "k1";
        }
    if(rg == 28){
        val = "gp";
        }
    if(rg == 29){
        val = "sp";
        }
    if(rg == 30){
        val = "fp";
        }
    if(rg == 31){
        val = "ra";
        }
        return val;
}

int binToDec(string address){
    return stoi(address, nullptr, 2);
}

void ControlUnit(string op){
	// generates control signals using opcode
	// initialize w/ zeros

    if (op == "000000"){ // ADD, SUB, AND, OR, NOR, SLT
        regWrite = 1;
        regDST = 1;
        branch = 0;
        aluSRC = 0;
        memWrite = 0;
        memToReg = 0;
        memRead = 0;
        jump = 0;
    }
    if (op == "100011"){ // LW
        regWrite = 1;
        regDST = 0;
        branch = 0;
        aluSRC = 1;
        memWrite = 0;
        memToReg = 1;
        memRead = 1;
        jump = 0;
    }
    if (op == "101011"){ // SW
        regWrite = 0;
        branch = 0;
        aluSRC = 1;
        memWrite = 1;
        memRead = 0;
        jump = 0;
    }
    if (op == "000100"){ // BEQ
        regWrite = 0;
        branch = 1;
        aluSRC = 0;
        memWrite = 0;
        memRead = 0;
        jump = 0;
    }
    if (op == "000010"){ // J format
        regWrite = 0;
        branch = 0;
        memWrite = 0;
        memRead = 0;
        jump = 1;
    }

    if (op == "000011"){ // JAL
        regWrite = 1;
        regDST = 10;
        memWrite = 0;
        memRead = 0;
        memToReg = 10;
        jump = 1;
    }
    if (op == "001000"){ // JR
        regWrite = 0;
        branch = 0;
        memWrite = 0;
        jumpREG = 1;
        jump = 1;
    }

}

void Writeback(int rg, int result){
	// Get computation results from ALU and data from data memory and update destination register in registerfile array
	// Last step of instruction excecution

    if (regWrite == 0 && memWrite == 1){ // SW
        d_mem[rg] = result;
        cout << "memory 0x" << hex << rg << " is modified to 0x" << hex << result << endl;
    }else if (regWrite == 1){ // R AND LW
        regfile[rg] = result;
        cout << "$" << rv(rg) << " is modified to 0x" << hex << result << endl;
    }
    total_clock_cycles = total_clock_cycles + 1;
}

void Mem(int rg, int address){
	// Each entry of d_mem array will be accessed w/ following addresses
	// refer to project description
    int val = 0;
    if (memWrite == 1){ //SW
        val = regfile[rg];
        Writeback(address, val);
    }else if (memRead == 1){ //lw
        val = d_mem[address/4]; // divide by 4 to place it in the memory by 4
        Writeback(rg, val);
    }else{ // R/I
        Writeback(rg,address);
    }
}

void execute(string alu_op, int rs, int rt, int rd, int shamt, int address){
	// Should recieve 4-bit alu_op input and run operations (page 10 of Processor-2 slides)
	// alu-zero for zero output
	// branch target address - should also calculate branch target address named branch_target
	// page 22 of Processor-1 slides
	// shift-left-2 of sign-extended offset input
	// add shift-left-2 w/ PC+4 value
    int val = 0;
    
    if (alu_op == "0000"){ //AND
        val = regfile[rs] & regfile[rt]; 
    } 
    if (alu_op == "0001"){  //OR
        val = regfile[rs] | regfile[rt]; 
    }
    if (alu_op == "0110"){ //SUBTRACT AND BEQ
        val = regfile[rs] - regfile[rt]; 
    }
    if (alu_op == "1100"){ //NOR
        val = ~(regfile[rs] | regfile[rt]); 
    }
    if (alu_op == "0010"){ // LW AND SW
        if (aluSRC == 1){
            val = regfile[rs] + address;
        }else{ // Else add
            val = regfile[rs] + regfile[rt];
        }
    }
    if (alu_op == "0111"){ // SLT
        if (regfile[rs] < regfile[rt]){
            val = 1;
        }else{
            val = 0;
        }
    }

    if(memToReg == 10){ //JAL
        cout << "somethin happen" << endl;
        jump_target = 31;
        Mem(jump_target, address);
    }
    //checking BEQ with alu_zero. if 1, jump to branch.
    if (val == 0 && branch == 1){
        alu_zero = 1;
    }else{
        alu_zero = 0;
    }

    // check LW, SW, or R/I type
    if (memWrite == 1 || memRead == 1){ //LW and SW
        Mem(rt, val);
    }else{ // Otherwise R or I type
        Mem(rd, val);
    }

}

void decode(string str){
	// should be able to do sign-extension for offset field of I-type instructions
	// for jump - fill jump address in jump_target, run shift-left-2 then merge with first 4 bits of next_pc
	int rs = 0;
	int rt = 0;
	int rd = 0;
	int shamt;
	int address;
	string funct;
	string immediate;
	string alu_op;
	string opcode = str.substr(0, 6); // get opcode

	ControlUnit(opcode); //control unit

	if(jump == 1){
		immediate = str.substr(6, 26);
		address = immCheck(immediate);
		address = address << 2;
        cout << "address :" << address << endl;
		jump_target = address;
	}
    else if(branch == 1){
		rs = binToDec(str.substr(6, 5));
        rt = binToDec(str.substr(11, 5));
		alu_op = alu(opcode);
		immediate = str.substr(16,16);
		address = immCheck(immediate);
		address = address << 2;
		branch_target = address;
	}
    else{
		rs = binToDec(str.substr(6, 5));
        if (regDST == 1 && regWrite == 1){
            rd = binToDec(str.substr(16, 5));
        }else{ //LW and SW
            rt = binToDec(str.substr(11, 5));
        }

        if (aluSRC == 0){ // R
            rt = binToDec(str.substr(11, 5));
            shamt = binToDec(str.substr(21,5));
            funct = str.substr(26, 6);
            alu_op = alu(funct); //alu_op determined by funct  
        }
        else{ // Immediate LW and SW
            immediate = str.substr(16, 16);
            alu_op = alu(opcode); //alu_op determined by opcode
            address = immCheck(immediate);
        }
	}
    
    execute(alu_op, rs, rt, rd, shamt, address);
	
}

void Fetch(vector<string> instruct){
	// reads one instruction from the input program text file per cycle
	// when reading one inst. per from txt file, increment pc by 4 and read i*4 instruction
	// if pc is 4, read first instruction
	// next_pc = pc + 4
	// add a logic that copies one of three possible values (next_pc, branch_target, jump_target)
	// to pc variable

	while(pc/4 < instruct.size()){
		cout << "total_clock_cycles " << dec << total_clock_cycles + 1 << ":" << endl;

		decode(instruct[pc/4]);
		
		int next_pc = pc + 4; // update pc

		// logic for copying values
		if (alu_zero == 1 && branch == 1){
            pc = next_pc + branch_target;
        }else if(jump == 1){
            pc = jump_target;
        }else{
            pc = next_pc;
        }
		cout << "pc is modified to 0x" << hex << pc << endl << endl;
	
	}

	cout << "program terminated:" << endl;
	cout << "total execution time: " << total_clock_cycles << " cycles." << endl;

}

int main() {
    
	vector<string> instruction; // vector to store text file contents
	string line; //line of code from text file
	string filename; //hardcoded filenames
	int sample; //user input of which file

        cout << "Enter file number to run: "; //user file choice
        cin >> sample;
    if(sample == 1){ //file 1
        regfile[9] = 0x20;
        regfile[10] = 0x5;
        regfile[16] = 0x70;
        d_mem[28] = 0x5;
        d_mem[29] = 0x10;
        filename = "sample_part1.txt";
        cout << "you chose file 1\n" << endl;
    }
    else if(sample == 2){ //file 2
        regfile[16] = 0x20;
        regfile[4] = 0x5;
        regfile[5] = 0x2;
        regfile[7] = 0xa;
        filename = "sample_part2.txt";
        cout << "you chose file 2\n" << endl;
    }

    ifstream file(filename); // load text file into the vector
    while(getline(file, line)){
        instruction.push_back(line);
    }
    Fetch(instruction);

}