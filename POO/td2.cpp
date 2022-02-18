/**
* Programme qui crée des listes de film avec ses acteurs et sa liste de film dans laquelle il joue.
* \file   td2.cpp
* \auteurs Sebastian Espin, Julien Roux
* \date   11 janvier 2022
* Créé le 1 janvier 2022
*/


#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS 

#include "structures.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp"
using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{

UInt8 lireUint8(istream& fichier)
{
	UInt8 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
UInt16 lireUint16(istream& fichier)
{
	UInt16 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUint16(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion//}

void ListeFilms::ajoutFilmDansListe(Film* film)
{

	Film** nouvellelistFilms;
	if (capacite_ == 0)
	{
		nouvellelistFilms = new Film* [1];
		delete[]elements_;
		elements_ = nouvellelistFilms;
		capacite_ = 1;
	}
	else if (capacite_ <= nElements_)
	{
		nouvellelistFilms = new Film* [capacite_ * 2];
		for (int i : range(nElements_))
		{
			nouvellelistFilms[i] = elements_[i];
		}
		capacite_ = capacite_ * 2;
		delete[]elements_;
		elements_ = nouvellelistFilms;
	}
	elements_[nElements_] = film;
	nElements_++;
}

void ListeFilms::enleverFilm(Film* film)
{
	Film** nouvellelistFilms = new Film * [capacite_];
	bool estRetirer = false;
	for (int i : range(nElements_))
	{
		if (elements_[i] == film)
		{
			nElements_--;
			estRetirer = true;
		}
		else
		{
			estRetirer == false ? nouvellelistFilms[i] = elements_[i] : nouvellelistFilms[i - 1] = elements_[i];
		}
	}
	

	delete[]elements_;
	elements_ = nouvellelistFilms;
}

Acteur* ListeFilms::trouverActeur(string nomActeur)
{
	if (elements_ == nullptr)
	{
		return nullptr;
	}
	for (Film*& film : span(elements_, nElements_)) 
	{
		for (Acteur*& acteur : span(film->acteurs.elements, film->acteurs.nElements)) {

			if (acteur->nom == nomActeur)
			{
				return acteur;
			}
		}
	}
	return nullptr;
}


Acteur* lireActeur(istream& fichier, ListeFilms& listFilms)
{
	Acteur acteur = {}; //La création de la structure Acteur sans argument crée un avertissement 
						//mais comme on les attribues juste après on a choisit de ne pas les prendres en compte.
	acteur.nom = lireString(fichier);
	acteur.anneeNaissance = lireUint16(fichier);
	acteur.sexe = lireUint8(fichier);

	Acteur* acteurTrouve = listFilms.trouverActeur(acteur.nom);
	if (acteurTrouve == nullptr)
	{
		acteur.joueDans.ajoutList(new Film * [1]);
		
		acteurTrouve = new Acteur;
		*acteurTrouve = acteur;

		return acteurTrouve;
	}
	else
	{
		return acteurTrouve;
	}
}

Film* lireFilm(istream& fichier, ListeFilms& listFilms)
{
	Film* film = new Film;   //La création de la structure Film sans argument crée un avertissement 
							 //mais comme on les attribues juste après on a choisit de ne pas les prendres en compte.
	film->titre = lireString(fichier);
	film->realisateur = lireString(fichier);
	film->anneeSortie = lireUint16(fichier);
	film->recette = lireUint16(fichier);
	film->acteurs.nElements = lireUint8(fichier);  
	film->acteurs.elements = new Acteur * [film->acteurs.nElements];
	film->acteurs.capacite = film->acteurs.nElements;

	for (int i = 0; i < film->acteurs.nElements; i++) {
		Acteur* acteur = lireActeur(fichier, listFilms);
		film->acteurs.elements[i] = acteur; 
		
		acteur->joueDans.ajoutFilmDansListe(film); 
	}

	return film; 
}

ListeFilms creerListe(string nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);

	int nElements = lireUint16(fichier);

	ListeFilms listFilms = {};
	listFilms.ajoutCapacite(0);
	listFilms.ajout_nElement(0);

	for (int i = 0; i < nElements; i++) {
		listFilms.ajoutFilmDansListe(lireFilm(fichier, listFilms));
	}

	return listFilms;
}

void ListeFilms::detruireFilm(int indexFilm)
{
	Film* film = elements_[indexFilm];
	enleverFilm(film);
	for (Acteur* acteur : span(film->acteurs.elements, film->acteurs.nElements))
	{
		acteur->joueDans--;
		if (acteur->joueDans.lire_nElement() == 0)
		{
			delete[] acteur->joueDans.lireElements();
			delete acteur;
		}
	}
	delete[]film->acteurs.elements;
	delete film;
}


void ListeFilms::detruireListFilms()
{
	for (int j : range(nElements_))
	{
		detruireFilm(0);
	}
	delete[]elements_;
}

void afficherActeur(const Acteur& acteur)
{
	cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

void afficherFilm(const Film* film) 
{
	cout << "Le film est : " << film->titre << endl;
	cout << "le film est réaliser par : " << film->realisateur << endl;
	cout << "Le film est sortie en : " << film->anneeSortie << endl;
	cout << "La recette du film est de : " << film->recette << " $" << endl;
	cout << "la liste des acteurs jouant dedans est : " << endl;
	for (Acteur* acteur : span(film->acteurs.elements, film->acteurs.nElements))
	{
		afficherActeur(*acteur);
	}
}

void ListeFilms::afficherListeFilms() const
{
	static const string ligneDeSeparation2 = "\n\033[31m>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\033[0m\n";
	cout << ligneDeSeparation2;
	for (Film* film : span(elements_, nElements_)) {
		afficherFilm(film);
		cout << ligneDeSeparation2;
	}
}

void afficherFilmographieActeur(ListeFilms& listeFilms, const string& nomActeur)
{
	const Acteur* acteur = listeFilms.trouverActeur(nomActeur);
	if (acteur == nullptr)
		cout << "Aucun acteur de ce nom" << endl;
	else
		acteur->joueDans.afficherListeFilms();
}

int main()
{
	bibliotheque_cours::activerCouleursAnsi();
	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	ListeFilms listeFilms = creerListe("films.bin");

	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	afficherFilm(listeFilms.lireElements()[0]);

	cout << ligneDeSeparation << "Les films sont:" << endl;
	listeFilms.afficherListeFilms();

	Acteur* ptrTemp = listeFilms.trouverActeur("Benedict Cumberbatch");
	ptrTemp->anneeNaissance = 1976;
	
	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	ptrTemp->joueDans.afficherListeFilms();
	
	listeFilms.detruireFilm(0);

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	listeFilms.afficherListeFilms();

	cout << ligneDeSeparation << "Liste des films où Daniel Craig joue sont:" << endl;
	afficherFilmographieActeur(listeFilms, "Daniel Craig");

	cout << ligneDeSeparation << "Liste des films où Vin Diesel joue sont:" << endl;
	afficherFilmographieActeur(listeFilms, "Vin Diesel");

	listeFilms.detruireListFilms();
}