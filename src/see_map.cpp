#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>

#include <cstdio>
#include <cstring>
#include <cassert>
#include <cstdint>

#include <endian.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


#ifndef linux // the code only works on linux, because there is use of some intern things of linux, that just don't work the same on windows
    #error does only work on linux
#endif

#define WAITING_TIME_MS 200 // refresh time for the print map function

#define stderror_print(text) \
    std::cerr << text << '\n'; \
    assert(!text)

#define perror_print(text) \
    perror(text); \
    assert(!text)


union Map
{
    uint64_t big = 0; 
    uint8_t little[8];

    Map(uint64_t int_64) : big(int_64) {}
    Map(void) {}
};


int open_bluetooth_serial(const char *port);
void configure_bluetooth_serial(int file_descr);
void read_map(int file_descr);
void print_map(void);

std::mutex mtx;
union Map map;
bool running = true;

int main(void)
{
    int bluetooth_serial = open_bluetooth_serial("/dev/rfcomm0");
    configure_bluetooth_serial(bluetooth_serial);

    running = true;

    std::thread t_read_map([bluetooth_serial](){read_map(bluetooth_serial);});
    std::thread t_print_map([](){print_map();});

    getchar();

    mtx.lock();
    running = false;
    mtx.unlock();

    t_read_map.join();
    t_print_map.join();
}

int open_bluetooth_serial(const char *port)
{
    int file_descr = open(port, O_RDWR | O_NOCTTY | O_SYNC);

    if(file_descr == -1)
    {
        stderror_print("can't open bluetooth serial monitor");
    }
    return file_descr;
}

void configure_bluetooth_serial(int file_descr)
{
    if(file_descr == -1)
    {
        stderror_print("didn't open bluetooth serial monitor");
    }

    struct termios term;

    // just for some compile optimalisations, and to make sure everything is NULL/0
    memset(&term, 0, sizeof(term));

    cfsetospeed(&term, B9600);
    cfsetispeed(&term, B9600);

    // 8 data bits, no parity, 1 stop bit
    term.c_cflag &= ~PARENB; // No parity
    term.c_cflag &= ~CSTOPB; // 1 stop bit
    term.c_cflag &= ~CSIZE;  // Clear data bits mask
    term.c_cflag |= CS8;     // 8 

    // Set raw input/output mode (disable software flow control)
    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;
    term.c_lflag &= ~ECHOE;
    term.c_lflag &= ~ISIG;

    term.c_iflag &= ~(IXON | IXOFF | IXANY);


    term.c_cc[VMIN] = 8;   // Wait for exactly 8 bytes
    term.c_cc[VTIME] = 1;  // Timeout in deciseconds

    term.c_cflag |= CREAD | CLOCAL; // Turn on receiver, ignore modem control lines


    if(tcsetattr(file_descr, TCSANOW, &term) != 0)
    {
        stderror_print("error setting port attributes");
    }
}

void read_map(int file_descr)
{
    while(true)
    {
        uint64_t map_l = 0;
        ssize_t bytes_read = read(file_descr, &map_l, sizeof(map_l));
        if(bytes_read != sizeof(map_l))
        {
            perror_print("read failed");
        }

        map_l = le64toh(map_l);

        {
            std::lock_guard<std::mutex> lock(mtx);
            if(!running) return;
            map = map_l;
        }
    }
}

void print_map(void)
{
    while(true)
    {
        Map map_l;
        {
            std::lock_guard<std::mutex> lock(mtx);
            if(!running) return;
            map_l = map;
        }
        std::ofstream map_file("map/position");
        if(!map_file)
        {
            perror_print("Failed to open map file");
        }

        std::cout << "\033[2J\033[1;1H"; // clearing the terminal
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j >= 0; j--)
            {
                if(map_l.little[i] & 1 << j)
                {
                    map_file << '#';
                    std::cout << '#';
                }
                else
                {
                    map_file << '.';
                    std::cout << '.';
                }
            }
            map_file << '\n';
            std::cout << '\n';
        }
        std::cout << (uint64_t)map_l.big << '\n';
        map_file.close();
        std::this_thread::sleep_for(std::chrono::milliseconds(WAITING_TIME_MS));
    }
}
