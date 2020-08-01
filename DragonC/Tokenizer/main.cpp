#include <iostream>
#include <vector>

using namespace std;

vector<string> tokenize( const string & raw )
{
    vector<string> tokenized;
    tokenized.push_back(raw);
    return tokenized;
}


int main()
{
    string raw = "test test2";
    vector<string> tokenizedText = tokenize(raw);
    for (std::vector<string>::iterator it = tokenizedText.begin(); it != tokenizedText.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';
    return 0;
}

