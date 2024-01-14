#ifndef CPU_HPP
#define CPU_HPP

class CPU
{
public:
    CPU();
    ~CPU();

    int get_cycles();
    int run();

    static const int CLOCK_SPEED = 1789773; // 1.789773 MHz

private:
    int cycles;
    long total_cycles;
};

#endif