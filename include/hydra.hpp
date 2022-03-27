#pragma once
#include <vector>
#include <string>

class Hydra;

class IteratedHydra{
    private:
    bool is_valid(int l, int r);
    bool preprocess();

    public:
    int length;
    std::vector<int> hydra_list;
    std::vector<int> chain;

    IteratedHydra();
    IteratedHydra(std::vector<int> _hydra_list);
    IteratedHydra(std::string _string);

    bool is_limit();
    IteratedHydra* expand(int n);
    IteratedHydra* predecessor();

    std::string to_string();
};