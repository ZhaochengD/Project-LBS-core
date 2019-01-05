这个项目是在中科院实习时做的项目，看似巨大，实际上我们只完成了滴滴搜寻最近车辆最核心的算法部分．原文件位于`lbs/server/grid/`这个目录，这里提前将它们放在现在这个目录．

|文件|作用|
|-|-|
|lbs_bitmap.h|位图数据结构的头文件|
|lbs_bitmap.cpp|位图数据结构的源文件|
|lbs_grid.h|网格数据结构的头文件|
|lbs_grid.cpp|网格数据结构的源文件|
|lbs_hashtable.h|哈希表数据结构的头文件|
|lbs_hashtable.cpp|哈希表数据结构的源文件|
|lbs_nn_heap.h|堆数据结构的头文件|
|lbs_nn_heap.cpp|堆数据结构的的源文件|
|lbs_defs.h|网格索引的内部存储单元|
|lbs_distance.h|计算top1问题的网格索引算法|
|nn_query.cpp|基于堆数据结构的topk问题解决方案|
|lbs_index.h|网格查询某辆车位置的头文件|
|lbs_index.cpp|网格查询某辆车位置的源文件|
|lbs|项目内的全部代码|
|PPT|参考文献|