#include"CCThreadPoolItem.h"

namespace nTool
{
	template<class Func>
	void CThreadPoolItem<Func>::finishing_()
	{
		if(assign_.available())	//it means detach_
		{
			commun_->communPoolDetach();
			detach_.signal();
		}
		else
		{
			commun_->communPoolFinish();
			assign_.signal();
		}
	}

	template<class Func>
	void CThreadPoolItem<Func>::loop_()
	{
		wait_.wait();	//make waiting_() block
		while(waiting_(),!destructor_)
		{
			running_();
			finishing_();
		}
	}

	template<class Func>
	CThreadPoolItem<Func>::CThreadPoolItem()
		:destructor_(false),joinable_(false),thr_(&CThreadPoolItem<Func>::loop_,this){}

	template<class Func>
	template<class ... Args>
	void CThreadPoolItem<Func>::assign(Args &&...args)
	{
		assign_.wait();
		exec_.init(std::forward<Args>(args)...);	//must prior to joinable_=true
		joinable_=true;	//if you use CAsyncExecutor::valid as joinable
						//when user call detach, it also becomes valid and joinable
						//so, you cannot combine joinable_ and CAsyncExecutor::valid
		wake_();
	}

	template<class Func>
	template<class ... Args>
	void CThreadPoolItem<Func>::assign_and_detach(Args &&...args)
	{
		detach_.wait();
		exec_.init(std::forward<Args>(args)...);
		wake_();
	}

	template<class Func>
	decltype(std::declval<CAsyncExecutor<Func>>().get()) CThreadPoolItem<Func>::join()
	{
		wait();
		joinable_=false;
		commun_->communPoolJoin();
		return exec_.get();
	}

	template<class Func>
	CThreadPoolItem<Func>::~CThreadPoolItem()
	{
		if(joinable())
			join();
		if(!detach_.available())
			detach_.wait();
		destructor_=true;
		wake_();
	}
}