#include <stddef.h>
#include <stdint.h>

#define HASH_SIZE 20        // SHA-1 binary size
#define HASH_HEX_SIZE 41    // 40 hex + null
#define MAX_NAME 256

/* ---------- Forward Declarations ---------- */
typedef struct Tree Tree;

/* ---------- Entry Type ---------- */
typedef enum {
    ENTRY_FILE,
    ENTRY_DIR
} EntryType;

/* ---------- Tree Entry ---------- */
typedef struct {
    char name[MAX_NAME];          // filename
    char mode[8];                 // "100644", "040000", etc.
    
    uint8_t hash[HASH_SIZE];      // binary SHA-1
    
    EntryType type;               // file or dir
    
    Tree *subtree;                // only valid if ENTRY_DIR
} TreeEntry;

/* ---------- Tree Object ---------- */
struct Tree {
    TreeEntry *entries;
    size_t entry_count;
};

/* ---------- Commit Object ---------- */
typedef struct {
    uint8_t hash[HASH_SIZE];          // commit's own hash
    
    uint8_t tree_hash[HASH_SIZE];     // root tree hash
    uint8_t parent_hash[HASH_SIZE];   // parent commit hash
    
    char author[100];
    char message[256];
    char timestamp[32];
} Commit;

/* ---------- Blob Object ---------- */
typedef struct {
    uint8_t hash[HASH_SIZE];  // blob hash
    size_t size;
    uint8_t *data;            // raw file content
} Blob;
