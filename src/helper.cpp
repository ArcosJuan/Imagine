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

vector<vector<string>> get_filters(const vector<string>& args){
    vector<vector<string>>filters;
    for (auto it = args.begin(); it != args.end(); ++it) {
        if (*it == "-f" | *it == "--filter"){
            vector<string> filter;
            for (auto jt = it+1; jt != args.end(); ++jt){
                if((*jt).find('-') < (*jt).length()) break;
                else filter.push_back(*jt);
            }
            if (filter.size() > 0) filters.push_back(filter);
            else cout << help("filter") << endl;
        }
    }
    return filters;
}

