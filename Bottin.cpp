//
// Created by Vincent on 11/28/2024.
//

#include "Bottin.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include "ContratException.h"

namespace TP3 {

    /**
     * \brief Constructeur de la classe Bottin à partir d'un fichier d'entrée.
     * \param[in] p_fichierEntree Le flux de fichier à lire pour extraire les données.
     * \param[in] p_table_size La taille des tables de hachage utilisées pour stocker les entrées.
     * \pre Le fichier doit être bien formé et respecter un format d'entrée spécifique.
     */
    Bottin::Bottin(std::ifstream& p_fichierEntree, size_t p_table_size)
    : m_tableauDesEntrees(), m_tableParNomPrenom(p_table_size), m_tableParTelephone(p_table_size), m_nombreEntrees(0) {
        std::regex line_format(R"(^[A-Za-z\s\.\-]+,\s[A-Za-z\s\.\-\(\)]+(?:\s\([A-Za-z\s\.\-]*\))?\s?\(?\d{3}\)?\s*\d{3}-\d{4}\s+(\(?\d{3}\)?\s*\d{3}-\d{4})\s+[\w\.\-]+@[\w\.\-]+\.[a-z]{2,},?$)");
        std::string line;
        std::getline(p_fichierEntree, line);

        while (std::getline(p_fichierEntree, line)) {
            PRECONDITION(std::regex_match(line, line_format))

            std::istringstream iss(line);
            std::string nomPrenom, telFixe, telCell, courriel;

            std::getline(iss, nomPrenom, '\t');
            std::getline(iss, telFixe, '\t');
            std::getline(iss, telCell, '\t');
            std::getline(iss, courriel, '\t');

            size_t pos = nomPrenom.find(", ");
            std::string nom = nomPrenom.substr(0, pos);
            std::string prenom = nomPrenom.substr(pos + 2);

            telFixe.erase(std::remove_if(telFixe.begin(), telFixe.end(), [](char c) {
                return c == '(' || c == ')' || c == '-' || c == ' ';
            }), telFixe.end());

            telCell.erase(std::remove_if(telCell.begin(), telCell.end(), [](char c) {
                return c == '(' || c == ')' || c == '-' || c == ' ';
            }), telCell.end());

            ajouter(nom, prenom, telFixe, telCell, courriel);
        }
    }

    /**
     * \brief Ajoute une nouvelle entrée au Bottin.
     * \param[in] p_nom Le nom de la personne.
     * \param[in] p_prenom Le prénom de la personne.
     * \param[in] p_telephoneFixe Le numéro de téléphone fixe de la personne.
     * \param[in] p_cellulaire Le numéro de téléphone cellulaire de la personne.
     * \param[in] p_courriel L'adresse courriel de la personne.
     * \pre Les noms et prénoms ne doivent pas être vides.
     * \pre Les numéros de téléphone fixe et cellulaire doivent être valides et ne pas être vides.
     * \pre L'adresse courriel ne doit pas être vide.
     * \pre Les numéros de téléphone ne doivent pas être déjà présents dans le Bottin.
     * \post Le nombre d'entrées dans le Bottin doit augmenter de 1.
     */
    void Bottin::ajouter(const std::string& p_nom, const std::string& p_prenom,
                     const std::string& p_telephoneFixe, const std::string& p_cellulaire,
                     const std::string& p_courriel) {
        PRECONDITION(!p_nom.empty() && !p_prenom.empty());
        PRECONDITION(!p_telephoneFixe.empty() && !p_cellulaire.empty());
        PRECONDITION(!p_courriel.empty());
        PRECONDITION(p_telephoneFixe.size() == 10 && std::all_of(p_telephoneFixe.begin(), p_telephoneFixe.end(), ::isdigit));
        PRECONDITION(p_cellulaire.size() == 10 && std::all_of(p_cellulaire.begin(), p_cellulaire.end(), ::isdigit));
        PRECONDITION(!m_tableParNomPrenom.contient(p_nom + p_prenom));
        PRECONDITION(!m_tableParTelephone.contient(p_telephoneFixe));
        m_tableauDesEntrees.emplace_back(p_nom, p_prenom, p_telephoneFixe, p_cellulaire, p_courriel);

        size_t index = m_tableauDesEntrees.size() - 1;

        std::string nomPrenomKey = p_nom + p_prenom;

        if (!m_tableParNomPrenom.contient(p_nom + p_prenom)) {
            m_tableParNomPrenom.inserer(p_nom + p_prenom, index);
        }

        if (!m_tableParTelephone.contient(p_telephoneFixe)) {
            m_tableParTelephone.inserer(p_telephoneFixe, index);
        }

        ++m_nombreEntrees;
        POSTCONDITION(m_tableParNomPrenom.contient(nomPrenomKey));
        POSTCONDITION(m_tableParTelephone.contient(p_telephoneFixe));
    }

