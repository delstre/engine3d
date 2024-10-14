#include "dictionary.hpp"

template <typename K, typename V>
V& Dictionary<K, V>::operator[](K& key) {
    for (int i = 0; i < size; i++) {
        if (ptr[i].key == key) {
            return ptr[i].value;
        }
    }

    return keyvalue_add(key, V());
}

template <typename K, typename V>
void Dictionary<K, V>::memalloc() {
    size += 10;
    ptr = new key_value_t<K, V>[size];
}

template <typename K, typename V>
V& Dictionary<K, V>::keyvalue_add(K& key, V& value) {
    if (real_size >= size) {
        memalloc();
    }
    
    real_size++;

    ptr[real_size - 1].key = key;
    ptr[real_size - 1].value = value;
    return ptr[real_size - 1].value;
}
