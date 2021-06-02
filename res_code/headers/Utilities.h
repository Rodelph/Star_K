#pragma once
#include <string>
#include <fstream>

static const char* extract_version(const char* full){
    std::string pl = full;
    static std::string p2;
    p2.clear();
    p2 = pl.substr(
        pl.find_last_of("/\\") + 1,
        (pl.length() - pl.find_last_of("/\\")) - (pl.length() - (pl.find_last_of(".")) + 1)
    );
    return p2.c_str();
}

#ifndef _DEBUG
static void write_log(const char* msg){
    std::ofstream logs;
    logs.open("./logs/out_log.txt", std::ofstream::app | std::ofstream::out);
    logs << msg;
    logs.close();
}
#else
static void write_log(const char* msg){

}
#endif