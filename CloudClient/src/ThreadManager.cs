using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace CloudClient.src
{
    static public class ThreadManager
    {
        public struct ThreadTask
        {
            public string filename;
            public string localfilename;
            public int operation;
        }

        static private Queue<ThreadTask> tasks = new Queue<ThreadTask>();
        static private Thread[] threadPool = new Thread[4];
        static private ManualResetEvent mre = new ManualResetEvent(false);
        static private Mutex queue_mutex = new Mutex();
        static private Mutex list_mutex = new Mutex();

        static private List<ThreadTask> finished_tasks = new List<ThreadTask>();

        static public void InitializeThreadPool()
        {
            for (int i = 0; i < 4; i++)
            {
                threadPool[i] = new Thread(ThreadFunction);
                threadPool[i].Start();
            }
        }

        static public void printTask(ThreadTask task)
        {
            //MessageBox.Show(task.filename);
        }
        static public void ThreadFunction()
        {
            //Wait for the list to not be empty
            do
            {
                queue_mutex.WaitOne();
                //if (tasks.Count == 0)
                //{
                //    queue_mutex.ReleaseMutex();
                //    mre.WaitOne();
                //}
                //mre.WaitOne();
                //if(tasks.Count == )
                if (tasks.Count != 0)
                {
                    ThreadTask currentTask = tasks.Dequeue();
                    queue_mutex.ReleaseMutex();

                    if (currentTask.operation == 0)
                        src.ServerAPI.DownloadFile(currentTask.filename, currentTask.localfilename);
                    else
                        src.ServerAPI.UploadFile(currentTask.filename, currentTask.localfilename);
                    //printTask(currentTask);

                    list_mutex.WaitOne();
                    finished_tasks.Add(currentTask);
                    list_mutex.ReleaseMutex();

                    continue;
                }
                queue_mutex.ReleaseMutex();
            } while (true);
        }

        static public void AddTask(ThreadTask task)
        {
            //mre.Reset();
            queue_mutex.WaitOne();
            tasks.Enqueue(task);
            queue_mutex.ReleaseMutex();
            ///mre.Set();
        }

        static public bool IsTaskFinished(ThreadTask task)
        {
            return finished_tasks.Contains(task);
        }

        static public ThreadState GetState(int i)
        {
            return threadPool[i].ThreadState;
        }
    }
}