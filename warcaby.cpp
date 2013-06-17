// Leszek Pawlak nr 93897, Informatyka zaoczne, gr. 3B

// plan ogólny:
// tworzone są 3 klasy. 
// 'szachownica' odpowiada za plaszę gry i jej wyświetlanie.
// 'gracz' odpowiada za obsługę całej gry od strony programu.
// 'pionek' odpowiada za właściwości pionków.
//
// plansza jest na strukturze map. Stworzyłem mapę stringów o 64 polach.
// wypełniam pola na przemian znakami "" i "_" - oznaczają one odpowiednio niedozwolone i dozwolone pola.
// pionkami będą znaki "x" i "o". 
// gracze wykonują ruch po kolei w funkcji 'ruch'.
// wybór pionka którym chcemy ruszyć odbywa się poprzez wpisanie współrzędnych 'wiersz' i 'kolumna'.
// Program przekształci współrzędne w odpowiednie miejsce w tablicy.
// następnie wybieramy pole, na które ma pójść nasz pionek, przez wpisanie współrzędnych 'wiersz' i 'kolumna'.
// pionek zmienia swoje położenie, jeżeli wpisane dane są poprawne merytorycznie. 
// W przeciwnym razie gracz zostaje poproszony o powtórzenie współrzędnych.

// jeżeli wystąpiło bicie, pionki przeciwnika znikną. Jeżeli przeciwnik nie ma pionków, gra jest skończona. 

// Program nie obsługuje:
// 1. Damek.
// 2. Obowiązku bicia.
// 3. Zapisu do pliku / odczytu z pliku.
// 4. Gry z komputerem.

#include<iostream>
#include<cstdlib>
#include<map>
#include<limits>
using namespace std;

class pionek;
class gracz;
class szachownica;

class pionek{
public:
  string znak;
  int tablicaRuchow[4];
  bool JestemNaSzachownicy;
  bool nadpiszMnie;
  void wypelnianieTablicyRuchow(szachownica, int, int);
  int polozenie[2];
  pionek(){
    znak = "";
    JestemNaSzachownicy = false;
    nadpiszMnie = false;
  }
  pionek(string znak1){
    znak = znak1;
    JestemNaSzachownicy = false;
    nadpiszMnie = false;
  }
  pionek(map<int,string> &plansza, string znak1, int wiersz, int kolumna){
    for(int i=0; i<4; ++i)
      tablicaRuchow[i] = 0; 
    znak = znak1;
    polozenie[0] = wiersz;
    polozenie[1] = kolumna;
    int pole = (wiersz -1)*8 + kolumna -1;
    plansza[pole] = znak;
    JestemNaSzachownicy = true;
    nadpiszMnie = false;
  }
  ~pionek(){}
};

class szachownica{
public:
  map<int,string> tablica;
  string znakZTablicy(int,int);
  void disp();
  szachownica() { // tworzenie i wypełnianie szachownicy startowej.
    for (int i = 0; i < 64; ++i)
    {
      if (i % 8 == 0){
        if (tablica[i-1] == "")
          tablica[i] = "";
        else
          tablica[i] = "_";
      }
      else if(tablica[i-1] == "")
        tablica[i] = "_";
    }
  };
  ~szachownica() {};
};

class gracz{
private:
  pionek a1,a2,a3,a4,a5,a6,a7,a8;
  string znak;
  bool bicie(pionek, int, int, szachownica&);
  int zmianaPola(szachownica&,pionek&,gracz&);
  pionek wyszukajPionek(int, int, bool);
  void nadpisz(pionek);
  void zbijam(int, int);

public:
  int zwyciestwo(szachownica, gracz);
  int ruch(szachownica&,gracz&);
  gracz() {}
  gracz(map<int,string> &plansza, int liczba){
    if (liczba == 1){
      znak = "x";
      a1 = pionek(plansza,znak,1,2);
      a2 = pionek(plansza,znak,1,4);
      a3 = pionek(plansza,znak,1,6);
      a4 = pionek(plansza,znak,1,8);
      a5 = pionek(plansza,znak,2,1);
      a6 = pionek(plansza,znak,2,3);
      a7 = pionek(plansza,znak,2,5);
      a8 = pionek(plansza,znak,2,7);
    }
    else if (liczba == 2){
      znak = "o";
      a1 = pionek(plansza,znak,8,1);
      a2 = pionek(plansza,znak,8,3);
      a3 = pionek(plansza,znak,8,5);
      a4 = pionek(plansza,znak,8,7);
      a5 = pionek(plansza,znak,7,2);
      a6 = pionek(plansza,znak,7,4);
      a7 = pionek(plansza,znak,7,6);
      a8 = pionek(plansza,znak,7,8);
    }
  }
  ~gracz() {}
};

