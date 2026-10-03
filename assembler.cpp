#include <iostream>
#include <fstream>
#include <string>
#include<algorithm>
#include<unordered_map>
#include<bitset>
using namespace std;

class Parser{
private : 
string line ;
size_t equalPos;
size_t semicolonPos;
public : 
void setLine(string newLine){
    line = newLine ;
    equalPos = line.find("=");
    semicolonPos = line.find(";");
}
bool isAInstruction(){
    return line[0] == '@' ;
}
string getInstructionA(){
    return line.substr(1) ;
}
string getCInstructionD(){
    if(equalPos != string::npos){
        return line.substr(0,equalPos);
    }
    return "";
}
string getCInstructionC(){
    if(semicolonPos != string::npos && equalPos != string::npos){
        return line.substr(equalPos + 1, semicolonPos - equalPos - 1);
    }
    if(equalPos != string::npos){
        return line.substr(equalPos + 1);
    }
    if(semicolonPos != string::npos){
        return line.substr(0, semicolonPos);
    }
    return line;
}
string getCInstructionJ(){
    if(semicolonPos != string::npos){
        return line.substr(semicolonPos + 1);
    }
    return "";
}
};
class Coder{
private:
unordered_map <string,string> dest;
unordered_map <string,string> compa0;
unordered_map <string,string> compa1;
unordered_map <string,string> jump;
public:
Coder(){
    // Initialize dest map
    dest[""] = "000";
    dest["M"] = "001";
    dest["D"] = "010";
    dest["MD"] = "011";
    dest["A"] = "100";
    dest["AM"] = "101";
    dest["AD"] = "110";
    dest["AMD"] = "111";
    // initialise jump map 
    jump[""] = "000";
    jump["JGT"] = "001";
    jump["JEQ"] = "010";
    jump["JGE"] = "011";
    jump["JLT"] = "100";
    jump["JNE"] = "101";
    jump["JLE"] = "110";
    jump["JMP"] = "111";
    // initialise compa0 map
    compa0[""] = "000000";
    compa0["0"] = "101010";
    compa0["1"] = "111111";
    compa0["-1"] = "111010";
    compa0["D"] = "001100";
    compa0["A"] = "110000";
    compa0["!D"] = "001101";
    compa0["!A"] = "110001";
    compa0["-D"] = "001111";
    compa0["-A"] = "110011";
    compa0["D+1"] = "011111";
    compa0["A+1"] = "110111";
    compa0["D-1"] = "001110";
    compa0["A-1"] = "110010";
    compa0["D+A"] = "000010";
    compa0["D-A"] = "010011";
    compa0["A-D"] = "000111";
    compa0["D&A"] = "000000";
    compa0["D|A"] = "010101";
    // initialise compa1 map
    compa1["M"] = "110000";
    compa1["!M"] = "110001";
    compa1["-M"] = "110011";
    compa1["M+1"] = "110111";
    compa1["M-1"] = "110010";
    compa1["D+M"] = "000010";
    compa1["D-M"] = "010011";
    compa1["M-D"] = "000111";
    compa1["D&M"] = "000000";
    compa1["D|M"] = "010101";
}
string getCompBinary(string comp){
    if(compa0.find(comp) != compa0.end()){
        return "0"+compa0[comp];
    }
    if(compa1.find(comp) != compa1.end()){
        return "1"+compa1[comp];
    }
    return "0000000";
}
string getDestBinary(string destStr){
    return dest[destStr];
};
string getJumpBinary(string jumpStr){
    return jump[jumpStr];
};
string getABinary(string A){
    int address = stoi(A);
    return "0" + bitset<15>(address).to_string();
}
};
class SymbolTable{
private :
unordered_map<string,string> table; 
unordered_map<string,string> labelTable;
int variableAddress = 16 ;
public : 
SymbolTable(){
    // Initialize predefined symbols
    table["SP"] = "0";
    table["LCL"] = "1";
    table["ARG"] = "2";
    table["THIS"] = "3";
    table["THAT"] = "4";
    for(int i=0;i<=15;i++){
        table["R"+to_string(i)] = to_string(i);
    }
    table["SCREEN"] = "16384";
    table["KBD"] = "24576";
}
void setLabelTable(string label,int lineNumber ){
    labelTable[label] = to_string(lineNumber);
}
string getInstruction(string A){
    if(table.find(A) != table.end()){
        return table[A];
    }
    if(labelTable.find(A) != labelTable.end()){
        return labelTable[A];
    }
    try{
    int address = stoi(A);
        return A;
    }catch(const invalid_argument& e){
        string address = to_string(variableAddress);
        table[A] = address;
        variableAddress++;
        return address; 
    }
}

};
void generateFile1(SymbolTable& symbolTable){
    ifstream inFile("file.asm");
    ofstream outFile("fileL.asm");
    string line ; 
    int lineNumber = 0;
    while(getline(inFile,line)){
        size_t commentPos = line.find("//");
        if(commentPos != string::npos){
            line = line.substr(0, commentPos);
        }
        line.erase(remove_if(line.begin(), line.end(), [](unsigned char c){
            return isspace(c);
        }), line.end());
        if(line.empty()){
            continue;
        }
       if(line[0] == '('){
        size_t closeBracketPos = line.find(')');
        string label = line.substr(1,closeBracketPos-1);
        symbolTable.setLabelTable(label,lineNumber);
       }else{
        lineNumber++;
        outFile << line << "\n" ; 
       }
    }
    inFile.close();
    outFile.close();
}


int main(){

    string line ;
    Parser parser ;
    Coder coder;
    SymbolTable symbolTable;
    bool firstLine = true;
    
    generateFile1(symbolTable);

    ifstream inFile("fileL.asm");
    ofstream outFile("file.hack");

    while(getline(inFile,line)){
        string binaryInstruction ="";
        cout << line << "\n";
        parser.setLine(line);
        if(!parser.isAInstruction()){
            string D = parser.getCInstructionD();
            string C = parser.getCInstructionC();
            string J = parser.getCInstructionJ();
            binaryInstruction = "111" + coder.getCompBinary(C) + coder.getDestBinary(D) + coder.getJumpBinary(J);
        }else{
            string A = parser.getInstructionA();
            string realA = symbolTable.getInstruction(A);
           binaryInstruction = coder.getABinary(realA);
        }

        if(!firstLine){
            outFile << "\n";
        }
        outFile << binaryInstruction ;
        firstLine = false;
    }
    inFile.close();
    outFile.close();
    return 0 ;
}