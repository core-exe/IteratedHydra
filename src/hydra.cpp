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
static HydraTerm zero_term = HydraTerm();

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

bool HydraTerm::is_valid() const{
    return hydra->is_finite();
}

bool HydraTerm::is_final() const{
    return _is_final;
}

bool HydraTerm::operator==(const HydraTerm& other){
    return index == other.index && *hydra == *(other.hydra);
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
    vector<HydraTerm*> _hydra_list = vector<HydraTerm*>();
    initialize(_hydra_list);
}

IteratedHydra::IteratedHydra(const IteratedHydra & _hydra){
    using namespace std;
    length = _hydra.length;
    hydra_list = _hydra.hydra_list;
    root_0 = _hydra.root_0;
    root_1 = _hydra.root_1;
    chain_0 = _hydra.chain_0;
    chain_1 = _hydra.chain_1;
    transition_1 = _hydra.transition_1;
    _is_empty = _hydra.is_empty();
    _is_finite = _hydra.is_finite();
}

IteratedHydra::IteratedHydra(std::vector<HydraTerm*> _hydra_list){
    using namespace std;
    initialize(_hydra_list);
}

IteratedHydra::IteratedHydra(std::string _string){
    using namespace std;
    vector<HydraTerm*> _hydra_list = vector<HydraTerm*>();
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
                    _hydra_list.push_back(new HydraTerm(str_to_int(_string.substr(idx_begin, idx_end-idx_begin))));
                } else {
                    subterm_end = i;
                    _hydra_list.push_back(new HydraTerm(index, _string.substr(subterm_begin, subterm_end-subterm_begin)));
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
    initialize(_hydra_list);
}

IteratedHydra::~IteratedHydra(){
    using namespace std;
    for(int i=0; i<length; i++){
        delete hydra_list[i];
    }
}

void IteratedHydra::push_term(HydraTerm* hydra_term){
    using namespace std;
    hydra_list.push_back(hydra_term);
    if(!hydra_term->is_valid())
        throw_exception_construct();
    
    // level 0 (index) process
    if(hydra_term->index == 0){
        root_0.push_back(-1);
        chain_0=vector<int>({length});
    }else if(hydra_term->index == chain_0.size()){
        root_0.push_back(chain_0[chain_0.size()-1]);
        chain_0.push_back(length);
    }else if(hydra_term->index < chain_0.size()){
        while(hydra_term->index < chain_0.size())
            chain_0.pop_back();
        root_0.push_back(chain_0[chain_0.size()-1]);
        chain_0.push_back(length);
    }else{
        throw_exception_construct();
    }

    // level 1 (term) process
    if(hydra_term->hydra->is_empty()){
        root_1.push_back(-1);
        chain_1=vector<int>({length});
        transition_1.push_back(&zero_term); // root transition
    }else{
        bool valid = true;
        chain_1=vector<int>(hydra_term->hydra->length+1);
        *chain_1.rbegin() = length;
        int current_length = hydra_term->hydra->length-1;
        for(int i=chain_0.size()-2; i>=0; i--){
            if(current_length<0)
                break;
            if(hydra_list[chain_0[i]]->hydra->length == current_length && hydra_list[chain_0[i]]->hydra->is_prefix_of(*hydra_term->hydra)){
                chain_1[current_length] = chain_0[i];
                current_length--;
            }
        }
        root_1.push_back(chain_1[hydra_term->hydra->length-1]);
        transition_1.push_back(*(hydra_term->hydra->hydra_list.rbegin()));
        if(current_length!=-1)
            throw_exception_construct();
    }
    length++;
}

void IteratedHydra::initialize(std::vector<HydraTerm*> _hydra_list){
    using namespace std;
    length = 0;
    root_0 = vector<int>();
    root_1 = vector<int>();
    chain_0 = vector<int>();
    chain_1 = vector<int>();
    transition_1 = vector<HydraTerm*>();
    _is_finite = true;
    _is_empty = (_hydra_list.size() == 0);
    for(int i = 0; i < _hydra_list.size(); i++){
        push_term(_hydra_list[i]);
        if(_hydra_list[i]->index > 0)
            _is_finite = false;
    }
}

