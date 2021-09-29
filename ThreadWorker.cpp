#include "ThreadWorker.h"


ThreadWorker::ThreadWorker()
{
    m_threadsCount = std::thread::hardware_concurrency();
    std::cout << m_threadsCount << std::endl;
    InitThreads();
}


void ThreadWorker::InitThreads()
{
    for (int i = 0; i < m_threadsCount - 3; i++)
    {
        std::shared_ptr<ThreadInfo> tempShared = std::make_shared<ThreadInfo>(ThreadInfo());
        m_Threads.push_back(std::thread(&ThreadWorker::ThreadProcess, this, tempShared));
        m_ThreadPool.push_back(tempShared);
    }
    m_processThreadPool = std::thread(&ThreadWorker::ProcessPool, this);
    std::cout << m_ThreadPool.size();
    JoinThreads();
}

void ThreadWorker::JoinThreads()
{
    for (auto& thread : m_Threads)
    {
        thread.detach();
    }
    m_processThreadPool.detach();
}

ThreadWorker::~ThreadWorker()
{}


void ThreadWorker::ProcessPool()
{
    while (true)
    {
        if (!m_requestsQueue.empty())
        {
            for (auto& threadInfo : m_ThreadPool)
            {
                if (!threadInfo->isThreadWorking && !m_requestsQueue.empty())
                {
                    AnswerContainer* tempr = m_requestsQueue.front().get();
                    m_requestsQueue.front().release();
                    m_mutex.lock();
                    threadInfo->set_Request(tempr);
                    threadInfo->isThreadWorking = true;
                    m_requestsQueue.pop();
                    m_mutex.unlock();
                }
            }
        }
    }
}

void ThreadWorker::ThreadProcess(std::shared_ptr<ThreadInfo> threadInfo)
{
    while (true)
    {
        if (threadInfo->isThreadWorking)
        {
            m_mutex.lock();
            threadInfo->ProcessRequest();
            m_mutex.unlock();
        }
    }
}
void ThreadWorker::PushRequest(AnswerContainer* request)
{
    m_mutex.lock();
    m_requestsQueue.push(std::make_unique<AnswerContainer> (*request));
    m_mutex.unlock();
}


ThreadInfo::ThreadInfo()
{
    isThreadWorking = false;
    Request = nullptr;
}

void ThreadInfo::set_Request(AnswerContainer* req)
{
    Request = req;
}

void ThreadInfo::ProcessRequest()
{
    Request->ProcessRequest();
    Request->RespondOnRequest();
    isThreadWorking = false;
    delete Request;
    Request = nullptr;
}

ThreadInfo::~ThreadInfo()
{
    isThreadWorking = false;
}