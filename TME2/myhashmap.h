#include "myVector.h"
#include "myList.h"
#include <algorithm>

template<typename K, typename V>
class myhashmap {

    struct Entry {
        const K Key;
        V value;

        Entry(const K key, V val) : Key(key), value(val) {}
    };

    typedef myVector<myList<Entry>> bucket_t;
    typedef typename bucket_t::Iterator Vector_Iterator;
    typedef typename myList<Entry>::Iterator List_Iterator;


    bucket_t bucket;

    size_t hash(const K& Key) {
        return std::hash<K>()(Key);
    }

    public:

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

    myhashmap(const std::size_t sz) {
        bucket = bucket_t(sz);
    }

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
        return false;
    }

    std::size_t size() const{
        std::size_t sz = 0;
        for (const auto& list : bucket){
            for (const auto& node : list) {
                ++sz;
            }
        }
        return sz;
    }

    

    std::vector<std::pair<K, V>> extract() const {
        std::vector<std::pair<K, V>> res;
        res.reserve(bucket.size());
        for (auto& node : *this) {
            res.push_back(std::pair<K, V>(node.Key, node.value));
        }
        return res;
    }

    Iterator begin() const { return Iterator(bucket.begin(), bucket.end()); }
    Iterator end() const { return Iterator(bucket.end(), bucket.end()); }

};