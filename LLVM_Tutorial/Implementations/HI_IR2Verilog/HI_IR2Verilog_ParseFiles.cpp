#include "HI_IR2Verilog.h"
#include "HI_print.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include <ios>
#include <map>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace llvm;

void HI_IR2Verilog::ParseSchedulingfromFile()
{
    std::ifstream Schedulingfile("Scheduling_log.txt");
    std::stringstream iss;
    std::string tmp_s;
    std::string TargetName;
    int latency;
    double timing;
    while (getline(Schedulingfile, tmp_s))
    {
        tmp_s = removeExtraSpace(tmp_s);
        iss << tmp_s;
        std::string tmp_n;
        while (1)
        {
            char tmp_c;
            iss >> tmp_c;
            if (tmp_c == '$')
            {
                TargetName += tmp_n;
                tmp_n.clear();
            }
            else if (tmp_c == '{')
            {
                continue;
            }
            else if (tmp_c == '~')
            {
                std::stringstream tmpss1;
                tmpss1 << tmp_n;
                tmpss1 >> latency;
                tmp_n.clear();
            }
            else if (tmp_c == '}')
            {
                std::stringstream tmpss2;
                tmpss2 << tmp_n;
                tmpss2 >> timing;
                tmp_n.clear();
                SchedulingInfor[TargetName].latency = latency;
                SchedulingInfor[TargetName].timing = timing;
                break;
            }
            else
            {
                tmp_n += tmp_c;
            }
        }
        // std::cout << "TargetName : " << TargetName << " Latency : " << latency << " Timing : " << timing << std::endl;
        TargetName.clear();
    }
}

void HI_IR2Verilog::ParseRAMfromFile()
{
    std::ifstream Schedulingfile("Ram_log.txt");
    std::stringstream iss;
    std::string tmp_s;
    std::string TargetName;
    std::string ArrayName;
    while (getline(Schedulingfile, tmp_s))
    {
        tmp_s = removeExtraSpace(tmp_s);
        iss << tmp_s;
        std::string tmp_n;
        while (1)
        {
            char tmp_c;
            iss >> tmp_c;
            if (tmp_c == '$')
            {
                TargetName += tmp_n;
                tmp_n.clear();
            }
            else if (tmp_c == '{')
            {
                continue;
            }
            else if (tmp_c == '}')
            {
                std::stringstream tmpss2;
                tmpss2 << tmp_n;
                tmpss2 >> ArrayName;
                tmp_n.clear();
                AccessMap[TargetName] = ArrayName;
                break;
            }
            else
            {
                tmp_n += tmp_c;
            }
        }
        // std::cout << "TargetName : " << TargetName << " Latency : " << latency << " Timing : " << timing << std::endl;
        TargetName.clear();
    }
}

std::string HI_IR2Verilog::removeExtraSpace(std::string tmp_s)
{
    // remove extra space from the line
    if (tmp_s.find("=") != std::string::npos)
        findAndReplaceAll(tmp_s, "=", " = ");
    while (tmp_s.find(" ") != std::string::npos)
    {
        tmp_s = tmp_s.replace(tmp_s.find(" "), 1, "");
    }
    while (tmp_s.find("  ") != std::string::npos)
    {
        tmp_s = tmp_s.replace(tmp_s.find("  "), 2, " ");
    }
    // while (tmp_s.find("  =") != std::string::npos)
    // {
    //     tmp_s = tmp_s.replace(tmp_s.find("  ="), 3, " =");
    // }
    // while (tmp_s.find("=  ") != std::string::npos)
    // {
    //     tmp_s = tmp_s.replace(tmp_s.find("=  "), 3, "= ");
    // }
    while (tmp_s.find("\n") != std::string::npos)
        tmp_s = tmp_s.replace(tmp_s.find(" \n"), 2, "\n");
    while (tmp_s.find("\t") != std::string::npos)
        tmp_s = tmp_s.replace(tmp_s.find(" \t"), 2, "\t");

    while (tmp_s.find("\n") != std::string::npos)
        tmp_s = tmp_s.replace(tmp_s.find("\n"), 1, "");
    while (tmp_s.find("\t") != std::string::npos)
        tmp_s = tmp_s.replace(tmp_s.find("\t"), 1, "");
    return tmp_s;
}

void HI_IR2Verilog::findAndReplaceAll(std::string &data, std::string toSearch, std::string replaceStr)
{
    // Get the first occurrence
    size_t pos = data.find(toSearch);

    // Repeat till end is reached
    while (pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}

std::string HI_IR2Verilog::getStringInstruction(Instruction &I)
{
    std::string str;
    llvm::raw_string_ostream(str) << I;
    return str;
}