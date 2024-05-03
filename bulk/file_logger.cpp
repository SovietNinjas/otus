#include "file_logger.h"

#include "writer_thread.h"

void FileLogger::release() { WriterThread::instance().add(*this); }
