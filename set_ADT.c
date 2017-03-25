#include "mergesort.h"
#include "set_ADT.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct set {
  int len;
  int max_len;
  int *data;
};


struct set *set_create(void) {
  struct set *s = malloc(sizeof(struct set));
  s->len = 0;
  s->max_len = 1;
  s->data = malloc(s->max_len * sizeof(int));
  return s;
}


void set_destroy(struct set *s) {
  assert(s);
  free(s->data);
  free(s);
}


int set_size(const struct set *s) {
  assert(s);
  return s->len;
}


bool set_member(const struct set *s, int i) {
  assert(s);
  int min = 0;
  int max = s->len - 1;
  while (min <= max) {
    int mid = min + (max - min) / 2;
    if (s->data[mid] == i) return true;
    else if (i < s->data[mid]) max = mid - 1;
    else min = mid + 1;  
  }
  return false;
}
  

void set_add(struct set *s, int i) {
  assert(s);
  if (s->len == s->max_len) {
      s->max_len *= 2;
      s->data = realloc(s->data, s->max_len * sizeof(int));
    }
  if (s->len == 0) {
    s->data[0] = i;
    s->len += 1;
  }   
  if (!set_member(s, i)) {
    int pos = 0;
    int flag = 0;
    for (int j = 0; j < s->len; j++) {
      if (i <= s->data[j]) {
        pos = j;
        flag = 1;
        break;
      }
    }
    if (!flag) pos = s->len;
    for (int j = s->len; j > pos; j--) {
      s->data[j] = s->data[j-1];
    }
    s->data[pos] = i;
    s->len += 1;
  }
}
    

void set_remove(struct set *s, int i) {
  assert(s);
  if (set_member(s, i)) {
    assert(s->len);
    int pos = 0;
    for (int j = 0; j < s->len; j++) {
      if (i == s->data[j]) {
        pos = j;
        break;
      }
    }
    for (int j = pos; j < s->len - 1; j++) {
      s->data[j] = s->data[j+1];
    }
    s->len -= 1;
  }
}


struct set *set_union(const struct set *s1, const struct set *s2) {
  assert(s1);
  assert(s2);
  int ulen = s1->len + s2->len;
  struct set *s = malloc(sizeof(struct set));
  s->len = ulen;
  s->max_len = ulen + 1;
  s->data = malloc(s->max_len * sizeof(int));
  if (s1->len == 0) {
    s->data = s2->data;
    return s;
  }
  if (s2->len == 0) {
    s->data = s1->data;
    return s;
  }
  int pos1 = 0;
  int pos2 = 0;
  for (int i = 0; i < ulen; i++) {
    if (pos1 == s1->len) {
      s->data[i] = s2->data[pos2];
      pos2++;
    }
    else if (pos2 == s2->len) {
      s->data[i] = s1->data[pos1];
      pos1++; 
    }
    else if (s1->data[pos1] < s2->data[pos2]) {
      s->data[i] = s1->data[pos1];
      pos1++;
    } else {
      s->data[i] = s2->data[pos2];
      pos2++;
    }
  }
  return s;
}
      

struct set *set_intersect(const struct set *s1, const struct set *s2) {
  assert(s1);
  assert(s2);
  if (s1->len == 0 || s2->len == 0) {
    struct set *s = set_create();
    return s;
  }
  int common = 0;
  for (int i = 0; i < s1->len; i++) {
    for (int j = 0; s1->data[i] >= s2->data[j]; j++) {
      if (s1->data[i] == s2->data[j]) common++;
    }
  } 
  struct set *s = malloc(sizeof(struct set));
  s->len = common;
  s->max_len = common + 1;
  s->data = malloc(s->max_len * sizeof(int));
  int pos = 0;
  for (int i = 0; i < s1->len; i++) {
    for (int j = 0; s1->data[i] >= s2->data[j]; j++) {
      if (s1->data[i] == s2->data[j]) {
        s->data[pos] = s1->data[i];
        pos++;
      }
    }
  }
  return s;
}

  
struct set *array_to_set(const int a[], int len) {
  assert(len);
  int a_dub[len];
  for (int i = 0; i < len; i++) {
    a_dub[i] = a[i];
  }
  merge_sort(a_dub, len);
  int set_len = 1;
  int pos = 0;
  for (int i = 1; i < len; i++) {
    if (a_dub[i] != a_dub[pos]) {
      pos = i;
      set_len++;
    }
  }
  struct set *s = malloc(sizeof(struct set));
  s->len = set_len;
  s->max_len = set_len + 1;
  s->data = malloc(s->max_len * sizeof(int));
  pos = 0;
  int index = 0;
  s->data[index] = a_dub[pos];
  for (int i = 1; i < len; i++) {
    if (a_dub[i] != a_dub[pos]) {
      pos = i;
      index++;
      s->data[index] = a_dub[pos];
    }
  }
  return s;
}
  
  
int *set_to_array(const struct set *s) {
  assert(s);
  if (s->len == 0) return NULL;
  int *array = malloc(s->len * sizeof(int));
  for (int i = 0; i < s->len; i++) {
    array[i] = s->data[i];
  }
  return array;
}

  
void set_print(const struct set *s) {
  assert(s);
  if (s->len) {
    printf("[");
    for (int i = 0; i < s->len - 1; i++) {
      printf("%d,", s->data[i]);
    }
    printf("%d]\n", s->data[s->len - 1]);
  } else printf("[empty]\n");
}

