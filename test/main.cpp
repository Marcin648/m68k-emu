#include <catch2/catch_session.hpp>

int main(int argc, char* argv[]){
    Catch::Session session;

    // A single run of the program benchmark takes several seconds, so the default of
    // 100 samples is impractical. Can still be overridden with --benchmark-samples.
    session.configData().benchmarkSamples = 5;

    int result = session.applyCommandLine(argc, argv);
    if(result != 0){
        return result;
    }

    return session.run();
}
