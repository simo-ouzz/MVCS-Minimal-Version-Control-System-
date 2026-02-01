#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* =======================
   SHA-1 context
   ======================= */

typedef struct {
    uint32_t state[5];
    uint64_t count;               /* bytes processed */
    unsigned char buffer[64];
} SHA1_CTX;

/* =======================
   Internal helpers
   ======================= */

#define SHA1_ROTL(x,n) (((x) << (n)) | ((x) >> (32 - (n))))

static void sha1_transform(uint32_t state[5], const unsigned char block[64]) {
    uint32_t a, b, c, d, e, t, w[80];

    for (int i = 0; i < 16; i++) {
        w[i]  = (uint32_t)block[i*4]     << 24;
        w[i] |= (uint32_t)block[i*4 + 1] << 16;
        w[i] |= (uint32_t)block[i*4 + 2] << 8;
        w[i] |= (uint32_t)block[i*4 + 3];
    }

    for (int i = 16; i < 80; i++)
        w[i] = SHA1_ROTL(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);

    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    for (int i = 0; i < 80; i++) {
        if (i < 20)
            t = SHA1_ROTL(a,5) + ((b & c) | (~b & d)) + e + w[i] + 0x5A827999;
        else if (i < 40)
            t = SHA1_ROTL(a,5) + (b ^ c ^ d) + e + w[i] + 0x6ED9EBA1;
        else if (i < 60)
            t = SHA1_ROTL(a,5) + ((b & c) | (b & d) | (c & d)) + e + w[i] + 0x8F1BBCDC;
        else
            t = SHA1_ROTL(a,5) + (b ^ c ^ d) + e + w[i] + 0xCA62C1D6;

        e = d;
        d = c;
        c = SHA1_ROTL(b,30);
        b = a;
        a = t;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

/* =======================
   Public API
   ======================= */

static void SHA1_Init(SHA1_CTX *ctx) {
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
    ctx->count = 0;
}

static void SHA1_Update(SHA1_CTX *ctx, const unsigned char *data, size_t len) {
    size_t index = ctx->count & 63;
    ctx->count += len;

    size_t part_len = 64 - index;

    if (len >= part_len) {
        memcpy(ctx->buffer + index, data, part_len);
        sha1_transform(ctx->state, ctx->buffer);
        data += part_len;
        len  -= part_len;
        index = 0;

        while (len >= 64) {
            sha1_transform(ctx->state, data);
            data += 64;
            len  -= 64;
        }
    }

    if (len)
        memcpy(ctx->buffer + index, data, len);
}

static void SHA1_Final(unsigned char digest[20], SHA1_CTX *ctx) {
    uint64_t bits = ctx->count * 8;
    size_t index = ctx->count & 63;

    ctx->buffer[index++] = 0x80;

    if (index > 56) {
        memset(ctx->buffer + index, 0, 64 - index);
        sha1_transform(ctx->state, ctx->buffer);
        index = 0;
    }

    memset(ctx->buffer + index, 0, 56 - index);

    for (int i = 0; i < 8; i++)
        ctx->buffer[56 + i] = (bits >> (56 - 8*i)) & 0xff;

    sha1_transform(ctx->state, ctx->buffer);

    for (int i = 0; i < 5; i++) {
        digest[i*4]     = (ctx->state[i] >> 24) & 0xff;
        digest[i*4 + 1] = (ctx->state[i] >> 16) & 0xff;
        digest[i*4 + 2] = (ctx->state[i] >> 8) & 0xff;
        digest[i*4 + 3] = ctx->state[i] & 0xff;
    }
}

#endif /* SHA1_H */
