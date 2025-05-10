#include <iostream>
#include <vector>
#include <list>
#include <tuple>

template <typename KeyType, typename ValueType>
class Hashmap {
public:

    //virtual void insert(const KeyType& key, const ValueType& value) = 0;
    virtual ValueType operator[](const KeyType& key) const = 0;
    virtual ValueType& operator[](const KeyType& key) = 0;

    virtual bool contains(const KeyType& key) const = 0;
    
    virtual void remove(const KeyType& key) = 0;
    

};

template <typename KeyType, typename ValueType, int size=100>
class HashmapES : public Hashmap<KeyType, ValueType> {
public:
    HashmapES() : table(HASH_SIZE) {}

    ValueType& operator[](const KeyType& key) {
        size_t index = hashFunction(key);
        for (auto& tuple : table[index]) {
            if (std::get<0>(tuple) == key) {
                return std::get<1>(tuple);
            }
        }
        
        ValueType defaultValue{};
        table[index].push_back(std::make_tuple(key, defaultValue, 1));
        return std::get<1>(table[index].back());
    }

    ValueType operator[](const KeyType& key) const {
        size_t index = hashFunction(key);
        for (const auto& tuple : table[index]) {
            if (std::get<0>(tuple) == key) {
                return std::get<1>(tuple);
            }
        }
        return ValueType{};
    }

    // void insert(const KeyType& key, const ValueType& value) override {
    //     size_t index = hashFunction(key);
    //     table[index].push_back(std::make_tuple(key, value, 1));
    // }

    bool contains(const KeyType& key) const override {
        size_t index = hashFunction(key);
        for (const auto& tuple : table[index]) {
            if (std::get<0>(tuple) == key) {
                return true;
            }
        }
        return false;
    }
    
    void remove(const KeyType& key) override {
        size_t index = hashFunction(key);
        auto& l = table[index];
        l.erase(std::remove_if(l.begin(), 
                              l.end(),
                              [key](std::tuple<KeyType, ValueType, bool> t) { return std::get<0>(t) == key; }),
               l.end());
    }

private:
    static const int HASH_SIZE = size;
    std::vector<std::list<std::tuple<KeyType, ValueType, bool>>> table;

    // Función de hash básica
    size_t hashFunction(const KeyType& key) const {
        return std::hash<KeyType>{}(key) % HASH_SIZE;
    }

};

template <typename KeyType, typename ValueType, int size=100>
class HashmapDA : public Hashmap<KeyType, ValueType> {
public:
    HashmapDA() : table(HASH_SIZE) {}

    ValueType& operator[](const KeyType& key) {
        int p = 0;
        size_t index = hashFunction(key, p++);
        size_t initial = index;
        while ((std::get<2>(table[index]) != 0) && (std::get<0>(table[index]) != key)) {
            index = hashFunction(key, p++);
            if (index == initial) break;
        }
        if ((std::get<2>(table[index]) == 1) && (std::get<0>(table[index]) == key)) {
            return std::get<1>(table[index]);
        } else {
            // TODO array lleno
            table[index] = std::make_tuple(key, ValueType{}, 1);
            return std::get<1>(table[index]);
        }
    }

    ValueType operator[](const KeyType& key) const {
        int p = 0;
        size_t index = hashFunction(key, p++);
        size_t initial = index;
        while ((std::get<2>(table[index]) != 0) && (std::get<0>(table[index]) != key)) {
            index = hashFunction(key, p++);
            if (index == initial) break;
        }
        if ((std::get<2>(table[index]) == 1) && (std::get<0>(table[index]) == key)) {
            return std::get<1>(table[index]);
        } else {
            return ValueType{};
        }
    }

    bool contains(const KeyType& key) const override {
        int p = 0;
        size_t index = hashFunction(key, p++);
        size_t initial = index;
        while ((std::get<2>(table[index]) != 0) && (std::get<0>(table[index]) != key)) {
            index = hashFunction(key, p++);
            if (index == initial) break;
        }
        return (std::get<2>(table[index]) == 1) && (std::get<0>(table[index]) == key);
    }
    
    void remove(const KeyType& key) override {
        int p = 0;
        size_t index = hashFunction(key, p++);
        size_t initial = index;
        while ((std::get<2>(table[index]) != 0) && (std::get<0>(table[index]) != key)) {
            index = hashFunction(key, p++);
            if (index == initial) break;
        }
        if ((std::get<2>(table[index]) == 1) && (std::get<0>(table[index]) == key)) {
            std::get<2>(table[index]) = 2;
        }
    }

private:
    static const int HASH_SIZE = size;
    std::vector<std::tuple<KeyType, ValueType, int>> table;  // 0 - invalid, 1 - valid, 2 - deleted

    // Función de hash básica
    size_t hashFunction(const KeyType& key, int p = 0) const {
        return (std::hash<KeyType>{}(key) + p) % HASH_SIZE;
    }

    size_t hashFunctionCP(const KeyType& key, int p = 0) const {
        int c1 = 3;
        int c2 = 5;
        return (std::hash<KeyType>{}(key) + c1 * p + c2 * p * p) % HASH_SIZE;
    }

    size_t hashFunctionDH(const KeyType& key, int p = 0) const {
        return (std::hash<KeyType>{}(key) + p) % HASH_SIZE;
    }
};

int main() {
    HashmapDA<std::string, int> myMap;
    myMap["apple"] = 5;
    myMap["banana"] = 10;
    myMap["platano"] = 52;
    myMap["naranja"] = 13;

    int value;
    if (myMap.contains("banana")) {
        std::cout << "Value for 'banana': " << myMap["banana"] << std::endl;
    } else {
        std::cout << "Key 'banana' not found." << std::endl;
    }

    if (myMap.contains("pera")) {
        std::cout << "Value for 'pera': " << myMap["pera"] << std::endl;
    } else {
        std::cout << "Key 'pera' not found." << std::endl;
    }

    std::cout << "Value for 'naranja': " << myMap["naranja"] << std::endl;
    std::cout << "Value for 'granada': " << myMap["granada"] << std::endl;

    return 0;
}