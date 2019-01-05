#include "server/grid/lbs_index.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/log.h"

#include "server/grid/lbs_distance.h"
#include "server/grid/lbs_grid.h"
#include "server/grid/lbs_bitmap.h"
#include "server/grid/lbs_nn_heap.h"

#define LBS_LON_MIN 116
#define LBS_LON_MAX 117
#define LBS_LAT_MIN 39
#define LBS_LAT_MAX 41

#define LBS_ROW_NUM 200
#define LBS_COL_NUM 100

static lbs_grid_t lbs_grid;

// 初始化网格索引
int lbs_grid_index_init() {
  // TODO: by student
  int ret = lbs_grid_init(&lbs_grid, LBS_LON_MIN, LBS_LON_MAX, LBS_LAT_MIN, LBS_LAT_MAX, LBS_ROW_NUM, LBS_COL_NUM);
  return 0;
}

// 更新接口[出租车位置更新]
int lbs_grid_index_update(double lon,
                           double lat,
                           uint64_t timestamp,
                           uint32_t id) {
  // TODO: by student
  int ret = lbs_grid_update(&lbs_grid, lon, lat, timestamp, id);
  return 0;
}

// 范围查询接口[查询某一范围内的所有出租车信息]
int lbs_grid_index_range_query(double lon1,
                               double lon2,
                               double lat1,
                               double lat2,
                               lbs_res_node_t* out) {
  int row1 = lbs_grid_cell_row(&lbs_grid, lat1);
  int col1 = lbs_grid_cell_col(&lbs_grid, lon1);
  int row2 = lbs_grid_cell_row(&lbs_grid, lat2);
  int col2 = lbs_grid_cell_row(&lbs_grid, lon2);
  //对每一个格子进行遍历
  for (int i = row1; i < row2; i ++){
    for (int j = col1; j < col2; j ++){
      int cell_id = lbs_grid_cell_id(&lbs_grid, i, j);
      //找到id对应的cell的地址
      lbs_cell_t* lbs_cell = lbs_grid_cell(&lbs_grid, cell_id);
      //对该cell里的双向列表进行遍历
      lbs_mov_node_t* headnode = (lbs_mov_node_t* )&(lbs_cell->dammy_node.queue);
      lbs_mov_node_t* currnode = (lbs_mov_node_t* )(lbs_cell->dammy_node.queue.next);
      while(currnode != headnode){
        if ((currnode->lon <= lon2) && (currnode->lon >= lon1) && (currnode->lat >= lat1) && (currnode->lat <= lat2)){
          lbs_res_node_t* currnode_cloned = (lbs_res_node_t* )malloc(sizeof(lbs_mov_node_t));
          memcpy(currnode_cloned, currnode, sizeof(lbs_res_node_t));
          lbs_queue_insert_head(&(out->queue), &(currnode_cloned->queue));
          }
        currnode = (lbs_mov_node_t* )currnode->queue.next;
          }
      }      
    }
  return 0;
}

// NN查询接口[查询离lon,lat最近的出租车]
// NN查询接口[查询离lon,lat最近的出租车]
int lbs_grid_index_nn_query(double lon, double lat, lbs_res_node_t* out) {
  
  //获取第src车所在的位置和网格信息
  double src_lon = lon;
  double src_lat = lat;
  int src_cell_row = lbs_grid_cell_row(&(lbs_grid), src_lat);
  int src_cell_col = lbs_grid_cell_col(&(lbs_grid), src_lon);
  int src_cell_id = lbs_grid_cell_id(&(lbs_grid), src_cell_row, src_cell_col);

  //初始化bitmap，并将src车所在的cell0记录在bitmap里
  lbs_bitmap_t lbs_bitmap;
  int ret = lbs_bitmap_init(&(lbs_bitmap), (lbs_grid.col_num*lbs_grid.row_num));
  ret = lbs_bitmap_setbit(&(lbs_bitmap), src_cell_id);

  //利用C0构建最小堆
  lbs_nnheap_t lbs_nnheap;
  ret = lbs_nnheap_init(&(lbs_nnheap));
  double distance = 0;
  ret = lbs_nnheap_insert(&(lbs_nnheap), &(lbs_grid.cell[src_cell_id].dammy_node), src_cell_id, 1, distance);

  //获取头结点，开始循环
  while(lbs_nnheap_top(&(lbs_nnheap))->is_grid){

    //获取头结点，以及该节点对应的cell_id
    lbs_heapnode_t* nnheap_top = lbs_nnheap_top(&(lbs_nnheap));
    int temp_cell_id = nnheap_top -> cell_id;


    //找到该cell里的所有车辆信息，并且插入到heap
    lbs_mov_node_t* headnode = (lbs_mov_node_t* )(nnheap_top->node);
    lbs_mov_node_t* currnode = (lbs_mov_node_t* )(nnheap_top->node->queue.next);
   
    //pop掉为grid的头结点
    lbs_nnheap_pop(&(lbs_nnheap));

    while(headnode != currnode){
      distance = lbs_distance(src_lon, src_lat, currnode->lon, currnode->lat);
      ret = lbs_nnheap_insert(&(lbs_nnheap), currnode, temp_cell_id, 0, distance);
      currnode = (lbs_mov_node_t*)currnode->queue.next;
    }
    //找出目前格子周围的未被检查的格子的cell_id，存入surrounding_id_arr中
    for (int i = -1; i == 1; i ++){
      for (int j = -1; j == 1; j ++){
        int new_cell_id = temp_cell_id + (i * LBS_COL_NUM) + j;
        ret = lbs_bitmap_isset(&(lbs_bitmap), new_cell_id);
        
        //在是0的情况下，计算min_dis，插入heap，更新bitmap
        if (ret == 0){
          distance = Dist_min(&(lbs_grid), src_lon, src_lat, new_cell_id, LBS_LON_MIN, LBS_LAT_MIN);
          ret = lbs_nnheap_insert(&(lbs_nnheap), &(lbs_grid.cell[new_cell_id].dammy_node), new_cell_id, 1, distance);
          ret = lbs_bitmap_setbit(&(lbs_bitmap), new_cell_id);
        }
      }
    }
  }

  // 将算出目标车辆节点，插入到out里，这个out就是list of 符合条件的车
  lbs_heapnode_t* nnheap_top = lbs_nnheap_top(&(lbs_nnheap));
  lbs_res_node_t * currnode_cloned = (lbs_res_node_t *) malloc(sizeof(lbs_res_node_t));
  memcpy(currnode_cloned, nnheap_top->node, sizeof(lbs_res_node_t));
  lbs_queue_insert_head(&(out->queue), &(currnode_cloned->queue));
  return 0;
}
