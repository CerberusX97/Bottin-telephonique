/**
 * \file   ContratException.h contenant la déclaration de la classe ContratException et de ses héritiers
 * \brief  Hiérarchie de classe pour la gestion des erreurs par la théorie du contrat
 *
 * Ces classes constituent la hiérarchie pour la gestion de la
 * théorie du contrat.  Elle maintient les données nécessaires à
 * la sauvegarde des renseignements de l'erreur.  Cette classe et sa
 * hiérarchie sont intéressantes lors de l'utilisation des exceptions.
 *
 * En release, définir la constante NDEBUG pour désactiver les Macros
 *
 * <pre>
 * Classes:
 * std::logic_error        Classe de base des exceptions logiques
 * ContratException:       Classe de base des exceptions de contrat.
 * AssertionException:     Classe de gestion des erreurs d'assertion.
 * PreconditionException:  Classe de gestion des erreurs de précondition.
 * PostconditionException: Classe de gestion des erreurs de postcondition.
 * InvariantException:     Classe de gestion des erreurs d'invariant.

 * Attributs:
 *      std::string  m_expression: Le test logique qui a échoué.
 *      std::string  m_fichier:    Le nom du fichier.
 *      unsigned int m_ligne:      Le numéro de ligne
 * </pre>
 *
 * \note  Cette hiérarchie de classe est inspiré du livre de Horstmann:
 * Mastering object oriented design in C++, 1995, Ref:12-2007257.
 * Voir la classe ChiError fournie avec le livre.
 *
 * \author YR, TE
 * \version V3.0 révisée balises Doxygen C++ normes
 */

#ifndef CONTRATEXCEPTION_H_DEJA_INCLU
#define CONTRATEXCEPTION_H_DEJA_INCLU

#include <string>
#include <stdexcept>

/**
 * \class ContratException
 * \brief Classe de base des exceptions de contrat.
 */
class ContratException : public std::logic_error
{
public:
  ContratException (const std::string& p_fichier, unsigned int p_ligne,
                    const std::string& p_expression, const std::string& p_message);

  ~ContratException () throw () { }
  ;
  std::string reqTexteException () const;

private:
  std::string m_expression;
  std::string m_fichier;
  unsigned int m_ligne;
};

/**
 * \class AssertionException
 * \brief Classe pour la gestion des erreurs d'assertion.
 */

class AssertionException : public ContratException
{
public:
  AssertionException (const std::string& p_fichier, unsigned int p_ligne,
                      const std::string& p_expression);
};

/**
 * \class PreconditionException
 * \brief Classe pour la gestion des erreurs de précondition.
 */

class PreconditionException : public ContratException
{
public:
  PreconditionException (const std::string& p_fichier, unsigned int p_ligne,
                         const std::string& p_expression);
};

/**
 * \class PostconditionException
 * \brief Classe pour la gestion des erreurs de postcondition.
 */
class PostconditionException : public ContratException
{
public:
  PostconditionException (const std::string& p_fichier, unsigned int p_ligne,
                          const std::string& p_expression);
};

/**
 * \class InvariantException
 * \brief Classe pour la gestion des erreurs d'invariant.
 */
class InvariantException : public ContratException
{
public:
  InvariantException (const std::string& p_fichier, unsigned int p_ligne,
                      const std::string& p_expression);
};


// --- Définition des macros de contrôle de la théorie du contrat

#if !defined(NDEBUG)
// --- Mode debug

#define INVARIANTS() \
      verifieInvariant()

#define ASSERTION(f)     \
      if (!(f)) throw AssertionException(__FILE__,__LINE__, #f);
#define PRECONDITION(f)  \
      if (!(f)) throw PreconditionException(__FILE__, __LINE__, #f);
#define POSTCONDITION(f) \
      if (!(f)) throw PostconditionException(__FILE__, __LINE__, #f);
#define INVARIANT(f)   \
      if (!(f)) throw InvariantException(__FILE__,__LINE__, #f);

// --- LE MODE RELEASE
#else

#define PRECONDITION(f);
#define POSTCONDITION(f);
#define INVARIANTS();
#define INVARIANT(f);
#define ASSERTION(f);

#endif  // --- if !defined (NDEBUG)
#endif  // --- ifndef CONTRATEXCEPTION_H_DEJA_INCLU

