#include <bits/stdc++.h>
using namespace std;

#define STACK_SIZE 262144

/* GLOBALS */
char STACK[STACK_SIZE];
unordered_map<string, int> register_value;
unordered_map<string, int> function_line;
vector<string> lines;

int sp = STACK_SIZE;
int pc = 0;
bool start = false;
bool skip = true;

void fill_registers() {
    register_value["x0"] = 0;
    register_value["x1"] = 0;
    register_value["x2"] = STACK_SIZE;
    register_value["x3"] = 0;
    register_value["x4"] = 0;
    register_value["x5"] = 0;
    register_value["x6"] = 0;
    register_value["x7"] = 0;
    register_value["x8"] = 0;
    register_value["x9"] = 0;
    register_value["x10"] = 0;
    register_value["x11"] = 0;
    register_value["x12"] = 0;
    register_value["x13"] = 0;
    register_value["x14"] = 0;
    register_value["x15"] = 0;
    register_value["x16"] = 0;
    register_value["x17"] = 0;
    register_value["x18"] = 0;
    register_value["x19"] = 0;
    register_value["x20"] = 0;
    register_value["x21"] = 0;
    register_value["x22"] = 0;
    register_value["x23"] = 0;
    register_value["x24"] = 0;
    register_value["x25"] = 0;
    register_value["x26"] = 0;
    register_value["x27"] = 0;
    register_value["x28"] = 0;
    register_value["x29"] = 0;
    register_value["x30"] = 0;
    register_value["x31"] = 0;
}


/* TRIES TO READ A FILE */
void read_file(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Error: Could not open file " + path);
    }
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
}

/* TRIMS THE LINE IN A FORMATTED WAY */
string trim(const string& line){
    string clean_line = line;
    clean_line.erase(remove(clean_line.begin(), clean_line.end(), '\t'), clean_line.end());
    if (clean_line.find(" ") == string::npos) return clean_line;
    size_t pos = clean_line.find("#");
    if (pos != string::npos) {
        clean_line = clean_line.substr(0, pos);
    }
    size_t start = clean_line.find_first_not_of(" ");
    size_t end = clean_line.find_last_not_of(" ");
    if (start == string::npos || end == string::npos) {
        return "";
    }
    clean_line = clean_line.substr(start, end - start + 1);
    stringstream ss(clean_line);
    string word, result;
    while (ss >> word) {
        word.erase(remove(word.begin(), word.end(), ','), word.end());
        if (!result.empty()) {
            result += " ";
        }
        result += word;
    }
    return result;
}

/* EXECUTE INSTRUCTIONS */
void execute_add(const string& rd, const string& rs1, const string& rs2){
    register_value[rd] = register_value[rs1] + register_value[rs2];
}

void execute_sub(const string& rd, const string& rs1, const string& rs2){
    register_value[rd] = register_value[rs1] - register_value[rs2];
}

void execute_xor(const string& rd, const string& rs1, const string& rs2){
    register_value[rd] = register_value[rs1] ^ register_value[rs2];
}

void execute_or(const string& rd, const string& rs1, const string& rs2){
    register_value[rd] = register_value[rs1] | register_value[rs2];
}

void execute_and(const string& rd, const string& rs1, const string& rs2){
    register_value[rd] = register_value[rs1] & register_value[rs2];
}

void execute_sll(const string& rd, const string& rs1, const string& rs2){
    register_value[rd] = register_value[rs1] << register_value[rs2];
}

void execute_srl(const string& rd, const string& rs1, const string& rs2){
    register_value[rd] = (unsigned)register_value[rs1] >> register_value[rs2];
}

void execute_addi(const string& rd, const string& rs1, const int imm){
    register_value[rd] = register_value[rs1] + imm;
}

void execute_xori(const string& rd, const string& rs1, const int imm){
    register_value[rd] = register_value[rs1] ^ imm;
}

void execute_ori(const string& rd, const string& rs1, const int imm){
    register_value[rd] = register_value[rs1] | imm;
}

