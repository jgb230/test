#ifndef MD5_H
#define MD5_H

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <cstring>

#ifdef WIN32
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#define Sleep sleep
#define closesocket close
#endif

std::string md5(std::string dat);
std::string md5(const void* dat, size_t len);
std::string _md5file(const char* filename);
std::string _md5file(std::FILE* file);
std::string md5sum6(std::string dat);
std::string md5sum6(const void* dat, size_t len);
void printTime_(const char *func, long int line);

#endif // end of MD5_H
