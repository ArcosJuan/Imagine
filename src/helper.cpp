#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "helper.h"

using namespace std;


string help(const string option) {
    map<string, string> help;
    return help[option];
}

string help(const vector<string>& args) {
    for (auto it = args.begin(), end = args.end(); it != end; ++it) {
        if (*it == "-h" || *it == "--help"){
            auto context = it + 1;

            if (context == end) return help("main");
            else if (*context == "-f"  | *context == "--filter") return help("filter");
            else return help(*context);
        }
    }
    return "";
}

