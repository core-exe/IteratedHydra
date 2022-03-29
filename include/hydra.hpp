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

    bool operator==(const HydraTerm& other);

    std::string to_string();
};

class IteratedHydra{
    private:
    bool _is_empty, _is_finite;
    void initialize(std::vector<HydraTerm*> _hydra_list);

    public:
    int length;
    std::vector<HydraTerm*> hydra_list;
    std::vector<int> root_0, root_1;
    std::vector<int> chain_0, chain_1;
    std::vector<HydraTerm*> transition_1;

    IteratedHydra();
    IteratedHydra(const IteratedHydra & _hydra);
    IteratedHydra(std::vector<HydraTerm*> _hydra_list);
    IteratedHydra(std::string _string);
    ~IteratedHydra();

    void push_term(HydraTerm* hydra_term);

    bool expandable();
    IteratedHydra* expand(int n);
    bool is_successor();
    IteratedHydra* predecessor();
    bool is_empty() const;
    bool is_finite() const;

    bool is_prefix_of(const IteratedHydra& other);
    bool operator==(const IteratedHydra& other);

    std::string to_string() const;
    void display_detail(int depth = 0);
};

