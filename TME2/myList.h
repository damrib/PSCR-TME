template <typename V>
class myList {

    struct Node {
        Node* next;
        V value;

        Node(Node* next, V value) : next(next), value(value) {}
    };

    Node* head = nullptr;

    public:

    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = V;
        using pointer           = V*;
        using reference         = V&;

        Iterator(Node* ptr) : m_ptr(ptr) {}

        reference operator*() const { return m_ptr->value; }
        pointer operator->() { return &(m_ptr->value); }

        Iterator& operator++() { 
            m_ptr = m_ptr->next; 
            return *this; 
        }

        Iterator operator++(int) { 
            Iterator tmp = *this; 
            ++(*this); 
            return tmp; 
        }

        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.m_ptr == b.m_ptr;
        }

        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.m_ptr != b.m_ptr;
        }

        private:

            Node* m_ptr;

    };

    void push_front(const V& val) {
        Node* new_node = new Node(head, val);
        head = new_node;
    }

    ~myList(){
        while (head){
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    V Pop() {
        V res = head->value;

        Node* tmp = head;
        head = head->next;

        delete tmp;
        return res; 
    }

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }

};