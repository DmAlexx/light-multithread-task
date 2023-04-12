// all of function in this progam is doing job in multithread

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>

//this function generate random numbers during 60 sec, every second 5 values (total numbers of generated values is 300)
//FIRST THREAD
void generateRandomNumbers(std::queue<int>& numbersQueue, std::mutex& mtx)
{
	srand(time(NULL));
	auto start_time = std::chrono::steady_clock::now();
	while (std::chrono::steady_clock::now() - start_time < std::chrono::seconds(60))
	{
		for (int i = 0; i < 5; ++i)
		{
			mtx.lock();
			numbersQueue.push(rand());
			mtx.unlock();
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

//this function is doing console output all generated values, one value every second
//(300 values during 300 seconds / 60 seconds = 5 minutes (to output all values))
//SECOND THREAD

void consoleOut(std::queue<int>& numbersQueue, std::mutex& mtx)
{
	while (numbersQueue.size() > 0)
	{
		mtx.lock();
		std::cout << numbersQueue.front() << std::endl;
		numbersQueue.pop();
		mtx.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main()
{
	std::mutex mtx; // using this variable to except possibility of simultaneous use of numbersQueue
	std::queue<int> numbersQueue;

	// creating first thread
	std::thread generator(generateRandomNumbers, std::ref(numbersQueue), std::ref(mtx));
	// creating second thread
	std::thread output(consoleOut, std::ref(numbersQueue), std::ref(mtx));

	generator.join(); // waiting for end job of first thread
	output.join(); // waiting for end job of second thread
	return 0;
}