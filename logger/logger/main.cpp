#include "config.h"

#include "logger.h"

int main(int argc, char **argv)
{
	KM::CLogger::Instance()->Init("log.properties");
	LOG_INFO("hello");
	LOG_ERROR("hello");
	return 0;
};