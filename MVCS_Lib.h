#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <zlib.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define HASH_SIZE 20


int git_blob_sha1(const char *path, unsigned char hash[HASH_SIZE]) {
    FILE *f = fopen(path, "rb");
    if (!f) return -1;

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);

    SHA_CTX ctx;
    SHA1_Init(&ctx);

    char header[64];
    int header_len = snprintf(header, sizeof(header), "blob %zu", size) + 1;
    SHA1_Update(&ctx, header, header_len);

    unsigned char buf[8192];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
        SHA1_Update(&ctx, buf, n);
    }

    fclose(f);
    SHA1_Final(hash, &ctx);
    return 0;
}

void compress_file_and_write(const char *path) {
    Buffer input = read_file(path);
    if (!input.data || input.size == 0) {
        fprintf(stderr, "Failed to read file\n");
        return;
    }

    /* 1️⃣ Build Git blob header: "blob <size>\0" */
    char header[64];
    int header_len = snprintf(header, sizeof(header), "blob %zu", input.size) + 1; // +1 for '\0'

    /* 2️⃣ Allocate buffer for header + file content */
    size_t total_size = header_len + input.size;
    unsigned char *object = malloc(total_size);
    if (!object) {
        fprintf(stderr, "Memory allocation failed\n");
        free(input.data);
        return;
    }

    memcpy(object, header, header_len);
    memcpy(object + header_len, input.data, input.size);

    /* 3️⃣ Allocate compressed buffer */
    uLongf compressed_size = compressBound(total_size);
    unsigned char *compressed = malloc(compressed_size);
    if (!compressed) {
        fprintf(stderr, "Memory allocation failed\n");
        free(input.data);
        free(object);
        return;
    }

    /* 4️⃣ Compress like Git */
    int res = compress2(
        compressed,
        &compressed_size,
        object,
        total_size,
        Z_BEST_COMPRESSION
    );

    if (res != Z_OK) {
        fprintf(stderr, "Compression failed: %d\n", res);
        free(input.data);
        free(object);
        free(compressed);
        return;
    }

    /* 5️⃣ Write raw compressed object (binary) */
    fwrite(compressed, 1, compressed_size, stdout);

    /* cleanup */
    free(input.data);
    free(object);
    free(compressed);
}

void Init(int argc, char *argv[]) {

    if (argc > 1 && strcmp(argv[1], "init") == 0) {
        printf("MVCS repository initialized.\n");
        CreateDirectory(".mvcs", NULL);
        CreateDirectory(".//.mvcs//objects", NULL);
        CreateDirectory(".//.mvcs//refs", NULL);
        CreateDirectory(".//.mvcs//refs//heads", NULL);
        FILE *headFile = fopen(".//.mvcs//HEAD", "w");
        if (headFile != NULL) {
            fprintf(headFile, "ref: refs/heads/main\n");
            fclose(headFile);
        }
        return;
    } else {
        printf("MVCS: no command given.\n");
        return;
    }


}