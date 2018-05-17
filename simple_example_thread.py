import threading
import random
import time

MAX_ITERATION = 5
current_thread = 0
semaphore = threading.Semaphore()

def function():
    global current_thread
    thread_id = current_thread
    current_thread += 1
    for i in range(MAX_ITERATION):
        time.sleep(5*random.random())
        semaphore.acquire()
        print(f'Thread: {thread_id:2} | Iteração: {i+1}')
        semaphore.release()

def main():
    n_thread = int(input('Digite a quantidade de threads: '))
    for i in range(n_thread):
        thread = threading.Thread(target=function)
        thread.start()
    for i in range(n_thread):
        thread.join()

if __name__ == '__main__':
    main()
