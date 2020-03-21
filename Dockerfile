FROM centos
RUN yum update -y && yum upgrade -y 
RUN yum install passwd -y
RUN useradd -p 13_Caviar0 -s /bin/bash test1
RUN useradd -g test1 -p 13_Caviar0 -s /bin/bash test2
COPY 736ssb.sh .