int main(){
  cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nWITAJ! Czas na grę w warcaby!" << endl;
  cout<<"Oto krótkie zasady.\n1. Wpisujesz zawsze wiersz i kolumnę pionka, o którego współrzędne zostaniesz poproszony." << endl;
  cout<<"2. Jeżeli wybierzesz pionek do ruchu musisz wykonać nim ruch. (nie można się wycofać!)" << endl;
  cout<<"\nGRAJMY!" << endl;
  cout<< endl;
  szachownica jeden;
        
  gracz bialy(jeden.tablica,1);
  gracz czerwony(jeden.tablica,2);
  jeden.disp();
  
  int czyzby = 0;
  int l = 0;
  while(czyzby == 0){
    l = bialy.ruch(jeden, czerwony);
    if (l == 0)
      return 0;
    jeden.disp();
    czyzby = bialy.zwyciestwo(jeden, czerwony); 
    if (czyzby == 0){
      l = czerwony.ruch(jeden, bialy);
      if(l == 0)
        return 0;
      jeden.disp();
      czyzby = czerwony.zwyciestwo(jeden, bialy);
    }
  }
  return 0;
}

string szachownica::znakZTablicy(int wiersz, int kolumna){
  int pole = (wiersz -1)*8 + kolumna -1;
  return tablica[pole];
}

