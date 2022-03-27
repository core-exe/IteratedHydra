#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <exception>
#include "hydra.hpp"
using namespace std;

inline static void throw_exception_construct(){throw "Invalid Construction.";}
static int flush_number_queue(queue<char> &_str){
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
    hydra_list = vector<int>();
    length = 0;
}

IteratedHydra::IteratedHydra(vector<int> _hydra_list){
    hydra_list = _hydra_list;
    length = _hydra_list.size();
    if(!preprocess())
        throw_exception_construct();
}

IteratedHydra::IteratedHydra(string _string){
    int str_length = _string.size();
    string::iterator itr = _string.begin();
    stack<char> char_stack = stack<char>();
    queue<char> number_queue = queue<char>();
    int status = 0;
    /*
    status = 0: not inside component;
    status = 1: inside component;
    */
    for(int _=0; _<str_length; _++){
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
    // TODO
    return false;
}