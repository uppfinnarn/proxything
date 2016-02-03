#ifndef PROXYTHING_FS_ENTRY_H
#define PROXYTHING_FS_ENTRY_H

#include <proxything/fs_service.h>
#include <boost/asio.hpp>
#include <functional>

namespace proxything
{
	using namespace boost::asio;
	
	/**
	 * A filesystem entry.
	 */
	class fs_entry : public basic_io_object<fs_service>
	{
	public:
		/**
		 * Constructor.
		 * 
		 * @param  service Parent IO service
		 */
		explicit fs_entry(io_service &service):
			basic_io_object<fs_service>(service) { }
		
		virtual ~fs_entry() { };
		
		/**
		 * Asynchronously opens the file.
		 * 
		 * @param filename Filename to open
		 * @param cb       Callback
		 */
		void async_open(const std::string &filename, fs_service::OpenHandler cb)
		{
			get_service().async_open(get_implementation(), filename, cb);
		}
	};
}

#endif
