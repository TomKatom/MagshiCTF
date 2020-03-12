#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable : 4996)
#include <stdio.h>
#include <string.h>


int convert1(char ch);
char convert2(int val);
int mod(int var, int var2);
size_t b64_decoded_size(const char* in);
int b64_isvalidchar(char c);
int b64_decode(const char* in, unsigned char* out, size_t outlen);

const int b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
	59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, 50, 51 };

int b64_decode(const char* in, unsigned char* out, size_t outlen);

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
	int val = 0;
	rewind(fp);
	char* fcontent = (char*)malloc(sizeof(char) * (fsize - 1));
	fread(fcontent, 1, fsize, fp);
	char* new_str = malloc(sizeof(char) * (fsize - 1));
	fclose(fp);
	fcontent[fsize] = 0;
	new_str[fsize] = 0;
	printf("Flag before = %s \n", fcontent);
	printf("\nDecrypt.. \n");
	if (!b64_decode(fcontent, new_str, 36)) { puts("Cannot decode base 64"); exit(1); }  //36 - size of flag
	new_str[36] = 0;
	printf("Phase 3 completed - %s \n", new_str);
	for (int i = 0; i < strlen(new_str); i++) {
		val = convert1(new_str[i]);
		val = (val - i) % 37;
		if (val < 0) val += 37;
		new_str[i] = convert2(val);
	}
	printf("Phase 2 completed - %s \n", new_str);

	char new_str2[37] = { 0 };

	for (int i = 0; i < strlen(new_str); i++) {
		val = (i + 3) % strlen(new_str);
		new_str2[val] = new_str[i];
	}
	printf("Phase 1 completed - %s \n\n", new_str2);


	fp = fopen(argv[1], "w");
	fprintf(fp, "%s", new_str2);

	fclose(fp);
	free(new_str);
	free(fcontent);
	free(fp);
	return 0;
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

size_t b64_decoded_size(const char* in)
{
	size_t len;
	size_t ret;
	size_t i;

	if (in == NULL)
		return 0;

	len = strlen(in);
	ret = len / 4 * 3;

	for (i = len; i-- > 0; ) {
		if (in[i] == '=') {
			ret--;
		}
		else {
			break;
		}
	}

	return ret;
}

int b64_isvalidchar(char c)
{
	if (c >= '0' && c <= '9')
		return 1;
	if (c >= 'A' && c <= 'Z')
		return 1;
	if (c >= 'a' && c <= 'z')
		return 1;
	if (c == '+' || c == '/' || c == '=')
		return 1;
	return 0;
}

int b64_decode(const char* in, unsigned char* out, size_t outlen)
{
	size_t len;
	size_t i;
	size_t j;
	int    v;

	if (in == NULL || out == NULL)
		return 0;

	len = strlen(in);
	if (outlen < b64_decoded_size(in) || len % 4 != 0)
		return 0;

	for (i = 0; i < len; i++) {
		if (!b64_isvalidchar(in[i])) {
			return 0;
		}
	}

	for (i = 0, j = 0; i < len; i += 4, j += 3) {
		v = b64invs[in[i] - 43];
		v = (v << 6) | b64invs[in[i + 1] - 43];
		v = in[i + 2] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 2] - 43];
		v = in[i + 3] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 3] - 43];

		out[j] = (v >> 16) & 0xFF;
		if (in[i + 2] != '=')
			out[j + 1] = (v >> 8) & 0xFF;
		if (in[i + 3] != '=')
			out[j + 2] = v & 0xFF;
	}

	return 1;
}