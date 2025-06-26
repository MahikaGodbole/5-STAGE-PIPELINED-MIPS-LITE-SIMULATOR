// ECE586_FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h> 
#include <iostream> 	//input-output stream
#include <fstream>		//file stream
#include <string>		//string classes like string, wstring etc.
#include <sstream>		//string stream functionality
#include <filesystem>	//Retrieve information about paths, files, directories
#include <string.h>		//allows for strcpy, strlen functions
using namespace std;


/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Function Prototypes
*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
string get_input_file();
string decode_hex(string hexa_address);
int decode_instructions(string binary_address,int Register[], int Memory[], int counter[], int register_change[], int memory_change[], uint8_t special, int PC, string DynamicPCArray[]);
void print_results(int counter[], int Register[], int Memory[],int register_change[], int memory_change[], uint8_t special, int PC);
void update_register_changes(string binary_address, int register_change[], bool if_imm);
void update_memory_changes(string binary_address,int Register[], int register_change[]);
void check_pipeline(void);
void simulate_timing_without_fwd(string binary_code, int Register[], int register_tracking[], int &CC_without_fwd, int &stall_by_RAW_without_fwd, int &stall_by_branch_without_fwd, int &stall_by_RAW_without_fwd_counter, int &stall_by_branch_without_fwd_counter);
void simulate_timing_with_fwd(string binary_code, int Register[], int register_tracking[], int& CC_with_fwd, int& stall_by_RAW_with_fwd, int& stall_by_branch_with_fwd, int& stall_by_RAW_with_fwd_counter, int& stall_by_branch_with_fwd_counter);
void track_register_changes(int register_tracking[], int r_des, bool stall_by_2, bool stall_by_1);









/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Instruction class with declared functions + definitions
Contains the following functions:
    Arithmetic
        Add, ADDI
        SUB, SUBI
        MUL, MULI
    Logical
        OR, ORI
        AND, ANDI
        XOR, XORI
    Memory
        LDW
        STW
    Control
        BEZ,
        BNEZ,
        BEQ,
        JR
        HALT
*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










class instructions_exe {

public:

    /*/////////////////////////////////////////////////////////////
    If the binary value is negative (leading 1), take the one's complement
    Add 1 to the resulting value and return the result
    *//////////////////////////////////////////////////////////////
    int complement_of_two(string binary_16_bit) {

        for (int i = 0; i < binary_16_bit.length(); i++) {
            if (binary_16_bit[i] == '1') {
                binary_16_bit[i] = '0';
            }
            else {
                binary_16_bit[i] = '1';
            }
        }
        cout << "binary_16_bit" << endl;
        int dec_16b_1comp = stoi(binary_16_bit, nullptr, 2);
        cout << dec_16b_1comp << endl;
        int dec_16b_2comp = dec_16b_1comp + 1;

        return dec_16b_2comp;
    }


    /*/////////////////////////////////////////////////////////////
    Addition function of values in two registers
    Stores in target register
    *//////////////////////////////////////////////////////////////
    void addition_function(string binary_address, int Register[], int Memory[]) {

        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string destination_reg_bin = binary_address.substr(16, 5);		//Gets destination_reg binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int destination_reg = stoi(destination_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

        //Set the value to the register, [destination_reg] = [source_reg] + [target_reg]
        //register 0 must remain a value of 0. It cannot be changed in MIPS
        if (destination_reg != 0) {
            Register[destination_reg] = Register[source_reg] + Register[target_reg];
        }
        //Print operands and result
        cout << "r"<< destination_reg <<": " << Register[destination_reg] << "; r"<< source_reg <<": " << Register[source_reg] << "; r"<< target_reg <<": " << Register[target_reg] << endl;
    }
    /*/////////////////////////////////////////////////////////////
    Subtraction function of values in two registers
    Stores in target register
    *//////////////////////////////////////////////////////////////
    void subtraction_function(string binary_address, int Register[], int Memory[]) {
        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string destination_reg_bin = binary_address.substr(16, 5);		//Gets destination_reg binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int destination_reg = stoi(destination_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

        //Set the value to the register, [destination_reg] = [source_reg] - [target_reg]
        //register 0 must remain a value of 0. It cannot be changed in MIPS
        if (destination_reg != 0) {
            Register[destination_reg] = Register[source_reg] - Register[target_reg];
        }
        //Print operands and result
        cout << "r" << destination_reg << ": " << Register[destination_reg] << "; r" << source_reg << ": " << Register[source_reg] << "; r" << target_reg << ": " << Register[target_reg] << endl;
    }
    /*/////////////////////////////////////////////////////////////
    Multiplication function of values in two registers
    Stores in target register
    *//////////////////////////////////////////////////////////////
    void multiplication_function(string binary_address, int Register[], int Memory[]) {
        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string destination_reg_bin = binary_address.substr(16, 5);		//Gets destination_reg binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);           //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int destination_reg = stoi(destination_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

        //Set the value to the register, [destination_reg] = [source_reg] * [target_reg]      
        //register 0 must remain a value of 0. It cannot be changed in MIPS       
        if (destination_reg != 0) {
            Register[destination_reg] = Register[source_reg] * Register[target_reg];
        }
        //Print operands and result
        cout << "r" << destination_reg << ": " << Register[destination_reg] << "; r" << source_reg << ": " << Register[source_reg] << "; r" << target_reg << ": " << Register[target_reg] << endl;
    }
    /*/////////////////////////////////////////////////////////////
    Add Immediate function. Adds value in one register with an immediate value
    Stores in target register
    *//////////////////////////////////////////////////////////////	
    void addition_immediate_function(string binary_address, int Register[], int Memory[]) {

        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction		
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string immBinary = binary_address.substr(16, 16);		//Gets immValue binary string from binary instruction
        int immValue = 0;

        if (immBinary.substr(0, 1) == "1") {
            cout << "Negative number... Need to use 2s' complement" << endl;
            int twoComplementToDecimal = complement_of_two(immBinary);
            immValue = twoComplementToDecimal * (-1);      //Sets integer to negative value
        }
        else {
            immValue = stoi(immBinary, nullptr, 2);      //takes binary string and turns to an integer
        }

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

        //Set the value to the register, [target_reg] = [source_reg] + immValue
        //register 0 must remain a value of 0. It cannot be changed in MIPS
        if (target_reg != 0) {
            Register[target_reg] = Register[source_reg] + immValue;
        }
        //Print operands and results
        cout << "r" << target_reg << ": " << Register[target_reg] << "; r" << source_reg << ": " << Register[source_reg] << "; immValue: " << immValue << endl;

    }
    /*/////////////////////////////////////////////////////////////
    Subtract Immediate function. Adds value in one register with an immediate value
    Stores in target register
    *//////////////////////////////////////////////////////////////	
    void subtract_immediate_function(string binary_address, int Register[], int Memory[]) {

        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string immBinary = binary_address.substr(16, 16);		//Gets immValue binary string from binary instruction

        int immValue = 0;
        if (immBinary.substr(0, 1) == "1") {
            cout << "Negative number... Need to use 2s' complement" << endl;
            int twoComplementToDecimal = complement_of_two(immBinary);
            immValue = twoComplementToDecimal * (-1);      //Sets integer to negative value
        }
        else {
            immValue = stoi(immBinary, nullptr, 2);      //takes binary string and turns to an integer
        }

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);      //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);      //takes binary string and turns to an integer

        //Set the value to the register, [target_reg] = [source_reg] - immValue
        //register 0 must remain a value of 0. It cannot be changed in MIPS
        if (target_reg != 0) {
            Register[target_reg] = Register[source_reg] - immValue;
        }

