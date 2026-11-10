#include <cmath>
#include <cstddef>
#include <ostream>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

inline int make_random(const int& low, const int& high) {
    static std::mt19937 engine{ std::random_device{}() };
    return std::uniform_int_distribution<int>(low, high)(engine);
}

inline double make_random(const double& low, const double& high) {
    static std::mt19937 engine{ std::random_device{}() };
    return std::uniform_real_distribution<double>(low, high)(engine);
}

inline float make_random(const float& low, const float& high) {
    return static_cast<float>(make_random(static_cast<double>(low),
        static_cast<double>(high)));
}

inline std::string make_random(const std::string& low, const std::string& high) {
    static std::mt19937 engine{ std::random_device{}() };
    const std::size_t len = std::uniform_int_distribution<std::size_t>(
        low.size(), high.size())(engine);
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    std::uniform_int_distribution<int> pick(0, 25);
    std::string result;
    for (std::size_t i = 0; i < len; ++i) {
        result.push_back(alphabet[pick(engine)]);
    }
    return result;
}

inline std::pair<int, double> make_random(const std::pair<int, double>& low,
    const std::pair<int, double>& high) {
    return { make_random(low.first, high.first),
            make_random(low.second, high.second) };
}

template <typename T>
class Set {
public:
    static constexpr double k_epsilon = 1e-9;
    Set() = default;
    Set(const T* values, std::size_t count) {
        for (std::size_t i = 0; i < count; ++i) {
            add_unique(values[i]);
        }
    }
    Set(std::size_t count, const T& low, const T& high) {
        for (std::size_t i = 0; i < count; ++i) {
            add_unique(make_random(low, high));
        }
    }
    Set(const Set& other) { copy_from(other); }
    
    Set& operator=(const Set& other) {
        if (this != &other) {
            release();
            copy_from(other);
        }
        return *this;
    }
    
    ~Set() { release(); }
    
    const T& operator[](std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Set: index out of range");
        }
        return data_[index];
    }
    
    std::size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    
    bool contains(const T& value) const {
        return find_index(value) != size_;
    }
    
    Set& operator+=(const T& value) {
        add_unique(value);
        return *this;
    }
    
    Set& operator-=(const T& value) {
        const std::size_t index = find_index(value);
        if (index != size_) {
            erase_at(index);
        }
        return *this;
    }
    
    Set operator+(const Set& other) const {
        Set result = *this;
        for (std::size_t i = 0; i < other.size_; ++i) {
            result.add_unique(other.data_[i]);
        }
        return result;
    }
    
    Set operator-(const Set& other) const {
        Set result;
        for (std::size_t i = 0; i < size_; ++i) {
            if (!other.contains(data_[i])) {
                result.add_unique(data_[i]);
            }
        }
        return result;
    }
    
    Set intersect(const Set& other) const {
        Set result;
        for (std::size_t i = 0; i < size_; ++i) {
            if (other.contains(data_[i])) {
                result.add_unique(data_[i]);
            }
        }
        return result;
    }
    
    Set operator+(const T& value) const {
        Set result = *this;
        result += value;
        return result;
    }
    
    Set operator-(const T& value) const {
        Set result = *this;
        result -= value;
        return result;
    }
    
    bool operator==(const Set& other) const {
        if (size_ != other.size_) return false;
        for (std::size_t i = 0; i < size_; ++i) {
            if (!values_equal(data_[i], other.data_[i])) return false;
        }
        return true;
    }
    
    bool operator!=(const Set& other) const { return !(*this == other); }
    
    std::ostream& print(std::ostream& os) const {
        os << "{";
        for (std::size_t i = 0; i < size_; ++i) {
            if (i > 0) os << ", ";
            print_value(os, data_[i]);
        }
        return os << "}";
    }

private:
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
    
    std::size_t find_index(const T& value) const {
        for (std::size_t i = 0; i < size_; ++i) {
            if (values_equal(data_[i], value)) return i;
        }
        return size_;
    }
    
    void add_unique(const T& value) {
        if (contains(value)) return;
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 4 : capacity_ * 2);
        }
        data_[size_] = value;
        ++size_;
    }
    
    void erase_at(std::size_t index) {
        for (std::size_t i = index; i + 1 < size_; ++i) {
            data_[i] = data_[i + 1];
        }
        --size_;
    }
    
    void reserve(std::size_t new_capacity) {
        if (new_capacity <= capacity_) return;
        T* new_data = new T[new_capacity];
        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
    
    void release() {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }
    
    void copy_from(const Set& other) {
        if (other.size_ == 0) return;
        data_ = new T[other.size_];
        for (std::size_t i = 0; i < other.size_; ++i) {
            data_[i] = other.data_[i];
        }
        size_ = other.size_;
        capacity_ = other.size_;
    }
    
     static bool values_equal(const T& a, const T& b) {
         if constexpr (std::is_floating_point<T>::value) {
             return std::fabs(static_cast<double>(a) - static_cast<double>(b))
                 < k_epsilon;
         }
         else {
             return a == b;
         }
     }
    
     static void print_value(std::ostream& os, const T& value) {
         if constexpr (std::is_same<T, std::pair<int, double>>::value) {
             os << "(" << value.first << ", " << value.second << ")";
         }
         else {
             os << value;
         }
     }
}
