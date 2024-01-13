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

#include <defs.h>
#include <stub.c>

// change to your project's ID - ask Matt
#define PROJECT_ID 2

#define reg_silife_ctrl         (*(volatile uint32_t*)0x30000000)
#define reg_silife_config       (*(volatile uint32_t*)0x30000004)
#define reg_max7219_ctrl    		(*(volatile uint32_t*)0x30000010)
#define reg_max7219_config 			(*(volatile uint32_t*)0x30000014)
#define reg_max7219_brightness  (*(volatile uint32_t*)0x30000018)
#define reg_silife_dbg_selfaddr (*(volatile uint32_t*)0x30000020)
#define reg_silife_trng         (*(volatile uint32_t*)0x30000030)
#define reg_silife_vga          (*(volatile uint32_t*)0x30000040)

#define grid_mem				((volatile uint32_t*)0x30001000)

// reg_silife_ctrl bits:
#define CTRL_ENABLE             (1 << 0)
#define CTRL_PULSE              (1 << 1)

// reg_silife_config bits:
#define CONFIG_GRID_WRAP 				(1 << 0)
#define CONFIG_SYNC_EN_N        (1 << 4)
#define CONFIG_SYNC_EN_E        (1 << 5)
#define CONFIG_SYNC_EN_S        (1 << 6)
#define CONFIG_SYNC_EN_W        (1 << 7)

// reg_max7219_ctrl bits:
#define MAX7219_ENABLE					(1 << 0)
#define MAX7219_PAUSE						(1 << 1)
#define MAX7219_FRAME						(1 << 2)
#define MAX7219_BUSY						(1 << 3)

// reg_max7219_config bits:
#define MAX7219_REVERSE_COLUMNS (1 << 0)
#define MAX7219_SERPENTINE		  (1 << 1)

// reg_max7219_brightness bits:
#define MAX7219_BRIGHTNESS_MASK (0xf)

// reg_silife_trng bits:
#define TRNG_PULSE							(1 << 0)
#define TRNG_BUSY								(1 << 1)

// reg_silife_vga bits:
#define VGA_ENABLE							(1 << 0)


void main()
{
	/* 
	IO Control Registers
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 3-bits | 1-bit | 1-bit | 1-bit  | 1-bit  | 1-bit | 1-bit   | 1-bit   | 1-bit | 1-bit | 1-bit   |

	Output: 0000_0110_0000_1110  (0x1808) = GPIO_MODE_USER_STD_OUTPUT
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 110    | 0     | 0     | 0      | 0      | 0     | 0       | 1       | 0     | 0     | 0       |
	
	 
	Input: 0000_0001_0000_1111 (0x0402) = GPIO_MODE_USER_STD_INPUT_NOPULL
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 001    | 0     | 0     | 0      | 0      | 0     | 0       | 0       | 0     | 1     | 0       |

	*/

	// MAX7219 outputs
	reg_mprj_io_8 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_9 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_10 = GPIO_MODE_USER_STD_OUTPUT;

	// SPI_CTRL I/Os
	reg_mprj_io_22 = GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_23 = GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_24 = GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_25 = GPIO_MODE_USER_STD_OUTPUT;

	// VGA outputs
	reg_mprj_io_26 = GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_27 = GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_28 = GPIO_MODE_USER_STD_OUTPUT;

	/* Apply configuration */
	reg_mprj_xfer = 1;
	while (reg_mprj_xfer == 1);

	// activate the project by setting the 0th bit of 1st bank of LA
	reg_la0_iena = 0; // input enable off
	reg_la0_oenb = 0xFFFFFFFF; // enable all of bank0 logic analyser outputs (ignore the name, 1 is on, 0 off)
	reg_la0_data |= (1 << PROJECT_ID); // enable the project

	// reset design with 0bit of 2nd bank of LA
	reg_la1_oenb = 1; // enable
	reg_la1_iena = 0;
	reg_la1_data = 1;
	reg_la1_data = 0;

	// Enable the wishbone interface
	reg_wb_enable = 1;

	// Generate a random pattern
	// reg_silife_trng = TRNG_PULSE;

	// Enable VGA output
	reg_silife_vga = VGA_ENABLE;

	// Initialize the grid with a predefined pattern
	grid_mem[ 0] = 0b00000000000000000000000000000000;
	grid_mem[ 1] = 0b00000000000000000000000000000000;
	grid_mem[ 2] = 0b00011110101000001000110000000000;
	grid_mem[ 3] = 0b00100000001000000001000011100000;
	grid_mem[ 4] = 0b00011100101000001011110100010000;
	grid_mem[ 5] = 0b00000010101000001001000111100000;
	grid_mem[ 6] = 0b00000010101000001001000100000000;
	grid_mem[ 7] = 0b00111100101111101001000011110000;
	grid_mem[ 8] = 0b00000000000000000000000000000000;
	grid_mem[ 9] = 0b00000000000000000000000000000000;
	grid_mem[10] = 0b00000000000000000000000000000000;
	grid_mem[11] = 0b00000000000000000000000000000000;
	grid_mem[12] = 0b00000000000000000000000000000000;
	grid_mem[13] = 0b00000000000000000000000000000000;
	grid_mem[14] = 0b00000000000110000011000000000000;
	grid_mem[15] = 0b00000000000011000110000000000000;
	grid_mem[16] = 0b00000000010010101010010000000000;
	grid_mem[17] = 0b00000000011101101101110000000000;
	grid_mem[18] = 0b00000000001010101010100000000000;
	grid_mem[19] = 0b00000000000111000111000000000000;
	grid_mem[20] = 0b00000000000000000000000000000000;
	grid_mem[21] = 0b00000000000111000111000000000000;
	grid_mem[22] = 0b00000000001010101010100000000000;
	grid_mem[23] = 0b00000000011101101101110000000000;
	grid_mem[24] = 0b00000000010010101010010000000000;
	grid_mem[25] = 0b00000000000011000110000000000000;
	grid_mem[26] = 0b00000000000110000011000000000000;
	grid_mem[27] = 0b00000000000000000000000000000000;
	grid_mem[28] = 0b00000000000000000000000000000000;
	grid_mem[29] = 0b00000000000000000000000000000000;
	grid_mem[30] = 0b00000000000000000000000000000000;
	grid_mem[31] = 0b00000000000000000000000000000000;

	while(true);
}
