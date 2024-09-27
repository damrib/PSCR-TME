template<typename V>
class myVector{

    std::size_t sz, capacity;
    V* array;

    public:

    struct Iterator{
        using iterator_category = std::input_iterator_tag;
        using diffenrence_type  = std::ptrdiff_t;
        using value_type        = V;
        using pointer           = V*;
        using reference         = V&;

        Iterator(pointer ptr) : ptr(ptr) {}

        const reference operator*() const { return *ptr; }
        const pointer operator->() { return ptr; }

        Iterator& operator++() const { ++ptr; return *this; }
        Iterator operator++() { 
            Iterator tmp = *this; 
            ++ptr; 
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.ptr == a.ptr;
        }

        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.ptr != b.ptr;
        }

        private:

            pointer ptr;
    };

    myVector() : sz(0), capacity(4) {
        array = new V[capacity];
    }

    myVector(const std::size_t cap) : sz(cap), capacity(cap) {
        array = new V[cap];
    }

    myVector(const myVector& vec) {
        sz = vec.sz;
        capacity = vec.capacity;
        array = new V[capacity];
        for (int i = 0; i < sz; ++i) {
            array[i] = vec[i];
        }
    }

    myVector& operator=(const myVector& vec) {

        delete[] array;
        
        sz = vec.sz;
        capacity = vec.capacity;
        array = new V[capacity];
        for (int i = 0; i < sz; ++i) {
            array[i] = vec[i];
        }

        return *this;
    }

    ~myVector() {
        delete[] array;
    }

    V& operator[](const size_t index)       { return array[index]; }
    V  operator[](const size_t index) const { return array[index]; }

    size_t size() const {
        return sz;
    }


    Iterator begin()  const { return Iterator(&array[0]);  }
    Iterator cbegin() const { return Iterator(&array[0]);  }
    Iterator end()  const { return Iterator(&array[sz]); }
    Iterator cend() const { return Iterator(&array[sz]); }

};