void pionek::wypelnianieTablicyRuchow(szachownica obiekt, int wiersz, int kolumna){
  // funkcja generuje zapisuje do tablicy ruchy, na które może poruszyć się pionek.
  // uwzględnia bicie w każdym kierunku i poprawne ruchy pionów.
  for(int i=0; i<4; ++i)
      this-> tablicaRuchow[i] = 0; 
  int pole = 0;
  int i = 0;
  string znak = this-> znak;
  if(znak == "x"){
    if(wiersz-1 > 0 && wiersz-1 < 9 && kolumna-1 > 0 && kolumna-1 < 9){
      if(obiekt.znakZTablicy(wiersz-1, kolumna-1) != "" && obiekt.znakZTablicy(wiersz-1,kolumna-1) != "_" && obiekt.znakZTablicy(wiersz-1,kolumna-1) != this-> znak){
        pole = (wiersz -3)*8 + kolumna -3;
        if(wiersz-2 > 0 && wiersz-2 < 9 && kolumna-2 > 0 && kolumna-2 < 9 && obiekt.tablica[pole] == "_"){
          this-> tablicaRuchow[i] = pole;
          ++i;
        }
      }
    }
    
    if(wiersz-1 > 0 && wiersz-1 < 9 && kolumna+1 > 0 && kolumna+1 < 9){
      if(obiekt.znakZTablicy(wiersz-1, kolumna+1) != "" && obiekt.znakZTablicy(wiersz-1,kolumna+1) != "_" && obiekt.znakZTablicy(wiersz-1,kolumna+1) != this-> znak){
        pole = (wiersz -3)*8 + kolumna +1;
        if(wiersz-2 > 0 && wiersz-2 < 9 && kolumna+2 > 0 && kolumna+2 < 9 && obiekt.tablica[pole] == "_"){
          this-> tablicaRuchow[i] = pole;
          ++i;
        }
      }
    }
    
    if(wiersz+1 > 0 && wiersz+1 < 9 && kolumna-1 > 0 && kolumna-1 < 9){
      pole = wiersz*8 + kolumna-2;
      if(obiekt.tablica[pole] == "_"){
        this-> tablicaRuchow[i] = pole;
        ++i;        
      }
      else if(obiekt.znakZTablicy(wiersz+1, kolumna-1) != "" && obiekt.znakZTablicy(wiersz+1,kolumna-1) != "_" && obiekt.znakZTablicy(wiersz+1,kolumna-1) != this-> znak){
        pole = (wiersz +1)*8 + kolumna -3;
        if(wiersz+2 > 0 && wiersz+2 < 9 && kolumna-2 > 0 && kolumna-2 < 9 && obiekt.tablica[pole] == "_"){
          this-> tablicaRuchow[i] = pole;
          ++i;
        }
      }
    }
    
    if(wiersz+1 > 0 && wiersz+1 < 9 && kolumna+1 > 0 && kolumna+1 < 9){
      pole = wiersz*8 + kolumna;
      if(obiekt.tablica[pole] == "_"){
        this-> tablicaRuchow[i] = pole;
        ++i;
      }
      else if(obiekt.znakZTablicy(wiersz+1, kolumna+1) != "" && obiekt.znakZTablicy(wiersz+1,kolumna+1) != "_" && obiekt.znakZTablicy(wiersz+1,kolumna+1) != this-> znak){
        pole = (wiersz +1)*8 + kolumna +1;
        if(wiersz+2 > 0 && wiersz+2 < 9 && kolumna+2 > 0 && kolumna+2 < 9 && obiekt.tablica[pole] == "_"){
          this-> tablicaRuchow[i] = pole;
          ++i;
        }
      }
    }
  
  } else if (znak == "o"){
    
    if(wiersz+1 > 0 && wiersz+1 < 9 && kolumna-1 > 0 && kolumna-1 < 9){
      if(obiekt.znakZTablicy(wiersz+1, kolumna-1) != "" && obiekt.znakZTablicy(wiersz+1,kolumna-1) != "_" && obiekt.znakZTablicy(wiersz+1,kolumna-1) != this-> znak){
        pole = (wiersz +1)*8 + kolumna -3;
        if(wiersz+2 > 0 && wiersz+2 < 9 && kolumna-2 > 0 && kolumna-2 < 9 && obiekt.tablica[pole] == "_"){
          this-> tablicaRuchow[i] = pole;
          ++i;
        }
      }
    }
    
    if(wiersz+1 > 0 && wiersz+1 < 9 && kolumna+1 > 0 && kolumna+1 < 9){
      if(obiekt.znakZTablicy(wiersz+1, kolumna+1) != "" && obiekt.znakZTablicy(wiersz+1,kolumna+1) != "_" && obiekt.znakZTablicy(wiersz+1,kolumna+1) != this-> znak){
        pole = (wiersz +1)*8 + kolumna +1;
        if(wiersz+2 > 0 && wiersz+2 < 9 && kolumna+2 > 0 && kolumna+2 < 9 && obiekt.tablica[pole] == "_"){
          this-> tablicaRuchow[i] = pole;
          ++i;
        }
      }
    }
    
    if(wiersz-1 > 0 && wiersz-1 < 9 && kolumna-1 > 0 && kolumna-1 < 9){
      pole = (wiersz-2)*8 + kolumna-2;
      if(obiekt.tablica[pole] == "_"){
        this-> tablicaRuchow[i] = pole;
        ++i;        
      }
      else if(obiekt.znakZTablicy(wiersz-1, kolumna-1) != "" && obiekt.znakZTablicy(wiersz-1,kolumna-1) != "_" && obiekt.znakZTablicy(wiersz-1,kolumna-1) != this-> znak){
        pole = (wiersz -3)*8 + kolumna -3;
        if(wiersz-2 > 0 && wiersz-2 < 9 && kolumna-2 > 0 && kolumna-2 < 9 && obiekt.tablica[pole] == "_"){
          this-> tablicaRuchow[i] = pole;
          ++i;
        }
      }
    }
    
    if(wiersz-1 > 0 && wiersz-1 < 9 && kolumna+1 > 0 && kolumna+1 < 9){
      pole = (wiersz-2)*8 + kolumna;
      if(obiekt.tablica[pole] == "_"){
        this-> tablicaRuchow[i] = pole;
        ++i;
      }
      else if(obiekt.znakZTablicy(wiersz-1, kolumna+1) != "" && obiekt.znakZTablicy(wiersz-1,kolumna+1) != "_" && obiekt.znakZTablicy(wiersz-1,kolumna+1) != this-> znak){
        pole = (wiersz -3)*8 + kolumna +1;
        if(wiersz-2 > 0 && wiersz-2 < 9 && kolumna+2 > 0 && kolumna+2 < 9 && obiekt.tablica[pole] == "_"){
          this-> tablicaRuchow[i] = pole;
          ++i;
        }
      }
    }
  }
}

