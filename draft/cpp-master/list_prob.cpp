/** 
 * Complete implementation of a list
 */
#include <iostream>

/// Entry of a list
class ListEntry
{
    /// Value stored in the list
    int m_v;
    /// Pointer to next list element or nullptr
    ListEntry *m_next;
public:
    /// Constructor
    ListEntry(int v, ListEntry *next);
    /// Returns next element in the list
    ListEntry *getNext() { return m_next; }
    /// Sets next element in the list
    void setNext(ListEntry *next) { m_next = next; }
    /// Rerurns the value
    int getValue() { return m_v; }
};

/// A linked list
class List
{

    /// Head of the list
    ListEntry *m_head;

public:
    /// Constructor
    List();
    /// Destructor
    ~List();

    /// Insert an element into the list
    void insert(int v);

    using int_bool_pair = std::pair<bool, int>;

    /// Remove an element at given index \p idx
    /// \return a pair (true, value) if value was removed
    /// \return a pair (false, 0) if the index is not in the list
    int_bool_pair remove_at_index(unsigned idx);
    
    /// Returns true if the list is empty
    bool is_empty();

    /// Returns the head of the list
    ListEntry *get_head();

    /// Prints the list to an output stream
    void print(std::ostream &out);
    
    /// Helper method to print the list recursively from a given \p entry
    void print_rec(std::ostream &out, ListEntry &entry);
};

/// Overload << operator to print the list
std::ostream &operator<<(std::ostream &out, List &v);


int main(void)
{
    std::cout << "Hello world!\n";

    int r;
    std::unique_ptr<List> l(new List());
    l->insert(5);
    l->insert(4);
    l->insert(3);

    // l->print(std::cerr);
    // std::cerr << "\n";
    std::cerr << *l << "\n";

    for (unsigned i = 0; i < 3; i++)
    {
        auto res = l->remove_at_index(1);
        if (res.first)
        {
            std::cerr << "Removed element: " << res.second << "\n";
        }
        else
        {
            std::cerr << "Index too large: idx=" << i << "\n";
        }
    }
    return 0;
}
