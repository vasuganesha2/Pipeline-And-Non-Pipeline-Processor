#include<iostream>
#include<bits/stdc++.h>
using namespace std;


int timer = 0;

//Global Variables

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

//register set for MIPS
//vector<string> regs = {"$sero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5",
//"$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};

map<string, string> registerFile;

void initializeRegisterFile() {
    for (int i = 0; i < 32; ++i) {
        string binaryAddress = bitset<5>(i).to_string();
                                   //Initially All values of register is write to 0
        registerFile[binaryAddress] = bitset<32>(0).to_string();
    }
}

int N(bitset<32> a)
{
    int n = 0;
    for(int i = 0; i < 32; i ++)    n += a[i] * (1 << i);
    return n;
}

std::bitset<32> SubtractBitsets(const std::bitset<32>& a, const std::bitset<32>& b) {

    bitset<32> result(N(a) - N(b));
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
    bitset<32> result (N(a) + N(b));
    return result;
}

bitset<32> MultiplyBitsets(bitset<32>& a, bitset<32>& b) {
    bitset<32> result(N(a) * N(b));
    return result;
}

bitset<32> ORBitsets(bitset<32>& a, bitset<32>& b) {
    std::bitset<32> result = a | b;
    return result;
}

bitset<32> XORBitsets(bitset<32>& a, bitset<32>&b)
{
    std::bitset<32> result = a ^ b;
    return result;
}


int N(string a)
{
    int d = 0;

    for(char digit : a)
    {
        d = d * 2 + (digit - '0');
    }

    return d;
}

bool IsZero(bitset<32>& bits) {
    return bits.none();
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

    void show()
    {
        vector<string> a;
        for(auto instruction : Instructions)
            a.push_back(instruction.getInstruction());
        
    }
};