void szachownica::disp(){
  // funkcja wyświetla planszę.
  int wyswietl = 1;
  cout << "\033[1;33m 1  2  3  4  5  6  7  8\033[0m" << endl;
  for (int i = 0; i < 64; ++i)
  {
    if(i % 8 == 0 && i > 0){
      cout << "\033[1;33m " << wyswietl << "\033[0m" << endl;
      ++wyswietl;
    }
    if (tablica[i] == "")
      cout<< "\033[1;42m  " << tablica[i] << " \033[0m";
    else{
      if (tablica[i] == "x")
        cout<< "\033[1;37m " << tablica[i] << " \033[0m";
      else if(tablica[i] == "o")
        cout<< "\033[1;31m " << tablica[i] << " \033[0m";
      else
        cout<< "\033[0;37m " << tablica[i] << " \033[0m";
    }
  }
  cout << "\033[1;33m 8 \033[0m" << endl;
  cout<< endl;
}

pionek gracz::wyszukajPionek(int wiersz, int kolumna, bool f){
  // funkcja wyszukuje pionek z klasy gracz i zwraca go.
  pionek wsk;
  if (this-> a1.polozenie[0] == wiersz && this-> a1.polozenie[1] == kolumna){
    if(f)
      this-> a1.nadpiszMnie = true;
    wsk = this-> a1;
  }
  else if (this-> a2.polozenie[0] == wiersz && this-> a2.polozenie[1] == kolumna){
    if(f)
      this-> a2.nadpiszMnie = true;
    wsk = this-> a2;
  }
  else if (this-> a3.polozenie[0] == wiersz && this-> a3.polozenie[1] == kolumna){
    if(f)
      this-> a3.nadpiszMnie = true;
    wsk = this-> a3;
  }
  else if (this-> a4.polozenie[0] == wiersz && this-> a4.polozenie[1] == kolumna){
    if(f)
      this-> a4.nadpiszMnie = true;
    wsk = this-> a4;
  }
  else if (this-> a5.polozenie[0] == wiersz && this-> a5.polozenie[1] == kolumna){
    if(f)
      this-> a5.nadpiszMnie = true;
    wsk = this-> a5;
  }
  else if (this-> a6.polozenie[0] == wiersz && this-> a6.polozenie[1] == kolumna){
    if(f)
      this-> a6.nadpiszMnie = true;
    wsk = this-> a6;
  }
  else if (this-> a7.polozenie[0] == wiersz && this-> a7.polozenie[1] == kolumna){
    if(f)
      this-> a7.nadpiszMnie = true;
    wsk = this-> a7;
  }
  else if (this-> a8.polozenie[0] == wiersz && this-> a8.polozenie[1] == kolumna){
    if(f)
      this-> a8.nadpiszMnie = true;
    wsk = this-> a8;
  }
  return wsk;
}

int gracz::ruch(szachownica &obiekt, gracz &drugi){
  // główna fukcja odpowiadająca za grę.
  if (this-> znak == "x")
    cout<< "\nGracz BIAŁY(x) ma ruch." << endl;
  else
    cout<< "\nGracz CZERWONY(o) ma ruch." << endl;
  int wiersz = 0;
  int kolumna = 0;
  cout<< "Proszę wybrać podać współrzędne pionka, którym chcesz wykonać ruch." << endl;
  
  cout<< "wiersz: ";
  while(!(cin>> wiersz)){
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Podałeś literę. Podaj liczbę: ";
  }
  if(wiersz > 8 || wiersz < 1){
    cout<< "Podałeś liczbę, która nie oznacza wiersza. To błąd.\nWykonaj ruch ponownie!\n" << endl;
    int p = this-> ruch(obiekt, drugi);
    return p;
  }
  
  cout<< "kolumna: ";
  while(!(cin>> kolumna)){
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Podałeś literę. Podaj liczbę: ";
  }
  if(kolumna > 8 || kolumna < 1){
    cout<< "Podałeś liczbę, która nie oznacza kolumny. To błąd.\nWykonaj ruch ponownie!\n" << endl;
    int p = this-> ruch(obiekt, drugi);
    return p;
  }
  
  int pole = (wiersz -1)*8 + kolumna -1;
  string zTablicy = obiekt.tablica[pole];
  if(zTablicy != this-> znak){
    cout<< "\nmusisz wybrać inne pole - na tym polu nie masz pionka!\n" << endl;
    obiekt.disp();
    int p = this-> ruch(obiekt, drugi);
    return p;
  } else {
    pionek n = wyszukajPionek(wiersz,kolumna,true); // wyszukujemy jaką nazwę ma pionek przez nas wybrany.
    n.wypelnianieTablicyRuchow(obiekt, wiersz, kolumna); // wszystkie ruchy dla pionka zostaną zapisane w jego tablicyRuchów.
    if(n.tablicaRuchow[0] == 0){
      cout<< "Wybrano pionka, który nie może wykonać ruchu. wybierz inny pionek." << endl;
      int p = this-> ruch(obiekt, drugi);
      return p;
    }

    int l = this-> zmianaPola(obiekt, n, drugi); // wykonanie ruchu.
      return l;
  }
  return 1;
}

