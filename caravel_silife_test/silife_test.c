/*
 * SPDX-FileCopyrightText: 2020 Efabless Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "verilog/dv/caravel/defs.h"

#define PROJECT_ID              2

#define silife_reg_ctrl          (*(volatile uint32_t*)0x30000000)

#define SILIFE_CTRL_ENABLE			(1 << 0)
#define SILIFE_CTRL_STEP				(1 << 1)
#define SILIFE_CTRL_MAX7219_EN	(1 << 2)

void main()
{
	  // MAX7219 Output
		reg_mprj_io_8  = GPIO_MODE_USER_STD_OUTPUT;
		reg_mprj_io_9  = GPIO_MODE_USER_STD_OUTPUT;
		reg_mprj_io_10 = GPIO_MODE_USER_STD_OUTPUT;
		reg_mprj_io_11 = GPIO_MODE_USER_STD_OUTPUT;
		reg_mprj_io_12 = GPIO_MODE_USER_STD_OUTPUT;
		reg_mprj_io_13 = GPIO_MODE_USER_STD_OUTPUT;
		reg_mprj_io_14 = GPIO_MODE_USER_STD_OUTPUT;
		reg_mprj_io_15 = GPIO_MODE_USER_STD_OUTPUT;

    /* Apply configuration */
    reg_mprj_xfer = 1;
    while (reg_mprj_xfer == 1);

    /* Activate the project */
    reg_la0_iena = 0; // input enable off
    reg_la0_oenb = 0; // output enable on
    reg_la0_data = 1 << PROJECT_ID;

		/* Reset SiLife */
    reg_la1_iena = 0;
    reg_la1_oenb = 0;
    reg_la1_data |= 1;
    reg_la1_data &= ~1;

		/* Enable MAX7219 Output */
		silife_reg_ctrl = SILIFE_CTRL_MAX7219_EN;
}
