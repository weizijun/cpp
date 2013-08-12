#ifndef __PP_DAEMON_H__
#define __PP_DAEMON_H__

#include <string>

#if (defined(WIN32) || defined(WIN64))
typedef int (* PFDaemonFunc)(int, char **);
//�ṩ��SCP(Service Control Dispatcher)ʹ�ã�����֪ͨSCM���÷���
bool PPLaunchDaemonize(const std::string & strServiceName, 
					   const std::string & strServiceDescription, int Argc, char **Argv);
//�ṩ������ʹ�ã�SCM���÷���ʱ����
int PPDaemonize(const std::string & strServiceName, PFDaemonFunc pDaemonFunc);
bool PPUninstallDaemonize(const std::string & strServiceName);
#else
int PPDaemonize(const std::string & strServiceName);
#endif

#endif