        //Print operands and results
        cout << "r"<< target_reg <<": " << Register[target_reg] << "; r"<< source_reg <<": " << Register[source_reg] << "; immValue: " << immValue << endl;

    }
    /*/////////////////////////////////////////////////////////////
    Multiply Immediate function. Adds value in one register with an immediate value
    Stores in target register
    *//////////////////////////////////////////////////////////////	
    void multiply_immediate_function(string binary_address, int Register[], int Memory[]) {

        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string immBinary = binary_address.substr(16, 16);		//Gets immValue binary string from binary instruction

        int immValue = 0;
        if (immBinary.substr(0, 1) == "1") {
            cout << "Negative number... Need to use 2s' complement" << endl;
            int twoComplementToDecimal = complement_of_two(immBinary);
            immValue = twoComplementToDecimal * (-1);      //Sets integer to negative value
        }
        else {
            immValue = stoi(immBinary, nullptr, 2);      //takes binary string and turns to an integer
        }

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);      //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);      //takes binary string and turns to an integer

        //Set the value to the register, [target_reg] = [source_reg] * immValue
        //register 0 must remain a value of 0. It cannot be changed in MIPS
        if (target_reg != 0) {
            Register[target_reg] = Register[source_reg] * immValue;
        }

        //Print operands and results
        cout << "r" << target_reg << ": " << Register[target_reg] << "; r" << source_reg << ": " << Register[source_reg] << "; immValue: " << immValue << endl;

    }

    /*/////////////////////////////////////////////////////////////
    Logical functions:
    - OR
    - OR Immediate
    - AND
    - AND Immediate
    - XOR
    - XOR Immediate
    *//////////////////////////////////////////////////////////////	
    /*/////////////////////////////////////////////////////////////
    OR function. ORs values of in 2 registers
    Stores in target register
    *//////////////////////////////////////////////////////////////	










    void OR_function(string binary_address, int Register[], int Memory[]) {

        //Decode the instruction
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string destination_reg_bin = binary_address.substr(16, 5);		//Gets destination_reg binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);      //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);      //takes binary string and turns to an integer
        int destination_reg = stoi(destination_reg_bin, nullptr, 2);      //takes binary string and turns to an integer

        //Set the value to the register, [destination_reg] = [source_reg] | [target_reg]
        //register 0 must remain a value of 0. It cannot be changed in MIPS
        if (destination_reg != 0) {
            Register[destination_reg] = Register[source_reg] | Register[target_reg];
        }

        //Print operands and result
        cout << "r" << destination_reg << ": " << Register[destination_reg] << "; r" << source_reg << ": " << Register[source_reg] << "; r" << target_reg << ": " << Register[target_reg] << endl;
    }
    /*/////////////////////////////////////////////////////////////
    OR Immediate function. ORs value in one register with an immediate value
    Stores in target register
    *//////////////////////////////////////////////////////////////	
    void OR_immediate_function(string binary_address, int Register[], int Memory[]) {

        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string immBinary = binary_address.substr(16, 16);		//Gets immValue binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int immValue = stoi(immBinary, nullptr, 2);        //takes binary string and turns to an integer

        //Set the value to the register, [target_reg] = [source_reg] | immValue
        //register 0 must remain a value of 0. It cannot be changed in MIPS
        if (target_reg != 0) {
            Register[target_reg] = Register[source_reg] | immValue;
        }

        //Print operands and results
        cout << "r" << target_reg << ": " << Register[target_reg] << "; r" << source_reg << ": " << Register[source_reg] << "; immValue: " << immValue << endl;
    }
    /*/////////////////////////////////////////////////////////////
    AND function. ANDs values of in 2 registers
    Stores in target register
    *//////////////////////////////////////////////////////////////	


    void AND_function (string binary_address, int Register[], int Memory[]) {
        //Decode the instruction
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string destination_reg_bin = binary_address.substr(16, 5);		//Gets destination_reg binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int destination_reg = stoi(destination_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

        //Set the value to the register, [destination_reg] = [source_reg] & [target_reg]
       //register 0 must remain a value of 0. It cannot be changed in MIPS
        if (destination_reg != 0) {
            Register[destination_reg] = Register[source_reg] & Register[target_reg];
        }

        //Print operands and result
        cout << "r" << destination_reg << ": " << Register[destination_reg] << "; r" << source_reg << ": " << Register[source_reg] << "; r" << target_reg << ": " << Register[target_reg] << endl;
    }
    /*/////////////////////////////////////////////////////////////
    AND Immediate function. ORs value in one register with an immediate value
    Stores in target register
    *//////////////////////////////////////////////////////////////	
    void AND_immediate_function (string binary_address, int Register[], int Memory[]) {
        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string immBinary = binary_address.substr(16, 16);		//Gets immValue binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int immValue = stoi(immBinary, nullptr, 2);          //takes binary string and turns to an integer

        //Set the value to the register, [target_reg] = [source_reg] & immValue
        //register 0 must remain a value of 0. It cannot be changed in MIPS
        if (target_reg != 0) {
            Register[target_reg] = Register[source_reg] & immValue;
        }

        //Print operands and result
        cout << "r" << target_reg << ": " << Register[target_reg] << "; r" << source_reg << ": " << Register[source_reg] << "; immValue: " << immValue << endl;
    }
    /*/////////////////////////////////////////////////////////////
    XOR function. XORs values of in 2 registers
    Stores in target register
    *//////////////////////////////////////////////////////////////	
    void XOR_function (string binary_address, int Register[], int Memory[]) {
        //Decode the instruction
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string destination_reg_bin = binary_address.substr(16, 5);		//Gets destination_reg binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int destination_reg = stoi(destination_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

        //Set the value to the register, [destination_reg] = [source_reg] ^ [target_reg]
       //register 0 must remain a value of 0. It cannot be changed in MIPS
        if (destination_reg != 0) {
            Register[destination_reg] = Register[source_reg] ^ Register[target_reg];
        }

        //Print operands and result
        cout << "r" << destination_reg << ": " << Register[destination_reg] << "; r" << source_reg << ": " << Register[source_reg] << "; r" << target_reg << ": " << Register[target_reg] << endl;
    }
    /*/////////////////////////////////////////////////////////////
    XOR Immediate function. ORs value in one register with an immediate value
    Stores in target register
    *//////////////////////////////////////////////////////////////	
    void XOR_imediate_function (string binary_address, int Register[], int Memory[]) {
        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction 
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string immBinary = binary_address.substr(16, 16);		//Gets immValue binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int immValue = stoi(immBinary, nullptr, 2);          //takes binary string and turns to an integer

        //register 0 must remain a value of 0. It cannot be changed in MIPS       
        if (target_reg != 0) {
            //Set the value to the register, [target_reg] = [source_reg] & immValue
            Register[target_reg] = Register[source_reg] ^ immValue;
        }

        //Print operands and result
        cout << "r" << target_reg << ": " << Register[target_reg] << "; r" << source_reg << ": " << Register[source_reg] << "; immValue: " << immValue << endl;
    }

    /*//////////////////////////////////////////////////////////////
    Memory Access Instructions
    - LDW
    - STW
    *///////////////////////////////////////////////////////////////
    /*/////////////////////////////////////////////////////////////
    LDW function. Reads value from memory address (that is stored in register)
    and writes value to a register
    *//////////////////////////////////////////////////////////////	
    void loadWord (string binary_address, int Register[], int Memory[], string DynamicPCArray[]) {
        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string immBinary = binary_address.substr(16, 16);		//Gets immValue binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2); 		    //Gets source_reg binary string from binary instruction
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        
        int immValue = 0;

        if (immBinary.substr(0, 1) == "1") {
            cout << "Negative number... Need to use 2s' complement" << endl;
            int twoComplementToDecimal = complement_of_two(immBinary);
            immValue = twoComplementToDecimal * (-1);      //Sets integer to negative value
        }
        else {
            immValue = stoi(immBinary, nullptr, 2);      //takes binary string and turns to an integer
        }

       //register 0 must remain a value of 0. It cannot be changed in MIPS
        if (target_reg != 0) {
            //Set the value to the register, [target_reg] = Mem[[source_reg] + immValue]
            cout << source_reg << endl;
            cout << Register[source_reg] << endl;
            cout << "immValue: " << immValue << endl;
            int memory_address = (Register[source_reg] + immValue)/4;
            cout << "Memory: " << memory_address << endl;
            string memory_data = DynamicPCArray[memory_address];

            for (int i = 0; i < memory_data.length(); i++) {

                memory_data[i] = toupper(memory_data[i]);
            }
            

            cout << memory_data << endl;
            string bin_ldw = decode_hex(memory_data);
            cout << bin_ldw << endl;
            int mem_data_dec = 0;

            if (bin_ldw.substr(0, 1) == "1") {
                cout << "Negative number... Need to use 2s' complement" << endl;
                int twoComplementToDecimal = complement_of_two(bin_ldw);
                mem_data_dec = twoComplementToDecimal * (-1);      //Sets integer to negative value
            }
            else {
                mem_data_dec = stoi(bin_ldw, nullptr, 2);      //takes binary string and turns to an integer
            }

            cout << mem_data_dec << endl;

            Register[target_reg] = mem_data_dec;
        }

        //Print operands and result
        cout << "R" << target_reg << ": " << Register[target_reg] << ";LW from Mem Address R" <<source_reg<<" + " << immValue <<" : " << Register[source_reg] + immValue << endl;
    }
    /*/////////////////////////////////////////////////////////////
    STW function. Writes value in one register to the memory address
    that is stored in another register
    *//////////////////////////////////////////////////////////////	
    void storeWord (string binary_address, int Register[], int Memory[]) {
        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string immBinary = binary_address.substr(16, 16);		//Gets immValue binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        
        int immValue = 0;

        if (immBinary.substr(0, 1) == "1") {
            cout << "Negative number... Need to use 2s' complement" << endl;
            int twoComplementToDecimal = complement_of_two(immBinary);
            immValue = twoComplementToDecimal * (-1);      //Sets integer to negative value
        }
        else {
            immValue = stoi(immBinary, nullptr, 2);      //takes binary string and turns to an integer
        }

        //Set the value to the register, Mem[[source_reg] + immValue] = [target_reg]
        Memory[Register[source_reg] + immValue] = Register[target_reg];

        //Print operands and result
        cout << "SW in Mem Address: " << Register[source_reg] + immValue << ";Data: " << Register[target_reg] << endl;
    }
    /*//////////////////////////////////////////////////////////////
    Control Flow Instructions
    - BZ
    - BEQ
    - JR
    -Halt --> Handled in instruction decoder since it just calls on pr
    inting results
    *///////////////////////////////////////////////////////////////
    /*/////////////////////////////////////////////////////////////
        STW function. Writes value in one register to the memory address
        that is stored in another register
        *//////////////////////////////////////////////////////////////	
    int branchIfZero (string binary_address, int Register[], int PC){
        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string immBinary = binary_address.substr(16, 16);		//Gets immValue binary string from binary instruction

        int immValue = 0;
        if (immBinary.substr(0, 1) == "1") {
            cout << "Negative number... Need to use 2s' complement" << endl;
            int twoComplementToDecimal = complement_of_two(immBinary);
            immValue = twoComplementToDecimal * (-1);      //Sets integer to negative value
        }
        else {
            immValue = stoi(immBinary, nullptr, 2);      //takes binary string and turns to an integer
        }
        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

        //If contents of source_reg equal 0, update the PC with the immediate value
        if (Register[source_reg] == 0) {
            PC = PC + immValue - 1;
            cout << "R[" << source_reg << "]: " << Register[source_reg];               //should say 0
            cout << " New PC: " << PC + 1 << endl;           //New program counter based on the immediate value + PC
        }
        else {
            cout << "R[" << source_reg << "]!= 0;";               //should say 0
            cout << " PC: " << PC << endl;;           //New program counter based on the immediate value + PC
        }
        return PC;
    }
    /*/////////////////////////////////////////////////////////////
        STW function. Writes value in one register to the memory address
        that is stored in another register
        *//////////////////////////////////////////////////////////////	
    int branchIfRegistersEqual (string binary_address, int Register[], int PC) {
        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
        string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
        string immBinary = binary_address.substr(16, 16);		//Gets immValue binary string from binary instruction
       
        int immValue = 0;
        if (immBinary.substr(0, 1) == "1") {
            cout << "Negative number... Need to use 2s' complement" << endl;
            int twoComplementToDecimal = complement_of_two(immBinary);
            immValue = twoComplementToDecimal * (-1);      //Sets integer to negative value
        }
        else {
            immValue = stoi(immBinary, nullptr, 2);      //takes binary string and turns to an integer
        }
        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
        int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

        cout << "source_reg: " << Register[source_reg] << ". target_reg: " << Register[target_reg] << endl;
        //If contents of source_reg equal contents of target_reg, update the PC with the immediate value
        if (Register[source_reg] == Register[target_reg]) {
            PC = PC + immValue - 1;
            cout << "R[" << source_reg << "]" << "equals R[" << target_reg << "] = " << Register[source_reg];
            cout << " New PC: " << PC + 1 << endl;;
        }
        else {
            cout << "R[" << source_reg << "]" << "!= R[" << target_reg << "];";
            cout << " PC: " << PC << endl;
        }
        
        return PC;
    }
    /*/////////////////////////////////////////////////////////////
        STW function. Writes value in one register to the memory address
        that is stored in another register
        *//////////////////////////////////////////////////////////////	
    int jumpToRegister(string binary_address, int Register[], int PC) {
        //Get instruction decoder
        string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction

        //Convert the binary value to dec
        int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

        PC = Register[source_reg];
        PC = PC / 4;
        cout << "New PC: " << PC << endl;
        PC = PC - 1;
        return PC;
    }
};

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Create an Register Array Reg[32] to store R0-R31 register
//Create an Memorry Array Mem[32768] to store all address content
Main Function:

