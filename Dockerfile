FROM centos
RUN yum update -y && yum upgrade -y 
RUN yum groupinstall 'Development Tools' -y
COPY lab5_6.cpp .
