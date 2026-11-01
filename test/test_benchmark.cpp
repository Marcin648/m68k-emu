#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "m68k.hpp"

#include <chrono>
#include <iostream>
#include <vector>

using namespace M68K;

double timeInterval(void) {
    static auto last_time = std::chrono::high_resolution_clock::now();
    auto time_now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dt = time_now - last_time;
    last_time = time_now;
    return dt.count() / 1000.0;
}

TEST_CASE("Program: benchmark", "[.][program][benchmark]") {
    size_t n = 0;
    double dtime = 0.0;

    BENCHMARK_ADVANCED("benchmark")(Catch::Benchmark::Chronometer meter) {
        std::vector<CPU> cpus(meter.runs());
        for (auto& cpu : cpus) {
            cpu.loadELF(M68K_TEST_BINARY_DIR "/benchmark.elf");
        }

        meter.measure([&](int i) {
            CPU& cpu = cpus[i];

            n = 0;
            timeInterval();
            while (cpu.state.registers.get(REG_PC, SIZE_LONG) != 0x101A4) {
                cpu.step();
                n++;
            }
            dtime = timeInterval();
            return n;
        });
    };

    std::cout << "\nExecute " << n << " instructions in " << dtime << " sec." << std::endl;
    std::cout << "Frequency: " << ((double)n / dtime) / 1000.0 << " kHz" << std::endl;
}
