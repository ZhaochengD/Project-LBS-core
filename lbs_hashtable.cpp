#include "server/grid/lbs_hashtable.h"
#include "server/grid/lbs_defs.h"
#include "server/grid/lbs_queue.h"
#include <stdio.h>
#include <stdlib.h>

//scp -r sx3@123.57.237.171:~/lbs .


//初始化
int lbs_hashtable_init(lbs_hashtable_t* lbs_hashtable){
  lbs_hashtable->size = 0;
  lbs_hashtable->capacity = 19997;
  //((lbs_hashtable_t *)强制数据类型转化，malloc动态分配，sizeof()计算内存大小，)
  lbs_hashtable->hash_nodes = (lbs_hashnode_t *)malloc(lbs_hashtable->capacity * sizeof(lbs_hashnode_t));
  for (int i = 0; i < lbs_hashtable->capacity; i++){
    //当传入的是struct变量，而不是指针，例如下文中的queue，使用.访问成员，不用分配内存
    lbs_queue_init(&(lbs_hashtable->hash_nodes[i].queue));  
  }
  return 0;
}
//
int lbs_hashtable_destroy(lbs_hashtable_t* lbs_hashtable){
  lbs_hashtable->capacity = 0;
  lbs_hashtable->size = 0;
  free(lbs_hashtable->hash_nodes);
}

int lbs_hashtable_set(lbs_hashtable_t * lbs_hashtable,
                      uint32_t id, lbs_mov_node_t * lbs_mov_node, int cell_id){
  lbs_hashnode_t* hash_nodes = (lbs_hashnode_t*)malloc(sizeof(lbs_hashnode_t));
  hash_nodes->cell_id = cell_id;
  hash_nodes->mov_node = lbs_mov_node;
  lbs_queue_init(&(hash_nodes->queue));
  int index = id % (lbs_hashtable->capacity);
  lbs_queue_insert_head(&(lbs_hashtable->hash_nodes[index].queue),&(hash_nodes->queue));
  return 0;
}

lbs_hashnode_t* lbs_hashtable_get(lbs_hashtable_t* lbs_hashtable, uint32_t id){
  //初始化头结点和头结点的下一个节点
  int index = id % (lbs_hashtable->capacity);
  lbs_hashnode_t* headnode = lbs_hashtable->hash_nodes + index;
  lbs_hashnode_t* curnode = (lbs_hashnode_t* )headnode->queue.next;
  //不断迭代，直到找到相同的id为止
  while(headnode != curnode){
    if (curnode->mov_node->id == id){
      return curnode;      
      }
    else{
      curnode = (lbs_hashnode_t* )curnode->queue.next;
      }
    }
  return NULL;
}