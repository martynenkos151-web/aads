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
