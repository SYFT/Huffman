#include <iostream>
#include <ctime>
#include <fstream>
#include <iomanip>
#include "huffmanCode.h"


int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    int start = time(0);
    if(argc == 3) {
        HuffmanCode::HCState S = HuffmanCode::decompress(argv[1], argv[2]);
        if(S == HuffmanCode::wrong_file)
            cout << "Can not operate files." << endl;
        else if(S == HuffmanCode::wrong_data)
            cout << "Something wrong happen." << endl;
        else {
            ifstream in(argv[1], ifstream::in);
            in.seekg(0, ios::end);
            double sizeA = (int) in.tellg();
            in.close();
            in.open(argv[2], ifstream::in);
            in.seekg(0, ios::end);
            double sizeB = (int) in.tellg();
            in.close();
            cout << "Origin filesize is " << fixed <<
                setprecision(2) << sizeA / 1024 << "KB" << endl;
            cout << "Filesize after decompressing is " << fixed <<
                setprecision(2) << sizeB / 1024 << "KB" << endl;
            int end = time(0);
            cout << "Cost time " <<
                (end - start) / 1000. << "s in total." << endl;
        }
    } else {
        cout << "Invalid parameter list." << endl;
        cout << "Try: " << argv[0] << " inputfilename outputfilename" << endl;
        cout << "See usage.txt for more help." << endl;
    }
    return 0;
}


