#ifndef SERVER_GRID_LBS_DISTANCE_H_
#define SERVER_GRID_LBS_DISTANCE_H_

#include <math.h>
#include"server/grid/lbs_grid.h"
#include"server/grid/lbs_defs.h"

#define PI 3.14159265
#define DEG2RAD(degree) (degree * PI) / 180.0
#define EARTH_RADIUS 6378.137

inline double lbs_distance(double lon1, double lat1, double lon2, double lat2) {
  double rad_lat1 = DEG2RAD(lat1);
  double rad_lat2 = DEG2RAD(lat2);

  double a = rad_lat1 - rad_lat2;
  double b = DEG2RAD(lon1) - DEG2RAD(lon2);

  double s = 2 * asin(sqrt(pow(sin(a / 2), 2) +
                           cos(rad_lat1) * cos(rad_lat2) * pow(sin(b / 2), 2)));
  s = s * EARTH_RADIUS;
  return s * 1000;
}

#endif  // SERVER_GRID_LBS_DISTANCE_H_

inline double Dist_min(lbs_grid_t* lbs_grid, double src_lon, double src_lat, int dis_cell_id, double LBS_LON_MIN, double LBS_LAT_MIN){
  double grid_distance;
  double des_lon;
  double des_lat;
  int src_cell_row = lbs_grid_cell_row(lbs_grid, src_lat);
  int src_cell_col = lbs_grid_cell_col(lbs_grid, src_lon);
  int des_cell_row;
  int des_cell_col;
  lbs_grid_cell_row_col(lbs_grid, dis_cell_id, &(des_cell_row), &(des_cell_col));
  //左上的格子
  if ((des_cell_row > src_cell_row) && (des_cell_col < src_cell_col)){
    des_lon = LBS_LON_MIN + (des_cell_col + 1) * lbs_grid->cell_width;
    des_lat = LBS_LAT_MIN + (des_cell_row) * lbs_grid->cell_height;
    grid_distance = lbs_distance(src_lon, src_lat, des_lon, des_lat);
    }
  //直上的格子
  else if ((des_cell_row > src_cell_row) && (des_cell_col == src_cell_col)){
    des_lon = src_lon;
    des_lat = LBS_LAT_MIN + (des_cell_row) * lbs_grid->cell_height;
    grid_distance = lbs_distance(src_lon, src_lat, des_lon, des_lat);
    }
  //右上的格子
  else if ((des_cell_row > src_cell_row) && (des_cell_col > src_cell_col)){
    des_lon = LBS_LON_MIN + (des_cell_col) * lbs_grid->cell_width;
    des_lat = LBS_LAT_MIN + (des_cell_row) * lbs_grid->cell_height; 
    grid_distance = lbs_distance(src_lon, src_lat, des_lon, des_lat);
    }
  //直左的格子
  else if ((des_cell_row == src_cell_row) && (des_cell_col < src_cell_col)){
    des_lon = LBS_LON_MIN + (des_cell_col + 1) * lbs_grid->cell_width;
    des_lat = src_lat;
    grid_distance = lbs_distance(src_lon, src_lat, des_lon, des_lat);
    }
  //直右的格子
  else if ((des_cell_row == src_cell_row) && (des_cell_col > src_cell_col)){
    des_lon = LBS_LON_MIN + (des_cell_col) * lbs_grid->cell_width;
    des_lat = src_lat;
    grid_distance = lbs_distance(src_lon, src_lat, des_lon, des_lat);    
    }
  //本身的格子
  else if ((des_cell_row == src_cell_row) && (des_cell_col == src_cell_col)){
    grid_distance = 0;    
    }
  //左下的格子
  else if ((des_cell_row < src_cell_row) && (des_cell_col < src_cell_col)){
    des_lon = LBS_LON_MIN + (des_cell_col + 1) * lbs_grid->cell_width;
    des_lat = LBS_LAT_MIN + (des_cell_row + 1) * lbs_grid->cell_height;
    grid_distance = lbs_distance(src_lon, src_lat, des_lon, des_lat);  
    }
  //直下的格子
  else if ((des_cell_row < src_cell_row) && (des_cell_col == src_cell_col)){
    des_lon = src_lon;
    des_lat = LBS_LAT_MIN + (des_cell_row + 1) * lbs_grid->cell_height;
    grid_distance = lbs_distance(src_lon, src_lat, des_lon, des_lat);   
    }
  //右下的格子
  else{
    des_lon = LBS_LON_MIN + (des_cell_col) * lbs_grid->cell_width;
    des_lat = LBS_LAT_MIN + (des_cell_row + 1) * lbs_grid->cell_height;
    grid_distance = lbs_distance(src_lon, src_lat, des_lon, des_lat);  
    }
  return grid_distance;
}
