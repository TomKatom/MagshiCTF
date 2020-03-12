#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable : 4996)
#include <stdio.h>
#include <string.h>

void magic1(char* new_str, char* buf);
void magic2(char* new_str);
char* magic3(const unsigned char* in);
int convert1(char ch);
char convert2(int val);
int mod(int var, int var2);
size_t b64_encoded_size(size_t inlen);

const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int main(int argc, char** argv) {
	if (argc != 2) {
		puts("Usage CrackMe.exe <file path>");
		exit(1);
	}

	FILE* fp = fopen(argv[1], "r");
	
	if (!fp) {
		puts("Error! cannot open file ");
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	int fsize = ftell(fp);
	rewind(fp);
	char* fcontent = (char*)malloc(sizeof(char) *( fsize-1));
	fread(fcontent, 1, fsize, fp);
	char *new_str = malloc(sizeof(char) * (fsize-1));
	fclose(fp);
	fcontent[fsize ] = 0;
	new_str[fsize] = 0;

	magic1(new_str, fcontent);
	magic2(new_str);
	new_str = magic3((const unsigned char*)new_str);
	fp = fopen(argv[1], "w");
	fprintf(fp, "%s", new_str);

	fclose(fp);
	free(new_str);
	free(fcontent);
	free(fp);
	return 0;
}

void magic1(char* new_str, char* buf) {
	int val = 0;
	for (int i = 0; i < strlen(buf); i++) {
		val = mod((i + 3), strlen(buf));
		new_str[i] = buf[val];
	}
}

void magic2(char* new_str) {
	int val = 0;
	for (int i = 0; i < strlen(new_str); i++) {
		int val = convert1(new_str[i]);
		val = mod((val + i), 37);
		new_str[i] = convert2(val);
	}
}

int convert1(char ch) {  //ors
	if (ch >= 97 && ch <= 122) return ((int)ch) - 87;
	else if (ch >= 48 && ch <= 57) return ((int)ch) - 48;
	else if (ch == 95) return 36;  //"_"
	else {
		puts("Invalid char ");
		exit(1);
	}
}

char convert2(int val) { //reverse to ord
	if (val >= 0 && val <= 9) return ((char)val + 48);
	else if (val < 36) return ((char)val + 87);
	return '_';
}

int mod(int var, int var2) {
	return var % var2;
}

char* magic3(const unsigned char* in)
{
	size_t len = strlen(in);
	char* out;
	size_t  elen;
	size_t  i;
	size_t  j;
	size_t  v;

	if (in == NULL || len == 0)
		return NULL;

	elen = b64_encoded_size(len);
	out = malloc(elen + 1);
	out[elen] = '\0';

	for (i = 0, j = 0; i < len; i += 3, j += 4) {
		v = in[i];
		v = i + 1 < len ? v << 8 | in[i + 1] : v << 8;
		v = i + 2 < len ? v << 8 | in[i + 2] : v << 8;

		out[j] = b64chars[(v >> 18) & 0x3F];
		out[j + 1] = b64chars[(v >> 12) & 0x3F];
		if (i + 1 < len) {
			out[j + 2] = b64chars[(v >> 6) & 0x3F];
		}
		else {
			out[j + 2] = '=';
		}
		if (i + 2 < len) {
			out[j + 3] = b64chars[v & 0x3F];
		}
		else {
			out[j + 3] = '=';
		}
	}

	return out;
}	

size_t b64_encoded_size(size_t inlen)
{
	size_t ret;

	ret = inlen;
	if (inlen % 3 != 0)
		ret += 3 - (inlen % 3);
	ret /= 3;
	ret *= 4;

	return ret;
}