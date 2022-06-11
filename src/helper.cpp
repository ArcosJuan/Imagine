#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "helper.h"

using namespace std;


string help(const string option) {
    map<string, string> help;
    
    help["main"] =  "IMAGINE - Image Filters \n"
                    "\nUsage: main [IMG_IN] [IMG_OUT] [OPTION...] \n"
                    "\nOptions: \n"
                    "   -h | --help [COMMAND] \n"
                    "       Show command info \n\n"
                    "   -f | --filter [FILTER_NAME] [N_THREADS] [ARG...] \n"
                    "       Applies the defined filter to the input image.  "
                            "You can apply this command many times and "
                            "the filters will be executed in the requested order.\n\n"
                    "   -s | --single-thread\n"
                    "       Run all desired filters in a single thread.\n\n"
                     "   -m | --multi-thread [N_THREADS]\n"
                    "       Run all desired filters in a certain number of threads.\n\n"
                    "\nFilters:\n"
                    "   plain [N_THREADS] [HUE]\n"
                    "   blackwhite [N_THREADS]\n"
                    "   contrast [N_THREADS] [LEVEL]\n"
                    "   bright [N_THREADS] [BRIGHT]\n"
                    "   shades [N_THREADS] [SCALE]\n"
                    "   merge [N_THREADS] [IMG_2] [ALPHA]\n"
                    "   frame [N_THREADS] [WIDTH] [HUE]\n"
                    "   blur [N_THREADS] [LEVEL]\n"
                    "   zoom [N_THREADS] [LEVEL]\n"
                    "   edge [N_THREADS]\n"
                    "\nExamples: \n"
                    "   main /imgs/motor.ppm /images/output.ppm -m 6 -f contrast 50 -f frame 20 200 \n"
                    "       Will apply the contrast and frame filters in that order in 6 threads. \n\n"
                    "   main ./imgs/totoro.ppm /images/output.ppm -f zoom 6 4 -f blackwhite 2 -f bright 1 0.5 \n"
                    "        Will apply zoom with 6 threads, blackwhite with 2 and bright with one in that order. \n\n";



    help["filter"] = "Option -f: {-f|--filter} [FILTER] [N_THREADS] [ARG...]";
    help["single-thread"] = "Option -s: {-s|--single-thread}";
    help["multi-thread"] = "Option -s: {-m|--multi-thread} [N_THREADS]";

    help["plain"] = "Usage: plain [N_THREADS] [HUE]";
    help["blackwhite"] = "Usage: blackwhite [N_THREADS]";
    help["contrast"] = "Usage: contrast [N_THREADS] [LEVEL]";
    help["bright"] = "Usage: bright [N_THREADS] [BRIGHT]";
    help["shades"] = "Usage: shades [N_THREADS] [SCALE]";
    help["merge"] = "Usage: merge [N_THREADS] [IMG_2] [ALPHA]";
    help["frame"] = "Usage: frame [N_THREADS] [WIDTH] [HUE]";
    help["blur"] = "Usage: blur [N_THREADS] [LEVEL]";
    help["zoom"] = "Usage: zoom [N_THREADS] [LEVEL]";
    help["edge"] = "Usage: edge [N_THREADS]";

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