*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




int main(){
    //#pragma comment(linker, "/STACK:8000000")
    string test_line;
    string user_test_file;
    ifstream inFile;
    ofstream test, PCprint, MemImageOutput;
    string binary_code;
    int Register [32];          //Holds register values, register 0 cannot be written to
    int Memory[32768];          //Holds memory values  
    int register_change[32];
    int memory_change[32768];
    int counter[5];             //Keeps track of type of instructions
    uint8_t special = 0;        //tells print whether it is a halt or not. Initialize to 0
    int register_tracking[5];
    
    //PC 
    string DynamicPCArray[1024];      //Points to a address which holds string data.
    int PC = -1;                 //Initializes program counter
    int ProgramEnd;             //Integer that holds total length of PC

    //Timing simulator without forwarding
    int CC_without_fwd = 0;
    int stall_by_RAW_without_fwd = 0;
    int stall_by_branch_without_fwd = 0;
    int stall_by_RAW_without_fwd_counter = 0;
    int stall_by_branch_without_fwd_counter = 0;

    //Timing simulator without forwarding
    int CC_with_fwd = 0;
    int stall_by_RAW_with_fwd = 0;
    int stall_by_branch_with_fwd = 0;
    int stall_by_RAW_with_fwd_counter = 0;
    int stall_by_branch_with_fwd_counter = 0;



    //initialize register array to values of 0
    for (int i = 0; i < 32; i++) {
        Register[i] = 0;                //initialize all registers to a value of 0
        register_change[i] = -999;      //just sets register_change for all 32 registers to a randomly chosen number, -999
    }

    //initialize memory array to values of 0
    for (int i = 0; i < 32768; i++) {
        Memory[i] = 0;          //Sets all memory address spaces to a value of 0
        memory_change[i] = -999;        //Sets all memory change spaces to a value of 0.
    }

    //initialize counter array to values of 0
    for (int i = 0; i < 5; i++) {
        counter[i] = 0;         //initialize all instruction type counters to 0
    }

    //initialize register tracking array
    for (int i = 0; i < 5; i++) {
        register_tracking[i] = -999;         //initialize all instruction type counters to 0
    }

    /*/////////////////////////////////////////////////////////////
    This section is where the program grabs the list of instructions
    And stores the instructions in a instruction memory array
    It also determines:
    - range of instruction addresses (PC)
    - Total instruction memory space
    *//////////////////////////////////////////////////////////////	


    user_test_file = get_input_file();	//user input file - gets user test file
    inFile.open(user_test_file);		//opens user test file
	
    //This gets the length of the program in single integer values
    //true PC is PC * 4
    while (!inFile.eof()){
        inFile >> test_line;
        if (!inFile.eof()) {
            if (PC >= 0) {
                PC = PC + 1;
            }
            else if (PC < 0) {
                PC = 0;
            }
        }
    }
    PC = PC + 1;
    ProgramEnd = PC;            //We count 0 as the PC start. Subtract 1 because the above while loop adds 1 more and if it does not get a line then ends. Need to remove the last read.
    cout << "Address seen as single bits: 0 to " << ProgramEnd << endl;  //Prints the total array address spce. This is the range used to access
    cout << "Total Program Addresses:0 to " << ProgramEnd*4 << endl;     //Print the total program end from 0 - Program End
    cout << "Total Program Memory Space is:0 to " << (ProgramEnd + 1) * 4 << endl;       //Shows total memory space.
    /*/////////////////////////////////////////////////////////////
    Create the new array given the total length of the program
      This will allow us to go back and forth in the program when
      branching
    *///////////////////////////////////////////////////////////
    //DynamicPCArray = new string[ProgramEnd];
   
    /*////////////////////////////////////
    //Debugging
   for (PC = 0; PC <= ProgramEnd; PC++) {
        cout << PC << endl;
    }
    *////////////////////////////////////

    //sets file back to the start to prepare for reading into our new array
    inFile.clear();
    inFile.seekg(0);
    
    test.open("test.txt");
    PCprint.open("PCTest.txt");
    //Reading strings into 
    for (PC = 0; PC <= ProgramEnd; PC++) {
        inFile >> test_line;
        if(!inFile.eof()){
            DynamicPCArray[PC] = test_line;
            test << DynamicPCArray[PC] << endl;
        }
        PCprint << PC << endl;
    }
    //Close File
    inFile.close();

    //start at beginning of the program and read in the values
    for (PC = 0; PC <= ProgramEnd; PC++) {
        cout << "-----------------------------" << endl;
        cout << "PC: " << PC << endl;
        cout << "Trace: " << DynamicPCArray[PC] << endl;
        binary_code = decode_hex(DynamicPCArray[PC]);		//Calls on decode_hex and returns the binary code for that line
        cout << binary_code << endl;		//Prints the binary code for that line

        //Get the total number of instructions counter[0]
        //Get the total number of arithmetic instructions counter[1]
        //Get the total number of logical instructions counter[2]
        //Get the total number of memory instructions counter[3]
        //Get the total number of control transfer instructions counter[4] 
        counter[0] = counter[0] + 1;	//Keeps track of total instructions

        if (binary_code.substr(0, 6) == "010001") {

            cout << "CC_without_fwd: " << CC_without_fwd + 5 << endl;
            cout << "stall_by_RAW_without_fwd: " << stall_by_RAW_without_fwd << endl;
            cout << "stall_by_RAW_without_fwd_counter: " << stall_by_RAW_without_fwd_counter << endl;
            cout << "stall_by_branch_without_fwd: " << stall_by_branch_without_fwd << endl;
            cout << "stall_by_branch_without_fwd_counter: " << stall_by_branch_without_fwd_counter << endl;
        }

       PC = decode_instructions(binary_code, Register, Memory, counter, register_change, memory_change, special, PC, DynamicPCArray);

       simulate_timing_without_fwd(binary_code, Register, register_tracking, CC_without_fwd, stall_by_RAW_without_fwd, stall_by_RAW_without_fwd_counter, stall_by_branch_without_fwd, stall_by_branch_without_fwd_counter);
       simulate_timing_with_fwd(binary_code, Register, register_tracking, CC_with_fwd, stall_by_RAW_with_fwd, stall_by_RAW_with_fwd_counter, stall_by_branch_with_fwd, stall_by_branch_with_fwd_counter);
       for (int i = 0; i < 5; i++) {
           cout << i << ": " << register_tracking[i] << endl;
       }
       //Without Forwarding Print Out
      
       cout << "CC_without_fwd: " << CC_without_fwd << endl;
       cout << "stall_by_RAW_without_fwd: " << stall_by_RAW_without_fwd << endl;
       cout << "stall_by_RAW_without_fwd_counter: " << stall_by_RAW_without_fwd_counter << endl;
       cout << "stall_by_branch_without_fwd: " << stall_by_branch_without_fwd << endl;
       cout << "stall_by_branch_without_fwd_counter: " << stall_by_branch_without_fwd_counter << endl;
    
       //With Forwarding Print Out
       cout << "CC_with_fwd: " << CC_with_fwd << endl;
       cout << "stall_by_RAW_with_fwd: " << stall_by_RAW_with_fwd << endl;
       cout << "stall_by_RAW_with_fwd_counter: " << stall_by_RAW_with_fwd_counter << endl;
       cout << "stall_by_branch_with_fwd: " << stall_by_branch_with_fwd << endl;
       cout << "stall_by_branch_with_fwd_counter: " << stall_by_branch_with_fwd_counter << endl;

       for (int i = 0; i < 32; i++) {
           if (register_change[i] != -999) {
               cout << "R" << register_change[i] << ": " << Register[register_change[i]] << endl;                //prints to console
               //Memory_Image << "R" << register_change[i] << ": " << Register[register_change[i]] << endl;        //prints to .txt file
           }
       }



       //Print Program Counter
       //string text = "Program counter: ";
       //cout << text << PC << endl;

    }
    print_results(counter, Register, Memory, register_change, memory_change, special, PC);
}