void execute_andi(const string& rd, const string& rs1, const int imm){
    register_value[rd] = register_value[rs1] & imm;
}

void execute_slli(const string& rd, const string& rs1, const int imm){
    register_value[rd] = register_value[rs1] << imm;
}

void execute_srli(const string& rd, const string& rs1, const int imm){
    register_value[rd] = (unsigned)register_value[rs1] >> imm;
}

void execute_li(const string& rd, const int imm){
    if (rd == "x0" || rd == "x2") {
        printf("Error occured: loading values in zero/sp register\n");
        exit(1);
    }
    register_value[rd] = imm;
}

void execute_lb(const string& rd, const int offset){
    if (rd == "x0" || rd == "x2") {
        printf("Error occured: loading values in zero/sp register\n");
        exit(1);
    }
    int addr = register_value["x2"] + offset;
    register_value[rd] = (int)(*(char*)(&STACK[addr]));
}

void execute_lh(const string& rd, const int offset){
    if (rd == "x0" || rd == "x2") {
        printf("Error occured: loading values in zero/sp register\n");
        exit(1);
    }
    int addr = register_value["x2"] + offset;
    register_value[rd] = (int)(*(short*)(&STACK[addr]));
}

void execute_lw(const string& rd, const int offset){
    if (rd == "x0" || rd == "x2") {
        printf("Error occured: loading values in zero/sp register\n");
        exit(1);
    }
    int addr = register_value["x2"] + offset;
    register_value[rd] = (*(int*)(&STACK[addr]));
}

void execute_sb(const string& rs, const int offset){
    int addr = register_value["x2"] + offset;
    (*(char*)(&STACK[addr])) = (char)register_value[rs]; 
}

void execute_sh(const string& rs, const int offset){
    int addr = register_value["x2"] + offset;
    (*(short*)(&STACK[addr])) = (short)register_value[rs];
}

void execute_sw(const string& rs, const int offset){
    int addr = register_value["x2"] + offset;
    (*(int*)(&STACK[addr])) = register_value[rs];
}

void execute_beq(const string& rs1, const string& rs2, const string& label){
    if (function_line.find(label) == function_line.end()) {
        printf("Error occured: label %s not found\n", label.c_str());
        exit(1);
    }
    if (register_value[rs1] == register_value[rs2]) {
        pc = function_line[label] - 1;
        skip = false;
    }
}

void execute_bne(const string& rs1, const string& rs2, const string& label){
    if (function_line.find(label) == function_line.end()) {
        printf("Error occured: label %s not found\n", label.c_str());
        exit(1);
    }
    if (register_value[rs1] != register_value[rs2]) {
        pc = function_line[label] - 1;
        skip = false;
    }
}

void execute_blt(const string& rs1, const string& rs2, const string& label){
    if (function_line.find(label) == function_line.end()) {
        printf("Error occured: label %s not found\n", label.c_str());
        exit(1);
    }
    if (register_value[rs1] < register_value[rs2]) {
        pc = function_line[label] - 1;
        skip = false;
    }
}

void execute_bge(const string& rs1, const string& rs2, const string& label){
    if (function_line.find(label) == function_line.end()) {
        printf("Error occured: label %s not found\n", label.c_str());
        exit(1);
    }
    if (register_value[rs1] >= register_value[rs2]) {
        pc = function_line[label] - 1;
        skip = false;
    }
}

void execute_bgt(const string& rs1, const string& rs2, const string& label){
    if (function_line.find(label) == function_line.end()) {
        printf("Error occured: label %s not found\n", label.c_str());
        exit(1);
    }
    if (register_value[rs1] > register_value[rs2]) {
        pc = function_line[label] - 1;
        skip = false;
    }
}

