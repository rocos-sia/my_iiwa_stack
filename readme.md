## 这个分支是当时帮王天赐配置iiwa-stack时版本
## 优点：
  1. 直接 `roslaunch iiwa_tool_moveit  test.launch  `就可以启动rviz显示，同时已经配置好工具了
  2. 如果需要更改工具，理论上需要更改sunrise客户端的工具偏移(用于选择运动工具系)  和 URDF文件里工具偏移(用于rviz的显示和iiwa-java的tf查询)
  3. 如果需要实现笛卡尔空间的直线运动或之类，frame字段需要填入数据参考的坐标系，如iiwa_link_0
