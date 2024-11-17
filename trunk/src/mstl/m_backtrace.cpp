// ======================================================================
// Author : $Author$
// Version: $Revision: 1372 $
// Date   : $Date: 2017-08-04 17:56:11 +0000 (Fri, 04 Aug 2017) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "m_backtrace.h"
#include "m_sstream.h"
#include "m_chunk_allocator.h"

/// \class backtrace
///
/// \brief Stores the backtrace from the point of construction.
///
/// The backtrace, or callstack, is the listing of functions called to
/// reach the construction of this object. This is useful for debugging,
/// to print the location of an error. To get meaningful output you'll
/// need to use a debug build with symbols and with frame pointers. For
/// GNU ld you will also need to link with the -rdynamic option to see
/// actual function names instead of __gxx_personality0+0xF4800.

using namespace mstl;


bool backtrace::m_isEnabled = true;


void backtrace::enable(bool flag) { m_isEnabled = flag; }
void backtrace::disable() { m_isEnabled = false; }


#ifdef __OPTIMIZE__

backtrace::backtrace(bool) {}
backtrace::backtrace(backtrace const&) {}
backtrace::~backtrace() throw() {}

backtrace const& backtrace::operator=(backtrace const&) { return *this; }
bool backtrace::is_debug_mode() { return false; }
void backtrace::text_write(ostringstream&, size_t) const {}

#else // __OPTIMIZE__

# include "m_sstream.h"
# include "m_iostream.h"
# include "m_utility.h"
# include "m_stdio.h"

# include <string.h>
# include <stdlib.h>
# include <assert.h>

# ifdef __unix__

#  include "m_fstream.h"

#  include <execinfo.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
#  include <sys/socket.h>
#  include <sys/wait.h>

#  ifndef DONT_USE_GDB
// addr2line is not working properly (wrong line numbers),
// but on Ubuntu the call of gdb is often crashing.
#   define USE_GDB
#  endif

#  ifndef DONT_USE_FRAME_ADDR
// with newer compilers this is crashing
#   define USE_ADDR2LINE
#  endif


#  define M_HAVE_THREADS

#  ifndef M_HAVE_THREADS

static bool isMainThread() { return true; }

#  elif defined(__WIN32__)

static bool isMainThread() { return false; } // backtrace not needed under windows

#  elif defined(__MacOSX__)

#   include <pthread.h>
static bool isMainThread() { return pthread_main_np(); }

#  elif defined(__linux__)

#   include <sys/syscall.h>
#   include <unistd.h>
static bool isMainThread() { return syscall(SYS_gettid) == getpid(); }

#  else // don't know how this should be determined

static bool isMainThread() { return true; }

#  endif


#  ifdef USE_GDB

namespace {

struct pstream : public iostream
{
	pstream(string const& cmd) { m_fp = ::popen(cmd.c_str(), "r"); }
	~pstream() throw() { ::fclose(m_fp); }

	bool is_open() const { return m_fp != 0; }
};

}


bool
check_exe(char const* path, char const* exe, string& buf)
{
	if (path == 0)
		return false;

	buf = path;
	buf += '/';
	buf += exe;

	if (access(buf, R_OK | X_OK) == 0)
		return true;

	return false;
}


bool
search_exe(char const* exe, string& buf)
{
	static char const* Usr_Bin_Path			= "/usr/bin";
	static char const* Usr_Local_Bin_Path	= "/usr/local/bin";

	char const*	path	= getenv("PATH");
	char const* p		= Usr_Bin_Path;
	char const* q		= Usr_Local_Bin_Path;

	if (path != 0 && strstr(path, Usr_Bin_Path) > strstr(path, Usr_Local_Bin_Path))
	{
		char const* s = p;
		p = q;
		q = s;
	}

	return check_exe(p, exe, buf) || check_exe(q, exe, buf);
}


string
gdb_cmd(char const* script_name)
{
	string cmd;

	if (search_exe("gdb", cmd))
	{
		cmd += " --nx --quiet --batch -iex 'set auto-load no' -x ";
		cmd += script_name;
		cmd += " --pid ";
		cmd.format("%ld", long(getpid()));
	}

	return cmd;
}

#  endif // USE_GDB

#  ifdef USE_ADDR2LINE

