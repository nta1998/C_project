#ifndef FINAL_PROJECT_EXT_LIST_H
#define FINAL_PROJECT_EXT_LIST_H

typedef struct ExtList ExtList;
typedef struct ExtNode ExtNode;

ExtList *extlist_create(void);
Bool     extlist_add(ExtList *e, const char *name,
                     long address);
ExtNode *extlist_first(ExtList *e);
ExtNode *extnode_next(ExtNode *n);
const char *extnode_name(ExtNode *n);
long        extnode_address(ExtNode *n);
Bool     extlist_empty(ExtList *e);
void     extlist_free(ExtList *e);

#endif