void gracz::nadpisz(pionek n){
  // fukcja zajmuje się znalezieniem pionka, który jest zmieniany oraz podmianą wartości.
  if (this-> a1.nadpiszMnie == true){
    this-> a1.znak = n.znak;
    this-> a1.JestemNaSzachownicy = n.JestemNaSzachownicy;
    this-> a1.nadpiszMnie = false;
    this-> a1.polozenie[0] = n.polozenie[0];
    this-> a1.polozenie[1] = n.polozenie[1];
    for(int i = 0; i<4; ++i)
      this-> a1.tablicaRuchow[i] = n.tablicaRuchow[i];
  }
  else if (this-> a2.nadpiszMnie == true){
    this-> a2.znak = n.znak;
    this-> a2.JestemNaSzachownicy = n.JestemNaSzachownicy;
    this-> a2.nadpiszMnie = false;
    this-> a2.polozenie[0] = n.polozenie[0];
    this-> a2.polozenie[1] = n.polozenie[1];
    for(int i = 0; i<4; ++i)
      this-> a2.tablicaRuchow[i] = n.tablicaRuchow[i];
  }
  else if (this-> a3.nadpiszMnie == true){
    this-> a3.znak = n.znak;
    this-> a3.JestemNaSzachownicy = n.JestemNaSzachownicy;
    this-> a3.nadpiszMnie = false;
    this-> a3.polozenie[0] = n.polozenie[0];
    this-> a3.polozenie[1] = n.polozenie[1];
    for(int i = 0; i<4; ++i)
      this-> a3.tablicaRuchow[i] = n.tablicaRuchow[i];
  }
  else if (this-> a4.nadpiszMnie == true){
    this-> a4.znak = n.znak;
    this-> a4.JestemNaSzachownicy = n.JestemNaSzachownicy;
    this-> a4.nadpiszMnie = false;
    this-> a4.polozenie[0] = n.polozenie[0];
    this-> a4.polozenie[1] = n.polozenie[1];
    for(int i = 0; i<4; ++i)
      this-> a4.tablicaRuchow[i] = n.tablicaRuchow[i];
  }
  else if (this-> a5.nadpiszMnie == true){
    this-> a5.znak = n.znak;
    this-> a5.JestemNaSzachownicy = n.JestemNaSzachownicy;
    this-> a5.nadpiszMnie = false;
    this-> a5.polozenie[0] = n.polozenie[0];
    this-> a5.polozenie[1] = n.polozenie[1];
    for(int i = 0; i<4; ++i)
      this-> a5.tablicaRuchow[i] = n.tablicaRuchow[i];
  }
  else if (this-> a6.nadpiszMnie == true){
    this-> a6.znak = n.znak;
    this-> a6.JestemNaSzachownicy = n.JestemNaSzachownicy;
    this-> a6.nadpiszMnie = false;
    this-> a6.polozenie[0] = n.polozenie[0];
    this-> a6.polozenie[1] = n.polozenie[1];
    for(int i = 0; i<4; ++i)
      this-> a6.tablicaRuchow[i] = n.tablicaRuchow[i];
  }
  else if (this-> a7.nadpiszMnie == true){
    this-> a7.znak = n.znak;
    this-> a7.JestemNaSzachownicy = n.JestemNaSzachownicy;
    this-> a7.nadpiszMnie = false;
    this-> a7.polozenie[0] = n.polozenie[0];
    this-> a7.polozenie[1] = n.polozenie[1];
    for(int i = 0; i<4; ++i)
      this-> a7.tablicaRuchow[i] = n.tablicaRuchow[i];
  }
  else if (this-> a8.nadpiszMnie == true){
    this-> a8.znak = n.znak;
    this-> a8.JestemNaSzachownicy = n.JestemNaSzachownicy;
    this-> a8.nadpiszMnie = false;
    this-> a8.polozenie[0] = n.polozenie[0];
    this-> a8.polozenie[1] = n.polozenie[1];
    for(int i = 0; i<4; ++i)
      this-> a8.tablicaRuchow[i] = n.tablicaRuchow[i];
  }
}

