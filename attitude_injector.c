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
	bool is_init = false;
	int marker = 0;
	int buff_size = 4096;
	char buff[buff_size];
	while (1) {
		int inj_len = 0;
		char inj[buff_size];
		int data_len = read(STDIN_FILENO, buff, buff_size);
		int inj_pos = 0;
		for (int i = 0; i < data_len; i++) {
			if (marker) {
				marker = 0;
				if (buff[i] == 0xd8) { //SOI
					if(is_init) {
						inj_pos = i + 1;
						ms_update();

						inj[inj_len++] = 0xFF;
						inj[inj_len++] = 0xE1;
						inj_len += sprintf(inj + inj_len, "http://ns.adobe.com/xap/1.0/");
						inj[inj_len++] = '\0';
						inj_len += sprintf(inj + inj_len, "<?xpacket begin=\"﻿");
						inj[inj_len++] = 0xEF;
						inj[inj_len++] = 0xBB;
						inj[inj_len++] = 0xBF;
						inj_len += sprintf(inj + inj_len, "\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?>");
						inj_len += sprintf(inj + inj_len, "<x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\"attitude-injector rev1\">");
						inj_len += sprintf(inj + inj_len, "<rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">");
						inj_len += sprintf(inj + inj_len, "<rdf:Description xmlns:GPano=\"http://ns.google.com/photos/1.0/panorama/\" rdf:about=\"\">");
						inj_len += sprintf(inj + inj_len, "<GPano:PoseHeadingDegrees>%f</GPano:PoseHeadingDegrees>", ypr[YAW]);
						inj_len += sprintf(inj + inj_len, "<GPano:PosePitchDegrees>%f</GPano:PosePitchDegrees>", ypr[PITCH]);
						inj_len += sprintf(inj + inj_len, "<GPano:PoseRollDegrees>%f</GPano:PoseRollDegrees>", ypr[ROLL]);
						inj_len += sprintf(inj + inj_len, "</rdf:Description>");
						inj_len += sprintf(inj + inj_len, "</rdf:RDF>");
						inj_len += sprintf(inj + inj_len, "</x:xmpmeta>");
						inj_len += sprintf(inj + inj_len, "<?xpacket end=\"w\"?>");
					}
				}
				if (buff[i] == 0xd9) { //EOI
					if(!is_init) {
						is_init = true;
						ms_open();
					}
				}
			} else if (buff[i] == 0xff) {
				marker = 1;
			}
		}
		if(is_init) {
			if(inj_pos > 0) {
				write(STDOUT_FILENO, buff, inj_pos);
			}
			if(inj_len > 0) {
				write(STDOUT_FILENO, inj, inj_len);
			}
			if(inj_pos < data_len) {
				write(STDOUT_FILENO, buff + inj_pos, data_len - inj_pos);
			}
		}
	}

	return 0;
}
