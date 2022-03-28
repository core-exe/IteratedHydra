#pragma once
#include <vector>
#include <string>
#include <memory>

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

    bool expandable();
    std::unique_ptr<IteratedHydra> expand(int n);
    bool is_successor();
    std::unique_ptr<IteratedHydra> predecessor();

    std::string to_string();
};