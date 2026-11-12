#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "set.h"


template <typename T>
void print_line(const Set<T>& set) {
    set.print(std::cout);
    std::cout << "\n";
}

template <typename T>
void demo_all_operations(const char* type_name, const T& low, const T& high) {
    std::cout << "\n========== ALL OPERATIONS: " << type_name
        << " ==========\n";
  
    Set<T> empty_set;
    std::cout << "[ctor default]        empty = " << empty_set.empty()
        << ", size = " << empty_set.size()
        << ", print = "; print_line(empty_set);

    Set<T> a(5, low, high);
    Set<T> b(5, low, high);
    std::cout << "[ctor random]         A = "; print_line(a);
    std::cout << "[ctor random]         B = "; print_line(b);

    T raw[4] = { b[0], b[0], b[0], b[0] };
    Set<T> from_array(raw, 4);
    std::cout << "[ctor from array]     duplicates collapsed = ";
    print_line(from_array);

    Set<T> copy_of_a(a);
    std::cout << "[copy ctor]           copy == A: "
        << (copy_of_a == a) << "\n";

    Set<T> assigned;
    assigned = a;
    std::cout << "[operator=]           assigned == A: "
        << (assigned == a) << "\n";

    std::cout << "[operator[]]          A[0] = " << a[0]
        << ", A.size = " << a.size() << "\n";

    std::cout << "[contains]            A contains A[0]: "
        << a.contains(a[0])
        << ", A contains B[0]: " << a.contains(b[0]) << "\n";

    Set<T> plus_eq = a;
    plus_eq += b[0];
    std::cout << "[operator+=]          A + B[0] = "; print_line(plus_eq);

    Set<T> minus_eq = plus_eq;
    minus_eq -= b[0];
    std::cout << "[operator-=]          (A + B[0]) - B[0] = ";
    print_line(minus_eq);
    std::cout << "[operator-= check]    back to A: "
        << (minus_eq == a) << "\n";

    std::cout << "[operator+ sets]      A + B = "; print_line(a + b);

    std::cout << "[operator- sets]      A - B = "; print_line(a - b);

    std::cout << "[intersect]           A ∩ B = "; print_line(a.intersect(b));

    std::cout << "[operator+ value]     A + B[0] = "; print_line(a + b[0]);

    std::cout << "[operator- value]     A - A[0] = "; print_line(a - a[0]);

    std::cout << "[operator==]          A == copy(A): "
        << (a == copy_of_a) << "\n";
    std::cout << "[operator!=]          A != B: "
        << (a != b) << "\n";

    std::cout << "[print]               A as string: ";
    print_line(a);

    try {
        (void)a[a.size()];
    }
    catch (const std::out_of_range& ex) {
        std::cout << "[exception index]     caught: " << ex.what() << "\n";
    }
}

template <typename T>
Set<T> task1_only_in_one(const Set<T>& a, const Set<T>& b) {
    return ((a - b) + (b - a));
}

template <typename T>
bool task2_all_present(const Set<T>& a, const Set<T>& b) {
    if ((a + b)==b) return true;
    return false;
}

template <typename T>
Set<T> task3_find_duplicates(const T* values, std::size_t count) {
    Set<T> seen;
    Set<T> duplicates;
    for (std::size_t i = 0; i < count; ++i) {
        if (seen.contains(values[i])) {
            duplicates += values[i];
        }
        else {
            seen += values[i];
        }
    }
    return duplicates;
}

struct SetRelation {
    bool a_is_subset;
    bool a_is_superset;
    bool equal;
    bool disjoint;
};

template <typename T>
SetRelation task4_compare(const Set<T>& a, const Set<T>& b) {
    return SetRelation{
        task2_all_present(a, b),
        task2_all_present(b, a),
        a == b,
        a.intersect(b).empty(),
    };
}

template <typename T>
void demo_task1(const char* type_name, const T& low, const T& high) {
    std::cout << "\n----- Task 1 (" << type_name
        << "): elements only in one array -----\n";
    Set<T> a(6, low, high);
    Set<T> b(6, low, high);

    std::cout << "A = "; print_line(a);
    std::cout << "B = "; print_line(b);
    std::cout << "Only in A or only in B: ";
    print_line(task1_only_in_one(a, b));
}

template <typename T>
void demo_task2(const char* type_name, const T& low, const T& high) {
    std::cout << "\n----- Task 2 (" << type_name
        << "): all elements of A present in B -----\n";
    Set<T> a(3, low, high);
    Set<T> b = a;
    Set<T> extra(3, low, high);
    for (std::size_t i = 0; i < extra.size(); ++i) b += extra[i];

    std::cout << "A = "; print_line(a);
    std::cout << "B = "; print_line(b);
    std::cout << "All elements of A are in B: "
        << task2_all_present(a, b) << "\n";
}

template <typename T>
void demo_task3(const char* type_name, const T& low, const T& high) {
    std::cout << "\n----- Task 3 (" << type_name
        << "): duplicates in array -----\n";
    Set<T> unique_source(5, low, high);

    T raw[10];
    std::size_t count = 0;
    for (std::size_t i = 0; i < unique_source.size(); ++i) {
        raw[count++] = unique_source[i];
    }
    
    for (std::size_t i = 0; i < 2 && i < unique_source.size(); ++i) {
        raw[count++] = unique_source[i];
    }
    
    if (unique_source.size() >= 3) {
        raw[count++] = unique_source[2];
    }

    std::cout << "Input: ";
    for (std::size_t i = 0; i < count; ++i) {
        std::cout << raw[i];
        if (i + 1 < count) std::cout << " ";
    }
    std::cout << "\nDuplicates: ";
    print_line(task3_find_duplicates<T>(raw, count));
}

int main() {

    demo_all_operations<int>("int", 0, 10);
    demo_all_operations<float>("float", 0.0f, 1.0f);
    demo_all_operations<std::string>("std::string", "aa", "zzzz");
    demo_all_operations<std::pair<int, double>>("std::pair<int, double>",
        { 0, 0.0 }, { 10, 1.0 });
    
    demo_task1<int>("int", 0, 10);
    demo_task2<int>("int", 0, 10);
    demo_task3<int>("int", 0, 10);
    demo_task4<int>("int", 0, 10);

    demo_task1<float>("float", 0.0f, 1.0f);
    demo_task2<float>("float", 0.0f, 1.0f);
    demo_task3<float>("float", 0.0f, 1.0f);
    demo_task4<float>("float", 0.0f, 1.0f);

    demo_task1<std::string>("std::string", "aa", "zzzz");
    demo_task2<std::string>("std::string", "aa", "zzzz");
    demo_task3<std::string>("std::string", "aa", "zzzz");
    demo_task4<std::string>("std::string", "aa", "zzzz");

    demo_task1<std::pair<int, double>>("std::pair<int, double>",
        { 0, 0.0 }, { 10, 1.0 });
    demo_task2<std::pair<int, double>>("std::pair<int, double>",
        { 0, 0.0 }, { 10, 1.0 });
    demo_task3<std::pair<int, double>>("std::pair<int, double>",
        { 0, 0.0 }, { 10, 1.0 });
    demo_task4<std::pair<int, double>>("std::pair<int, double>",
        { 0, 0.0 }, { 10, 1.0 });

    return 0;
}
