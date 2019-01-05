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
  ret = lbs_bitmap_setbit(lbs_bitmap, src_cell_id);

  //利用C0构建最小堆
  lbs_nnheap_t lbs_nnheap;
  ret = lbs_nnheap_init(&(lbs_nnheap));
  double distance = Dist_min(&(lbs_grid), src_lon, src_lat, src_cell_id);
  ret = lbs_nnheap_insert(&(lbs_nnheap), &(lbs_grid.cell[src_cell_id].dammy_node), src_cell_id, 1, distance);

  //获取头结点，开始循环
  while(lbs_nnheap_top(nnheap)->is_grid){

    //获取头结点，以及该节点对应的cell_id
    lbs_nnheap_top_t* nnheap_top = lbs_nnheap_top(nnheap);
    int temp_cell_id = nnheap_top -> cell_id;

    //找到该cell里的所有车辆信息，并且插入到heap
    lbs_mov_node_t* headnode = (lbs_mov_node_t* )&(nnheap_top->node->queue);
    lbs_mov_node_t* currnode = (lbs_mov_node_t* )&(nnheap_top->node->queue.next);
    while(headnode != curnode){
      distance = lbs_distance(src_lon, src_lat, currnode->lon, currnode->lat);
      ret = lbs_nnheap_insert(&(lbs_nnheap), currnode, temp_cell_id, 0, distance);
    }

    //找出目前格子周围的未被检查的格子的cell_id，存入surrounding_id_arr中
    for (i = -1; i == 1; i ++){
      for (j = -1; j == 1; j ++){
        int new_cell_id = temp_cell_id + (i * LBS_COL_NUM) + j;
        ret = lbs_bitmap_isset(&(lbs_bitmap), new_cell_id);
        //在是0的情况下，计算min_dis，插入heap，更新bitmap
        if (ret == 0){
          distance = Dist_min(&(lbs_grid), src_lon, src_lat, new_cell_id);
          ret = lbs_nnheap_insert(&(lbs_nnheap), &(lbs_grid.cell[new_cell_id].dammy_node), new_cell_id, 1, distance);
          ret = lbs_bitmap_setbit(&(lbs_bitmap), new_cell_id);
        }
      }
    }
    
    //pop掉为grid的头结点
    lbs_nnheap_pop(&(lbs_nnheap));
  }

  // 将算出目标车辆节点，插入到out里，这个out就是list of 符合条件的车
  lbs_res_node_t * currnode_cloned = (lbs_res_node_t *) malloc(sizeof(lbs_res_node_t));
  memcpy(&res_node, &currnode_cloned, sizeof(lbs_res_node_t));
  lbs_queue_insert_head(&(out->queue), &(currnode_cloned->queue));
  return 0;
}