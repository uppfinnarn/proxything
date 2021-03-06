#ifndef PROXYTHING_FS_SERVICE_H
#define PROXYTHING_FS_SERVICE_H

#include <proxything/impl/fs_service_threaded.h>
#include <proxything/util.h>
#include <boost/asio.hpp>
#include <thread>
#include <memory>

namespace proxything
{
	namespace asio = boost::asio;
	
	/**
	 * Service for asynchronous filesystem access.
	 * 
	 * This is a very barebones implementation, but it has the basic design
	 * down and is sufficient for our purposes.
	 */
	class fs_service : public asio::io_service::service
	{
		/**
		 * The concrete implementation used.
		 * 
		 * Currently, only a thread-based implementation is provided, but it
		 * would be trivial to add implementations using AIO or Windows OVERLAP
		 * instead.
		 */
		typedef impl::fs_service_threaded<std::thread> impl_type;
		
		/// An instance of impl_type
		impl_type m_impl;
		
	public:
		/// Service ID
		static asio::io_service::id id;
		
		/**
		 * Implementation-defined type for IO objects.
		 * 
		 * @see proxything::fs_entry
		 */
		typedef impl_type::implementation_type implementation_type;
		
		/**
		 * Callback type for opening a file.
		 * 
		 * @param ec Error code
		 */
		typedef std::function<void(const boost::system::error_code &ec)> OpenHandler;
		
		/**
		 * Callback type for closing a file.
		 * 
		 * @param ec Error code
		 */
		typedef std::function<void(const boost::system::error_code &ec)> CloseHandler;
		
		/**
		 * Callback type for reading a file.
		 * 
		 * @param ec Error code
		 */
		typedef std::function<void(const boost::system::error_code &ec, std::size_t size)> ReadHandler;
		
		/**
		 * Callback type for writing to a file.
		 * 
		 * @param ec   Error code
		 * @param size Bytes written
		 */
		typedef std::function<void(const boost::system::error_code &ec, std::size_t size)> WriteHandler;
		
		/**
		 * Constructor.
		 * 
		 * @param  service Parent IO service
		 */
		explicit fs_service(asio::io_service &service):
			asio::io_service::service(service) { }
		
		virtual ~fs_service() { };
		
		/**
		 * Constructs a new entry.
		 * 
		 * @param impl Implementation
		 */
		void construct(implementation_type &impl)
		{
			m_impl.construct(get_io_service(), impl);
		}
		
		/**
		 * Destroy an entry.
		 * 
		 * @param impl Implementation
		 */
		void destroy(implementation_type &impl)
		{
			m_impl.destroy(get_io_service(), impl);
		}
		
		/**
		 * Asynchronously opens a file.
		 * 
		 * @param impl     Implementation
		 * @param filename Filename
		 * @param mode     Open mode
		 * @param atomic   Open for atomic writes
		 * @param cb       Callback
		 */
		void async_open(implementation_type &impl, const std::string &filename, std::ios_base::openmode mode, bool atomic, OpenHandler cb)
		{
			m_impl.async_open(get_io_service(), impl, filename, mode, atomic, util::work_bound(get_io_service(), cb));
		}
		
		/**
		 * Asynchronously closes a file.
		 * 
		 * @param impl Implementation
		 * @param cb   Callback
		 */
		void async_close(implementation_type &impl, CloseHandler cb)
		{
			m_impl.async_close(get_io_service(), impl, util::work_bound(get_io_service(), cb));
		}
		
		/**
		 * Asynchronously reads a file.
		 * 
		 * @param impl    Implementation
		 * @param buffers Buffers
		 * @param cb      Callback
		 */
		template<typename BufsT>
		void async_read_some(implementation_type &impl, const BufsT &buffers, ReadHandler cb)
		{
			m_impl.async_read_some(get_io_service(), impl, buffers, util::work_bound(get_io_service(), cb));
		}
		
		/**
		 * Asynchronously writes a file.
		 * 
		 * @param impl    Implementation
		 * @param buffers Buffers
		 * @param cb      Callback
		 */
		template<typename BufsT>
		void async_write_some(implementation_type &impl, const BufsT &buffers, WriteHandler cb)
		{
			m_impl.async_write_some(get_io_service(), impl, buffers, util::work_bound(get_io_service(), cb));
		}
		
		/**
		 * Returns the entry's filename.
		 * 
		 * @param  impl Implementation
		 */
		std::string filename(const implementation_type &impl) const
		{
			return m_impl.filename(impl);
		}
		
	protected:
		/**
		 * Free all user handlers.
		 */
		void shutdown_service() { };
	};
}

#endif
