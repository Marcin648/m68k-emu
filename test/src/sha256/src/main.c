#include "sha256.h"

struct {
    BYTE hash[SHA256_BLOCK_SIZE];
    int done;
} result;

static const BYTE text[] =
    "The quick brown fox jumps over the lazy dog. "
    "Pack my box with five dozen liquor jugs. "
    "How vexingly quick daft zebras jump! "
    "Sphinx of black quartz, judge my vow. "
    "The five boxing wizards jump quickly. "
    "Bright vixens jump; dozy fowl quack. "
    "Jackdaws love my big sphinx of quartz. "
    "Waltz, bad nymph, for quick jigs vex.";

int main(void) {
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, text, sizeof(text) - 1);
    sha256_final(&ctx, result.hash);
    result.done = 1;
    for (;;);
}
