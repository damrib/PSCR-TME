#include <vector>

#include "myList.h"

template<typename K, typename V>
class myhashmap {

    struct Entry {
        const K Key;
        V value;

        Entry(const K key, V val) : Key(key), value(val) {}
    };

    typedef std::vector<myList<Entry>> bucket_t;

    bucket_t bucket;

    size_t hash(const K& Key) {
        return std::hash<K>()(Key);
    }

    public:

    myhashmap(int sz) {
        bucket = bucket_t(sz); // constructeur de vecteur avec taille n, n element init par defaut
    }

    const V* get(const K& key) {

        std::size_t hash_key = hash(key) % bucket.size();
        for (auto& node : bucket[hash_key]) {
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
        Entry E(key, value);
        bucket[hash_key].push_front(E);
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
        Entry E(key, 1);
        bucket[hash_key].push_front(E);
        return false;
    }

    std::size_t size() const{
        std::size_t sz = 0;
        for (int i = 0; i < bucket.size(); ++i){
            for (const auto& node : bucket[i]) {
                ++sz;
            }
        }
        return sz;
    }

    std::vector<std::pair<K, V>> extract() const {
        std::vector<std::pair<K, V>> res = std::vector<std::pair<K, V>>(0);
        for (const auto& list : bucket) {
            for (const auto& node: list) {
                res.push_back(std::pair<K, V>(node.Key, node.value));
            }
        }
        return res;
    }

};