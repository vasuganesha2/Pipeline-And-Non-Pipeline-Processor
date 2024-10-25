
#include<iostream>
#include<bits/stdc++.h>
using namespace std;

//Global Variables

    int timer = 0;
map<string, string> opcodeMap={
    {"000100", "beq"},
    {"000010", "j"},
    {"100011", "lw"},
    {"101011", "sw"},
    {"000000","R"},
    {"001000","Addi"},
    {"001001","Addiu"},
    {"001111","Li"}
};

map<string,string> Instruction_Map={
    {"10000","Add"},
    {"10010","Subtract"},
    {"00010","Mult"},   
    {"01101","Ori"},
    {"00110","xor"}
};

string read1,read2,write;

//register set for MIPS
//vector<string> regs = {"$sero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5",
//"$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};

map<string, string> registerFile;

string write_register;

int binaryStringToDecimal(string& binaryString) {
    int decimalResult = 0;

    for (char digit : binaryString) {
        decimalResult = decimalResult * 2 + (digit - '0');
    }

    return decimalResult;
}

void initializeRegisterFile() {
    for (int i = 0; i < 32; ++i) {
        string binaryAddress = bitset<5>(i).to_string();
        //cout<<binaryAddress<<endl;                           //Initially All values of register is write to 0
        registerFile[binaryAddress] = bitset<32>(0).to_string();
    }
}

std::bitset<32> SubtractBitsets(const std::bitset<32>& a, const std::bitset<32>& b) {
    std::bitset<32> result;
    bool borrow = 0;

    for (int i = 0; i < 32; i++) {
        bool bitA = a[i];
        bool bitB = b[i];

        // Calculate the difference of bits A, B, and the borrow
        bool diff = (bitA ^ bitB) ^ borrow;

        // Update the result
        result[i] = diff;

        // Calculate the borrow for the next bit
        borrow = (!bitA & bitB) | (!diff & (bitA ^ bitB) & borrow);
    }

    return result;
}

bitset<32> Convert_to_Binary(string& s) {
    bitset<32> result;

    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '1') {
            result.set(31 - i); 
        }
    }

    return result;
}

bitset<32> AddBitsets(bitset<32>& a, bitset<32>& b) {

    // bool carry = 0;

    // for (int i = 0; i < 32; i++) {
    //     bool bitA = a[i];
    //     bool bitB = b[i];

    //     bool sum = bitA ^ bitB ^ carry;

    //     result[i] = sum;

    //     carry = (bitA & bitB) | (bitA & carry) | (bitB & carry);
    // }
    int x = 0, y = 0;

    for(int i = 0; i < 32; i ++)    x += (1 << i) * a[i], y += (1 << i) * b[i];
    bitset<32> result(x+y);

    return result;
}

int N(bitset<32>a){
    int n=0;
    for(int i=0;i<32;i++) n+=a[i]*(1<<i);
    return n;
}
bitset<32> MultiplyBitsets(bitset<32>& a, bitset<32>& b) {
    
    int x = 0, y = 0;
    for(int i = 0; i < 32; i ++)
        x += (1 << i) * a[i], y += (1 << i) * b[i];

    bitset<32> result(x * y);
    return result;
}



bitset<32> ORBitsets(bitset<32>& a, bitset<32>& b) {
    std::bitset<32> result;

    for (int i = 0; i < 32; i++) {
        result[i] = a[i] | b[i];
    }

    return result;
}


bool IsZero(bitset<32>& bits) {
    return bits.none(); 
}

bitset<32> xorBitsets(bitset<32> bitset1,bitset<32> bitset2) {
    bitset<32> result(0);
    for(int i = 0; i < 32; i ++)    result[i] = bitset1[i] ^ bitset2[i];
    return result;
}


//Code start from here.



class Instruction
{
private:
    bitset<32> address;
    string instruction;

public:

    Instruction(string instruction, const bitset<32>& currentAddress)
    {
        this->instruction = instruction;
        this->address = currentAddress;
    }

    bitset<32> getAddress()
    {
        return address;
    }

    string getInstruction()
    {
        return instruction;
    }
};


//Instruction Memory

class Instruction_memory
{
private:
    vector<Instruction> Instructions;
    bitset<32> currentAddress; // Add this member to track the current address

public:
    Instruction_memory()
    {
        currentAddress = bitset<32>(0); // Initialize the address to 0
    }

    void addInstruction(string& instruction)
    {
        Instructions.push_back(Instruction(instruction, currentAddress));
        currentAddress = currentAddress.to_ulong() + 4; // Increment the address by 4
    }

    Instruction* Get_Instruction(string address)
    {
        for(auto &it:Instructions){
            if(it.getAddress().to_string() == address){
                return &it;
            }
        }
        return nullptr;
    }

    vector<Instruction> getInstructions() const
    {
        return Instructions;
    }
};



//Control Instruction

