#include <iostream>
#include <ctime>
#include <cmath>
using namespace std;
#define K 6
#define N 10

int main() {
    cout << "ZADATAK: ";
    cout << endl;
    srand(time(nullptr));
    bool matrix[K][N] = {{true, true, true, false,  false, false, false, false, false, false },
                         {true, false, false, true,  true, false, false, false, false, false },
                         {false, true, false, true,  false, true, false, false, false, false },
                         {true, true, false, true,  false, false, true, false, false, false },
                         {true, false, false, false,  false, false, false, true, true, false },
                         {false, true, false, false,  false, false, false, true, false, true }};
    int tezina[64] = {0};
    int tezine[64] = {0};
    bool bit;
    int i = 0;
    int cnt = 1;
    cout << "\nKodne reci:\n";
    for (int a = 0; a < 2; a++) {
        for (int b = 0; b < 2; b++) {
            for (int c = 0; c < 2; c++) {
                for (int d = 0; d < 2; d++) {
                    for (int e = 0; e < 2; e++) {
                        for (int f = 0; f < 2; f++) {
                            cout << cnt++ << ". ";
                            for (int j = 0; j < N; j++) {
                                if ((bit = a & matrix[0][j] ^ b & matrix[1][j] ^ c & matrix[2][j] ^ d & matrix[3][j] ^ e & matrix[4][j] ^ f & matrix[5][j])) tezina[i]++;
                                cout << bit;
                            }
                            cout << " =>" << " d = " << tezina[i] << endl;
                            tezine[tezina[i++]]++;
                        }
                    }
                }
            }
        }
    }
    int d = 0;
    for (int j = 0; j <= N; j++) {
        cout << "\nBroj kodnih reci sa tezinom ";
        cout << j << " je " << tezine[j] << "." << endl;
        if (!d && j && tezine[j]) d = j;
    }
    int g = -1;
    int c = -1;
    while (d >= 2 * (++c + 1) + 1);
    while (d >= c + (++g + 1) + 1);
    cout << "\nOvaj kod moze da detektuje ";
    if(g == 1){
        cout << g << " gresku";
    }else{
        cout << g << " gresaka";
    }
    cout << ", a ispravi ";
    if(c == 1){
        cout << c << " gresku.\n";
    }else{
        cout << c << " gresaka.\n";
    }

    cout << "Zadatak 2: ";
    cout << endl;
    int T = 1000;
    bool sindromi[N][N - K] = {     {false,  false,  false, true},
                                    {false,  false, true, false},
                                    {false,  false, true, true},
                                    {false, true,  false, false},
                                    {false, true,  false, true},
                                    {false, true,  true, false},
                                    {false, true,  true, true},
                                    {true, false,  false, false},
                                    {true, false,  false, true},
                                    {true, false,  true, false}};

    int greska,sindrom,interliverPara;
    bool izvor[K * T];
    bool zasticenaPredajnik[N * T];
    int predajnikCounter = 0;
    bool interliver[T][T];
    bool kanal[N * T];
    int kanalCounter = 0;
    bool deinterliver[T][T];
    bool zasticenaPrijemnik[N * T];
    int prijemnikCounter = 0;
    int brojGresaka = 0;
    cout << endl;
    cout << "Od kog bita unositi paket od 3 greske na svakih 50 bitova? ";
    cin >> greska;
    cout << "Koje parametre interlivera koristiti (1000 mora biti deljiv ovim brojem)? ";
    cin >> interliverPara;
    cout << endl;
    cout << "Izvorna poruka:";
    cout << endl;
    for (int j = 0; j < K * T; j++) {
        izvor[j] = rand() % 2;
        cout << izvor[j];
        if (j % 6 == 5) cout << " \n"[(j + 6) % 102 == 5];
    }
    cout << endl;
    cout << "Generisane kodne reci:";
    cout << endl;
    // Proces zastitnog kodovanja
    for (int p = 0; p < K * T; p += 6) {
        for (int j = 0; j < N; j++) {
            zasticenaPredajnik[predajnikCounter] =
                    izvor[p + 0] & matrix[0][j] ^
                    izvor[p + 1] & matrix[1][j] ^
                    izvor[p + 2] & matrix[2][j] ^
                    izvor[p + 3] & matrix[3][j] ^
                    izvor[p + 4] & matrix[4][j] ^
                    izvor[p + 5] & matrix[5][j];
            cout << zasticenaPredajnik[predajnikCounter++];
        }
        cout << " \n"[(p + 6) % 48 == 0];
    }
    // Interliver (punjenje i praznjenje)
    for (int p = 0; p < predajnikCounter;) {
        for (int j = 0; j < interliverPara; j++) {
            for (int k = 0; k < 10; k++) {
                interliver[j][k] = zasticenaPredajnik[p++];
            }
        }

        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < interliverPara; k++) {
                kanal[kanalCounter++] = interliver[k][j];
            }
        }
    }
    // Pojava greske/i u kanalu
    for (int g = greska; g < kanalCounter;) {
        kanal[g - 1] = !kanal[g - 1];
        kanal[g + 0] = !kanal[g + 0];
        kanal[g + 1] = !kanal[g + 1];
        g += 50;
    }
    // Deinterliver(punjenje i praznjenje)
    for (int p = 0; p < kanalCounter;) {
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < interliverPara; k++) {
                deinterliver[k][j] = kanal[p++];
            }
        }

        for (int j = 0; j < interliverPara; j++) {
            for (int k = 0; k < 10; k++) {
                zasticenaPrijemnik[prijemnikCounter++] = deinterliver[j][k];
            }
        }
    }
    // Nakon praznjenja deinterlivera sledi dekodovanje poruke
    for (int p = 0; p < N * T; p += 10) {
        sindrom = 0;
        for (int j = 0; j < 4; j++) {
            sindrom += (zasticenaPrijemnik[p + 0] & sindromi[0][j] ^
                         zasticenaPrijemnik[p + 1] & sindromi[1][j] ^
                         zasticenaPrijemnik[p + 2] & sindromi[2][j] ^
                         zasticenaPrijemnik[p + 3] & sindromi[3][j] ^
                         zasticenaPrijemnik[p + 4] & sindromi[4][j] ^
                         zasticenaPrijemnik[p + 5] & sindromi[5][j] ^
                         zasticenaPrijemnik[p + 6] & sindromi[6][j] ^
                         zasticenaPrijemnik[p + 7] & sindromi[7][j] ^
                         zasticenaPrijemnik[p + 8] & sindromi[8][j] ^
                         zasticenaPrijemnik[p + 9] & sindromi[9][j]) * (int) pow(2,  3 - j);
        }
        if (sindrom) {zasticenaPrijemnik[p + sindrom - 1] = !zasticenaPrijemnik[p + sindrom - 1];} // Ispravljanje greske
    }
    double cnst = 10.0/6.0;
    cout << endl;
    cout << "Dekodovana poruka (Originalna poruka)"; // paralelno poredjenje po recima
    cout << endl;
    for (int p = 0; p < N * T; p += 10) {
        if (zasticenaPrijemnik[p + 2] != izvor[int(round(p/cnst))] || zasticenaPrijemnik[p + 4] != izvor[int(round(p/cnst)) + 1] ||
            zasticenaPrijemnik[p + 5] != izvor[int(round(p/cnst)) + 2] || zasticenaPrijemnik[p + 6] != izvor[int(round(p/cnst)) + 3] ||
            zasticenaPrijemnik[p + 8] != izvor[int(round(p/cnst)) + 4] || zasticenaPrijemnik[p + 9] != izvor[int(round(p/cnst)) + 5])
            brojGresaka++;
        cout << zasticenaPrijemnik[p + 2] << zasticenaPrijemnik[p + 4] << zasticenaPrijemnik[p + 5] << zasticenaPrijemnik[p + 6] << zasticenaPrijemnik[p + 8] << zasticenaPrijemnik[p + 9]
             << " (" << izvor[int(round(p/cnst))] << izvor[int(round(p/cnst)) + 1] << izvor[int(round(p/cnst)) + 2]  << izvor[int(round(p/cnst)) + 3] << izvor[int(round(p/cnst)) + 4] << izvor[int(round(p/cnst)) + 5] << ") " << " \n"[(p + 10) % 12 == 0];
    }

    cout << "\nProcenat lose prenesenih bita je " << (brojGresaka * 100) / T << "%\n"; //greska bi trebalo da postoji samo u slucaju kada su parametri (de)interlivera manji od velicine paketa greske
}