#include "ppdaemon.h"

#if (defined(WIN32) || defined(WIN64))
#include <windows.h>

static std::string g_strDaemonName;
static PFDaemonFunc g_PFDaemonFunc;
static SERVICE_STATUS g_Status;
static SERVICE_STATUS_HANDLE g_hServiceStatus;

static BOOL IsInstalled()
{
    BOOL bResult = FALSE;

    //打开SCM(Service Control Manager)服务控制管理器
    SC_HANDLE hSCM = ::OpenSCManager(NULL, //本地机器
                                     NULL, //缺省数据库
                                     SC_MANAGER_ALL_ACCESS); //所有权限的管理器
    if (NULL != hSCM)
	{
        //尝试打开服务
        SC_HANDLE hService = ::OpenService(hSCM,
                                           g_strDaemonName.c_str(),
                                           SERVICE_QUERY_CONFIG);
        if (NULL != hService)
		{
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }

        ::CloseServiceHandle(hSCM);
    }
    
    return bResult;
}

static BOOL Install()
{
    //打开SCM(Service Control Manager)服务控制管理器
    SC_HANDLE hSCM = ::OpenSCManager(NULL, //本地机器
                                     NULL, //缺省数据库
                                     SC_MANAGER_ALL_ACCESS); //所有权限的管理器
	if (NULL == hSCM)
	{
		return FALSE;
	}

    //获取执行文件路径
    char t_szFilePath[MAX_PATH];
    ::GetModuleFileName(NULL, t_szFilePath, sizeof(t_szFilePath));

    //创建服务
    SC_HANDLE hService = ::CreateService(hSCM,
										 g_strDaemonName.c_str(),
                                         g_strDaemonName.c_str(),
                                         SERVICE_ALL_ACCESS,
                                         SERVICE_WIN32_OWN_PROCESS,
                                         SERVICE_DEMAND_START,        // start condition
                                         SERVICE_ERROR_NORMAL,
                                         t_szFilePath,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL);
    if (NULL == hService)
	{
        ::CloseServiceHandle(hSCM);
        return FALSE;
    }

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

static BOOL Uninstall()
{
    //打开SCM(Service Control Manager)服务控制管理器
    SC_HANDLE hSCM = ::OpenSCManager(NULL, //本地机器
                                     NULL, //缺省数据库
                                     SC_MANAGER_ALL_ACCESS); //所有权限的管理器
    if (NULL == hSCM)
	{
		return FALSE;
	}

    BOOL bResult = FALSE;
    SC_HANDLE hService = ::OpenService(hSCM,
                                       g_strDaemonName.c_str(),
                                       SERVICE_STOP | DELETE);
    if (NULL != hService)
	{
        if (::DeleteService(hService))
		{
            bResult = TRUE;
        }
		else
		{
			int t_LastError = GetLastError();
			//这里需要进行错误处理
        }
        ::CloseServiceHandle(hService);
    }
    
    ::CloseServiceHandle(hSCM);
    return bResult;
}


static void WINAPI Handler(DWORD dwOpcode)
{   
	switch (dwOpcode) {
		case SERVICE_CONTROL_STOP:
			{
				g_Status.dwCurrentState = SERVICE_STOP_PENDING;
				::SetServiceStatus(g_hServiceStatus, &g_Status);
				Uninstall();
				g_Status.dwCurrentState = SERVICE_STOPPED;
				::SetServiceStatus(g_hServiceStatus, &g_Status);
			}
			break;

			case SERVICE_CONTROL_PAUSE:
			{
				g_Status.dwCurrentState = SERVICE_PAUSE_PENDING;
				::SetServiceStatus(g_hServiceStatus, &g_Status);
				//处理中止
				g_Status.dwCurrentState = SERVICE_PAUSED;
				::SetServiceStatus(g_hServiceStatus, &g_Status);
			}
			break;

		case SERVICE_CONTROL_CONTINUE:
			{
				g_Status.dwCurrentState = SERVICE_CONTINUE_PENDING;
				::SetServiceStatus(g_hServiceStatus, &g_Status);
				//处理继续
				g_Status.dwCurrentState = SERVICE_RUNNING;
				::SetServiceStatus(g_hServiceStatus, &g_Status);
			}
			break;

		case SERVICE_CONTROL_INTERROGATE:
			{
			}
			break;

		case SERVICE_CONTROL_SHUTDOWN:
			{
			}
			break;

		default:
			{
			}
	}
	//::SetServiceStatus(pService->m_hServiceStatus, &pService->m_Status);
}

static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	//注册服务控制回调函数
	g_hServiceStatus = RegisterServiceCtrlHandler(g_strDaemonName.c_str(), Handler);
	if (NULL == g_hServiceStatus) {
		return;
	}
	g_Status.dwCurrentState = SERVICE_START_PENDING;
	::SetServiceStatus(g_hServiceStatus, &g_Status);
	//初始化

	g_Status.dwCurrentState = SERVICE_RUNNING;
	::SetServiceStatus(g_hServiceStatus, &g_Status);
	//运行中
	g_PFDaemonFunc(dwArgc, lpszArgv);
	
	g_Status.dwCurrentState = SERVICE_STOPPED;
	::SetServiceStatus(g_hServiceStatus, &g_Status);
}

