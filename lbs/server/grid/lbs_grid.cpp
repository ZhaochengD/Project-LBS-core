#include "server/grid/lbs_grid.h"
#include "server/grid/lbs_defs.h"
#include "server/grid/lbs_queue.h"
#include <stdio.h>
#include <stdlib.h>

//scp -r sx3@123.57.237.171:~/lbs .
//scp lbs_grid.cpp sx3@123.57.237.171:~/lbs/server/grid
int lbs_grid_init(lbs_grid_t* lbs_grid, double lon1, double lon2, double lat1, double lat2, int row_num, int col_num){
 lbs_grid->row_num = row_num;
 lbs_grid->col_num = col_num;
 lbs_grid->lon_min = lon1;
 lbs_grid->lat_min = lat1;
 lbs_grid->cell_width = (lon2-lon1)/col_num;
 lbs_grid->cell_height = (lat2-lat1)/row_num; 
 lbs_hashtable_init(&lbs_grid->hash_table);
 lbs_grid->cell = (lbs_cell_t* )malloc(row_num*col_num*sizeof(lbs_cell_t));
 for (int i = 0; i < row_num*col_num; i++){
   lbs_queue_init(&(lbs_grid->cell[i].dammy_node.queue));     
    }
 return 0;
}


//网格的删除
int lbs_grid_destroy(lbs_grid_t* lbs_grid){
  free(lbs_grid->cell);
  for (int i = (lbs_grid->row_num) * (lbs_grid->col_num); i == 0; i--){
      free(lbs_grid -> cell + i);    
      }
  return 0;
}


//更新移动位置
int lbs_grid_update(lbs_grid_t* lbs_grid, double lon, double lat, uint64_t timestamp, uint32_t id){
  lbs_hashnode_t* hashnode = lbs_hashtable_get(&(lbs_grid->hash_table), id);
  int cell_row_new = lbs_grid_cell_row(lbs_grid, lat);
  int cell_col_new = lbs_grid_cell_col(lbs_grid, lon);
  int cell_id_new = lbs_grid_cell_id(lbs_grid, cell_row_new, cell_col_new);
int cell_id_old = 0;
  if (hashnode != NULL) {
 cell_id_old = hashnode->cell_id;
}
  //在没有该节点的情况下
  if (hashnode == NULL){
    //更新mov_node
    lbs_mov_node_t* new_lbs_mov_node = (lbs_mov_node_t* )malloc(sizeof(lbs_mov_node_t));
    //new_lbs_queue_init(lbs_mov_node->queue);
    new_lbs_mov_node->lon = lon;
    new_lbs_mov_node->lat = lat;
    new_lbs_mov_node->id = id;
    new_lbs_mov_node->timestamp = timestamp;
    //重设hashnode
    int temp = lbs_hashtable_set(&(lbs_grid->hash_table), id, new_lbs_mov_node, cell_id_new);
    lbs_queue_insert_head(&(lbs_grid->cell[cell_id_new].dammy_node.queue), &(new_lbs_mov_node->queue));
    return 0;
    }
    
    //在需要更新的情况下
    else if (cell_id_new != cell_id_old){
      hashnode->mov_node->lon = lon;
      hashnode->mov_node->lat = lat;
      hashnode->mov_node->timestamp = timestamp;
      hashnode->cell_id = cell_id_new;
      lbs_queue_remove(&(hashnode->mov_node->queue));
      lbs_queue_insert_head(&(lbs_grid->cell[cell_id_new].dammy_node.queue), &(hashnode->mov_node->queue));
    }
 
    //在不需要更新的情况下
    else{
      cell_id_old = hashnode -> cell_id;
      hashnode->mov_node->lon = lon;
      hashnode->mov_node->lat = lat;
      hashnode->mov_node->timestamp = timestamp;
    }  
    
    return 0;
}

//计算cell row
int lbs_grid_cell_row(lbs_grid_t* lbs_grid, double lat) {
  int row = (lat - (lbs_grid->lat_min)) / (lbs_grid->cell_height);
  return row;
}

//计算cell col
int lbs_grid_cell_col(lbs_grid_t* lbs_grid, double lon) {
  int col = (lon - (lbs_grid->lon_min)) / (lbs_grid->cell_width);
  return col;
}

//计算cell id
int lbs_grid_cell_id(lbs_grid_t* lbs_grid, int cell_row, int cell_col) {
  int id = (lbs_grid->col_num) * cell_row + cell_col;
  return id;
}

//计算row 和 col
void lbs_grid_cell_row_col(lbs_grid_t* lbs_grid, int cell_id, int* cell_row, int* cell_col) {
  *cell_row = cell_id / lbs_grid->col_num;
  *cell_col = cell_id % lbs_grid->col_num;
}

//获取cell id 里面的 cell
lbs_cell_t* lbs_grid_cell(lbs_grid_t* lbs_grid, int cell_id){
  if (lbs_grid!= NULL){
    return &(lbs_grid->cell[cell_id]);
  }
}
