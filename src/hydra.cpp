#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <exception>
#include <sstream>
#include <memory>
#include <iostream>
#include "hydra.hpp"

inline static void throw_exception_construct(){throw "Invalid Construction.";}
static int flush_number_queue(std::queue<char> &_str){
    if(_str.empty())
        throw_exception_construct();
    int i = _str.front() - '0';
    _str.pop();
    while(!_str.empty()){
        i = i*10 + _str.front() - '0';
        _str.pop();
    }
    return i;
}

IteratedHydra::IteratedHydra(){
    hydra_list = std::vector<int>();
    chain = std::vector<int>();
    length = 0;
}

IteratedHydra::IteratedHydra(std::vector<int> _hydra_list){
    hydra_list = _hydra_list;
    length = _hydra_list.size();
    chain = std::vector<int>();
    if(!preprocess())
        throw_exception_construct();
}

IteratedHydra::IteratedHydra(std::string _string){
    using namespace std;
    hydra_list = vector<int>();
    chain = vector<int>();
    int str_length = _string.size();
    stack<char> char_stack = stack<char>();
    queue<char> number_queue = queue<char>();
    int status = 0;
    /*
    status = 0: not inside component;
    status = 1: inside component;
    */
    for(string::iterator itr = _string.begin(); itr != _string.end(); itr++){
        char c = *itr;
        if(c == '('){
            if(status == 0){
                status = 1;
                continue;
            } else if(status == 1){
                throw_exception_construct();
            }
        } else if(c == ')'){
            if(status == 1){
                status = 0;
                hydra_list.push_back(flush_number_queue(number_queue));
                continue;
            } else if(status == 0){
                throw_exception_construct();
            }
        } else if(c >= '0' && c <= '9'){
            number_queue.push(c);
        } else {
            throw_exception_construct();
        }
    }
    if(status != 0)
        throw_exception_construct();
    length = hydra_list.size();
    if(!preprocess())
        throw_exception_construct();
}

bool IteratedHydra::preprocess(){
    if(hydra_list.size() == 0)
        return true;
    // valid
    bool valid = true;
    int segment = 0;
    if(hydra_list[0] != 0)
        return false;
    for(int i = 1; i < hydra_list.size(); i++){
        if(hydra_list[i]==0){
            valid = valid && is_valid(segment, i);
            segment = i;
        }
    }
    valid = valid && is_valid(segment, hydra_list.size());
    if(!valid)
        return false;
    //compute chain
    chain = std::vector<int>(hydra_list[hydra_list.size()-1]+1);
    int level = hydra_list[hydra_list.size()-1];
    for(int i = hydra_list.size()-1; i >= 0; i--){
        if(hydra_list[i] == level){
            chain[level] = i;
            level--;
        }
    }
    return true;
}

bool IteratedHydra::is_valid(int l, int r){
    if(r - l == 1)
        return true;
    int base = hydra_list[l];
    int segment = l + 1;
    bool valid = true;
    if(hydra_list[segment] != base + 1)
        return false;
    for(int i = segment + 1; i < r; i++){
        if(hydra_list[i] == base + 1){
            valid = valid && is_valid(segment, i);
            segment = i;
        }
    }
    valid = valid && is_valid(segment, r);
    return valid;
}

bool IteratedHydra::expandable(){
    return length != 0 && hydra_list[length-1] != 0;
}

std::unique_ptr<IteratedHydra> IteratedHydra::expand(int n){
    using namespace std;
    vector<int> new_hydra_list = vector<int>(hydra_list);
    new_hydra_list.pop_back();
    for(int _=0; _<n; _++){
        for(int i=chain[chain.size()-2]; i<length-1; i++)
            new_hydra_list.push_back(hydra_list[i]);
    }
    return unique_ptr<IteratedHydra>(new IteratedHydra(new_hydra_list));
}

bool IteratedHydra::is_successor(){
    return length != 0 && hydra_list[length-1] == 0;
}

std::unique_ptr<IteratedHydra> IteratedHydra::predecessor(){
    using namespace std;
    vector<int> new_hydra_list = vector<int>(hydra_list);
    new_hydra_list.pop_back();
    return unique_ptr<IteratedHydra>(new IteratedHydra(new_hydra_list));
}

std::string IteratedHydra::to_string(){
    using namespace std;
    string s = string();
    for(vector<int>::iterator itr=hydra_list.begin(); itr != hydra_list.end(); itr++){
        stringstream sstr;
        string str_number = string();
        s = s + "(";
        sstr << *itr;
        sstr >> str_number;
        s = s + str_number;
        s = s + ")";
    }
    return s;
}