bool IteratedHydra::expandable(){
    return length != 0 && (*hydra_list.rbegin())->index != 0;
}

IteratedHydra* IteratedHydra::expand(int n){
    using namespace std;
    IteratedHydra* new_hydra_base = new IteratedHydra(vector<HydraTerm*>(hydra_list.begin(), hydra_list.end()-1));
    if(hydra_list[length-1]->is_final()){
        int repetition_pivot = *(chain_0.rbegin()+1);
        for(int i=1; i<=n; i++){
            for(int j=repetition_pivot; j<length-1; j++){
                new_hydra_base->push_term(new HydraTerm(*hydra_list[j]));
            }
        }
    }else if(hydra_list[length-1]->hydra->is_successor()){
        int repetition_pivot = *(chain_1.rbegin()+1);
        int delta = hydra_list[length-1]->index - hydra_list[repetition_pivot]->index;
        for(int i=1; i<=n; i++){
            for(int j=repetition_pivot; j<length-1; j++){
                HydraTerm* new_term = new HydraTerm(hydra_list[j]->index+delta*i, new IteratedHydra(*(hydra_list[j]->hydra->predecessor())));
                new_hydra_base->push_term(new_term);
            }
        }
    }else{
        // should not reach here.
    }
    return new_hydra_base;
}


bool IteratedHydra::is_successor(){
    return length != 0 && (*hydra_list.rbegin())->index == 0;
}

IteratedHydra* IteratedHydra::predecessor(){
    using namespace std;
    return new IteratedHydra(vector<HydraTerm*>(hydra_list.begin(), hydra_list.end()-1));
}

bool IteratedHydra::is_empty() const{
    return hydra_list.size() == 0;
}

bool IteratedHydra::is_finite() const{
    return _is_finite;
}

bool IteratedHydra::is_prefix_of(const IteratedHydra& other){
    using namespace std;
    if(length > other.length)
        return false;
    for(int i = 0; i < hydra_list.size(); i++){
        if(!(*hydra_list[i] == *other.hydra_list[i]))
            return false;
    }
    return true;
}

bool IteratedHydra::operator==(const IteratedHydra& other){
    if(length != other.length)
        return false;
    for(int i = 0; i < hydra_list.size(); i++){
        if(!(hydra_list[i] == other.hydra_list[i]))
            return false;
    }
    return true;
}

std::string IteratedHydra::to_string() const{
    using namespace std;
    string str;
    for(vector<HydraTerm*>::const_iterator itr = hydra_list.cbegin(); itr != hydra_list.cend(); itr++)
        str = str + (*itr)->to_string();
    return str;
}

void IteratedHydra::display_detail(int depth){
    using namespace std;
    string indent = string();
    for(int _=0; _<depth; _++)
        indent.push_back('\t');
    cout << "Detailed infomation for:" << endl;
    cout << to_string() << endl;

    cout << "Root 0:" << endl;
    for(int i=0; i<length; i++)
        cout << root_0[i] << " ";
    cout << endl;

    cout << "Root 1:" << endl;
    for(int i=0; i<length; i++)
        cout << root_1[i] << " ";
    cout << endl;

    cout << "Chain 0:" << endl;
    for(int i=0; i<chain_0.size(); i++)
        cout << chain_0[i] << " ";
    cout << endl;

    cout << "Chain 1:" << endl;
    for(int i=0; i<chain_1.size(); i++)
        cout << chain_1[i] << " ";
    cout << endl;

    cout << "Transition 1:" << endl;
    for(int i=0; i<length; i++)
        cout << transition_1[i]->to_string() << " ";
    cout << endl;
}