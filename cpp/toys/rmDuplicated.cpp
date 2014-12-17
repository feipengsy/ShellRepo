#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

void rm_duplicated(vector<int>& coll);
void rm_duplicated_stable(vector<int>& coll);
void print_out(const vector<int>& coll);

int main() {

    int coll[12] = {7,6,8,1,5,7,2,5,1,5,4,9};
    vector<int> coll_1(coll, coll + 12), coll_2(coll, coll + 12);

    cout << "Before removing: " << endl;
    print_out(coll_1);

    rm_duplicated(coll_1);
    rm_duplicated_stable(coll_2);

    cout << "Remove duplicated elements(unstable): " << endl;
    print_out(coll_1);
    cout << "Remove duplicated elements(stable): " << endl;
    print_out(coll_2);

    return 0;
}

void rm_duplicated(vector<int>& coll) 
{
    sort(coll.begin(), coll.end());
    coll.erase(unique(coll.begin(), coll.end()), coll.end());
}

void rm_duplicated_stable(vector<int>& coll)
{
    vector<int>::iterator it, ret, beg = coll.begin();
    for (it = beg + 1; it != coll.end();) {
        if ( std::find(beg, it, *it) != it ) {
            it = coll.erase(it);
        }
        else {
            ++it;
        }
    }
}

void print_out(const vector<int>& coll)
{
    vector<int>::const_iterator it;
    for (it = coll.begin(); it != coll.end(); ++it) {
        cout << *it << "  ";
    }
    cout << std::endl;
}