namespace {

static ssize_t
sock_read(void* cookie, char* buf, size_t len)
{
	ssize_t n = recv(reinterpret_cast<intptr_t>(cookie), buf, len, 0);
	return n <= 0 ? -1 : n;
}


static ssize_t
sock_write(void* cookie, char const* buf, size_t len)
{
	while (true)
	{
		ssize_t n = write(reinterpret_cast<intptr_t>(cookie), buf, len);

		if (n == 0)
			return -1;

		if (n > 0)
			return n;

		if (errno != EINTR)
			return -1;
	}

	return 0;	// satisfies the compiler
}


static int
sock_close(void* cookie)
{
	return close(reinterpret_cast<intptr_t>(cookie));
}


//static ssize_t sock_seek(void*, __off64_t*, int) { return -1; }
static int sock_seek(void*, __off64_t*, int) { return -1; }


static cookie_io_functions_t m_sock_io = { sock_read, sock_write, sock_seek, sock_close };


class proc_stream : public iostream
{
public:

	proc_stream(string const& cmd);
	~proc_stream() throw();

	bool is_open() const { return m_socket != -1; }

private:

	int	m_socket;
	pid_t	m_pid;
};


proc_stream::proc_stream(string const& cmd)
	:m_socket(-1)
	,m_pid(0)
{
	int socks[2] = { -1, -1 };

	try
	{
		if (::socketpair(PF_LOCAL, SOCK_STREAM, 0, socks) < 0)
			throw "socketpair() failed";

		m_pid = ::fork();

		if (m_pid == -1)
			throw "fork() failed";

		if (m_pid == 0)
		{
			::close(socks[0]);

			int sd = socks[1];

			if ((sd != 0 && ::dup2(sd, 0) < 0) || (sd != 1 && ::dup2(sd, 1) < 0))
				throw "dup2() failed";

			::close(sd);
			::execl("/bin/sh", "sh", "-c", cmd.c_str(), static_cast<char*>(0));

			throw "execl() failed";
		}

		::close(socks[1]);
		m_socket = socks[0];
		m_fp = ::fopencookie(reinterpret_cast<void*>(intptr_t(m_socket)), "r+", m_sock_io);
	}
	catch (char const* msg)
	{
		::fprintf(stderr, "%s\n", msg);
	}

	if (!m_fp)
	{
		::close(m_socket);
		m_socket = -1;
	}
}


proc_stream::~proc_stream() throw()
{
	if (m_socket == -1)
		return;

	if (m_fp)
	{
		::fclose(m_fp);
		m_fp = 0;
	}

	if (m_pid)
	{
		if (::waitpid(m_pid, 0, WNOHANG) == 0)
			::kill(m_pid, SIGTERM);

		::waitpid(m_pid, 0, 0);
	}
}

} // namespace


static string
addr2line_cmd()
{
	static char const* addr_to_line[] =
	{
		"/usr/local/bin/addr2line",
		"/usr/bin/addr2line",
	};

	string exe;
	exe.format("/proc/%ld/exe", long(::getpid()));

	if (::access(exe, R_OK) == 0)
	{
		for (size_t i = 0; i < sizeof(addr_to_line)/sizeof(addr_to_line[0]); ++i)
		{
			if (::access(addr_to_line[i], R_OK | X_OK) == 0)
				return string(addr_to_line[i]) + " -Cfse " + exe;
		}
	}

	return string();
}

#  endif // USE_ADDR2LINE

bool
mstl::backtrace::is_debug_mode()
{
	static int debug = -1;

	if (debug == -1)
	{
		string cmdline;
		cmdline.format("/proc/%ld/cmdline", long(::getppid()));
		fstream strm(cmdline.c_str(), ios_base::in);

		if (!strm)
		{
			debug = 0;
		}
		else
		{
			string	buf;
			int		c;

			while ((c = strm.get()) != EOF)
			{
				switch (c)
				{
					case '/':
						buf.clear();
						break;

					case ' ':
					case '\t':
					case '\0':
						if (buf == "gdb")
						{
							debug = 1;
							return true;
						}
						buf.clear();
						break;

					default:
						buf += c;
						break;
				}
			}

			debug = (buf == "gdb");
		}
	}

	return debug == 1;
}

# else

bool mstl::backtrace::is_debug_mode() { return false; }
inline static int backtrace(void**, int) { return 0; }
inline static char** backtrace_symbols(void* const*, int) { return 0; }

