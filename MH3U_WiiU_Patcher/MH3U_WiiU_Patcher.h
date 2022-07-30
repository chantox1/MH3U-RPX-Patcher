// MH3U_WiiU_Patcher.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <stdlib.h>

class Payload {
	public:
		char *data;
		size_t len;

		Payload(size_t len) {
			this->data = (char *)malloc(len);
			this->len = len;
		}

		Payload(char *data, size_t len) {
			this->data = data;
			this->len = len;
		}

		~Payload() {
			free(this->data);
		}
};

// TODO: Reference additional headers your program requires here.
