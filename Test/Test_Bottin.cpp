//
// Created by Vincent on 11/28/2024.
//

#include "gtest/gtest.h"
#include "Bottin.h"
#include <fstream>

/**
 * \class BottinTest
 * \brief Classe de test pour la classe Bottin.
 * Cette classe utilise Google Test pour tester la classe Bottin.
 */
class BottinTest : public ::testing::Test {
    public:
    TP3::Bottin bottin;

    /**
     * \brief Préparation avant chaque test.
     * \details Charge un fichier contenant des entrées pour initialiser l'objet bottin.
     * \pre Le fichier doit exister et être correctement formaté.
     */
    void SetUp() override {
        std::ifstream fichier("../../Bottin.txt");

        int size = 0;
        fichier >> size;

        bottin = TP3::Bottin(fichier, size);

        fichier.close();
    }
};

/**
 * \brief Test du constructeur de la classe Bottin.
 * \details Ce test vérifie si le constructeur initialise correctement l'objet Bottin avec le nombre d'entrées attendu.
 * \post Le nombre d'entrées du bottin doit être de 1438.
 */
TEST_F(BottinTest, ConstructeurInitialise) {
    EXPECT_EQ(bottin.nombreEntrees(), 1438);
}

/**
 * \brief Test de la méthode ajouter().
 * \details Ce test vérifie que l'ajout d'une nouvelle entrée fonctionne correctement, et que l'entrée peut être trouvée.
 * \pre L'entrée à ajouter ne doit pas exister déjà.
 */
TEST_F(BottinTest, AjouterValide) {
    bottin.ajouter("Doe", "John", "1234567890", "0987654321", "john.doe@example.com");

    EXPECT_NO_THROW(bottin.trouverAvecNomPrenom("Doe", "John"));
    EXPECT_NO_THROW(bottin.trouverAvecTelephone("1234567890"));

    const auto& entree = bottin.trouverAvecNomPrenom("Doe", "John");
    EXPECT_EQ(entree.m_nom, "Doe");
    EXPECT_EQ(entree.m_prenom, "John");
    EXPECT_EQ(entree.m_telephoneFixe, "1234567890");
    EXPECT_EQ(entree.m_cellulaire, "0987654321");
    EXPECT_EQ(entree.m_courriel, "john.doe@example.com");
}

/**
 * \brief Test de la méthode trouverAvecNomPrenom().
 * \details Ce test vérifie que l'on peut trouver une entrée en fonction du nom et du prénom.
 * \post L'entrée correspondant au nom et prénom doit avoir le courriel attendu.
 */
TEST_F(BottinTest, trouverAvecNomPrenom) {
    EXPECT_EQ(bottin.trouverAvecNomPrenom("Abplanalp", "Hans").m_courriel, "habplanalp@ucdavis.edu");
}

/**
 * \brief Test de la méthode trouverAvecTelephone().
 * \details Ce test vérifie que l'on peut trouver une entrée en fonction du numéro de téléphone.
 * \post L'entrée correspondant au numéro de téléphone doit être correcte.
 */
TEST_F(BottinTest, TrouverAvecTelephoneFindsCorrectEntry) {
    const auto& entree = bottin.trouverAvecTelephone("5367521366");

    EXPECT_EQ(entree.m_nom, "Abplanalp");
    EXPECT_EQ(entree.m_prenom, "Hans");
    EXPECT_EQ(entree.m_telephoneFixe, "5367521366");
}

/**
 * \brief Test de la méthode afficherBottin().
 * \details Ce test vérifie que l'affichage du bottin est correctement formaté.
 * \post Le texte généré doit contenir certaines informations d'entrée.
 */
TEST_F(BottinTest, AfficherBottinOutputsCorrectFormat) {
    std::ostringstream oss;
    bottin.afficherBottin(oss);

    std::string output = oss.str();
    EXPECT_NE(output.find("Abplanalp, Hans"), std::string::npos);
    EXPECT_NE(output.find("5367521366"), std::string::npos);
    EXPECT_NE(output.find("habplanalp@ucdavis.edu"), std::string::npos);
}

/**
 * \brief Test des ratios de collisions.
 * \details Ce test vérifie que les ratios de collisions pour les tables par nom/prénom et par téléphone sont valides.
 * \post Les ratios doivent être supérieurs ou égaux à 0.
 */
TEST_F(BottinTest, RatioDeCollisions) {
    double ratioNomPrenom = bottin.ratioDeCollisionsNomPrenom();
    double ratioTelephone = bottin.ratioDeCollisionTelephone();

    EXPECT_GE(ratioNomPrenom, 0.0);
    EXPECT_GE(ratioTelephone, 0.0);
}

/**
* \brief Test du nombre maximum de collisions.
* \details Ce test vérifie que le nombre maximum de collisions est correct pour les tables par nom/prénom et par téléphone.
* \post Le nombre maximum de collisions pour le nom/prénom et le téléphone doit être respectivement de 9 et 11.
*/
TEST_F(BottinTest, MaximumNbCollision) {
    int maxCollisionsNomPrenom = bottin.maximumNbCollisionNomPrenom();
    int maxCollisionsTelephone = bottin.maximumNbCollisionTelephone();

    EXPECT_EQ(maxCollisionsNomPrenom, 9);
    EXPECT_EQ(maxCollisionsTelephone, 11);
}