class ControlInstruction {
private:
    string opcode;

public:
    ControlInstruction(string opcode){
        this->opcode=opcode;
    }

    string decodeAndGenerateControlSignals(){
          bool RegDst, Branch, MemtoReg, ALUOp, MemWrite, ALUSrc, RegWrite,jump,MemRead;
          if(opcodeMap[opcode]=="j"){
            RegDst = false;
            Branch = false;         
            MemtoReg = true;
            ALUOp = false;
            MemWrite = false;
            ALUSrc = true;
            RegWrite = false;
            jump=true;
            MemRead=false;
          }
          else if(opcodeMap[opcode]=="lw"){
            RegDst = 1;
            Branch = false;
            MemtoReg = true;
            ALUOp = false;
            MemWrite = false;
            ALUSrc = true;
            RegWrite = true;
            jump=false;
            MemRead=1;
          }
          else if(opcodeMap[opcode]=="sw"){
                RegDst = 1;
                Branch = false;
                MemtoReg = false; // Assuming sw doesn't write to the register
                ALUOp = false;
                MemWrite = true;
                ALUSrc = true;
                RegWrite = false;
                jump=false;
                MemRead=false;
          }
          else if(opcodeMap[opcode]=="beq"){
                RegDst = 1;
                Branch = true;
                MemtoReg = false;
                ALUOp = false; // Set as needed for the specific ALU operation
                MemWrite = false;
                ALUSrc = 1;
                RegWrite = false;
                jump=false;
                MemRead=false;
          }
          else if(opcodeMap[opcode]=="R"){
                RegDst = 0;
                Branch = 0;
                MemtoReg = 0;
                ALUOp = 1; // Set as needed for the specific ALU operation
                MemWrite = 0;
                ALUSrc = 0;
                RegWrite = 1;
                jump=0;
                MemRead=0;
          }
          else if(opcodeMap[opcode]=="Addi")
          {
                RegDst = 1;
                Branch = 0;
                MemtoReg = 0;
                ALUOp = 0; 
                MemWrite = 0;
                ALUSrc = 1;
                RegWrite = 1;
                jump=0;
                MemRead=0; 
          }
          else if(opcodeMap[opcode]=="Addiu"){
                RegDst = 1;
                Branch = 0;
                MemtoReg = 0;
                ALUOp = 1; 
                MemWrite = 0;
                ALUSrc = 1;
                RegWrite = 1;
                jump=0;
                MemRead=0;   
          }
          else if(opcodeMap[opcode]=="Li")
          {
            RegDst = 1;
            Branch = 0;
            MemtoReg = 0;
            ALUOp = 1; 
            MemWrite = 0;
            ALUSrc = 1;
            RegWrite = 1;
            jump=0;
            MemRead=0; 
          }
            return
            to_string(RegDst) + 
            to_string(Branch) + 
            to_string(MemtoReg) + 
            to_string(ALUOp) + 
            to_string(MemWrite) +
            to_string(ALUSrc) +
            to_string(RegWrite) +
            to_string(jump) +
            to_string(MemRead);
    }
};

//Register File

class RegisterFile
{
    private:
    
        string instruction;

    public:
        RegisterFile(string instruction){
            this->instruction=instruction;
        }
        pair<string,string> read_data(){
           string var1_address = instruction.substr(6,5);
           string var2_address = instruction.substr(11,5);
        string opcode=instruction.substr(0,6);
           if(opcodeMap[opcode]=="Li"||opcodeMap[opcode]=="Addiu"||opcodeMap[opcode]=="Addi"||opcodeMap[opcode]=="lw")
           {
                read1=var1_address;
                read2="0";
           }
           else
           {
                read1=var1_address;
                read2=var2_address;
           }
           return {registerFile[var1_address],registerFile[var2_address]};
        }
        void write_reg(char ch,char ch2){
            //ControlInstruction c(instruction.substr(0,5));
            //string k=c.decodeAndGenerateControlSignals();
            if(ch=='1'){
                write_register=instruction.substr(11,5);
            }
            else{
                write_register=instruction.substr(16,5);
            }
            if(ch2=='1')
            {
                write=write_register;
            }
            else write="0";
        }
        void write_data(string data,char ch){
            if(ch=='1')
            {
                registerFile[write_register]=data;
            } 
        }
};

//Arthimetic Logic Unit

