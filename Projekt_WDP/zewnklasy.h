
using namespace std;

class Gracz {
public:
	float x; //wspolrzedne gracza
	float y;
	float vx; //predkosc gracza w poziomie
	float vy; //predkosc gracza w pionie
	int pkt; //zdobyte punkty
	int highscore;
	int zycia; //ilosc zyc

	void operator()(int maxX, int maxY, int playerSize) {
		if (x < playerSize - playerSize)
			x = playerSize - playerSize;
		if (y < playerSize - playerSize)
			y = playerSize - playerSize;
		if (x > maxX - playerSize)
			x = maxX - playerSize;
		if (y > maxY - playerSize)
			y = maxY - playerSize;
	}

	Gracz(float startX, float startY, float startVX, float startVY, int startPkt, int startZycia, int startHighscore)
		: x(startX), y(startY), vx(startVX), vy(startVY), pkt(startPkt), zycia(startZycia), highscore(startHighscore)
	{}

	~Gracz() {} //pusty destruktor
};


class Przeciwnik { //wspolrzedne i predkosc przeciwnika
public:
	int dx;
	int dy;
	int vdx;

	Przeciwnik(int startDX, int startDY, int startVDX)
		: dx(startDX), dy(startDY), vdx(startVDX)
	{}

	Przeciwnik(const Przeciwnik& other)
		: dx(other.dx), dy(other.dy), vdx(other.vdx)
	{}

	~Przeciwnik() {} //pusty destruktor
};

class GraObiekty {	// klasa abstrakcyjna, która nie jest u¿ywana do tworzenia obiektów, tylko slu¿y jako podstawa do dziedziczenia etc.
private:
	virtual bool zderzenie(int x, int y) = 0;	// "Czysto wirtualna" metoda

	virtual bool egzystencja(bool ist) {	// Wirtualna metoda uzywana do zastosowanaia polimofrizmu
		if (ist)
			ist = false;
		else
			cout << "Istnieje" << endl;
		return ist;
	}
};

class Tarcza : public GraObiekty {

	friend class GraObiekty; // deklaracja przyjaciela

public:
	int dx = 1800;
	int dy;
	int spawn; //liczba do losowania czasu spawnowania tarczy
	bool ruch = false; //czy tarcza ma sie poruszac w danym momencie
	bool aktywna = false; //czy tarcza jest aktywna
	int czas_start = 0; //ilosc klatek w momencie zebrania tarczy

	bool zderzenie(int x, int y) override { // implementacja czysto wirtualnej metody z klasy bazowej
		if ((x + 140 > dx - 60 && x + 140 < dx + 120 + 60) && (y + 140 > dy - 60 && y + 140 < dy + 180 + 60))
			return true;
		else
			return false;
	}

	bool egzystencja(bool ist) override { // implementacja metody w klasie pochodnej
		if (ist)
			cout << ": Obiekt Tarczy zostal w przestrzeni gry" << endl;
		else
			cout << ": Obiekt Tarczy zostal usuniety z przestrzeni gry" << endl;
		return ist;
	}
};

class Piwo : public GraObiekty {

	friend class GraObiekty; // deklaracja przyjaciela

public:
	int dx = 1800;
	int dy;
	int spawn; //liczba do losowania czasu spawnowania piwa
	bool aktywna = false;
	bool ruch = false; //czy piwo ma sie poruszac w danym momencie

	bool zderzenie(int x, int y) override { // implementacja czysto wirtualnej metody z klasy bazowej + nadpisywanie wirtualnej metody, wiêc zastosowanie polimorfizmu 
		if ((x + 140 > dx - 60 && x + 140 < dx + 120 + 60) && (y + 140 > dy - 60 && y + 140 < dy + 180 + 60))
			return true;
		else
			return false;
	}

	bool egzystencja(bool ist) override { // implementacja metody w klasie pochodnej
		if (ist)
			cout << ": Obiekt Piwa zostal w przestrzeni gry" << endl;
		else
			cout << ": Obiekt Piwa zostal usuniety z przestrzeni gry" << endl;
		return ist;
	}
};