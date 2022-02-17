#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include <memory>
#include "gsl/span"
using gsl::span;
using namespace std;

class Film; class Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {
public:
	ListeFilms() = default;
	ListeFilms(const std::string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements_ == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(shared_ptr<Film> film);
	void enleverFilm(const shared_ptr<Film> film);
	shared_ptr<Acteur> trouverActeur(const std::string& nomActeur) const;
	span<shared_ptr<Film>> enSpan() const;
	int size() const { return nElements_; }

private:
	void changeDimension(int nouvelleCapacite);

	int capacite_ = 0, nElements_ = 0;
	unique_ptr<shared_ptr<Film>[]> elements_ = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};

class ListeActeurs {
public:
	ListeActeurs() = default;
	ListeActeurs(int nElements) : nElements_(nElements) , elements_(make_unique<shared_ptr<Acteur>[]>(nElements)), capacite_(nElements) {};
	int lire_nElements() const { return nElements_; };
	span<shared_ptr<Acteur>> spanListeActeurs() const;

private:
	int capacite_, nElements_;
	unique_ptr<shared_ptr<Acteur>[]> elements_; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};


class Film
{
public:
	Film() = default;
	Film(string titre, string realisateur, int anneeSortie, int recette) : titre_(titre), realisateur_(realisateur), anneeSortie_(anneeSortie), recette_(recette) {};
	Film(ListeActeurs acteurs) : acteurs_(move(acteurs)) {};

	std::string lireTitre() { return titre_; }

	//std::string lire() { return acteurs_; }

	ListeActeurs lireActeur() { return acteurs_; }
	ListeActeurs lireActeur() const { return acteurs_; }

private:
	std::string titre_, realisateur_; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie_, recette_; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs_;
};

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	//ListeFilms joueDans;
};
//std::unique_ptr<int> monPtr = std::make_unique<int>(4);