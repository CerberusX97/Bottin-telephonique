/**
 * \file ContratException.cpp
 * \brief Implantation de la classe ContratException et de ses héritiers
 * \author YR, TE
 * \version v3.0 révisée balises Doxygen C++ normes
 */
#include "ContratException.h"
#include <sstream>

using namespace std;


/**
 * \brief Constructeur de la classe de base ContratException
 * \param p_fichier chaîne de caractères représentant le fichier source dans lequel a eu lieu l'erreur
 * \param p_ligne un entier représentant la ligne où a eu lieu l'erreur
 * \param p_message Message décrivant l'erreur
 * \param p_expression Test logique qui a échoué
 */
ContratException::ContratException (const std::string& p_fichier, unsigned int p_ligne,
                                    const std::string& p_expression, const std::string& p_message) :
logic_error (p_message), m_expression (p_expression), m_fichier (p_fichier), m_ligne (p_ligne) { }


/**
 * \brief Construit le texte complet relié à l'exception de contrat
 * \return une chaîne de caractères correspondant à l'exception
 */
std::string ContratException::reqTexteException () const
{
  // ---  Prépare le message
  ostringstream os;
  os << "Message : " << what () << endl;
  os << "Fichier : " << m_fichier << endl;
  os << "Ligne   : " << m_ligne << endl;
  os << "Test    : " << m_expression << endl;

  return os.str ();
}


/**
 * \brief Constructeur de la classe AssertionException \n
 *    	Le constructeur public AssertionException(...)initialise
 *     	sa classe de base ContratException. On n'a pas d'attribut local. Cette
 *     	classe est intéressante pour son TYPE lors du traitement des exceptions.
 * \param p_fichier chaîne de caractères représentant le fichier source dans lequel a eu lieu l'erreur
 * \param p_ligne un entier représentant la ligne où a eu lieu l'erreur
 * \param p_expression Test logique qui a échoué
 *
 */

AssertionException::AssertionException (const std::string& p_fichier, unsigned int p_ligne,
                                        const std::string& p_expression)
: ContratException (p_fichier, p_ligne, p_expression, "ERREUR D'ASSERTION") { }


/**
 * \brief Constructeur de la classe PreconditionException en initialisant la classe de base ContratException.
 * 		 La classe représente l'erreur de précondition dans la théorie du contrat.
 * \param p_fichier chaîne de caractères représentant le fichier source dans lequel a eu lieu l'erreur
 * \param p_ligne un entier représentant la ligne où a eu lieu l'erreur
 * \param p_expression Test logique qui a échoué
 */
PreconditionException::PreconditionException (const std::string& p_fichier, unsigned int p_ligne,
                                              const std::string& p_expression)
: ContratException (p_fichier, p_ligne, p_expression, "ERREUR DE PRECONDITION") { }


/**
 * \brief Constructeur de la classe PostconditionException en initialisant la classe de base ContratException.
 *        La classe représente des erreurs de postcondition dans la théorie du contrat.
 * \param p_fichier chaîne de caractères représentant le fichier source dans lequel a eu lieu l'erreur
 * \param p_ligne un entier représentant la ligne où a eu lieu l'erreur
 * \param p_expression Test logique qui a échoué
 */
PostconditionException::PostconditionException (const std::string& p_fichier, unsigned int p_ligne,
                                                const std::string& p_expression)
: ContratException (p_fichier, p_ligne, p_expression, "ERREUR DE POSTCONDITION") { }


/**
 * \brief Constructeur de la classe InvariantException en initialisant la classe de base ContratException.
 * La classe représente des erreurs d'invariant dans la théorie du contrat.
 * \param p_fichier chaîne de caractères représentant le fichier source dans lequel a eu lieu l'erreur
 * \param p_ligne un entier représentant la ligne où a eu lieu l'erreur
 * \param p_expression Test logique qui a échoué
 */
InvariantException::InvariantException (const std::string& p_fichier, unsigned int p_ligne,
                                        const std::string& p_expression)
: ContratException (p_fichier, p_ligne, p_expression, "ERREUR D'INVARIANT") { }
