#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "helper.h"

using namespace std;


string help(const string option) {
    map<string, string> help;

    help["usage"] = "Usage: imagine [IMG_IN] [IMG_OUT] [OPTION...] \n";

    help["usage-help"]          = "[-h | --help] [COMMAND] \n";
    help["usage-filter"]        = "[-f | --filter] [FILTER_NAME] [N_THREADS] [ARG...]\n";
    help["usage-single-thread"] = "[-s | --single-thread]\n";
    help["usage-multi-thread"]  = "[-m | --multi-thread] [N_THREADS]\n";
    help["usage-batch"]         = "[-b | --batch]\n";

    help["args-blackwhite"] = "blackwhite [N_THREADS]\n";
    help["args-contrast"]   = "contrast [N_THREADS] [LEVEL]\n";
    help["args-plain"]      = "plain [N_THREADS] [HUE]\n";
    help["args-bright"]     = "bright [N_THREADS] [BRIGHT]\n";
    help["args-shades"]     = "shades [N_THREADS] [SCALE]\n";
    help["args-merge"]      = "merge [N_THREADS] [IMG_2] [ALPHA]\n";
    help["args-frame"]      = "frame [N_THREADS] [WIDTH] [HUE]\n";
    help["args-blur"]       = "blur [N_THREADS] [LEVEL]\n";
    help["args-zoom"]       = "zoom [N_THREADS] [LEVEL]\n";
    help["args-edge"]       = "edge [N_THREADS]\n";


    help["main"] =  "IMAGINE - Image Filters \n \n"
                    + help["usage"] +

                    "\nOptions: \n"
                    "   " + help["usage-help"] +
                    "       Show command info. \n\n"

                    "   " + help["usage-filter"] + 
                    "       Applies the defined filter to the input image.  "
                            "You can apply this command many times and "
                            "the filters will be executed in the requested order.\n\n"

                    "   " + help["usage-single-thread"] +
                    "       Run all desired filters in a single thread.\n\n"

                    "   " + help["usage-multi-thread"] + 
                    "       Run all desired filters in a certain number of threads.\n\n"

                    "   " + help["usage-batch"] + 
                    "       Apply filters to a batch of images in a folder (IMG_IN) and" 
                            "save them in another directory (IMG_OUT).\n\n"

                    "\nFilters:\n"
                    "   " + help["args-plain"] +
                    "   " + help["args-blackwhite"] + 
                    "   " + help["args-contrast"] + 
                    "   " + help["args-bright"] + 
                    "   " + help["args-shades"] + 
                    "   " + help["args-merge"] + 
                    "   " + help["args-frame"] + 
                    "   " + help["args-blur"] + 
                    "   " + help["args-zoom"] + 
                    "   " + help["args-edge"] + 

                    "\nExamples: \n"
                    "   imagine /imgs/motor.ppm /images/output.ppm -m 6 -f contrast 50 -f frame 20 200 \n"
                    "       Will apply the contrast and frame filters in that order in 6 threads. \n\n"

                    "   imagine ./imgs/totoro.ppm /images/output.ppm -f zoom 6 4 -f blackwhite 2 -f bright 1 0.5 \n"
                    "        Will apply zoom with 6 threads, blackwhite with 2 and bright with one in that order. \n\n"

                    "   imagine ./imgs/ /images/out/ -b -m 2 -f zoom 4 -f blur 10\n"
                    "        Will apply zoom and blur to all the images in the './imgs' directory with 2 threads. \n\n";


    return help[option];
}

string help(const vector<string>& args) {
    for (auto it = args.begin(), end = args.end(); it != end; ++it) {
        if (*it == "-h" || *it == "--help"){
            auto context = it + 1;

            if (context == end) return help("main");
            else if (!help(*context).empty()) return help(*context);
            else if (!help("args-" + *context).empty())return help("args-" + *context);
            else if (!help("usage-" + *context).empty()) return help("usage-" + *context);
            else return help("main");
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
            else cout << help("usage-filter") << endl;
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

bool batch(const vector<string>& args) {
    vector<string> options {"-b", "--batch"};
    return _has_option(args, options);
}

int get_multi_thread(const vector<string>& args) {  
    try{
        for (auto it = args.begin(), end = args.end(); it != end; ++it) {
            if ((*it == "-m" | *it == "--multi-thread") & it + 1 != end ) return stoi(*(it+1));
        }
    }
    catch (...){
        cout << help("usage-multi-thread") << endl;
    }
    return 1;
}

