#include "Win32Pipe.h"

bool CWin32Pipe::CreatePipe(  ) {

	// Create a pipe to send data
	this->m_pipe = CreateNamedPipe(
		L"\\\\.\\pipe\\TankAI" , // name of the pipe
		PIPE_ACCESS_DUPLEX, // 1-way pipe -- send only
		PIPE_TYPE_MESSAGE, // send data as a byte stream
		1, // only allow 1 instance of this pipe
		0, // no outbound buffer
		0, // no inbound buffer
		0, // use default wait time
		NULL // use default security attributes
	);

	if (this->m_pipe == NULL || this->m_pipe == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to create outbound pipe instance.";
		// look up error code here using GetLastError()
		return false; ;
	}

	// This call blocks until a client process connects to the pipe
	BOOL result = ConnectNamedPipe(this->m_pipe, NULL);
	if (!result) {
		std::cout << "Failed to make connection on named pipe." << std::endl;
		// look up error code here using GetLastError()
		this->Close(); 
		return false;
	}

	// Waiting on connection 
	return true; 
}
bool CWin32Pipe::ConnectPipe(  ) {
	// Open the named pipe
	// Most of these parameters aren't very relevant for pipes.
	this->m_pipe = CreateFile(
		L"\\\\.\\pipe\\TankAI" ,
		GENERIC_READ, // only need read access
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (this->m_pipe == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to connect to pipe." << std::endl;
		// look up error code here using GetLastError()
		return false;
	}

	


	return true; 
} 
bool CWin32Pipe::Send( std::string data ) {

	// This call blocks until a client process reads all the data
	DWORD numBytesWritten = 0;
	BOOL result  = WriteFile(
		this->m_pipe, // handle to our outbound pipe
		data.c_str(), // data to send
		data.size() * sizeof(wchar_t), // length of data to send (bytes)
		&numBytesWritten, // will store actual amount of data sent
		NULL // not using overlapped IO
	);
	
	return result ; 
}
bool CWin32Pipe::Recv( std::string &data ) {

	/// wcout << "Reading data from pipe..." << endl;

	// The read operation will block until there is data to read
	wchar_t buffer[128];
	DWORD numBytesRead = 0;
	BOOL result = ReadFile(
		this->m_pipe,
		buffer, // the data from the pipe will be put here
		127 * sizeof(wchar_t), // number of bytes allocated
		&numBytesRead, // this will store number of bytes actually read
		NULL // not using overlapped IO
	);

	if (result) {
		buffer[numBytesRead / sizeof(wchar_t)] = '\0'; // null terminate the string

		std::wstring ws( buffer );
		data = std::string ( ws.begin(), ws.end() );

	} else {
		std::cout << "Failed to read data from the pipe." << std::endl;
	}

	this->Close(); 

	return true ; 
}

void CWin32Pipe::Close()  {
	// Close our pipe handle
	CloseHandle(this->m_pipe);
}