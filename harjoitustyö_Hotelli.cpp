//Joona Saario
//Hotellinvaraus harjoitustyö 3 pistettä.

#include <iostream>  // Sisältää I/O-kirjaston (cin ja cout)
#include <string>     // merkkijonot
#include <vector>     // huoneiden taulukointi
#include <cstdlib>    // satunnaislukuja varten
#include <ctime>      
#include <algorithm>  // sisältää yleisiä algoritmeja
#include <clocale>    // kielen vaihdos
#include <limits>     // sisältää numeric_limitsin syötteentarkistusta varten.


using namespace std;

// Huoneen määrittely
struct Huone {
    int numero;       // Huoneen numero
    bool varattu;     // Onko huone varattu
    string varaaja;   // Varaajan nimi
    int varausnumero; // Varausnumero (arvotaan)
    int varauksenPituus; // öiden määrä
};

//näytön tyhjennys, selkeyttää valikkoa
void UusiSivu() {
#ifdef _WIN32
    system("cls");  // Windows
#else
    system("clear"); // Unix
#endif
}

// Huoneiden alustus (määrä ja hinta)
void alustaHuoneet(vector<Huone>& huoneet, int& huoneen_hinta) {
    srand(static_cast<unsigned int>(time(0))); // satunnaislukugeneraattori

    int huoneiden_maara = 30 + rand() % 41; // Arvotaan määrä väliltä 30-70.
    huoneen_hinta = 80 + rand() % 21; // Arvotaan hinta per yö 80-100 euron väliltä.

    // luodaan huoneet ja lisätään vektoriin
    for (int i = 0; i < huoneiden_maara; ++i) {
        huoneet.push_back({i + 1, false, "", 0, 0}); // Alustetaan huone numerolla i + 1, huoneet alussa vapaita
    }
}

// varausnumero (10000-99999)
int varaus_numero() {
    return 100000 + rand() % 900000;
}

// vapaiden huoneiden funktio
void vapaat_huoneet(const vector<Huone>& huoneet) {
    cout << "Vapaat huoneet: ";
    // käy läpi kaikki vapaat huoneet
    for (size_t i = 0; i < huoneet.size(); ++i) {
        if (!huoneet[i].varattu) {
            cout << huoneet[i].numero << " "; // Tulostetaan huoneen numero
        }
    }
    cout << endl;
}

// syötteentarkistus kokonaisluvuille
int syotteen_tarkistus() {
    int syote;
    while (!(cin >> syote)) {
        cin.clear(); // poistaa virheet
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // poistaa virheelliset syötteet
        cout << "Virheellinen syöte, yritä uudelleen: ";
    }
    return syote;
}

// syötteentarkistus merkkijonolle
string merkkijonon_tarkistus() {
    string syote;
    cin.ignore();
    getline(cin, syote); // lukee koko rivin
    return syote;
}

// huoneen varausfunktio
void huoneen_varaus(vector<Huone>& huoneet, int huoneen_hinta) {
    UusiSivu();

    // tarkistetaan, onko kaikki huoneet varattu
    if (all_of(huoneet.begin(), huoneet.end(), [](const Huone& h) { return h.varattu; })) {
        cout << "Kaikki huoneet on jo varattu." << endl;
        return;
    }

    // etsitään vapaat huoneet ja lisätään ne vektoriin
    vector<int> vapaatHuoneet;
    for (const auto& huone : huoneet) {
        if (!huone.varattu) {
            vapaatHuoneet.push_back(huone.numero);
        }
    }

    // arpoo vapaan huoneen
    srand(time(0));
    int randomIndex = rand() % vapaatHuoneet.size();
    int huonenumero = vapaatHuoneet[randomIndex];

    // pyydetään käyttäjältä öiden määrä
    int oiden_maara;
    cout << "Syötä öiden määrä: ";
    oiden_maara = syotteen_tarkistus();

    if (oiden_maara <= 0) {
        cout << "Öiden määrä ei voi olla nolla tai negatiivinen." << endl;
        return;
    }

    // Varaajan nimi
    string varaaja;
    cout << "Syötä varaajan nimi: ";
    varaaja = merkkijonon_tarkistus();

    // arpoo varausnumeron
    int varausnumero = varaus_numero();

    // merkitään huone varatuksi ja tallennetaan varaajan nimi, varausnumero ja varauksen pituus
    huoneet[huonenumero - 1].varattu = true;
    huoneet[huonenumero - 1].varaaja = varaaja;
    huoneet[huonenumero - 1].varausnumero = varausnumero;
    huoneet[huonenumero - 1].varauksenPituus = oiden_maara;

    // Lasketaan varauksen loppusumma
    int loppusumma = huoneen_hinta * oiden_maara;

    // tulostaa varauksen tiedot
    cout << "Huone " << huonenumero << " varattu onnistuneesti!" << endl;
    cout << "Varausnumero: " << varausnumero << endl;
    cout << "Hinta per yö: " << huoneen_hinta << " euroa." << endl;
    cout << "Laskun loppusumma: " << loppusumma << " euroa." << endl;
}

