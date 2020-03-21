FROM amazonlinux
RUN yum -y install sudo
RUN sudo yum update -y && yum upgrade -y 
RUN sudo yum install passwd -y
RUN sudo useradd -p 13_Caviar0 -s /bin/bash test1
RUN sudo useradd -g test1 -p 13_Caviar0 -s /bin/bash test2
COPY script3.sh .
