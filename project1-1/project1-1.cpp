#include <cstdio>
#include <cstdint>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#define INFILE  "input.txt"
#define OUTFILE "output.txt"

using namespace std;

int N; 
uint64_t in[100000];

struct FIO {
    size_t sz, idx;
    int ifd, ofd;
    unsigned char * buf;

    FIO(int ifd, int ofd) : idx(0) {
        struct stat status;
        fstat(ifd, &status);
        sz = status.st_size;
        buf = static_cast<unsigned char *>(
            mmap(
                NULL,
                sz,
                PROT_READ,
                MAP_FILE | MAP_PRIVATE,
                ifd,
                0
            )
        );
    }

    inline bool is_whitespace() {
        return
            buf[idx] == '\n' || buf[idx] == '\r' || buf[idx] == '\t' ||
            buf[idx] == '\f' || buf[idx] == '\v' || buf[idx] == ' ';
    }

    inline void consume_whitespace() {
        while (idx < sz && is_whitespace()) idx++;
    }

    inline uint64_t read_uint64() {
        uint64_t ret = 0;
        int neg = 0;
        consume_whitespace();
        if (buf[idx] == '-') {
            neg = 1;
            idx++;
        }
        while (idx < sz && !is_whitespace()) {
            ret = ret * 10 + buf[idx++] - '0';
        }
        return neg ? -ret : ret;
    }
};

int main(int argc, char * argv[]) {
    FIO fio(
        open(INFILE, O_RDONLY),
        open(OUTFILE, O_WRONLY)
    );

    int N = static_cast<int>(fio.read_uint64());
    for (int i = 0; i < N; i++)
        in[i] = fio.read_uint64();
    
    cout << N << endl;
    for (int i = 0; i < N; i++)
        cout << in[i] << " ";
    cout << endl;

    return 0;
}