//Control Instruction
class ControlInstruction {
private:
    string opcode;

public:
    ControlInstruction()
    {
        this->opcode = "";
    }
    ControlInstruction(string opcode){
        this->opcode=opcode;
    }
    void init(string opcode)
    {
        this->opcode = opcode;
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
    public:
        string instruction, read1 = "0", read2 = "0", write = "0", write_register;

    public:
        RegisterFile()
        {
            this->instruction = instruction;
        }
        RegisterFile(string instruction){
            this->instruction=instruction;
        }
        void init(string instruction)
        {
            this->instruction = instruction;
        }
        pair<string,string> read_data(){
           string var1_address = instruction.substr(6,5);
           string var2_address = instruction.substr(11,5);

           string opcode=instruction.substr(0,6);
           if(opcodeMap[opcode]=="Li"||opcodeMap[opcode]=="Addiu"||opcodeMap[opcode]=="Addi"||opcodeMap[opcode]=="lw")
           {
                read1=var1_address;
           }
           else
           {
                read1=var1_address;
                read2=var2_address;
           }

           return {registerFile[var1_address],registerFile[var2_address]};
        }
        void write_reg(char ch,char ch2){
            if(ch=='1'){
                write_register=instruction.substr(11,5);
            }
            else{
                write_register=instruction.substr(16,5);
            }
            if(ch2=='1'){
                write = write_register;
            }
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
        ALU()
        {

        }
        ALU(string variable1,string variable2,string variable3,string function,char AluSrc,char AluOp)
        {
            this->variable1=variable1;
            this->variable2=variable2;
            this->variable3=variable3;
            this->function=function;
            this->AluSrc=AluSrc;
            this->AluOp=AluOp;
        }
        void init(string variable1,string variable2,string variable3,string function,char AluSrc,char AluOp)
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
                        bitset<32> result=bitset1^bitset2;
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

class Data
{
private:
    string address;
    string data;

public:

    Data(string data,string address)
    {
        this->data=data;  
        this->address=address;
    }

    string getData()
    {
        return data;
    }

    string getAddress()
    {
        return address;
    }
};

//Data Memory

class DataMemory
{
    public:
        map<string,string> datas;                                              
        void StoreData(string data,string address)
        {
            datas[address]=data;
        }

        map<string,string> getDatas()
        {
            return datas;
        }

        string getData(string address){
            if(datas.find(address) == datas.end())  return "0000000000000000000000000000000";
            return datas[address];
        }
};

DataMemory DM;

multiset<string> ToBeModified;

bitset<32> PC(0);
struct InstructionOperations
{
    string instruction, opcode, controls, jump, variable3, wb_result, write_register;
    bitset<32> var3;
    pair<string, string> Variables, Results;
    ControlInstruction CI;
    RegisterFile RF;
    ALU AL;
    bitset<32> Address;
    Instruction* currentInstruction;

    string writeTo = "0", read1 = "0", read2 = "0";

    bool uninit = true;
    bool stalled_at_id = false;
    bool done = false;

    InstructionOperations() {};

    void init(Instruction* currentInstruction, bitset<32> PC)
    {
        uninit = false;
        this->Address = PC;
        this->currentInstruction = currentInstruction;
    }

    void InstructionFetch()
    {
        if(done)    return;

        instruction = currentInstruction -> getInstruction();
        opcode=instruction.substr(0, 6);
    }

    int InstructionDecode()
    {
        if(done)    return false;

        CI.init(opcode);
        RF.init(instruction);
        controls = CI.decodeAndGenerateControlSignals();


        jump=instruction.substr(6,26);
        jump="000000"+jump;
        if(controls[7]=='1')
        {
            bitset<32>bitset1=Convert_to_Binary(jump);
            PC=bitset1;
            //debug(timer, N(bitset1));
            return 1;
        }

        Variables= RF.read_data();
        RF.write_reg(controls[0], controls[6]);

        writeTo = RF.write, read1 = RF.read1, read2 = RF.read2;

        if((read1 != "0" and ToBeModified.count(read1)) or (read2 != "0" and ToBeModified.count(read2)))
        {
            stalled_at_id = true;
            return 2;
        }
       
        stalled_at_id = false;
        variable3 = instruction.substr(16, 16);
        variable3="0000000000000000"+variable3;
        var3=Convert_to_Binary(variable3);
        variable3=var3.to_string();
       
        return 0;
    }

    bool ExecuteAlu()
    {
        if(done)    return false;

        AL.init(Variables.first,Variables.second,variable3,instruction.substr(27,5),controls[5],controls[3]);

        Results=AL.Operation();

        if(Results.first=="1"&&controls[1]=='1'){
            bitset<32>bitset1=Convert_to_Binary(variable3);
            bitset<32>bitset2=4;
            PC=AddBitsets(bitset1,bitset2);
            return true;
        }
        return false;
    }

    void MemoryPhase()
    {
        if(done)    return;
        wb_result="00000000000000000000000000000000";

        if(controls[4]=='1')
            DM.StoreData(Variables.second,Results.second);
       
        if(controls[8]=='1')
            wb_result=DM.getData(Results.second);
        else
            wb_result=AL.Operation().second;
    }

    void WriteBack()
    {
        if(done)    return;

        if(controls[6]=='1')
            RF.write_data(wb_result,controls[6]);
        else
            RF.write_data(Results.second,controls[6]);
    }

    void clear()
    {
        done = true;
        instruction = "0";
    }
};

Instruction_memory IM;


//bitset<32> Instruction::address(0);
deque<InstructionOperations> q(6);

int main() {
    initializeRegisterFile();
    string filename = "prev.txt";
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

    for(int i = 2; i <= 5; i ++)    q[i].clear();

    while(1)
    {
        ++ timer;

        Instruction* currentInstruction;

        bool remain = false;
        for(int i = 1; i <= 5; i ++)
            remain = remain or !q[i].done;
       
        if(!q[2].stalled_at_id)
            currentInstruction = IM.Get_Instruction(PC.to_string());

        bool no_more_instructions = (currentInstruction==nullptr);

        if(no_more_instructions and q[1].uninit)
        {
            q[0].clear();
            q[1].clear();
        }

        if(remain or !no_more_instructions)
        {
            if(!q[2].stalled_at_id)
                q[1].init(currentInstruction, PC);

            if(!q[3].done)
                ToBeModified.insert(q[3].writeTo);
           
            q[5].WriteBack();
            if(!q[5].done)
                if(ToBeModified.find(q[5].writeTo) != ToBeModified.end())           //write back done in first half of cycle
                    ToBeModified.erase(ToBeModified.find(q[5].writeTo));

            q[4].MemoryPhase();
            bool beq_encountered = q[3].ExecuteAlu();
            int id_status = q[2].InstructionDecode();
            q[1].InstructionFetch();

            cerr << timer << " " << N(PC) << endl;
            cerr << "IF: " << q[1].instruction << endl;
            cerr << "ID: " << q[2].instruction << endl;
            cerr << "EX: " << q[3].instruction << endl;
            cerr << "DM: " << q[4].instruction << endl;
            cerr << "WB: " << q[5].instruction << endl;
            cerr << endl << endl;

            if(beq_encountered)
            {
                q[1].clear();
                q[2].clear();
                q[3].clear();
                //q[4].clear();
                q.pop_back();
                q.push_front(InstructionOperations());
            }

            else if(id_status == 1)         //jump
            {
                q[1].clear();
                q[2].clear();
                q.pop_back();
                q.push_front(InstructionOperations());
            }

            else if(id_status == 2)         //stall
            {
                q.pop_back();
                q.insert(q.begin() + 3, InstructionOperations());    
                q[3].clear();
            }

            else
            {
                PC=PC.to_ullong()+4;
                q.pop_back();
                q.push_front(InstructionOperations());
            }
            if(no_more_instructions)    q[0].clear();
        }
        else    break;
        //if(timer==200) break;
    }

    std::cout<<DM.getDatas().size()<<endl;

    for(auto it:DM.getDatas()){
        cout << it.first << " " << it.second << endl;
    }
    file.close();
}