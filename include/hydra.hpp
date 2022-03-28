#pragma once
#include <vector>
#include <string>
#include <memory>

class HydraTerm;
class IteratedHydra;

class HydraTerm{
    private:
    bool _is_final;

    public:
    int index;
    IteratedHydra* hydra;

    HydraTerm();
    HydraTerm(const HydraTerm & _hydra_term);
    HydraTerm(int _index);
    HydraTerm(int _index, IteratedHydra* _hydra);
    HydraTerm(int _index, std::string _hydra_string);
    ~HydraTerm();

    bool is_valid() const;
    bool is_final() const;
    std::string to_string();
};

class IteratedHydra{
    private:
    bool _is_empty, _is_finite;
    void initialize();

    public:
    int length;
    std::vector<HydraTerm*> hydra_list;

    IteratedHydra();
    IteratedHydra(const IteratedHydra & _hydra);
    IteratedHydra(std::vector<HydraTerm*> _hydra_list);
    IteratedHydra(std::string _string);
    ~IteratedHydra();

    bool expandable();
    std::unique_ptr<IteratedHydra> expand(int n);
    bool is_successor();
    std::unique_ptr<IteratedHydra> predecessor();
    bool is_empty();
    
    bool is_finite();

    std::string to_string();
};