class ALU
{
    private:
        string variable1;
        string variable2;
        string variable3;
        string function;
        char AluSrc;
        char AluOp;
    public:
        ALU(string variable1,string variable2,string variable3,string function,char AluSrc,char AluOp)
        {
            this->variable1=variable1;
            this->variable2=variable2;
            this->variable3=variable3;
            this->function=function;
            this->AluSrc=AluSrc;
            this->AluOp=AluOp;
        }
        pair<string,string> Operation()
        {
            string input1=variable1,input2;
            input2=variable2;
            bitset<32> bitset1 = Convert_to_Binary(input1);
            bitset<32> bitset2 = Convert_to_Binary(input2);
            
            if(AluSrc=='0'){
                if(AluOp){
                    bitset<32>result1 = SubtractBitsets(bitset1, bitset2);
                    bitset<1> zero_output=IsZero(result1);
                    if(Instruction_Map[function]=="Add"){
                        bitset<32> result = AddBitsets(bitset1,bitset2);
                        return {zero_output.to_string(),result.to_string()};
                    }
                    else if(Instruction_Map[function]=="Subtract"){
                        bitset<32>result = SubtractBitsets(bitset1,bitset2);
                        return {zero_output.to_string(),result.to_string()};     
                    }
                    else if(Instruction_Map[function]=="Mult"){
                    
                        bitset<32> result = MultiplyBitsets(bitset1,bitset2);
                        return {zero_output.to_string(),result.to_string()};
                    }
                    else if(Instruction_Map[function]=="Ori"){
                        bitset<32> result = ORBitsets(bitset1,bitset2);
                        return {zero_output.to_string(),result.to_string()}; 
                    }
                    else if(Instruction_Map[function]=="xor"){
                        //cout<<"hi"<<endl;
                        bitset<32> result=xorBitsets(bitset1,bitset2);
                        return {zero_output.to_string(),result.to_string()};
                    }
                    else{
                        return {zero_output.to_string(),"0000000000000000000000000000000"};
                    }
                }
            }

            
            bitset<32>result1 = SubtractBitsets(bitset1,bitset2);
            bitset<1> zero_output=IsZero(result1);
            
            input2=variable3;
            bitset2=Convert_to_Binary(input2);
            bitset<32>result = AddBitsets(bitset1,bitset2);
            return {zero_output.to_string(),result.to_string()};
        }
};

//Data Class

// class Data
// {
// private:
//     string address;
//     string data;

// public:
//     Data(string data,string address)
//     {
//         this->data=data;   
//         this->address=address;
//     }

//     string getData()
//     {
//         return data;
//     }

//     string getAddress()
//     {
//         return address;
//     }
// };

//Data Memory

class DataMemory
{
    private:
        map<string,string> datas;
    public:
                                                                     
        void StoreData(string data,string address)
        {
            datas[address]=data;
        }

        map<string,string> getDatas()
        {
            return datas;
        }

        string getData(string address){
            return datas[address];
        }
};

Instruction_memory IM;

DataMemory DM;

int main() {
    initializeRegisterFile();
    string filename = "output1.txt"; 
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open the file." << endl;
        return 1; 
    }

    string line;
    while (getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        IM.addInstruction(line);
    }


    bitset<32> PC(0);


    while(1)
    {
        ++ timer;
        Instruction* currentInstruction = IM.Get_Instruction(PC.to_string());
        if(currentInstruction!=nullptr){

            string instruction=currentInstruction->getInstruction();
            
            string opcode=instruction.substr(0,6);
    
            ControlInstruction CI(opcode);
            
            RegisterFile RF(instruction);
            string controls=CI.decodeAndGenerateControlSignals();
            string jump=instruction.substr(6,26);
            jump="000000"+jump;
            if(controls[7]=='1'){
                
                bitset<32>bitset1=Convert_to_Binary(jump);
                PC=bitset1;
                continue;
            }
            pair<string,string> Variables= RF.read_data();
            RF.write_reg(controls[0],controls[6]);
            string variable3 = instruction.substr(16, 16);

            variable3="0000000000000000"+variable3; 

            bitset<32> var3=Convert_to_Binary(variable3);
            variable3=var3.to_string();
            ALU AL(Variables.first,Variables.second,variable3,instruction.substr(27,5),controls[5],controls[3]);

            pair<string,string> Results=AL.Operation();

            if(Results.first=="1"&&controls[1]=='1'){
                bitset<32>bitset1=Convert_to_Binary(variable3);
                bitset<32>bitset2=4;
                PC=AddBitsets(bitset1,bitset2);
                continue;
            }

            else if(controls[1]=='1'){
                PC=PC.to_ullong()+4;
                continue;
            }
                
            string wb_result="00000000000000000000000000000000";

            if(controls[4]=='1'){
                DM.StoreData(Variables.second,Results.second); 
                
            }  

            if(controls[8]=='1'){
                wb_result=DM.getData(Results.second);
            } 
            else{
                wb_result=AL.Operation().second;
            }

            if(controls[6]=='1'){
                RF.write_data(wb_result,controls[6]);
            }
            else{
                RF.write_data(Results.second,controls[6]);
            }
            
        }
        else {
            break;
        }
        PC=PC.to_ullong()+4;
        
    }
    std::cout<<DM.getDatas().size()<<endl;

    for(auto it:DM.getDatas()){
        std::cout<<it.first<<" "<<it.second<<endl;
    }

    file.close();

}