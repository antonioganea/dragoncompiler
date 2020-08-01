#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

string fileBuffer;

#define COMMENT_CHARACTER '#'

bool loadFromFile( string path, string & fb ) {
    char * buffer;
    std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
    if (!file){
        std::cout << "[buffer] failed to load : " << path << std::endl;
        return false;
    } else {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        buffer = new char[size];
        // std::vector<char> buffer(size);
        if (file.read(buffer, size))
        {
            std::cout << "[buffer] Loaded : " << size << " bytes." << std::endl;
            fb = buffer;
            return true;
        }
    }
    return true;
}

void removeComments( string & buffer ){
    stringstream s;

    string::iterator it;

    char temp;
    bool commentFlag = false;
    for ( it = buffer.begin(); it != buffer.end(); it++ ){
        temp = *it;
        if ( temp == COMMENT_CHARACTER ){
            commentFlag = true;
            continue;
        }
        if ( temp == '\n' ){
            commentFlag = false;
        }
        if ( !commentFlag ){
            s << temp;
        }
    }
    buffer = s.str();
}

void dump(){
    cout << fileBuffer << endl;
}

struct ASTNode{
    std::string raw;
    std::vector<ASTNode*> sons;
    void loadRaw( const string & buffer ){
        raw = buffer;
    }
    virtual void parse() = 0;
};

struct Program : public ASTNode {

};

#include <set>
#include <map>
class FSM{
private:
    int states;
    // accept token directly if it reaches any of those states
    set<int> acceptStates;

    // accept token if in any of this states and char cannot jump anywhere
    set<int> maxAcceptStates;

    // arrows[pair<state,char>] = nextState
    map<pair<int,char>,int> arrows;

    int currentState;

    string tokenBuffer;
public:

    FSM(){
        tokenBuffer = "";
        currentState = 0;

        loadFSM();
    }

    bool isStateMaxAcceptState( int state ){
        return maxAcceptStates.find(state) != maxAcceptStates.end();
    }

    bool isStateDirectAcceptState( int state ){
        return acceptStates.find(state) != acceptStates.end();
    }

    void resetState(){
        tokenBuffer = "";
        currentState = 0;
    }

    bool parse( char input, vector<string> & tokens ) {
        pair<int,char> from(currentState,input);

        map<pair<int,char>,int>::iterator it;
        it = arrows.find(from);
        if (it != arrows.end()){ // If can jump
            int destination = arrows[from];

            currentState = destination;
            tokenBuffer += input;

            if ( isStateDirectAcceptState(destination) ) { // if landed on a direct-accept state

                tokens.push_back(tokenBuffer);

                resetState();
                return true;
            }

        } else { // If cannot jump
            if ( isStateMaxAcceptState(currentState) ){
                tokens.push_back(tokenBuffer);

                resetState();


                parse(input,tokens); // go for the next one, starting from the beginning
                return true;
            } else { // If on a blank state, and cannot go anywhere, drop everything and restart from 0
                resetState();
                return false;
            }
        }


        return false;
    }

    void pushLink( int fromState, char byChar, int toState ){
        pair<int,char> key = pair<int,char>(fromState, byChar);
        arrows[key] = toState;
    }

    void pushAlphabetical(int fromState, int toState) {
        for ( char it = 'a'; it <= 'z'; it++ ){
            pushLink(fromState,it,toState);
        }
        for ( char it = 'A'; it <= 'Z'; it++ ){
            pushLink(fromState,it,toState);
        }
    }

    void pushNumerical(int fromState, int toState) {
        for ( char it = '0'; it <= '9'; it++ ){
            pushLink(fromState,it,toState);
        }
    }

    void loadFSM(){
        states = 12;
        pushAlphabetical(0,1);
        pushAlphabetical(1,1);
        pushNumerical(0,11);
        pushNumerical(11,11);

        pushLink(0,'-',5);
        pushLink(5,'-',6);

        pushLink(0,'+',2);
        pushLink(2,'+',3);

        pushLink(0,'=',7);
        pushLink(0,'>',7);
        pushLink(0,'<',7);
        pushLink(7,'=',8);

        pushLink(0,'(',4);
        pushLink(0,')',4);
        pushLink(0,'{',4);
        pushLink(0,'}',4);
        pushLink(0,'[',4);
        pushLink(0,']',4);
        pushLink(0,',',4);
        pushLink(0,';',4);
        pushLink(0,':',4);

        pushLink(0,'!',9);
        pushLink(9,'=',10);

        // Direct accept states
        acceptStates.insert(3);
        acceptStates.insert(4);
        acceptStates.insert(6);
        acceptStates.insert(8);
        acceptStates.insert(10);

        // Max accept states
        maxAcceptStates.insert(1);
        maxAcceptStates.insert(2);
        maxAcceptStates.insert(5);
        maxAcceptStates.insert(7);
        maxAcceptStates.insert(11);
    }

};

vector<string> tokenize( const string & raw ){
    // Vector of string to save tokens
    vector <string> tokens;

    // stringstream class check1
    stringstream check1(raw);

    string intermediate;

    // Tokenizing w.r.t. space ' '
    while(getline(check1, intermediate, ';'))
    {
        tokens.push_back(intermediate);
    }

    cout << "TOKENS" << endl;
    // Printing the token vector
    for(int i = 0; i < tokens.size(); i++)
        cout << tokens[i] << '\n';
    return tokens;
}

enum class TOKEN_TYPE{
    KEYWORD_VEC,
    KEYWORD_VAR,
    KEYWORD_ROM,
    KEYWORD_FUNC,
    KEYWORD_RETURN,
    KEYWORD_IF,
    KEYWORD_FOREVER,
    KEYWORD_BREAK,
    GENERAL_IDENTIFIER,
    NUMERAL,
    SEMICOLON,
    COLON,
    COMMA,
    EQUALS,
    LOP_EQUALS,
    LOP_NOT_EQUALS,
    LOP_GREATER_THAN,
    LOP_GREATER_EQUAL,
    LOP_LESS_THAN,
    LOP_LESS_EQUAL,
    CURLY_BRACKET_OPEN,
    CURLY_BRACKET_CLOSED,
    SQUARE_BRACKET_OPEN,
    SQUARE_BRACKET_CLOSED,
    PARANTHESIS_OPEN,
    PARANTHESIS_CLOSED,
};

int main(){
    loadFromFile("input.cd",fileBuffer);
    // dump();

    removeComments(fileBuffer);
    dump();

    //tokenize("var a = 10;");

    FSM myfsm;

    string buffer = fileBuffer;

    vector<string> outputTokens;

    char temp;
    for ( string::iterator it = buffer.begin(); it != buffer.end(); it++ ){
        temp = *it;
        myfsm.parse( temp, outputTokens );
    }


    cout << "Tokens scanned " << outputTokens.size() << endl;

    for ( vector<string>::iterator it = outputTokens.begin(); it != outputTokens.end(); it++ ){
        cout << *it << " ";
    }



    return 0;
}