# endif // __unix__

# include <cxxabi.h>

static char const*
demangle_type_name(char* buf, size_t buf_size, size_t* pdm_size)
{
	size_t bl = ::strlen(buf);

	char		dmname[256];
	size_t	sz = sizeof(dmname);
	int		failed;

	abi::__cxa_demangle(buf, dmname, &sz, &failed);

	if (!failed)
	{
		bl = min(::strlen(dmname), buf_size - 1);
		::memcpy(buf, dmname, bl);
		buf[bl] = 0;
	}

	if (pdm_size)
		*pdm_size = bl;

	return buf;
}


static size_t
extract_abi_name(char const* isym, char* nmbuf)
{
	// Prepare the demangled name, if possible
	size_t nm_size = 0;

	if (isym)
	{
		// Copy out the name; the strings are:
		// 1. "file(function+0x42) [0xAddress]"
		// 2. "file [0xAddress]"
		char const* nm_start = ::strchr(isym, '(');

		if (nm_start++ == 0)
		{
			nm_size = min(::strlen(isym), size_t(256));
			::memcpy(nmbuf, isym, nm_size);
		}
		else
		{
			char const* nm_end = ::strrchr(nm_start, '+');

			if (nm_end)
			{
				nm_size = min(size_t(distance(nm_start, nm_end)), size_t(256));
				::memcpy(nmbuf, nm_start, nm_size);
			}
			else
			{
				nm_size = min(::strlen(isym), size_t(256));
				::memcpy(nmbuf, isym, nm_size);
			}
		}
	}

	nmbuf[nm_size] = 0;

	// Demangle
	demangle_type_name(nmbuf, 256U, &nm_size);

	return nm_size + 1;
}


mstl::backtrace::backtrace(bool wanted)
	:m_nframes(0)
	,m_allocator(new allocator(512))
	,m_skip(0)
	,m_trace(nullptr)
{
	if (wanted)
		symbols();
}


mstl::backtrace::backtrace(backtrace const& v)
	:m_nframes(0)
	,m_allocator(nullptr)
	,m_skip(0)
	,m_trace(nullptr)
{
	operator=(v);
}



mstl::backtrace::~backtrace() throw()
{
	delete m_allocator;
	delete m_trace;
}


mstl::backtrace const&
mstl::backtrace::operator=(backtrace const& v)
{
	ostringstream strm;
	v.text_write(strm, 1);
	if (!m_trace)
		m_trace = new string();
	*m_trace = strm.str();
	delete m_allocator;
	m_allocator = nullptr;
	return *this;
}


# if defined(__unix__)
#  if defined(USE_GDB)

bool
mstl::backtrace::symbols_gdb()
{
	char const GDB_Script[] = "set width 5000\nbacktrace\nquit\n";

	char script_name[128];
	snprintf(script_name, sizeof(script_name), "/tmp/gdb-script.%ld", long(::getpid()));
	int fd = ::open(script_name, O_WRONLY | O_CREAT | O_TRUNC, 0770);
	if (fd == -1)
		return false;
	::write(fd, GDB_Script, sizeof(GDB_Script) - 1);
	::close(fd);

	string cmd(gdb_cmd(script_name));
	if (cmd.empty())
		return false;

	pstream strm(cmd);
	if (!strm.is_open())
		return false;

//	m_skip = 4;	seems to be too much on some systems

	string line;

	m_nframes = 0;

	while (strm.getline(line) && line[0] != '#')
		continue;

	while (strm.getline(line) && line[0] == '#')
	{
		char* in_pos;
		char* lp_pos;
		char* rp_pos;
		char* at_pos;

		if ((in_pos = const_cast<char*>(::strstr(line, " in "))) == 0)
			continue;
		in_pos += 4;

		if (*in_pos == '+' || *in_pos == '-')
		{
			if ((lp_pos = ::strstr(in_pos + 2, "(self=0x")) == 0)
				continue;
		}
		else
		{
			if ((lp_pos = ::strchr(in_pos + 1, '(')) == 0)
				continue;
		}
		if ((rp_pos = lp_pos)[-1] == ' ')
			--rp_pos;
		else if ((rp_pos = ::strchr(lp_pos + 1, ')')) == 0)
			continue;
		if ((at_pos = ::strstr(rp_pos + 1, " at ")) == 0)
			continue;
		at_pos += 4;
		*rp_pos = '\0';

		if (::strcmp(in_pos, "_start") == 0)
			break;

		if (::strncmp(in_pos, "??", 2) != 0)
		{
			unsigned at_len = ::strlen(at_pos);
			unsigned in_len = ::strlen(in_pos);

			m_addresses[m_nframes] = 0;
			m_symbols[m_nframes] = m_allocator->alloc(at_len + in_len + 4);

			char* s = m_symbols[m_nframes];
			::memcpy(s, in_pos, in_len);
			s += in_len;
			*s++ = ' ';
			*s++ = '[';
			::memcpy(s, at_pos, at_len);
			s += at_len;
			*s++ = ']';
			*s++ = '\n';

			if (++m_nframes == sizeof(m_symbols)/sizeof(m_symbols[0]))
				return true;
		}

		if (::strcmp(in_pos, "main") == 0)
			return true;
	}

	return m_nframes > 0;
}