int gracz::zmianaPola(szachownica &obiekt, pionek &n, gracz &drugi){
  // funkcja zajmuje się wykonaniem fizycznej zmiany położenia pionka "n" na szachownicy.
  // sprawdza czy występuje bicie, jeżeli tak, to wywołuje funkcję bicie z klasy gracza przeciwnego. 
  // dane zbijanego pionka należą do klasy przeciwnika.
  cout<< "Podaj pole na którym ma się znaleźć pionek." << endl;
  int wiersz = 0;
  int kolumna = 0;
  cout<< "wiersz: ";
  while(!(cin>> wiersz)){
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Podałeś literę. Podaj liczbę: ";
  }
  if(wiersz > 8 || wiersz < 1){
    cout<< "Podałeś liczbę, która nie oznacza wiersza. To błąd.\nWykonaj ruch ponownie!\n" << endl;
    int p = this-> ruch(obiekt, drugi);
    return p;
  }
  
  cout<< "kolumna: ";
  while(!(cin>> kolumna)){
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Podałeś literę. Podaj liczbę: ";
  }
  if(kolumna > 8 || kolumna < 1){
    cout<< "Podałeś liczbę, która nie oznacza kolumny. To błąd.\nWykonaj ruch ponownie!\n" << endl;
    int p = this-> ruch(obiekt, drugi);
    return p;
  }
  
  int pole = (wiersz-1)*8 + kolumna-1;
  bool k = false;
  for(int i=0; i < 4; ++i){
    if(pole == n.tablicaRuchow[i] && pole != 0){
      k = true;
      break;
    }
  }
  if(k == true){
    bool jeszcze_raz = drugi.bicie(n, wiersz, kolumna, obiekt);
    pole = (n.polozenie[0]-1)*8 + n.polozenie[1]-1;
    obiekt.tablica[pole] = "_";
    n.polozenie[0] = wiersz;
    n.polozenie[1] = kolumna;
    n.wypelnianieTablicyRuchow(obiekt, wiersz, kolumna);
    
    this-> nadpisz(n);

    pole = (n.polozenie[0]-1)*8 + n.polozenie[1]-1;
    obiekt.tablica[pole] = n.znak;
    bool napewno = false;
    for(int i = 0; i < 4; ++i){
      if(n.tablicaRuchow[i] == 0)
        break;
      if(abs(n.tablicaRuchow[i] - pole) > 13){
        napewno = true;
        break;
      }
    }
    if(jeszcze_raz == true && napewno == true){
      obiekt.disp();
      cout<<"\nMasz do wykonania kolejny ruch." << endl;
      this-> ruch(obiekt, drugi);
    }
  }
  else{
    int l = 0;
    cout<< "\nZłe współrzędne pola." << endl;
    cout<< "jeżeli, chcesz grać dalej wpisz 0 i wciśnij enter." << endl;
    cout<< "jeżeli wrócić do wyboru pionka wpisz 1 i wciśnij enter." << endl;
    cout<< "jeżeli chcesz wyjść z gry wpisz 2 i wciśnij enter." << endl;
    cin>>l;
    switch(l){
      case 0: {
        cout<<"Wybrałeś grę dalej. Wpisz współrzędne ponownie z większą uwagą." << endl;
        l = this-> zmianaPola(obiekt, n, drugi);
        return l;
      }break;
      
      case 1: {
        cout<<"Wybrałeś powrót do wyboru pionka." << endl;
        l = this-> ruch(obiekt,drugi);
        return l;
      }break;
      
      case 2: {
        cout<<"Wybrałeś zakończenie gry. Żegnaj!" << endl;
        return 0;
      }break;
      
      default: {
        cout<<"podano niepoprawne polecenie. kończę program." << endl;
        return 0;
      }break;
    }
  }
  return 1;
}

