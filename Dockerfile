FROM centos
RUN yum update -y && yum upgrade -y
RUN groupunstall "Development Tools" -y
COPY parent.cpp .
COPY children.cpp .
