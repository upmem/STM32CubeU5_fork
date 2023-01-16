/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include "board_config.h"

/* DPU_DRAM configuration */
const dpu_dram_board_config_t dpu_dram_config[NB_DPU_DRAM] =
{
    { .ss_port = GPIO_PORT_SS0,  .ss_pin = GPIO_PIN_SS0  },
    { .ss_port = GPIO_PORT_SS1,  .ss_pin = GPIO_PIN_SS1  },
    { .ss_port = GPIO_PORT_SS2,  .ss_pin = GPIO_PIN_SS2  },
    { .ss_port = GPIO_PORT_SS3,  .ss_pin = GPIO_PIN_SS3  },
    { .ss_port = GPIO_PORT_SS4,  .ss_pin = GPIO_PIN_SS4  },
    { .ss_port = GPIO_PORT_SS5,  .ss_pin = GPIO_PIN_SS5  },
    { .ss_port = GPIO_PORT_SS6,  .ss_pin = GPIO_PIN_SS6  },
    { .ss_port = GPIO_PORT_SS7,  .ss_pin = GPIO_PIN_SS7  },
    { .ss_port = GPIO_PORT_SS8,  .ss_pin = GPIO_PIN_SS8  },
    { .ss_port = GPIO_PORT_SS9,  .ss_pin = GPIO_PIN_SS9  },
    { .ss_port = GPIO_PORT_SS10, .ss_pin = GPIO_PIN_SS10 },
    { .ss_port = GPIO_PORT_SS11, .ss_pin = GPIO_PIN_SS11 },
    { .ss_port = GPIO_PORT_SS12, .ss_pin = GPIO_PIN_SS12 },
    { .ss_port = GPIO_PORT_SS13, .ss_pin = GPIO_PIN_SS13 },
    { .ss_port = GPIO_PORT_SS14, .ss_pin = GPIO_PIN_SS14 },
    { .ss_port = GPIO_PORT_SS15, .ss_pin = GPIO_PIN_SS15 }
};
