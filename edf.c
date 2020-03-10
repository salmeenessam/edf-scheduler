#include <stdio.h>
#include <stdbool.h>
    /*
     * EDF:
     * Check if processor utilization time is less than 100% where U = sum(execTime/deadline)
     * first process is the process with the earliest deadline
     * if a process enters or ends, search for new process with earliest deadline
     * preemt old process with new process
     * each second decrement [executionTimeRemaining]
     * if period of process has passed, make the process enter the queue again
     * 
     */

struct process 
{
    int entryTime;                  // Time at which the process enters the queue
    int executionTime;              // Burst time
    int executionTimeRemaining;     // Remaining burst time to complete the process
    int deadline;                   // Period + originalDeadline
    int originalDeadline;           
    int period;
    int id;                         // ie P1, P2, P3...
};

int main()
{
    struct process processes[1000]; // process queue

    int totalTime;  // total time available to run processes
    int n;          // number of processes
    
    printf("Enter total output time: ");
    scanf("%d",&totalTime);

    printf("Enter number of processes: ");
    scanf("%d",&n);
    

    // take info for each process from user
    for(int i = 0; i < n; i++)
    {
        printf("P%d entry time: ", i+1);
        scanf("%d",&processes[i].entryTime);

        printf("P%d execution time: ", i+1);
        scanf("%d",&processes[i].executionTime);
        processes[i].executionTimeRemaining = processes[i].executionTime;
        
        printf("P%d deadline: ", i+1);
        scanf("%d",&processes[i].originalDeadline);
        processes[i].deadline = processes[i].originalDeadline;

        printf("P%d period: ", i+1);
        scanf("%d",&processes[i].period);

        processes[i].id = i+1;

    }



    int U = 0; //CPU utilization
    for (int i =0;i<n;i++)
    {
        struct process p = processes[i];
        U += (p.executionTimeRemaining/p.deadline);
    }

    if(U >= 1) 
    {
        // If proocess utilizatiooon more than 1, stop running code
        printf("Processor Utilization more than 1 cannot run algorithm");
        return 0;
    }


    int t = 0;
    while(t < totalTime)
    {
        // Check if process will re-enter according to period
        for(int i = 0; i<n; i++)
        {
            if(t % processes[i].period  == 0)
            {   
                processes[i].executionTimeRemaining = processes[i].executionTime;
                processes[i].deadline = processes[i].originalDeadline + t;  // add period to deadline
                printf("P%d has entered, ", processes[i].id);
                printf("deadline:%d\n",  processes[i].deadline);
            }
        }

        // Check for nearest deadline by sorting the list (bubble sort)
        for(int j = 0; j<n;j++)
        {
            for(int i = 0; i<n; i++)
            {
                if(i+1 < n) //prevent accessing out of bounds
                {

                    if(processes[i+1].deadline - t < processes[i].deadline - t) // check to see which deadline is earliest
                    {
                        struct process temp = processes[i+1];
                        processes[i+1] = processes[i];
                        processes[i] = temp;
                    }
                }
            }
        }

        int processWithNearestDeadline = 0;
        bool idle = false;

        // Check if process did not finish, if it finished then execute process with next earliest deadline
        for(int i = 0; i<n;i++)
        {
            if(processes[i].executionTimeRemaining > 0)
            {
                processes[i].executionTimeRemaining -=1;
                processWithNearestDeadline = i;
                // printf("P%d remaining execution: %d\n", processes[i].id, processes[i].executionTimeRemaining);
                break;
            } else if (i==n-1) {
                idle = true;
            }
        }

        t++;
        if(idle){   //no process running
            printf("T%d: IDLE\n", t);
        }else {
            printf("T%d: P%d\n", t, processes[processWithNearestDeadline].id);
        }
    }

    return 0;
}
