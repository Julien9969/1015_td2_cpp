#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include "gsl/span"
#include <memory>
using gsl::span;
using namespace std;
struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {
public:
	ListeFilms() = default;
	ListeFilms(const std::string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(shared_ptr<Film> film);
	void enleverFilm(const shared_ptr<Film> film);
	shared_ptr<Acteur> trouverActeur(const std::string& nomActeur) const;
	span<shared_ptr<Film>> enSpan() const;
	int size() const { return nElements; }

	shared_ptr<Film>& operator[] (int index){
		return this->elements[index];
	}
	

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	unique_ptr<shared_ptr<Film>[]> elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};

class ListeActeurs {
public:
	ListeActeurs() = default;
	ListeActeurs(int nElements) : nElements_(nElements), elements_(make_unique<shared_ptr<Acteur>[]>(nElements)), capacite_(nElements) {};
	int lire_nElements() const { return nElements_; };
	span<shared_ptr<Acteur>> spanListeActeurs() const;

	shared_ptr<Acteur>& operator[] (int index) {
		return this->elements_[index];
	}

	


	/*
	ListeActeurs& operator= (ListeActeurs& autre) noexcept {
		capacite_ = autre.capacite_;
		nElements_ = autre.nElements_;
		for (int i : range(nElements_))
		elements_[i] = autre.elements_[i];
		return *this;
	}
	*/
private:
	int capacite_ = 0, nElements_ = 0;
	unique_ptr<shared_ptr<Acteur>[]> elements_; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};

struct Film
{
	friend ostream& operator<< (ostream& o, const Film& f);
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie = 0, recette = 0; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;

	//Film(const Film& autre) : titre(autre.titre), realisateur(autre.realisateur), anneeSortie(autre.anneeSortie), recette(autre.recette), acteurs(autre.acteurs) {};

	//Film& operator= (Film& autre)  noexcept = default;
		
		/*titre = autre.titre;
		realisateur = autre.realisateur;
		anneeSortie = autre.anneeSortie;
		recette = autre.recette;
		acteurs = autre.acteurs;
		return *this;*/



};

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	//ListeFilms joueDans;
};
