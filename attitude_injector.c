/*
 Copyright (c) 2012, Broadcom Europe Ltd
 Copyright (c) 2012, OtherCrashOverride
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the copyright holder nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Video decode demo using OpenMAX IL though the ilcient helper library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>

#include "attitude_injector.h"
#include "MotionSensor.h"

int main(int argc, char *argv[]) {

	// init motion sensor
	ms_open();

	int marker = 0;
	int buff_size = 4096;
	unsigned char buff[buff_size];
	while (1) {
		int data_len = read(STDIN_FILENO, buff, buff_size);
		for (int i = 0; i < data_len; i++) {
			if (marker) {
				marker = 0;
				if (buff[i] == 0xd8) { //SOI
					ms_update();
					unsigned char str[256];
					int len = sprintf(str, "%f, %f, %f\n", ypr[YAW], ypr[PITCH], ypr[ROLL])
					write(STDERR_FILENO, str, len);
				}
				if (buff[i] == 0x0d) { //EOI
				}
			} else if (buff[i] == 0xff) {
				marker = 1;
			}
		}
		write(STDOUT_FILENO, buff, data_len);
	}

	return 0;
}