    /**
     * \brief Recherche une entrée dans le Bottin à partir du nom et du prénom.
     * \param[in] p_nom Le nom de la personne à rechercher.
     * \param[in] p_prenom Le prénom de la personne à rechercher.
     * \return L'entrée correspondant au nom et prénom spécifiés.
     * \pre Le nom et le prénom ne doivent pas être vides.
     * \throws ContratException Si l'entrée n'est pas trouvée dans le Bottin.
     */
    const Bottin::Entree& Bottin::trouverAvecNomPrenom(const std::string& p_nom, const std::string& p_prenom) const {
        PRECONDITION(!p_nom.empty());
        PRECONDITION(!p_prenom.empty());
        std::string key = p_nom + p_prenom;

        if (!m_tableParNomPrenom.contient(key)) {
            throw ContratException(__FILE__, __LINE__, "p_telephoneFixe n'existe pas dans m_tableParNomPrenom",
                                   "Entree introuvable avec le Nom et prenom donne.");
        }

        size_t index = m_tableParNomPrenom.element(key);
        return m_tableauDesEntrees[index];
    }

    /**
     * \brief Recherche une entrée dans le Bottin à partir du numéro de téléphone.
     * \param[in] p_telephone Le numéro de téléphone à rechercher.
     * \return L'entrée correspondant au numéro de téléphone spécifié.
     * \pre Le numéro de téléphone ne doit pas être vide.
     * \throws ContratException Si l'entrée n'est pas trouvée dans le Bottin.
     */
    const Bottin::Entree& Bottin::trouverAvecTelephone(const std::string& p_telephone) const {
        PRECONDITION(!p_telephone.empty());

        if (!m_tableParTelephone.contient(p_telephone)) {
            throw ContratException(__FILE__, __LINE__, "p_telephoneFixe n'existe pas dans m_tableParTelephone",
                                   "Entree introuvable avec le numero de telephone donne.");
        }

        size_t index = m_tableParTelephone.element(p_telephone);
        return m_tableauDesEntrees[index];
    }

    /**
     * \brief Affiche toutes les entrées du Bottin.
     * \param[in] p_out Le flux de sortie où afficher les entrées du Bottin.
     */
    void Bottin::afficherBottin(std::ostream& p_out) const {
        for (const auto& entry : m_tableauDesEntrees) {
            p_out << entry << std::endl;
        }
    }

    /**
     * \brief Retourne le nombre d'entrées dans le Bottin
     * \return Le nombre d'entrées dans le Bottin.
     */
    int Bottin::nombreEntrees() const {
        return m_nombreEntrees;
    }

    /**
     * \brief Calcule le ratio de collisions dans la table des noms et prénoms.
     * \return Le ratio de collisions dans la table des noms et prénoms.
     */
    double Bottin::ratioDeCollisionsNomPrenom() const {
        double nbRatio;
        int nbCollisions;
        int maximumCollisionUneInsertion;
        m_tableParNomPrenom.statistiques(nbRatio, nbCollisions, maximumCollisionUneInsertion);
        return nbRatio;
    }

    /**
     * \brief Calcule le ratio de collisions dans la table des numéros de téléphone.
     * \return Le ratio de collisions dans la table des numéros de téléphone.
     */
    double Bottin::ratioDeCollisionTelephone() const {
        double nbRatio;
        int nbCollisions;
        int maximumCollisionUneInsertion;
        m_tableParTelephone.statistiques(nbRatio, nbCollisions, maximumCollisionUneInsertion);
        return nbRatio;

    }

    /**
     * \brief Retourne le nombre maximum de collisions dans la table des noms et prénoms.
     * \return Le nombre maximum de collisions dans la table des noms et prénoms.
     */
    int Bottin::maximumNbCollisionNomPrenom() const {
        double nbRatio;
        int nbCollisions;
        int maximumCollisionUneInsertion;
        m_tableParNomPrenom.statistiques(nbRatio, nbCollisions, maximumCollisionUneInsertion);
        return maximumCollisionUneInsertion;
    }

    /**
     * \brief Retourne le nombre maximum de collisions dans la table des numéros de téléphone.
     * \return Le nombre maximum de collisions dans la table des numéros de téléphone.
     */
    int Bottin::maximumNbCollisionTelephone() const {
        double nbRatio;
        int nbCollisions;
        int maximumCollisionUneInsertion;
        m_tableParTelephone.statistiques(nbRatio, nbCollisions, maximumCollisionUneInsertion);
        return maximumCollisionUneInsertion;
    }
}