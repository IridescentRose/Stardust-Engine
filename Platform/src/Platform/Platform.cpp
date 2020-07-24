#include <Platform/Platform.h>

namespace Stardust::Platform {

}

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <Platform/PSP/Dialogs.h>
#include <Utilities/Logger.h>

PSP_MODULE_INFO("Stardust-Application", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

#endif

bool Stardust::Platform::detail::initNetworks()
{
#if CURRENT_PLATFORM == PLATFORM_PSP
	Utilities::detail::core_Logger->log("Attempting Network Init");
	sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
	sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
	sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEURI);
	sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEHTTP);
	sceUtilityLoadNetModule(PSP_NET_MODULE_HTTP);

	//Result stores our codes from the initialization process
	int result = 0;
	result = sceNetInit(128 * 1024, 42, 0, 42, 0); //Creates the network manager with a buffer
	if (result < 0) { //These If Blocks close the game on an error

		Utilities::detail::core_Logger->log("Failed sceNetInit", Utilities::LOGGER_LEVEL_WARN);
		return false;
	}
	result = sceNetInetInit(); //Initializes Inet
	if (result < 0) {
		Utilities::detail::core_Logger->log("Failed sceInetInit", Utilities::LOGGER_LEVEL_WARN);
		return false;
	}
	result = sceNetApctlInit(0x10000, 48); //Initializes Access Point Control
	if (result < 0) {
		Utilities::detail::core_Logger->log("Failed sceNetApctlInit", Utilities::LOGGER_LEVEL_WARN);
		return false;
	}
	result = sceNetResolverInit(); //Initializes DNS resolver (unused)
	if (result < 0) {
		Utilities::detail::core_Logger->log("Failed sceNetResolverInit", Utilities::LOGGER_LEVEL_WARN);
		return false;
	}


	return PSP::ShowNetworkDialog();
#elif CURRENT_PLATFORM == PLATFORM_WIN
	WSAData data;
	int res = WSAStartup(MAKEWORD(2, 2), &data);
	if (res != 0) {
		throw std::runtime_error("WSAStartup Failed: " + res);
		return false;
	}

	return true;
#else
	return true;
#endif
}

void Stardust::Platform::detail::closeNetworks()
{
#if CURRENT_PLATFORM == PLATFORM_PSP
	//NET
	sceNetApctlTerm();
	sceNetResolverTerm();
	sceNetInetTerm();
	sceNetTerm();

	//Modules
	sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
	sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
	sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEURI);
	sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEHTTP);
	sceUtilityUnloadNetModule(PSP_NET_MODULE_HTTP);

	Utilities::detail::core_Logger->log("Cleaning up Networking Driver");
#endif
}

bool Stardust::Platform::detail::getHTTPFile(const char* url, const char* filepath)
{
#if CURRENT_PLATFORM == PLATFORM_PSP
	int templ, connection, request, ret, status, dataend, fd, byteswritten;
	SceULong64 contentsize;
	unsigned char readbuffer[8192];

	ret = sceHttpInit(20000);

	if (ret < 0) {
		Utilities::detail::core_Logger->log("HTTP INIT FAIL");
		return false;
	}

	templ = sceHttpCreateTemplate((char*)"xxx-agent/0.0.1 libhttp/1.0.0", 1, 1);

	if (templ < 0) {
		Utilities::detail::core_Logger->log("HTTP TEMPLATE FAIL");
		return false;
	}

	ret = sceHttpSetResolveTimeOut(templ, 3000000);

	if (ret < 0) {
		Utilities::detail::core_Logger->log("HTTP RESOLVE FAIL");
		return false;
	}

	ret = sceHttpSetRecvTimeOut(templ, 60000000);

	if (ret < 0) {
		Utilities::detail::core_Logger->log("HTTP RECV FAIL");
		return false;
	}

	ret = sceHttpSetSendTimeOut(templ, 60000000);

	if (ret < 0) {
		Utilities::detail::core_Logger->log("HTTP SEND FAIL");
		return false;
	}

	connection = sceHttpCreateConnectionWithURL(templ, url, 0);

	if (ret < 0) {
		Utilities::detail::core_Logger->log("HTTP CONNECTION FAIL");
		return false;
	}

	request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_GET, (char*)url, 0);

	if (ret < 0) {
		Utilities::detail::core_Logger->log("HTTP REQUEST URL FAIL");
		return false;
	}

	ret = sceHttpSendRequest(request, 0, 0);

	if (ret < 0) {
		Utilities::detail::core_Logger->log("HTTP SEND REQUEST FAIL");
		return false;
	}


	ret = sceHttpGetStatusCode(request, &status);

	if (ret < 0) {
		Utilities::detail::core_Logger->log("HTTP STATUS FAIL");
		return false;
	}

	if (status != 200)
		return false;

	ret = sceHttpGetContentLength(request, &contentsize);

	if (ret < 0) {
		Utilities::detail::core_Logger->log("HTTP LENGTH FAIL");
		return false;
	}

	dataend = 0;
	byteswritten = 0;


	// create a dir for the file, if it doesn't exist it would create a folder
	std::string file_string = filepath;
	size_t found = file_string.find_last_of("/");
	sceIoMkdir(file_string.substr(0, found).c_str(), 0777);

	fd = sceIoOpen(filepath, PSP_O_WRONLY | PSP_O_CREAT, 0777);

	while (dataend == 0)
	{
		ret = sceHttpReadData(request, readbuffer, 8192);

		if (ret < 0)
		{
			sceIoWrite(fd, filepath, 4);
			sceIoClose(fd);
			return false;


			if (ret < 0) {
				Utilities::detail::core_Logger->log("HTTP WRITE ERR");
				return false;
			}
		}

		if (ret == 0)
			dataend = 1;

		if (ret > 0)
		{
			byteswritten += ret;
			sceIoWrite(fd, readbuffer, ret);
		}
	}

	sceIoClose(fd);
	sceHttpDeleteRequest(request);
	sceHttpDeleteConnection(connection);
	sceHttpDeleteTemplate(templ);
	sceHttpEnd();

	return true;
#else
	//TODO: GET HTTP LIBRARY
	return false;
#endif
}

void Stardust::Platform::detail::closeSockets(int fd)
{
#if (CURRENT_PLATFORM == PLATFORM_PSP) || (CURRENT_PLATFORM == PLATFORM_NIX) || (CURRENT_PLATFORM == PLATFORM_VITA)
	close(fd);
#elif CURRENT_PLATFORM == PLATFORM_WIN
	closesocket(fd);
#endif
}

bool Stardust::Platform::detail::setBlocking(int fd, int blocking)
{
#if CURRENT_PLATFORM == PLATFORM_PSP || (CURRENT_PLATFORM == PLATFORM_NIX) || (CURRENT_PLATFORM == PLATFORM_VITA)
	if (blocking) {
		int flags = fcntl(fd, F_GETFL, 0);
		flags &= ~O_NONBLOCK;
		fcntl(fd, F_SETFL, flags);
	}
	else {
		fcntl(fd, F_SETFL, O_NONBLOCK);
	}
	return true;
#elif CURRENT_PLATFORM == PLATFORM_WIN
	unsigned long iMode = !blocking;
	int iResult = ioctlsocket(fd, FIONBIO, &iMode);
	if (iResult != NO_ERROR) {
		return false;
	}
	return true;
#endif
}
