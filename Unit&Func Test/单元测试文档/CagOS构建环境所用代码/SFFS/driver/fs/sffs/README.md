spi flash file system [specific]

使用限制：
user
1. write：a. 不支持文件修改，必须读出-修改-重新写
        b. 必须按顺序写
2. open 分三种类型：a. 打开文件，如果不存在则返回失败。 
		b. 强制创建新文件，如果存在则先删除已有文件。
		c. 打开文件，如果不存在则创建文件
3. read：
4. close：a. 文件没有被close 过，则将文件头写入flash
        b. 文件之前被close 过，则本次操作不写flash
        c. 没有经过close 的文件在重启后会丢失
5. remove：删除没有关闭的文件会记录一条异常，不影响正常使用，但是如果对删除过的文件没有重新打开就操作会进入错误状态。
6. lseek：最大可以将文件指针偏移到文件末尾，不能产生文件空洞
7. 单个文件大小限制为：2MB
8. 总文件个数限制为：20
9. 单任务，同时只能对一个文件进行操作

driver
1. 约定好的函数接口
2. 提供读、写、擦（sector、sub-sector）接口
3. flash 至少有两个sector：hdr sector 和data sector，并且sector 划分为n 个sub-sector
4. file number <= hdr_sector sub-sector number
5. 

flash 错误处理：
1. 假设read 一般不会出硬件错误，一旦出现fs 只报错，由应用处理（如读出其他文件，然后整片擦除）
2. write 报硬件错误，则先执行一次erase 再尝试写一次，如果还出错就报硬件异常，可以认为这片flash 有问题，不可靠
3. erase 处理同write

写/擦的长度
1. 实验板的flash 只能进行sector erase，开发板的n25q64a 可以进行sub-sector erase
2. 实验板上的hdr sector 需要整块erase，而开发板的hdr 只需要按sub-sector erase 即可
3. 实验板上的写错误只能报错，开发板的写错误可以把当前sub-sector erase 之后再重新write 处理，两次都失败才会报错

报错机制：
1. 返回值FS_OK，FS_FAIL
2. 错误码，记录上一次操作的错误，必须与FS_FAIL 搭配使用，
3. 错误码会被覆盖，而且不会被主动清除
4. 简单的错误不会使用错误码（后续会有列表）

层级：
fs->dfs->driver
sffs->dfs_ops->device->driver

configure file `drv_config.h`: flash,dfs,sffs.