// varauksen etsimis- ja poistamisfunktio
void etsi_varaus(vector<Huone>& huoneet, int huoneen_hinta) {
    UusiSivu();
    int valinta;
    cout << "Etsi tai poista varaus" << endl << endl;
    cout << "1. Etsi varausnumerolla" << endl;
    cout << "2. Etsi varaajan nimellä" << endl;
    cout << "3. Poista varaus" << endl;
    cout << "4. Palaa päävalikkoon" << endl << endl;
    cout << "Syötä valintasi: ";
    valinta = syotteen_tarkistus();

    switch (valinta) {
    case 1: { // Etsi varaus numerolla
        UusiSivu();
        int varausnumero;
        cout << "Syötä varausnumero: ";
        varausnumero = syotteen_tarkistus();

        // Etsitään varausnumero ja tarkistetaan, onko varaus voimassa
        bool loytyi = false;
        for (const auto& huone : huoneet) {
            if (huone.varausnumero == varausnumero) {
                // Tulostaa huonevarauksen tiedot
                cout << "Varaajan nimi: " << huone.varaaja << endl;
                cout << "Varausnumero: " << huone.varausnumero << endl;
                cout << "Huonenumero: " << huone.numero << endl;
                cout << "Varauksen pituus: " << huone.varauksenPituus << " yötä. " << endl;
                cout << "Kokonaishinta: " << huone.varauksenPituus * huoneen_hinta << " euroa" << endl;
                loytyi = true;
                break;
            }
        }

        if (!loytyi) {
            cout << "Varausta ei löytynyt." << endl;
        }

        cin.get();
        break;
    }
    case 2: { // Etsi varaus varaajan nimellä
        UusiSivu();
        string varaaja;
        cout << "Syötä varaajan nimi: ";
        varaaja = merkkijonon_tarkistus();

        bool loytyi = false;
        // Käydään läpi kaikki huoneet ja etsitään varaajan nimi
        for (const auto& huone : huoneet) {
            if (huone.varattu && huone.varaaja == varaaja) {
                cout << "Varaajan nimi: " << varaaja << endl;
                cout << "Varausnumero: " << huone.varausnumero << endl;
                cout << "Huonenumero: " << huone.numero << endl;
                cout << "Varauksen pituus: " << huone.varauksenPituus << " yötä" << endl;
                cout << "Kokonaishinta: " << huone.varauksenPituus * huoneen_hinta << " euroa" << endl;
                loytyi = true;
            }
        }

        if (!loytyi) {
            cout << "Varaajaa ei löytynyt." << endl;
        }

        cin.get();
        break;
    }
    case 3: { // Poista varaus
        UusiSivu();
        int varausnumero;
        cout << "Syötä poistettavan varauksen varausnumero: ";
        varausnumero = syotteen_tarkistus(); // Käytetään tarkistettua syötettä

        bool loytyi = false;
        for (auto& huone : huoneet) {
            if (huone.varausnumero == varausnumero) {
                huone.varattu = false;
                huone.varaaja = "";
                huone.varausnumero = 0;
                huone.varauksenPituus = 0;
                cout << "Varaus numero " << varausnumero << " poistettu." << endl;
                loytyi = true;
                break;
            }
        }

        if (!loytyi) {
            cout << "Varausta ei löytynyt." << endl;
        }

        cin.get();
        break;
    }
    case 4: {
        break;
    }
    default:
        cout << "Virheellinen valinta!" << endl;
    }
}

// Pääohjelma
int main() {
    setlocale(LC_ALL, "fi_FI");  // Asetetaan suomenkieliseksi

    vector<Huone> huoneet; // Vektori huoneista
    int huoneen_hinta; // Hinta per yö
    alustaHuoneet(huoneet, huoneen_hinta); // Alustetaan huoneet

    int valinta;
    while (true) {
        UusiSivu(); // Tyhjennetään näyttö
        cout << "Tervetuloa varausjärjestelmään!" << endl << endl;
        cout << "Valitse toiminto: " << endl;
        cout << "1. Varaa hotellihuone" << endl;
        cout << "2. Etsi tai poista varaus" << endl;
        cout << "3. Sulje ohjelma" << endl << endl;
        cout << ": ";
        valinta = syotteen_tarkistus();

        switch (valinta) {
        case 1:
            huoneen_varaus(huoneet, huoneen_hinta);  // Varataan huone
            break;
        case 2:
            etsi_varaus(huoneet, huoneen_hinta);  // Etsitään varaus
            break;
        case 3:
            cout << "Ohjelma sulkeutuu" << endl;
            return 0;
        default:
            cout << "Virheellinen valinta!" << endl;
        }
        cin.get();
    }
}