void gracz::zbijam(int wiersz, int kolumna){
  // fukcja odpowiada za wykonanie samej operacji zbijania.
  // wyszukuje pionek, który ma zostać zbity.
  // fukcja nadpisz - zmienia parametry pionka w klasie gracz, usuwając go z planszy.
  pionek n = this-> wyszukajPionek(wiersz, kolumna, true);
  n.JestemNaSzachownicy = false;
  n.znak = "_";
  for(int i=0; i<4; ++i)
      n.tablicaRuchow[i] = 0;
  n.polozenie[0] = wiersz;
  n.polozenie[1] = kolumna;
  this-> nadpisz(n);
}

bool gracz::bicie(pionek n, int wiersz, int kolumna, szachownica &obiekt){
  // funkcja stwierdza czy występuje bicie. jeżeli tak - zwraca true.
  if(abs(n.polozenie[0] - wiersz) == 2 && abs(n.polozenie[1] - kolumna) == 2){ // do wykonania mamy skreślenie 1 pola. - bicie piona
    int wiersz_nowy = abs(n.polozenie[0] + wiersz)/2;
    int kolumna_nowa = abs(n.polozenie[1] + kolumna)/2;
    int pole_bicia = (wiersz_nowy-1)*8 + kolumna_nowa-1;
    int pole = (wiersz-1)*8 + kolumna-1;
    bool dalej = false;
    for (int i = 0; i < 4; ++i){
      if (n.tablicaRuchow[i] == pole){
        dalej = true;
        break;
      }
    }
    if(dalej){
      obiekt.tablica[pole_bicia] = "_";
      this-> zbijam(wiersz_nowy, kolumna_nowa);
      return true;
    }
  }  
  return false;
}

int gracz::zwyciestwo(szachownica obiekt, gracz przeciwnik){
  // sprawdza rezultat gry.
  // zwraca 1 - zwyciestwo
  // zwraca 0 - gra nie roztrzygnięta.

  // wygrana:
  // 1. kiedy nie ma pionków przeciwnika na planszy.
  // 2. przeciwnik nie może wykonać ruchu.

  // remis:
  // żaden pionek nie może się ruszyć.
  
  bool przeciwnik_sie_ruszy = false;
  bool jest_pionek_wroga = false;
  bool ja_sie_rusze = false;

  for(int i=0; i < 64; ++i){
    int wiersz = 1;
    if (i > 7)
      wiersz = (i/8) +1;
    int kolumna = i+1 - (wiersz-1)*8;

    if(obiekt.tablica[i] == przeciwnik.znak){
      pionek n = przeciwnik.wyszukajPionek(wiersz, kolumna, false);
      if (n.JestemNaSzachownicy == true){
        jest_pionek_wroga = true;
        n.wypelnianieTablicyRuchow(obiekt,wiersz,kolumna);
        if(n.tablicaRuchow[0] != 0)
          przeciwnik_sie_ruszy = true;
      }
    }
    else if(obiekt.tablica[i] == this->znak){
      pionek n = this-> wyszukajPionek(wiersz, kolumna, false);
      if(n.JestemNaSzachownicy == true){
        n.wypelnianieTablicyRuchow(obiekt,wiersz,kolumna);
        if(n.tablicaRuchow[0] != 0)
          ja_sie_rusze = true;
      }
    }
  }

  if (przeciwnik_sie_ruszy == false || jest_pionek_wroga == false){
    if (this-> znak == "x")
      cout<< "\nWygrał gracz BIAŁY!" << endl;
    else
      cout<< "\nWygrał gracz CZERWONY!" << endl;
    return 1;
  }
  if (przeciwnik_sie_ruszy == false || ja_sie_rusze == false){
    cout<< "\nREMIS!" << endl;
    return 2;
  }
  return 0;
}
