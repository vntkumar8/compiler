#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;

vector <string> production, non_terminals, terminals, parser_table[100];
map <  string,  vector< pair<string, int> > > first1;
map< string, vector< string> > follow;
map<string, int> vis, vis2;
int no_of_rules;
