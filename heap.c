/*
 * This is an implementation of a generic binary min-heap for int64_bit
 * numbers
 * Implemented by Victor Cheruiyot
 */
#include <stddef.h> 
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef void(*call_free)(void *);

typedef struct min_heap{
	size_t actual_count;
	size_t heap_size;
	size_t elemsize;
	void (*free_fn)(void*);
	void *elements;
}min_heap;

min_heap *initialize_heap(size_t elemsize, call_free f){
	min_heap *heap = malloc(sizeof(min_heap));
	heap->actual_count = 1;
	heap->heap_size = 10;
	heap->elemsize = elemsize;
	heap->elements = malloc(elemsize * heap->heap_size);
	heap->free_fn = f;
	return heap;
}
void check_heap_size(min_heap **heap){
	min_heap *cur = *heap;
	if(cur->heap_size == cur->actual_count){
		cur->heap_size *= 2;
		void *bigger = realloc(cur->elements, cur->elemsize * cur->heap_size);
		assert(bigger != NULL);
		cur->elements = bigger;
	}
	return;
}
int compar(void *a, void *b){
	return *(int *)a - *(int *)b;
}
void swap(void *parent, void *child, size_t elemsize){
	void *temp = malloc(elemsize);
	memcpy(temp, parent, elemsize);
	memcpy(parent, child, elemsize);
	memcpy(child, temp, elemsize);
	free(temp);
}
void bubble_up(min_heap **heap){
	void *arr = (*heap)->elements;
	size_t last_element = (*heap)->actual_count;
	while(last_element/2){
		void *child = (char *)arr + last_element * (*heap)->elemsize;
		void *parent = (char *)arr + last_element/2 * (*heap)->elemsize;
		int comparison = compar(child, parent);
		if(comparison < 0){
			swap(parent, child, (*heap)->elemsize);
		}
		last_element /= 2;
	}
}
void push(min_heap *heap, void *value){
	check_heap_size(&heap);
	void *arr = heap->elements;
	void *cur = (char *)arr + heap->actual_count * heap->elemsize;
	memcpy(cur, value, heap->elemsize);
	bubble_up(&heap);
	heap->actual_count++;
	if(heap->free_fn){
		heap->free_fn(value);
	}
}
void bubble_down(min_heap *heap, size_t start_index){
	void *arr = heap->elements;
	size_t total_elems = heap->actual_count;
	while(true){
		void *parent = (char *)arr + start_index * heap->elemsize;
		void *left_child = (char *)arr + (start_index * 2 * heap->elemsize);
		void *right_child = (char *)arr + (start_index * 2 + 1) * heap->elemsize;
		int left_compare = 0;
		if(start_index * 2 < total_elems){
			left_compare = compar(parent, left_child);
		}
		int right_compare = 0;
		if((start_index * 2 + 1) < total_elems){
			right_compare = compar(parent, right_child);
		}
		if(right_compare == 0 && left_compare == 0){
			break;
		}
		if(left_compare > 0 && left_compare > right_compare){
			swap(parent, left_child, heap->elemsize);
			start_index *= 2;
			continue;
		}
		if(right_compare > 0){
			swap(parent, right_child, heap->elemsize);
			start_index = start_index * 2 + 1;
			continue;
		}
		break;
	
	}
}
void *min(min_heap *heap){
	return (char *)heap->elements + (1 * heap->elemsize);
}

void heap_remove(min_heap *heap, void *value){
	void *arr = heap->elements;
	size_t arr_size = heap->actual_count;
	for(size_t i = 1; i < arr_size; i++){
		void *element = (char *)arr + (i * heap->elemsize);
		int found = compar(element, value);
		if(!found){
			void *last_element = (char *)arr + (heap->actual_count - 1) * heap->elemsize;
			memmove(element, last_element, heap->elemsize);
			heap->actual_count--; //need to free that block as well
			bubble_down(heap, i);
			break;
		}
	}
}
void print_heap(min_heap *heap){
	void *arr = heap->elements;
	size_t count = heap->actual_count;
	printf("[");
	int i;
	for(i = 1; i < count - 1; i++){
		int value = *(int *)((char *)arr + i * heap->elemsize);
		printf("%d ", value);
	}
	if (heap->actual_count > 1){
		int value = *(int *)((char *)arr + i * heap->elemsize);
		printf("%d]\n", value);
	}else{
		printf("]\n");
	}
}
void print_minimum(min_heap *heap){
	int minimum = *(int *)min(heap);
	printf("%d\n", minimum);
}
void run_query(min_heap *heap, int type, int data){
	switch(type){
		case 1:
			print_heap(heap);
			break;
		case 2:
			heap_remove(heap, &data);
			break;
		case 3:
			print_minimum(heap);
			break;
	}
}
int main(){
	int n_query;
	scanf("%d", &n_query);
	min_heap *heap = initialize_heap(sizeof(int), NULL);
	for(int i = 0; i < n_query; i++){
		int heap_query_type = 0;
		int data = 0;
		scanf("%d", &heap_query_type);
		if (heap_query_type != 3){
			scanf("%d", &data);
		}
		run_query(heap, heap_query_type, data);
	}
	free(heap->elements);
	free(heap);
}
