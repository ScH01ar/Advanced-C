# Advanced-C
> 记录软院网络工程大三上高级C语言编程课程

## 上机作业说明
### 1 2
目录1、2下的可在`unpv13e`目录下自建文件夹，自行编写makefile。
```makefile
include ../Make.defines

PROGS =	cli srv c2 s2 c3 s3

all:	${PROGS}

cli:	cli.o
		${CC} ${CFLAGS} -o $@ cli.o ${LIBS}

srv:	srv.o
		${CC} ${CFLAGS} -o $@ srv.o ${LIBS}
		
s2:		s2.o
		${CC} ${CFLAGS} -o $@ s2.o ${LIBS}
		
c2:	    c2.o
		${CC} ${CFLAGS} -o $@ c2.o ${LIBS}

s3:	    s3.o
		${CC} ${CFLAGS} -o $@ s3.o ${LIBS}		
c3:	    c3.o
		${CC} ${CFLAGS} -o $@ c3.o ${LIBS}		

clean:
		rm -f ${PROGS} ${CLEANFILES}

```
### 3

目录3推荐放在`unpv13e/udpcliserv`目录下，在该目录下的makefile中添加对应文件即可。

运行时多开几个终端分别运行`./mysrv`,`./mycli 127.0.0.1 aa`,`./mycli 127.0.0.1 bb`,`./mycli 127.0.0.1 cc`