#  endif // defined(USE_GDB)
#  ifdef USE_ADDR2LINE

static void*
frameAddress(unsigned i)
{
	switch (i)
	{
		case  0: return __builtin_frame_address( 0);
		case  1: return __builtin_frame_address( 1);
		case  2: return __builtin_frame_address( 2);
		case  3: return __builtin_frame_address( 3);
		case  4: return __builtin_frame_address( 4);
		case  5: return __builtin_frame_address( 5);
		case  6: return __builtin_frame_address( 6);
		case  7: return __builtin_frame_address( 7);
		case  8: return __builtin_frame_address( 8);
		case  9: return __builtin_frame_address( 9);
		case 10: return __builtin_frame_address(10);
		case 11: return __builtin_frame_address(11);
		case 12: return __builtin_frame_address(12);
		case 13: return __builtin_frame_address(13);
		case 14: return __builtin_frame_address(14);
		case 15: return __builtin_frame_address(15);
		case 16: return __builtin_frame_address(16);
		case 17: return __builtin_frame_address(17);
		case 18: return __builtin_frame_address(18);
		case 19: return __builtin_frame_address(19);
		case 20: return __builtin_frame_address(20);
		case 21: return __builtin_frame_address(21);
		case 22: return __builtin_frame_address(22);
		case 23: return __builtin_frame_address(23);
		case 24: return __builtin_frame_address(24);
		case 25: return __builtin_frame_address(25);
		case 26: return __builtin_frame_address(26);
		case 27: return __builtin_frame_address(27);
		case 28: return __builtin_frame_address(28);
		case 29: return __builtin_frame_address(29);
		case 30: return __builtin_frame_address(30);
		case 31: return __builtin_frame_address(31);
		case 32: return __builtin_frame_address(32);
		case 33: return __builtin_frame_address(33);
		case 34: return __builtin_frame_address(34);
		case 35: return __builtin_frame_address(35);
		case 36: return __builtin_frame_address(36);
		case 37: return __builtin_frame_address(37);
		case 38: return __builtin_frame_address(38);
		case 39: return __builtin_frame_address(39);
		case 40: return __builtin_frame_address(40);
		case 41: return __builtin_frame_address(41);
		case 42: return __builtin_frame_address(42);
		case 43: return __builtin_frame_address(43);
		case 44: return __builtin_frame_address(44);
		case 45: return __builtin_frame_address(45);
		case 46: return __builtin_frame_address(46);
		case 47: return __builtin_frame_address(47);
		case 48: return __builtin_frame_address(48);
		case 49: return __builtin_frame_address(49);
		case 50: return __builtin_frame_address(50);
		case 51: return __builtin_frame_address(51);
		case 52: return __builtin_frame_address(52);
		case 53: return __builtin_frame_address(53);
		case 54: return __builtin_frame_address(54);
		case 55: return __builtin_frame_address(55);
		case 56: return __builtin_frame_address(56);
		case 57: return __builtin_frame_address(57);
		case 58: return __builtin_frame_address(58);
		case 59: return __builtin_frame_address(59);
		case 60: return __builtin_frame_address(60);
		case 61: return __builtin_frame_address(61);
		case 62: return __builtin_frame_address(62);
		case 63: return __builtin_frame_address(63);
	}

	return 0;
}


