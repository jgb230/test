#include "md5.h"


	#define F(x, y, z)   ((z) ^ ((x) & ((y) ^ (z))))
	#define G(x, y, z)   ((y) ^ ((z) & ((x) ^ (y))))
	#define H(x, y, z)   ((x) ^ (y) ^ (z))
	#define I(x, y, z)   ((y) ^ ((x) | ~(z)))
	#define _STEP(f, a, b, c, d, x, t, s) \
		(a) += f((b), (c), (d)) + (x) + (t); \
		(a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
		(a) += (b);

	#if defined(__i386__) || defined(__x86_64__) || defined(__vax__)
		#define _SET(n) \
			(*(MD5_u32 *)&ptr[(n) * 4])
		#define _GET(n) \
			_SET(n)
	#else
		#define _SET(n) \
			(ctx->block[(n)] = \
			(MD5_u32)ptr[(n) * 4] | \
			((MD5_u32)ptr[(n) * 4 + 1] << 8) | \
			((MD5_u32)ptr[(n) * 4 + 2] << 16) | \
			((MD5_u32)ptr[(n) * 4 + 3] << 24))
		#define _GET(n) \
			(ctx->block[(n)])
	#endif

	typedef unsigned int MD5_u32;

	typedef struct {
		MD5_u32 lo, hi;
		MD5_u32 a, b, c, d;
		unsigned char buffer[64];
		MD5_u32 block[16];
	} MD5_CTX;
 
	static void _MD5_Init(MD5_CTX *ctx);
	static void _MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size);
	static void _MD5_Final(unsigned char *result, MD5_CTX *ctx);

	static const void *body(MD5_CTX *ctx, const void *data, unsigned long size){
		const unsigned char *ptr;
		MD5_u32 a, b, c, d;
		MD5_u32 saved_a, saved_b, saved_c, saved_d;
 
		ptr = (const unsigned char*)data;
 
		a = ctx->a;
		b = ctx->b;
		c = ctx->c;
		d = ctx->d;
 
		do {
			saved_a = a;
			saved_b = b;
			saved_c = c;
			saved_d = d;

			_STEP(F, a, b, c, d, _SET(0), 0xd76aa478, 7)
			_STEP(F, d, a, b, c, _SET(1), 0xe8c7b756, 12)
			_STEP(F, c, d, a, b, _SET(2), 0x242070db, 17)
			_STEP(F, b, c, d, a, _SET(3), 0xc1bdceee, 22)
			_STEP(F, a, b, c, d, _SET(4), 0xf57c0faf, 7)
			_STEP(F, d, a, b, c, _SET(5), 0x4787c62a, 12)
			_STEP(F, c, d, a, b, _SET(6), 0xa8304613, 17)
			_STEP(F, b, c, d, a, _SET(7), 0xfd469501, 22)
			_STEP(F, a, b, c, d, _SET(8), 0x698098d8, 7)
			_STEP(F, d, a, b, c, _SET(9), 0x8b44f7af, 12)
			_STEP(F, c, d, a, b, _SET(10), 0xffff5bb1, 17)
			_STEP(F, b, c, d, a, _SET(11), 0x895cd7be, 22)
			_STEP(F, a, b, c, d, _SET(12), 0x6b901122, 7)
			_STEP(F, d, a, b, c, _SET(13), 0xfd987193, 12)
			_STEP(F, c, d, a, b, _SET(14), 0xa679438e, 17)
			_STEP(F, b, c, d, a, _SET(15), 0x49b40821, 22)
			_STEP(G, a, b, c, d, _GET(1), 0xf61e2562, 5)
			_STEP(G, d, a, b, c, _GET(6), 0xc040b340, 9)
			_STEP(G, c, d, a, b, _GET(11), 0x265e5a51, 14)
			_STEP(G, b, c, d, a, _GET(0), 0xe9b6c7aa, 20)
			_STEP(G, a, b, c, d, _GET(5), 0xd62f105d, 5)
			_STEP(G, d, a, b, c, _GET(10), 0x02441453, 9)
			_STEP(G, c, d, a, b, _GET(15), 0xd8a1e681, 14)
			_STEP(G, b, c, d, a, _GET(4), 0xe7d3fbc8, 20)
			_STEP(G, a, b, c, d, _GET(9), 0x21e1cde6, 5)
			_STEP(G, d, a, b, c, _GET(14), 0xc33707d6, 9)
			_STEP(G, c, d, a, b, _GET(3), 0xf4d50d87, 14)
			_STEP(G, b, c, d, a, _GET(8), 0x455a14ed, 20)
			_STEP(G, a, b, c, d, _GET(13), 0xa9e3e905, 5)
			_STEP(G, d, a, b, c, _GET(2), 0xfcefa3f8, 9)
			_STEP(G, c, d, a, b, _GET(7), 0x676f02d9, 14)
			_STEP(G, b, c, d, a, _GET(12), 0x8d2a4c8a, 20)
			_STEP(H, a, b, c, d, _GET(5), 0xfffa3942, 4)
			_STEP(H, d, a, b, c, _GET(8), 0x8771f681, 11)
			_STEP(H, c, d, a, b, _GET(11), 0x6d9d6122, 16)
			_STEP(H, b, c, d, a, _GET(14), 0xfde5380c, 23)
			_STEP(H, a, b, c, d, _GET(1), 0xa4beea44, 4)
			_STEP(H, d, a, b, c, _GET(4), 0x4bdecfa9, 11)
			_STEP(H, c, d, a, b, _GET(7), 0xf6bb4b60, 16)
			_STEP(H, b, c, d, a, _GET(10), 0xbebfbc70, 23)
			_STEP(H, a, b, c, d, _GET(13), 0x289b7ec6, 4)
			_STEP(H, d, a, b, c, _GET(0), 0xeaa127fa, 11)
			_STEP(H, c, d, a, b, _GET(3), 0xd4ef3085, 16)
			_STEP(H, b, c, d, a, _GET(6), 0x04881d05, 23)
			_STEP(H, a, b, c, d, _GET(9), 0xd9d4d039, 4)
			_STEP(H, d, a, b, c, _GET(12), 0xe6db99e5, 11)
			_STEP(H, c, d, a, b, _GET(15), 0x1fa27cf8, 16)
			_STEP(H, b, c, d, a, _GET(2), 0xc4ac5665, 23)
			_STEP(I, a, b, c, d, _GET(0), 0xf4292244, 6)
			_STEP(I, d, a, b, c, _GET(7), 0x432aff97, 10)
			_STEP(I, c, d, a, b, _GET(14), 0xab9423a7, 15)
			_STEP(I, b, c, d, a, _GET(5), 0xfc93a039, 21)
			_STEP(I, a, b, c, d, _GET(12), 0x655b59c3, 6)
			_STEP(I, d, a, b, c, _GET(3), 0x8f0ccc92, 10)
			_STEP(I, c, d, a, b, _GET(10), 0xffeff47d, 15)
			_STEP(I, b, c, d, a, _GET(1), 0x85845dd1, 21)
			_STEP(I, a, b, c, d, _GET(8), 0x6fa87e4f, 6)
			_STEP(I, d, a, b, c, _GET(15), 0xfe2ce6e0, 10)
			_STEP(I, c, d, a, b, _GET(6), 0xa3014314, 15)
			_STEP(I, b, c, d, a, _GET(13), 0x4e0811a1, 21)
			_STEP(I, a, b, c, d, _GET(4), 0xf7537e82, 6)
			_STEP(I, d, a, b, c, _GET(11), 0xbd3af235, 10)
			_STEP(I, c, d, a, b, _GET(2), 0x2ad7d2bb, 15)
			_STEP(I, b, c, d, a, _GET(9), 0xeb86d391, 21)
 
			a += saved_a;
			b += saved_b;
			c += saved_c;
			d += saved_d;
 
			ptr += 64;
		} while (size -= 64);
 
		ctx->a = a;
		ctx->b = b;
		ctx->c = c;
		ctx->d = d;
 
		return ptr;
	}
 
	void _MD5_Init(MD5_CTX *ctx){
		ctx->a = 0x67452301;
		ctx->b = 0xefcdab89;
		ctx->c = 0x98badcfe;
		ctx->d = 0x10325476;
 
		ctx->lo = 0;
		ctx->hi = 0;
	}
 
	void _MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size){
		MD5_u32 saved_lo;
		unsigned long used, free;
 
		saved_lo = ctx->lo;
		if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
			ctx->hi++;
		ctx->hi += size >> 29;
		used = saved_lo & 0x3f;
 
		if (used){
			free = 64 - used;
			if (size < free) {
				memcpy(&ctx->buffer[used], data, size);
				return;
			}
 
			memcpy(&ctx->buffer[used], data, free);
			data = (unsigned char *)data + free;
			size -= free;
			body(ctx, ctx->buffer, 64);
		}
 
		if (size >= 64) {
			data = body(ctx, data, size & ~(unsigned long)0x3f);
			size &= 0x3f;
		}
 
		memcpy(ctx->buffer, data, size);
	}
 
	void _MD5_Final(unsigned char *result, MD5_CTX *ctx){
		unsigned long used, free;
		used = ctx->lo & 0x3f;
		ctx->buffer[used++] = 0x80;
		free = 64 - used;
 
		if (free < 8) {
			memset(&ctx->buffer[used], 0, free);
			body(ctx, ctx->buffer, 64);
			used = 0;
			free = 64;
		}
 
		memset(&ctx->buffer[used], 0, free - 8);
 
		ctx->lo <<= 3;
		ctx->buffer[56] = ctx->lo;
		ctx->buffer[57] = ctx->lo >> 8;
		ctx->buffer[58] = ctx->lo >> 16;
		ctx->buffer[59] = ctx->lo >> 24;
		ctx->buffer[60] = ctx->hi;
		ctx->buffer[61] = ctx->hi >> 8;
		ctx->buffer[62] = ctx->hi >> 16;
		ctx->buffer[63] = ctx->hi >> 24;
		body(ctx, ctx->buffer, 64);
		result[0] = ctx->a;
		result[1] = ctx->a >> 8;
		result[2] = ctx->a >> 16;
		result[3] = ctx->a >> 24;
		result[4] = ctx->b;
		result[5] = ctx->b >> 8;
		result[6] = ctx->b >> 16;
		result[7] = ctx->b >> 24;
		result[8] = ctx->c;
		result[9] = ctx->c >> 8;
		result[10] = ctx->c >> 16;
		result[11] = ctx->c >> 24;
		result[12] = ctx->d;
		result[13] = ctx->d >> 8;
		result[14] = ctx->d >> 16;
		result[15] = ctx->d >> 24;
		memset(ctx, 0, sizeof(*ctx));
	}



