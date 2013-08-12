#ifndef __PP_DAEMON_H__
#define __PP_DAEMON_H__

#include <string>

#if (defined(WIN32) || defined(WIN64))
typedef int (* PFDaemonFunc)(int, char **);
//提供给SCP(Service Control Dispatcher)使用，程序通知SCM调用服务
bool PPLaunchDaemonize(const std::string & strServiceName, 
					   const std::string & strServiceDescription, int Argc, char **Argv);
//提供给服务使用，SCM调用服务时启动
int PPDaemonize(const std::string & strServiceName, PFDaemonFunc pDaemonFunc);
bool PPUninstallDaemonize(const std::string & strServiceName);
#else
int PPDaemonize(const std::string & strServiceName);
#endif

#endif
