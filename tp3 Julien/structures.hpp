/*
* Programme qui garde le même fonctionnement du programme du TD2 mais qui change les pointeurs brutes lié a listeActeurs
* en pointeurs intelligents. Le programme surcharge aussi << pour afficher directement un Film et possède une nouvelle fonction pour
* trouver un FIlm via un argument donné dans un lambda. Enfin on a transformé ListeActeurs en Liste Template pour crée des liste de string par exemple.
*
* Fichier : td3.cpp, structures.h//
* Auteurs : Sebastian Espin, Julien Roux
* Date : 25/02/2022
* Crée : 15/02/2022
*/


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
	ListeFilms(const ListeFilms& l) { assert(l.elements == nullptr); }
	~ListeFilms();
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const std::string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }

	template<typename U>
	int trouverFilmCorrespondant(const U& critere);
	

	Film& operator[] (int index){
		return *this->elements[index];
	}

private:
	void changeDimension (int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};

template<typename T>
class Liste 
{
public:
	Liste () = default;
	Liste (int nElements) : nElements_(nElements), elements_(make_unique<shared_ptr<T>[]>(nElements)), capacite_(nElements) {};
	int lire_nElements () const { return nElements_; };
	span<shared_ptr<T>> spanListeActeurs () const;
	

	shared_ptr<T>& operator[] (int index) {
		return this->elements_[index];
	}

	Liste (const Liste& autre) {
		capacite_ = autre.capacite_;
		nElements_ = autre.nElements_;
		elements_ = make_unique<shared_ptr<T>[]>(nElements_);

		for (int i = 0; i < nElements_; ++i) 
		{
			elements_[i] = autre.elements_[i];
		}
	};
	
	Liste& operator= (Liste&&) = default;

private:
	int capacite_ = 0, nElements_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_;
};

using ListeActeurs = Liste<Acteur>;

struct Film
{
	friend ostream& operator<< (ostream& o, const Film& f);
	std::string titre, realisateur; 
	int anneeSortie = 0, recette = 0;
	ListeActeurs acteurs;
};

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	//ListeFilms joueDans;
};