using namespace std;

/* Return Calculated raw result(always little-endian), the size is always 16 */
void _md5bin(const void* dat, size_t len, unsigned char out[16]) {
    MD5_CTX c;
    _MD5_Init(&c);
    _MD5_Update(&c, dat, len);
    _MD5_Final(out, &c);
}

static char _hb2hex(unsigned char hb) {
    hb = hb & 0xF;
    return hb < 10 ? '0' + hb : hb - 10 + 'a';
}

string _md5file(const char* filename){
	std::FILE* file = std::fopen(filename, "rb");
	string res = _md5file(file);
	std::fclose(file);
	return res;
}

string _md5file(std::FILE* file){

	MD5_CTX c;
    _MD5_Init(&c);

	char buff[BUFSIZ];
	unsigned char out[16];
	size_t len = 0;
	while( ( len = std::fread(buff ,sizeof(char), BUFSIZ, file) ) > 0) {
		_MD5_Update(&c, buff, len);
	}
	_MD5_Final(out, &c);

	string res;
	for(size_t i = 0; i < 16; ++ i) {
        res.push_back(_hb2hex(out[i] >> 4));
        res.push_back(_hb2hex(out[i]));
    }
	return res;
}

string md5(const void* dat, size_t len) {
    string res;
    unsigned char out[16];
    _md5bin(dat, len, out);
    for(size_t i = 0; i < 16; ++ i) {
        res.push_back(_hb2hex(out[i] >> 4));
        res.push_back(_hb2hex(out[i]));
    }
    return res;
}

std::string md5(std::string dat){
	return md5(dat.c_str(), dat.length());
}

/* Generate shorter md5sum by something like base62 instead of base16 or base10. 0~61 are represented by 0-9a-zA-Z */
string md5sum6(const void* dat, size_t len){
    static const char* tbl = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string res;
    unsigned char out[16];
    _md5bin(dat, len, out);
    for(size_t i = 0; i < 6; ++i) {
        res.push_back(tbl[out[i] % 62]);
    }
    return res;
}

std::string md5sum6(std::string dat){
	return md5sum6(dat.c_str(), dat.length() );
}