void execute_ble(const string& rs1, const string& rs2, const string& label){
    if (function_line.find(label) == function_line.end()) {
        printf("Error occured: label %s not found\n", label.c_str());
        exit(1);
    }
    if (register_value[rs1] <= register_value[rs2]) {
        pc = function_line[label] - 1;
        skip = false;
    }
}

void execute_j(const string& label){
    if (function_line.find(label) == function_line.end()) {
        printf("Error occured: label %s not found\n", label.c_str());
        exit(1);
    }
    pc = function_line[label] - 1;
}

void execute_ecall(){
    int type = register_value["x10"];
    switch (type) {
        case 1 :
            printf("%d\n", register_value["x11"]);
            break;
        case 10:
            exit(register_value["x11"]);
            break;
        case 11:
            printf("%c", (char)register_value["x11"]);
            break;
        default:
            printf("Error occured: unsupported ecall number\n");
            exit(1);
    }
}

void execute_call(const string& label){
    if (function_line.find(label) == function_line.end()) {
        printf("Error occured: label %s not found\n", label.c_str());
        exit(1);
    }
    register_value["x1"] = pc + 1;
    pc = function_line[label];
    skip = false;
}

void execute_ret(){
    pc = register_value["x1"];
    if (pc < 0 || pc >= lines.size()) {
        printf("Error occured: invalid return address %d\n", pc);
        exit(1);
    }
    skip = false;
}

void execute_mul(const string& rd, const string& rs1, const string& rs2){
    register_value[rd] = register_value[rs1] * register_value[rs2];
}

void execute_div(const string& rd, const string& rs1, const string& rs2){
    if (register_value[rs2] == 0) {
        printf("Error occured: division by zero in div\n");
        exit(EXIT_FAILURE);
    }
    register_value[rd] = register_value[rs1] / register_value[rs2];
}

void execute_rem(const string& rd, const string& rs1, const string& rs2){
    if (register_value[rs2] == 0) {
        printf("Error occured: division by zero in rem\n");
        exit(EXIT_FAILURE);
    }
    register_value[rd] = register_value[rs1] % register_value[rs2];
}

void execute_mv(const string& rd, const string& rs1){
    register_value[rd] = register_value[rs1];
}

/* CHECKS IF INSTRUCTION IS ALU */
bool isALU(const string& instruction) {
    return instruction == "add" || instruction == "sub" || instruction == "xor" || instruction == "or" || instruction == "and" ||
    instruction == "sll" || instruction == "srl" || instruction == "mul" || instruction == "div" || instruction == "rem";
}

/* CHECKS IF INSTRUCTION IS ALU IMMEDIATE */
bool isALUI(const string& instruction) {
    return instruction == "addi" || instruction == "xori" || instruction == "ori" || instruction == "andi" ||
    instruction == "slli" || instruction == "srli";
}

/* CHECKS IF INSTRUCTION IS BRANCH */
bool isBRANCH(const string& instruction) {
    return instruction == "beq" || instruction == "bne" || instruction == "blt" || instruction == "bge" ||
           instruction == "bgt" || instruction == "ble";
}

/* CHECKS IF INSTRUCTION IS LOAD */
bool isLOAD(const string& instruction) {
    return instruction == "lb" || instruction == "lh" || instruction == "lw";
}

/* CHECKS IF INSTRUCTION IS STORE */
bool isSTORE(const string& instruction) {
    return instruction == "sb" || instruction == "sh" || instruction == "sw";
}