bool
mstl::backtrace::symbols_linux()
{
	string cmd(addr2line_cmd());

	if (cmd.empty())
		return false;

	proc_stream stream(cmd);

	if (!stream.is_open())
		return false;
	
	string	func;
	string	file;
	void*		address;

	m_nframes = 0;

	static_assert(sizeof(m_symbols)/sizeof(m_symbols[0]) == 64, "buffer size mismatch");

	while (func != "main" && (address = ::frameAddress(m_nframes)))
	{
		m_addresses[m_nframes] = static_cast<void**>(address)[1];

		stream << m_addresses[m_nframes];
		stream << '\n';
		stream.getline(func);
		func.unhook();
		stream.getline(file);

		if (func == "??")
		{
			m_addresses[m_nframes] = 0;
			m_symbols[m_nframes] = 0;
		}
		else
		{
			m_symbols[m_nframes] = m_allocator->alloc(func.size() + file.size() + 4);

			char* s = m_symbols[m_nframes];
			::memcpy(s, func, func.size());
			s += func.size();
			*s++ = ' ';
			*s++ = '[';
			::memcpy(s, file, file.size());
			s += file.size();
			*s++ = ']';
			*s++ = '\n';
		}

		++m_nframes;
	}

	return true;
}

#  endif // USE_ADDR2LINE
# endif	// defined(__unix__)


bool
mstl::backtrace::empty() const
{
	M_ASSERT(m_allocator || m_trace);
	return m_allocator ? m_allocator->empty() : m_trace->empty();
}


void
mstl::backtrace::symbols()
{
	if (!m_isEnabled || !m_allocator || !::isMainThread() || !empty())
		return;

# ifdef __unix__

	if (is_debug_mode())
		return;

#  ifdef USE_GDB
	if (symbols_gdb())
		return;
#  endif

#  ifdef USE_ADDR2LINE
	if (symbols_linux())
		return;
#  endif

	m_allocator->clear();
	m_skip = 0;

# endif // __unix__

	m_nframes = ::backtrace(m_addresses, sizeof(m_addresses)/sizeof(m_addresses[0]));

	char** symbols = ::backtrace_symbols(m_addresses, m_nframes);

	if (!symbols)
		return;

	char nmbuf[256];

	for (unsigned i = 0; i < m_nframes; ++i)
	{
		size_t sz = ::extract_abi_name(symbols[i], nmbuf);

		if (sz)
		{
			m_symbols[i] = m_allocator->alloc(sz);
			::memcpy(m_symbols[i], nmbuf, sz - 1);
			m_symbols[i][sz - 1] = '\n';
		}
		else
		{
			m_symbols[i] = 0;
		}
	}

	::free(symbols);
}


void
mstl::backtrace::text_write(ostringstream& os, unsigned skip) const
{
	M_ASSERT(m_allocator || m_trace);

	if (empty())
		return;
	
	if (m_allocator)
	{
		unsigned i = 0;

		skip += m_skip;

		if (::strncmp(m_symbols[0], "__read_nocancel", 12) == 0)
			skip += 4;

		for ( ; i < m_nframes && skip; ++i)
		{
			if (m_symbols[i])
				--skip;
		}

		for ( ; i < m_nframes; ++i)
		{
			char const* s = m_symbols[i];

			if (	s
				&& ::strstr(s, "m_exception.ipp") == 0
				&& ::strstr(s, "assertion_failure_exception") == 0
				&& ::strstr(s, "backtrace::backtrace") == 0
				&& ::strstr(s, "exception::exception") == 0
				&& ::strstr(s, "Exception::Exception") == 0
				&& ::strstr(s, "Error::Error") == 0)
			{
				char const* e = ::strchr(s, '\n') + 1;

#if 0
				if (m_addresses[i])
					os.format(sizeof(long) == 8 ? "0x%016lx" : "0x%08lx", long(m_addresses[i]));
#endif

				os.put(' ');
				os.write(s, distance(s, e));

				if (::strncmp(s, "main\n", 5) == 0 || ::strncmp(s, "__libc_start_main\n", 18) == 0)
					return;
			}
		}
	}
	else
	{
		os << m_trace;
	}
}

#endif // __OPTIMIZE__


void
mstl::backtrace::clear()
{
#ifndef __OPTIMIZE__
	if (m_allocator)
		m_allocator->clear();
	if (m_trace)
		m_trace->clear();
#endif
}

// vi:set ts=3 sw=3:
