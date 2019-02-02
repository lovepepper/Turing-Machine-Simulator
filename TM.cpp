#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;

typedef string STATE;
typedef char SYMBOL;
typedef char DIRECTION;

struct transFrom{
    STATE fromState;
    SYMBOL readSymbol;
    transFrom(){}
    transFrom(STATE str, SYMBOL ch){
        fromState = str;
        readSymbol = ch;
    }
};

inline bool operator < (transFrom const& l, transFrom const& r){
    return (l.fromState < r.fromState) || (l.fromState == r.fromState && l.readSymbol < r.readSymbol);
}

struct transTo{
    SYMBOL writeSymbol;
    DIRECTION direc;
    STATE toState;
    transTo(){}
    transTo(SYMBOL ch1, DIRECTION ch2, STATE str){
        writeSymbol = ch1;
        direc = ch2;
        toState = str;
    }
};

class TM{
    private:
        const int base = 500;
        int head = base;
        STATE currentState;
        char tape[1000];
        set<STATE> setQ; //states
        set<SYMBOL> setS; //initial symbols 
        set<SYMBOL> setT; //tape alphabet symbols
        STATE initialState;
        SYMBOL blankSymbol;
        set<STATE> setF; //final states
        map<transFrom, transTo> transFunctions;

    public:
        ofstream ccout;
        TM(){}
        TM(string FileDir){    
            constructTM(FileDir + "/test.tm"); 
            memset(tape, blankSymbol, 1000); 
            ccout.open(FileDir + "/console.txt");
        }
        void constructTM(string filename);
        void parser(string line);
        void readStateSet(set<STATE>& S, string line);
        void readSymbolSet(set<SYMBOL>& S, string line);
        void readTrans(string line);
        bool checkValidInput(string input);
        void displayID(int step);
        int execute(string input);

};

void TM::constructTM(string filename){
    ifstream in;
    in.open(filename.data());
    string line;
    while(getline(in, line))
        parser(line);
    in.close();
}

void TM::parser(string line){
    const char *p = line.data();
    if((p[0] == ';') or (line.length() == 0))
        return;
    else if(p[0] == '#'){
        switch(p[1]){
            case 'Q':
                readStateSet(setQ, line);
                break;
            case 'S':
                readSymbolSet(setS, line);
                break;
            case 'T':
                readSymbolSet(setT, line);
                break;
            case 'q':
                initialState = line.substr(line.find("=") + 2, line.size() - line.find("=") - 2);
                break;
            case 'B':
                blankSymbol = p[line.find("=") + 2];
                break;
            case 'F':
                readStateSet(setF, line);
                break;
        }
    }
    else
        readTrans(line);
}

void TM::readStateSet(set<STATE>& S, string line){
    int pos = line.find("{");
    int len = line.size() - pos - 2;
    string sstr = line.substr(pos + 1, len);
    replace(sstr.begin(), sstr.end(), ',', ' ');
    istringstream is(sstr);
    STATE s;
    while(is >> s)
        S.insert(s);
}

void TM::readSymbolSet(set<SYMBOL>& S, string line){
    int pos = line.find("{");
    int len = line.size() - pos - 2;
    string sstr = line.substr(pos + 1, len);
    replace(sstr.begin(), sstr.end(), ',', ' ');
    istringstream is(sstr);
    SYMBOL s;
    while(is >> s)
        S.insert(s);
}

void TM::readTrans(string line){
    istringstream is(line);
    STATE fromState;
    SYMBOL readSymbol;
    SYMBOL writeSymbol;
    DIRECTION direc;
    STATE toState;
    for(int i = 1; i <= 5; i++){
        switch(i){
            case 1:
                is >> fromState;
                break;
            case 2:
                is >> readSymbol;
                break;
            case 3:
                is >> writeSymbol;
                break;
            case 4:
                is >> direc;
                break;
            case 5:
                is >> toState;
                break;
        }
    }
    transFrom tf(fromState, readSymbol);
    transTo tt(writeSymbol, direc, toState);
    transFunctions.insert(make_pair(tf, tt));
}

