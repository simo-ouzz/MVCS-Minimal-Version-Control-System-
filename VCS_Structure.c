#include <cstddef>

struct 
{
    char *hash;
    char *author;
    char *date;
    char *message;
}; typedef struct Commit;

struct
{
    char *name;
    struct tree **subtrees;
    int subtree_count;
    struct commit **commits;
    int commit_count;
}; typedef struct  Tree;

struct repository
{
    char *name;
    struct tree *root_tree;
}; typedef struct repository Repository;

struct blob
{
    char *filename;
    char *content;
    size_t size;
}; typedef struct blob Blob;

