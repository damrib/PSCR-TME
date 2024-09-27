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

        Iterator(Node* ptr) : ptr(ptr) {}

        reference operator*() const { return ptr->value; }
        pointer operator->() { return &(ptr->value); }

        Iterator& operator++() { 
            ptr = ptr->next; 
            return *this; 
        }

        Iterator operator++(int) { 
            Iterator tmp = *this; 
            ++(*this); 
            return tmp; 
        }

        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.ptr == b.ptr;
        }

        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.ptr != b.ptr;
        }

        private:

            Node* ptr;

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