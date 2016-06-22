/* dict.h

Hash tables

Copyright (c) 2010-2011 Matthias Kramm <kramm@quiss.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE. */

#ifndef __dict_h__
#define __dict_h__

#include <stdbool.h>
#include <stdio.h>

typedef bool (*equals_func)(const void* o1, const void* o2);
typedef unsigned int (*hash_func)(const void* o);
typedef void* (*dup_func)(const void* o);
typedef void (*free_func)(void* o);

typedef struct _type_t {
    equals_func equals;
    hash_func hash;
    dup_func dup;
    free_func free;
} type_t;

extern type_t charptr_type;
extern type_t stringstruct_type;
extern type_t ptr_type;
extern type_t int_type;

#define PTR_TO_INT(p) (((char*)(p)) - ((char*)NULL))
#define INT_TO_PTR(i) (((char*)NULL) + (int)(i))

typedef struct _dictentry {
    void* key;
    unsigned int hash;
    void* data;
    struct _dictentry* next;
} dictentry_t;

typedef struct _dict {
    dictentry_t** slots;
    type_t* key_type;
    int hashsize;
    int num;
} dict_t;

dict_t* dict_new(type_t* type);
void dict_init(dict_t* dict, int size);
void dict_init2(dict_t* dict, type_t* type, int size);
dictentry_t* dict_put(dict_t* h, const void* key, void* data);
void dict_put2(dict_t* h, const char* s, void* data);
int dict_count(dict_t* h);
void dict_dump(dict_t* h, FILE* fi, const char* prefix);
dictentry_t* dict_get_slot(dict_t* h, const void* key);
char dict_contains(dict_t* h, const void* s);
void* dict_lookup(dict_t* h, const void* s);
char dict_del(dict_t* h, const void* s);
char dict_del2(dict_t* h, const void* key, void* data);
dict_t* dict_clone(dict_t*);

void dict_foreach_keyvalue(dict_t* h,
                           void (*runFunction)(void* data, const void* key, void* val),
                           void* data);
void dict_foreach_value(dict_t* h, void (*runFunction)(void*));
void dict_free_all(dict_t* h, char free_keys, void (*free_data_function)(void*));
void dict_clear(dict_t* h);
void dict_destroy_shallow(dict_t* dict);
void dict_destroy(dict_t* dict);
#define DICT_ITERATE_DATA(d, t, v)                                                                 \
    int v##_i;                                                                                     \
    dictentry_t* v##_e;                                                                            \
    t v;                                                                                           \
    for (v##_i = 0; v##_i < (d)->hashsize; v##_i++)                                                \
        for (v##_e = (d)->slots[v##_i]; v##_e && ((v = (t)v##_e->data) || 1); v##_e = v##_e->next)
#define DICT_ITERATE_KEY(d, t, v)                                                                  \
    int v##_i;                                                                                     \
    dictentry_t* v##_e;                                                                            \
    t v;                                                                                           \
    for (v##_i = 0; v##_i < (d)->hashsize; v##_i++)                                                \
        for (v##_e = (d)->slots[v##_i]; v##_e && ((v = (t)v##_e->key) || 1); v##_e = v##_e->next)
#define DICT_ITERATE_ITEMS(d, t1, v1, t2, v2)                                                      \
    int v1##_i;                                                                                    \
    dictentry_t* v1##_e;                                                                           \
    t1 v1;                                                                                         \
    t2 v2;                                                                                         \
    for (v1##_i = 0; v1##_i < (d)->hashsize; v1##_i++)                                             \
        for (v1##_e = (d)->slots[v1##_i];                                                          \
             v1##_e && (((v1 = (t1)v1##_e->key) || 1) && ((v2 = (t2)v1##_e->data) || 1));          \
             v1##_e = v1##_e->next)

#endif
