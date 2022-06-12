#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <vector>

using namespace std;

string help(const string option);
string help(const vector<string>& args);
vector<vector<string>> get_filters(const vector<string>& args);
bool single_thread(const vector<string>& args);
bool multi_thread(const vector<string>& args);
bool batch(const vector<string>& args);
int get_multi_thread(const vector<string>& args);

#endif