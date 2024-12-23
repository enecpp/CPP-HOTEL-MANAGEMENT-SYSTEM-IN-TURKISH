#include<iostream>
#include<string.h>

#define max 100
using namespace std;

// Müşteri Sınıfı
class Musteri
{
public:
    char isim[100];
    char adres[100];
    char telefon[12];
    char giris_tarihi[20];
    char cikis_tarihi[20];
    float on_odeme;
    int rezervasyon_id;
};

class Oda
{
public:
    char tip;
    char konfor_tip;
    char klima;
    int oda_numarasi;
    int kira;
    int durum;

    Musteri musteri;
    Oda odaEkle(int);
    void odaAra(int);
    void odaSil(int);
    void odaGoster(Oda);
};


Oda odalar[max];
int sayac = 0;

Oda Oda::odaEkle(int rno)
{
    Oda oda;
    oda.oda_numarasi = rno;
    cout << "\nTip Klima/Klimasiz (K/N) : ";
    cin >> oda.klima;
    cout << "\nTip Konfor (L/S) : ";
    cin >> oda.tip;
    cout << "\nTip Boyut (B/K) : ";
    cin >> oda.konfor_tip;
    cout << "\nGunluk Kira : ";
    cin >> oda.kira;
    oda.durum = 0;

    cout << "\n Oda Basariyla Eklendi!";
    return oda;
}

void Oda::odaAra(int rno)
{
    int bulundu = 0;
    int i;
    for (i = 0; i < sayac; i++)
    {
        if (odalar[i].oda_numarasi == rno)
        {
            bulundu = 1;
            break;
        }
    }
    if (bulundu == 1)
    {
        cout << "Oda Detaylari\n";
        if (odalar[i].durum == 1)
        {
            cout << "\nOda Rezerve Edilmis";
        }
        else
        {
            cout << "\nOda Musait";
        }
        odaGoster(odalar[i]);
    }
    else
    {
        cout << "\nOda bulunamadi";
    }
}

void Oda::odaGoster(Oda tempOda)
{
    cout << "\nOda Numarasi: \t" << tempOda.oda_numarasi;
    cout << "\nTip Klima/Klimasiz (K/N): " << tempOda.klima;
    cout << "\nTip Konfor (L/S): " << tempOda.tip;
    cout << "\nTip Boyut (B/K): " << tempOda.konfor_tip;
    cout << "\nKira: " << tempOda.kira;
}


class OtelYonetim : protected Oda
{
public:
    void checkIn();
    void musaitOdaGetir();
    void musteriAra(char*);
    void checkOut(int);
    void misafirOzetRaporu();
};

void OtelYonetim::misafirOzetRaporu()
{
    if (sayac == 0)
    {
        cout << "\n Otelde Misafir Yok !!";
    }
    for (int i = 0; i < sayac; i++)
    {
        if (odalar[i].durum == 1)
        {
            cout << "\n Musteri Adi : " << odalar[i].musteri.isim;
            cout << "\n Oda Numarasi : " << odalar[i].oda_numarasi;
            cout << "\n Adres (sadece sehir) : " << odalar[i].musteri.adres;
            cout << "\n Telefon : " << odalar[i].musteri.telefon;
            cout << "\n---------------------------------------";
        }
    }
}


void OtelYonetim::checkIn()
{
    int bulundu = 0, rno;

    cout << "\nOda numarasini girin : ";
    cin >> rno;
    for (int i = 0; i < sayac; i++)
    {
        if (odalar[i].oda_numarasi == rno)
        {
            bulundu = 1;
            if (odalar[i].durum == 1)
            {
                cout << "\nOda zaten rezerve edilmis";
                return;
            }

            cout << "\nRezervasyon ID'si girin: ";
            cin >> odalar[i].musteri.rezervasyon_id;

            cout << "\nMusteri Adi (Ilk Adi): ";
            cin >> odalar[i].musteri.isim;

            cout << "\nAdres (sadece sehir): ";
            cin >> odalar[i].musteri.adres;

            cout << "\nTelefon: ";
            cin >> odalar[i].musteri.telefon;

            cout << "\nGiris Tarihi: ";
            cin >> odalar[i].musteri.giris_tarihi;

            cout << "\nCikis Tarihi: ";
            cin >> odalar[i].musteri.cikis_tarihi;

            cout << "\nOn Odeme: ";
            cin >> odalar[i].musteri.on_odeme;

            odalar[i].durum = 1;

            cout << "\n Musteri Basariyla Check-in Yapti..";
            break;
        }
    }
    if (!bulundu)
    {
        cout << "\nOda bulunamadi";
    }
}


void OtelYonetim::musaitOdaGetir()
{
    int bulundu = 0;
    for (int i = 0; i < sayac; i++)
    {
        if (odalar[i].durum == 0)
        {
            odaGoster(odalar[i]);
            cout << "\n\nSonraki oda icin enter tusuna basin";
            bulundu = 1;
        }
    }
    if (!bulundu)
    {
        cout << "\nTum odalar rezerve edilmis";
    }
}


