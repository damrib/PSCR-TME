#include "myVector.h"
#include "myList.h"
#include <algorithm>

template<typename K, typename V>
class myhashmap {


    // definition de sous-classe et de type interne a la classe
    struct Entry {
        const K Key;
        V value;

        Entry(const K key, V val) : Key(key), value(val) {}
    };

    typedef myVector<myList<Entry>> bucket_t;
    typedef typename bucket_t::Iterator Vector_Iterator;
    typedef typename myList<Entry>::Iterator List_Iterator;

    // declaration des champs de la classe
    bucket_t bucket;
    size_t sz;

    // méthode privé
    size_t hash(const K& Key) {
        return std::hash<K>()(Key);
    }

    void grow(const size_t new_capacity) {
        bucket_t new_bucket(new_capacity);
        for (auto& list : bucket) {
            while ( ! list.empty() ) {
                Entry E = list.Pop();
                size_t new_hash_key = hash(E.Key) % new_capacity;
                new_bucket[new_hash_key].push_front(std::move(E));
            }
        } 
        bucket = new_bucket;
    }

    public:

    // definition de l'iterateur de la classe
    class Iterator{
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Entry;
        using pointer = Entry*;
        using reference = Entry&; 

        Vector_Iterator vect_it;
        Vector_Iterator vect_end;
        List_Iterator list_it;

        public:

        Iterator(Vector_Iterator bck_it, Vector_Iterator end) {
            vect_it = bck_it;
            vect_end = end;
            list_it = vect_it->begin();
            while (vect_it != vect_end && list_it == vect_it->end()) {
                ++vect_it;
                list_it = vect_it->begin();
            }
        }

        reference operator*() const { return *list_it; }
        pointer operator->() { return list_it->ptr; }

        Iterator& operator++() {
            if (++list_it == vect_it->end()) {
                while ( vect_it != vect_end && (list_it == vect_it->end())){
                    ++vect_it;
                    list_it = vect_it->begin();
                }
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator res = *this;
            if (++list_it == vect_it->end()) {
                while ( vect_it != vect_end && list_it == vect_it->end() ){
                    ++vect_it;
                    list_it = vect_it->begin();
                }
            }
            return res;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.vect_it == b.vect_it && a.list_it == b.list_it;
        }

        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.vect_it != b.vect_it || a.list_it != b.list_it;
        }

    };
    // methode pour obtenir des iterateurs
    Iterator begin() const { return Iterator(bucket.begin(), bucket.end()); }
    Iterator end() const { return Iterator(bucket.end(), bucket.end()); }

    // definition de Constructeur et d'operateur d'affectation
    myhashmap(const std::size_t s) {
        bucket = bucket_t(s);
        sz = 0;
        std::cout << capacity() << std::endl;
    }

    // methodes publiques
    const V* get(const K& key) {

        std::size_t hash_key = hash(key) % bucket.size();
        for (const auto& node : bucket[hash_key]) {
            if ( node.Key == key ){
                return &(node.value);
            }
        }

        return nullptr;
    }

    bool put(const K& key, const V& value) {
        std::size_t hash_key = hash(key) % bucket.size();
        for (auto& node : bucket[hash_key]) {
            if ( node.Key == key ){
                node.value = value;
                return true;
            }
        }
        
        bucket[hash_key].push_front(std::move(Entry(key, value)));
        if (++sz >= capacity() * 10) grow(capacity() * 4);
        return false;
    }

    bool incr(const K& key){
        std::size_t hash_key = hash(key) % bucket.size();
        for (auto& node : bucket[hash_key]) {
            if ( node.Key == key ){
                node.value = node.value + 1;
                return true;
            }
        }
        bucket[hash_key].push_front(std::move(Entry(key, 1)));
        if (++sz >= capacity() * 10) grow(capacity() * 4);
        return false;
    }

    std::size_t size() const{
        return sz;
    }

    std::size_t capacity() const {
        return bucket.get_capacity();
    }

    std::vector<std::pair<K, V>> extract() const {
        std::vector<std::pair<K, V>> res;
        res.reserve(bucket.size());
        for (auto& node : *this) {
            res.push_back(std::pair<K, V>(node.Key, node.value));
        }
        return res;
    }

};