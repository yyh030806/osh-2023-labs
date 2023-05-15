# lab3 实验报告

## 一、编译和运行方法

1. 编译：

   a.多线程版本：gcc server.c  -lpthread -o server

   b.select I/O复用版本：gcc server_select.c -o server_select

2. 运行：

   a.多线程版本：./server

   b.select I/O复用版本：./server_select

## 二、整体设计和技术

1.解析和检验http头：对于异常请求返回500 Inter Server Error

2.实现请求资源内容：根据路径访问请求资源，若存在，返回内容以及内容长度；若不存在，返回 404 Not Found

3.多线程版本：在原有server的基础上，将处理请求的部分，使用线程处理技术，创建独立的线程进                   行，提高了并发性

4.select 版本：在原有server的基础上，使用了select I/O复用技术，通过描述符集合，和select的函数，对多个socket进行监听，大大提高了性能

## 三、测试结果和分析

测试参数：siege -c 50 -t 10s http://127.0.0.1:8000/index.html

（由于未知原因，需要设置时间参数来停止测试，故本次测试采用固定时间变量的方法）

1.原始版本测试结果：

Transactions:		       21936 hits
Availability:		      100.00 %
Elapsed time:		        9.95 secs
Data transferred:	        0.21 MB
Response time:		        0.01 secs
Transaction rate:	     2204.62 trans/sec
Throughput:		        0.02 MB/sec
Concurrency:		       24.41
Successful transactions:       21936
Failed transactions:	           0
Longest transaction:	        7.77
Shortest transaction:	        0.00

2.多线程版本测试结果：

Transactions:		       25971 hits
Availability:		       99.98 %
Elapsed time:		        9.20 secs
Data transferred:	        0.25 MB
Response time:		        0.00 secs
Transaction rate:	     2822.93 trans/sec
Throughput:		        0.03 MB/sec
Concurrency:		        4.85
Successful transactions:       25971
Failed transactions:	           5
Longest transaction:	        2.78
Shortest transaction:	        0.00

3.select 版本测试结果：

Transactions:		       23308 hits
Availability:		      100.00 %
Elapsed time:		       10.02 secs
Data transferred:	        0.22 MB
Response time:		        0.01 secs
Transaction rate:	     2326.15 trans/sec
Throughput:		        0.02 MB/sec
Concurrency:		       33.82
Successful transactions:       23308
Failed transactions:	           0
Longest transaction:	        7.73
Shortest transaction:	        0.00

结果分析：多线程在性能上相对于原始版本有较大提升，但是可靠性有微小下降。select I/O复用在性能上有较小提升，可靠性不变。 