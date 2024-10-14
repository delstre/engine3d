#include <memory>

template <typename K, typename V>
struct key_value_t {
    K* key;
    V* value;
};

template <typename K, typename V, typename Allocator = std::allocator<key_value_t<K, V>>>
class Dictionary {
public:
    Dictionary() : size(0) {}
    V& operator[](K& key);

private:
    int size = 0;
    int real_size = 0;
    key_value_t<K, V>* ptr = nullptr;

    void memalloc();
    V& keyvalue_add(K& key, V& value);
};
