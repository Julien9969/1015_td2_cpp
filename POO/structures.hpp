#pragma once
// Structures mémoires pour une collection de films.

#include <string>

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {

public:
	ListeFilms() = default;

	ListeFilms(int capacite, int nElements, Film** elements) : capacite_(capacite), nElements_(nElements), elements_(elements) {}

	void ajoutCapacite (int capacite) {
		capacite_ = capacite;
	}
	void ajout_nElement (int nElements) {
		nElements_ = nElements;
	}
	void ajoutList(Film** elements) {
		elements_ = elements;
	}

	const int lireCapacite() const{
		return capacite_;
	}
	const int lire_nElement() const{
		return nElements_;
	}
	Film** lireElements() const{
		return elements_;
	}
	
	void operator--(int) {
		nElements_--;
	}

	void ajoutFilmDansListe(Film* film);

	void enleverFilm(Film* film);

	void detruireListFilm();

	void detruireFilm(int indexFilm);

	Acteur* trouverActeur(std::string nomActeur);

	void afficherListeFilms() const;

private:
	int capacite_, nElements_;
	Film** elements_; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
};

struct ListeActeurs {
	int capacite, nElements;
	Acteur** elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};

struct Film
{
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};
