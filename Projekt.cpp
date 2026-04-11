#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>

using namespace std;

enum class TypDanych { LICZBA };

struct Element {
    TypDanych typ;
    double wartosc = 0.0;
    int precyzja = 0;
};

class ParserJson {
    string caly_tekst;
    size_t pozycja = 0;

    void pomin_spacje() {
        while (pozycja < caly_tekst.length() && isspace(caly_tekst[pozycja])) {
            pozycja++;
        }
    }

public:
    ParserJson(string tekst_wejsciowy) : caly_tekst(tekst_wejsciowy) {}

    Element czytaj_liczbe() {
        pomin_spacje();
        
        size_t poczatek = pozycja;
        int pozycja_kropki = -1;

        while (pozycja < caly_tekst.length() && (isdigit(caly_tekst[pozycja]) || caly_tekst[pozycja] == '.' || caly_tekst[pozycja] == '-')) {
            
            if (caly_tekst[pozycja] == '-' && pozycja != poczatek) {
                break;
            }

            if (caly_tekst[pozycja] == '.') {
                if (pozycja_kropki != -1) {
                    break;
                }
                pozycja_kropki = pozycja;
            }

            pozycja++;
        }

        Element wynik;
        wynik.typ = TypDanych::LICZBA;
        
        string wycinek = caly_tekst.substr(poczatek, pozycja - poczatek);
        wynik.wartosc = stod(wycinek);
        
        if (pozycja_kropki != -1) {
            wynik.precyzja = pozycja - (pozycja_kropki + 1);
        } else {
            wynik.precyzja = 0;
        }

        return wynik;
    }
};

int main() {
    string tekst_testowy = "    -8476784.73648967khdgfh   "; 
    
    ParserJson parser(tekst_testowy);
    Element wynik = parser.czytaj_liczbe();

    cout << fixed << setprecision(wynik.precyzja);
    cout << "Wyciagnieta liczba: " << wynik.wartosc << endl;

}