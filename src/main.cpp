#include "Color.hpp"
#include "QuadtreeNode.hpp"
#include "Quadtree.hpp"
#include "utility.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "library/stb_image.h"
#include "library/stb_image_write.h"
#include <chrono> 
#include <filesystem>
using namespace std::chrono;

int main(){

    string addressPath, savePath, GIFPath;
    int errorMethod = 0;
    double compressionRate = 0;
    unsigned char* imageData;
    double percentage = 0;
    int nNode = 0, depthTree = 0;

    double executeTime = 0;

    int width, height, channels;

    cout << "Masukkan alamat gambar yang ingin dikompresi: ";
    getline(cin, addressPath);

    cout << "Masukkan alamat tempat menyimpan gambar: ";
    getline(cin, savePath);

    cout << "Masukkan alamat tempat menyimpan GIF: ";
    getline(cin, GIFPath);

    unsigned char* data = stbi_load(addressPath.c_str(), &width, &height, &channels, 3);

    vector<vector<Color>> image = imageToVector(data,width,height,channels);

    uintmax_t sizeBytes = filesystem::file_size(addressPath);


    do{

        cout << "Berikut error method yang bisa kamu pilih: \n";
        cout << "1. Variance\n";
        cout << "2. Median Absolute Deviation\n";
        cout << "3. Max Pixel Difference\n";
        cout << "4. Enthrophy\n";
        cout << "5. SSIM\n";
        cout << "Pilih error method : ";
        cin >> errorMethod;

        if (cin.fail()) {
            cout << "Itu bukan angka, coba lagi!\n";
            cin.clear();              
            cin.ignore(1000, '\n');   
        }else if (errorMethod < 1 && errorMethod > 5){
            cout << "Pilih angka di rentang 1 - 5\n";
        }
        
    }while (errorMethod < 1 && errorMethod > 5);
    
    while (true) {
        cout << "Masukkan compression rate: ";
        cin >> compressionRate;
        
        if (cin.fail()) {
            cout << "Itu bukan angka, coba lagi!\n";
            cin.clear();              
            cin.ignore(1000, '\n');   
        } else {
            break; 
        }
    }
    
    if (compressionRate == 0){
        
        double threshold = 0;
        int minBlock = 1;

        while (true) {
            cout << "Masukkan threshold: ";
            cin >> threshold;
            if (cin.fail()) {
                cout << "Itu bukan angka, coba lagi!\n";
                cin.clear();              
                cin.ignore(1000, '\n');   
            } else {
                break; 
            }
        }
        
        while (true) {
            cout << "Masukkan minimal block size: ";
            cin >> minBlock;
            
            if (cin.fail()) {
                cout << "Itu bukan angka, coba lagi!\n";
                cin.clear();              
                cin.ignore(1000, '\n');   
            } else {
                break; 
            }
        }

        cout << "Sedang mengkompresi...\n";
        cout << "Loading... 0%\n";
        
        auto start = high_resolution_clock::now();
        Quadtree quadtree(image,errorMethod,threshold,minBlock,0);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        executeTime = duration.count();

        vector<vector<Color>> compression(height, vector<Color>(width));
    
        mergeBlock(quadtree.getRoot(), compression, width, height);

        nNode = quadtree.countNode(quadtree.getRoot());
        depthTree = quadtree.countDepth(quadtree.getRoot(),0);

        imageData = convertToByteArray(compression, width, height, channels);
        
    }else{

        auto start = high_resolution_clock::now();
        cout << "Sedang mengkompresi...\n";
        cout << "Loading... 0%\n";
        imageData = imageByCompressionRate(image, compressionRate, errorMethod, sizeBytes, nNode, depthTree);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        executeTime = duration.count();
    }

    cout << "Complete... 100%\n";


    stbi_image_free(data);

    


    stbi_write_jpg(savePath.c_str(), width, height, channels, imageData, 90);

    createGif(image,imageData,GIFPath.c_str(),width, height, depthTree);

    cout << "Waktu eksekusi: " << executeTime << " ms" << endl;



    cout << "Ukuran gambar sebelum: " << sizeBytes << " Bytes" << endl;
    cout << "Ukuran gambar sesudah: " << filesystem::file_size(savePath) << " Bytes" << endl;

    percentage = (1 -((double) filesystem::file_size(savePath) / sizeBytes)) * 100;

    cout << "Persentase kompresi: " << percentage << " %" << endl;

    
    cout << "Node: " << nNode << endl;
    cout << "depth: " << depthTree << endl;
    
    delete[] imageData;
    
}