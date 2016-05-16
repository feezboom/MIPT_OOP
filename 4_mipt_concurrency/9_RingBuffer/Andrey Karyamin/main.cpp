#include <iostream>
#include "spsc_ring_buffer.h"

using namespace std;

int main() {
    spsc_ring_buffer<int> a;
    a.call();

    return 0;
}