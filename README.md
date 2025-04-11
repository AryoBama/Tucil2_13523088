# Tucil2_13523088
## Image Compression by Quadtree
Made by Aryo Bama Wiratama, 13523088

## Description
Kompresi gambar adalah proses mengurangi ukuran file gambar tanpa mengorbankan kualitas secara signifikan (lossless) atau dengan kehilangan informasi tertentu yang tidak terlalu terlihat oleh mata manusia (lossy). Tujuannya adalah untuk menghemat ruang penyimpanan, mempercepat pengiriman melalui internet, dan meningkatkan efisiensi penyimpanan serta transmisi data. Salah satu metode kompresi gambar adalah dengan menggunakan struktur data quadtree. Quadtree merupakan struktur data berjenjang yang berfungsi untuk memecah ruang atau data menjadi segmen-segmen lebih kecil, dan sering diaplikasikan dalam pemrosesan citra. Ketika digunakan untuk kompresi gambar, Quadtree membagi citra menjadi blok-blok kecil berdasarkan homogenitas warna atau nilai intensitas piksel. Proses ini dimulai dengan membagi gambar menjadi empat bagian, kemudian mengevaluasi apakah setiap kuadran memiliki keseragaman nilai berdasarkan analisis komponen warna RGB, dengan cara membandingkan komposisi nilai merah (R), hijau (G), dan biru (B) pada piksel-piksel dalam kuadran tersebut. Apabila suatu bagian tidak memiliki nilai yang homogen, maka bagian tersebut akan terus dibagi hingga mencapai tingkat keseragaman yang diinginkan atau hingga mencapai ukuran blok minimum yang telah ditetapkan.
Dalam implementasi teknis, sebuah Quadtree direpresentasikan sebagai simpul (node) dengan maksimal empat anak (children). Simpul daun (leaf) merepresentasikan area gambar yang seragam, sementara simpul internal menunjukkan area yang masih membutuhkan pembagian lebih lanjut. Setiap simpul menyimpan informasi seperti posisi (x, y), ukuran (width, height), dan nilai rata-rata warna atau intensitas piksel dalam area tersebut.

Algoritma divide and conquer dapat digunakan untuk mengimplementasikan kompresi menggunakan quadtree. Berikut adalah ide penggunaan algoritma divide conquer dalam pengkompresian gambar menggunakan struktur data quadtree:

1.	Solve: 
Jika blocksize (ukuran block) ≤ minblock ATAU suatu blok dianggap homogen (umumnya error < threshold), lakukan normalisasi warna blok sesuai dengan rata-rata nilai RGB blok.
2.	Divide:
Jika blocksize (ukuran block) ≥ minblock DAN suatu blok dianggap tidak homogen (umumnya error > threshold), bagi blok menjadi 4 subblok sama besar.
3.	Conquer
Untuk setiap subblock, ulangi proses solve dan divide secara rekursif.
4.	Combine
Setelah semua subblok diproses, gabungkan informasi dari subblok-subblok pada struktur quadtree untuk membentuk suatu gambar yang telah terkompresi


## Getting Started
1. Untuk menjalankan program ini, pastikan sudah menginstal compiler MinGW untuk C++. Versi C++ yang digunakan adalah C++17
2. Program ini dibuat di lingkungan Windows. Penggunaan di sistem operasi lain dapat menyebabkan kesalahan.
3. Unduh atau klon repository ini
   ```bash
   https://github.com/AryoBama/Tucil2_13523088.git
   ```
4. Pindah ke direktori project
   ```bash
   Tucil2_13523088
   ```
5. Compile program
   ```bash
   make build
   ```
6. Jalankan program
   ```bash
   make run
   ```
7. Pengguna dapat menjalankan file executable langsung

8. Pindah ke direktori bin
   ```bash
   cd bin
   ```
9. Jalankan program
   ```bash
   ./main
   ```
10. Alternatif command jika compile gagal menggunakan command
    ```
    g++ -g -Wall -std=c++17 src/Color.cpp src/Quadtree.cpp src/QuadtreeNode.cpp src/main.cpp src/utility.cpp -o bin/main
    ```
## Library
Projek ini menggunakan single header library untuk memudahkan dalam melakukan image processing, sebagai berikut:
1.	stb_image.h
Digunakan untuk membaca gambar
Berikut source code dari library di atas: 
https://github.com/nothings/stb/blob/master/stb_image.h
2.	stb_write_image.h
Digunakan untuk membuat gambar
Berikut source code dari library di atas:
https://github.com/nothings/stb/blob/master/stb_image_write.h
3.	gif.h
Digunakan untuk membuat gif

Berikut source code dari library di atas:
https://github.com/charlietangora/gif-h/blob/master/gif.h#L766

Tentu saja penggunaan library dimaksudkan untuk mempermudah proses dalam tugas ini. Penggunaan library ini bukan untuk mencari hasil instan, melainkan sebagai alat bantu yang efisien agar dapat lebih fokus pada logika inti dari program yang saya kerjakan
