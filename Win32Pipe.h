#pragma once

#include <string>
#include <iostream>
#include <windows.h>

// set up the pipes
	// http://avid-insight.co.uk/joomla/component/k2/item/589-introduction-to-win32-named-pipes-cpp

class CWin32Pipe
{
private: 
	HANDLE m_pipe ; 

public:

	// [Server] Blocking waiting on connect 
	bool CreatePipe( ) ; 
	
	// [Client] Connect to an existing pipe 
	bool ConnectPipe(  ); 

	void Close() ;

	bool Send( std::string data ); 
	bool Recv( std::string &data ); 
};

