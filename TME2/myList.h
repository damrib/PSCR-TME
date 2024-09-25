template <typename V>
class myList {

    Node* head = nullptr;

    Struct Node {
        Node* next;
        V value;
    };

    public:

    void push(const V& val) {
        Node* new_node = new Node();
        new_node->value = val;
        new_node->next = head;
        head = new_node;
    }

    void ~myList(){
        while (head){
            Node* tmp = head->next;
            delete node;
            head = tmp;
        }
    }

};