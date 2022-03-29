# include <iostream>
# include <vector>
# include <string>

# include "hydra.hpp"

using namespace std;

int main(){ 
    cout << "expand parameter:" << endl;
    int N;
    cin >> N; 
    cout << "expand with parameter n = " << N << endl;

    while(true){
        cout << "Input Iterated Hydra notation:" << endl;
        string s=string();
        cin >> s;
        IteratedHydra* hydra;
        try{
            hydra = new IteratedHydra(s);
        }
        catch(const char* error){
            cout << error << endl;
            continue;
        }
        //hydra->display_detail();

        bool is_successor = hydra->is_successor(), expandable = hydra->expandable();
        cout << "Is successor: " << is_successor << endl;
        if(is_successor)
            cout << "Predecessor: " << endl << hydra->predecessor()->to_string() << endl;
        cout << endl;
        cout << "Is Expandable: " << expandable << endl;
        if(expandable)
            cout << "Expand to: " << endl << hydra->expand(N)->to_string() << endl;
        cout << endl;
        delete hydra;
        cout << endl;
    }
    return 0;
}