/* CLEANS THE REGISTER FROM BRACES */
string clean_register(const string& reg) {
    string clean_reg = reg;
    clean_reg.erase(remove(clean_reg.begin(), clean_reg.end(), '('), clean_reg.end());
    clean_reg.erase(remove(clean_reg.begin(), clean_reg.end(), ')'), clean_reg.end());
    if (clean_reg == "zero") clean_reg = "x0";
    else if (clean_reg == "ra") clean_reg = "x1";
    else if (clean_reg == "sp") clean_reg = "x2";
    else if (clean_reg == "gp") clean_reg = "x3";
    else if (clean_reg == "tp") clean_reg = "x4";
    else if (clean_reg == "t0") clean_reg = "x5";
    else if (clean_reg == "t1") clean_reg = "x6";
    else if (clean_reg == "t2") clean_reg = "x7";
    else if (clean_reg == "s0" || clean_reg == "fp") clean_reg = "x8";
    else if (clean_reg == "s1") clean_reg = "x9";
    else if (clean_reg == "a0") clean_reg = "x10";
    else if (clean_reg == "a1") clean_reg = "x11";
    else if (clean_reg == "a2") clean_reg = "x12";
    else if (clean_reg == "a3") clean_reg = "x13";
    else if (clean_reg == "a4") clean_reg = "x14";
    else if (clean_reg == "a5") clean_reg = "x15";
    else if (clean_reg == "a6") clean_reg = "x16";
    else if (clean_reg == "a7") clean_reg = "x17";
    else if (clean_reg == "s2") clean_reg = "x18";
    else if (clean_reg == "s3") clean_reg = "x19";
    else if (clean_reg == "s4") clean_reg = "x20";
    else if (clean_reg == "s5") clean_reg = "x21";
    else if (clean_reg == "s6") clean_reg = "x22";
    else if (clean_reg == "s7") clean_reg = "x23";
    else if (clean_reg == "s8") clean_reg = "x24";
    else if (clean_reg == "s9") clean_reg = "x25";
    else if (clean_reg == "s10") clean_reg = "x26";
    else if (clean_reg == "s11") clean_reg = "x27";
    else if (clean_reg == "t3") clean_reg = "x28";
    else if (clean_reg == "t4") clean_reg = "x29";
    else if (clean_reg == "t5") clean_reg = "x30";
    else if (clean_reg == "t6") clean_reg = "x31";
    return clean_reg;
}
/* VALIDATES THE REGISTER */
bool validate_register(const string& reg) {
    return register_value.find(reg) != register_value.end();
}