/*////////////////////////////////////////////////////
Prints the final results of the pipeline
*/////////////////////////////////////////////////////
void print_results(int counter[], int Register[], int Memory[],int register_change[], int memory_change[], uint8_t special, int PC) {
    ofstream Memory_Image;
    string text;        //what to write to conole and txt file

    Memory_Image.open("Memory_Image.txt");      //open .txt file

    //1st line
    text = "\n ****Instructions count summary ****";
    cout << text << endl;
    Memory_Image << text << endl;
    //second line
    text = "Total number of instructions: ";
    cout << text << counter[0] << endl;
    Memory_Image << text << counter[0] << endl;

    //3rd line
    text = "Total number of arithmetic instructions: ";
    cout << text << counter[1] << endl;
    Memory_Image << text << counter[1] << endl;
    
    //4th line
    text = "Total number of logical instructions : ";
    cout << text << counter[2] << endl;
    Memory_Image << text << counter[2] << endl;

    //5th line
    text = "Total number of memory instructions: ";
    cout << text << counter[3] << endl;
    Memory_Image << text << counter[3] << endl;

    //6th line
    text = "Total number of control transfer instructions: ";
    cout << text << counter[4] << endl;
    Memory_Image << text << counter[4] << endl;

    //Print Final Register Contents
    text = "\nFinal Register State:";
    cout << text << endl;
    Memory_Image << text << endl;

    //Print Program Counter
    text = "Program counter: ";
    cout << text << (PC+1)*4 << endl;
    Memory_Image << text << (PC+1)*4 << endl;
    
    for (int i = 0; i < 32; i++) {
        if (register_change[i] != -999){
            cout << "R" << register_change[i] << ": " << Register[register_change[i]] << endl;                //prints to console
            Memory_Image << "R" << register_change[i] << ": " << Register[register_change[i]] << endl;        //prints to .txt file
        }
    }

    text = "\nFinal Memory State: ";
    cout << text << endl;
    Memory_Image << text << endl;
    for (int i = 0; i < 32768; i++) {
        if (memory_change[i] != -999) {
            cout << "Address: " << i << " ,Contents: " << Memory[i] << endl;
            Memory_Image << "Address: " << i << " ,Contents: " << Memory[i] << endl;
        }
    }
    //Print Timing Simulator
    text = "\nTiming Simulator:";
    cout << text << endl;
    Memory_Image << text << endl;

    text = "Without Forwarding:";
    cout << text << endl;
    Memory_Image << text << endl;
    //Print stalls
    text = "Total stalls: ";
    cout << text << endl;
    Memory_Image << text << endl;
    //Print clock cycles
    text = "Total number of clock cycles: ";
    cout << text << endl;
    Memory_Image << text << endl;

    text = "With Forwarding:";
    cout << text << endl;
    Memory_Image << text << endl;
    //Print stalls
    text = "Total stalls: ";
    cout << text << endl;
    Memory_Image << text << endl;
    //Print clock cycles
    text = "Total number of clock cycles: ";
    cout << text << endl;
    Memory_Image << text << endl;

    //If program is halted
    if (special == 1){
        text = "Program was Halted";
        cout << text << endl;
        Memory_Image << text << endl;
    }
    exit(0);

}
/*////////////////////////////////////////////////////
//Get user input file and check if the file is valid
*/////////////////////////////////////////////////////


