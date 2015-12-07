#ifndef __CCSmartThread
#define __CCSmartThread
#include<thread>

namespace nTool
{
	class CSmartThread
	{
		std::thread thr_;
	public:
		CSmartThread() noexcept=default;
		template<class Func,class ... Args>
		explicit CSmartThread(Func &&,Args &&...);
		CSmartThread(const CSmartThread &)=delete;
		CSmartThread& operator=(const CSmartThread &)=delete;
		CSmartThread& operator=(CSmartThread &&) noexcept=default;
		~CSmartThread();
	};
}

#include"CCSmartThreadT.cpp"

#endif