#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "led_matrix.pio.h"

#define MATRIX_GPIO_PIN 7
#define FRAME_DIMENSION 5
#define I_L 0.1

static const double no_number[5][5] = {
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0}, // apagados
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0}
};

static const double numbers[10][5][5] = {
    {
        {I_L, I_L, I_L, I_L, I_L},
        {I_L, 0.0, 0.0, 0.0, I_L},
        {I_L, 0.0, 0.0, 0.0, I_L}, // 0
        {I_L, 0.0, 0.0, 0.0, I_L},
        {I_L, I_L, I_L, I_L, I_L}
    },
    {
        {0.0, 0.0, I_L, 0.0, 0.0},
        {0.0, 0.0, I_L, 0.0, 0.0},
        {0.0, 0.0, I_L, 0.0, 0.0}, // 1
        {0.0, 0.0, I_L, 0.0, 0.0},
        {0.0, I_L, I_L, I_L, 0.0}
    },
    {
        {I_L, I_L, I_L, I_L, I_L},
        {0.0, 0.0, 0.0, 0.0, I_L},
        {I_L, I_L, I_L, I_L, I_L}, // 2
        {I_L, 0.0, 0.0, 0.0, 0.0},
        {I_L, I_L, I_L, I_L, I_L}
    },
    {
        {I_L, I_L, I_L, I_L, I_L},
        {0.0, 0.0, 0.0, 0.0, I_L},
        {0.0, I_L, I_L, I_L, I_L}, // 3
        {0.0, 0.0, 0.0, 0.0, I_L},
        {I_L, I_L, I_L, I_L, I_L}
    },
    {
        {I_L, 0.0, 0.0, 0.0, I_L},
        {I_L, 0.0, 0.0, 0.0, I_L},
        {I_L, I_L, I_L, I_L, I_L}, // 4
        {0.0, 0.0, 0.0, 0.0, I_L},
        {0.0, 0.0, 0.0, 0.0, I_L}
    },
    {
        {I_L, I_L, I_L, I_L, I_L},
        {I_L, 0.0, 0.0, 0.0, 0.0},
        {I_L, I_L, I_L, I_L, I_L}, // 5
        {0.0, 0.0, 0.0, 0.0, I_L},
        {I_L, I_L, I_L, I_L, I_L}
    },
    {
        {I_L, I_L, I_L, I_L, I_L},
        {I_L, 0.0, 0.0, 0.0, 0.0},
        {I_L, I_L, I_L, I_L, I_L}, // 6
        {I_L, 0.0, 0.0, 0.0, I_L},
        {I_L, I_L, I_L, I_L, I_L}
    },
    {
        {I_L, I_L, I_L, I_L, I_L},
        {0.0, 0.0, 0.0, 0.0, I_L},
        {0.0, 0.0, 0.0, I_L, 0.0}, // 7
        {0.0, 0.0, I_L, 0.0, 0.0},
        {0.0, 0.0, I_L, 0.0, 0.0}
    },
    {
        {I_L, I_L, I_L, I_L, I_L},
        {I_L, 0.0, 0.0, 0.0, I_L},
        {I_L, I_L, I_L, I_L, I_L}, // 8
        {I_L, 0.0, 0.0, 0.0, I_L},
        {I_L, I_L, I_L, I_L, I_L}
    },
    {
        {I_L, I_L, I_L, I_L, I_L},
        {I_L, 0.0, 0.0, 0.0, I_L},
        {I_L, I_L, I_L, I_L, I_L}, // 9
        {0.0, 0.0, 0.0, 0.0, I_L},
        {I_L, I_L, I_L, I_L, I_L}
    },
};