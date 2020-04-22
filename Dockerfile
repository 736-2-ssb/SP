FROM centos
RUN yum update -y && yum upgrade -y 
RUN yum install -y gcc gdb binutils
COPY sum.s .
COPY sum1.c .
