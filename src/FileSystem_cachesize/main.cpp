#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

static const int FileNum = 10;

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

static const string root = "../filesystem/files/";

double clockcycleToNanosec(double cycles) {
    return cycles*10/24;
}
string gen_random(const int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}

void generateFixSizeFiles() {
    int filesize[FileNum+1];
    for (int i = 0; i <= FileNum; i++){
        filesize[i] = pow(2, i);
    }

    for (int i = 0; i <= FileNum; i++){
        string sizeString;
        string filename = "../filesystem/files/" + to_string(filesize[i]) + "MB.txt";
        ofstream MyFile(filename);
        string outputString = gen_random(filesize[i]*1024*1024);
        MyFile << outputString;
        MyFile.close();
    }

}

//void flush_FBC(){
//    string big_file_name = "../filesystem/files/7.txt";
//    ifstream big_file;
//    big_file.open(big_file_name);
//    string dumb_string = "";
//    if (big_file.is_open()){
//        while(big_file.good()){
//            big_file >> dumb_string;
//        }
//    }
//    big_file.close();
//}
//
//void FBC_size(){
//    string outputfile = "../filesystem/result/FBC_output_cpp.txt";
//    ofstream output(outputfile);
//
//    // Use 1GB File to flush FBC
//    flush_FBC();
//
//    string dumb_string = "dummy value";
//
//    // makesure that big file has flush the fbc
//    for (int i = 0; i < FileNum-1; i++) {
//        double time_total = 0;
//        for (int i = 0; i < TRIAL_NUM; i++){
//            double cycles_total = 0;
//            string filename = root + to_string(i) + ".txt";
//            ifstream test_file;
//            test_file.open(filename);
//            uint64_t start, end;
//            if (test_file.is_open()){
//                start = rdtsc();
//                test_file >> dumb_string;
//                end = rdtsc();
//                test_file.close();
//            }
//            cycles_total =  1.0*(end - start);
//            time_total += clockcycleToNanosec(cycles_total);
//
//        }
//        output << (to_string(i) + "\t");
//        output << (to_string(time_total/TRIAL_NUM) + "\n");
//    }
//
//}

int main() {
    generateFixSizeFiles();
    return 0;
}