string get_input_file() {
    string input_file;


    cout << "Please enter a valid test file:\n";
    //cin >> input_file;
    input_file = "\\thoth.cecs.pdx.edu\\Home04\\mgodbole\\Downloads\\sample_memory_image.txt";
   
    cout << "Input file: " << input_file << endl;     //Write out the input file


    bool file_flag = true;		//Set a flag
	
	//Check if file has valid data, if valid set file_flag to false and ask for valid file location
    while (file_flag) {
        ifstream f(input_file.c_str());		//Read from this file
        if (!(f.good())) {
            cout << "Test file is not valid. Please enter a valid test file:\n";
            cin >> input_file;
            cout << "Input file: " << input_file << endl;
        }
        else {
            file_flag = false;
        }
    }
    return input_file;		//returns input file to main
}

/*//////////////////////////////////////////////
Function that Converts hex to binary
*///////////////////////////////////////////////
string decode_hex(string hexa_address) {

    long int i = 0;
    string address = "";
    string temp_address = "";

    while (hexa_address[i]) {

        switch (hexa_address[i]) {

        case '0':

            temp_address = "0000";
            break;

        case '1':
            temp_address = "0001";
            break;

        case '2':
            temp_address = "0010";
            break;

        case '3':
            temp_address = "0011";
            break;

        case '4':
            temp_address = "0100";
            break;

        case '5':
            temp_address = "0101";
            break;

        case '6':
            temp_address = "0110";
            break;

        case '7':
            temp_address = "0111";
            break;

        case '8':
            temp_address = "1000";
            break;

        case '9':
            temp_address = "1001";
            break;

        case 'A':
            temp_address = "1010";
            break;

        case 'B':
            temp_address = "1011";
            break;

        case 'C':
            temp_address = "1100";
            break;

        case 'D':
            temp_address = "1101";
            break;

        case 'E':
            temp_address = "1110";
            break;

        case 'F':
            temp_address = "1111";
            break;

        default:
            cout << "\nInvalid hexadecimal digit. Hex digit must be number of capital letter: A, B, C, D, E, F: "
                << hexa_address[i] << endl;
        }
        address = address.append(temp_address);		//appends binary values to address
        // cout << address << endl;
        i++;
    }
    return address;
}

