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
int registerfile[31] = {0}; // initialize registerfile entries with 0
int d_mem [31] = {0}; // initialize d_mem entries with 0
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
        immediate = "-" + negCheck(immediate);
    }
    return stoi(immediate, nullptr, 2);
}

void regVal(int rg){
    if(rg == 0){ cout << "zero";}
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

int binToDec(string address){
    return stoi(address, nullptr, 2);
}

void ControlUnit(string op){
	// called by decode to generate control signals
	// recieves 6-bit opcode value and generate 9 control signals
	// page 3 of processor-3 slide
	// declare one global signal per control signal (RegWrite, RegDist, ...) and initialize w/ zeros

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
        registerfile[rg] = result;
        cout << "$";
        regVal(rg);
        cout << " is modified to 0x" << hex << result << endl;
    }
    total_clock_cycles = total_clock_cycles + 1;
}

void Mem(int rg, int address){
	// Each entry of d_mem array will be accessed w/ following addresses
	// refer to project description
    int val = 0;
    if (memWrite == 1){ //SW
        val = registerfile[rg];
        Writeback(address, val);
    }else if (memRead == 1){ //lw
        val = d_mem[address/4]; // divide by 4 to place it in the memory by 4
        Writeback(rg, val);
    }else{ // R/I
        Writeback(rg,address);
    }
}

void Execute(string alu_op, int rs, int rt, int rd, int shamt, int address){
	// Should recieve 4-bit alu_op input and run operations (page 10 of Processor-2 slides)
	// alu-zero for zero output
	// branch target address - should also calculate branch target address named branch_target
	// page 22 of Processor-1 slides
	// shift-left-2 of sign-extended offset input
	// add shift-left-2 w/ PC+4 value
    int val = 0;
    
    if (alu_op == "0000"){ //AND
        val = registerfile[rs] & registerfile[rt]; 
    } 
    if (alu_op == "0001"){  //OR
        val = registerfile[rs] | registerfile[rt]; 
    }
    if (alu_op == "0110"){ //SUBTRACT AND BEQ
        val = registerfile[rs] - registerfile[rt]; 
    }
    if (alu_op == "1100"){ //NOR
        val = ~(registerfile[rs] | registerfile[rt]); 
    }
    if (alu_op == "0010"){ // LW AND SW
        if (aluSRC == 1){
            val = registerfile[rs] + address;
        }else{ // Else add
            val = registerfile[rs] + registerfile[rt];
        }
    }
    if (alu_op == "0111"){ // SLT
        if (registerfile[rs] < registerfile[rt]){
            val = 1;
        }else{
            val = 0;
        }
    }

    if(memToReg == 10){
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
    }else{ // Otherwise R/I
        Mem(rd, val);
    }

}

void Decode(string str){
	// Similar to lab 3
	// should be able to do sign-extension for offset field of I-type instructions
	// page 15 of lecture slide
	// for jump - fill jump address in jump_target, run shift-left-2
	// then merge with first 4 bits of next_pc
	int rs = 0;
	int rt = 0;
	int rd = 0;
	int shamt;
	int address;
	string funct;
	string immediate;
	string alu_op;
	string firstSix = str.substr(0, 6); // get opcode

	// cout << firstSix << endl;

	ControlUnit(firstSix); // check opcode

	if(jump == 1){
		immediate = str.substr(6, 26);
		address = immCheck(immediate);
		address = address << 2;
		jump_target = address;
	}else if(branch == 1){
		rs = binToDec(str.substr(6, 5));
        rt = binToDec(str.substr(11, 5));
		alu_op = alu(firstSix);
		immediate = str.substr(16,16);
		address = immCheck(immediate);
		address = address << 2;
		branch_target = address;
	}else{
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
        }else{ // Immediate LW and SW
            immediate = str.substr(16, 16);
            alu_op = alu(firstSix); //alu_op determined by opcode
            address = immCheck(immediate);
        }
	}
    
    Execute(alu_op, rs, rt, rd, shamt, address);
	
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

		Decode(instruct[pc/4]);
		
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
    // sample_part1.txt
    //register initial
    // $t1 = 0x20
    // $t2 = 0x5 
    // $s0 = 0x70
    // d_mem initial
    // 0x70 = 0x5
    // 0x74 = 0x10

    // registerfile[9] = 0x20;
    // registerfile[10] = 0x5;
    // registerfile[16] = 0x70;
    // d_mem[28] = 0x5;
    // d_mem[29] = 0x10;

    // sample_part2.txt
    //register initial
    // $s0 = 0x20
    // $a0 = 0x5 
    // $a1 = 0x2 
    // $a3 = 0xa

    // registerfile[16] = 0x20;
    // registerfile[4] = 0x5;
    // registerfile[5] = 0x2;
    // registerfile[7] = 0xa;


    // get 32 bit binary from text file
	vector<string> instruction; // create vector for storing mips instruction
	string str; // str string for reading from filename using getline
	string filename; // string for the name of file
	

    cout << "Enter file name to run:\n"; // file name input
    cin >> filename;

    if(filename == "sample_part1.txt"){
        registerfile[9] = 0x20;
        registerfile[10] = 0x5;
        registerfile[16] = 0x70;
        d_mem[28] = 0x5;
        d_mem[29] = 0x10;
    }else if(filename == "sample_part2.txt"){
        registerfile[16] = 0x20;
        registerfile[4] = 0x5;
        registerfile[5] = 0x2;
        registerfile[7] = 0xa;
    }

    ifstream file(filename); // input file to ifstream for operation
    while(getline(file, str)){
        instruction.push_back(str);
    }
    Fetch(instruction);

}
// Fetch(), Decode(), Execute(), Mem(), and 
//WriteBack() of the first part (supporting 10 instructions)