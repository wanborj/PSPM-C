#include "hashtable.h"
#include "string.h"
#include "PSPMport.h"

#define TABLESIZE 20

struct kv{
	struct kv * next;
	char * key;
	void * value;
	void (*free_value)(void *);
};


struct HashTable{
	int			length;
	struct kv ** table;
};

int prv_hashtable_get_length(struct HashTable * ht){
	return ht->length;
}


static void init_kv(struct kv * kv)
{
	kv->next = NULL;
	kv->key  = NULL;
	kv->value= NULL;
	kv->free_value = NULL;
}

static void free_kv(struct kv * kv)
{
	if(kv){
		if(kv->free_value){
			kv->free_value(kv->value);
		}
		port_free(kv->key);
		kv->key = NULL;
		port_free(kv);
		kv = NULL;
	}
}

static unsigned int hash_33(char * key)
{
	unsigned int hash = 5381;
	int i;
	while( *key ){
		hash = (hash << 5) + hash + *(key++);
	}

	return hash%TABLESIZE;
}

// variable c must be equivalent to zero
/*
static void * memset(void * src, int c, int len)
{
	if(address == NULL)
		return NULL;
	char *tmpsrc = (char *)src;
	while(len -- ){
		*tmpsrc++ = (char)c;
	}

	return src;
}
*/

prv_hashtable_t * prv_hashtable_new()
{
	prv_hashtable_t * ht = (prv_hashtable_t *)port_malloc(sizeof(struct HashTable));
	if(NULL == ht){
		prv_hashtable_delete(ht);
		return NULL;
	}
	ht->table = port_malloc(sizeof(struct kv *) * TABLESIZE);
	if(NULL == ht->table){
		prv_hashtable_delete(ht);
		return NULL;
	}
	memset(ht->table, 0, sizeof(struct kv*)*TABLESIZE);
}

void prv_hashtable_delete(prv_hashtable_t *ht)
{
	if(ht){
		if(ht->table){
			int i = 0;
			for(;i<TABLESIZE;++i){
				struct kv *p = ht->table[i];
				struct kv *q = NULL;
				while(p){
					q = p->next;
					free_kv(p);
					p=q;
				}
			}
			port_free(ht->table);
			ht->table = NULL;
		}
	}
}


int prv_hashtable_put(prv_hashtable_t *ht, char *key, void * value, void (*free_value)(void*))
{
	int i = hash_33(key);
	struct kv * p = ht->table[i];
	struct kv * prep = p;

	while(p){
		if(strcmp(p->key, key)==0){
			if(p->free_value){
				p->free_value(p->value);
			}
			p->value = value;
			p->free_value = vPortFree; // port_free can not be a pointer
			break;
		}
		prep = p;
		p = p->next;
	}

	if(p == NULL){
		char *kstr = port_malloc(strlen(key) + 1);
		if(kstr == NULL){
			return -1;
		}
		struct kv *kv = port_malloc(sizeof(struct kv));
		if( NULL == kv ){
			port_free(kstr);
			kstr = NULL;
			return -1;
		}
		init_kv(kv);
		strcpy(kstr, key);
		kv->key = kstr;
		kv->value = value;
		kv->free_value = vPortFree;  // port_free can not be a pointer

		if(prep == NULL){
			ht->table[i] = kv;
		}else{
			prep->next = kv;
		}
	}
	ht->length ++;

	return 0;
}


void * prv_hashtable_get(prv_hashtable_t *ht, char *key)
{
	int i = hash_33(key);
	struct kv * p = ht->table[i];
	while(p){
		if(strcmp(key, p->key) == 0){
			return p->value;
		}
		p = p->next;
	}
	return NULL;
}


void prv_hashtable_remove(prv_hashtable_t *ht, char *key)
{
	int i = hash_33(key);
	struct kv * p = ht->table[i];
	struct kv * prep = p;
	while(p){
		if(strcmp(key, p->key) == 0){
			if(p == prep){
				ht->table[i] = NULL;
			}else{
				prep->next = p->next;
			}
			free_kv(p);
		}
		prep = p;
		p=p->next;
	}
}
