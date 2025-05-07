#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <thread>
#include <functional>
#include <algorithm>
#include <queue>
#include <future>
#include <vector>
#include <mutex>
#include <boost/filesystem.hpp>
#include <atomic>

using namespace std;
using namespace boost::filesystem;

//  Test function to simulate reading a file and returns its length.  
auto readFile = [](const string& filePath, int sleepMilli) -> size_t {
    int fileSize = 0;

    ifstream fs(filePath.c_str());

    size_t numBytes = 0;
    if(fs.is_open()) {
        fs.seekg(0, ios::end);
        numBytes = fs.tellg();
    }

    fs.close();
    if(sleepMilli > 0) {
        this_thread::sleep_for(chrono::milliseconds(sleepMilli));
    }

    return numBytes;
};

int main() {

    cout << "Enter a directory and we'll process all .txt files in it" << endl;
    cout << "Or Q to quit" << endl;
    string input;
    cin >> input;

    vector<future<size_t>> bytesRead;

    while(input != "Q") {
        path dir(input);
        if(is_directory(dir)) {
            cout << "Processing files in directory " << input << endl;
            for(auto entry: directory_iterator(dir)) {
                int sleepMillis = 100 + rand()%500;
                bytesRead.push_back(async(readFile, entry.path().string(), sleepMillis));
            }

        } else {
            cout << "Error - exiting" << input << endl;
        }
        cout << "Enter a directory and we'll process all .txt files in it" << endl;
        cout << "Or Q to quit" << endl << flush;
        cin >> input;
    }
    cout << "Counting bytes read...." << endl;
    size_t total = 0;
    for(auto iter = bytesRead.begin(); iter != bytesRead.end(); ++iter) {
        total += iter->get();
        cout << "Total = " << total << endl;
    }
    cout << "Exiting..." << endl;
}
