2017-09-27
版本1.0
Android4.4.4（dalvik vm ）可以完成动态修改，
  src/main/cpp/native-lib.cpp
  src/main/cpp/mandroid/MDalvik.cpp
  具体查看这两个文件实现。
 但是在5.0系统修改失败。具体还要分析art系统执行解析dex文件处理方式。
 **************************************************************