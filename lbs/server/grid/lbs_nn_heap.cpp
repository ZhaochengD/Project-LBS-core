#include "server/grid/lbs_nn_heap.h"

#include <stdlib.h>
#include <string.h>

int lbs_nnheap_init(lbs_nnheap_t* lbs_nnheap){
  	lbs_nnheap->size = 0;
  	lbs_nnheap->capacity = 10;
	lbs_nnheap->heap_nodes = (lbs_heapnode_t* )calloc(lbs_nnheap->capacity, sizeof(lbs_heapnode_t));
  	return 0;
}

int lbs_nnheap_destroy(lbs_nnheap_t* lbs_nnheap){
  	free(lbs_nnheap->heap_nodes);
}

int lbs_nnheap_insert(lbs_nnheap_t* lbs_nnheap, lbs_mov_node_t* lbs_mov_node,
                       int cell_id, uint8_t is_grid, double distance){
  //新建一个heapnode
  	lbs_heapnode_t new_heapnode;
  	new_heapnode.is_grid = is_grid;
  	new_heapnode.cell_id = cell_id;
  	new_heapnode.node = lbs_mov_node;
  	new_heapnode.distance = distance;
    
  //在capacity已经被填满以后，重新分配内存。插入新节点
  	if (lbs_nnheap->size == lbs_nnheap->capacity){
    	lbs_nnheap->heap_nodes = (lbs_heapnode_t* )realloc(lbs_nnheap->heap_nodes, 2* lbs_nnheap->capacity*(sizeof(lbs_heapnode_t))); 
		lbs_nnheap->capacity *= 2;
	}
	int size = lbs_nnheap->size;
  	int new_node_index = size + 1;
  	lbs_nnheap->heap_nodes[new_node_index-1] = new_heapnode;
  
  //取出新插入的节点和其父节点的秩，以及一个中间节点
  	lbs_nnheap->size++;
  	int child = lbs_nnheap->size;
  	int parent = (child) / 2;
  	lbs_heapnode_t temp_heapnode;

  //互换直至插入节点到达正确的位置
  	while (lbs_nnheap->heap_nodes[parent-1].distance > lbs_nnheap->heap_nodes[child-1].distance){
    	temp_heapnode = lbs_nnheap->heap_nodes[child-1];
    	lbs_nnheap->heap_nodes[child-1] = lbs_nnheap->heap_nodes[parent-1];
    	lbs_nnheap->heap_nodes[parent-1] = temp_heapnode;
		child = parent;
    	parent = (parent) / 2;
		if (parent == 0){
			break;
		}
    }
  	return 0;
}

lbs_heapnode_t *lbs_nnheap_top(lbs_nnheap_t *lbs_nnheap){
  	return lbs_nnheap->heap_nodes;
}

void lbs_nnheap_pop(lbs_nnheap_t *lbs_nnheap){
  	//将头结点替换为最后一个节点，并释放最后一个节点的内存
  	lbs_nnheap->heap_nodes[0] = lbs_nnheap->heap_nodes[lbs_nnheap->size-1];
  	lbs_nnheap->size--;
 	//free(lbs_nnheap->heap_nodes + size);数组的空间是连续的，不能一次free掉，链表的内存分配是不连续的所以必须借助一个循环体，不停地释放
	lbs_heapnode_t temp_heapnode;
  	//计算父节点和左右子节点的秩
  	int parent = 0;
  	int l_child = 2 * parent + 1;
  	int r_child = 2 * parent + 2;
	int min_child;
  	while (l_child < lbs_nnheap->size) {  
		//if have r_child
		if (r_child < lbs_nnheap->size){
		  	//if so, find min child
			if (lbs_nnheap->heap_nodes[r_child].distance < lbs_nnheap->heap_nodes[l_child].distance){
				min_child = r_child;
			}
			else{
				min_child = l_child;
			}
		}
		// if min < parent
		if (lbs_nnheap->heap_nodes[min_child].distance < lbs_nnheap->heap_nodes[parent].distance){
			//modify parent,l_child,r_child;
			temp_heapnode = lbs_nnheap->heap_nodes[min_child];
        	lbs_nnheap->heap_nodes[min_child] = lbs_nnheap->heap_nodes[parent];
        	lbs_nnheap->heap_nodes[parent] = temp_heapnode;
        	parent = min_child;
		}			
		//else break;
		else{
			break;
		}
      	l_child = 2 * parent + 1;
      	r_child = 2 * parent + 2;
    }      
}