/* PROCESSES THE FILE LINE BY LINE */
void process(const string& line) {
    string trimmed = trim(line); // FORMATTED EACH LINE PROPERLY
    if (trimmed == ".text") {
        start = true;
        return;
    }
    if (trimmed.empty() || start == false) return;
    stringstream ss(trimmed);
    string instruction, rd, rs1, rs2, imm, offset, label;
    ss >> instruction;
    if (isALU(instruction)) {
        ss >> rd >> rs1 >> rs2;
        rd = clean_register(rd);
        rs1 = clean_register(rs1);
        rs2 = clean_register(rs2);
        if (validate_register(rd) && validate_register(rs1) && validate_register(rs2)) {
            if (instruction == "add") execute_add(rd, rs1, rs2);
            else if (instruction == "sub") execute_sub(rd, rs1, rs2);
            else if (instruction == "xor") execute_xor(rd, rs1, rs2);
            else if (instruction == "or") execute_or(rd, rs1, rs2);
            else if (instruction == "and") execute_and(rd, rs1, rs2);
            else if (instruction == "sll") execute_sll(rd, rs1, rs2);
            else if (instruction == "srl") execute_srl(rd, rs1, rs2);
            else if (instruction == "mul") execute_mul(rd, rs1, rs2);
            else if (instruction == "div") execute_div(rd, rs1, rs2);
            else if (instruction == "rem") execute_rem(rd, rs1, rs2);
        }
        else {
            printf("Invalid register at line %d\n", pc + 1);
            exit(1);
        }
        return;
    }
    if (isALUI(instruction)) {
        ss >> rd >> rs1 >> imm;
        rd = clean_register(rd);
        rs1 = clean_register(rs1);
        int immediate = stoi(imm);

        if (validate_register(rd) && validate_register(rs1)) {
            if (instruction == "addi") execute_addi(rd, rs1, immediate);
            else if (instruction == "xori") execute_xori(rd, rs1, immediate);
            else if (instruction == "ori") execute_ori(rd, rs1, immediate);
            else if (instruction == "andi") execute_andi(rd, rs1, immediate);
            else if (instruction == "slli") execute_slli(rd, rs1, immediate);
            else if (instruction == "srli") execute_srli(rd, rs1, immediate);
        }
        else {
            printf("Invalid register at line %d\n", pc + 1);
            exit(1);
        }
        return;
    }
    if (isBRANCH(instruction)) {
        ss >> rs1 >> rs2 >> label;
        rs1 = clean_register(rs1);
        rs2 = clean_register(rs2);

        if (validate_register(rs1) && validate_register(rs2)) {
            if (instruction == "beq") execute_beq(rs1, rs2, label);
            else if (instruction == "bne") execute_bne(rs1, rs2, label);
            else if (instruction == "blt") execute_blt(rs1, rs2, label);
            else if (instruction == "bge") execute_bge(rs1, rs2, label);
            else if (instruction == "bgt") execute_bgt(rs1, rs2, label);
            else if (instruction == "ble") execute_ble(rs1, rs2, label);
        }
        else {
            printf("Invalid register at line %d\n", pc + 1);
            exit(1);
        }
        return;
    }
    if (isLOAD(instruction)) {
        ss >> rd >> offset;
        rd = clean_register(rd);
        int off = stoi(offset.substr(0, offset.find('(')));
        if (instruction == "lb") execute_lb(rd, off);
        else if (instruction == "lh") execute_lh(rd, off);
        else if (instruction == "lw") execute_lw(rd, off);
        else {
            printf("Invalid register at line %d\n", pc + 1);
            exit(1);
        }
        return;
    }
    if (isSTORE(instruction)) {
        ss >> rd >> offset;
        rd = clean_register(rd);
        int off = stoi(offset.substr(0, offset.find('(')));
        if (instruction == "sb") execute_sb(rd, off);
        else if (instruction == "sh") execute_sh(rd, off);
        else if (instruction == "sw") execute_sw(rd, off);
        else {
            printf("Invalid register at line %d\n", pc + 1);
            exit(1);
        }
        return;
    }
    if (instruction == "li") {
        ss >> rd >> imm;
        rd = clean_register(rd);
        int immediate = stoi(imm);

        if (validate_register(rd)) {
            execute_li(rd, immediate);
        }
        else {
            printf("Invalid register at line %d\n", pc + 1);
            exit(1);
        }
        return;
    }
    if (instruction == "mv") {
        ss >> rd >> rs1;
        rd = clean_register(rd);
        rs1 = clean_register(rs1);
        if (validate_register(rd)) {
            execute_mv(rd, rs1);
        }
        else {
            printf("Invalid register at line %d\n", pc + 1);
            exit(1);
        }
        return;
    }
    if (instruction == "call") {
        ss >> label;
        execute_call(label);
        return;
    }
    if (instruction == "j") {
        ss >> label;
        execute_j(label);
        return;
    }
    if (instruction == "ecall") {
        execute_ecall();
        return;
    }
    if (instruction == "ret") {
        execute_ret();
        return;
    }
}

/* EMULATES ASSEMBLY FILE */
void emulate(){
    while (pc < lines.size()) {
        string curr = lines[pc];
        if (curr.find(":") != string::npos) {pc++; continue;}
        process(curr);
        if (skip) pc++;
        skip = true;
    }
}

int main(int argc, char* argv[]){
    string path;
    printf("Enter file path to emulate: ");
    cin >> path;
    try {
        read_file(path);
        for (int i = 0; i < lines.size(); i++) {
            string trimmed = trim(lines[i]);
            if (trimmed.find(':') != string::npos) {
                size_t pos = trimmed.find(':');
                string label = trimmed.substr(0, pos);
                function_line[label] = i + 1;
            }
        }
    } catch (const exception& e) {
        cout <<"Error happened when trying to read a file\n";
        return 1;
    }
    fill_registers();
    emulate();
    return 0;
}