/**
 * \brief Test de recherche d'une entrée avec un nom non existant.
 * \details Ce test vérifie que la recherche d'une entrée avec un nom et prénom qui n'existe pas génère une exception.
 * \post Une exception doit être levée si l'entrée est introuvable.
 */
TEST_F(BottinTest, RechercheNomNonExistant) {
    try {
        bottin.trouverAvecNomPrenom("NonExistent", "Name");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "Entree introuvable avec le Nom et prenom donne.");
    }
}

/**
 * \brief Test de recherche d'un numéro de téléphone non existant.
 * \details Ce test vérifie que la recherche d'une entrée avec un numéro de téléphone qui n'existe pas génère une exception.
 * \post Une exception doit être levée si l'entrée est introuvable.
 */
TEST_F(BottinTest, RechercheNumeroNonExistant) {
    try {
        bottin.trouverAvecTelephone("1234567890");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "Entree introuvable avec le numero de telephone donne.");
    }
}

/**
 * \brief Test de la précondition pour un nom vide.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un nom vide est passé à la méthode trouverAvecNomPrenom().
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionEmptyNom) {
    try {
        bottin.trouverAvecNomPrenom("", "John");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}

/**
 * \brief Test de la précondition pour un prénom vide.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un prénom vide est passé à la méthode trouverAvecNomPrenom().
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionEmptyPrenom) {
    try {
        bottin.trouverAvecNomPrenom("Doe", "");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}

/**
 * \brief Test de la précondition pour un numéro de téléphone vide.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un numéro de téléphone vide est passé à la méthode trouverAvecTelephone().
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionEmptyTelephone) {
    try {
        bottin.trouverAvecTelephone("");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}

/**
 * \brief Test de la précondition pour un nom vide lors de l'ajout.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un nom vide est passé à la méthode ajouter().
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionNomVide) {
    try {
        bottin.ajouter("", "John", "1234567890", "0987654321", "john.doe@example.com");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}

/**
 * \brief Test de la précondition pour un prénom vide lors de l'ajout.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un prénom vide est passé à la méthode ajouter().
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionPrenomVide) {
    try {
        bottin.ajouter("Doe", "", "1234567890", "0987654321", "john.doe@example.com");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}

/**
 * \brief Test de la précondition pour un numéro de téléphone fixe vide lors de l'ajout.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un numéro de téléphone fixe vide est passé à la méthode ajouter().
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionTelephoneFixeVide) {
    try {
        bottin.ajouter("Doe", "John", "", "0987654321", "john.doe@example.com");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}

/**
 * \brief Test de la précondition pour un numéro de téléphone cellulaire vide lors de l'ajout.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un numéro de téléphone cellulaire vide est passé à la méthode ajouter().
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionCellulaireVide) {
    try {
        bottin.ajouter("Doe", "John", "1234567890", "", "john.doe@example.com");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}

/**
 * \brief Test de la précondition pour un email vide lors de l'ajout.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un email vide est passé à la méthode ajouter().
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionEmailVide) {
    try {
        bottin.ajouter("Doe", "John", "1234567890", "0987654321", "");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}

/**
 * \brief Test de la précondition pour un numéro de téléphone fixe invalide lors de l'ajout.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un numéro de téléphone fixe invalide est passé à la méthode ajouter().
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionTailleTelephoneFixeInvalide) {
    try {
        bottin.ajouter("Doe", "John", "123456789", "0987654321", "john.doe@example.com");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}

/**
 * \brief Test de la précondition pour un numéro de téléphone cellulaire invalide lors de l'ajout.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un numéro de téléphone cellulaire invalide est passé à la méthode ajouter().
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionTailleCellulaireInvalide) {
    try {
        bottin.ajouter("Doe", "John", "1234567890", "98765432", "john.doe@example.com");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}

/**
 * \brief Test de la précondition pour des doublons de nom/prénom lors de l'ajout.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un doublon de nom et prénom est trouvé lors de l'ajout.
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionNomPrenomDoublon) {
    bottin.ajouter("Doe", "John", "1234567890", "0987654321", "john.doe@example.com");

    try {
        bottin.ajouter("Doe", "John", "1122334455", "5566778899", "john.duplicate@example.com");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}

/**
 * \brief Test de la précondition pour des doublons de numéro de téléphone lors de l'ajout.
 * \details Ce test vérifie qu'une exception est levée lorsqu'un doublon de numéro de téléphone est trouvé lors de l'ajout.
 * \post Une exception de précondition doit être levée.
 */
TEST_F(BottinTest, TestPreconditionTelephoneDoublon) {
    bottin.ajouter("Doe", "John", "1234567890", "0987654321", "john.doe@example.com");

    try {
        bottin.ajouter("Do", "John", "1234567890", "5566778899", "john.duplicate@example.com");
        FAIL() << "Pas d'erreur";
    } catch (const std::exception& e) {
        EXPECT_EQ(std::string(e.what()), "ERREUR DE PRECONDITION");
    }
}