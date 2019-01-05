#include "server/grid/lbs_index.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/log.h"

#include "server/grid/lbs_distance.h"
#include "server/grid/lbs_grid.h"

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
  int cell_row_min = lbs_grid_cell_row(&lbs_grid, lat1);
  int cell_row_max = lbs_grid_cell_row(&lbs_grid, lat2);
  int cell_col_min = lbs_grid_cell_col(&lbs_grid, lon1);
  int cell_col_max = lbs_grid_cell_col(&lbs_grid, lon2);
  for (int r = cell_row_min; r <= cell_row_max; r++){
    for (int c = cell_col_min; c<=cell_col_max; c++){
      int curr_cell_id = lbs_grid_cell_id(&lbs_grid, r, c);
        lbs_queue_t* head_queue = &(lbs_grid.cell[curr_cell_id].dammy_node.queue);
        lbs_queue_t* temp_queue = (lbs_grid.cell[curr_cell_id].dammy_node.queue).next;
        while (temp_queue != head_queue){
          lbs_mov_node_t * temp_mov_node = (lbs_mov_node_t *) temp_queue;
          // get info inside 以及判断
          if(temp_mov_node->lon >= lon1 && temp_mov_node->lon <= lon2 &&
             temp_mov_node->lat >= lat1 && temp_mov_node->lat <= lat2){
               // 符合条件，做一个deepcopy然后加进 out list
              lbs_res_node_t * deepcopy_mov_node = (lbs_res_node_t *) malloc(sizeof(lbs_res_node_t));
              memcpy(deepcopy_mov_node, temp_mov_node, sizeof(lbs_res_node_t));
              lbs_queue_insert_head(&(out->queue), &(deepcopy_mov_node->queue));
          }
          temp_queue = (lbs_queue_t *)temp_mov_node->queue.next;
        }//end while
    }
  }
  return 0;
}



// NN查询接口[查询离lon,lat最近的出租车]
int lbs_grid_index_nn_query(double lon, double lat, lbs_res_node_t* out) {
  // 这个out就是list of 符合条件的车
  return 0;
}
                                                                                                                             1,1          顶端
