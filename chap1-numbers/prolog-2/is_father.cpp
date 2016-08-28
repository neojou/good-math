//
//  main.cpp
//  little_logic
//
//  Created by Phillip Voyle on 30/10/15.
//  Copyright © 2015 PV. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

#include "variant.h"
#include "variable_handle.h"
#include "parameter.h"
#include "text_clause.h"
#include "parser.h"
#include "compiled_clause.h"
#include "runtime.h"

void enumerate_predicate(const std::string& src_string, runtime& rt)
{
    parser p;
    text_predicate src = p.parse_predicate(src_string);

    std::map<std::string, int> first_occurrence;
    int position = 0;
    auto pred = rt.compile_predicate(src, first_occurrence, position);

    std::vector<variable_handle> arguments = rt.instantiate_predicate_arguments(pred);
    rt.prove(pred, arguments,[&]() {
        std::cout << "success:" << std::endl;
for(auto prm: first_occurrence)
        {
            std::cout << " " << prm.first << " = ";
            arguments[prm.second - 1].print(rt.get_stack());
            std::cout << std::endl;
        }
        return false; //not done yet.
    });

    rt.unstack_arguments(arguments);
}

void run_predicate(const std::string& src_string, runtime& rt)
{
    parser p;
    text_predicate src = p.parse_predicate(src_string);

    std::map<std::string, int> first_occurrence;
    int position = 0;
    auto pred = rt.compile_predicate(src, first_occurrence, position);

    std::vector<variable_handle> arguments = rt.instantiate_predicate_arguments(pred);
    rt.prove(pred, arguments,[&]() {
        std::cout << "success:" << std::endl;
for(auto prm: first_occurrence)
        {
            std::cout << " " << prm.first << " = ";
            arguments[prm.second - 1].print(rt.get_stack());
            std::cout << std::endl;
        }
        return true;
    });

    rt.unstack_arguments(arguments);
}

void execute_program(const text_program& program, runtime& rt)
{
for(auto def:program)
    {
        auto compiled = rt.compile_clause(def);
        if(compiled.goal.predicate_index == -1) //a query
        {
            if(rt.prove(compiled.requirements))
            {
                std::cout << "yes." << std::endl;
            }
            else
            {
                std::cout << "no." << std::endl;
            }
        }
        else
        {
            rt.install_clause(compiled);
        }
    }
}

int main(int argc, char* argv[])
{
    std::string program = ""
                          "isMom(mao, country).\n"
                          "isMom(good, licter).\n"
                          "isMom(good, wu).\n"
                          "isDad(stone, country).\n"
                          "isDad(country, licter).\n"
                          "isDad(country, wu).\n"
                          "isParent(A, B) :- isMom(A, B).\n"
                          "isParent(A, B) :- isDad(A, B).\n"
                          "isGrandParent(A, C) :- isParent(A, B), isParent(B, C).\n"
                          ;

    runtime rt;
    std::stringstream ssprogram(program);
    parser p;
    execute_program(p.parse_program(ssprogram), rt);
    rt.dump_program();

    std::cout << "run_predicate : " << std::endl;
    run_predicate("isGrandParent(stone, B)", rt);

    std::cout << "enumerate_predicate : " << std::endl;
    enumerate_predicate("isGrandParent(stone, C)", rt);
    return 0;
}
