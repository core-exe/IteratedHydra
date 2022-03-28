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
static int str_to_int(std::string num_string){
    using namespace std;
    stringstream sstr;
    int i;
    sstr << num_string;
    sstr >> i;
    return i;
}

HydraTerm::HydraTerm(){
    index = 0;
    hydra = new IteratedHydra();
    _is_final = hydra->is_empty();
}

HydraTerm::HydraTerm(const HydraTerm & _hydra_term){
    index = _hydra_term.index;
    hydra = new IteratedHydra(*_hydra_term.hydra);
    _is_final = _hydra_term.is_final();
}

HydraTerm::HydraTerm(int _index){
    index = _index;
    hydra = new IteratedHydra();
    _is_final = hydra->is_empty();
}

HydraTerm::HydraTerm(int _index, IteratedHydra* _hydra){
    index = _index;
    hydra = _hydra;
    _is_final = hydra->is_empty();
}

HydraTerm::HydraTerm(int _index, std::string _hydra_string){
    index = _index;
    hydra = new IteratedHydra(_hydra_string);
    _is_final = hydra->is_empty();
}

HydraTerm::~HydraTerm(){
    delete hydra;
}

//TODO: is_valid for HydraTerm
bool HydraTerm::is_valid() const{
    return true;
}

bool HydraTerm::is_final() const{
    return _is_final;
}

std::string HydraTerm::to_string(){
    using namespace std;
    string str = string("("), str_idx = string();
    stringstream sstr;
    sstr << index;
    sstr >> str_idx;
    str = str + str_idx;
    if(_is_final)
        return str + ")";
    return str + "," + hydra->to_string() + ")";
}

IteratedHydra::IteratedHydra(){
    using namespace std;
    hydra_list = vector<HydraTerm*>();
    initialize();
}

IteratedHydra::IteratedHydra(const IteratedHydra & _hydra){
    hydra_list = _hydra.hydra_list;
    initialize();
}

IteratedHydra::IteratedHydra(std::vector<HydraTerm*> _hydra_list){
    using namespace std;
    hydra_list = _hydra_list;
    initialize();
}

IteratedHydra::IteratedHydra(std::string _string){
    using namespace std;
    hydra_list = vector<HydraTerm*>();
    //parse string
    int depth = 0;
    bool is_final = false;
    int idx_begin, idx_end, subterm_begin, subterm_end;
    int index;
    for(int i = 0; i < _string.size(); i++){
        char c = _string[i];
        if(c == '('){
            depth++;
            if(depth == 1){
                is_final = true;
                idx_begin = i + 1;
            }
        } else if(c == ')') {
            depth--;
            if(depth == 0){
                if(is_final){
                    idx_end = i;
                    hydra_list.push_back(new HydraTerm(str_to_int(_string.substr(idx_begin, idx_end-idx_begin))));
                } else {
                    subterm_end = i;
                    hydra_list.push_back(new HydraTerm(index, _string.substr(subterm_begin, subterm_end-subterm_begin)));
                }
            }
        } else if(c == ',') {
            if(depth == 1){
                is_final = false;
                idx_end = i;
                index = str_to_int(_string.substr(idx_begin, idx_end-idx_begin));
                subterm_begin = i + 1;
            }
        } else if(c >= '0' && c <= '9') {
            continue;
        } else if(c == ' ') {
            continue;
        } else 
            throw_exception_construct();
    }
    initialize();
}

IteratedHydra::~IteratedHydra(){
    using namespace std;
    for(vector<HydraTerm*>::iterator itr = hydra_list.begin(); itr != hydra_list.end(); itr++)
        delete *itr;
}

void IteratedHydra::initialize(){
    //TODO initializing the notation
}

bool IteratedHydra::is_empty(){
    return hydra_list.size() == 0;
}

std::string IteratedHydra::to_string(){
    using namespace std;
    string str;
    for(vector<HydraTerm*>::iterator itr = hydra_list.begin(); itr != hydra_list.end(); itr++)
        str = str + (*itr)->to_string();
    return str;
}