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

bool _has_option(const vector<string>& args, vector<string> options) {
    for (auto it = args.begin(), end = args.end(); it != end; ++it) {
        for (auto &option : options){
            if (*it == option) return true;
        }
    }
    return false;
}

bool single_thread(const vector<string>& args) {
    vector<string> options {"-s", "--single-thread"};
    return _has_option(args, options);
}

bool multi_thread(const vector<string>& args) {
    vector<string> options {"-m", "--multi-thread"};
    return _has_option(args, options);
}

int get_multi_thread(const vector<string>& args) {  
    try{
        for (auto it = args.begin(), end = args.end(); it != end; ++it) {
            if ((*it == "-m" | *it == "--multi-thread") & it + 1 != end ) return stoi(*(it+1));
        }
    }
    catch (...){
        cout << help("multi-thread") << endl;
    }
    return 1;
}

