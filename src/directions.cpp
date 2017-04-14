#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>

#include "common.hpp"

//Forward declaration required due to CFFI's requirement to have unmangled symbols
extern "C" {
    int binary_directions(uint8_t* dst, const uint8_t* src, uint32_t width, uint32_t height);
}

/**
 * Vincinity direction algorithm
 * Sets bits in the output array based on if the surrounding pixels
 * are zero or non-zero.
 * The following bits are set based on this attribute:
 *
 *    0 1 2
 *    3   4
 *    5 6 7
 */
int binary_directions(uint8_t* dst, const uint8_t* src, uint32_t width, uint32_t height) {
    // 1st pass
    for (uint32_t x = 0; x < width; ++x) {
        for (uint32_t y = 0; y < height; ++y) {
            // Are we at the borders?
            bool x0 = (x == 0);
            bool y0 = (y == 0);
            bool xe = (x == (width - 1));
            bool ye = (y == (height - 1));
            // Get neighbors
            uint8_t north = y0 ? 0 : IMG_XY(src, x, y - 1);
            uint8_t west = x0 ? 0 : IMG_XY(src, x - 1, y);
            uint8_t northwest = (x0 || y0) ? 0 : IMG_XY(src, x - 1, y - 1);
            uint8_t northeast = (xe || y0) ? 0 : IMG_XY(src, x + 1, y - 1);
            uint8_t east = xe ? 0 : IMG_XY(src, x + 1, y);
            uint8_t south = ye ? 0 : IMG_XY(src, x, y + 1);
            uint8_t southwest = (x0 || ye) ? 0 : IMG_XY(src, x - 1, y + 1);
            uint8_t southeast = (xe || ye) ? 0 : IMG_XY(src, x + 1, y + 1);
            // Set value
            IMG_XY(dst, x, y) =
                  ((northwest == 0) ? 0 : (1 << 0))
                | ((north == 0) ? 0 : (1 << 1))
                | ((northeast == 0) ? 0 : (1 << 2))
                | ((west == 0) ? 0 : (1 << 3))
                | ((east == 0) ? 0 : (1 << 4))
                | ((southwest == 0) ? 0 : (1 << 5))
                | ((south == 0) ? 0 : (1 << 6))
                | ((southeast == 0) ? 0 : (1 << 7));
        }
    }
    return 0;
}
