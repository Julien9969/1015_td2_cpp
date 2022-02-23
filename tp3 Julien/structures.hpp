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
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const std::string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }



	//function<bool(Film)>
	template<typename T>
	int trouverFilmCorrespondant(const T& critere);
	


	Film& operator[] (int index){
		
		return *this->elements[index];
	}
	

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};

class ListeActeurs {
public:
	ListeActeurs() = default;
	/*ListeActeurs(int nElements) {
		nElements_ = nElements;
		elements_ = make_unique<shared_ptr<Acteur>[]>(nElements);
		capacite_ = nElements;
	};*/

	ListeActeurs(int nElements) : nElements_(nElements), elements_(make_unique<shared_ptr<Acteur>[]>(nElements)), capacite_(nElements) {};
	int lire_nElements() const { return nElements_; };
	span<shared_ptr<Acteur>> spanListeActeurs() const;

	shared_ptr<Acteur>& operator[] (int index) {
		return this->elements_[index];
	}

	//ListeActeurs(const ListeActeurs& autre) : elements_(autre.elements_.get()), capacite_(autre.capacite_), nElements_(autre.nElements_) {};
	ListeActeurs(const ListeActeurs& autre) {
		//ListeActeurs liste = ListeActeurs(autre.nElements_);
		capacite_ = autre.capacite_;
		nElements_ = autre.nElements_;
		//elements_.~unique_ptr();
		elements_ = make_unique<shared_ptr<Acteur>[]>(nElements_);
		for (int i = 0; i < nElements_; ++i) {
			elements_[i] = autre.elements_[i];
		}
	};
/*
	ListeActeurs& operator= (const ListeActeurs& autre) {
		if (this != &autre) {
			capacite_ = autre.capacite_;
			nElements_ = autre.nElements_;
			//elements_.~unique_ptr();
			elements_.release();
			unique_ptr<shared_ptr<Acteur>[]> elements_ = make_unique<shared_ptr<Acteur>[]>(nElements_);
			for (int i = 0; i < nElements_; ++i) {
				elements_[i] = autre.elements_[i];
			}



		}
		return *this;

	};*/

	
	ListeActeurs& operator= (ListeActeurs&&) = default;
	

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
	Film() = default;

	//Film(string titreb, string realisateurb, int annee, int recetteb, ListeActeurs acteursb) : titre(titreb), realisateur(realisateurb), anneeSortie(annee), recette(recetteb), acteurs(acteursb) {};
	//Film(const Film& autre) : titre(autre.titre), realisateur(autre.realisateur), anneeSortie(autre.anneeSortie), recette(autre.recette), acteurs(autre.acteurs) {};
	/*Film(const Film& autre) {
		//Film* film = new Film;
		//Film film = film;
		//Film* film = new Film;
		titre = autre.titre;
		realisateur = autre.realisateur;
		recette = autre.recette;
		anneeSortie = autre.recette;
		//acteurs = ListeActeurs(autre.acteurs.lire_nElements());
		acteurs = autre.acteurs;

		//film->acteurs.
		//film->

		//for (int i = 0; i < autre.acteurs.lire_nElements(); ++i) {}

	}*/



	//~Film() = default;
	/*
	Film& operator= (Film& autre) noexcept {
		
		titre = autre.titre;
		realisateur = autre.realisateur;
		anneeSortie = autre.anneeSortie;
		recette = autre.recette;
		acteurs = autre.acteurs;
		return *this;
	}
	*/


};

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	//ListeFilms joueDans;
};