bool TM::checkValidInput(string input){
    const char *p = input.data();
    for(int i = 0; i < input.length(); i++){
        if(setS.find(p[i]) != setS.end())
            continue;
        else
            return false;
    }
    return true;
}

void TM::displayID(int step){
    int leftmost = head, rightmost = head;
    for(int i = 0; i < 1000; i++)
        if(tape[i] != blankSymbol){
            leftmost = i;
            break;
        }
    for(int i = 999; i >= 0; i--)
        if(tape[i] != blankSymbol){
            rightmost = i;
            break;
        }
    if(leftmost > head)
        leftmost = head;
    if(rightmost < head)
        rightmost = head;
    ccout << "Step  : " << step << endl;
    ccout << "Index :";
    assert(leftmost <= rightmost);
    for(int i = leftmost; i <= rightmost; i++)
        ccout << " " << abs(i - base);
    ccout << endl;
    ccout << "Tape  : ";
    for(int i = leftmost; i < rightmost; i++){
        ccout << tape[i];
        int t = abs(i - base);
        if(t == 0)
            ccout << " ";
        while(t){
            ccout << " ";
            t /= 10;
        }
    }
    ccout << tape[rightmost] << endl;
    ccout << "Head  : ";
    for(int i = leftmost; i <= rightmost; i++){
        if(i == head){
            ccout << "^" << endl;
            break;
        }
        else
            ccout << " ";
        int t = abs(i - base);
        if(t == 0)
            ccout << " ";
        while(t){
            ccout << " ";
            t /= 10;
        }
    }
    ccout << "State : " << currentState << endl;
    ccout << "---------------------------------------------" << endl;
}

int TM::execute(string input){
    int flag;
    ccout << "Input: " << input << endl;
    if(!checkValidInput(input)){
        ccout << "==================== ERR ====================" << endl;
        ccout << "The Input \"" << input << "\" is illegal" << endl;
        flag = -1;
    }
    else{
        ccout << "==================== RUN ====================" << endl;
        int step = 0;
        head = base;
        memcpy((tape + base), input.data(), input.length());
        currentState = initialState;
        displayID(step);
        while(true){
            step++;
            transFrom tf1(currentState, tape[head]);
            transFrom tf2(currentState, '*');
            if(transFunctions.find(tf1) != transFunctions.end()){
                transTo tt = transFunctions[tf1];
                if(tt.writeSymbol != '*')
                    tape[head] = tt.writeSymbol;
                if(tt.direc == 'l')
                    head--;
                else if(tt.direc == 'r')
                    head++;
                currentState = tt.toState;
                displayID(step);
            }
            else if(transFunctions.find(tf2) != transFunctions.end()){
                transTo tt = transFunctions[tf2];
                if(tt.writeSymbol != '*')
                    tape[head] = tt.writeSymbol;
                if(tt.direc == 'l')
                    head--;
                else if(tt.direc == 'r')
                    head++;
                currentState = tt.toState;
                displayID(step);
            }
            else
                break;
        }
        int leftmost, rightmost;
        for(int i = 0; i < 1000; i++)
            if(tape[i] != blankSymbol){
                leftmost = i;
                break;
            }
        for(int i = 999; i >= 0; i--)
            if(tape[i] != blankSymbol){
                rightmost = i;
                break;
            }
        ccout << "Result: ";
        for(int i = leftmost; i <= rightmost; i++)
            ccout << tape[i];
        ccout << endl;
        if(tape[leftmost] == 'T')
            flag = 1;
        else
            flag = 0;
    }
    ccout << "==================== END ====================" << endl;
    memset(tape, blankSymbol, 1000);
    return flag;
}

int main(int argc, char* argv[]){
    string FileDir = argv[1];
    TM tm(FileDir);
    ifstream testin;
    testin.open(FileDir + "/input.txt");
    string input;
    ofstream testout;
    testout.open(FileDir + "/output.txt");
    while(getline(testin, input)){
        int res = tm.execute(input);
        switch(res){
            case -1:
                testout << "Error" << endl;
                break;
            case 0:
                testout << "False" << endl;
                break;
            case 1:
                testout << "True" << endl;
                break;
        }
    }
    testin.close();
    testout.close();
    return 0;
}