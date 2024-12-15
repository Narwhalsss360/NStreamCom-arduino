#include <nstreamcom.h>
#include <iostream>

int main() {
    using std::cout;
    using std::endl;
    using nstreamcom::nsize_int;
    using nstreamcom::nsize;

    constexpr const nsize_int TEST_SIZE = 0xFEEDBEEF;

    nsize size = TEST_SIZE;
    if (size != TEST_SIZE) {
        cout << "Error mismatched values, line:" << __LINE__ << endl;
        return 1;
    }

    size.encode();
    size.decode();

    if (size != TEST_SIZE) {
        cout << "Error, encode/decode error, line:" << __LINE__ << endl;
        return 1;
    }

    cout << "Success!" << endl;

    return 0;
}
