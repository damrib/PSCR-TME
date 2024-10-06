template <typename V>
class myList {

    struct Node {
        Node* next;
        V value;

        Node(Node* nx, V&& v) noexcept : next(nx), value(v) {}

    };

    Node* head = nullptr;

    public:

    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = V;
        using pointer           = V*;
        using reference         = V&;

        Iterator () : ptr(nullptr) {}

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

    /**void push_front(const V& val) {
        head = new Node(head, val);
    }**/

    void push_front(V&& val) noexcept {
        head = new Node(head, std::move(val));
    }

   myList() {
        head = nullptr;
   }

   myList(const myList& list) {
       for (auto& node: list) {
           push_front(std::move(node));
       }
   }

   myList& operator=(const myList& list) {
        while(! empty()){
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        for (auto& node: list) {
           push_front(std::move(node));
        }
        return *this;
   }

    ~myList(){
        while (! empty()){
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

    bool empty() const {
        return head == nullptr;
    }

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }

};