/*////////////////////////////////////////////////////
Instruction decoder stage
*/////////////////////////////////////////////////////
int decode_instructions(string binary_address, int Register[], int Memory[], int counter[],int register_change[], int memory_change[], uint8_t special, int PC, string DynamicPCArray[]) {
    instructions_exe function;		//instruction object
    string result;
    bool if_imm_flag = false;
	
	//Checking if instruction is valid before proceeding
    if (binary_address != "00000000000000000000000000000000") {
        string opcode = binary_address.substr(0, 6);
		//Checking if an arithmetic instruction
        if (opcode == "000000") {
            cout << "call ADD function..." << endl;
            counter[1] = counter[1] + 1;    //counts instructions of type arithmetic
            function.addition_function(binary_address, Register, Memory);
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        else if (opcode == "000010") {

            cout << "call SUB function..." << endl;
            counter[1] = counter[1] + 1;
            function.subtraction_function(binary_address, Register, Memory);
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        else if (opcode == "000100") {

            cout << "call MUL function..." << endl;
            counter[1] = counter[1] + 1;
            function.multiplication_function(binary_address, Register, Memory);
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        else if (opcode == "000001") {

            cout << "call ADDI function..." << endl;
            counter[1] = counter[1] + 1;    //counts instructions of type arithmetic
            function.addition_immediate_function(binary_address, Register, Memory);

            if_imm_flag = true;
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        else if (opcode == "000011") {

            cout << "call SUBI function..." << endl;
            counter[1] = counter[1] + 1;    //counts instructions of type arithmetic
            function.subtract_immediate_function(binary_address, Register, Memory);

            if_imm_flag = true;
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        else if (opcode == "000101") {

            cout << "call MULI function..." << endl;
            counter[1] = counter[1] + 1;    //counts instructions of type arithmetic
            function.multiply_immediate_function(binary_address, Register, Memory);

            if_imm_flag = true;
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        //Logical Operations
        else if (opcode == "000110") {
            //OR instruction
            cout << "call OR function..." << endl;
            counter[2] = counter[2] + 1;    //counts instructions of type arithmetic
            function.OR_function(binary_address, Register, Memory);

            if_imm_flag = true;
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        else if (opcode == "000111") {
            //ORI instruction
            cout << "call ORI Immediate function..." << endl;
            counter[2] = counter[2] + 1;    //counts instructions of type arithmetic
            function.OR_immediate_function(binary_address, Register, Memory);

            if_imm_flag = true;
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        else if (opcode == "001000") {
            //AND instruction
            cout << "call AND function..." << endl;
            counter[2] = counter[2] + 1;    //counts instructions of type arithmetic
            function.AND_function (binary_address, Register, Memory);

            if_imm_flag = true;
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        else if (opcode == "001001") {
            //AND Immediate instruction
            cout << "call ANDI Immediate function..." << endl;
            counter[2] = counter[2] + 1;    //counts instructions of type arithmetic
            function.AND_immediate_function (binary_address, Register, Memory);

            if_imm_flag = true;
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        else if (opcode == "001010") {
            //XOR instruction
            cout << "call XOR function..." << endl;
            counter[2] = counter[2] + 1;    //counts instructions of type arithmetic
            function.XOR_function (binary_address, Register, Memory);

            if_imm_flag = true;
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        else if (opcode == "001011") {
            //XOR Immediate instruction
            cout << "call XORI Immediate function..." << endl;
            counter[2] = counter[2] + 1;    //counts instructions of type arithmetic
            function.XOR_imediate_function (binary_address, Register, Memory);

            if_imm_flag = true;
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        //Memory Operations
        else if (opcode == "001100") {
            //LDW instruction
            cout << "call LDW function..." << endl;
            counter[3] = counter[3] + 1;    //counts instructions of type arithmetic
            function.loadWord (binary_address, Register, Memory, DynamicPCArray);

            if_imm_flag = true;
            update_register_changes(binary_address, register_change, if_imm_flag);
        }
        else if (opcode == "001101") {
            //STW instruction
            cout << "call STW function..." << endl;
            counter[3] = counter[3] + 1;    //counts instructions of type arithmetic
            function.storeWord (binary_address, Register, Memory);
            
            update_memory_changes(binary_address, Register, memory_change);
        }
        //Control Operations
        else if (opcode == "001110") {
            //BZ instruction
            cout << "Call BZ function..." << endl;
            PC = function.branchIfZero (binary_address, Register, PC);
            counter[4] = counter[4] + 1;    //counts instructions of type arithmetic         
        }
        else if (opcode == "001111") {
            //BEQ instruction
            cout << "Call BEQ function..." << endl;
            PC = function.branchIfRegistersEqual (binary_address, Register, PC);
            counter[4] = counter[4] + 1;    //counts instructions of type control
        }
        else if (opcode == "010000") {
            //JR instruction
            cout << "Call JR function..." << endl;
            PC = function.jumpToRegister(binary_address, Register, PC);
            counter[4] = counter[4] + 1;    //counts instructions of type Control
        }       
        else if (opcode == "010001") {
            //Halt instruction
            cout << "Halting Program..." << endl;
            counter[4] = counter[4] + 1;    //counts instructions of type Control
            special = 1;
            print_results(counter, Register, Memory, register_change, memory_change, special, PC);
        }
    }
    else {
        cout << "0x00000000 input... skip!" << endl;
    }
    return PC;
}

/*//////////////////////////////////////////////////////////////////////////////////
Keeps track of changes in register values
*//////////////////////////////////////////////////////////////////////////////////
void update_register_changes(string binary_address, int register_change[], bool if_imm) {

    bool if_exist_flag = false;
    int r_reg;

    //Get destination_reg register
    if (if_imm == false) {
        r_reg = stoi(binary_address.substr(16, 5), nullptr, 2);
        
    }

    //Get target_reg register
    else {
        r_reg = stoi(binary_address.substr(11, 5), nullptr, 2);
    }

    for (int i = 0; i < 32; i++) {
        //cout << register_change[i] << endl;
        if (r_reg == register_change[i]) {
            if_exist_flag = true;
        }
        if (register_change[i] == -999) {
            break;
        }
    }

    if (if_exist_flag == false) {
        for (int i = 0; i < 32; i++) {
            //cout << "r: " << register_change[i] << endl;
            if (register_change[i] == -999) {
                register_change[i] = r_reg;
                //cout << "register change: " << r_reg << endl;
                break;
            }
        }
    }
}


/*//////////////////////////////////////////////////////////////////////////////////
Keep track of the change in memory values 
*//////////////////////////////////////////////////////////////////////////////////
void update_memory_changes(string binary_address, int Register[], int memory_change[]) {

    int m_mem, rreg, immValue;
    bool if_exist_flag = false;


    rreg = stoi(binary_address.substr(6, 5), nullptr, 2);        //get source register
    immValue = stoi(binary_address.substr(16, 16), nullptr, 2);       //Get immediate value for adding to the source register contents
    

    m_mem = Register[rreg] + immValue;       //Memory address = Target Register contents

    for (int i = 0; i < 32768; i++) {
        if (m_mem == memory_change[i]) {
            if_exist_flag = true;
        }
        if (memory_change[i] == -999) {
            break;
        }
    }

    if (if_exist_flag == false) {
        for (int i = 0; i < 32768; i++) {
            if (memory_change[m_mem] == -999) {
                memory_change[m_mem] = m_mem;
                cout << m_mem << endl;
                break;
            }
        }
    }
}

// Keep track of number of cycles without using forwarding
void simulate_timing_without_fwd(string binary_address, int Register[], int register_tracking[], int &CC_without_fwd, int &stall_by_RAW_without_fwd, int &stall_by_RAW_without_fwd_counter, int &stall_by_branch_without_fwd, int &stall_by_branch_without_fwd_counter) {
    instructions_exe function;		//instruction object
    string result;
    bool stall_by_2 = false;
    bool stall_by_1 = false;
    int latest_data = 4;

    //Checking if instruction is valid before proceeding
    if (binary_address != "00000000000000000000000000000000") {
        string opcode = binary_address.substr(0, 6);

        for (int i = 0; i < 5; i++) {
            if (register_tracking[i] == -999) {
                latest_data = i - 1;
                break;
            }
        }

        //Checking if an instruction is ADD; SUB; MUL; OR; AND; and XOR
        if (opcode == "000000" || opcode == "000010" || opcode == "000100" || opcode == "000110" || opcode == "001000" || opcode == "001010") {

            string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
            string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
            string destination_reg_bin = binary_address.substr(16, 5);		//Gets destination_reg binary string from binary instruction

            //Convert the binary value to dec
            int source_reg = stoi(source_reg_bin, nullptr, 2);      //takes binary string and turns to an integer
            int target_reg = stoi(target_reg_bin, nullptr, 2);      //takes binary string and turns to an integer
            int destination_reg = stoi(destination_reg_bin, nullptr, 2);      //takes binary string and turns to an integer


            if (latest_data == -1) {
                CC_without_fwd = CC_without_fwd + 1; // 1 per cycle
                track_register_changes(register_tracking, destination_reg, stall_by_2, stall_by_1);
            }

            else if (latest_data != 0) {
                if (target_reg == register_tracking[latest_data] || source_reg == register_tracking[latest_data]) {
                    CC_without_fwd = CC_without_fwd + 3; //1 per cycle and 2 stalls
                    stall_by_RAW_without_fwd = stall_by_RAW_without_fwd + 2;
                    stall_by_RAW_without_fwd_counter = stall_by_RAW_without_fwd_counter + 1;
                    stall_by_2 = true;
                }
                else if (target_reg == register_tracking[latest_data - 1] || source_reg == register_tracking[latest_data - 1]) {
                    CC_without_fwd = CC_without_fwd + 2; // 1 per cycle and 1 stalls
                    stall_by_RAW_without_fwd = stall_by_RAW_without_fwd + 1;
                    stall_by_RAW_without_fwd_counter = stall_by_RAW_without_fwd_counter + 1;
                    stall_by_1 = true;
                }
                else {
                    CC_without_fwd = CC_without_fwd + 1; // 1 per cycle
                    cout << CC_without_fwd << endl;
                }
                //perform register tracking
                track_register_changes(register_tracking, destination_reg, stall_by_2, stall_by_1);

            }

            else {
                if (target_reg == register_tracking[latest_data] || source_reg == register_tracking[latest_data]) {
                    CC_without_fwd = CC_without_fwd + 3; //1 per cycle and 2 stalls
                    stall_by_RAW_without_fwd = stall_by_RAW_without_fwd + 2;
                    stall_by_RAW_without_fwd_counter = stall_by_RAW_without_fwd_counter + 1;
                    stall_by_2 = true;
                }
                else {
                    CC_without_fwd = CC_without_fwd + 1; // 1 per cycle
                    cout << CC_without_fwd << endl;
                }
                //perform register tracking
                track_register_changes(register_tracking, destination_reg, stall_by_2, stall_by_1);
            }
        }

        //Checking if an instruction is ADDI; SUBI; MULI; ORI; ANDI; XORI; LDW; and STW
        else if (opcode == "000001" || opcode == "000011" || opcode == "000101" || opcode == "000111" || opcode == "001001" || opcode == "001011" || opcode == "001100" || opcode == "001101") {

            string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
            string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction

            int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
            int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

            if (latest_data == -1) {
                CC_without_fwd = CC_without_fwd + 1; // 1 per cycle
                track_register_changes(register_tracking, target_reg, stall_by_2, stall_by_1);
            }

            else if (latest_data != 0) {
                if (source_reg == register_tracking[latest_data]) {
                    CC_without_fwd = CC_without_fwd + 3; //1 per cycle and 2 stalls
                    stall_by_RAW_without_fwd = stall_by_RAW_without_fwd + 2;
                    stall_by_RAW_without_fwd_counter = stall_by_RAW_without_fwd_counter + 1;
                    stall_by_2 = true;
                }
                else if (source_reg == register_tracking[latest_data - 1]) {
                    CC_without_fwd = CC_without_fwd + 2; // 1 per cycle and 1 stalls
                    stall_by_RAW_without_fwd = stall_by_RAW_without_fwd + 1;
                    stall_by_RAW_without_fwd_counter = stall_by_RAW_without_fwd_counter + 1;
                    stall_by_1 = true;
                }
                else {
                    CC_without_fwd = CC_without_fwd + 1; // 1 per cycle
                }
                //perform register tracking
                track_register_changes(register_tracking, target_reg, stall_by_2, stall_by_1);

            }

            else {
                if (source_reg == register_tracking[latest_data]) {
                    CC_without_fwd = CC_without_fwd + 3; //1 per cycle and 2 stalls
                    stall_by_RAW_without_fwd = stall_by_RAW_without_fwd + 2;
                    stall_by_RAW_without_fwd_counter = stall_by_RAW_without_fwd_counter + 1;
                    stall_by_2 = true;
                }
                else {
                    CC_without_fwd = CC_without_fwd + 1; // 1 per cycle
                }
                //perform register tracking
                track_register_changes(register_tracking, target_reg, stall_by_2, stall_by_1);
            }
        }

        //Checking if an instruction is branch instructions: BZ
        else if (opcode == "001110") {

            string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
            int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

            if (source_reg == register_tracking[latest_data]) {
                CC_without_fwd = CC_without_fwd + 2;
                stall_by_RAW_without_fwd = stall_by_RAW_without_fwd + 2;
                stall_by_RAW_without_fwd_counter = stall_by_RAW_without_fwd_counter + 1;

            }

            else if (source_reg == register_tracking[latest_data - 1]) {
                CC_without_fwd = CC_without_fwd + 1;
                stall_by_RAW_without_fwd = stall_by_RAW_without_fwd + 1;
                stall_by_RAW_without_fwd_counter = stall_by_RAW_without_fwd_counter + 1;

            }

            if (Register[source_reg] == 0) {
                CC_without_fwd = CC_without_fwd + 3;
                stall_by_branch_without_fwd = stall_by_branch_without_fwd + 2;
                stall_by_branch_without_fwd_counter = stall_by_branch_without_fwd_counter + 1;
                stall_by_2 = true;
            }
            else {
                CC_without_fwd = CC_without_fwd + 1; // 1 per cycle
            }

            track_register_changes(register_tracking, -1, stall_by_2, stall_by_1);
            if (register_tracking[0] == -999) {
                for (int i = 0; i < 4; i++) {
                    register_tracking[i] = register_tracking[i + 1];
                }
            }
        }

        //Call BEQ instruction
        else if (opcode == "001111") {

            string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
            string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction

            int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
            int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

            if (source_reg == register_tracking[latest_data] || target_reg == register_tracking[latest_data]) {
                CC_without_fwd = CC_without_fwd + 2;
                stall_by_RAW_without_fwd = stall_by_RAW_without_fwd + 2;
                stall_by_RAW_without_fwd_counter = stall_by_RAW_without_fwd_counter + 1;

            }

            else if (source_reg == register_tracking[latest_data - 1] || target_reg == register_tracking[latest_data - 1]) {
                CC_without_fwd = CC_without_fwd + 1;
                stall_by_RAW_without_fwd = stall_by_RAW_without_fwd + 1;
                stall_by_RAW_without_fwd_counter = stall_by_RAW_without_fwd_counter + 1;

            }


            if (Register[source_reg] == Register[target_reg]) {
                CC_without_fwd = CC_without_fwd + 3;
                stall_by_branch_without_fwd = stall_by_branch_without_fwd + 2;
                stall_by_branch_without_fwd_counter = stall_by_branch_without_fwd_counter + 1;
                stall_by_2 = true;
            }
            else {
                CC_without_fwd = CC_without_fwd + 1; // 1 per cycle
            }

            track_register_changes(register_tracking, -1, stall_by_2, stall_by_1);
            if (register_tracking[0] == -999) {
                for (int i = 0; i < 4; i++) {
                    register_tracking[i] = register_tracking[i + 1];
                }
            }
        }

        //Call JR instruction
        else if (opcode == "010000") {
            string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
            int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

            CC_without_fwd = CC_without_fwd + 3;
            stall_by_branch_without_fwd = stall_by_branch_without_fwd + 2;
            stall_by_branch_without_fwd_counter = stall_by_branch_without_fwd_counter + 1;
            stall_by_2 = true;
            
            track_register_changes(register_tracking, -1, stall_by_2, stall_by_1);

            if (register_tracking[0] == -999) {
                for (int i = 0; i < 4; i++) {
                    register_tracking[i] = register_tracking[i + 1];
                }
            }
        }

        else if (opcode == "010001") {
            //Halt instruction
            cout << "Halting Program..." << endl;
            CC_without_fwd = CC_without_fwd + 4;
        }
    }
    else {
        cout << "0x00000000 input... skip!" << endl;
    }
}

// Perform register tracking task
void track_register_changes(int register_tracking[], int r_des, bool stall_by_2, bool stall_by_1) {

    bool register_tracking_not_full = false;


    if (stall_by_2 == true) {

        for (int i = 0; i < 5; i++) {
            if (register_tracking[i] == -999) {
                register_tracking_not_full = true;

                if (i == 1 || i == 2) {
                    register_tracking[i] = -1;
                    register_tracking[i + 1] = -1;
                    register_tracking[i + 2] = r_des;
                    break;
                }
                else if (i == 3) {
                    register_tracking[0] = register_tracking[1];
                    register_tracking[1] = register_tracking[2];
                    register_tracking[2] = -1;
                    register_tracking[3] = -1;
                    register_tracking[4] = r_des;
                    break;
                }
                else if (i == 4) {
                    register_tracking[0] = register_tracking[2];
                    register_tracking[1] = register_tracking[3];
                    register_tracking[2] = -1;
                    register_tracking[3] = -1;
                    register_tracking[4] = r_des;
                    break;
                }
            }
        }

        if (register_tracking_not_full == false) {
            for (int i = 0; i < 2; i++) {
                register_tracking[i] = register_tracking[i + 3];
            }
            register_tracking[2] = -1;
            register_tracking[3] = -1;
            register_tracking[4] = r_des;
        }
    }

    else if (stall_by_1 == true) {
        for (int i = 0; i < 5; i++) {
            if (register_tracking[i] == -999) {
                register_tracking_not_full = true;

                if (i == 1 || i == 2 || i == 3) {
                    register_tracking[i] = -1;
                    register_tracking[i + 1] = r_des;
                    break;
                }
                else if (i == 4) {
                    register_tracking[0] = register_tracking[1];
                    register_tracking[1] = register_tracking[2];
                    register_tracking[2] = register_tracking[3];
                    register_tracking[3] = -1;
                    register_tracking[4] = r_des;
                    break;
                }
            }
        }
        if (register_tracking_not_full == false) {
            for (int i = 0; i < 3; i++) {
                register_tracking[i] = register_tracking[i + 2];
            }
            register_tracking[3] = -1;
            register_tracking[4] = r_des;
            
        }
    }

    else {

        for (int i = 0; i < 5; i++) {
            if (register_tracking[i] == -999) {
                register_tracking[i] = r_des;
                register_tracking_not_full = true;
                break;
            }
        }

        if (register_tracking_not_full == false) {
            for (int i = 0; i < 4; i++) {
                register_tracking[i] = register_tracking[i + 1];
            }

            register_tracking[4] = r_des;
        }
    }

}


// Keep track of number of cycles without using forwarding
void simulate_timing_with_fwd(string binary_address, int Register[], int register_tracking[], int& CC_with_fwd, int& stall_by_RAW_with_fwd, int& stall_by_RAW_with_fwd_counter, int& stall_by_branch_with_fwd, int& stall_by_branch_with_fwd_counter) {
    instructions_exe function;		//instruction object
    string result;
    bool stall_by_2 = false;
    bool stall_by_1 = false;
    int latest_data = 4;

    //Checking if instruction is valid before proceeding
    if (binary_address != "00000000000000000000000000000000") {
        string opcode = binary_address.substr(0, 6);

        for (int i = 0; i < 5; i++) {
            if (register_tracking[i] == -999) {
                latest_data = i - 1;
                break;
            }
        }

        //Checking if an instruction is ADD; SUB; MUL; OR; AND; and XOR
        if (opcode == "000000" || opcode == "000010" || opcode == "000100" || opcode == "000110" || opcode == "001000" || opcode == "001010") {

            string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
            string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction
            string destination_reg_bin = binary_address.substr(16, 5);		//Gets destination_reg binary string from binary instruction

            //Convert the binary value to dec
            int source_reg = stoi(source_reg_bin, nullptr, 2);      //takes binary string and turns to an integer
            int target_reg = stoi(target_reg_bin, nullptr, 2);      //takes binary string and turns to an integer
            int destination_reg = stoi(destination_reg_bin, nullptr, 2);      //takes binary string and turns to an integer


            if (latest_data == -1) {
                CC_with_fwd = CC_with_fwd + 1; // 1 per cycle
                track_register_changes(register_tracking, destination_reg, stall_by_2, stall_by_1);
            }

            else if (latest_data != 0) {
                if (target_reg == register_tracking[latest_data] || source_reg == register_tracking[latest_data]) {
                    CC_with_fwd = CC_with_fwd + 3; //1 per cycle and 2 stalls
                    stall_by_RAW_with_fwd = stall_by_RAW_with_fwd + 2;
                    stall_by_RAW_with_fwd_counter = stall_by_RAW_with_fwd_counter + 1;
                    stall_by_2 = true;
                }
                else if (target_reg == register_tracking[latest_data - 1] || source_reg == register_tracking[latest_data - 1]) {
                    CC_with_fwd = CC_with_fwd + 2; // 1 per cycle and 1 stalls
                    stall_by_RAW_with_fwd = stall_by_RAW_with_fwd + 1;
                    stall_by_RAW_with_fwd_counter = stall_by_RAW_with_fwd_counter + 1;
                    stall_by_1 = true;
                }
                else {
                    CC_with_fwd = CC_with_fwd + 1; // 1 per cycle
                    cout << CC_with_fwd << endl;
                }
                //perform register tracking
                track_register_changes(register_tracking, destination_reg, stall_by_2, stall_by_1);

            }

            else {
                if (target_reg == register_tracking[latest_data] || source_reg == register_tracking[latest_data]) {
                    CC_with_fwd = CC_with_fwd + 3; //1 per cycle and 2 stalls
                    stall_by_RAW_with_fwd = stall_by_RAW_with_fwd + 2;
                    stall_by_RAW_with_fwd_counter = stall_by_RAW_with_fwd_counter + 1;
                    stall_by_2 = true;
                }
                else {
                    CC_with_fwd = CC_with_fwd + 1; // 1 per cycle
                    cout << CC_with_fwd << endl;
                }
                //perform register tracking
                track_register_changes(register_tracking, destination_reg, stall_by_2, stall_by_1);
            }
        }

        //Checking if an instruction is ADDI; SUBI; MULI; ORI; ANDI; XORI; LDW; and STW
        else if (opcode == "000001" || opcode == "000011" || opcode == "000101" || opcode == "000111" || opcode == "001001" || opcode == "001011" || opcode == "001100" || opcode == "001101") {

            string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
            string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction

            int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
            int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

            if (latest_data == -1) {
                CC_with_fwd = CC_with_fwd + 1; // 1 per cycle
                track_register_changes(register_tracking, target_reg, stall_by_2, stall_by_1);
            }

            else if (latest_data != 0) {
                if (source_reg == register_tracking[latest_data]) {
                    CC_with_fwd = CC_with_fwd + 3; //1 per cycle and 2 stalls
                    stall_by_RAW_with_fwd = stall_by_RAW_with_fwd + 2;
                    stall_by_RAW_with_fwd_counter = stall_by_RAW_with_fwd_counter + 1;
                    stall_by_2 = true;
                }
                else if (source_reg == register_tracking[latest_data - 1]) {
                    CC_with_fwd = CC_with_fwd + 2; // 1 per cycle and 1 stalls
                    stall_by_RAW_with_fwd = stall_by_RAW_with_fwd + 1;
                    stall_by_RAW_with_fwd_counter = stall_by_RAW_with_fwd_counter + 1;
                    stall_by_1 = true;
                }
                else {
                    CC_with_fwd = CC_with_fwd + 1; // 1 per cycle
                }
                //perform register tracking
                track_register_changes(register_tracking, target_reg, stall_by_2, stall_by_1);

            }

            else {
                if (source_reg == register_tracking[latest_data]) {
                    CC_with_fwd = CC_with_fwd + 3; //1 per cycle and 2 stalls
                    stall_by_RAW_with_fwd = stall_by_RAW_with_fwd + 2;
                    stall_by_RAW_with_fwd_counter = stall_by_RAW_with_fwd_counter + 1;
                    stall_by_2 = true;
                }
                else {
                    CC_with_fwd = CC_with_fwd + 1; // 1 per cycle
                }
                //perform register tracking
                track_register_changes(register_tracking, target_reg, stall_by_2, stall_by_1);
            }
        }

        //Checking if an instruction is branch instructions: BZ
        else if (opcode == "001110") {

            string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
            int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

            if (source_reg == register_tracking[latest_data]) {
                CC_with_fwd = CC_with_fwd + 2;
                stall_by_RAW_with_fwd = stall_by_RAW_with_fwd + 2;
                stall_by_RAW_with_fwd_counter = stall_by_RAW_with_fwd_counter + 1;

            }

            else if (source_reg == register_tracking[latest_data - 1]) {
                CC_with_fwd = CC_with_fwd + 1;
                stall_by_RAW_with_fwd = stall_by_RAW_with_fwd + 1;
                stall_by_RAW_with_fwd_counter = stall_by_RAW_with_fwd_counter + 1;

            }

            if (Register[source_reg] == 0) {
                CC_with_fwd = CC_with_fwd + 3;
                stall_by_branch_with_fwd = stall_by_branch_with_fwd + 2;
                stall_by_branch_with_fwd_counter = stall_by_branch_with_fwd_counter + 1;
                stall_by_2 = true;
            }
            else {
                CC_with_fwd = CC_with_fwd + 1; // 1 per cycle
            }

            track_register_changes(register_tracking, -1, stall_by_2, stall_by_1);
            if (register_tracking[0] == -999) {
                for (int i = 0; i < 4; i++) {
                    register_tracking[i] = register_tracking[i + 1];
                }
            }
        }

        //Call BEQ instruction
        else if (opcode == "001111") {

            string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
            string target_reg_bin = binary_address.substr(11, 5);		//Gets target_reg binary string from binary instruction

            int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer
            int target_reg = stoi(target_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

            if (source_reg == register_tracking[latest_data] || target_reg == register_tracking[latest_data]) {
                CC_with_fwd = CC_with_fwd + 2;
                stall_by_RAW_with_fwd = stall_by_RAW_with_fwd + 2;
                stall_by_RAW_with_fwd_counter = stall_by_RAW_with_fwd_counter + 1;

            }

            else if (source_reg == register_tracking[latest_data - 1] || target_reg == register_tracking[latest_data - 1]) {
                CC_with_fwd = CC_with_fwd + 1;
                stall_by_RAW_with_fwd = stall_by_RAW_with_fwd + 1;
                stall_by_RAW_with_fwd_counter = stall_by_RAW_with_fwd_counter + 1;

            }


            if (Register[source_reg] == Register[target_reg]) {
                CC_with_fwd = CC_with_fwd + 3;
                stall_by_branch_with_fwd = stall_by_branch_with_fwd + 2;
                stall_by_branch_with_fwd_counter = stall_by_branch_with_fwd_counter + 1;
                stall_by_2 = true;
            }
            else {
                CC_with_fwd = CC_with_fwd + 1; // 1 per cycle
            }

            track_register_changes(register_tracking, -1, stall_by_2, stall_by_1);
            if (register_tracking[0] == -999) {
                for (int i = 0; i < 4; i++) {
                    register_tracking[i] = register_tracking[i + 1];
                }
            }
        }

        //Call JR instruction
        else if (opcode == "010000") {
            string source_reg_bin = binary_address.substr(6, 5);		//Gets source_reg binary string from binary instruction
            int source_reg = stoi(source_reg_bin, nullptr, 2);          //takes binary string and turns to an integer

            CC_with_fwd = CC_with_fwd + 3;
            stall_by_branch_with_fwd = stall_by_branch_with_fwd + 2;
            stall_by_branch_with_fwd_counter = stall_by_branch_with_fwd_counter + 1;
            stall_by_2 = true;

            track_register_changes(register_tracking, -1, stall_by_2, stall_by_1);

            if (register_tracking[0] == -999) {
                for (int i = 0; i < 4; i++) {
                    register_tracking[i] = register_tracking[i + 1];
                }
            }
        }

        else if (opcode == "010001") {
            //Halt instruction
            cout << "Halting Program..." << endl;
            CC_with_fwd = CC_with_fwd + 4;
        }
    }
    else {
        cout << "0x00000000 input... skip!" << endl;
    }
}