void OtelYonetim::musteriAra(char* isim)
{
    int bulundu = 0;
    for (int i = 0; i < sayac; i++)
    {
        if (odalar[i].durum == 1 && strcmp(odalar[i].musteri.isim, isim) == 0)
        {
            cout << "\nMusteri Adi: " << odalar[i].musteri.isim;
            cout << "\nOda Numarasi: " << odalar[i].oda_numarasi;

            cout << "\n\nSonraki kayit icin enter tusuna basin\n";
            bulundu = 1;
        }
    }
    if (!bulundu)
    {
        cout << "\nKisi bulunamadi.\n";
    }
}


void OtelYonetim::checkOut(int odaNumarasi)
{
    int bulundu = 0, gunler;
    float faturaTutari = 0;
    for (int i = 0; i < sayac; i++)
    {
        if (odalar[i].durum == 1 && odalar[i].oda_numarasi == odaNumarasi)
        {
            bulundu = 1;
            cout << "\nGun Sayisini Girin:\t";
            cin >> gunler;
            faturaTutari = gunler * odalar[i].kira;

            cout << "\n\t######## CheckOut Detaylari ########\n";
            cout << "\nMusteri Adi : " << odalar[i].musteri.isim;
            cout << "\nOda Numarasi : " << odalar[i].oda_numarasi;
            cout << "\nAdres : " << odalar[i].musteri.adres;
            cout << "\nTelefon : " << odalar[i].musteri.telefon;
            cout << "\nToplam Borc : " << faturaTutari << " /";
            cout << "\nOn Odeme: " << odalar[i].musteri.on_odeme << " /";
            cout << "\n*** Odenecek Toplam: " << faturaTutari - odalar[i].musteri.on_odeme << "/ sadece";

            odalar[i].durum = 0;
            break;
        }
    }
    if (!bulundu)
    {
        cout << "\nOda bulunamadi veya zaten check-out yapilmis";
    }
}


void odalariYonet()
{
    Oda oda = Oda();
    int secim, rno, bulundu = 0;
    do
    {
        cout << "\n### Odalari Yonet ###";
        cout << "\n1. Oda Ekle";
        cout << "\n2. Oda Ara";
        cout << "\n3. Ana Menuye Don";
        cout << "\n\nSecenek Girin: ";
        cin >> secim;

        switch (secim)
        {
        case 1:
            cout << "\nOda Numarasi Girin: ";
            cin >> rno;
            for (int i = 0; i < sayac; i++)
            {
                if (odalar[i].oda_numarasi == rno)
                {
                    bulundu = 1;
                    break;
                }
            }
            if (bulundu == 1)
            {
                cout << "\nOda Numarasi Mevcut.\nLutfen benzersiz bir numara girin";
                bulundu = 0;
            }
            else
            {
                odalar[sayac] = oda.odaEkle(rno);
                sayac++;
            }
            break;
        case 2:
            cout << "\nOda numarasini girin: ";
            cin >> rno;
            oda.odaAra(rno);
            break;
        case 3:

            break;
        default:
            cout << "\nLutfen dogru secenegi girin";
            break;
        }
    } while (secim != 3);
}


int main()
{
    OtelYonetim hm;
    int secim, rno;
    char isim[100];

    do
    {
        cout << "######## Otel Yonetimi #########\n";
        cout << "\n1. Odalari Yonet";
        cout << "\n2. Odaya Check-In Yap";
        cout << "\n3. Musait Odalar";
        cout << "\n4. Musteri Ara";
        cout << "\n5. Odaya Check-Out Yap";
        cout << "\n6. Misafir Ozet Raporu";
        cout << "\n7. Cikis";
        cout << "\n\nSecenek Girin: ";
        cin >> secim;
        switch (secim)
        {
        case 1:
            odalariYonet();
            break;
        case 2:
            if (sayac == 0)
            {
                cout << "\nOda verisi mevcut degil.\nLutfen once odalari ekleyin.";
            }
            else
            {
                hm.checkIn();
            }
            break;
        case 3:
            if (sayac == 0)
            {
                cout << "\nOda verisi mevcut degil.\nLutfen once odalari ekleyin.";
            }
            else
            {
                hm.musaitOdaGetir();
            }
            break;
        case 4:
            if (sayac == 0)
            {
                cout << "\nOdalar mevcut degil.\nLutfen once odalari ekleyin.";
            }
            else
            {
                cout << "Musteri Adini Girin: ";
                cin >> isim;
                hm.musteriAra(isim);
            }
            break;
        case 5:
            if (sayac == 0)
            {
                cout << "\nOdalar mevcut degil.\nLutfen once odalari ekleyin.";
            }
            else
            {
                cout << "Oda Numarasini Girin : ";
                cin >> rno;
                hm.checkOut(rno);
            }
            break;
        case 6:
            hm.misafirOzetRaporu();
            break;
        case 7:
            cout << "\nYAZILIMI KULLANDIGINIZ ICIN TESEKKUR EDERIZ\n";
            break;
        default:
            cout << "\nLutfen dogru secenegi girin";
            break;
        }
    } while (secim != 7);

    return 0;
}