#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>
#include <vector>

using namespace std;

enum class TypDanych { LICZBA, TEKST, LOGICZNY, PUSTY, TABLICA, BLAD };

struct Element {
    TypDanych typ;
    double wartosc_liczba = 0.0;
    string wartosc_tekst = "";
    bool wartosc_bool = false;
    vector<Element> wartosc_tablica;
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
    ParserJson(string t) : caly_tekst(t) {}

    bool ma_wiecej() {
        pomin_spacje();
        return pozycja < caly_tekst.length();
    }

    Element czytaj_liczbe() {
        size_t poczatek = pozycja;
        int pk = -1;
        while (pozycja < caly_tekst.length() && (isdigit(caly_tekst[pozycja]) || caly_tekst[pozycja] == '.' || caly_tekst[pozycja] == '-')) {
            if (caly_tekst[pozycja] == '-' && pozycja != poczatek) break;
            if (caly_tekst[pozycja] == '.') {
                if (pk != -1) break;
                pk = pozycja;
            }
            pozycja++;
        }
        Element w; w.typ = TypDanych::LICZBA;
        string wycinek = caly_tekst.substr(poczatek, pozycja - poczatek);
        w.wartosc_liczba = stod(wycinek);
        w.precyzja = (pk != -1) ? (pozycja - (pk + 1)) : 0;
        return w;
    }

    Element czytaj_tekst() {
        pozycja++; 
        size_t poczatek = pozycja;
        while (pozycja < caly_tekst.length() && caly_tekst[pozycja] != '\"') {
            pozycja++;
        }
        Element w; w.typ = TypDanych::TEKST;
        w.wartosc_tekst = caly_tekst.substr(poczatek, pozycja - poczatek);
        pozycja++; 
        return w;
    }

    Element czytaj_slowa() {
        size_t poczatek = pozycja;
        while (pozycja < caly_tekst.length() && isalpha(caly_tekst[pozycja])) {
            pozycja++;
        }
        string s = caly_tekst.substr(poczatek, pozycja - poczatek);
        Element w;
        if (s == "true") { w.typ = TypDanych::LOGICZNY; w.wartosc_bool = true; }
        else if (s == "false") { w.typ = TypDanych::LOGICZNY; w.wartosc_bool = false; }
        else if (s == "null") { w.typ = TypDanych::PUSTY; }
        else { w.typ = TypDanych::BLAD; }
        return w;
    }

    Element czytaj_tablice() {
        Element w_tab; 
        w_tab.typ = TypDanych::TABLICA;
        pozycja++; 
        while (pozycja < caly_tekst.length() && caly_tekst[pozycja] != ']') {
            pomin_spacje();
            if (caly_tekst[pozycja] == ']') break;
            w_tab.wartosc_tablica.push_back(parsuj());
            pomin_spacje();
            if (caly_tekst[pozycja] == ',') pozycja++;
        }
        pozycja++; 
        return w_tab;
    }

    Element parsuj() {
        pomin_spacje();
        if (pozycja >= caly_tekst.length()) return {TypDanych::BLAD};
        char z = caly_tekst[pozycja];
        if (z == '[') return czytaj_tablice();
        if (z == '\"') return czytaj_tekst();
        if (isdigit(z) || z == '-') return czytaj_liczbe();
        if (isalpha(z)) return czytaj_slowa();
        return {TypDanych::BLAD};
    }
};

void wypisz_element(Element e) {
    if (e.typ == TypDanych::TABLICA) {
        for (const auto& sub : e.wartosc_tablica) {
            wypisz_element(sub); 
        }
    } else if (e.typ == TypDanych::LICZBA) {
        cout << fixed << setprecision(e.precyzja) << e.wartosc_liczba << endl;
    } else if (e.typ == TypDanych::TEKST) {
        cout << e.wartosc_tekst << endl;
    } else if (e.typ == TypDanych::LOGICZNY) {
        cout << (e.wartosc_bool ? "true" : "false") << endl;
    } else if (e.typ == TypDanych::PUSTY) {
        cout << "null" << endl;
    }
}

int main() {
    string wejscie, linia;
    while (getline(cin, linia)) {
        wejscie += linia + "\n";
    }

    if (wejscie.empty()) return 0;

    ParserJson parser(wejscie);

    while (parser.ma_wiecej()) {
        Element wynik = parser.parsuj();
        wypisz_element(wynik); 
    }

}
