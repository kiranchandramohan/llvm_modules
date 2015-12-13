#include <iostream>
#include <string>
#include <map>
using namespace std;

map<string, long long> opcodeCounter;

void printOpcodeStatistics();
void handleOpcode(char* opcode, int count)
{
  opcodeCounter[opcode] += count;
}

void printOpcodeStatistics()
{
  long long sum = 0;
  for (map<string, long long>::iterator i = opcodeCounter.begin(); i != opcodeCounter.end(); ++i) {
    cout << i->first << "\t" << i->second << "\n";
    sum += i->second;
  }
  cout << "TOTAL\t" << sum << "\n";
}
