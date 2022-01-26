#pragma once

#include <cstdlib>
#include <thread>
#include <array>
#include <functional>
#include <deque>
#include <concepts>
#include <condition_variable>
#include <mutex>
#include <bitset>

namespace lio
{
    template <std::size_t ThreadCount>
    requires (ThreadCount > 0)
    class ThreadPool
    {
    public:

        ThreadPool() : working_count(ThreadCount)
        {
            for (std::thread& worker : workers)
            {
                worker = std::thread(&ThreadPool::WorkerFunction, this);
            }
        }

        ~ThreadPool()
        {
            WaitForAll();
            is_active = false;
            task_assignment_cv.notify_all();
            for (std::thread& worker : workers)
            {
                worker.join();
            }
        }

        void WaitForAll()
        {
            if (working_count == 0)
                return;

            std::unique_lock<std::mutex> task_finished_lock(task_finished_mutex);
            task_finished_cv.wait(task_finished_lock);
        }

        template <typename Task>
        requires requires (Task task)
        { { task() } -> std::same_as<void>; }
        void AddTask(Task task)
        {
            std::unique_lock<std::mutex> tasks_lock(tasks_mutex);

            tasks.emplace_back(task);

            tasks_lock.unlock();

            has_task_waiting = true;
            task_assignment_cv.notify_one();
        }

        template <typename Task, typename... Args>
        requires requires (Task task, Args&&... args)
        { { task(std::forward<Args>(args)...) } -> std::same_as<void>; }
        void AddTask(Task task, Args&&... args)
        {
            AddTask([task, args...](){ task(std::forward<Args>(args)...); });
        }

        static constexpr std::size_t GetThreadCount()
        {
            return ThreadCount;
        }

        std::size_t GetWorkingThreadCount() const
        {
            return working_count;
        }

        std::size_t GetWaitingTaskCount() const
        {
            std::unique_lock<std::mutex> tasks_lock(tasks_mutex);

            return tasks.size();
        }

    private:

        void WorkerFunction()
        {
            while (true)
            {
                if (!has_task_waiting)
                {
                    --working_count;
                    if (working_count == 0)
                        task_finished_cv.notify_all();

                    std::unique_lock<std::mutex> task_assignment_lock(task_assignment_mutex);
                    task_assignment_cv.wait(task_assignment_lock);

                    if (!is_active)
                        return;

                    ++working_count;
                }

                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> tasks_lock(tasks_mutex);

                    task = tasks.front();
                    tasks.pop_front();

                    if (tasks.empty())
                        has_task_waiting = false;

                    tasks_lock.unlock();
                }

                task();
            }
        }

        std::atomic_size_t working_count;
        std::atomic_bool is_active = true;
        std::atomic_bool has_task_waiting = false;

        std::condition_variable task_finished_cv;
        std::mutex task_finished_mutex;

        std::condition_variable task_assignment_cv;
        std::mutex task_assignment_mutex;

        std::deque<std::function<void()>> tasks;
        mutable std::mutex tasks_mutex;

        std::array<std::thread, ThreadCount> workers;
    };
}