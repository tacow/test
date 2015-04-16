# 自动构建脚本，尹涛编写
# 修改日期2013/05/06

# 需要编译的子目录，如subdir1:subdir2:subdir3，由冒号分隔，这些子目录中也需要有Makefile
SUBDIRS = 

# 每次都需要重新编译的文件，由冒号分隔
REBUILD_FILES = 

# 附加的编译选项
CFLAGS = 
# 附加的链接选项
LDFLAGS = 
# 宏定义列表，如def1:def2:def3，由冒号分隔，对应gcc的-D选项
DEFINITIONS = 
# 引入头文件目录列表，如inc_dir1:inc_dir2:inc_dir3，由冒号分隔，对应gcc的-I选项
INCLUDE_DIRS = ../../include/
# 引入库文件目录列表，如lib_dir1:lib_dir2:lib_dir3，由冒号分隔，对应gcc的-L选项
LIBRARY_DIRS = 
# 引入库列表，如lib1:lib2:lib3，由冒号分隔，对应gcc的-l选项
LIBRARYS = rt
# 引入模块列表（Release版），如xxx.a:xxx.so，由冒号分隔
MODULES_RELEASE = ../../lib/logger.a
# 引入模块列表（Debug版），如xxx_d.a:xxx_d.so，由冒号分隔
MODULES_DEBUG = ../../lib/logger_d.a
# 目标文件名（Release版）
DESTINATION_RELEASE = logtest
# 目标文件名（Debug版）
DESTINATION_DEBUG = logtest_d
# 编译Release版后执行的命令
AFTER_BUILD_RELEASE = 
# 编译Debug版后执行的命令
AFTER_BUILD_DEBUG = 

# 是否使用PTHREAD线程库，取值为TRUE或者FALSE
USE_PTHREAD = TRUE
# 目标代码平台，32位程序设为32，64位程序设为64，对应gcc的-m选项
DESTINATION_PLATFORM = 
# 目标代码类型，可执行程序设为EXECUTABLE，静态库设为ARCHIVE，合并静态库设为MERGE_ARCHIVE，动态库设为SHARED_OBJECT
DESTINATION_TYPE = EXECUTABLE
# 如果目标代码为动态库，设置soname（Release版），如xxx.so.1
SONAME_RELEASE = 
# 如果目标代码为动态库，设置soname（Debug版），如xxx_d.so.1
SONAME_DEBUG = 

include Makefile.template