bool PPLaunchDaemonize(const std::string & strServiceName, const std::string & strServiceDescription, int Argc, char **Argv)
{
	g_strDaemonName = strServiceName;
	
	//打开SCM(Service Control Manager)服务控制管理器
    SC_HANDLE hSCM = ::OpenSCManager(NULL, //本地机器
                                     NULL, //缺省数据库
                                     SC_MANAGER_ALL_ACCESS); //所有权限的管理器
	if (NULL == hSCM)
	{
		return false;
	}

    //获取当前执行文件的路径
    char t_szFilePath[MAX_PATH];
    ::GetModuleFileName(NULL, t_szFilePath, sizeof(t_szFilePath));

    //创建服务
    SC_HANDLE hService = ::CreateService(hSCM,
										g_strDaemonName.c_str(),
										g_strDaemonName.c_str(),
										SERVICE_ALL_ACCESS,
										SERVICE_WIN32_OWN_PROCESS,
										SERVICE_DEMAND_START,        // start condition
										SERVICE_ERROR_NORMAL,
										t_szFilePath,
										NULL,
										NULL,
										NULL,
										NULL,
										NULL);
    if (NULL == hService)
	{
		int t_LastError = GetLastError();
		if(ERROR_SERVICE_EXISTS == t_LastError || ERROR_SERVICE_MARKED_FOR_DELETE == t_LastError)
		{
			//如果服务存在或者服务被标识为删除，则打开服务
			hService = ::OpenService(hSCM,
								   g_strDaemonName.c_str(),
								   SERVICE_ALL_ACCESS);
		}
		if(NULL == hService)
		{
			::CloseServiceHandle(hSCM);
			return false;
		}
    }
	else
	{
		char szDescription[1024];
		strncpy(szDescription, strServiceDescription.c_str(), sizeof(szDescription) - 1);
		SERVICE_DESCRIPTION sd;
		sd.lpDescription = szDescription;
		if(0 == ::ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, (void *)&sd))
		{
			int t_LastError = GetLastError();
		}
	}

	//启动服务
	if(::StartService(hService, Argc, (LPCSTR *)Argv))
	{
		::CloseServiceHandle(hService);
		::CloseServiceHandle(hSCM);
		return true;
	}
	else
	{
		int t_LastError = GetLastError();
		if(ERROR_SERVICE_ALREADY_RUNNING == t_LastError)
		{
			return true;
		}
	}

    //清理上下文
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return false;
}

int PPDaemonize(const std::string & strServiceName, PFDaemonFunc pDaemonFunc)
{
	g_strDaemonName = strServiceName;
	g_PFDaemonFunc = pDaemonFunc;

	g_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_Status.dwCurrentState = SERVICE_STOPPED;
	g_Status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_Status.dwWin32ExitCode = 0;
	g_Status.dwServiceSpecificExitCode = 0;
	g_Status.dwCheckPoint = 0;
	g_Status.dwWaitHint = 0;

	SERVICE_TABLE_ENTRY st[] = {
		{(LPSTR)g_strDaemonName.c_str(), ServiceMain},
		{NULL, NULL}
	};
	BOOL bStart = ::StartServiceCtrlDispatcher(st);
	if(FALSE == bStart)
	{
		int t_LastError = GetLastError();
		return t_LastError;
	}

	if(!Uninstall())
	{
		return -2;
	}

	return 0;
}

bool PPUninstallDaemonize(const std::string & strServiceName)
{
	g_strDaemonName = strServiceName;
	return TRUE == Uninstall();
}

#else

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>

int PPDaemonize(const std::string &)
{
	int                 i, fd0, fd1, fd2;
	pid_t               pid;
	struct rlimit       rl;
	struct sigaction    sa;
	/*
	 * Clear file creation mask.
	 */
	umask(0);

	/*
	 * Get maximum number of file descriptors.
	 */
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
	{
		return -1;
	}

	/*
	 * Become a session leader to lose controlling TTY.
	 */
	if ((pid = fork()) < 0)
	{
		return -1;
	}
	else if (pid != 0) /* parent */
	{
		exit(0);
	}
	setsid();

	/*
	 * Ensure future opens won't allocate controlling TTYs.
	 */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
	{
		return -2;
	}
	if ((pid = fork()) < 0)
	{
		return -3;
	}
	else if (pid != 0) /* parent */
	{
		exit(0);
	}

	/*
	 * Change the current working directory to the root so
	 * we won't prevent file systems from being unmounted.
	 */
	//if (chdir("/") < 0)
	//{
	//	return -4;
	//}

	/*
	 * Close all open file descriptors.
	 */
	if (rl.rlim_max == RLIM_INFINITY)
	{
		rl.rlim_max = 1024;
	}
	for (i = 0; i < (int)rl.rlim_max; i++)
	{
		close(i);
	}

	/*
	 * Attach file descriptors 0, 1, and 2 to /dev/null.
	 */
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		exit(1);
	}
	return 0;
}

#endif
